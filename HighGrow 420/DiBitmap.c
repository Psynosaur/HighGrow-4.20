/******************************************************************************\
*
*  DIBITMAP.C - Managing 256 colour bitmaps
*
\******************************************************************************/
#include <windows.h>
#include "stdio.h"
#include "dibapi.h"

/******************************************************************************\
*  FUNCTION:    Loading and managing device independant bitmaps
\******************************************************************************/
 
HPALETTE DICreateDIBPalette(LPBITMAPINFO lpbmi, LPINT lpiNumColors)
    {
    LPBITMAPINFOHEADER  lpbi;
    LPLOGPALETTE     lpPal;
    HANDLE           hLogPal;
    HPALETTE         hPal = NULL;
    int              i;
 
    lpbi = (LPBITMAPINFOHEADER)lpbmi;
    if(lpbi->biBitCount <= 8)
        *lpiNumColors = (1 << lpbi->biBitCount);
    else
        *lpiNumColors = 0;  // No palette needed for 24 BPP DIB
 
    if(*lpiNumColors)
       {
       hLogPal = GlobalAlloc (GHND, sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*(*lpiNumColors));
       lpPal = (LPLOGPALETTE) GlobalLock (hLogPal);
       lpPal->palVersion    = 0x300;
       lpPal->palNumEntries = *lpiNumColors;
 
       for(i = 0;  i < *lpiNumColors;  i++)
          {
          lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
          lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
          lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
          lpPal->palPalEntry[i].peFlags = 0;
          }
       hPal = CreatePalette (lpPal);
       GlobalUnlock (hLogPal);
       GlobalFree   (hLogPal);
       }
    return hPal;
    }


HBITMAP DILoadResourceBitmap(HINSTANCE hInstance, LPSTR lpString, HPALETTE FAR* lphPalette)
    {
    HRSRC  hRsrc;
    HGLOBAL hGlobal;
    HBITMAP hBitmapFinal = NULL;
    LPBITMAPINFOHEADER  lpbi;
    HDC hdc;
    int iNumColors;
 
    if(hRsrc = FindResource(hInstance, lpString, RT_BITMAP))
       {
       hGlobal = LoadResource(hInstance, hRsrc);
       lpbi = (LPBITMAPINFOHEADER)LockResource(hGlobal);
       hdc  = GetDC(NULL);

       *lphPalette = DICreateDIBPalette ((LPBITMAPINFO)lpbi, &iNumColors);
       if(*lphPalette)
          {
          SelectPalette(hdc,*lphPalette,FALSE);
          RealizePalette(hdc);
          }
 
       hBitmapFinal = CreateDIBitmap(hdc, (LPBITMAPINFOHEADER)lpbi,(LONG)CBM_INIT,
                                    (LPSTR)lpbi + lpbi->biSize + iNumColors*sizeof(RGBQUAD),
                                    (LPBITMAPINFO)lpbi, DIB_RGB_COLORS );
 
       ReleaseDC(NULL,hdc);
       UnlockResource(hGlobal);
       FreeResource(hGlobal);
       }
    return (hBitmapFinal);
    }

 
/******************************************************************************\
*  FUNCTION: API for drawing device independant bitmaps
\******************************************************************************/

void DIDrawBitmap(HDC hdc, int iX, int iY, HBITMAP hbmp, HPALETTE hpal, DWORD rop)
    { // draws the given bitmap at the given x and y position in the DC
    HDC      hMemDC;
    HBITMAP  hPrevBmp;
    HPALETTE hPrevPal;
    BITMAP   bm;

    GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
    hMemDC   = CreateCompatibleDC(hdc);
    SelectPalette(hMemDC,hpal,FALSE);
    RealizePalette(hMemDC);
    hPrevPal = SelectPalette(hdc,hpal,FALSE);
    RealizePalette(hdc);
    hPrevBmp = SelectObject(hMemDC,hbmp);
    // now blt the entire bitmap
    BitBlt(hdc,iX,iY,bm.bmWidth,bm.bmHeight,hMemDC,0,0,rop);
    SelectPalette(hdc,hPrevPal,FALSE);
    SelectObject(hMemDC,hPrevBmp);
    DeleteDC(hMemDC);
    }


