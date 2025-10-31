/****************************************************************************\
*                                                                            *
* RobbieDLL by Slick Software 1996                                            *
*                                                                            *
* MESSAGES.C - This module handles all the window's messages.                *
*                                                                            *
\****************************************************************************/

#include "sprite32.h"
#include "robbie.h"
#include "messages.h"
#include "animate.h"
#include "bubble.h"
#include "toonfile.h"
#include "expresns.h"
#include "rdllwin.h"
#include "playsnd.h"

BOOL RobbieRegistered=TRUE;
UINT gnMaxBackTiles=10;

// Sprite's info structures
SPRITE   sprite_01;
TOONSTEP FileToonStep;
TOONSTEP FrameToonStep;

BOOL gbFrameWalking = FALSE;  // set if we're doing a non-file walk
BOOL gbRegistered   = TRUE;  // set if this is the registered version.
BOOL bToonFilePlaying;
BOOL bRobbieSizing;
BOOL wallyloaded  = FALSE;
BOOL BubbleErased = TRUE;
UINT uWalkDir;  // 1=left_to_right, 2=right_to_left

HBITMAP hRobbieBmps[20+iMaxPoses];

// ****************** SOME MISCELLANEOUS STUFF ********************

int Random(int minrand, int maxrand)
   { // returns a random number from minrand to maxrand in value
   int t = maxrand+1;
   while((t>maxrand)|(t<minrand))
         t = rand()/10;
   return t;
   }

// ****************** BUBBLE DRAWING FUNCTIONS ********************

void RobbieSetBubblePos(HWND hWnd)
   {
   POINT ptMouth;

   ptMouth.x = sprite_01.bmX+65;
   ptMouth.y = sprite_01.bmY+35;
   SetBubblePos(hWnd, ptMouth, uWalkDir);
   }

void HideRobbieBubble(HWND hWnd)
   {
   bubble_hide(hWnd);
   BubbleErased=TRUE;
   }

void ShowRobbieBubble(HWND hWnd, LPCSTR szbubtext)
   {
   if(!gbRegistered)
      {
      if(Random(0,100)==50)
         bubble_show(hWnd, " Hey, this version of the Rasta Robbie DLL is still unregistered!");
      else  
         bubble_show(hWnd, szbubtext);
      }
   else  
      bubble_show(hWnd, szbubtext);

   BubbleErased=FALSE;
   }

// ************** LOADING AND FREEING BITMAPS *********************

void FreeRobbieBitmaps(void)
   {
   int hbm;
   
   FreeBubbleBitmaps();
   BubbleErased=TRUE;
   
   if(hFacialExprBmp)
      {
      DeleteObject(hFacialExprBmp);
      hFacialExprBmp=0;
      }

   // Delete our bitmap objects
   for(hbm=0; hbm<(20+iMaxPoses); hbm+=1)
      {
      if(hRobbieBmps[hbm])  
         DeleteObject(hRobbieBmps[hbm]);

      hRobbieBmps[hbm]=0;
      }

   wallyloaded=FALSE;
   }

BOOL InitRobbieSprites(HWND hWnd, POINT ptStart)
   {
   HDC hDC;

   hDC = GetDC(hWnd);
   sprite_01.hbmImg_id = hRobbieBmps[0];
   EraseSprite(hDC, &sprite_01);
   sprite_01.xPrev = ptStart.x;
   sprite_01.yPrev = ptStart.y;
   InitSpriteInfo(hDC, sprite_01.hbmImg_id, &sprite_01);
   ReleaseDC(hWnd, hDC);              
   return TRUE;
   }


