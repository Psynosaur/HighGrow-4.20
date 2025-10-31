
#include "windows.h"

extern BOOL Animating;

typedef struct
   {
   int     WaveIndex;     // channel (1 to 7) for sound file (0=No Sound)
   POINT   pFrame;        // frame bitmap origin pos
   HBITMAP hFrame;        // handle to frame bitmap
   } ANIFRAME;

// *************** PUBLIC FUNCTIONS **************
#define ANIM_TIMER    1000  // timer identifier


// Handles WM_TIMER message
void anim_timer(HWND hWnd);
// WHERE: hWnd = Window to walk in. (also identifies winproc for notify msg)

// Animates the currently initialized animation.
BOOL anim_animate(HWND hWnd, UINT uNotifyMsg, int speed);
// WHERE: hWnd       = Window to walk in. 
//        uNotifyMsg = Message will be sent when animation is complete.
//        speed      = speed factor. (1=normal, 2=fast, 3=superfast)

// Interrupts the current animation.
BOOL anim_interrupt(HWND hWnd);
// WHERE: hWnd       = Window we're walking in. 

// Continues with an interrupted animation
BOOL anim_continue(HWND hWnd);
// WHERE: hWnd       = Window we were walking in. 

// Initializes the animation with our bitmaps.
int anim_init(POINT ptStart, POINT ptEnd, int step, SPRITE* spr, HBITMAP FAR* hBitmaps, int iPose);
// WHERE: ptStart   = starting point of walk. (usually end of prev walk)
//        ptEnd     = end point of walk
//        step      = number of pixels to move each frame in the walk
//        *hBitmaps = pointer to an array of handles to our 20 bitmaps
//        iPose     = between 0&8 shows which of the bitmaps is the end pose
// RETURNS: Total number of frames in the sequence.

