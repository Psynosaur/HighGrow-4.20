// header files 
#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <direct.h>
#include "dibapi.h"
#include "dibitmap.h"  // for handling 256 color bitmaps
#include "global.h"

//**********************************************************************
//
//
//  SaveDIB()           - Saves the specified dib in a file
//  LoadDIB()           - Loads a DIB from a file
//  DestroyDIB()        - Deletes DIB when finished using it
//  CreateDIB()         - Creates new DIB
//  FindDIBBits()       - Sets pointer to the DIB bits
//  DIBWidth()          - Gets the width of the DIB
//  DIBHeight()         - Gets the height of the DIB
//  PaletteSize()       - Calculates the buffer size required by a palette
//  DIBNumColors()      - Calculates number of colors in the DIB's color table
//  CreateDIBPalette()  - Creates a palette from a DIB
//  DIBToBitmap()       - Creates a bitmap from a DIB
//  BitmapToDIB()       - Creates a DIB from a bitmap
//  PalEntriesOnDevice()- Gets the number of palette entries of a device
//  GetSystemPalette()  - Returns a handle to the current system palette
//  AllocRoomForDIB()   - Allocates memory for a DIB
//  ChangeDIBFormat()   - Changes a DIB's BPP and/or compression format
//  ChangeBitmapFormat()- Changes a bitmap to a DIB with specified BPP and
//                        compression format
//
//   You have a royalty-free right to use, modify, reproduce and
//   distribute the Sample Files (and/or any modified version) in
//   any way you find useful, provided that you agree that
//   Microsoft has no warranty obligations or liability for any
//   Sample Application Files which are modified.
//
//*******************************************************************



// Dib Header Marker - used in writing DIBs to files

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')




static char *szErrors[] =
{
   "Not a Windows DIB file!",
   "Couldn't allocate memory!",
   "Error reading file!",
   "Error locking memory!",
   "Error opening file!",
   "Error creating palette!",
   "Error getting a DC!",
   "Error creating Device Dependent Bitmap",
   "StretchBlt() failed!",
   "StretchDIBits() failed!",
   "SetDIBitsToDevice() failed!",
   "Printer: StartDoc failed!",
   "Printing: GetModuleHandle() couldn't find GDI!",
   "Printer: SetAbortProc failed!",
   "Printer: StartPage failed!",
   "Printer: NEWFRAME failed!",
   "Printer: EndPage failed!",
   "Printer: EndDoc failed!",
   "SetDIBits() failed!",
   "File Not Found!",
   "Invalid Handle",
   "General Error on call to DIB function"
};


void FAR DIBError(int ErrNo)
{
   if ((ErrNo < ERR_MIN) || (ErrNo >= ERR_MAX))
      MessageBox(NULL, "Undefined Error!", NULL, MB_OK | MB_ICONHAND);
   else
      MessageBox(NULL, szErrors[ErrNo], NULL, MB_OK | MB_ICONHAND);
}

/*************************************************************************
 *
 * CopyWindowToDIB()
 *
 * Parameters:
 *
 * HWND hWnd        - specifies the window
 *
 * WORD fPrintArea  - specifies the window area to copy into the device-
 *                    independent bitmap
 *
 * Return Value:
 *
 * HDIB             - identifies the device-independent bitmap
 *
 * Description:
 *
 * This function copies the specified part(s) of the window to a device-
 * independent bitmap.
 *
 ************************************************************************/


HDIB FAR CopyWindowToDIB(HWND hWnd, WORD fPrintArea)
{
   HDIB hDIB = NULL;  // handle to DIB

   // check for a valid window handle 

   if (!hWnd)
      return NULL;
   switch (fPrintArea)
      {
   case PW_WINDOW: // copy entire window
   {
      RECT rectWnd;

      // get the window rectangle   

      GetWindowRect(hWnd, &rectWnd);

      //  get the DIB of the window by calling
      //  CopyScreenToDIB and passing it the window rect
       
      hDIB = CopyScreenToDIB(&rectWnd);
   }
      break;

   case PW_CLIENT: // copy client area
   {
      RECT rectClient;
      POINT pt1, pt2;

      // get the client area dimensions 

      GetClientRect(hWnd, &rectClient);

      // convert client coords to screen coords 
      pt1.x = rectClient.left;
      pt1.y = rectClient.top;
      pt2.x = rectClient.right;
      pt2.y = rectClient.bottom;
      ClientToScreen(hWnd, &pt1);
      ClientToScreen(hWnd, &pt2);
      rectClient.left = pt1.x;
      rectClient.top = pt1.y;
      rectClient.right = pt2.x;
      rectClient.bottom = pt2.y;

      //  get the DIB of the client area by calling
      //  CopyScreenToDIB and passing it the client rect
       
      hDIB = CopyScreenToDIB(&rectClient);
   }
      break;

   default:    // invalid print area
      return NULL;
      }

   // return the handle to the DIB 
   return hDIB;
}


/*************************************************************************
 *
 * CopyScreenToDIB()
 *
 * Parameter:
 *
 * LPRECT lpRect    - specifies the window
 *
 * Return Value:
 *
 * HDIB             - identifies the device-independent bitmap
 *
 * Description:
 *
 * This function copies the specified part of the screen to a device-
 * independent bitmap.
 *
 *
 ************************************************************************/


HDIB FAR CopyScreenToDIB(LPRECT lpRect)
{
   HBITMAP hBitmap;    // handle to device-dependent bitmap
   HPALETTE hPalette;  // handle to palette
   HDIB hDIB = NULL;   // handle to DIB

   //  get the device-dependent bitmap in lpRect by calling
   //  CopyScreenToBitmap and passing it the rectangle to grab
    

   hBitmap = CopyScreenToBitmap(lpRect);

   // check for a valid bitmap handle 
   if (!hBitmap)
      return NULL;

   // get the current palette 
   hPalette = GetSystemPalette();

   // convert the bitmap to a DIB   
   hDIB = BitmapToDIB(hBitmap, hPalette);

   // clean up   
   DeleteObject(hPalette);
   DeleteObject(hBitmap);

   // return handle to the packed-DIB   
   return hDIB;
}


/*************************************************************************
 *
 * CopyWindowToBitmap()
 *
 * Parameters:
 *
 * HWND hWnd        - specifies the window
 *
 * WORD fPrintArea  - specifies the window area to copy into the device-
 *                    dependent bitmap
 *
 * Return Value:
 *
 * HDIB         - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function copies the specified part(s) of the window to a device-
 * dependent bitmap.
 *
 *
 ************************************************************************/


HBITMAP FAR CopyWindowToBitmap(HWND hWnd, WORD fPrintArea)
{
   HBITMAP hBitmap = NULL;  // handle to device-dependent bitmap

   // check for a valid window handle   

   if (!hWnd)
      return NULL;
   switch (fPrintArea)
      {
   case PW_WINDOW: // copy entire window
   {
      RECT rectWnd;

      // get the window rectangle   

      GetWindowRect(hWnd, &rectWnd);

      /*  get the bitmap of that window by calling
       *  CopyScreenToBitmap and passing it the window rect
       */
      hBitmap = CopyScreenToBitmap(&rectWnd);
   }
   break;

   case PW_CLIENT: // copy client area
   {
      RECT rectClient;
      POINT pt1, pt2;

      // get client dimensions   

      GetClientRect(hWnd, &rectClient);

      // convert client coords to screen coords   
      pt1.x = rectClient.left;
      pt1.y = rectClient.top;
      pt2.x = rectClient.right;
      pt2.y = rectClient.bottom;
      ClientToScreen(hWnd, &pt1);
      ClientToScreen(hWnd, &pt2);
      rectClient.left = pt1.x;
      rectClient.top = pt1.y;
      rectClient.right = pt2.x;
      rectClient.bottom = pt2.y;

      /*  get the bitmap of the client area by calling
       *  CopyScreenToBitmap and passing it the client rect
       */
      hBitmap = CopyScreenToBitmap(&rectClient);
   }
   break;

   default:    // invalid print area
      return NULL;
      }

   // return handle to the bitmap   
   return hBitmap;
}


/*************************************************************************
 *
 * CopyScreenToBitmap()
 *
 * Parameter:
 *
 * LPRECT lpRect    - specifies the window
 *
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function copies the specified part of the screen to a device-
 * dependent bitmap.
 *
 *
 ************************************************************************/