void DIStretchBitmap(HDC hdc, int iX, int iY, int percent, HBITMAP hbmp, HPALETTE hpal)
    { // draws the given bitmap at the given x and y position in the DC
    HDC      hMemDC;
    HBITMAP  hPrevBmp;
    HPALETTE hPrevPal;
    BITMAP   bm;
    int      idw, idh;

    GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
    hMemDC   = CreateCompatibleDC(hdc);
    SelectPalette(hMemDC,hpal,FALSE);
    RealizePalette(hMemDC);
    hPrevPal = SelectPalette(hdc,hpal,FALSE);
    RealizePalette(hdc);
    hPrevBmp = SelectObject(hMemDC,hbmp);
    // calc the destination width & height
    idw = (bm.bmWidth*percent)/100;
    idh = (bm.bmHeight*percent)/100;
    // now stretch the entire bitmap
//    BitBlt(hdc,iX,iY,bm.bmWidth,bm.bmHeight,hMemDC,0,0,rop);
    StretchBlt(hdc,iX,iY,idw,idh, hMemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
    SelectPalette(hdc,hPrevPal,FALSE);
    SelectObject(hMemDC,hPrevBmp);
    DeleteDC(hMemDC);
    }


void DIDrawImage(HDC hdc, int image, int iX, int iY, HBITMAP hbmp, HPALETTE hpal)
    { // draws the given image number within a bitmap of rectangular images
    // at the given x and y position in the DC
    HDC      hMemDC;
    HBITMAP  hPrevBmp;
    HPALETTE hPrevPal;
    BITMAP   bm;

    GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
    hMemDC   = CreateCompatibleDC(hdc);
    SelectPalette(hMemDC,hpal,FALSE);
    RealizePalette(hMemDC);
    hPrevPal = SelectPalette(hdc,hpal,FALSE);
    RealizePalette(hdc);
    hPrevBmp = SelectObject(hMemDC,hbmp);
    // now blt the correct portion of our bitmap
    BitBlt(hdc,iX,iY,bm.bmHeight,bm.bmHeight,hMemDC,image*bm.bmHeight,0,SRCCOPY);
    SelectPalette(hdc,hPrevPal,FALSE);
    SelectObject(hMemDC,hPrevBmp);
    DeleteDC(hMemDC);
    }


void DIDrawImageEx(HDC hdc, int image, int imwidth, int iX, int iY, HBITMAP hbmp, HPALETTE hpal)
    { // draws the given image number within a bitmap of rectangular images
    // at the given x and y position in the DC
    HDC      hMemDC;
    HBITMAP  hPrevBmp;
    HPALETTE hPrevPal;
    BITMAP   bm;

    GetObject(hbmp, sizeof(BITMAP), (LPSTR)&bm);
    hMemDC   = CreateCompatibleDC(hdc);
    SelectPalette(hMemDC,hpal,FALSE);
    RealizePalette(hMemDC);
    hPrevPal = SelectPalette(hdc,hpal,FALSE);
    RealizePalette(hdc);
    hPrevBmp = SelectObject(hMemDC,hbmp);
    // now blt the correct portion of our bitmap
    BitBlt(hdc,iX,iY,imwidth,bm.bmHeight,hMemDC,image*imwidth,0,SRCCOPY);
    SelectPalette(hdc,hPrevPal,FALSE);
    SelectObject(hMemDC,hPrevBmp);
    DeleteDC(hMemDC);
    }



void DITileBitmap(HDC hdc, RECT rc, HBITMAP hBitmap, HPALETTE hPalette)
    {
    BITMAP  bm;
    HDC     hMemDC;
    HANDLE  hPrevBmp;
    HPALETTE hPrevPal;
    POINT   ptStart;
     
    GetObject(hBitmap, sizeof(BITMAP), (void FAR*) &bm);
    hMemDC = CreateCompatibleDC(hdc);
    SelectPalette(hMemDC,hPalette,FALSE);
    RealizePalette(hMemDC);
    hPrevPal = SelectPalette(hdc,hPalette,FALSE);
    RealizePalette(hdc);
    hPrevBmp = SelectObject(hMemDC,hBitmap);

    ptStart.x=rc.left;
    ptStart.y=rc.top;
    while(ptStart.y < rc.bottom)
     {   while(ptStart.x < rc.right)
         {   BitBlt(hdc, ptStart.x, ptStart.y, bm.bmWidth, bm.bmHeight,
                    hMemDC, 0, 0, SRCCOPY);
             ptStart.x += bm.bmWidth;
         }   ptStart.x = rc.left;
         ptStart.y += bm.bmHeight;
     }  
    SelectPalette(hdc,hPrevPal,FALSE);
    SelectObject(hMemDC,hPrevBmp);
    DeleteDC(hMemDC);
    }


void DIFreeBitmap(HBITMAP hbmp, HPALETTE hpal)
    {
    if(hbmp) DeleteObject(hbmp);
    if(hpal) DeleteObject(hpal);
    }


/******************************************************************************\
*  FUNCTIONS: To Load, Free & Tile a lightened 256 colour leaf image in dialogs
\******************************************************************************/

HBITMAP  hDlgBkgndBmp = NULL;
HPALETTE hDlgBkgndPal = NULL;

void DILoadDlgLeafBitmap(HINSTANCE hInst)
    {
    hDlgBkgndBmp = DILoadResourceBitmap(hInst, "dlgleaves", &hDlgBkgndPal);
    }

void DIFreeDlgLeafBitmap(void)
    {
    if(hDlgBkgndBmp)
        DIFreeBitmap(hDlgBkgndBmp, hDlgBkgndPal);
    }


BOOL DIRectTileDlgLeafBitmap(HDC hDC, RECT rc)
    {
    HRGN hRegion = 0;  // Clip rect region
    BOOL bReturn = FALSE;
    // ----------
    if(hDlgBkgndBmp == 0) return FALSE;
    // --
    hRegion = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
    if(hRegion)
        {
        SelectClipRgn(hDC, hRegion);
        DITileBitmap(hDC, rc, hDlgBkgndBmp, hDlgBkgndPal);
        SelectClipRgn(hDC, NULL);
        DeleteObject(hRegion);
        bReturn = TRUE;
        }
    return bReturn;
    }


BOOL DITileDlgLeafBitmap(HWND hDlg, HDC hDC)
    {
    RECT rc;
    GetClientRect(hDlg, &rc);
    return DIRectTileDlgLeafBitmap(hDC, rc);
    }


/******************************************************************************\
*  FUNCTION: API for SHADING a rectangle region in a DC
\******************************************************************************/

BOOL DIShadeRect(HDC hDC, LPRECT lpRect, COLORREF crShadeColor) 
   { 
   COLORREF  crOldBkColor, crOldTextColor;
   HBRUSH    hBrush, hOldBrush;
   HBITMAP   hBitmap, hBrushBitmap, hOldMemBitmap;
   int       crOldBkMode, nWidth, nHeight;
   HDC       hMemDC;
   RECT      rcRect = { 0, 0, 0, 0};
   // The bitmap bits are for a monochrome "every-other-pixel"
   //     bitmap (for a pattern brush)
   WORD      Bits[8] = { 0x0055, 0x00aa, 0x0055, 0x00aa,
                         0x0055, 0x00aa, 0x0055, 0x00aa };

// WORD      Bits[8] = { 0x0077, 0x00FF, 0x00DD, 0x00FF, 
//                       0x0077, 0x00FF, 0x00DD, 0x00FF }; 

   // The Width and Height of the target area
   nWidth = lpRect->right - lpRect->left + 1;
   nHeight = lpRect->bottom - lpRect->top + 1;

   // Need a pattern bitmap
   hBrushBitmap = CreateBitmap( 8, 8, 1, 1, &Bits );
   // Need to store the original image
   hBitmap = CreateCompatibleBitmap( hDC, nWidth, nHeight );
   // Need a memory DC to work in
   hMemDC = CreateCompatibleDC( hDC );
   // Create the pattern brush
   hBrush = CreatePatternBrush( hBrushBitmap );

   // Has anything failed so far? If so, abort!
   if( (hBrushBitmap==NULL) || (hBitmap==NULL) ||
       (hMemDC==NULL) || (hBrush==NULL) )
      {
      if( hBrushBitmap != NULL ) DeleteObject(hBrushBitmap);
      if( hBitmap != NULL ) DeleteObject( hBitmap );
      if( hMemDC != NULL ) DeleteDC( hMemDC );
      if( hBrush != NULL ) DeleteObject( hBrush );
      return FALSE;
      }

   // Select the bitmap into the memory DC
   hOldMemBitmap = SelectObject( hMemDC, hBitmap );

   // How wide/tall is the original?
   rcRect.right = nWidth;
   rcRect.bottom = nHeight;

   // Lay down the pattern in the memory DC
   FillRect( hMemDC, &rcRect, hBrush );

   // Fill in the non-color pixels with the original image
   BitBlt( hMemDC, 0, 0, nWidth, nHeight, hDC,
   lpRect->left, lpRect->top, SRCAND );

   // Set the color scheme
   crOldTextColor = SetTextColor( hDC, crShadeColor );
   crOldBkColor = SetBkColor( hDC, RGB(0,0,0) );
   crOldBkMode = SetBkMode( hDC, OPAQUE );

   // Select the pattern brush
   hOldBrush = SelectObject( hDC, hBrush );
   // Fill in the color pixels, and set the others to black
   FillRect( hDC, lpRect, hBrush );
   // Fill in the black ones with the original image
   BitBlt( hDC, lpRect->left, lpRect->top, nWidth, nHeight,
           hMemDC, 0, 0, SRCPAINT );

   // Restore target DC settings
   SetBkMode( hDC, crOldBkMode );
   SetBkColor( hDC, crOldBkColor );
   SetTextColor( hDC, crOldTextColor );

   // Clean up
   SelectObject( hMemDC, hOldMemBitmap );
   DeleteObject( hBitmap );
   DeleteDC( hMemDC );
   DeleteObject( hBrushBitmap );
   SelectObject( hDC, hOldBrush );
   DeleteObject( hBrush );

   return TRUE;
   } 


/******************************************************************************\
*  FUNCTION: API for SHADING an elliptical region in a DC
\******************************************************************************/


BOOL DIShadeRegion(HDC hDC, LPRECT lpRect, HRGN hrgn) 
    {
    COLORREF  crOldBkColor, crOldTextColor;
    HBRUSH    hBrush, hOldBrush;
    HBITMAP   hBitmap, hBrushBitmap, hOldMemBitmap;
    int       crOldBkMode, nWidth, nHeight;
    HDC       hMemDC;
    RECT      rcRect = { 0, 0, 0, 0};
    COLORREF  crShade = RGB(0,0,0);
    // The bitmap bits are for a monochrome "every-other-pixel"
    //     bitmap (for a pattern brush)
    WORD      Bits[8] = { 0x0055, 0x00aa, 0x0055, 0x00aa,
                          0x0055, 0x00aa, 0x0055, 0x00aa }; 

    // The Width and Height of the target area
    nWidth = lpRect->right - lpRect->left + 1;
    nHeight = lpRect->bottom - lpRect->top + 1;

    // Need a pattern bitmap
    hBrushBitmap = CreateBitmap( 8, 8, 1, 1, &Bits );
    // Need to store the original image
    hBitmap = CreateCompatibleBitmap( hDC, nWidth, nHeight );
    // Need a memory DC to work in
    hMemDC = CreateCompatibleDC( hDC );
    // Create the pattern brush
    hBrush = CreatePatternBrush( hBrushBitmap );

    // Has anything failed so far? If so, abort!
    if( (hBrushBitmap==NULL) || (hBitmap==NULL) ||
        (hMemDC==NULL) || (hBrush==NULL) )
        {
       if( hBrushBitmap != NULL ) DeleteObject(hBrushBitmap);
       if( hBitmap != NULL ) DeleteObject( hBitmap );
       if( hMemDC != NULL ) DeleteDC( hMemDC );
       if( hBrush != NULL ) DeleteObject( hBrush );
       return FALSE;
        }

    // Select the bitmap into the memory DC
    hOldMemBitmap = SelectObject( hMemDC, hBitmap );
 
    // How wide/tall is the original?
    rcRect.right = nWidth;
    rcRect.bottom = nHeight;

    // Lay down the pattern in the memory DC
    FillRect( hMemDC, &rcRect, hBrush );

    // Fill in the non-color pixels with the original image
    BitBlt( hMemDC, 0, 0, nWidth, nHeight, hDC,
    lpRect->left, lpRect->top, SRCAND );

    // Set the color scheme
    crOldTextColor = SetTextColor( hDC, crShade);
    crOldBkColor = SetBkColor( hDC, crShade);
    crOldBkMode = SetBkMode( hDC, OPAQUE );

    // Select the pattern brush
    hOldBrush = SelectObject( hDC, hBrush );
    // Fill in the color pixels, and set the others to black
    FillRgn( hDC, hrgn, hBrush );
    // Fill in the black ones with the original image
    BitBlt( hDC, lpRect->left, lpRect->top, nWidth, nHeight,
            hMemDC, 0, 0, SRCPAINT );

    // Restore target DC settings
    SetBkMode( hDC, crOldBkMode );
    SetBkColor( hDC, crOldBkColor );
    SetTextColor( hDC, crOldTextColor );

    // Clean up
    SelectObject(hMemDC, hOldMemBitmap);
    SelectObject(hDC, hOldBrush);
    // delete objects
    if(hBitmap)      DeleteObject(hBitmap);
    if(hMemDC)       DeleteDC(hMemDC);
    if(hBrushBitmap) DeleteObject(hBrushBitmap);
    if(hBrush)       DeleteObject(hBrush);
    return TRUE;
    }


BOOL DIShadeEllipse(HDC hDC, LPRECT lpRect) 
    {
    HRGN hrgn=0;
    // create the elliptic region
    hrgn = CreateEllipticRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);  
    if(hrgn) 
        {
        // select the clipping region
        SelectClipRgn(hDC, hrgn); 
        // and shade the region
        DIShadeRegion(hDC, lpRect, hrgn);
        DeleteObject(hrgn);
        return TRUE;
        }
    return FALSE;
    }

