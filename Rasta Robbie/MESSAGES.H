
// Some header files...
#include <WINDOWS.H>

/* MESSAGES.C */ 
extern SPRITE   sprite_01;
extern TOONSTEP FileToonStep;
extern TOONSTEP FrameToonStep;

extern BOOL gbFrameWalking;  // set if we're doing a non-file walk

#define BUBSTART_TIMER  1001
#define BUBBLEON_TIMER  1002
#define EXPRESN_TIMER   1003

int Random(int minrand, int maxrand);
