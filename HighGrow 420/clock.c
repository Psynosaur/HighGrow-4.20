// ------------------------------------------------------------
// CLOCK.C -- Small Analog Clock Program
//            based on a sample by Charles Petzold, 1996
// ------------------------------------------------------------

#include <windows.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "clock.h"
#include "zoomer.h"
#include "global.h"
#include "calc.h"
 
#define TWOPI     (2 * 3.14159)
#define BRGREEN   (RGB(128,192,128))
#define LTGREEN   (RGB(0,192,0))
#define DKGREEN   (RGB(0,128,0))

char szClockClass[] = "HGClock" ;

// ------------------------------------------------------------
// REGISTERING THE CLOCK WINDOW'S CLASS
// ------------------------------------------------------------
// EXTRA WINDOW MEMORY HOLDS:
// --------------------------
// Cell 0 = Clock Style & timer ID
// Cell 1 = Grey Brush Handle
// Cell 2 = Temperature & Humidity clock-type value


BOOL CLRegisterClockClass(HINSTANCE hInstance)
     {
     WNDCLASSEX  wndclass ;

	 wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
     wndclass.lpfnWndProc   = ClockWndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 32;  // holds clock type style, etc
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = NULL ;
     wndclass.hCursor       = LoadCursor(hInstance, "ZOOM") ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject(GRAY_BRUSH);
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szClockClass ;
	 wndclass.hIconSm       = NULL ;

     return RegisterClassEx (&wndclass);
     }


// ------------------------------------------------------------
// DRAWING THE CLOCK
// ------------------------------------------------------------

/*
void DrawClock(HWND hwnd, HDC hdc)
    {
    RECT  rc;
    SelectObject (hdc, GetStockObject(BLACK_PEN)) ;
    SelectObject (hdc, GetStockObject(WHITE_BRUSH)) ;
    GetClientRect(hwnd, &rc);
    Ellipse (hdc, rc.left+1, rc.top+1, rc.right-1, rc.bottom-1);
    }
*/

void CLRotatePoint (POINT pt[], int iNum, int iAngle)
     {
     int   i ;
     POINT ptTemp ;
     for (i = 0 ; i < iNum ; i++)
          {
          ptTemp.x = (int) (pt[i].x * cos (TWOPI * iAngle / 360) +
                            pt[i].y * sin (TWOPI * iAngle / 360)) ;
          ptTemp.y = (int) (pt[i].y * cos (TWOPI * iAngle / 360) -
                            pt[i].x * sin (TWOPI * iAngle / 360)) ;
          pt[i] = ptTemp ;
          }
     }


void CLDrawTicks (HDC hdc, int iStyle)
     {
     int   iAngle ;
     POINT pt[3] ;
     HBRUSH hGreenBrush  = CreateSolidBrush(RGB(0,255,0));
     HBRUSH hRedBrush    = CreateSolidBrush(RGB(255,0,0));
     HBRUSH hDGreenBrush = CreateSolidBrush(DKGREEN);
     HBRUSH hPrevBrush  = SelectObject(hdc, hDGreenBrush);

     for (iAngle = 0 ; iAngle < 360 ; iAngle += 30)
          {
          pt[0].x =   0 ;
          pt[0].y = 850 ;
          CLRotatePoint (pt, 1, iAngle) ;
          pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100 ;
          pt[0].x -= pt[2].x / 2 ;
          pt[0].y -= pt[2].y / 2 ;
          pt[1].x  = pt[0].x + pt[2].x ;
          pt[1].y  = pt[0].y + pt[2].y ;

          if(iStyle==STYLE_THERM)
              {
              if((iAngle>=240)&&(iAngle<=360))
                   SelectObject(hdc, hGreenBrush);
              else SelectObject(hdc, hRedBrush);
              }

          if(iStyle==STYLE_HYGRO)
              {
              if((iAngle>=300)||(iAngle<=120))
                   SelectObject(hdc, hGreenBrush);
              else SelectObject(hdc, hRedBrush);
              }

          if((iStyle==STYLE_CLOCK)||(iAngle!=180))
              Ellipse (hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y) ;
          }
     SelectObject(hdc, hPrevBrush);
     if(hGreenBrush) DeleteObject(hGreenBrush);
     if(hRedBrush)   DeleteObject(hRedBrush);
     }


