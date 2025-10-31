#include <WINDOWS.H>

#define EX_EYECLOSED 1
#define EX_EYEOPEN   2
#define EX_MOUTH01   3
#define EX_MOUTH02   4
#define EX_MOUTH03   5


extern  HBITMAP hFacialExprBmp; // holds wally's expressions 4x10x10 per pose

BOOL DrawPoseExpression(HWND hWnd, int iExpr);
BOOL StartExpressionTimer(HWND hWnd);        // uiSecs gives intended wait time
BOOL StartTalkExpressionTimer(HWND hWnd); // Starts expression timer for wave file playback
void StopExpressionTimer(HWND hWnd);
void DrawExpressionTimer(HWND hWnd);