void LoadRobbieBitmaps(HINSTANCE ghInst)
   {
   char szResource[7];
   char szResID[3];
   int  i;
   
   FreeRobbieBitmaps();
   // load the bitmaps of wally walking from left to right
   for(i=0;i<8;i+=1)
      {
      _ltoa(i+1, szResID, 10);
      strcpy(szResource, "rrl2r");
      strcat(szResource, szResID);
      hRobbieBmps[i]=LoadBitmap(ghInst, szResource);
      }

   // load the bitmaps of wally walking from right to left
   for(i=0;i<8;i+=1)
      {
      _ltoa(i+1, szResID, 10);
      strcpy(szResource, "rrr2l");
      strcat(szResource, szResID);
      hRobbieBmps[i+8]=LoadBitmap(ghInst, szResource);
      }

   // load the bitmaps of wally turning 
   hRobbieBmps[16] = LoadBitmap(ghInst, (LPSTR)"rrltn1");
   hRobbieBmps[17] = LoadBitmap(ghInst, (LPSTR)"rrltn2");
   hRobbieBmps[18] = LoadBitmap(ghInst, (LPSTR)"rrrtn1");
   hRobbieBmps[19] = LoadBitmap(ghInst, (LPSTR)"rrrtn2");

   // load the bitmaps of wally posing
   for(i=0;i<iMaxPoses;i+=1)
      {
      _ltoa(i+1, szResID, 10);
      strcpy(szResource, "pose");
      if(i<9) strcat (szResource, "0");
      strcat(szResource, szResID);
      hRobbieBmps[i+20]=LoadBitmap(ghInst, szResource);
      }

   LoadBubbleBitmaps(GetDesktopWindow(), ghInst);
   hFacialExprBmp = LoadBitmap(ghInst, "expresns");

   wallyloaded = TRUE;
   }

// *********** WALLY'S BASIC WALKING FUNCTION *******************

BOOL RobbieWalk(HWND hWnd, POINT ptStart, POINT ptEnd, int iPose)
   {
   iPose=min(max(iPose, 0),iMaxPoses-1);
   if(anim_init(ptStart, ptEnd, 20, &sprite_01, &hRobbieBmps[0], iPose)==0)
      return FALSE;

   if((ptEnd.x-ptStart.x)>0)   
      uWalkDir = 1;
   else
      uWalkDir = 2;

   return anim_animate(hWnd, WM_ENDFRAME, gnWalkSpeed);
   }


// **************************************************************
// *******************WALKING WALLY BY FILE**********************
// **************************************************************

POINT ptStart;
BOOL  bPlaybackReady = FALSE;
UINT  uiInterrupted   = 0;      // 1=interrupted while walking, 2=while talking
int   CurFileStep;
int   iFrameWaitHold=0;
char  szBubbleTextHold[70]="\0";

void SetFrameBubbleTimer(HWND hWnd, LPCSTR szbubtext, int ibWaitTime)
   { // Displays the given text in a bubble after ibWaitTime secs.
   // sets BUBSTART_TIMER going for half a second, and then turns it off
   // and starts BUBBLEON_TIMER to wait for ibWaitTime seconds. If ibWaitTime
   // is = -1, we'll ignore it, unless we must activate BUBBLEON_TIMER.
   // zero ibWaitTime = turns BUBBLEON_TIMER off.

   if(ibWaitTime==0)
      {
      if((szBubbleTextHold[0])&&(!IsSoundEnabled(szBubbleTextHold)))
         HideRobbieBubble(hWnd);

      szBubbleTextHold[0]=0;
      StopExpressionTimer(hWnd);
      KillTimer(hWnd, (UINT)BUBBLEON_TIMER);
      }
   else 
      {
      if(iFrameWaitHold)
         {
         BOOL bTalking=FALSE;

         DrawPoseExpression(hWnd, EX_EYEOPEN);
         KillTimer(hWnd, (UINT)BUBSTART_TIMER);
         SetTimer(hWnd, (UINT)BUBBLEON_TIMER, iFrameWaitHold*1000, NULL);

         if(IsSoundEnabled(szBubbleTextHold))
            bTalking = PlayToonWave(hWnd, (szBubbleTextHold)); 

         if(bTalking)
            StartTalkExpressionTimer(hWnd);
         else
            {
            StartExpressionTimer(hWnd);
            if(szBubbleTextHold[0]!=0)
               {
               RobbieSetBubblePos(hWnd);
               ShowRobbieBubble(hWnd, szBubbleTextHold);
               }
            }
         BubbleErased=FALSE;  // must be done regardless of text or not
         iFrameWaitHold=0;
         }
      else
         {
         if(ibWaitTime>0)
            {
            DrawPoseExpression(hWnd, EX_EYECLOSED);
            iFrameWaitHold = ibWaitTime;
            SetTimer(hWnd, (UINT)BUBSTART_TIMER, 500, NULL);
            strcpy(szBubbleTextHold, szbubtext);
            }
         }
      }
   }


