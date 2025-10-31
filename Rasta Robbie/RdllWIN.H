
/******************************************************************************\
*                 FUNCTIONS REQUIRED TO CONTROL WALKING WINDOW
\******************************************************************************/

extern HWND      ghEzWin;
extern BOOL      IsEzPlaying;
extern HINSTANCE ghInstance;

LRESULT CALLBACK RobbieWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

HANDLE EzRobbieWinShow(HWND hParent);
int EzRobbieMessageLoop(void);