BOOL DIShadeRoundRect(HDC hDC, LPRECT lpRect) 
    {
    HRGN hrgn=0;
    // create the rounded rectangular region
    hrgn = CreateRoundRectRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom, 20, 20);  
    if(hrgn) 
        {
        // select the clipping region
        SelectClipRgn(hDC, hrgn); 
        // and shade the region
        DIShadeRegion(hDC, lpRect, hrgn);
        DeleteObject(hrgn);
        return TRUE;
        }
    return FALSE;
    }


/******************************************************************************\
*  FUNCTIONS: To check if we can do alpha-blending AND doing the blending
\******************************************************************************/

// *******************************************
// in DIBITMAP.C
// *******************************************

HINSTANCE  hBlendLibInst;
FARPROC    DIAlphaBlendFunction;

BOOL DILoadAlphaBlendLibrary(void)
      { // loading the DLL that we'll use for alpha-blending the star image
      hBlendLibInst = LoadLibrary("msimg32.dll");
      if(hBlendLibInst)
           {
           DIAlphaBlendFunction = GetProcAddress(hBlendLibInst, "AlphaBlend");
           if(DIAlphaBlendFunction) 
               return TRUE;
	     }
	return FALSE;
	}


void DIFreeAlphaBlendLibrary(void)
	{ // freeing the alpha-blending library
      if(hBlendLibInst)
          {
          FreeLibrary(hBlendLibInst);
          hBlendLibInst        = 0;
	      DIAlphaBlendFunction = 0;
 	    }
	}