void CLDrawHands(HDC hdc, struct tm *datetime, BOOL bChange)
     {
     static POINT pt[3][5] = { 0, -150, 100, 0, 0, 550, -100, 0, 0, -150,
                               0, -200,  50, 0, 0, 750,  -50, 0, 0, -200,
                               0,    0,   0, 0, 0,   0,    0, 0, 0,  750 } ;
     int          i, iAngle[3] ;
     POINT        ptTemp[3][5] ;
     iAngle[0] = (datetime->tm_hour * 30) % 360 + datetime->tm_min / 2 ;
     iAngle[1] =  datetime->tm_min  *  6 ;
     iAngle[2] =  datetime->tm_sec  *  6 ;
     memcpy (ptTemp, pt, sizeof (pt)) ;
     for (i = bChange ? 0 : 2 ; i < 3 ; i++)
          {
          CLRotatePoint (ptTemp[i], 5, iAngle[i]) ;
          Polygon (hdc, ptTemp[i], 5) ;
          }
     }


void CLDrawGuagePointer(HDC hdc, int iPercent)
     {
     static POINT pt[5] = { 0, -150, 80, 0, 0, 550, -80, 0, 0, -150 };
     int    iAngle;
     POINT  ptTemp[5] ;
     iAngle = (iPercent * 3)-150;
     memcpy(ptTemp, pt, sizeof (pt));
     CLRotatePoint(ptTemp, 5, iAngle);
     Polygon(hdc, ptTemp, 5);
     }

/*
void CLDrawBorder(HDC hdc, LPRECT rcWin)
     {
     RECT rc;
     HPEN hPen   = SelectObject(hdc, GetStockObject(WHITE_PEN));
     HPEN hBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
     CopyRect(&rc, rcWin);
     Ellipse (hdc, rc.left,   rc.top,   rc.right,   rc.bottom) ;
     Ellipse (hdc, rc.left+1, rc.top+1, rc.right-1, rc.bottom-1) ;
     SelectObject(hdc, GetStockObject(BLACK_PEN));
     Ellipse (hdc, rc.left+2, rc.top+2, rc.right-2, rc.bottom-2) ;
     SelectObject(hdc, hPen);
     SelectObject(hdc, hBrush);
     }
*/

void CLDrawBorder(HDC hdc, LPRECT rcWin)
     {
     RECT rc;
     HPEN hDKPen = CreatePen(PS_SOLID, 3, DKGREEN);
     HPEN hPen   = SelectObject(hdc, hDKPen);
     HPEN hBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
     CopyRect(&rc, rcWin);
     Ellipse (hdc, rc.left,   rc.top,   rc.right,   rc.bottom) ;
     SelectObject(hdc, hPen);
     SelectObject(hdc, hBrush);
     DeleteObject(hDKPen);
     }

void CLDrawGuageText(HWND hwnd, HDC hdc, LPRECT rcWin, LPCSTR szAppendText)
     {
     UINT uiFlag;
     RECT rcTemp;
     char szBuf[100]="\0";
     int iVal = GetWindowLong(hwnd, 8);
     CopyRect(&rcTemp, rcWin);
     InflateRect(&rcTemp, 0, -((rcTemp.bottom-rcTemp.top)/3));
     wsprintf(szBuf, "%i", iVal);
     if((rcWin->right - rcWin->left)>100)
     strcat(szBuf, szAppendText);
     if((iVal<20)||(iVal>80)) // should we show text on top?
          {
          uiFlag = DT_TOP;
          rcTemp.top-=8;
          }
     else {
          uiFlag = DT_BOTTOM;
          rcTemp.bottom+=8;
          }
     DrawText(hdc, szBuf, -1, &rcTemp, DT_SINGLELINE|DT_CENTER|uiFlag);
     }


// ------------------------------------------------------------
// SETTING A CIRCULAR WINDOW
// ------------------------------------------------------------