HBITMAP FAR CopyScreenToBitmap(LPRECT lpRect)
{
   HDC hScrDC, hMemDC;           // screen DC and memory DC
   HBITMAP hBitmap, hOldBitmap;  // handles to deice-dependent bitmaps
   int nX, nY, nX2, nY2;         // coordinates of rectangle to grab
   int nWidth, nHeight;          // DIB width and height
   int xScrn, yScrn;             // screen resolution

   // check for an empty rectangle   

   if (IsRectEmpty(lpRect))
      return NULL;

   /*  create a DC for the screen and create
    *  a memory DC compatible to screen DC
    */
   hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
   hMemDC = CreateCompatibleDC(hScrDC);

   // get points of rectangle to grab   
   nX = lpRect->left;
   nY = lpRect->top;
   nX2 = lpRect->right;
   nY2 = lpRect->bottom;

   // get screen resolution   
   xScrn = GetDeviceCaps(hScrDC, HORZRES);
   yScrn = GetDeviceCaps(hScrDC, VERTRES);

   // make sure bitmap rectangle is visible   
   if (nX < 0)
      nX = 0;
   if (nY < 0)
      nY = 0;
   if (nX2 > xScrn)
      nX2 = xScrn;
   if (nY2 > yScrn)
      nY2 = yScrn;
   nWidth = nX2 - nX;
   nHeight = nY2 - nY;

   // create a bitmap compatible with the screen DC   
   hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);

   // select new bitmap into memory DC   
   hOldBitmap = SelectObject(hMemDC, hBitmap);

   // bitblt screen DC to memory DC   
   BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);

   /*  select old bitmap back into memory DC and get handle to
    *  bitmap of the screen
    */
   hBitmap = SelectObject(hMemDC, hOldBitmap);

   // clean up   
   DeleteDC(hScrDC);
   DeleteDC(hMemDC);

   // return handle to the bitmap   
   return hBitmap;
}


/*************************************************************************
 *
 * PaintDIB()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * HDIB hDIB        - handle to global memory with a DIB spec
 *                    in it followed by the DIB bits
 *
 * LPRECT lpDIBRect - rectangle of DIB to output into lpDCRect
 *
 * Return Value:
 *
 * BOOL             - TRUE if DIB was drawn, FALSE otherwise
 *
 * Description:
 *   Painting routine for a DIB.  Calls StretchDIBits() or
 *   SetDIBitsToDevice() to paint the DIB.  The DIB is
 *   output to the specified DC, at the coordinates given
 *   in lpDCRect.  The area of the DIB to be output is
 *   given by lpDIBRect.
 *
 * NOTE: This function always selects the palette as background. Before
 * calling this function, be sure your palette is selected to desired
 * priority (foreground or background).
 *
 *
 ************************************************************************/

BOOL FAR PaintDIB(HDC      hDC,
                  LPRECT   lpDCRect,
                  HDIB     hDIB,
                  LPRECT   lpDIBRect,
                  HPALETTE hPal)
{
   LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
   LPSTR    lpDIBBits;           // Pointer to DIB bits
   BOOL     bSuccess=FALSE;      // Success/fail flag
   HPALETTE hOldPal=NULL;        // Previous palette

   // Check for valid DIB handle   
   if (!hDIB)
      return FALSE;

   /* Lock down the DIB, and get a pointer to the beginning of the bit
    *  buffer
    */
   lpDIBHdr  = GlobalLock(hDIB);
   lpDIBBits = FindDIBBits(lpDIBHdr);

   // Select and realize our palette as background   
   if (hPal)
   {
      hOldPal = SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
   }

   // Make sure to use the stretching mode best for color pictures   
   SetStretchBltMode(hDC, COLORONCOLOR);

   // Determine whether to call StretchDIBits() or SetDIBitsToDevice()   
   if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
       (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
      bSuccess = SetDIBitsToDevice(hDC,                        // hDC
                                   lpDCRect->left,             // DestX
                                   lpDCRect->top,              // DestY
                                   RECTWIDTH(lpDCRect),        // nDestWidth
                                   RECTHEIGHT(lpDCRect),       // nDestHeight
                                   lpDIBRect->left,            // SrcX
                                   (int)DIBHeight(lpDIBHdr) -
                                      lpDIBRect->top - 
                                      RECTHEIGHT(lpDIBRect),   // SrcY
                                   0,                          // nStartScan
                                   (WORD)DIBHeight(lpDIBHdr),  // nNumScans
                                   lpDIBBits,                  // lpBits
                                   (LPBITMAPINFO)lpDIBHdr,     // lpBitsInfo
                                   DIB_RGB_COLORS);            // wUsage
   else 
      bSuccess = StretchDIBits(hDC,                            // hDC
                               lpDCRect->left,                 // DestX
                               lpDCRect->top,                  // DestY
                               RECTWIDTH(lpDCRect),            // nDestWidth
                               RECTHEIGHT(lpDCRect),           // nDestHeight
                               lpDIBRect->left,                // SrcX
                               lpDIBRect->top,                 // SrcY
                               RECTWIDTH(lpDIBRect),           // wSrcWidth
                               RECTHEIGHT(lpDIBRect),          // wSrcHeight
                               lpDIBBits,                      // lpBits
                               (LPBITMAPINFO)lpDIBHdr,         // lpBitsInfo
                               DIB_RGB_COLORS,                 // wUsage
                               SRCCOPY);                       // dwROP

   // Unlock the memory block   
   GlobalUnlock(hDIB);

   // Reselect old palette   
   if (hOldPal)
     SelectPalette(hDC, hOldPal, FALSE);

   // Return with success/fail flag   
   return bSuccess;
}


/*************************************************************************
 *
 * PaintBitmap()
 *
 * Parameters:
 *
 * HDC hDC          - DC to do output to
 *
 * LPRECT lpDCRect  - rectangle on DC to do output to
 *
 * HBITMAP hDDB     - handle to device-dependent bitmap (DDB)
 *
 * LPRECT lpDDBRect - rectangle of DDB to output into lpDCRect
 *
 * HPALETTE hPalette - handle to the palette to use with hDDB
 *
 * Return Value:
 *
 * BOOL             - TRUE if bitmap was drawn, FLASE otherwise
 *
 * Description:
 *
 * Painting routine for a DDB.  Calls BitBlt() or
 * StretchBlt() to paint the DDB.  The DDB is
 * output to the specified DC, at the coordinates given
 * in lpDCRect.  The area of the DDB to be output is
 * given by lpDDBRect.  The specified palette is used.
 *
 * NOTE: This function always selects the palette as background. Before
 * calling this function, be sure your palette is selected to desired
 * priority (foreground or background).
 *
 *
 ************************************************************************/

BOOL FAR PaintBitmap(HDC      hDC,
                     LPRECT   lpDCRect, 
                     HBITMAP  hDDB, 
                     LPRECT   lpDDBRect, 
                     HPALETTE hPal)
{
   HDC      hMemDC;            // Handle to memory DC
   HBITMAP  hOldBitmap;        // Handle to previous bitmap
   HPALETTE hOldPal1 = NULL;   // Handle to previous palette
   HPALETTE hOldPal2 = NULL;   // Handle to previous palette
   BOOL     bSuccess = FALSE;  // Success/fail flag

   // Create a memory DC   
   hMemDC = CreateCompatibleDC (hDC);

   // If this failed, return FALSE   
   if (!hMemDC)
      return FALSE;

   // If we have a palette, select and realize it   
   if (hPal)
   {
      hOldPal1 = SelectPalette(hMemDC, hPal, TRUE);
      hOldPal2 = SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
   }

   // Select bitmap into the memory DC   
   hOldBitmap = SelectObject (hMemDC, hDDB);

   // Make sure to use the stretching mode best for color pictures      
   SetStretchBltMode (hDC, COLORONCOLOR);

   // Determine whether to call StretchBlt() or BitBlt()    
   if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDDBRect)) &&
       (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDDBRect)))
      bSuccess = BitBlt(hDC,
                        lpDCRect->left,
                        lpDCRect->top,
                        lpDCRect->right - lpDCRect->left,
                        lpDCRect->bottom - lpDCRect->top,
                        hMemDC,
                        lpDDBRect->left,
                        lpDDBRect->top,
                        SRCCOPY);
   else
      bSuccess = StretchBlt(hDC,
                            lpDCRect->left, 
                            lpDCRect->top, 
                            lpDCRect->right - lpDCRect->left,
                            lpDCRect->bottom - lpDCRect->top,
                            hMemDC,
                            lpDDBRect->left, 
                            lpDDBRect->top, 
                            lpDDBRect->right - lpDDBRect->left,
                            lpDDBRect->bottom - lpDDBRect->top,
                            SRCCOPY);

   // Clean up   
   SelectObject(hMemDC, hOldBitmap);

   if (hOldPal1)
      SelectPalette (hMemDC, hOldPal1, FALSE);

   if (hOldPal2)
      SelectPalette (hDC, hOldPal2, FALSE);

   DeleteDC (hMemDC);

   // Return with success/fail flag   
   return bSuccess;
}


