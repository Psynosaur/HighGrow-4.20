// *************************************************************
// ******* ZOOMER.C -- Zoom Window creation and dragging *******
// *************************************************************

#include <windows.h>
#include <winreg.h>
#include "zoomer.h"
// *****************
// #include "global.h"
// *****************

#define ID_ZOOM_TIMER 10
#define ID_BUTTON     1

#define OFS_ODC   0
#define OFS_BMP   4
#define OFS_DPN   8
#define OFS_LPN   12
#define OFS_ZOOM  16

BOOL    bDown = FALSE;
HWND    hZoomWindow = 0;

// *************************************************************
// *** OFFSCREEN DC FUNCTIONS
// *************************************************************


HDC ZoomCreateODC(HWND hZoomWnd, HWND hParentWnd)
    {
    // create and prepare an offscreen DC for painting into and save it's
    // handle in the window extra bytes
    RECT    rcwin, rcparent, rcodc;
    HDC     hdc, hodc;
    HBITMAP hbmp, hobmp;
    // get the size of our windows
    GetClientRect(hParentWnd, &rcparent);
    GetClientRect(hZoomWnd, &rcwin);
    rcodc.right  = rcparent.right+rcwin.right;
    rcodc.bottom = rcparent.bottom+rcwin.bottom;
    // first create the DC and a compatible bitmap to initialize it
    hdc  = GetDC(hParentWnd);
    hodc = CreateCompatibleDC(hdc);
    hbmp = CreateCompatibleBitmap(hdc, rcodc.right, rcodc.bottom);
    // select the bitmap object into the offscreen DC
    hobmp = SelectObject(hodc, hbmp);
    // first fill the entire DC with white colour
    FillRect(hodc, &rcodc, GetStockObject(WHITE_BRUSH));
    // Fill the DC with the parent window's image
    BitBlt(hodc,rcwin.right/2,rcwin.bottom/2,rcparent.right,rcparent.bottom,hdc,0,0,SRCCOPY);
    // Save the required handles in our Window's extra bytes area
    SetWindowLong(hZoomWnd, OFS_ODC, (LONG)hodc);
    SetWindowLong(hZoomWnd, OFS_BMP, (LONG)hobmp);
    // release the temporary DC (offscreen DC and BITMAP are deleted later)
    ReleaseDC(hParentWnd, hdc);
    // now return the offscreen DC handle as a flag
    return hodc;
    }


void ZoomDeleteODC(HWND hZoomWnd)
    {
    HDC hodc      = (HDC)GetWindowLong(hZoomWnd, OFS_ODC);
    HBITMAP hobmp = (HBITMAP)GetWindowLong(hZoomWnd, OFS_BMP);
    HBITMAP hprevbmp;
    // --- reselect and delete the bitmap and offscreen dc 
    if((hobmp)&&(hodc))
        {
        hprevbmp = SelectObject(hodc, hobmp);
        if(hprevbmp) DeleteObject(hprevbmp);
        DeleteDC(hodc);
        }
    // ZERO the required handles in our Window's extra bytes area
    SetWindowLong(hZoomWnd, OFS_ODC, 0);
    SetWindowLong(hZoomWnd, OFS_BMP, 0);
    }


void ZoomPaintODC(HWND hZoomWnd, HDC hSDC)
    {
    int   iw, ih;
    int   isx, isy, isw, ish;
    RECT  rc;
    POINT pt;
    HDC   hodc     = (HDC)GetWindowLong(hZoomWnd, OFS_ODC);
    int   ipercent = (int)GetWindowLong(hZoomWnd, OFS_ZOOM);
    if(hodc)
        {
        GetWindowRect(hZoomWnd, &rc);
        iw   = rc.right  - rc.left;
        ih   = rc.bottom - rc.top;
        pt.x = rc.left;
        pt.y = rc.top;
        ScreenToClient(GetParent(hZoomWnd), &pt);
        // if this is a 100 percent (no zoom), do a normal bitblt
        if(ipercent == 100)
            BitBlt(hSDC,0,0,iw,ih,hodc,pt.x,pt.y,SRCCOPY);
        else 
            {
            isw = iw*100/ipercent;
            ish = ih*100/ipercent;
            isx = pt.x+(iw/2)-(isw/2)+(iw/2);
            isy = pt.y+(ih/2)-(ish/2)+(ih/2);
            StretchBlt(hSDC,0,0,iw,ih,hodc,isx,isy,isw,ish,SRCCOPY);
            }
        }
    }