void DIAlphaBlendBitmap(HDC hdc, HBITMAP hBlendImage, int ix, int iy, int iBlendValue)
    {
    BITMAP        bm;
    HBITMAP       hbmp;
    HDC           hsdc;
    BLENDFUNCTION bf;

    if((hBlendLibInst!=0)&&(DIAlphaBlendFunction!=0))
        {
        // first setup the blendfunction structure
        bf.BlendOp             = AC_SRC_OVER;
        bf.BlendFlags          = 0;
        bf.AlphaFormat         = 0;
        bf.SourceConstantAlpha = iBlendValue;
        // now setup the source and destination DCs
        hsdc = CreateCompatibleDC(hdc);
        hbmp = SelectObject(hsdc,hBlendImage);
        GetObject(hBlendImage, sizeof(BITMAP), (LPSTR)&bm);
        // now do the alphablending
        DIAlphaBlendFunction(hdc, ix, iy, bm.bmWidth, bm.bmHeight, hsdc, 0, 0, bm.bmWidth, bm.bmHeight, bf);
        // now do some cleaning up
        SelectObject(hsdc,hbmp);
        DeleteDC(hsdc);
        }
    }

void DITileBlendBitmap(HDC hdc, RECT rc, HBITMAP hBlendImage, int iBlendValue)
    {
    POINT         ptStart;
    BITMAP        bm;
    HBITMAP       hbmp;
    HDC           hsdc;
    BLENDFUNCTION bf;

    if((hBlendLibInst!=0)&&(DIAlphaBlendFunction!=0))
        {
        // first setup the blendfunction structure
        bf.BlendOp             = AC_SRC_OVER;
        bf.BlendFlags          = 0;
        bf.AlphaFormat         = 0;
        bf.SourceConstantAlpha = iBlendValue;
        // now setup the source and destination DCs
        hsdc = CreateCompatibleDC(hdc);
        hbmp = SelectObject(hsdc,hBlendImage);
        GetObject(hBlendImage, sizeof(BITMAP), (LPSTR)&bm);
        // set the start x and y position
        ptStart.x = rc.left;
        ptStart.y = rc.top;
        DPtoLP(hdc, &ptStart, 1);
        while(ptStart.y < (rc.bottom-rc.top))
              {   
              while(ptStart.x < (rc.right-rc.left))
                  {   
                  DIAlphaBlendFunction(hdc, ptStart.x, ptStart.y, bm.bmWidth, bm.bmHeight, 
                                       hsdc, 0, 0, bm.bmWidth, bm.bmHeight, bf);
                  ptStart.x = ptStart.x + bm.bmWidth;
                  }   
              ptStart.x = rc.left;
              ptStart.y = ptStart.y + bm.bmHeight;
              } 
        // now do some cleaning up
        SelectObject(hsdc,hbmp);
        DeleteDC(hsdc);
        }
    }