/*************************************************************************
 *
 * CreateDIB()
 *
 * Parameters:
 *
 * DWORD dwWidth    - Width for new bitmap, in pixels
 * DWORD dwHeight   - Height for new bitmap 
 * WORD  wBitCount  - Bit Count for new DIB (1, 4, 8, or 24)
 *
 * Return Value:
 *
 * HDIB             - Handle to new DIB
 *
 * Description:
 *
 * This function allocates memory for and initializes a new DIB by
 * filling in the BITMAPINFOHEADER, allocating memory for the color
 * table, and allocating memory for the bitmap bits.  As with all
 * HDIBs, the header, colortable and bits are all in one contiguous
 * memory block.  This function is similar to the CreateBitmap() 
 * Windows API.
 *
 * The colortable and bitmap bits are left uninitialized (zeroed) in the
 * returned HDIB.
 *
 *
 ************************************************************************/

HDIB FAR CreateDIB(DWORD dwWidth, DWORD dwHeight, WORD wBitCount)
{
   BITMAPINFOHEADER bi;         // bitmap header
   LPBITMAPINFOHEADER lpbi;     // pointer to BITMAPINFOHEADER
   DWORD dwLen;                 // size of memory block
   HDIB hDIB;
   DWORD dwBytesPerLine;        // Number of bytes per scanline


   // Make sure bits per pixel is valid
   if (wBitCount <= 1)
      wBitCount = 1;
   else if (wBitCount <= 4)
      wBitCount = 4;
   else if (wBitCount <= 8)
      wBitCount = 8;
   else if (wBitCount <= 24)
      wBitCount = 24;
   else
      wBitCount = 4;  // set default value to 4 if parameter is bogus

   // initialize BITMAPINFOHEADER
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = dwWidth;         // fill in width from parameter
   bi.biHeight = dwHeight;       // fill in height from parameter
   bi.biPlanes = 1;              // must be 1
   bi.biBitCount = wBitCount;    // from parameter
   bi.biCompression = BI_RGB;    
   bi.biSizeImage = 0;           // 0's here mean "default"
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrUsed = 0;
   bi.biClrImportant = 0;

   // calculate size of memory block required to store the DIB.  This
   // block should be big enough to hold the BITMAPINFOHEADER, the color
   // table, and the bits

   dwBytesPerLine = WIDTHBYTES(wBitCount * dwWidth);
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + (dwBytesPerLine * dwHeight);

   // alloc memory block to store our bitmap
   hDIB = GlobalAlloc(GHND, dwLen);

   // major bummer if we couldn't get memory block
   if (!hDIB)
   {
      return NULL;
   }

   // lock memory and get pointer to it
   lpbi = (VOID FAR *)GlobalLock(hDIB);

   // use our bitmap info structure to fill in first part of
   // our DIB with the BITMAPINFOHEADER
   *lpbi = bi;

   // Since we don't know what the colortable and bits should contain,
   // just leave these blank.  Unlock the DIB and return the HDIB.

   GlobalUnlock(hDIB);

   // return handle to the DIB   
   return hDIB;
}



/*************************************************************************
 *
 * FindDIBBits()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * LPSTR            - pointer to the DIB bits
 *
 * Description:
 *
 * This function calculates the address of the DIB's bits and returns a
 * pointer to the DIB bits.
 *
 *
 ************************************************************************/


LPSTR FAR FindDIBBits(LPSTR lpDIB)
{
   return (lpDIB + *(LPDWORD)lpDIB + PaletteSize(lpDIB));
}


/*************************************************************************
 *
 * DIBWidth()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - width of the DIB
 *
 * Description:
 *
 * This function gets the width of the DIB from the BITMAPINFOHEADER
 * width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * width field if it is an OS/2-style DIB.
 *
 *
 ************************************************************************/


DWORD FAR DIBWidth(LPSTR lpDIB)
{
   LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER lpbmc;  // pointer to an OS/2-style DIB

   // point to the header (whether Win 3.0 and OS/2)   

   lpbmi = (LPBITMAPINFOHEADER)lpDIB;
   lpbmc = (LPBITMAPCOREHEADER)lpDIB;

   // return the DIB width if it is a Win 3.0 DIB   
   if (lpbmi->biSize == sizeof(BITMAPINFOHEADER))
      return lpbmi->biWidth;
   else  // it is an OS/2 DIB, so return its width   
      return (DWORD)lpbmc->bcWidth;
}


/*************************************************************************
 *
 * DIBHeight()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * DWORD            - height of the DIB
 *
 * Description:
 *
 * This function gets the height of the DIB from the BITMAPINFOHEADER
 * height field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
 * height field if it is an OS/2-style DIB.
 *
 *
 ************************************************************************/


DWORD FAR DIBHeight(LPSTR lpDIB)
{
   LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
   LPBITMAPCOREHEADER lpbmc;  // pointer to an OS/2-style DIB

   // point to the header (whether OS/2 or Win 3.0   

   lpbmi = (LPBITMAPINFOHEADER)lpDIB;
   lpbmc = (LPBITMAPCOREHEADER)lpDIB;

   // return the DIB height if it is a Win 3.0 DIB   
   if (lpbmi->biSize == sizeof(BITMAPINFOHEADER))
      return lpbmi->biHeight;
   else  // it is an OS/2 DIB, so return its height   
      return (DWORD)lpbmc->bcHeight;
}


/*************************************************************************
 *
 * PaletteSize()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - size of the color palette of the DIB
 *
 * Description:
 *
 * This function gets the size required to store the DIB's palette by
 * multiplying the number of colors by the size of an RGBQUAD (for a
 * Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an OS/2-
 * style DIB).
 *
 *
 ************************************************************************/


WORD FAR PaletteSize(LPSTR lpDIB)
{
   // calculate the size required by the palette   
   if (IS_WIN30_DIB (lpDIB))
      return (DIBNumColors(lpDIB) * sizeof(RGBQUAD));
   else
      return (DIBNumColors(lpDIB) * sizeof(RGBTRIPLE));
}


/*************************************************************************
 *
 * DIBNumColors()
 *
 * Parameter:
 *
 * LPSTR lpDIB      - pointer to packed-DIB memory block
 *
 * Return Value:
 *
 * WORD             - number of colors in the color table
 *
 * Description:
 *
 * This function calculates the number of colors in the DIB's color table
 * by finding the bits per pixel for the DIB (whether Win3.0 or OS/2-style
 * DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
 * if 24, no colors in color table.
 *
 *
 ************************************************************************/


WORD FAR DIBNumColors(LPSTR lpDIB)
{
   WORD wBitCount;  // DIB bit count

   /*  If this is a Windows-style DIB, the number of colors in the
    *  color table can be less than the number of bits per pixel
    *  allows for (i.e. lpbi->biClrUsed can be set to some value).
    *  If this is the case, return the appropriate value.
    */

   if (IS_WIN30_DIB(lpDIB))
   {
      DWORD dwClrUsed;

      dwClrUsed = ((LPBITMAPINFOHEADER)lpDIB)->biClrUsed;
      if (dwClrUsed)
     return (WORD)dwClrUsed;
   }

   /*  Calculate the number of colors in the color table based on
    *  the number of bits per pixel for the DIB.
    */
   if (IS_WIN30_DIB(lpDIB))
      wBitCount = ((LPBITMAPINFOHEADER)lpDIB)->biBitCount;
   else
      wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;

   // return number of colors based on bits per pixel   
   switch (wBitCount)
      {
   case 1:
      return 2;

   case 4:
      return 16;

   case 8:
      return 256;

   default:
      return 0;
      }
}


/*************************************************************************
 *
 * CreateDIBPalette()
 *
 * Parameter:
 *
 * HDIB hDIB        - specifies the DIB
 *
 * Return Value:
 *
 * HPALETTE         - specifies the palette
 *
 * Description:
 *
 * This function creates a palette from a DIB by allocating memory for the
 * logical palette, reading and storing the colors from the DIB's color table
 * into the logical palette, creating a palette from this logical palette,
 * and then returning the palette's handle. This allows the DIB to be
 * displayed using the best possible colors (important for DIBs with 256 or
 * more colors).
 *
 *
 ************************************************************************/


