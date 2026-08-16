////////////////////////////////////////////////////////////////
// GDI+ port of Paul DiLascia's CPicture (MSDN, Oct 2001).
// Same behaviour as the original IPicture-based wrapper:
//   load from resource / load from file / stretch-render to DC.
//
// Both load paths copy the bytes into a HGLOBAL and hand GDI+ a
// COM IStream over them (CreateStreamOnHGlobal) - the MinGW GDI+
// headers expose Image(IStream*,BOOL) but not the MSVC-only C++
// Stream/MemoryStream wrappers. The bytes stay alive for the
// lifetime of the image (GDI+ may read the stream lazily).
////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Picture.h"
#include <gdiplus.h>
#include <string.h>
#include <ole2.h>      // CreateStreamOnHGlobal

using namespace Gdiplus;

static ULONG_PTR s_GdiToken = 0;

// --------------------------------------------------------------------
// One-time GDI+ initialization.
// --------------------------------------------------------------------
static void EnsureGdiPlusInitialized(void)
{
    if(!s_GdiToken)
    {
        GdiplusStartupInput gi;
        GdiplusStartup(&s_GdiToken, &gi, NULL);
    }
}

// --------------------------------------------------------------------
// Build the image from raw bytes (shared by both load paths).
// Leaves m_pData set to the backing bytes on success.
// --------------------------------------------------------------------
BOOL CPicture::LoadFromBytes(const void* pData, DWORD cbData)
{
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cbData);
    if(!hGlobal)
        return FALSE;

    memcpy(GlobalLock(hGlobal), pData, cbData);
    GlobalUnlock(hGlobal);

    IStream* pStream = NULL;
    BOOL ok = FALSE;

    if(SUCCEEDED(CreateStreamOnHGlobal(hGlobal, FALSE, &pStream)))
    {
        m_pImage = new Image(pStream, FALSE);
        if(m_pImage && m_pImage->GetLastStatus() == Ok)
            ok = TRUE;
        pStream->Release();
    }

    if(ok)
    {
        m_pData = hGlobal;    // keep the backing bytes alive
        m_hr = S_OK;
    }
    else
    {
        delete m_pImage;
        m_pImage = NULL;
        GlobalFree(hGlobal);
        m_hr = E_FAIL;
    }

    return ok;
}

// --------------------------------------------------------------------
CPicture::CPicture()
{
    m_pImage = NULL;
    m_pData  = NULL;
    m_hr     = E_FAIL;
}

CPicture::~CPicture()
{
    Free();
}

// --------------------------------------------------------------------
// Load from resource (type "IMAGE" - same as the original).
// --------------------------------------------------------------------
BOOL CPicture::Load(HINSTANCE hInst, UINT nIDRes)
{
    Free();
    EnsureGdiPlusInitialized();

    HRSRC       hRsrc  = FindResource(hInst, MAKEINTRESOURCE(nIDRes), "IMAGE");
    DWORD       dwSize = 0;
    const BYTE* lpRsrc = NULL;

    if(!hRsrc)
        return FALSE;

    dwSize = SizeofResource(hInst, hRsrc);
    lpRsrc = (const BYTE*)LoadResource(hInst, hRsrc);

    BOOL bRet = (lpRsrc && dwSize > 0) ? LoadFromBytes(lpRsrc, dwSize) : FALSE;

    FreeResource(hRsrc);
    return bRet;
}

// --------------------------------------------------------------------
// Load from file.
// --------------------------------------------------------------------
BOOL CPicture::Load(LPCTSTR lpFileName)
{
    Free();
    EnsureGdiPlusInitialized();

    HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, 0, NULL);
    if(hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    DWORD dwSize = GetFileSize(hFile, NULL);
    BOOL  bRet   = FALSE;

    if(dwSize > 0 && dwSize != INVALID_FILE_SIZE)
    {
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
        if(hGlobal)
        {
            LPVOID lpBuf  = GlobalLock(hGlobal);
            DWORD  cbRead = 0;
            BOOL   bRead  = ReadFile(hFile, lpBuf, dwSize, &cbRead, NULL);
            GlobalUnlock(hGlobal);

            if(bRead && cbRead == dwSize)
                bRet = LoadFromBytes(lpBuf, dwSize);
            // LoadFromBytes copied the bytes; free this buffer in
            // either case (it allocates its own backing copy).
            if(!bRet)
                GlobalFree(hGlobal);
        }
    }

    CloseHandle(hFile);
    return bRet;
}

// --------------------------------------------------------------------
// Stretch-render to a device context (matches the old
// IPicture::Render(hdc, prcBounds, NULL, NULL) behaviour).
// --------------------------------------------------------------------
BOOL CPicture::Render(HDC hdc, const RECT* prcBounds,
                      const RECT* prcSrc,
                      const RECT* prcMFBounds)
{
    if(!m_pImage)
        return FALSE;

    REAL fx = 0, fy = 0, fw = 1, fh = 1;

    if(prcBounds)
    {
        fx = (REAL)prcBounds->left;
        fy = (REAL)prcBounds->top;
        fw = (REAL)(prcBounds->right - prcBounds->left);
        fh = (REAL)(prcBounds->bottom - prcBounds->top);
    }

    Graphics g(hdc);
    Status st = g.DrawImage(m_pImage, fx, fy, fw, fh);
    m_hr = (st == Ok) ? S_OK : E_FAIL;
    return (st == Ok);
}

// --------------------------------------------------------------------
// Picture size in pixels (GDI+ reports pixels directly - the
// original converted from himetric units).
// --------------------------------------------------------------------
SIZE CPicture::GetImageSize(HDC hdc) const
{
    SIZE sz = {0, 0};
    (void)hdc;

    if(!m_pImage)
        return sz;

    // (m_hr updated via const_cast - same trick as the original)
    const_cast<CPicture*>(this)->m_hr =
        (m_pImage->GetLastStatus() == Ok) ? S_OK : E_FAIL;
    sz.cx = (LONG)m_pImage->GetWidth();
    sz.cy = (LONG)m_pImage->GetHeight();
    return sz;
}

// --------------------------------------------------------------------
void CPicture::Free()
{
    if(m_pImage)
    {
        delete m_pImage;
        m_pImage = NULL;
    }
    if(m_pData)
    {
        GlobalFree(m_pData);
        m_pData = NULL;
    }
    m_hr = E_FAIL;
}
