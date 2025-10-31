
#include <windows.h>
#include <math.h>
#include "sprite32.h"
#include "animate.h"

ANIFRAME anim_walk[255];
SPRITE*  anim_sprite;
BOOL     Animating = FALSE;
int      WalkFrames;   // total number of frames in walk.
int      AnimFrame;
int      AnimSpeed;
UINT     WM_ANIMNOTIFY;


// ********************************************************************
// ********************* PRIVATE FUNCTIONS ****************************
// ********************************************************************

void animate_frame(HWND hwnd, SPRITE *sprite_id, ANIFRAME *frame_id, int curFrame)
   {
   HDC   hdc;
   POINT ptStart;
   
   // prepare our sprite's next bitmap
   hdc = GetDC(hwnd);
   sprite_id->hbmImg_id = frame_id[curFrame].hFrame;   
   InitSpriteInfo(hdc, sprite_id->hbmImg_id, sprite_id);   
   ReleaseDC(hwnd, hdc);
   
   // set the animation's start point
   if(curFrame == 1)
      ptStart = frame_id[0].pFrame;
   else
      ptStart = frame_id[curFrame-1].pFrame;
      
   // then we set the animation's start point
   sprite_id->xPrev = ptStart.x;
   sprite_id->yPrev = ptStart.y;
      
   // here we animate the sucker
   MoveSprite(hwnd, sprite_id, &frame_id[curFrame].pFrame);

   // and finally we set the animation's end point
   sprite_id->xPrev = sprite_id->bmX;
   sprite_id->yPrev = sprite_id->bmY;
   } 
   
// ********************************************************************
// ********************* PUBLIC FUNCTIONS ****************************
// ********************************************************************

// Handles WM_TIMER message
void anim_timer(HWND hWnd)
   // hWnd = Window to walk in. (also identifies winproc for notify msg)
   {

   if(IsIconic(hWnd) == FALSE)
      {
      HDC hdc;

      if(Animating==FALSE) 
         AnimFrame = WalkFrames;

      hdc = GetDC(hWnd);

      AnimFrame += 1;
      if(AnimFrame>WalkFrames)
         {
         AnimFrame = 0;
         Animating = FALSE;
         KillTimer(hWnd, (UINT)ANIM_TIMER);
         SendMessage(hWnd, WM_ANIMNOTIFY, 0, 0L);
         }         
      else 
         animate_frame(hWnd, anim_sprite, anim_walk, AnimFrame);
               
      ReleaseDC(hWnd, hdc);
      }
   }


BOOL anim_animate(HWND hWnd, UINT uNotifyMsg, int speed)
   // hWnd       = Window to walk in. 
   // uNotifyMsg = Message will be sent when animation is complete.
   // speed      = speed factor. (1=slow, > 5=superfast)
   {
   if(Animating) return FALSE;
                              
   WM_ANIMNOTIFY = uNotifyMsg;

   speed = 210-(min(max(speed,1),5)*20);
   
   AnimSpeed = speed;
   Animating = (BOOL)SetTimer(hWnd, (UINT)ANIM_TIMER, speed, NULL);
   
   return Animating;
   }


BOOL anim_interrupt(HWND hWnd)
   // hWnd       = Window to walk in. 
   {
   if(Animating==FALSE) return FALSE;
                              
   KillTimer(hWnd, (UINT)ANIM_TIMER);

   Animating = FALSE;   
   return Animating;
   }

BOOL anim_continue(HWND hWnd)
   // hWnd       = Window to walk in. 
   {
   if(Animating==TRUE) return FALSE;
                              
   Animating = (BOOL)SetTimer(hWnd, (UINT)ANIM_TIMER, AnimSpeed, NULL);
   return Animating;
   }


int anim_init(POINT ptStart, POINT ptEnd, int step, SPRITE* spr, HBITMAP* hBitmaps, int iPose)
   // ptStart   = starting point of walk. (usually end of prev walk)
   // ptEnd     = end point of walk
   // step      = number of pixels to move each frame in the animation
   // *hBitmaps = pointer to an array of handles to our 28 bitmaps
   // iPose     = between 0&7 shows which of the bitmaps is the end pose
   // ***Returns the total number of frames in the sequence.
   {
   int i, j;         // for our loops
   int dx, dy;       // holds the calculated delta x and y
   int ptr, turnptr; // pointers to the walk and turn bitmaps in handle array
   int stepx, stepy; // factors to add to current x and y position in walk
   int length;       // total length in pixels of walk
   
   if(Animating) return 0;
   
   anim_sprite = spr;

   // calculate delta x and y
   dx = ptStart.x - ptEnd.x;
   dy = ptStart.y - ptEnd.y;

   length = (int)max((sqrtl(powl(dx,2)+powl(dy,2))), 1.0);

   WalkFrames = length/max(step,1);          // frames/walklen

   i=0;
   for(j=0;j<WalkFrames;j+=8)
      i+=8;
  
   WalkFrames = max(i,1);
   
   stepx  = dx/WalkFrames;    // pixels to move in x direction in each step 
   stepy  = dy/WalkFrames;    // pixels to move in y direction in each step 

   // calculate which direction we're going
   if(dx<0)
      {   // we're going right
      ptr     = 0;    
      turnptr = 16;
      }
   else
      {   // we're going left
      ptr     = 8;            
      turnptr = 18;  
      }
      
   // set all the walk frame positions and bitmaps
   i = ptr;
   for(j=0; j<WalkFrames+1; j+=1)
      {

      anim_walk[j].pFrame.x = ptStart.x;
      anim_walk[j].pFrame.y = ptStart.y;
      anim_walk[j].hFrame   = hBitmaps[i];

      ptStart.x = ptStart.x - stepx;
      ptStart.y = ptStart.y - stepy;
      
      if(i==ptr+7) i=ptr;
      i+=1;
      }

   // now change the first two and last two bitmaps 
   anim_walk[0].hFrame = hBitmaps[turnptr];
   anim_walk[1].hFrame = hBitmaps[turnptr+1];
   anim_walk[WalkFrames-1].hFrame = hBitmaps[turnptr];
   anim_walk[WalkFrames].hFrame   = hBitmaps[turnptr+1];

   // now we add one frame and set the selected pose bitmap
   WalkFrames+=1;
   anim_walk[WalkFrames].pFrame.x = anim_walk[WalkFrames-1].pFrame.x;
   anim_walk[WalkFrames].pFrame.y = anim_walk[WalkFrames-1].pFrame.y;
   anim_walk[WalkFrames].hFrame = hBitmaps[20+iPose];
      
   AnimFrame = 0;
      
   return WalkFrames; 
   }
   
   