HPALETTE FAR CreateDIBPalette(HDIB hDIB)
{
   LPLOGPALETTE lpPal;      // pointer to a logical palette
   HANDLE hLogPal;          // handle to a logical palette
   HPALETTE hPal = NULL;    // handle to a palette
   int i, wNumColors;       // loop index, number of colors in color table
   LPSTR lpbi;              // pointer to packed-DIB
   LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
   LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (OS/2)
   BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB

   // if handle to DIB is invalid, return NULL   

   if (!hDIB)
      return NULL;

   // lock DIB memory block and get a pointer to it   
   lpbi = GlobalLock(hDIB);

   // get pointer to BITMAPINFO (Win 3.0)   
   lpbmi = (LPBITMAPINFO)lpbi;

   // get pointer to BITMAPCOREINFO (OS/2 1.x)   
   lpbmc = (LPBITMAPCOREINFO)lpbi;

   // get the number of colors in the DIB   
   wNumColors = DIBNumColors(lpbi);

   // is this a Win 3.0 DIB?   
   bWinStyleDIB = IS_WIN30_DIB(lpbi);
   if (wNumColors)
   {
      // allocate memory block for logical palette   
      hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) *
                wNumColors);

      // if not enough memory, clean up and return NULL   
      if (!hLogPal)
      {
     GlobalUnlock(hDIB);
     return NULL;
      }

      // lock memory block and get pointer to it   
      lpPal = (LPLOGPALETTE)GlobalLock(hLogPal);

      // set version and number of palette entries   
      lpPal->palVersion = PALVERSION;
      lpPal->palNumEntries = wNumColors;

      /*  store RGB triples (if Win 3.0 DIB) or RGB quads (if OS/2 DIB)
       *  into palette
       */ 
      for (i = 0; i < wNumColors; i++)
      {
     if (bWinStyleDIB)
     {
        lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
        lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
        lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
        lpPal->palPalEntry[i].peFlags = 0;
     }
     else
     {
        lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
        lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
        lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
        lpPal->palPalEntry[i].peFlags = 0;
     }
      }

      // create the palette and get handle to it   
      hPal = CreatePalette(lpPal);

      // if error getting handle to palette, clean up and return NULL   
      if (!hPal)
      {
     GlobalUnlock(hLogPal);
     GlobalFree(hLogPal);
     return NULL;
      }
   }

   // clean up   
   GlobalUnlock(hLogPal);
   GlobalFree(hLogPal);
   GlobalUnlock(hDIB);

   // return handle to DIB's palette   
   return hPal;
}


/*************************************************************************
 *
 * DIBToBitmap()
 *
 * Parameters:
 *
 * HDIB hDIB        - specifies the DIB to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *
 * Return Value:
 *
 * HBITMAP          - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a bitmap from a DIB using the specified palette.
 * If no palette is specified, default is used.
 *
 * NOTE:
 *
 * The bitmap returned from this funciton is always a bitmap compatible
 * with the screen (e.g. same bits/pixel and color planes) rather than
 * a bitmap with the same attributes as the DIB.  This behavior is by
 * design, and occurs because this function calls CreateDIBitmap to
 * do its work, and CreateDIBitmap always creates a bitmap compatible
 * with the hDC parameter passed in (because it in turn calls
 * CreateCompatibleBitmap).
 *
 * So for instance, if your DIB is a monochrome DIB and you call this
 * function, you will not get back a monochrome HBITMAP -- you will
 * get an HBITMAP compatible with the screen DC, but with only 2
 * colors used in the bitmap.
 *
 * If your application requires a monochrome HBITMAP returned for a
 * monochrome DIB, use the function SetDIBits().
 *
 * Also, the DIBpassed in to the function is not destroyed on exit. This
 * must be done later, once it is no longer needed.
 *
 *
 ************************************************************************/


HBITMAP FAR DIBToBitmap(HDIB hDIB, HPALETTE hPal)
{
   LPSTR lpDIBHdr, lpDIBBits;  // pointer to DIB header, pointer to DIB bits
   HBITMAP hBitmap;            // handle to device-dependent bitmap
   HDC hDC;                    // handle to DC
   HPALETTE hOldPal = NULL;    // handle to a palette

   // if invalid handle, return NULL   

   if (!hDIB)
      return NULL;

   // lock memory block and get a pointer to it   
   lpDIBHdr = GlobalLock(hDIB);

   // get a pointer to the DIB bits   
   lpDIBBits = FindDIBBits(lpDIBHdr);

   // get a DC   
   hDC = GetDC(NULL);
   if (!hDC)
   {
      // clean up and return NULL   
      GlobalUnlock(hDIB);
      return NULL;
   }

   // select and realize palette   
   if (hPal)
      hOldPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   // create bitmap from DIB info. and bits   
   hBitmap = CreateDIBitmap(hDC, (LPBITMAPINFOHEADER)lpDIBHdr, CBM_INIT,
                lpDIBBits, (LPBITMAPINFO)lpDIBHdr, DIB_RGB_COLORS);

   // restore previous palette   
   if (hOldPal)
      SelectPalette(hDC, hOldPal, FALSE);

   // clean up   
   ReleaseDC(NULL, hDC);
   GlobalUnlock(hDIB);

   // return handle to the bitmap   
   return hBitmap;
}


/*************************************************************************
 *
 * BitmapToDIB()
 *
 * Parameters:
 *
 * HBITMAP hBitmap  - specifies the bitmap to convert
 *
 * HPALETTE hPal    - specifies the palette to use with the bitmap
 *
 * Return Value:
 *
 * HDIB             - identifies the device-dependent bitmap
 *
 * Description:
 *
 * This function creates a DIB from a bitmap using the specified palette.
 *
 *
 ************************************************************************/


HDIB FAR BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
   BITMAP bm;                   // bitmap structure
   BITMAPINFOHEADER bi;         // bitmap header
   BITMAPINFOHEADER FAR *lpbi;  // pointer to BITMAPINFOHEADER
   DWORD dwLen;                 // size of memory block
   HANDLE hDIB, h;              // handle to DIB, temp handle
   HDC hDC;                     // handle to DC
   WORD biBits;                 // bits per pixel

   // check if bitmap handle is valid   

   if (!hBitmap)
      return NULL;

   // fill in BITMAP structure, return NULL if it didn't work   
   if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm))
      return NULL;

   // if no palette is specified, use default palette   
   if (hPal == NULL)
      hPal = GetStockObject(DEFAULT_PALETTE);

   // calculate bits per pixel   
   biBits = bm.bmPlanes * bm.bmBitsPixel;

   // make sure bits per pixel is valid   
   if (biBits <= 1)
      biBits = 1;
   else if (biBits <= 4)
      biBits = 4;
   else if (biBits <= 8)
      biBits = 8;
   else // if greater than 8-bit, force to 24-bit   
      biBits = 24;

   // initialize BITMAPINFOHEADER   
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = bm.bmWidth;
   bi.biHeight = bm.bmHeight;
   bi.biPlanes = 1;
   bi.biBitCount = biBits;
   bi.biCompression = BI_RGB;
   bi.biSizeImage = 0;
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrUsed = 0;
   bi.biClrImportant = 0;

   // calculate size of memory block required to store BITMAPINFO   
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi);

   // get a DC   
   hDC = GetDC(NULL);

   // select and realize our palette   
   hPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   // alloc memory block to store our bitmap   
   hDIB = GlobalAlloc(GHND, dwLen);

   // if we couldn't get memory block   
   if (!hDIB)
   {
      // clean up and return NULL   
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }

   // lock memory and get pointer to it   
   lpbi = (VOID FAR *)GlobalLock(hDIB);

   // use our bitmap info. to fill BITMAPINFOHEADER   
   *lpbi = bi;

   /*  call GetDIBits with a NULL lpBits param, so it will calculate the
    *  biSizeImage field for us
    */
   GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, NULL, (LPBITMAPINFO)lpbi,
         DIB_RGB_COLORS);

   // get the info. returned by GetDIBits and unlock memory block   
   bi = *lpbi;
   GlobalUnlock(hDIB);

   // if the driver did not fill in the biSizeImage field, make one up   
   if (bi.biSizeImage == 0)
      bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

   // realloc the buffer big enough to hold all the bits   
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage;
   if (h = GlobalReAlloc(hDIB, dwLen, 0))
      hDIB = h;
   else
   {
      // clean up and return NULL   
      GlobalFree(hDIB);
      hDIB = NULL;
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }

   // lock memory block and get pointer to it   
   lpbi = (VOID FAR *)GlobalLock(hDIB);

   /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
    *  bits this time
    */
   if (GetDIBits(hDC, hBitmap, 0, (WORD)bi.biHeight, (LPSTR)lpbi + (WORD)lpbi
         ->biSize + PaletteSize((LPSTR)lpbi), (LPBITMAPINFO)lpbi,
         DIB_RGB_COLORS) == 0)
   {
      // clean up and return NULL   
      GlobalUnlock(hDIB);
      hDIB = NULL;
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }
   bi = *lpbi;

   // clean up   
   GlobalUnlock(hDIB);
   SelectPalette(hDC, hPal, TRUE);
   RealizePalette(hDC);
   ReleaseDC(NULL, hDC);

   // return handle to the DIB   
   return hDIB;
}


