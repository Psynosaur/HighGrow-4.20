
/******************************************************************************\
*
*  PROGRAM:     RDLLWIN.C
*
*  PURPOSE:     sets up a transparent window over the parent window, to receive
*               all the Rasta Robbie messages.
*
\******************************************************************************/

#include <windows.h>
#include "rdllwin.h"
#include "Robbie.h"

BOOL      IsEzPlaying=FALSE;
BOOL      IsEzWaiting=FALSE;

HINSTANCE ghInstance=0;
HINSTANCE hPrevInst=0;
HWND      ghEzWin=0;

HANDLE EzRobbieWinShow(HWND hParent)
  {
//  HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);

  if(ghEzWin) return ghEzWin; // getout if we already have a window
    
  if(!hPrevInst)
    {
    WNDCLASS wc;
    hPrevInst = ghInstance;
    
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC)RobbieWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = ghInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor (NULL, IDC_WAIT);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = (LPCTSTR)"RobbieWinClass";

    if (!RegisterClass (&wc))
      {
      MessageBox (NULL, (LPCTSTR)"RobbieWin: RegisterClass() failed",
                  (LPCTSTR)"Error! - RobbieDLL", MB_OK | MB_ICONEXCLAMATION);
      return(0);
      }
    }

  if(!(ghEzWin=CreateWindow((LPCTSTR)"RobbieWinClass", NULL, WS_CHILD,
                               0, 0, 0, 0, hParent, NULL, ghInstance, NULL)))
    return(0);

  ShowWindow (ghEzWin, TRUE);
  return(ghEzWin);
  }


int EzRobbieMessageLoop(void)
   {
   MSG msg;
   
   IsEzWaiting=TRUE;
   while (GetMessage (&msg, NULL, 0, 0))
      {
      TranslateMessage (&msg);
      DispatchMessage  (&msg);
      }
   IsEzWaiting=FALSE;
   return (msg.wParam);
   }


/******************************************************************************\
*
*  FUNCTION:    RobbieWndProc (standard window procedure INPUTS/RETURNS)
*
\******************************************************************************/

LRESULT CALLBACK RobbieWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
  static HBITMAP hBackground;
  HINSTANCE ghInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

  switch (message)
    {
    case WM_CREATE:
         IsEzPlaying=TRUE;
    break;

    case WM_ERASEBKGND:
    return TRUE;     

    case WM_TIMER:
         Robbie_Timer(hwnd, wParam);
    break;

    case WM_PAINT:
         {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint(hwnd, &ps);
         Robbie_Paint(hdc);
         EndPaint(hwnd, &ps);
         }
    break;

    case WM_SIZE:
         {
         RECT rc;
         GetClientRect(GetParent(hwnd), &rc);
         MoveWindow(hwnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, 0);
         Robbie_Size(hwnd, lParam);
         }
    break;

    case WM_ENDFRAME:
         Robbie_EndFrame(hwnd);
    break;

    case WM_ENDSOUND:
         Robbie_EndSound(hwnd);
    break;

    case WM_ENDPLAYBACK:
         if(IsEzWaiting==FALSE)
            SendMessage(GetParent(hwnd), message, 0, 0L);
         DestroyWindow(hwnd);
    break;

    case WM_DESTROY:
         ghEzWin=0;
         IsEzPlaying=FALSE;
         if(IsEzWaiting==TRUE)
            PostQuitMessage(0);
    break;

    default:
        return (DefWindowProc(hwnd, message, wParam, lParam));
    }
  return (0);
}