BOOL DIAlphaBlendEllipse(HDC hdc, RECT rc, HBITMAP hBlendBmp, int iBlendValue)
    { // used to draw the pot shadows if alpha-blending is possible
    HRGN hrgn = 0;
    // create the elliptic region
    hrgn = CreateEllipticRgn(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top); 
    if(hrgn!=0)
        {
        SelectClipRgn(hdc, hrgn); 
        DITileBlendBitmap(hdc, rc, hBlendBmp, iBlendValue);
        SelectClipRgn(hdc, NULL); 
        DeleteObject(hrgn);
        return TRUE;
        }
    return FALSE;
    }


BOOL DIAlphaBlendOffsetRect(HDC hdc, RECT rc, HBITMAP hBlendBmp, int iBlendValue, int iXofs, int iYofs)
    { // used to draw the pot shadows if alpha-blending is possible
    HRGN hrgn = 0;
    POINT pt[10];
    // ----------
    // fill the points for this polygon
    pt[0].x = rc.left;
    pt[0].y = rc.top+iYofs;
    pt[1].x = rc.right-rc.left-iXofs;
    pt[1].y = rc.top;
    pt[2].x = rc.right-rc.left;
    pt[2].y = rc.bottom-rc.top-iYofs;
    pt[3].x = rc.left+iXofs;
    pt[3].y = rc.bottom-rc.top;
    pt[4].x = rc.left;
    pt[4].y = rc.top+iYofs;
    // create the offset rectangular region
    hrgn = CreatePolygonRgn((POINT *)&pt, 5, WINDING); 
    if(hrgn!=0)
        {
        SelectClipRgn(hdc, hrgn); 
        DITileBlendBitmap(hdc, rc, hBlendBmp, iBlendValue);
        SelectClipRgn(hdc, NULL); 
        DeleteObject(hrgn);
        return TRUE;
        }
    return FALSE;
    }