BOOL PlayFileFrame(HWND hwnd)
   {
   POINT ptEnd;

   // first we set our exit conditions
   if(!GetCurToonStep(CurFileStep, &FileToonStep)) return FALSE;
   // set the window point from frame structure      
   ptEnd.x = FileToonStep.itXpos-sprite_01.bmWidth/2;
   ptEnd.y = FileToonStep.itYpos-sprite_01.bmHeight/2;
   // adjust file point for window new dimensions
   AdjustPlayWindowPoint(hwnd, &ptEnd);
   // set the walking going...
   RobbieWalk(hwnd, ptStart, ptEnd, FileToonStep.itPose);
   // ..and set the new start point
   ptStart = ptEnd;
   // hold our current file's position
   CurToonStep =CurFileStep;
   CurFileStep+=1;
   return TRUE;
   }

int GetFrameProgress(int iLength)
   { // returns the frame file position as a percentage of Length
   return(CurFileStep*iLength)/max(MaxToonSteps, 1);
   }
   

void InitFrameFilePlayback(HWND hWnd)
   {
   RECT rc;

   CurFileStep = 0;
   GetCurToonStep(CurFileStep, &FileToonStep);
   GetClientRect(hWnd, &rc);
   ptStart.y = FileToonStep.itYpos;
   ptStart.x = FileToonStep.itXpos;
   // adjust the point for the current window size
   AdjustPlayWindowPoint(hWnd, &ptStart);
   if(ptStart.x<(rc.right-rc.left)/2)
      ptStart.x = -150; 
   else
      ptStart.x = (rc.right-rc.left)+30; 

   InitRobbieSprites(hWnd, ptStart);
   bPlaybackReady = TRUE;
   }
   
void EndFrameFilePlayback(HWND hWnd)
   {
   RECT  rc;
   POINT pt;
   
   GetClientRect(hWnd, &rc);

   pt.y = ptStart.y;

   if(ptStart.x<((rc.right-rc.left)/2))
      pt.x   = -150; 
   else
      pt.x   = (rc.right-rc.left)+30; 

   FileToonStep.sztText[0]= 0;
   FileToonStep.uiWait    = 5;

   RobbieWalk(hWnd, ptStart, pt, 0);
   CurFileStep = 0;
   bPlaybackReady   = FALSE;
   bToonFilePlaying = FALSE;
   }   

void FilePlaybackTimer(HWND hWnd)
   {
   if(BubbleErased==FALSE)
      bToonFilePlaying = TRUE;
   if(bPlaybackReady==FALSE)
      {
      InitFrameFilePlayback(hWnd);
      bToonFilePlaying = TRUE;
      }
   if(CurFileStep==MaxToonSteps)
      EndFrameFilePlayback(hWnd);
   else
      PlayFileFrame(hWnd);

   SetFrameBubbleTimer(hWnd, NULL, 0);
   }
   
void ContinueFilePlayback(HWND hWnd)
   { // responds to the WM_ENDFRAME message
   if(bToonFilePlaying==FALSE)
      SendMessage(hWnd, WM_ENDPLAYBACK, 0, 0L);
   else
      { 
      bToonFilePlaying=FALSE;
      SetFrameBubbleTimer(hWnd, FileToonStep.sztText, FileToonStep.uiWait);
      }
   }

void RobbieFileReset(HWND hWnd)
   {
   if(wallyloaded)
      {
      InitFrameFilePlayback(hWnd);
      bPlaybackReady   = FALSE;
      bToonFilePlaying = FALSE;
      }
   }

BOOL RobbiePlayToonFile(HWND hWnd, LPSTR szToonFile)
   { // plays the given toonfile if it is one
   if(szToonFile==NULL) return FALSE;
   if(!SaverFileOpen(hWnd, szToonFile)) 
      return FALSE;
   else
      {
      RobbieFileReset(hWnd);
      FilePlaybackTimer(hWnd);
      return TRUE;
      }
   }

// ***************************************************************
// ************PLAYING EMBEDDED TOONFILE RESOURCES****************
// ***************************************************************

BOOL RobbiePlayToonResource(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource)
   { // plays the given toonfile if it is one
   if(szToonResource==NULL) return FALSE;
   if(!LoadToonResource(hWnd, ghInst, szToonResource)) 
      return FALSE;
   else 
      {
      RobbieFileReset(hWnd);
      FilePlaybackTimer(hWnd);
      return TRUE;
      }
   }

// ************* SINGLE FRAME WALKING **********************
BOOL  gbSingleWalk    =FALSE;  // if walking without walking offscreen again
BOOL  gbFrameOnscreen =FALSE;
BOOL  gbIntroPlayed   =FALSE;

