////////////////////////////////////////////////////////////////
// GDI+ port of Paul DiLascia's "Picture object" (MSDN, Oct 2001),
// which this game used via Picture.h/Picture.cpp (OLE IPicture).
//
// The original depended on MFC (CFile/CArchive/CDC/CRect/CSize) and
// ATL (CComQIPtr). Neither is available here, and the MinGW-w64
// headers ship a *different* "IPicture" (VB-era GUID 7BF80980-...)
// with an incompatible Render() layout, so OLE IPicture is out.
//
// GDI+ is the modern successor: same load-from-file /
// load-from-resource / stretch-render-to-DC behaviour. Both load
// paths funnel through a COM IStream over the raw bytes, because
// the MinGW GDI+ headers expose Image(IStream*,BOOL) but not the
// C++ Stream/MemoryStream wrappers MSVC has.
//
// Only the surface actually used by JPGView.cpp is kept:
//   Load from resource, Load from file, Render to HDC, Free,
//   GetImageSize.
////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <gdiplus.h>

class CPicture {
public:
    CPicture();
    ~CPicture();

    // Load from various sources
    BOOL Load(HINSTANCE hInst, UINT nIDRes);   // resource of type "IMAGE"
    BOOL Load(LPCTSTR lpFileName);             // from file

    // stretch-render to the given bounds on a device context
    // (defaults mirror the original: Render(&dc) / Render(&dc, &rc) / Render(&dc, &rc, prcMF))
    BOOL Render(HDC hdc, const RECT* prcBounds = NULL,
                const RECT* prcSrc = NULL,
                const RECT* prcMFBounds = NULL);

    // size of the picture in pixels
    SIZE GetImageSize(HDC hdc) const;

    void Free();

private:
    // build the image from raw bytes (shared by both load paths);
    // on success leaves m_pData set to the backing bytes
    BOOL LoadFromBytes(const void* pData, DWORD cbData);

    Gdiplus::Image* m_pImage;   // was: CComQIPtr<IPicture>
    HGLOBAL         m_pData;    // bytes backing the IStream (kept alive while image lives)
    HRESULT         m_hr;       // last error code
};
