#include "sprite32.h"
#include "bubble.h"

SPRITE sprite_bubble;
SPRITE sprite_smbubble;

HBITMAP hBubbleLeft;
HBITMAP hBubbleRight;
HBITMAP hSmBubbleLeft;
HBITMAP hSmBubbleRight;

UINT    uBubbleDirection; // 1=left_to_right, 2=right_to_left

// **************** BUBBLE TEXT STUFF **********************
 
void InitBubbleSprite(HWND hWnd, POINT ptMouth)
   {
   HDC hDC;
   
   if(uBubbleDirection==1)
      {
      sprite_bubble.hbmImg_id = hBubbleLeft;
      sprite_bubble.xPrev     = ptMouth.x+15;
      sprite_smbubble.hbmImg_id = hSmBubbleLeft;
      sprite_smbubble.xPrev     = ptMouth.x+15;
      }
   else 
      {
      sprite_bubble.hbmImg_id = hBubbleRight;
      sprite_bubble.xPrev     = ptMouth.x-15-sprite_bubble.bmWidth;
      sprite_smbubble.hbmImg_id = hSmBubbleRight;
      sprite_smbubble.xPrev     = ptMouth.x-15-sprite_smbubble.bmWidth;
      }
      
   sprite_bubble.yPrev = ptMouth.y-50;
   sprite_bubble.bmX   = sprite_bubble.xPrev; 
   sprite_bubble.bmY   = sprite_bubble.yPrev; 
   sprite_smbubble.yPrev = ptMouth.y-35;
   sprite_smbubble.bmX   = sprite_smbubble.xPrev; 
   sprite_smbubble.bmY   = sprite_smbubble.yPrev; 
  
   hDC = GetDC(hWnd);
   InitSpriteInfo(hDC, sprite_bubble.hbmImg_id,   &sprite_bubble);
   InitSpriteInfo(hDC, sprite_smbubble.hbmImg_id, &sprite_smbubble);
   ReleaseDC(hWnd, hDC);
   }
    
void LoadBubbleBitmaps(HWND hWnd, HINSTANCE ghInst)
   {
   POINT ptDud;
   ptDud.x = 0;
   ptDud.y = 0;
   hBubbleRight  = LoadBitmap(ghInst, (LPSTR)"bubright");
   hBubbleLeft   = LoadBitmap(ghInst, (LPSTR)"bubleft");
   hSmBubbleRight= LoadBitmap(ghInst, (LPSTR)"smbubright");
   hSmBubbleLeft = LoadBitmap(ghInst, (LPSTR)"smbubleft");
   InitBubbleSprite(hWnd, ptDud);
   }

void FreeBubbleBitmaps(void)
   {
   if(hBubbleLeft)    DeleteObject(hBubbleLeft);
   if(hBubbleRight)   DeleteObject(hBubbleRight);
   if(hSmBubbleLeft)  DeleteObject(hSmBubbleLeft);
   if(hSmBubbleRight) DeleteObject(hSmBubbleRight);
   hBubbleLeft=0;
   hBubbleRight=0;
   hSmBubbleLeft=0;
   hSmBubbleRight=0;
   DeleteSprite(&sprite_bubble);
   DeleteSprite(&sprite_smbubble);
   }


void SetBubblePos(HWND hWnd, POINT ptMouth, UINT uPrefDir)
   { // bubble pos will decide on which side to show bubble, BUT
     // we will use the prefdir parameter as the preferred bubble 
     // direction: uPrefDir=1=bubleft, uPrefDir=2=bubright
   RECT rc;
   GetClientRect(hWnd, &rc);

   // change direction if within a quarter away from either side
   if(uPrefDir==1)
      {
      if(ptMouth.x>(3*((rc.right-rc.left)/4))) 
         uPrefDir = 2;
      }
   else
      { 
      if(ptMouth.x<(rc.right-rc.left)/4)       
         uPrefDir = 1;
      }
      
   // change direction if bubble will NOT fit on screen
   if(uPrefDir==1)
      {
      if((ptMouth.x+sprite_bubble.bmWidth)>(rc.right-rc.left)) 
         uPrefDir = 2;
      }
   else
      { 
      if((ptMouth.x-sprite_bubble.bmWidth)<0)                  
         uPrefDir = 1;
      }
      
   // now set our global variable and init the sprite's info
   uBubbleDirection   = uPrefDir;
   InitBubbleSprite(hWnd, ptMouth);
   }


int BubType; // 1=large, 2=small

BOOL bubble_show(HWND hWnd, LPCSTR szBubbleText)
   {
   RECT   rcTemp1, rcTemp2, rcText;
   BITMAP bm;
   BOOL   bFlag;
   HFONT  hfont, hfontOld;
   int    PrevMapMode, iPtSize = 14;
   HDC    hdc=GetDC(hWnd);
   
   GetSprite(hdc, &sprite_bubble);
   GetSprite(hdc, &sprite_smbubble);

   GetObject(sprite_bubble.hbmImg_id, sizeof(BITMAP), (void*) &bm);
   
   if(uBubbleDirection==1)
      rcText.left = sprite_bubble.bmX+10;
   else 
      rcText.left = sprite_bubble.bmX-10;

   rcText.top     = sprite_bubble.bmY+12;
   rcText.right   = rcText.left+bm.bmWidth; 
   rcText.bottom  = rcText.top+bm.bmHeight; 
   InflateRect(&rcText, -20, -12);

   rcTemp1.left  = rcText.left;
   rcTemp1.right = rcText.right;
   rcTemp2.left  = rcText.left;
   rcTemp2.right = rcText.right;
   
   PrevMapMode = SetMapMode(hdc, MM_TEXT);
   hfont    = CreateFont(-iPtSize, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "MS Serif");         
   hfontOld = SelectObject(hdc, hfont);

   // then we calculate the rect requd for 1 character of text
   DrawText(hdc, szBubbleText, 1, &rcTemp1, DT_CALCRECT);
   // then we calculate the rect requd for all the text   
   DrawText(hdc, szBubbleText, -1, &rcTemp2,
	        DT_CENTER|DT_WORDBREAK|DT_NOPREFIX|DT_CALCRECT);

   // now if we have more than two lines of text,
   // we're going to be using the large bubble bitmap
   if((rcTemp2.bottom-rcTemp2.top)/(rcTemp1.bottom-rcTemp1.top)<3)
     {
	 BubType = 2;
     DrawSprite(hdc, &sprite_smbubble);
	 rcText.top    +=13;
	 rcText.bottom +=13;
     }
   else 
     { // if we somehow got 4 lines of text, we move the top up a little
     if((rcTemp2.bottom-rcTemp2.top)/(rcTemp1.bottom-rcTemp1.top)==4)
    	 rcText.top    -=4;
     BubType = 1;
     DrawSprite(hdc, &sprite_bubble);
     }
   
   bFlag = DrawText(hdc, szBubbleText, -1, &rcText, 
	                DT_CENTER|DT_WORDBREAK|DT_NOPREFIX);

   SetMapMode(hdc, PrevMapMode);
   SelectObject(hdc, hfontOld);
   DeleteObject(hfont);
   ReleaseDC(hWnd, hdc);

   return bFlag;
   }   

BOOL bubble_hide(HWND hWnd)
   {
   HDC hdc;
   hdc = GetDC(hWnd);

   if(BubType==1)
      EraseSprite(hdc, &sprite_bubble);
   else
      EraseSprite(hdc, &sprite_smbubble);

   ReleaseDC(hWnd, hdc);

   return 0;
   }