BOOL FramePlaybackInit(HWND hwnd)
   { // loads & inits, and sets up the offscreen start point 
   RECT rc;

   if(gbFrameWalking) return FALSE;

   GetClientRect(hwnd, &rc);
   ptStart.y = FrameToonStep.itYpos;
   if(FrameToonStep.itXpos<((rc.right-rc.left)/2))
      ptStart.x = -150; 
   else
      ptStart.x = (rc.right-rc.left)+30; 

   // set the sprite's start position
   InitRobbieSprites(hwnd, ptStart);
   gnWalkSpeed      = 3;
   bToonFilePlaying = FALSE;
   gbFrameOnscreen  = FALSE;
   gbSingleWalk     = FALSE;
   return TRUE;
   }
   
BOOL PlayRobbieFrame(HWND hwnd)
   {
   POINT ptEnd;

   ptEnd.x   = FrameToonStep.itXpos;
   ptEnd.y   = FrameToonStep.itYpos;
   // set the walking going...
   RobbieWalk(hwnd, ptStart, ptEnd, FrameToonStep.itPose);
   gbFrameWalking = TRUE;
   // and reset the start point for next move
   ptStart = ptEnd;
   return TRUE;
   }

void FramePlaybackEnd(HWND hWnd)
   {
   if(gbFrameOnscreen)
      {
      gbFrameWalking  = FALSE;
      gbFrameOnscreen = FALSE;
      SendMessage(hWnd, WM_ENDPLAYBACK, 0, 0L);
      }
   else
      SetFrameBubbleTimer(hWnd, FrameToonStep.sztText, FrameToonStep.uiWait);
   }


void FramePlaybackTimer(HWND hWnd)
   {
   RECT rc;

   if(!BubbleErased)
     SetFrameBubbleTimer(hWnd, NULL, 0);

   gbFrameOnscreen = TRUE;

   if(gbSingleWalk)
      FramePlaybackEnd(hWnd);
   else
      {
      GetClientRect(hWnd, &rc);

      if(FrameToonStep.itXpos<((rc.right-rc.left)/2))
         FrameToonStep.itXpos = -150; 
      else
         FrameToonStep.itXpos = (rc.right-rc.left)+30; 
      
      PlayRobbieFrame(hWnd);
      }
   }


// ************ PLAYING TOON STEPS *************

BOOL RobbiePlayToonStep(HWND hWnd, PTOONSTEP tstep, POINT ptEnd)
   { // walks from x and y position of toonstep struct to ptEnd
   // When end point is reached, wally winks and talks if specified.
   // first check our exit condition
   int  iMaxX, iMaxY;
   RECT rc;

   if(gbFrameWalking) return FALSE;
   if(CurFileStep!=0) return FALSE;
   // zero our text buffer
   FrameToonStep.sztText[0]=0;
   // calculate our maximum end point
   GetClientRect(hWnd, &rc);
   iMaxX = (rc.right-rc.left)+150;
   iMaxY = (rc.bottom-rc.top)+150;
   // first fix start point and adjust his toonstep
   ptStart.x     = tstep->itXpos-65;
   ptStart.y     = tstep->itYpos-65;
   tstep->itXpos = ptEnd.x;
   tstep->itYpos = ptEnd.y;
   // adjust the end point for center of bitmap
   ptEnd.x -=65;
   ptEnd.y -=65;
   // check if sprite hasn't yet moved to position
   if(((abs(sprite_01.bmX - ptStart.x))>65)|
      ((abs(sprite_01.bmY - ptStart.y))>65)|
      (sprite_01.hbmImg_id==0))
       InitRobbieSprites(hWnd, ptStart);
   // now copy his struct into our frametoonstep
   FrameToonStep.itXpos = max(min(ptEnd.x, iMaxX), -150);
   FrameToonStep.itYpos = max(min(ptEnd.y, iMaxY), -150);
   FrameToonStep.itPose = max(min(tstep->itPose, iMaxPoses-1), 0);
   FrameToonStep.uiWait = max(min(tstep->uiWait, 100), 1);
   FrameToonStep.btWink = tstep->btWink;
   // now sort out our text buffer
   FrameToonStep.sztText[0]=0; 
   if(tstep->sztText[0]!=0)
      strcpy(FrameToonStep.sztText, tstep->sztText);
   // set our global vars
   gnWalkSpeed     = 3;
   gbSingleWalk    = TRUE;
   gbFrameWalking  = TRUE;
   gbFrameOnscreen = FALSE;
   // and go for it!
   return RobbieWalk(hWnd, ptStart, ptEnd, FrameToonStep.itPose);
   }

