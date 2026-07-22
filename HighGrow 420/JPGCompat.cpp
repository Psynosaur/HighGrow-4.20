/* MFC-free implementation of the legacy JPGView interface. */
#include <windows.h>
#include <gdiplus.h>
#include "JPGView.h"

using namespace Gdiplus;

static ULONG_PTR gGdiPlusToken = 0;
static Image *gGrowroomImage = 0;
static IStream *gGrowroomStream = 0;
static Image *gRoomEditImage = 0;
static IStream *gRoomEditStream = 0;

static BOOL HGIStartGdiPlus(void)
    {
    GdiplusStartupInput input;
    if(gGdiPlusToken) return TRUE;
    return (GdiplusStartup(&gGdiPlusToken, &input, 0) == Ok);
    }

static void HGIFreeImage(Image **image, IStream **stream)
    {
    if(*image) { delete *image; *image = 0; }
    if(*stream) { (*stream)->Release(); *stream = 0; }
    }

static Image *HGILoadImageResource(HINSTANCE instance, UINT resource,
                                   IStream **stream)
    {
    HRSRC hResource;
    HGLOBAL hLoaded;
    HGLOBAL hMemory;
    void *data;
    DWORD size;
    Image *image;

    hResource = FindResource(instance, MAKEINTRESOURCE(resource), "IMAGE");
    if(!hResource) return 0;
    hLoaded = LoadResource(instance, hResource);
    size = SizeofResource(instance, hResource);
    data = LockResource(hLoaded);
    if(!data || !size) return 0;
    hMemory = GlobalAlloc(GMEM_MOVEABLE, size);
    if(!hMemory) return 0;
    CopyMemory(GlobalLock(hMemory), data, size);
    GlobalUnlock(hMemory);
    if(CreateStreamOnHGlobal(hMemory, TRUE, stream) != S_OK)
        { GlobalFree(hMemory); return 0; }
    image = new Image(*stream);
    if(image->GetLastStatus() != Ok)
        { delete image; (*stream)->Release(); *stream = 0; return 0; }
    return image;
    }

static Image *HGILoadImageFile(LPCTSTR fileName)
    {
#ifdef UNICODE
    return Image::FromFile(fileName, FALSE);
#else
    WCHAR wideName[MAX_PATH];
    if(!MultiByteToWideChar(CP_ACP, 0, fileName, -1, wideName, MAX_PATH)) return 0;
    return Image::FromFile(wideName, FALSE);
#endif
    }

static void HGIRenderImage(Image *image, HDC hdc, RECT rect)
    {
    Graphics graphics(hdc);
    graphics.SetInterpolationMode(InterpolationModeHighQualityBilinear);
    graphics.DrawImage(image, rect.left, rect.top,
                       rect.right - rect.left, rect.bottom - rect.top);
    }

BOOL HGIDrawResourceWindow(HWND hwnd, HINSTANCE instance, HDC hdc, UINT resource)
    {
    RECT rect;
    IStream *stream = 0;
    Image *image;
    if(!HGIStartGdiPlus()) return FALSE;
    image = HGILoadImageResource(instance, resource, &stream);
    if(!image) return FALSE;
    GetClientRect(hwnd, &rect);
    HGIRenderImage(image, hdc, rect);
    HGIFreeImage(&image, &stream);
    return TRUE;
    }

BOOL HGIDrawFileWindow(HWND hwnd, HDC hdc, LPCTSTR fileName)
    {
    RECT rect;
    Image *image;
    if(!HGIStartGdiPlus()) return FALSE;
    image = HGILoadImageFile(fileName);
    if(!image || image->GetLastStatus() != Ok)
        { if(image) delete image; return FALSE; }
    GetClientRect(hwnd, &rect);
    HGIRenderImage(image, hdc, rect);
    delete image;
    return TRUE;
    }

BOOL HGILoadGrowroomResourceImage(HINSTANCE instance, UINT resource)
    {
    if(gGrowroomImage || !HGIStartGdiPlus()) return FALSE;
    gGrowroomImage = HGILoadImageResource(instance, resource, &gGrowroomStream);
    return gGrowroomImage != 0;
    }

BOOL HGILoadGrowroomFileImage(LPCTSTR fileName)
    {
    if(gGrowroomImage || !HGIStartGdiPlus()) return FALSE;
    gGrowroomImage = HGILoadImageFile(fileName);
    if(!gGrowroomImage || gGrowroomImage->GetLastStatus() != Ok)
        { if(gGrowroomImage) delete gGrowroomImage; gGrowroomImage = 0; }
    return gGrowroomImage != 0;
    }

void HGIFreeGrowroomImage(void)
    { HGIFreeImage(&gGrowroomImage, &gGrowroomStream); }

void HGIRenderGrowroomImage(HDC hdc, RECT rect)
    { if(gGrowroomImage) HGIRenderImage(gGrowroomImage, hdc, rect); }

BOOL HGILoadRoomEditImage(HINSTANCE instance, UINT resource)
    {
    if(gRoomEditImage || !HGIStartGdiPlus()) return FALSE;
    gRoomEditImage = HGILoadImageResource(instance, resource, &gRoomEditStream);
    return gRoomEditImage != 0;
    }

void HGIFreeRoomEditImage(void)
    { HGIFreeImage(&gRoomEditImage, &gRoomEditStream); }

void HGIRenderRoomEditImage(HDC hdc, RECT rect)
    { if(gRoomEditImage) HGIRenderImage(gRoomEditImage, hdc, rect); }