void CLSetCircularWindow(HWND hwnd)
     {
     HRGN hrgn;
     RECT rc;
     // get the window rectangle
     GetClientRect(hwnd, &rc);
     // now create a circular region around the window
     hrgn = CreateEllipticRgn(rc.left, rc.top, rc.right+1, rc.bottom+1);
     // and now set our window region, making a circular window
     SetWindowRgn(hwnd, hrgn, FALSE);
     }

// ------------------------------------------------------------
// SETTING THE DRAWING MODE TO ISOTROPIC
// ------------------------------------------------------------

void CLSetIsotropic (HDC hdc, int cxClient, int cyClient)
     {
     SetMapMode (hdc, MM_ISOTROPIC) ;
     SetWindowExtEx (hdc, 1000, 1000, NULL) ;
     SetViewportExtEx (hdc, cxClient / 2, -cyClient / 2, NULL) ;
     SetViewportOrgEx (hdc, cxClient / 2,  cyClient / 2, NULL) ;
     }


// ------------------------------------------------------------
// CLOCK'S WINPROC
// ------------------------------------------------------------

int iPrevTemp = 0;

LRESULT CALLBACK ClockWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static struct tm dtPrevious;
     static BOOL      bMouseDown=FALSE;
     static HBRUSH    hBrightGreenBrush = 0;
     static HBRUSH    hLightGreenBrush  = 0;
     static HBRUSH    hDarkGreenBrush   = 0;
     static HPEN      hBrightGreenPen   = 0;
     static HPEN      hLightGreenPen    = 0;
     static HPEN      hDarkGreenPen     = 0;
     BOOL             bChange;
     HDC              hdc ;
     PAINTSTRUCT      ps ;
     time_t           lTime ;
     struct tm       *datetime ;
     HINSTANCE ghInst  = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
     
     switch (iMsg)
          {
          case WM_CREATE:
               {
               int iStyle = (int)((LPCREATESTRUCT)lParam)->lpCreateParams;
               // create our drawing objects
               hBrightGreenPen   = CreatePen(PS_SOLID, 1, BRGREEN);
               hLightGreenPen    = CreatePen(PS_SOLID, 1, LTGREEN);
               hDarkGreenPen     = CreatePen(PS_SOLID, 1, DKGREEN);
               hBrightGreenBrush = CreateSolidBrush(BRGREEN);
               hLightGreenBrush  = CreateSolidBrush(LTGREEN);
               hDarkGreenBrush   = CreateSolidBrush(DKGREEN);
               // set the window region to be circular
               CLSetCircularWindow(hwnd);
               // set clock style in first memory cell
               SetWindowLong(hwnd, 0, iStyle);
               // set green pen handle in second memory cell
               SetWindowLong(hwnd, 4, (LONG)hBrightGreenPen);
               // now do te style-specific create stuff
               if(iStyle == STYLE_CLOCK)
                  {
                  time (&lTime) ;
                  datetime = localtime (&lTime) ;
                  dtPrevious = * datetime ;
                  SetTimer(hwnd, STYLE_CLOCK, 1000, NULL);
                  }

               if(iStyle == STYLE_THERM)
                  {
                  SetTimer(hwnd, STYLE_THERM, 60*1000, NULL);
                  // set the initial guage value
                  SetWindowLong(hwnd, 8, giRoomTemp);
                  iPrevTemp = giRoomTemp;
                  }

               if(iStyle == STYLE_HYGRO)
                  {
                  SetTimer(hwnd, STYLE_HYGRO, 15*60*1000, NULL);
                  // set the initial guage value
                  SetWindowLong(hwnd, 8, giHumidity);
                  }

               if(iStyle == STYLE_ZOOM)
                  {
                  HICON hicon = LoadIcon(ghInst, "zoom");
                  // set the initial guage value
                  SetWindowLong(hwnd, 8, (LONG)hicon);
                  }

               return 0 ;
               }

          case WM_ERASEBKGND:
               {
               RECT   rc;
               GetClientRect(hwnd, &rc);
               FillRect((HDC)wParam, &rc, hBrightGreenBrush);
               return TRUE;
               }

          case WM_TIMER :
               {
               HANDLE hBr, hPen;
               RECT rc;

               hdc = GetDC (hwnd) ;
               GetClientRect(hwnd, &rc);
               CLSetIsotropic (hdc, rc.right-rc.left, rc.bottom-rc.top) ;
               hBr  = SelectObject (hdc, hBrightGreenBrush) ;
               hPen = SelectObject (hdc, (HPEN)GetWindowLong(hwnd, 4)) ;
               if(wParam == STYLE_CLOCK)
                   {
                   time (&lTime) ;
                   datetime = localtime (&lTime) ;
                   bChange = datetime->tm_hour != dtPrevious.tm_hour ||
                             datetime->tm_min  != dtPrevious.tm_min ;
                   CLDrawHands(hdc, &dtPrevious, bChange) ;
                   SelectObject (hdc, hDarkGreenBrush) ;
                   SelectObject(hdc, GetStockObject(BLACK_PEN)) ;
                   CLDrawHands(hdc, datetime, TRUE) ;
                   dtPrevious = *datetime ;
                   }

               if(wParam == STYLE_THERM)
                   { // first recalculate the temperature
                   int i = CAReCalcGrowroomTemperature();
                   if(i != iPrevTemp) // if it's changed since we checked
                       { // first store the new value
                       SetWindowLong(hwnd, 4, (LONG)i);
                       // now force a redraw of this clock
                       InvalidateRect(hwnd, NULL, TRUE);                   
                       }
                   }

               if(wParam == STYLE_HYGRO)
                   {
                   int i = CACalcRoomHumidity();
                   i = max(20,min(110,i));
                   if(i != iPrevTemp) // if it's changed since we checked
                       { // first store the new value
                       SetWindowLong(hwnd, 4, (LONG)i);
                       // now force a redraw of this clock
                       InvalidateRect(hwnd, NULL, TRUE);                   
                       }
                   }

               SelectObject(hdc, hBr);
               SelectObject(hdc, hPen);
               ReleaseDC(hwnd, hdc) ;
               return 0 ;
               }

          case WM_PAINT :
               {
               RECT rc;
               HANDLE hBr, hPen, hFont;
               int iMode  =0;
               int iStyle = GetWindowLong(hwnd, 0);
               hdc = BeginPaint (hwnd, &ps) ;
               GetClientRect(hwnd, &rc);
               CLDrawBorder(hdc, &rc);
               hBr  = SelectObject (hdc, hDarkGreenBrush) ;
               hPen = SelectObject (hdc, GetStockObject(BLACK_PEN)) ;
               hFont= SelectObject (hdc, GetStockObject(DEFAULT_GUI_FONT));
               iMode= SetBkMode(hdc, TRANSPARENT);
               if(iStyle == STYLE_CLOCK)
                  {
                  CLSetIsotropic (hdc, rc.right-rc.left, rc.bottom-rc.top) ;
                  CLDrawHands(hdc, &dtPrevious, TRUE) ;
                  }

               if(iStyle == STYLE_THERM)
                   {
                   char szTemp[50]="\0";
                   int iC = GetWindowLong(hwnd, 8);
                   int iF = ((iC * 18)/10) + 32;
                   wsprintf(szTemp, " Degrees Celcius (%i F)", iF);
                   CLDrawGuageText(hwnd, hdc, &rc, szTemp);
                   CLSetIsotropic (hdc, rc.right-rc.left, rc.bottom-rc.top) ;
                   CLDrawGuagePointer(hdc, iC);
                   }

               if(iStyle == STYLE_HYGRO)
                   {
                   CLDrawGuageText(hwnd, hdc, &rc, " % Relative Humidity");
                   CLSetIsotropic (hdc, rc.right-rc.left, rc.bottom-rc.top) ;
                   CLDrawGuagePointer(hdc, GetWindowLong(hwnd, 8));
                   }

               if(iStyle == STYLE_ZOOM)
                  {
                  HICON hicon = (HICON)GetWindowLong(hwnd, 8);
                  if(hicon) DrawIcon(hdc, 10, 9, hicon);
                  }
               else
                  {
                  // now we can draw the tics
                  CLDrawTicks(hdc, iStyle);
                  }

               SetBkMode(hdc, iMode);
               SelectObject(hdc, hBr);
               SelectObject(hdc, hPen);
               SelectObject(hdc, hFont);
               EndPaint (hwnd, &ps) ;
               return 0 ;
               }

          case WM_LBUTTONDOWN:
               {
               RECT rc;
               int iStyle = GetWindowLong(hwnd, 0);
               // -------
               // check if full dragging effects are set
               GetClientRect(hwnd, &rc);
               if(iStyle == STYLE_ZOOM)
                   {
                   ZoomCreateWindow(GetParent(hwnd), 300, 400);
                   }
               else
                   { // first kill the zoomer window if it's around
                   SetWindowPos(hwnd, HWND_TOP, 0, 0, rc.right*5, rc.bottom*5, 
                                SWP_NOACTIVATE|SWP_NOMOVE);
                   CLSetCircularWindow(hwnd);
                   SetCapture(hwnd);
                   }
               bMouseDown = TRUE;
               return 0 ;
               }

          case WM_LBUTTONUP:
               {
               RECT rc;
               int iFlag = 0;
               int iStyle = GetWindowLong(hwnd, 0);
               if(!bMouseDown) return 0;
               if(iStyle != STYLE_ZOOM)
                   {
                   GetClientRect(hwnd, &rc);
                   SetWindowPos(hwnd, HWND_TOP, 0, 0, rc.right/5, rc.bottom/5, 
                                SWP_NOACTIVATE|SWP_NOMOVE);
                   CLSetCircularWindow(hwnd);
                   ReleaseCapture();
                   bMouseDown = FALSE;
                   }
               return 0 ;
               }

          case WM_DESTROY :
               {
               int iStyle = GetWindowLong(hwnd, 0);
               HPEN hPen = (HPEN)GetWindowLong(hwnd, 4);
               if(hPen) DeleteObject(hPen);
               KillTimer(hwnd, GetWindowLong(hwnd, 0)) ;
 
               if(iStyle == STYLE_ZOOM)
                  {
                  HICON hicon;
                  // now delete the magnifying glass icon
                  hicon = (HICON)GetWindowLong(hwnd, 8);
                  if(hicon) DeleteObject(hicon);
                  }
               // delete the global objects now
               if(hBrightGreenPen)   DeleteObject(hBrightGreenPen);
               if(hLightGreenPen)    DeleteObject(hLightGreenPen);
               if(hDarkGreenPen)     DeleteObject(hDarkGreenPen);
               if(hBrightGreenBrush) DeleteObject(hBrightGreenBrush);
               if(hLightGreenBrush)  DeleteObject(hLightGreenBrush);
               if(hDarkGreenBrush)   DeleteObject(hDarkGreenBrush);
               return 0 ;
               }
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

// ------------------------------------------------------------
// MAIN API FUNCTION TO DISPLAY THE CLOCK
// ------------------------------------------------------------

// iStyle 1 = Analog Clock
// iStyle 2 = Thermometer
// iStyle 3 = Hygrometer
// iStyle 3 = Magnifier

HWND CLCreateClockWindow(HWND hClockParent, HINSTANCE hInst, RECT rc, int iStyle)
     {
     HWND hClock = CreateWindow(szClockClass, NULL, WS_CHILD|WS_VISIBLE,
                                rc.left, rc.top, rc.bottom, rc.right,
                                hClockParent, (HMENU)iStyle, hInst, (LPVOID)iStyle) ;
     if(!hClock)
          {
          MessageBox (hClockParent, "Could not create the Clock Window!", 
                      szClockClass, MB_ICONEXCLAMATION | MB_OK) ;
          return NULL;
          }
     // now show and update the clock window
     ShowWindow(hClock, SW_SHOWNORMAL) ;
     UpdateWindow(hClock) ;
     return hClock;
     }