/*************************************************************************
 *
 * PalEntriesOnDevice()
 *
 * Parameter:
 *
 * HDC hDC          - device context
 *
 * Return Value:
 *
 * int              - number of palette entries on device
 *
 * Description:
 *
 * This function gets the number of palette entries on the specified device
 *
 *
 ************************************************************************/


int FAR PalEntriesOnDevice(HDC hDC)
{
   int nColors;  // number of colors

   /*  Find out the number of palette entries on this
    *  device.
    */

   nColors = GetDeviceCaps(hDC, SIZEPALETTE);

   /*  For non-palette devices, we'll use the # of system
    *  colors for our palette size.
    */
   if (!nColors)
      nColors = GetDeviceCaps(hDC, NUMCOLORS);
   assert(nColors);
   return nColors;
}


/*************************************************************************
 *
 * GetSystemPalette()
 *
 * Parameters:
 *
 * None
 *
 * Return Value:
 *
 * HPALETTE         - handle to a copy of the current system palette
 *
 * Description:
 *
 * This function returns a handle to a palette which represents the system
 * palette.  The system RGB values are copied into our logical palette using
 * the GetSystemPaletteEntries function.  
 *
 * History:   
 *            
 *
 ************************************************************************/


HPALETTE FAR GetSystemPalette(void)
{
   HDC hDC;                // handle to a DC
   static HPALETTE hPal = NULL;   // handle to a palette
   HANDLE hLogPal;         // handle to a logical palette
   LPLOGPALETTE lpLogPal;  // pointer to a logical palette
   int nColors;            // number of colors

   // Find out how many palette entries we want.   

   hDC = GetDC(NULL);
   if (!hDC)
      return NULL;
   nColors = PalEntriesOnDevice(hDC);   // Number of palette entries

   // Allocate room for the palette and lock it.   
   hLogPal = GlobalAlloc(GHND, sizeof(LOGPALETTE) + nColors * sizeof(
             PALETTEENTRY));

   // if we didn't get a logical palette, return NULL   
   if (!hLogPal)
      return NULL;

   // get a pointer to the logical palette   
   lpLogPal = (LPLOGPALETTE)GlobalLock(hLogPal);

   // set some important fields   
   lpLogPal->palVersion = PALVERSION;
   lpLogPal->palNumEntries = nColors;

   // Copy the current system palette into our logical palette   

   GetSystemPaletteEntries(hDC, 0, nColors, 
                           (LPPALETTEENTRY)(lpLogPal->palPalEntry));

   /*  Go ahead and create the palette.  Once it's created,
    *  we no longer need the LOGPALETTE, so free it.
    */

   hPal = CreatePalette(lpLogPal);

   // clean up   
   GlobalUnlock(hLogPal);
   GlobalFree(hLogPal);
   ReleaseDC(NULL, hDC);

   return hPal;
}


/*************************************************************************
 *
 * AllocRoomForDIB()
 *
 * Parameters:
 *
 * BITMAPINFOHEADER - bitmap info header stucture
 *
 * HBITMAP          - handle to the bitmap
 *
 * Return Value:
 *
 * HDIB             - handle to memory block
 *
 * Description:
 *
 *  This routine takes a BITMAPINOHEADER, and returns a handle to global
 *  memory which can contain a DIB with that header.  It also initializes
 *  the header portion of the global memory.  GetDIBits() is used to determine
 *  the amount of room for the DIB's bits.  The total amount of memory
 *  needed = sizeof(BITMAPINFOHEADER) + size of color table + size of bits.
 *
 *
 ************************************************************************/

HANDLE AllocRoomForDIB(BITMAPINFOHEADER bi, HBITMAP hBitmap)
{
   DWORD              dwLen;
   HANDLE             hDIB;
   HDC                hDC;
   LPBITMAPINFOHEADER lpbi;
   HANDLE             hTemp;

   /* Figure out the size needed to hold the BITMAPINFO structure
    * (which includes the BITMAPINFOHEADER and the color table).
    */

   dwLen = bi.biSize + PaletteSize((LPSTR) &bi);
   hDIB  = GlobalAlloc(GHND,dwLen);

   // Check that DIB handle is valid   
   if (!hDIB)
      return NULL;

   /* Set up the BITMAPINFOHEADER in the newly allocated global memory,
    * then call GetDIBits() with lpBits = NULL to have it fill in the
    * biSizeImage field for us.
    */
   lpbi  = (VOID FAR *)GlobalLock(hDIB);
   *lpbi = bi;

   hDC   = GetDC(NULL);
   GetDIBits(hDC, hBitmap, 0, (WORD) bi.biHeight,
          NULL, (LPBITMAPINFO) lpbi, DIB_RGB_COLORS);
   ReleaseDC(NULL, hDC);

   /* If the driver did not fill in the biSizeImage field,
    * fill it in -- NOTE: this is a bug in the driver!
    */
   if (lpbi->biSizeImage == 0)
      lpbi->biSizeImage = WIDTHBYTES((DWORD)lpbi->biWidth * lpbi->biBitCount) *
              lpbi->biHeight;

   // Get the size of the memory block we need   
   dwLen = lpbi->biSize + PaletteSize((LPSTR) &bi) + lpbi->biSizeImage;

   // Unlock the memory block   
   GlobalUnlock(hDIB);

   // ReAlloc the buffer big enough to hold all the bits   
   if (hTemp = GlobalReAlloc(hDIB,dwLen,0))
      return hTemp;
   else
      {
      // Else free memory block and return failure   
      GlobalFree(hDIB);
      return NULL;
      }
}


/*************************************************************************
 *
 * DestroyDIB ()
 *
 * Purpose:  Frees memory associated with a DIB
 *
 * Returns:  Nothing
 *
 *
 *************************************************************************/


WORD FAR DestroyDIB(HDIB hDib)
{
   GlobalFree(hDib);
   return 0;
}

/*********************************************************************
 *
 * Local Function Prototypes
 *
 *********************************************************************/


//HANDLE ReadDIBFile(int);
//BOOL DIBRead(int, LPSTR, DWORD);
//BOOL SaveDIBFile(void);
//BOOL WriteDIB(LPSTR, HANDLE);
//DWORD PASCAL DIBWrite(int, VOID FAR *, DWORD);


//************************************************************************
//************************************************************************
//   READING A FILE INTO A DIB
//************************************************************************
//************************************************************************

//*************************************************************************
//  Function:  DIBRead (int, LPSTR, DWORD)
//   Purpose:  Routine to read files greater than 64K in size.
//   Returns:  TRUE if successful.
//             FALSE if an error occurs.
//*************************************************************************

BOOL DIBRead(int hFile, LPSTR lpBuffer, DWORD dwSize)
   {
   char *lpInBuf = (char *)lpBuffer;
   int nBytes;
   // Read in the data in 32767 byte chunks (or a smaller amount if it's
   // the last chunk of data read)
   while (dwSize)
      {
      nBytes = (int)(dwSize > (DWORD)32767 ? 32767 : LOWORD (dwSize));
      if (_lread(hFile, (LPSTR)lpInBuf, nBytes) != (WORD)nBytes)
         return FALSE;
      dwSize -= nBytes;
      lpInBuf += nBytes;
      }
   return TRUE;
   }

//*************************************************************************
//  Function:  ReadDIBFile (int)
//   Purpose:  Reads in the specified DIB file into a global chunk of memory.
//   Returns:  A handle to a dib (hDIB) if successful.NULL if an error occurs.
//  Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything
//             from the end of the BITMAPFILEHEADER structure on is
//             returned in the global memory handle.
// 
//  NOTE: The DIB API were not written to handle OS/2 DIBs, so this
//  function will reject any file that is not a Windows DIB.
//************************************************************************

