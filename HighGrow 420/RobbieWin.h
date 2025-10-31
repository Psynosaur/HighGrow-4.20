/******************************************************************************\
* 
*                                ROBBIEWIN.H
*
\******************************************************************************/

/******************************************************************************\
*                                 GLOBAL VARS
\******************************************************************************/

typedef int (*PFNDLL)();

HWND ghwndRobbie;

/******************************************************************************\
*                              FUNCTION PROTOTYPES
\******************************************************************************/

// LRESULT CALLBACK RobbieWndProc(HWND, UINT, WPARAM, LPARAM);
// BOOL CreateAnimationWindow(HWND hwnd);
// void DestroyAnimationWindow(void);
// void RRWakeWally(HWND hMainWin);

void RRFreeRobbieLibrary(HWND hwnd);
void InterruptPlayback(HWND hWnd);
BOOL CALLBACK ObjectivesDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RRPlayWindowToon(HWND hwnd, LPCSTR szToonFile);
void RRStopWindowToon(HWND hwnd);
BOOL RRPlayWindowFrame(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText);