// ************ PLAYING TOON FRAMES ***************

BOOL RobbiePlayToonFrame(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText)
   {
   if(gbFrameWalking) return FALSE;

   FrameToonStep.itXpos = ptEnd.x-65;
   FrameToonStep.itYpos = ptEnd.y-65; 
   FrameToonStep.itPose = max(min(iPose, iMaxPoses), 1)-1;
   FrameToonStep.uiWait = 5;
   FrameToonStep.btWink = bWink;
   FrameToonStep.sztText[0]=0;
   if(szText!=NULL)
      strcpy(FrameToonStep.sztText, szText);

   if(FramePlaybackInit(hWnd))
      {
      PlayRobbieFrame(hWnd);
      return TRUE;
      }
   return FALSE;
   }

// ************ WALLYEX PLAYBACK ROUTINES (IN OWN WINDOW)***********

BOOL RobbiePlayToonFrameEz(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText)
   {
   HANDLE hEzWin=0;

   if(gbFrameWalking) return FALSE;
      
   hEzWin=EzRobbieWinShow(hWnd);
   if(!hEzWin) return FALSE;
      
   return RobbiePlayToonFrame(hEzWin, ptEnd, iPose, bWink, szText);
   }


BOOL RobbiePlayToonResourceEz(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource)
   {
   HANDLE hEzWin=0;

   if(gbFrameWalking) return FALSE;
      
   hEzWin=EzRobbieWinShow(hWnd);
   if(!hEzWin) return FALSE;
      
   return RobbiePlayToonResource(hEzWin, ghInst, szToonResource);
   }

BOOL RobbiePlayToonFileEz(HWND hWnd, LPSTR szToonFile)
   {
   HANDLE hEzWin=0;

   if(gbFrameWalking) return FALSE;
      
   hEzWin=EzRobbieWinShow(hWnd);
   if(!hEzWin) return FALSE;
      
   return RobbiePlayToonFile(hEzWin, szToonFile);
   }

// *** WALLYEZ PLAYBACK ROUTINES (returns when finished playing)******

BOOL RobbiePlayToonFrameEx(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText)
   {
   if(!RobbiePlayToonFrameEz(hWnd, ptEnd, iPose, bWink, szText))
     return FALSE;
   // Now wait until playback has ended.
   EzRobbieMessageLoop();
   // Finally return to calling app
   return TRUE;
   }

BOOL RobbiePlayToonResourceEx(HWND hWnd, HINSTANCE ghInst, LPSTR szToonResource)
   {
   if(!RobbiePlayToonResourceEz(hWnd, ghInst, szToonResource))
     return FALSE;
   // Now wait until playback has ended.
   EzRobbieMessageLoop();
   // Finally return to calling app
   return TRUE;
   }

BOOL RobbiePlayToonFileEx(HWND hWnd, LPSTR szToonFile)
   {
   if(!RobbiePlayToonFileEz(hWnd, szToonFile))
     return FALSE;
   // Now wait until playback has ended.
   EzRobbieMessageLoop();
   // Finally return to calling app
   return TRUE;
   }

// ************ INTERRUPTING AND CONTINUING PLAYBACK ***************

BOOL IsRobbiePlaying(void)
   { // check all possible playing conditions
   // first check if we've interrupted playback ourselves.
   if(uiInterrupted>0) return FALSE;
   // if animation library reports it, then it's true
   if(Animating)       return TRUE;
   // if bubble wait is happening, return true
   if(!BubbleErased)   return TRUE;
   // otherwise we're not playing
   return FALSE;
   }


BOOL PauseRobbiePlayback(HWND hWnd)
   {
   HWND hPlayWin;

   if(IsEzPlaying)
      hPlayWin = ghEzWin;
   else
      hPlayWin = hWnd;

   if(IsRobbiePlaying())
      {
      if(BubbleErased)
         {
         anim_interrupt(hPlayWin);
         uiInterrupted = 1;
         }
      else
         {
         SetFrameBubbleTimer(hPlayWin, NULL, 0);
         uiInterrupted = 2;
         }
      return TRUE;
      }
   return FALSE;
   }

BOOL StopRobbiePlayback(HWND hWnd)
   {
   PauseRobbiePlayback(hWnd);

   if(IsEzPlaying)
       SendMessage(ghEzWin, WM_CLOSE, 0, 0);

   return TRUE;
   }

          
