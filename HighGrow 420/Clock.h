
// ------------------------------------------------------------
// CLOCK.H -- Small Analog Clock Program
//            based on a sample by Charles Petzold, 1996
// ------------------------------------------------------------

#define STYLE_CLOCK   100
#define STYLE_THERM   101
#define STYLE_HYGRO   102
#define STYLE_ZOOM    103

BOOL CLRegisterClockClass(HINSTANCE hInstance);
LRESULT CALLBACK ClockWndProc (HWND, UINT, WPARAM, LPARAM);
HWND CLCreateClockWindow(HWND hClockParent, HINSTANCE hInst, RECT rc, int iStyle);
