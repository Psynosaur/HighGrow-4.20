/******************************************************************************\
*
*  DIBITMAP.H - Managing 256 colour bitmaps
*
\******************************************************************************/

HBITMAP DILoadResourceBitmap(HINSTANCE hInstance, LPSTR lpString, HPALETTE FAR* lphPalette);
void DIDrawBitmap(HDC hdc, int iX, int iY, HBITMAP hbmp, HPALETTE hpal, DWORD rop);
void DIStretchBitmap(HDC hdc, int iX, int iY, int percent, HBITMAP hbmp, HPALETTE hpal);
void DIDrawImage(HDC hdc, int image, int iX, int iY, HBITMAP hbmp, HPALETTE hpal);
void DIDrawImageEx(HDC hdc, int image, int imwidth, int iX, int iY, HBITMAP hbmp, HPALETTE hpal);
void DITileBitmap(HDC hdc, RECT rc, HBITMAP hBitmap, HPALETTE hPalette);
void DIFreeBitmap(HBITMAP hbmp, HPALETTE hpal);

void DILoadDlgLeafBitmap(HINSTANCE hInst);
void DIFreeDlgLeafBitmap(void);
BOOL DITileDlgLeafBitmap(HWND hDlg, HDC hDC);
BOOL DIRectTileDlgLeafBitmap(HDC hDC, RECT rc);

BOOL DILoadAlphaBlendLibrary(void);
void DIFreeAlphaBlendLibrary(void);
void DITileBlendBitmap(HDC hdc, RECT rc, HBITMAP hBlendImage, int iBlendValue);
void DIAlphaBlendBitmap(HDC hdc, HBITMAP hBlendImage, int ix, int iy, int iBlendValue);
BOOL DIAlphaBlendEllipse(HDC hdc, RECT rc, HBITMAP hBlendBmp, int iBlendValue);
BOOL DIAlphaBlendOffsetRect(HDC hdc, RECT rc, HBITMAP hBlendBmp, int iBlendValue, int iXofs, int iYofs);

BOOL DIShadeEllipse(HDC hDC, LPRECT lpRect);
BOOL DIShadeRoundRect(HDC hDC, LPRECT lpRect);

BOOL DIShadeRect(HDC hDC, LPRECT lpRect, COLORREF crShadeColor);

void DIDrawTransparentSection(HDC hdc, HBITMAP hBitmap, HPALETTE hPalette,
                                    int xSStart, int ySStart, int iWidth, int iHeight,
                                    int xDStart, int yDStart, COLORREF cTransparentColor);

void DIDrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, HPALETTE hPalette,
                             short xStart, short yStart, COLORREF cTransparentColor);

BOOL DIB_DrawWindowDIB(HWND hwnd, HANDLE hdib);