HANDLE ReadDIBFile(int hFile)
   {
   BITMAPFILEHEADER bmfHeader;
   UINT nNumColors;   // Number of colors in table
   HANDLE hDIB;        
   HANDLE hDIBtmp;    // Used for GlobalRealloc() //MPB
   LPBITMAPINFOHEADER lpbi;
   DWORD offBits;
   // -----------
   // Allocate memory for header & color table.	We'll enlarge this
   // memory as needed.
   hDIB = GlobalAlloc(GMEM_MOVEABLE,
       (DWORD)(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)));
   if (!hDIB) return NULL;
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   if (!lpbi) 
     {
     GlobalFree(hDIB);
     return NULL;
     }
   // read the BITMAPFILEHEADER from our file
   if (sizeof (BITMAPFILEHEADER) != _lread (hFile, (LPSTR)&bmfHeader, sizeof (BITMAPFILEHEADER)))
     goto ErrExit;
   if (bmfHeader.bfType != 0x4d42)	// 'BM'   
     goto ErrExit;
   // read the BITMAPINFOHEADER
   if (sizeof(BITMAPINFOHEADER) != _lread (hFile, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)))
     goto ErrExit;
   // Check to see that it's a Windows DIB -- an OS/2 DIB would cause
   // strange problems with the rest of the DIB API since the fields
   // in the header are different and the color table entries are smaller.
   // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.
   if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
     goto ErrExit;
   // Now determine the size of the color table and read it.  Since the
   // bitmap bits are offset in the file by bfOffBits, we need to do some
   // special processing here to make sure the bits directly follow
   // the color table (because that's the format we are susposed to pass
   // back)
   if (!(nNumColors = (UINT)lpbi->biClrUsed))
      { // no color table for 24-bit, default size otherwise
      if (lpbi->biBitCount != 24)
        nNumColors = 1 << lpbi->biBitCount; // standard size table   
      }
   // fill in some default values if they are zero
   if (lpbi->biClrUsed == 0)
     lpbi->biClrUsed = nNumColors;
   if (lpbi->biSizeImage == 0)
     {
     lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3)
			 * lpbi->biHeight;
     }
   // get a proper-sized buffer for header, color table and bits
   GlobalUnlock(hDIB);
   hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize + nNumColors * sizeof(RGBQUAD) + lpbi->biSizeImage, 0);
   if (!hDIBtmp) // can't resize buffer for loading
     goto ErrExitNoUnlock; //MPB
   else
     hDIB = hDIBtmp;
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   // read the color table
   _lread (hFile, (LPSTR)(lpbi) + lpbi->biSize, nNumColors * sizeof(RGBQUAD));
   // offset to the bits from start of DIB header
   offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD);
   // If the bfOffBits field is non-zero, then the bits might *not* be
   // directly following the color table in the file.  Use the value in
   // bfOffBits to seek the bits.
   if (bmfHeader.bfOffBits != 0L)
      _llseek(hFile, bmfHeader.bfOffBits, SEEK_SET);
   if (DIBRead(hFile, (LPSTR)lpbi + offBits, lpbi->biSizeImage))
     goto OKExit;
ErrExit:
    GlobalUnlock(hDIB);    
ErrExitNoUnlock:    
    GlobalFree(hDIB);
    return NULL;
OKExit:
    GlobalUnlock(hDIB);
    return hDIB;
   }


//*************************************************************************
// LoadDIB()   Loads the specified DIB from a file, allocates memory for it,
//             and reads the disk file into the memory.
// Parameters: LPSTR lpFileName - specifies the file to load a DIB from
// Returns:    A handle to a DIB, or NULL if unsuccessful.
// NOTE:       The DIB API were not written to handle OS/2 DIBs; This
//              function will reject any file that is not a Windows DIB.
//*************************************************************************

HDIB FAR LoadDIB(LPSTR lpFileName)
   {
   HDIB hDIB;
   int hFile;
   OFSTRUCT ofs;
   // Set the cursor to a hourglass, in case the loading operation
   // takes more than a sec, the user will know what's going on.
   SetCursor(LoadCursor(NULL, IDC_WAIT));
   if((hFile = OpenFile(lpFileName, &ofs, OF_READ)) != -1)
      {
      hDIB = ReadDIBFile(hFile);
      _lclose(hFile);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      return hDIB;
      }
   else
      {
      DIBError(ERR_FILENOTFOUND);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      return NULL;
      }
   }


//***************************************************************************
// FUNCTION   : DIBWrite(int fh, VOID FAR *pv, DWORD ul)
// PURPOSE    : Writes data in steps of 32k till all the data is written.
//              Normal _lwrite uses a WORD as 3rd parameter, so it is
//              limited to 32767 bytes, but this procedure is not.
// RETURNS    : 0 - If write did not proceed correctly.
//              number of bytes written otherwise.
//**************************************************************************

DWORD PASCAL DIBWrite(int iFileHandle, VOID FAR *lpBuffer, DWORD dwBytes)
   {
   DWORD dwBytesTmp = dwBytes;       // Save # of bytes for return value
   BYTE *hpBuffer = lpBuffer;   // make a huge pointer to the data
   // -----------------------
   // Write out the data in 32767 byte chunks.
   while (dwBytes > 32767)
      {
      if (_lwrite(iFileHandle, (LPSTR)hpBuffer, (WORD)32767) != 32767)
         return 0;
      dwBytes -= 32767;
      hpBuffer += 32767;
      }
   // Write out the last chunk (which is < 32767 bytes)   
   if (_lwrite(iFileHandle, (LPSTR)hpBuffer, (WORD)dwBytes) != (WORD)dwBytes)
      return 0;
   return dwBytesTmp;
   }

//***************************************************************************
// SaveDIB()    Saves the specified DIB into the specified file name on disk.
//              No error checking is done, so if the file already exists, it 
//              will be written over.
// Parameters:  HDIB hDib - Handle to the dib to save
//              LPSTR lpFileName - pointer to full pathname to save DIB under
// Return value: 0 if successful, or one of:
//              ERR_INVALIDHANDLE
//              ERR_OPEN
//              ERR_LOCK
//***************************************************************************

WORD FAR SaveDIB(HDIB hDib, LPSTR lpFileName)
   {
   BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
   LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
   int fh;     // file handle for opened file
   OFSTRUCT of;     // OpenFile structure
   DWORD dwDIBSize;
   DWORD dwError;   // Error return from DIBWrite
   // -----------
   if (!hDib)     return ERR_INVALIDHANDLE;
   fh = OpenFile(lpFileName, &of, OF_CREATE | OF_READWRITE);
   if (fh == -1)  return ERR_OPEN;
   // Get pointer to DIB memory, first of which contains a BITMAPINFO structure
   lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib);
   if (!lpBI)     return ERR_LOCK;
   // Check to see if we're dealing with an OS/2 DIB.  If so, don't
   // save it because our functions aren't written to deal with these DIBs.
   if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
     {
     GlobalUnlock(hDib);
     return ERR_NOT_DIB;
     }
   // Fill in the fields of the file header
   // Fill in file type (first 2 bytes must be "BM" for a bitmap)   
   bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"
   // Calculating the size of the DIB is a bit tricky (if we want to
   // do it right).  The easiest way to do this is to call GlobalSize()
   // on our global handle, but since the size of our global memory may have
   // been padded a few bytes, we may end up writing out a few too
   // many bytes to the file (which may cause problems with some apps,
   // like HC 3.0).
   //
   // So, instead let's calculate the size manually.
   //
   // To do this, find size of header plus size of color table.  Since the
   // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
   // the size of the structure, let's use this.
   dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation
   // Now calculate the size of the image
   if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
      {
      // It's an RLE bitmap, we can't calculate size, so trust the
      // biSizeImage field
      dwDIBSize += lpBI->biSizeImage;
      }
   else 
      {
      DWORD dwBmBitsSize;  // Size of Bitmap Bits only
      // It's not RLE, so size is Width (DWORD aligned) * Height
      dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
      dwDIBSize += dwBmBitsSize;
      // Now, since we have calculated the correct size, why don't we
      // fill in the biSizeImage field (this will fix any .BMP files which 
      // have this field incorrect).
      lpBI->biSizeImage = dwBmBitsSize;
      }
   // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
   bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
   bmfHdr.bfReserved1 = 0;
   bmfHdr.bfReserved2 = 0;
   // Now, calculate the offset the actual bitmap bits will be in
   // the file -- It's the Bitmap file header plus the DIB header,
   // plus the size of the color table.
   bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
                      PaletteSize((LPSTR)lpBI);
   // Write the file header   
   _lwrite(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
   // Write the DIB header and the bits -- use local version of
   // DIBWrite, so we can write more than 32767 bytes of data
   dwError = DIBWrite(fh, (LPSTR)lpBI, dwDIBSize);
   GlobalUnlock(hDib);
   _lclose(fh);
   if (dwError == 0)
      return ERR_OPEN; // oops, something happened in the write
   else
      return 0; // Success code
   }


