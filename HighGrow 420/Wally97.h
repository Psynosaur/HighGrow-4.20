
/******************************************************************************\
*                     GLOBAL VARIABLES AND TOONSTEP STRUCTURE
\******************************************************************************/
#define WM_ENDSOUND    0x3B9           // same as the MM_MCINOTIFY message in MCI

#define WM_ENDPLAYBACK (WM_USER+1001)
#define WM_ENDFRAME    (WM_USER+1002)

typedef struct
   { // Toon Step File structure
   int  itXpos;            // x-Position of sprite top left
   int  itYpos;            // y-Position of sprite top left
   int  itPose;            // Wally's Pose at that frame. 0 > 15 = Valid poses
   UINT uiWait;            // Wait after displaying bubble text
   BOOL btWink;            // Wink at end of frame
   char sztText[70];       // text to display in bubble
   } TOONSTEP;

typedef TOONSTEP FAR* PTOONSTEP;

/******************************************************************************\
*                              FUNCTION PROTOTYPES
\******************************************************************************/

// ************ FUNCTIONS TO BE CALLED WHEN PROCESSING MESSAGES **************

// * When processing WM_TIMER message
void Wally_Timer(HWND hWnd, WPARAM wParam);

// * When processing WM_PAINT message
void Wally_Paint(HDC hdc);

// * When processing WM_SIZE message
void Wally_Size(HWND hWnd, LPARAM lParam);

// * When processing WM_ENDFRAME message
void Wally_EndFrame(HWND hWnd);

// * When processing WM_ENDSOUND message
void Wally_EndSound(HWND hWnd);


// ****************** WALLY'S AVAILABLE ANIMATION FUNCTIONS ******************

// Wally walks to given point, poses, winks, speaks the text, and walks off again. 
BOOL WallyPlayToonFrame(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText);

// Wally walks from x&y pos in toonstep structure to tpEnd
BOOL WallyPlayToonStep(HWND hWnd, PTOONSTEP tstep, POINT ptEnd);

// play the given toonfile in the given window.
BOOL WallyPlayToonFile(HWND hWnd, LPSTR szToonFile);

// loads and plays the given toonfile resource.
BOOL WallyPlayToonResource(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource);

// **************** EZWALLY PLAYBACK FUNCTIONS (OWN WINDOW) ******************

BOOL WallyPlayToonFrameEz(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText);
BOOL WallyPlayToonFileEz(HWND hWnd, LPSTR szToonFile);
BOOL WallyPlayToonResourceEz(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource);

// *********************** ANIMATION CONTROL FUNCTIONS ***********************

// check if animation is currently playing and not interrupted.
BOOL IsWallyPlaying(void);

// stops the animation playback, close the EZ window and return true.
BOOL StopWallyPlayback(HWND hWnd);

// pause the animation playback. returns true if paused.
BOOL PauseWallyPlayback(HWND hWnd);

// continues animation playback. returns true if continued.
BOOL ContinueWallyPlayback(HWND hWnd);