/******************************************************************************\
*  FUNCTION: API for drawing a section of a 256 colour transparent bitmap
\******************************************************************************/

void DIDrawTransparentSection(HDC hdc, HBITMAP hBitmap, HPALETTE hPalette,
                                    int xSStart, int ySStart, int iWidth, int iHeight,
                                    int xDStart, int yDStart, COLORREF cTransparentColor)
   {
   COLORREF   cColor;
   HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
   HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
   HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
   HPALETTE   hOldPal1, hOldPal2, hOldPal3, hOldPal4, hOldPal5, hOldPal6;

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap

   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcObject = CreateCompatibleDC(hdc);
   hdcMem    = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);

   if(hPalette)
      {
      hOldPal1 = SelectPalette(hdc, hPalette, TRUE);
      hOldPal2 = SelectPalette(hdcBack, hPalette, TRUE);
      hOldPal3 = SelectPalette(hdcObject, hPalette, TRUE);
      hOldPal4 = SelectPalette(hdcMem, hPalette, TRUE);
      hOldPal5 = SelectPalette(hdcSave, hPalette, TRUE);
      hOldPal6 = SelectPalette(hdcTemp, hPalette, TRUE);
      RealizePalette(hdc);
      }

   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.

   // Monochrome DC
   bmAndBack   = CreateBitmap(iWidth, iHeight, 1, 1, NULL);

   // Monochrome DC
   bmAndObject = CreateBitmap(iWidth, iHeight, 1, 1, NULL);

   bmAndMem    = CreateCompatibleBitmap(hdc, iWidth, iHeight);
   bmSave      = CreateCompatibleBitmap(hdc, iWidth, iHeight);

   // Each DC must select a bitmap object to store pixel data.
   bmBackOld   = SelectObject(hdcBack, bmAndBack);
   bmObjectOld = SelectObject(hdcObject, bmAndObject);
   bmMemOld    = SelectObject(hdcMem, bmAndMem);
   bmSaveOld   = SelectObject(hdcSave, bmSave);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));

   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, iWidth, iHeight, hdcTemp, xSStart, ySStart, SRCCOPY);

   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparentColor);

   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcObject, 0, 0, iWidth, iHeight, hdcTemp, xSStart, ySStart, SRCCOPY);

   // Set the background color of the source DC back to the original
   // color.
   SetBkColor(hdcTemp, cColor);

   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, iWidth, iHeight, hdcObject, 0, 0, NOTSRCCOPY);

   // Copy the background of the main DC to the destination.
   BitBlt(hdcMem, 0, 0, iWidth, iHeight, hdc, xDStart, yDStart, SRCCOPY);

   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcMem, 0, 0, iWidth, iHeight, hdcObject, 0, 0, SRCAND);

   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, xSStart, ySStart, iWidth, iHeight, hdcBack, 0, 0, SRCAND);

   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcMem, 0, 0, iWidth, iHeight, hdcTemp, xSStart, ySStart, SRCPAINT);

   // Copy the destination to the screen.
   BitBlt(hdc, xDStart, yDStart, iWidth, iHeight, hdcMem, 0, 0, SRCCOPY);

   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, xSStart, ySStart, iWidth, iHeight, hdcSave, 0, 0, SRCCOPY);

   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack, bmBackOld));
   DeleteObject(SelectObject(hdcObject, bmObjectOld));
   DeleteObject(SelectObject(hdcMem, bmMemOld));
   DeleteObject(SelectObject(hdcSave, bmSaveOld));

   if(hOldPal1)  SelectPalette (hdc, hOldPal1, FALSE);
   if(hOldPal2)  SelectPalette (hdcBack, hOldPal2, FALSE);
   if(hOldPal3)  SelectPalette (hdcObject, hOldPal3, FALSE);
   if(hOldPal4)  SelectPalette (hdcMem, hOldPal4, FALSE);
   if(hOldPal5)  SelectPalette (hdcSave, hOldPal5, FALSE);
   if(hOldPal6)  SelectPalette (hdcTemp, hOldPal6, FALSE);
   RealizePalette(hdc);

   // Delete the memory DCs.
   DeleteDC(hdcMem);
   DeleteDC(hdcBack);
   DeleteDC(hdcObject);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
   }