BOOL ContinueRobbiePlayback(HWND hWnd)
   {
   HWND hPlayWin;

   if(IsEzPlaying)
      hPlayWin = ghEzWin;
   else
      hPlayWin = hWnd;

   switch(uiInterrupted)
      {
      case 1:
         {
         uiInterrupted = 0;
         anim_continue(hPlayWin);
         }
      return TRUE;

      case 2:
         {
         uiInterrupted = 0;
         if(gbFrameWalking)
            {
            gbFrameOnscreen=FALSE;  // forces redisplay of text
            FramePlaybackTimer(hPlayWin);
            }
         else
            {
            bToonFilePlaying=TRUE;
            ContinueFilePlayback(hPlayWin);
            }
         }
      return TRUE;
      }
   return FALSE;
   }

   

// ******************* WINDOWS MESSAGES ****************************

void Robbie_EndFrame(HWND hWnd)
   {
   if(gbFrameWalking) 
      FramePlaybackEnd(hWnd);
   else               
      ContinueFilePlayback(hWnd);
   }

void Robbie_Timer(HWND hWnd, WPARAM wParam)
   {
   switch (wParam)
      {
      case ANIM_TIMER: // set when we're animating
           anim_timer(hWnd);
      break;
      case EXPRESN_TIMER: // set when we're making facial expressions
           DrawExpressionTimer(hWnd);
      break;
      case BUBSTART_TIMER: // set to wait 1sec before text.
           SetFrameBubbleTimer(hWnd, NULL, -1);
      break;
      case BUBBLEON_TIMER: // set when displaying text bubble
           if(gbFrameWalking)
              FramePlaybackTimer(hWnd);
           else
              FilePlaybackTimer(hWnd);
      break;
      }
   }

void Robbie_EndSound(HWND hWnd)
   {
   // close sound device, and draw last mouth position
   if(!BubbleErased)
      DrawPoseExpression(hWnd, EX_MOUTH03);

   StopExpressionTimer(hWnd);
   }

// Handles WM_PAINT
void Robbie_Paint(HDC hdc)
   {
   BITMAP bm;

   if(sprite_01.hbmImg_id!=0)
      {
      SaveDC(hdc);
      GetObject(sprite_01.hbmImg_id, sizeof(BITMAP), (void FAR*) &bm);

      if(wallyloaded)
         {
		 EraseSprite(hdc, &sprite_01);
         if(bRobbieSizing)
            {
            GetSprite(hdc, &sprite_01);
            bRobbieSizing = FALSE;
            }
         DrawSprite(hdc, &sprite_01);
         }
      RestoreDC(hdc, -1);
	  }
   }

// Handles WM_SIZE
void Robbie_Size(HWND hwnd, LPARAM lParam)
   {
   bRobbieSizing = TRUE;
   if(wallyloaded==TRUE)
      {
      dwWinSize=MAKELONG(LOWORD(lParam), HIWORD(lParam));
	  if(sprite_01.hbmImg_id != 0)
	     {
         HDC  hdc = GetDC(hwnd);
         InitSpriteInfo(hdc, sprite_01.hbmImg_id, &sprite_01);
         ReleaseDC(hwnd, hdc);
         InvalidateRect(hwnd, NULL, TRUE);
		 }
      }
   }

void FreeRobbieDLL(void)
   {
   if(ghEzWin) DestroyWindow(ghEzWin);
   FreeRobbieBitmaps();
   DeleteSprite(&sprite_01);
   FreeFrameMemory();
   }

/******************************************************************************\
*
*  FUNCTION:    DllMain
*
*  INPUTS:      hDLL       - handle of DLL
*               dwReason   - indicates why DLL called
*               lpReserved - reserved
*
*  RETURNS:     TRUE (always, in this example.)
*
*               Note that the retuRn value is used only when
*               dwReason = DLL_PROCESS_ATTACH.
*
*               Normally the function would return TRUE if DLL initial-
*               ization succeeded, or FALSE it it failed.
*
\******************************************************************************/

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
   {
   switch (dwReason)
     {
     case DLL_PROCESS_ATTACH:
          DisableThreadLibraryCalls(hDLL);
          srand((unsigned)time(NULL)); // initialize random generation
          LoadRobbieBitmaps(hDLL);
          ghInstance = hDLL;
          sprite_01.hbmImg_id = 0; // prevents initial sprite drawing
     break;

     case DLL_PROCESS_DETACH:
          UnregisterClass((LPCTSTR)"RobbieWinClass", hDLL);
          FreeRobbieDLL();
     break;
     }

   return TRUE;
   }



