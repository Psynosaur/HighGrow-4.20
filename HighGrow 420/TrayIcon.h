// ***************************************************************
// *********************** TRAYICON.H ****************************
// ***************************************************************

#define MYWM_NOTIFYICON   WM_USER+1024
#define IDI_HIGHGROW      1
extern  BOOL gbIsMainWindowHidden; // global flag tells us what state we're in

BOOL TRTaskBarAddIcon(HWND hwnd, HINSTANCE hInst, LPSTR lpszTip);
BOOL TRTaskBarDeleteIcon(HWND hwnd);
void TR_MYWM_NOTIFYICON(HWND hwnd, HINSTANCE hInst, WPARAM wParam, LPARAM lParam);