// *************************************************************
// *** ZOOM WINPROC FUNCTIONS
// *************************************************************

void ZoomSetCircularWindow(HWND hwnd)
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


// *************************************************************
// *** ZOOM MENU CREATION
// *************************************************************

// zoom magnification
#define IDM_ZX2    1
#define IDM_ZX3    2
#define IDM_ZX4    3
#define IDM_ZX5    4
#define IDM_ZX6    5
#define IDM_ZX7    6
#define IDM_ZX8    7
#define IDM_ZX9    8
#define IDM_ZX10   9
// zoom window size
#define IDM_100   11
#define IDM_200   12
#define IDM_300   13
#define IDM_400   14
#define IDM_500   15
// closing the zoom window
#define IDM_CLOSE 21

void ZoomPopupMenu(HWND hwnd, POINT pt)
     {
     RECT rc;
     int i, istate;
     int icurzoom; 
     int icursize;
     char sztext[50] ="\0";
     // create a popup menu for displaying at right-click time
     HMENU hpopup = CreatePopupMenu();
     HMENU hzoom  = CreatePopupMenu();
     HMENU hsize  = CreatePopupMenu();
     // ---
     GetClientRect(hwnd, &rc);
     icursize = max(0, (rc.right/100));
     icurzoom = max(0, ((int)GetWindowLong(hwnd,OFS_ZOOM)/100)-1);
     // add the menu items
     if((hpopup)&&(hzoom)&&(hsize))
        { 
        // first add the zoom magnification factor items
        AppendMenu(hpopup, MF_POPUP|MF_STRING|MF_ENABLED, (UINT)hzoom,  "Zoom Factor") ;
        for(i=0;i<9;i++)
            {
            if(i+1==icurzoom)
                 istate = MF_STRING|MF_ENABLED|MF_CHECKED;
            else istate = MF_STRING|MF_ENABLED;
            wsprintf(sztext, "Zoom x%i", (i+2));
            AppendMenu(hzoom,  istate, IDM_ZX2+i,  sztext) ;
            }
        // now add the zoom window size menu items
        AppendMenu(hpopup, MF_POPUP|MF_STRING|MF_ENABLED, (UINT)hsize,  "Window Size") ;
        for(i=0;i<5;i++)
            {
            if(i+1==icursize)
                 istate = MF_STRING|MF_ENABLED|MF_CHECKED;
            else istate = MF_STRING|MF_ENABLED;
            wsprintf(sztext, "Diameter =%i", (i+1)*100);
            AppendMenu(hsize,  istate, IDM_100+i,  sztext) ;
            }
        // add the separator and exit item
        AppendMenu(hpopup, MF_SEPARATOR, 0, NULL) ;
        AppendMenu(hpopup, MF_STRING, IDM_CLOSE, "Close Zoomer") ;
        // now show the menu
        TrackPopupMenu(hpopup, TPM_LEFTALIGN|TPM_TOPALIGN, pt.x, pt.y, 0, hwnd, NULL);
        // delete the menus
        DestroyMenu(hpopup);
        DestroyMenu(hsize);
        DestroyMenu(hzoom);
        }
     }


void ZoomWindowZoomChange(HWND hwnd, int ipercent)
     {
     SetWindowLong(hwnd,OFS_ZOOM,(LONG)ipercent);
     InvalidateRect(hwnd, NULL, FALSE);
     // ---
     bDown = FALSE;
     }


void ZoomWindowSizeChange(HWND hwnd, int ipixels)
     {
     SetWindowRgn(hwnd, NULL, FALSE);
     SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, ipixels, ipixels, SWP_NOMOVE);
     ZoomSetCircularWindow(hwnd);
     // ---
     bDown = FALSE;
     }


// *************************************************************
// *** ZOOM WINDOW'S WINPROC
// *************************************************************