/*
//************************************************************************
//************************************************************************
// PRIVATE GRAPHICS FILE TYPE CREATION
//************************************************************************
//************************************************************************

DWORD GetDiskDriveIDNumber(void)
    {
    DWORD dwSerial = 0;
    LPDWORD lpDW   = &dwSerial;
    char szVolumeBuf[MAX_PATH]="\0";
    // ---
    // here we get some information unique to this drive
    GetVolumeInformation(NULL, (LPTSTR)&szVolumeBuf, MAX_PATH, lpDW, 
                        (LPDWORD)NULL, (LPDWORD)NULL, (LPTSTR)NULL, 0);
    // return the value in our double
    return dwSerial;
    }

//***************************************************************************
// DIWritePrivateHeader - Writes our Private Header into the start of the file
// DIWritePublicHeader  - Writes our Private Header into the start of the file
//***************************************************************************

UINT DIWritePrivateHeader(int iFileHandle)
    {
    ROOMIMAGEFILEHEADER ri;
    // --- Prepare the file header info
    strcpy(ri.szFileDesc, "HighGrow Growroom Image File");
    strcpy(ri.szFileDate, "JPG");
    ri.dwBitmapSize = (DWORD)(filelength(iFileHandle)/4*5)-12345;
    ri.dwRegCode    = GetDiskDriveIDNumber();
    // --- Write the file header   
    return _lwrite(iFileHandle, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADER));
    }

UINT DIWritePublicHeader(int iFileHandle)
    {
    ROOMIMAGEFILEHEADER ri;
    // --- Prepare the file header info
    strcpy(ri.szFileDesc, "HighGrow Growroom Image File");
    strcpy(ri.szFileDate, "GPJ");
    ri.dwBitmapSize = (DWORD)(filelength(iFileHandle)/4*5)-12345;
    ri.dwRegCode    = 1234554321;
    // --- Write the file header   
    return _lwrite(iFileHandle, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADER));
    }

//****************************************************************************
// DICheckPrivateHeader - Checks the header for our unique processor ID number
// DICheckPublicHeader  - Checks the header for our public (unused) ID number
//****************************************************************************


BOOL DICheckPrivateHeader(int iFileHandle)
    { // this checks to see if the room image file header is valid
    // *** FOR THIS PROCESSOR ONLY ***
    ROOMIMAGEFILEHEADER ri;
    // --- Read the file header into the struct
    _lread(iFileHandle, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADER));
    // --- Prepare the file header info
    if(strcmp(ri.szFileDesc, "HighGrow Growroom Image File") != 0)
        return FALSE;
    if(strcmp(ri.szFileDate, "JPG")  != 0)
        return FALSE;
    if(ri.dwBitmapSize != (DWORD)(filelength(iFileHandle)/4*5)-12345)
        return FALSE;
    if(ri.dwRegCode != GetDiskDriveIDNumber())
        return FALSE;
    // --- return our flag
    return TRUE;
    }

BOOL DICheckPublicHeader(int iFileHandle)
    { // this checks to see if the room image file header is valid
    // *** FOR THIS PROCESSOR ONLY ***
    ROOMIMAGEFILEHEADER ri;
    // --- Read the file header into the struct
    _lread(iFileHandle, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADER));
    // --- Prepare the file header info
    if(strcmp(ri.szFileDesc, "HighGrow Growroom Image File") != 0)
        return FALSE;
    if((strcmp(ri.szFileDate, "ABC")!=0)&&
       (strcmp(ri.szFileDate, "GPJ")!=0))
        return FALSE;
    if(ri.dwBitmapSize != (DWORD)(filelength(iFileHandle)/4*5)-12345)
        return FALSE;
    if(ri.dwRegCode != 1234554321)
        return FALSE;
    // --- return our flag
    return TRUE;
    }

//*************************************************************************
//  Function:  ReadHGBFile (int)
//   Purpose:  Reads in the specified DIB file into a global chunk of memory.
//   Returns:  A handle to a dib (hDIB) if successful.NULL if an error occurs.
//  Comments:  BITMAPFILEHEADER is stripped off of the DIB.  Everything
//             from the end of the BITMAPFILEHEADER structure on is
//             returned in the global memory handle.
// 
//  NOTE: The DIB API were not written to handle OS/2 DIBs, so this
//  function will reject any file that is not a Windows DIB.
//************************************************************************

HANDLE ReadHGBFile(int hFile)
   {
   BITMAPFILEHEADER bmfHeader;
   UINT nNumColors;   // Number of colors in table
   HANDLE hDIB;        
   HANDLE hDIBtmp;    // Used for GlobalRealloc() //MPB
   LPBITMAPINFOHEADER lpbi;
   DWORD offBits;
   // -----------
   // Allocate memory for header & color table.	We'll enlarge this
   // memory as needed.
   hDIB = GlobalAlloc(GMEM_MOVEABLE,
       (DWORD)(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)));
   if (!hDIB) return NULL;
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   if (!lpbi) 
     {
     GlobalFree(hDIB);
     return NULL;
     }
   // read and check the Private Header info
   if(DICheckPrivateHeader(hFile) == FALSE)
     goto ErrExit;
   // read the BITMAPFILEHEADER from our file
   if (sizeof (BITMAPFILEHEADER) != _lread (hFile, (LPSTR)&bmfHeader, sizeof (BITMAPFILEHEADER)))
     goto ErrExit;
   if (bmfHeader.bfType != 0x4d42)	// 'BM'   
     goto ErrExit;
   // read the BITMAPINFOHEADER
   if (sizeof(BITMAPINFOHEADER) != _lread (hFile, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)))
     goto ErrExit;
   // Check to see that it's a Windows DIB -- an OS/2 DIB would cause
   // strange problems with the rest of the DIB API since the fields
   // in the header are different and the color table entries are smaller.
   // If it's not a Windows DIB (e.g. if biSize is wrong), return NULL.
   if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
     goto ErrExit;
   // Now determine the size of the color table and read it.  Since the
   // bitmap bits are offset in the file by bfOffBits, we need to do some
   // special processing here to make sure the bits directly follow
   // the color table (because that's the format we are susposed to pass
   // back)
   if (!(nNumColors = (UINT)lpbi->biClrUsed))
      { // no color table for 24-bit, default size otherwise
      if (lpbi->biBitCount != 24)
        nNumColors = 1 << lpbi->biBitCount; // standard size table   
      }
   // fill in some default values if they are zero
   if (lpbi->biClrUsed == 0)
     lpbi->biClrUsed = nNumColors;
   if (lpbi->biSizeImage == 0)
     {
     lpbi->biSizeImage = ((((lpbi->biWidth * (DWORD)lpbi->biBitCount) + 31) & ~31) >> 3)
			 * lpbi->biHeight;
     }
   // get a proper-sized buffer for header, color table and bits
   GlobalUnlock(hDIB);
   hDIBtmp = GlobalReAlloc(hDIB, lpbi->biSize + nNumColors * sizeof(RGBQUAD) + lpbi->biSizeImage, 0);
   if (!hDIBtmp) // can't resize buffer for loading
     goto ErrExitNoUnlock; //MPB
   else
     hDIB = hDIBtmp;
   lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
   // read the color table
   _lread (hFile, (LPSTR)(lpbi) + lpbi->biSize, nNumColors * sizeof(RGBQUAD));
   // offset to the bits from start of DIB header
   offBits = lpbi->biSize + nNumColors * sizeof(RGBQUAD);
   // If the bfOffBits field is non-zero, then the bits might *not* be
   // directly following the color table in the file.  Use the value in
   // bfOffBits to seek the bits.
   if (bmfHeader.bfOffBits != 0L)
      _llseek(hFile, bmfHeader.bfOffBits + sizeof(ROOMIMAGEFILEHEADER), SEEK_SET);
   if (DIBRead(hFile, (LPSTR)lpbi + offBits, lpbi->biSizeImage))
     goto OKExit;
ErrExit:
    GlobalUnlock(hDIB);    
ErrExitNoUnlock:    
    GlobalFree(hDIB);
    return NULL;
OKExit:
    GlobalUnlock(hDIB);
    return hDIB;
   }




//*************************************************************************
// LoadHGB()   Loads the specified DIB from a file, allocates memory for it,
//             and reads the disk file into the memory.
// Parameters: LPSTR lpFileName - specifies the file to load a DIB from
// Returns:    A handle to a DIB, or NULL if unsuccessful.
// NOTE:       The DIB API were not written to handle OS/2 DIBs; This
//              function will reject any file that is not a Windows DIB.
//*************************************************************************

HDIB FAR LoadHGB(LPSTR lpFileName)
   {
   HDIB hDIB;
   int hFile;
   OFSTRUCT ofs;
   // Set the cursor to a hourglass, in case the loading operation
   // takes more than a sec, the user will know what's going on.
   SetCursor(LoadCursor(NULL, IDC_WAIT));
   if((hFile = OpenFile(lpFileName, &ofs, OF_READ)) != -1)
      {
      hDIB = ReadHGBFile(hFile);
      _lclose(hFile);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      return hDIB;
      }
   else
      {
      DIBError(ERR_FILENOTFOUND);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      return NULL;
      }
   }


//*************************************************************************
//  Function:  CheckHGBFileHeader (int)
//************************************************************************

BOOL CheckHGBFileHeader(HWND hWnd, int hFile)
   {
   UINT ui    = 0;
   BOOL bFlag = FALSE;
   // -------
   // check if this is a "registered" or "precessor-stamped" image file
   bFlag = DICheckPrivateHeader(hFile);
   // if this file header contains his uniques processor ID number
   if(!bFlag) // if it doesn't contain it
       { 
       // first we reset the file pointer to the start again
       _llseek(hFile, 0, SEEK_SET);
       // now check if the file header contains the public processor ID
       bFlag = DICheckPublicHeader(hFile);
       // if the public key is also invalid, he must have copied this file
       // if public key is valid, upgrade file to store the unique processor number
       if(bFlag)
           {
           // first we reset the file pointer to the start again
           _llseek(hFile, 0, SEEK_SET);
           // now check if the file header contains the public processor ID
           ui = DIWritePrivateHeader(hFile);
           // check if our write failed
           if(ui <= 0) bFlag = FALSE;
           }
       else
           {
           MessageBox(hWnd, "Invalid Grow Room Image File!\nUsing default Room Image",
                      "Hey Dude!", MB_ICONEXCLAMATION);
           }
       }
   return bFlag;
   }


//*************************************************************************
// CheckHGB()  Loads the specified DIB from a file, and checks if it is valid
//*************************************************************************


BOOL CheckHGBFile(HWND hWnd, LPSTR lpFileName)
   {
   BOOL bFlag = FALSE;
   int hFile;
   OFSTRUCT ofs;
   // Set the cursor to a hourglass, in case the loading operation
   // takes more than a sec, the user will know what's going on.
   SetCursor(LoadCursor(NULL, IDC_WAIT));
   if((hFile = OpenFile(lpFileName, &ofs, OF_READWRITE)) != -1)
      {
      bFlag = CheckHGBFileHeader(hWnd, hFile);
      _lclose(hFile);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      }
   else
      {
      DIBError(ERR_FILENOTFOUND);
      SetCursor(LoadCursor(NULL, IDC_ARROW));
      }
   return bFlag;
   }


//***************************************************************************
// SaveHGB()    Saves the specified DIB into the specified file name on disk.
//              No error checking is done, so if the file already exists, it 
//              will be written over.
// Parameters:  HDIB hDib - Handle to the dib to save
//              LPSTR lpFileName - pointer to full pathname to save DIB under
// Return value: 0 if successful, or one of:
//              ERR_INVALIDHANDLE
//              ERR_OPEN
//              ERR_LOCK
//***************************************************************************


WORD FAR SaveHGB(HDIB hDib, LPSTR lpFileName)
   {
   BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
   LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
   int fh;     // file handle for opened file
   OFSTRUCT of;     // OpenFile structure
   DWORD dwDIBSize;
   DWORD dwError;   // Error return from DIBWrite
   // -----------
   if(!hDib)     return ERR_INVALIDHANDLE;
   fh = OpenFile(lpFileName, &of, OF_CREATE | OF_READWRITE);
   if(fh == -1)  return ERR_OPEN;
   // Get pointer to DIB memory, first of which contains a BITMAPINFO structure
   lpBI = (LPBITMAPINFOHEADER)GlobalLock(hDib);
   if(!lpBI)     return ERR_LOCK;
   // Check to see if we're dealing with an OS/2 DIB.  If so, don't
   // save it because our functions aren't written to deal with these DIBs.
   if (lpBI->biSize != sizeof(BITMAPINFOHEADER))
     {
     GlobalUnlock(hDib);  return ERR_NOT_DIB;
     }
   // Fill in the fields of the file header
   // Fill in file type (first 2 bytes must be "BM" for a bitmap)   
   bmfHdr.bfType = DIB_HEADER_MARKER;  // "BM"
   // Calculating the size of the DIB is a bit tricky (if we want to
   // do it right).  The easiest way to do this is to call GlobalSize()
   // on our global handle, but since the size of our global memory may have
   // been padded a few bytes, we may end up writing out a few too
   // many bytes to the file (which may cause problems with some apps,
   // like HC 3.0).
   //
   // So, instead let's calculate the size manually.
   //
   // To do this, find size of header plus size of color table.  Since the
   // first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
   // the size of the structure, let's use this.
   dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPSTR)lpBI);  // Partial Calculation
   // Now calculate the size of the image
   if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4)) 
      {
      // It's an RLE bitmap, we can't calculate size, so trust the
      // biSizeImage field
      dwDIBSize += lpBI->biSizeImage;
      }
   else 
      {
      DWORD dwBmBitsSize;  // Size of Bitmap Bits only
      // It's not RLE, so size is Width (DWORD aligned) * Height
      dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
      dwDIBSize += dwBmBitsSize;
      // Now, since we have calculated the correct size, why don't we
      // fill in the biSizeImage field (this will fix any .BMP files which 
      // have this field incorrect).
      lpBI->biSizeImage = dwBmBitsSize;
      }
   // Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)
   bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
   bmfHdr.bfReserved1 = 0;
   bmfHdr.bfReserved2 = 0;
   // Now, calculate the offset the actual bitmap bits will be in
   // the file -- It's the Bitmap file header plus the DIB header,
   // plus the size of the color table.
   bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize +
                      PaletteSize((LPSTR)lpBI);
   // Write the public (unused) file header
   DIWritePublicHeader(fh);
   // Write the file header   
   _lwrite(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
   // Write the DIB header and the bits -- use local version of
   // DIBWrite, so we can write more than 32767 bytes of data
   dwError = DIBWrite(fh, (LPSTR)lpBI, dwDIBSize);
   GlobalUnlock(hDib);
   _lclose(fh);
   // returb success or an error
   if (dwError == 0)
      return ERR_OPEN; // oops, something happened in the write
   else
      return 0; // Success code
   }


//************************************************************************
//************************************************************************
// CONVERTING .BMP IMAGE FILES TO .HGB (PRIVATE) FILE FORMAT
//************************************************************************
//************************************************************************


BOOL ConvertBitmapToHGB(LPSTR lpSourceFileName)
    {
    HDIB hdib;
    WORD result;
    char szDestFile[MAX_PATH];
    int iLen = strlen(lpSourceFileName)-3;
    // -----------------------
    hdib = LoadDIB(lpSourceFileName);
    // --
    if(hdib)
        {
        // create the destination file name
        strcpy(szDestFile, lpSourceFileName);
        strcpy(szDestFile+iLen, "hgb");
        // save the file as a .HGB file
        result = SaveHGB(hdib, szDestFile);
        DestroyDIB(hdib);
        // return the resulting flag
        if(result==0)  return TRUE;
        else           return FALSE;
        }
    // failed to read the dib, so return false
    return FALSE;
    }


BOOL ConvertHGBToBitmap(LPSTR lpSourceFileName)
    {
    HDIB hdib;
    WORD result;
    char szDestFile[MAX_PATH];
    int iLen = strlen(lpSourceFileName)-3;
    // -----------------------
    hdib = LoadHGB(lpSourceFileName);
    // --
    if(hdib)
        {
        // create the destination file name
        strcpy(szDestFile, lpSourceFileName);
        strcpy(szDestFile+iLen, "bmp");
        // save the file as a .BMP file
        result = SaveDIB(hdib, szDestFile);
        DestroyDIB(hdib);
        // return the resulting flag
        if(result==0)  return TRUE;
        else           return FALSE;
        }
    // failed to read the dib, so return false
    return FALSE;
    }


//************************************************************************
//************************************************************************
// DRAWING/STRETCHING A VALID .HGB DIB FILE INTO A WINDOW
//************************************************************************
//************************************************************************


BOOL DIDrawHGBImageFile(HWND hwnd, LPSTR lpImageName)
    {
    HDIB hdib;
    HWND hParent = GetParent(hwnd); // parent of static is dialog
    // -----------
    // first check if this is a valid image file
    if(CheckHGBFile(hParent, lpImageName))
        {  // load the dib file
        hdib = LoadHGB(lpImageName);
        if(hdib)
            {
            // now draw the dib
            DIB_DrawWindowDIB(hwnd, hdib);
            DestroyDIB(hdib);
            return TRUE;
            }
        }
    return FALSE;
    }


//************************************************************************
//************************************************************************
// LOADING AND FREEING A VALID .HGB DIB FILE
//************************************************************************
//************************************************************************


HBITMAP DILoadHGBImageFile(HWND hwnd, LPSTR lpImageName, HPALETTE FAR* lphPalette)
    {
    HDIB     hdib=0;
    HBITMAP  hbmp=0;
    HPALETTE hpal=0;
    // -----------
    // first check if this is a valid image file
    if(CheckHGBFile(hwnd, lpImageName))
        {
        // load the dib file
        hdib = LoadHGB(lpImageName);
        if(hdib)
            {
            hpal = CreateDIBPalette(hdib);
            hbmp = DIBToBitmap(hdib, hpal);
            DestroyDIB(hdib);
            return hbmp;
            }
        }
    return FALSE;
    }

*/

