#include "expresns.h"
#include "sprite32.h"
#include "robbie.h" // we've defined our TOONSTEP struct there
#include "toonfile.h"
#include "messages.h"
#include "playsnd.h"

// **************************************************************
// *****************WALLY'S FACIAL EXPRESSIONS*******************
// **************************************************************

int MouthExprInfo[iMaxPoses*2] // holds x and y positions for all Mouth Expressions
        ={57,32,62,34,56,32,55,34,55,32,59,31,60,32,64,31,
          60,32,57,36,56,39,58,36,58,37,60,37,59,36,58,36,
          55,35,57,39,58,37,57,35,56,35,57,39,58,38,58,34};

int EyeExprInfo[iMaxPoses*2]   // holds x and y positions for all Eye Expressions
        ={53,17,58,14,56,16,59,14,55,15,63,16,57,17,64,15,
          56,19,63,19,58,20,54,17,63,17,57,19,64,18,54,18,
          53,19,51,20,53,17,58,14,54,18,49,22,54,19,53,18};

HBITMAP  hFacialExprBmp; // holds bitmap to wally's facial expressions

BOOL DrawFacialExpression(HWND hWnd, POINT ptExpr, int iExpr)
   { // draws the facial expression required from the expressions bitmap.
   // NOTE: the FileToonStep structure MUST hold the iPose element before calling this.
   // ptExpr points to the start of the 10x10 pixel bitmap destination within the sprite.
   // iExpr is between 1 and 4, pointing to the current pose's expression bitmap offset.
   // if iExpr is zero, then do nothing!
   POINT  ptBmp, ptSize;    // x and y offset into expressions bitmap & Bitmap size
   HDC    hDC, hdcMem;
   HANDLE hPrevObject;

   if(!iExpr) return FALSE;

   ptExpr.x += sprite_01.bmX;
   ptExpr.y += sprite_01.bmY;
   ptSize.x = 10;
   ptSize.y = 10;
   ptBmp.x  = 11*(iExpr-1);

   if(gbFrameWalking)
      ptBmp.y = 11*(FrameToonStep.itPose);
   else
      ptBmp.y = 11*(FileToonStep.itPose);

   hDC    = GetDC(hWnd);
   hdcMem = CreateCompatibleDC(hDC);

   hPrevObject = SelectObject(hdcMem, hFacialExprBmp);
   SetMapMode(hdcMem, GetMapMode(hDC));
   
   DPtoLP(hDC,    &ptExpr, 1);
   DPtoLP(hDC,    &ptSize, 1);
   DPtoLP(hdcMem, &ptBmp,  1);
   
   BitBlt(hDC, ptExpr.x, ptExpr.y, ptSize.x, ptSize.y, 
         hdcMem, ptBmp.x, ptBmp.y, SRCCOPY);
         
   SelectObject(hdcMem, hPrevObject);
   DeleteDC(hdcMem);
   ReleaseDC(hWnd, hDC);

   return TRUE;
   }




BOOL DrawPoseExpression(HWND hWnd, int iExpr)
   { // draws the required facial expression for the current pose.
   // iExpr is between 1 and 4, pointing to the current pose's expression bitmap offset.
   // NOTE: the FileToonStep structure must be filled before calling this.
   POINT ptExpr;
   int   iPoseNum;  // this one's correct

   if(gbFrameWalking)
      {
      if((iExpr<EX_MOUTH01)&(!FrameToonStep.btWink))  return FALSE;
      iPoseNum = FrameToonStep.itPose;
      }
   else
      {
      if((iExpr<EX_MOUTH01)&(!FileToonStep.btWink))   return FALSE;
      iPoseNum = FileToonStep.itPose;
      }

   if(iExpr<EX_MOUTH01)
     {
     ptExpr.x = EyeExprInfo[iPoseNum*2];
     ptExpr.y = EyeExprInfo[(iPoseNum*2)+1];
     }
   else
     {
     ptExpr.x = MouthExprInfo[iPoseNum*2];
     ptExpr.y = MouthExprInfo[(iPoseNum*2)+1];
     }
   return DrawFacialExpression(hWnd, ptExpr, iExpr);
   }

// *************** HANDLING THE TALKING EXPRESSION TIMER *******************
BOOL bExpressing=FALSE;
int  iRobbieWords;
int  iCurWord;
int  iPrevExpr;

void StopExpressionTimer(HWND hWnd)
   {
   if(bExpressing)
      KillTimer(hWnd, (UINT)EXPRESN_TIMER);

   CloseSoundDevice();
   bExpressing = FALSE;
   }


void DrawExpressionTimer(HWND hWnd)
   {
   int iExpr;

   iCurWord += 1;

   if(iCurWord==iRobbieWords)
      {
      DrawPoseExpression(hWnd, EX_MOUTH03);
      StopExpressionTimer(hWnd);
      }
   else
      {
      iExpr = Random(EX_MOUTH01, EX_MOUTH03);
      while(iExpr==iPrevExpr)
            iExpr = Random(EX_MOUTH01, EX_MOUTH03);

      DrawPoseExpression(hWnd, iExpr);
      iPrevExpr=iExpr;
      }
   }


BOOL StartExpressionTimer(HWND hWnd)
   { // uiSecs gives intended wait time

   if(bExpressing) return FALSE;
   
   iCurWord    = 0;
   if(gbFrameWalking)
      {
      if(FrameToonStep.sztText[0]==0) return FALSE;
      iRobbieWords = strlen(FrameToonStep.sztText+1)/4;
      }
   else
      {
      if(FileToonStep.sztText[0]==0) return FALSE;
      iRobbieWords = strlen(FileToonStep.sztText+1)/4;
      }

   SetTimer(hWnd, (UINT)EXPRESN_TIMER, 250, NULL); 
   bExpressing = TRUE;
   return bExpressing;
   }

BOOL StartTalkExpressionTimer(HWND hWnd)
   {
   if(bExpressing) return FALSE;
   bExpressing = TRUE;
   SetTimer(hWnd, (UINT)EXPRESN_TIMER, 250, NULL); 
   // set word count to a ridiculously high value. 
   // We stop expression timer with a MM_MCINOTIFY
   iRobbieWords = 1000; 
   iCurWord    = 0;
   return bExpressing;
   }