LRESULT CALLBACK ZoomWndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     static BOOL bFullDrag = FALSE;
     // --------
     switch (iMsg)
          {
          case WM_CREATE :
              {
              SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bFullDrag, 0);
              if(bFullDrag==FALSE)
                   SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);
               ZoomSetCircularWindow(hwnd);
               ZoomCreateODC(hwnd, GetParent(hwnd));
              }
          return 0 ;

          case WM_TIMER :
              { // if button is up, we destroy the window
               if(HIWORD(GetKeyState(VK_LBUTTON))==0)
                   DestroyWindow(hwnd);
               }
          return 0 ;

          case WM_SHOWWINDOW:
               InvalidateRect(hwnd, NULL, FALSE);
          return 0 ;

          case WM_MOVING:
               InvalidateRect(hwnd, NULL, FALSE);
          return 0 ;

          case WM_WINDOWPOSCHANGING:
                {
                RECT  rcparent, rcwin;
                POINT ptorgn, ptend;
                HWND  hparent    = GetParent(hwnd);
                LPWINDOWPOS lpwp = (LPWINDOWPOS)lParam;
                // --
                GetClientRect(hwnd, &rcwin);
                GetWindowRect(hparent, &rcparent);
                ptorgn.x = rcparent.left   - (rcwin.right /2);
                ptorgn.y = rcparent.top    - (rcwin.bottom/2);
                ptend.x  = rcparent.right  - (rcwin.right /2);
                ptend.y  = rcparent.bottom - (rcwin.bottom/2);
                // --
                lpwp->x = min(ptend.x, max(ptorgn.x, lpwp->x));
                lpwp->y = min(ptend.y, max(ptorgn.y, lpwp->y));
                }
          return 0 ;

          case WM_NCHITTEST:
                if(GetKeyState(VK_LBUTTON))
                    {
                    bDown = TRUE;
                    return HTCAPTION;
                    }
                else
                    {
                    if(bDown == TRUE)
                        {
                        bDown = FALSE;
                        DestroyWindow(hwnd);
                        return HTNOWHERE;
                        }
                    }
          return HTCLIENT;

          case WM_NCRBUTTONUP:
                {
                POINT point ;
                point.x = LOWORD (lParam);
                point.y = HIWORD (lParam);
                DestroyWindow(hwnd);
//                ZoomPopupMenu(hwnd, point);
//                bDown = FALSE;
                }
          return 0 ;

          case WM_PAINT:
			  {
              RECT   rc;
              HPEN   hdpen, hlpen, hopen;
              HBRUSH hobr;
		      HDC    hdc;
		      PAINTSTRUCT ps;
			  // ---
              hdc  = BeginPaint (hwnd, &ps) ;
              // first we stretch the parent window's offscreen DC into our window
              ZoomPaintODC(hwnd, hdc);
              // now we draw the zoom window's circular edge
              hdpen = (HPEN)GetWindowLong(hwnd, OFS_DPN);
              hlpen = (HPEN)GetWindowLong(hwnd, OFS_LPN);
              GetClientRect(hwnd, &rc);
              hopen = SelectObject(hdc, hdpen);
              hobr  = SelectObject(hdc, GetStockObject(NULL_BRUSH));
              Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
              InflateRect(&rc, -2, -2);
              SelectObject(hdc, hlpen);
              Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
              SelectObject(hdc, hopen);
              SelectObject(hdc, hobr);
              // -- 
              EndPaint (hwnd, &ps) ;
			  }
          return 0 ;

          case WM_COMMAND :
               switch (LOWORD (wParam))
                    {
                    case IDM_ZX2:  
                         ZoomWindowZoomChange(hwnd, 200);
                    return 0;
                    case IDM_ZX3:  
                         ZoomWindowZoomChange(hwnd, 300);
                    return 0;
                    case IDM_ZX4:  
                         ZoomWindowZoomChange(hwnd, 400);
                    return 0;
                    case IDM_ZX5:  
                         ZoomWindowZoomChange(hwnd, 500);
                    return 0;
                    case IDM_ZX6:  
                         ZoomWindowZoomChange(hwnd, 600);
                    return 0;
                    case IDM_ZX7:  
                         ZoomWindowZoomChange(hwnd, 700);
                    return 0;
                    case IDM_ZX8:  
                         ZoomWindowZoomChange(hwnd, 800);
                    return 0;
                    case IDM_ZX9:  
                         ZoomWindowZoomChange(hwnd, 900);
                    return 0;
                    case IDM_ZX10:  
                         ZoomWindowZoomChange(hwnd, 1000);
                    return 0;

                    case IDM_100:  
                         ZoomWindowSizeChange(hwnd, 100);
                    return 0;
                    case IDM_200:  
                         ZoomWindowSizeChange(hwnd, 200);
                    return 0;
                    case IDM_300:  
                         ZoomWindowSizeChange(hwnd, 300);
                    return 0;
                    case IDM_400:  
                         ZoomWindowSizeChange(hwnd, 400);
                    return 0;
                    case IDM_500:  
                         ZoomWindowSizeChange(hwnd, 500);
                    return 0;

                    case IDM_CLOSE :
                         SendMessage (hwnd, WM_CLOSE, 0, 0) ;
                    return 0 ;
                    }
               break ;


          case WM_DESTROY :
               bDown = FALSE;
               KillTimer (hwnd, ID_ZOOM_TIMER);
               DeleteObject((HDC)GetWindowLong(hwnd, OFS_DPN));
               DeleteObject((HPEN)GetWindowLong(hwnd, OFS_LPN));
               ZoomDeleteODC(hwnd);
               // if full-drag was off, we reset the full-drag switch to false
               if(bFullDrag==FALSE)
                   SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, 0, 0);
               hZoomWindow = 0;
          return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
     }