/******************************************************************************\
*  FUNCTION: API for drawing 256 colour transparent bitmaps
\******************************************************************************/


void DIDrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, HPALETTE hPalette,
                             short xStart, short yStart, COLORREF cTransparentColor)
   {
   BITMAP bm;
   POINT  pt;
   // -----------
   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   pt.x = bm.bmWidth;        // Get width of bitmap
   pt.y = bm.bmHeight;       // Get height of bitmap
   DPtoLP(hdc, &pt, 1);      // Convert from device
   // ---------------------
   DIDrawTransparentSection(hdc,hBitmap,hPalette,0,0,pt.x,pt.y,xStart,yStart,cTransparentColor);
   }


/******************************************************************************\
*  FUNCTION: NEW API for drawing 256 colour transparent bitmaps
\******************************************************************************/


BOOL DIB_DrawWindowDIB(HWND hwnd, HANDLE hdib)
	{
	HDC hdc;
    int iw,  ih;
    int ibw, ibh;
	RECT rcwin, rcpic;
    HPALETTE hpal;
    LPVOID lpv;
    HRGN hrgn, horgn;

	if(hdib)
		{
		hdc  = GetDC(hwnd);
        lpv  = GlobalLock(hdib);
        hpal = CreateDIBPalette(hdib);
        ibw  = DIBWidth(lpv);
        ibh  = DIBHeight(lpv);
        // set the picture's rect
        rcpic.left   = 0;
        rcpic.top    = 0;
        rcpic.right  = ibw;
        rcpic.bottom = ibh;
        // now calc the destination rect
        GetClientRect(hwnd, &rcwin);
        hrgn  = CreateRectRgn(rcwin.left, rcwin.top, rcwin.right, rcwin.bottom);
        horgn = SelectObject(hdc, hrgn);
        iw  = rcwin.right;
        ih  = rcwin.bottom;
        if((ibw/iw)<=(ibh/ih))
            {
            rcwin.bottom = (iw*ibh)/ibw;
            }
        else
            {
            rcwin.right  = (ih*ibw)/ibh;
            }
        // now we'll draw the DIB
        PaintDIB(hdc, &rcwin, hdib, &rcpic, hpal);
        // finally, we clean up and return a true
        GlobalUnlock(hdib);
        if(horgn) SelectObject(hdc, horgn);
        if(hrgn)  DeleteObject(hrgn);
        if(hpal)  DeleteObject(hpal);
        ReleaseDC(hwnd, hdc);
        return TRUE;
		}
    return FALSE;
	}

