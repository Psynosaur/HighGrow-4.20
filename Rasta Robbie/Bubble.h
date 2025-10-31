#include <WINDOWS.H>

// extern SPRITE sprite_bubble;

void SetBubblePos(HWND hWnd, POINT ptMouth, UINT uPrefDir);
void LoadBubbleBitmaps(HWND hWnd, HINSTANCE ghInst);
void FreeBubbleBitmaps(void);
BOOL bubble_show(HWND hWnd, LPCSTR szBubbleText);
BOOL bubble_hide(HWND hWnd);