// *************************************************************
// *** ZOOM WINDOW'S WINCLASS REGISTRATION
// *************************************************************

ATOM ZoomLoadClass(HINSTANCE hInstance)
     {
     WNDCLASSEX  wndclass;
     // ------------
     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
     wndclass.lpfnWndProc   = ZoomWndProc ;
     wndclass.cbClsExtra    = 32;
     wndclass.cbWndExtra    = 32;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = NULL ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject(NULL_BRUSH);
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = "HGZoomerClass";
	 wndclass.hIconSm       = NULL ;
     // --------------
	 return RegisterClassEx (&wndclass) ;
	 }


// *************************************************************
// *** ZOOM WINDOW CREATION
// *************************************************************

HWND _stdcall ZoomCreateWindow(HWND hParent, int iDiameter, int iZoomPercent)
     {
     RECT  rc;
     POINT ptorg;
     HINSTANCE hInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
     // first load the zoomer window class
     ZoomLoadClass(hInst);
     // --------------------------
     if(hZoomWindow != 0)
        {
        if(IsWindow(hZoomWindow)) 
             {
             DestroyWindow(hZoomWindow);
             return FALSE;
             }
        hZoomWindow = 0; 
        }
     // now calculate the parent window's centre point for our window origin
     GetWindowRect(hParent, &rc);
     ptorg.x = (rc.right  - rc.left - iDiameter)/2;
     ptorg.y = (rc.bottom - rc.top  - iDiameter)/2;
     ClientToScreen(hParent, &ptorg);
     // now create the circular zoom window
     hZoomWindow = CreateWindowEx(WS_EX_TOPMOST, "HGZoomerClass", NULL, 
                                   WS_POPUP|WS_CLIPSIBLINGS,
		 	                       ptorg.x, ptorg.y, iDiameter, iDiameter, 
                                   hParent, NULL, hInst, NULL) ;
     // if we created the window...
     // we must create some objects and set the refresh timer
     if(hZoomWindow)
        { 
        // set the zoom percentage into our extra window memory
        SetWindowLong(hZoomWindow,OFS_ZOOM,(LONG)iZoomPercent);
        // create the dark pen for the zoom window edge
        SetWindowLong(hZoomWindow,OFS_DPN,(LONG)CreatePen(PS_SOLID, 2, RGB(0,0,0)));
        // create the dark pen for the zoom window edge
        SetWindowLong(hZoomWindow,OFS_LPN,(LONG)CreatePen(PS_SOLID, 2, RGB(192,192,192)));
        // now set the mouse position
        ptorg.x += iDiameter/2;
        ptorg.y += iDiameter/2; 
        SetCursorPos(ptorg.x, ptorg.y);
        // set the mouse pointer to down
        bDown = TRUE;
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, GetMessageExtraInfo());
        // create our mouse button checking timer (every ten millisecs
        SetTimer(hZoomWindow, ID_ZOOM_TIMER, 10, NULL);
        // finally we can show the window
        ShowWindow(hZoomWindow, SW_SHOWNOACTIVATE);
        UpdateWindow(hZoomWindow);
        }
     return hZoomWindow;
     }
