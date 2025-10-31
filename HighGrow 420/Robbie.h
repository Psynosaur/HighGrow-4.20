
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
   int  itPose;            // Robbie's Pose at that frame. 0 > 15 = Valid poses
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
void Robbie_Timer(HWND hWnd, WPARAM wParam);

// * When processing WM_PAINT message
void Robbie_Paint(HDC hdc);

// * When processing WM_SIZE message
void Robbie_Size(HWND hWnd, LPARAM lParam);

// * When processing WM_ENDFRAME message
void Robbie_EndFrame(HWND hWnd);

// * When processing WM_ENDSOUND message
void Robbie_EndSound(HWND hWnd);


// ****************** Robbie'S AVAILABLE ANIMATION FUNCTIONS ******************

// Robbie walks to given point, poses, winks, speaks the text, and walks off again. 
BOOL RobbiePlayToonFrame(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText);

// Robbie walks from x&y pos in toonstep structure to tpEnd
BOOL RobbiePlayToonStep(HWND hWnd, PTOONSTEP tstep, POINT ptEnd);

// play the given toonfile in the given window.
BOOL RobbiePlayToonFile(HWND hWnd, LPSTR szToonFile);

// loads and plays the given toonfile resource.
BOOL RobbiePlayToonResource(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource);

// **************** EZRobbie PLAYBACK FUNCTIONS (OWN WINDOW) ******************

BOOL RobbiePlayToonFrameEz(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText);
BOOL RobbiePlayToonFileEz(HWND hWnd, LPSTR szToonFile);
BOOL RobbiePlayToonResourceEz(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource);

// *********************** ANIMATION CONTROL FUNCTIONS ***********************

// check if animation is currently playing and not interrupted.
BOOL IsRobbiePlaying(void);

// stops the animation playback, close the EZ window and return true.
BOOL StopRobbiePlayback(HWND hWnd);

// pause the animation playback. returns true if paused.
BOOL PauseRobbiePlayback(HWND hWnd);

// continues animation playback. returns true if continued.
BOOL ContinueRobbiePlayback(HWND hWnd);
