/****************************************************************************\
*                                                                            *
* Sprite32, by Slick Software 1995                                            *
*                                                                            *
* SPRITE32.C - This module handles all the sprite dragging and animation.     *
*                                                                            *
\****************************************************************************/

#include "sprite32.h"
 
// ****************************************************************
// DRAWING A TRANSPARENT BITMAP 
// ****************************************************************

void _stdcall BlitSpriteBitmap(HDC hdc, HBITMAP hBitmap, int Xpos, int Ypos, 
                               int Width, int Height, COLORREF cTransparent)
   {
   COLORREF   cColor;
   HBITMAP    bmAndBack,  bmAndMask,  bmAndCache,  bmSave;
   HBITMAP    bmPrevBack, bmPrevMask, bmPrevCache, bmPrevSave;
   HDC        hdcBack,    hdcMask,    hdcCache,    hdcSave,  hdcTemp; 

   hdcTemp = CreateCompatibleDC(hdc);
   SelectObject(hdcTemp, hBitmap);   // Select the bitmap
   // Create some DCs to hold temporary data.
   hdcBack   = CreateCompatibleDC(hdc);
   hdcMask   = CreateCompatibleDC(hdc);
   hdcCache  = CreateCompatibleDC(hdc);
   hdcSave   = CreateCompatibleDC(hdc);
   // Create a bitmap for each DC. DCs are required for a number of
   // GDI functions.
   // Monochrome DC
   bmAndBack  = CreateBitmap(Width, Height, 1, 1, NULL);
   bmAndMask  = CreateBitmap(Width, Height, 1, 1, NULL);
   bmAndCache = CreateCompatibleBitmap(hdc, Width, Height);
   bmSave     = CreateCompatibleBitmap(hdc, Width, Height);
   // Each DC must select a bitmap object to store pixel data.
   bmPrevBack  = SelectObject(hdcBack, bmAndBack);
   bmPrevMask  = SelectObject(hdcMask, bmAndMask);
   bmPrevCache = SelectObject(hdcCache,bmAndCache);
   bmPrevSave  = SelectObject(hdcSave, bmSave);
   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));
   // Save the bitmap sent here, because it will be overwritten.
   BitBlt(hdcSave, 0, 0, Width, Height, hdcTemp, 0, 0, SRCCOPY);
   // Set the background color of the source DC to the color.
   // contained in the parts of the bitmap that should be transparent
   cColor = SetBkColor(hdcTemp, cTransparent);
   // Create the object mask for the bitmap by performing a BitBlt
   // from the source bitmap to a monochrome bitmap.
   BitBlt(hdcMask, 0, 0, Width, Height, hdcTemp, 0, 0, SRCCOPY);
   // Set the background color of the source DC back to the original color.
   SetBkColor(hdcTemp, cColor);
   // Create the inverse of the object mask.
   BitBlt(hdcBack, 0, 0, Width, Height, hdcMask, 0, 0, NOTSRCCOPY);
   // Copy the background of the main DC to the destination.
   BitBlt(hdcCache, 0, 0, Width, Height, hdc, Xpos, Ypos, SRCCOPY);
   // Mask out the places where the bitmap will be placed.
   BitBlt(hdcCache, 0, 0, Width, Height, hdcMask, 0, 0, SRCAND);
   // Mask out the transparent colored pixels on the bitmap.
   BitBlt(hdcTemp, 0, 0, Width, Height, hdcBack, 0, 0, SRCAND);
   // XOR the bitmap with the background on the destination DC.
   BitBlt(hdcCache, 0, 0, Width, Height, hdcTemp, 0, 0, SRCPAINT);
   // Copy the destination to the screen.
   BitBlt(hdc, Xpos, Ypos, Width, Height, hdcCache, 0, 0, SRCCOPY);
   // Place the original bitmap back into the bitmap sent here.
   BitBlt(hdcTemp, 0, 0, Width, Height, hdcSave, 0, 0, SRCCOPY);
   // Delete the memory bitmaps.
   DeleteObject(SelectObject(hdcBack, bmPrevBack));
   DeleteObject(SelectObject(hdcMask, bmPrevMask));
   DeleteObject(SelectObject(hdcCache,bmPrevCache));
   DeleteObject(SelectObject(hdcSave, bmPrevSave));
   // Delete the memory DCs.
   DeleteDC(hdcCache);
   DeleteDC(hdcBack);
   DeleteDC(hdcMask);
   DeleteDC(hdcSave);
   DeleteDC(hdcTemp);
   }


// ****************************************************************
// SPRITE ANIMATION STUFF
// ****************************************************************


void InitSpriteInfo(HDC hdc, HBITMAP hbm, SPRITE *sprite_id)
   {
   HDC     hdcMem, hdcBlit;  // Handles to window and memory dcs
   HBITMAP hbmPrev, hbmBlit; // Handles to bitmaps
   BITMAP  bm;               // BITMAP data structure

   hdcMem  = CreateCompatibleDC(hdc);
   hdcBlit = CreateCompatibleDC(hdc);

   // Get width and height of bitmap
   GetObject(hbm, sizeof(BITMAP), (LPSTR)&bm);

   // Initialize sprite's info and store rect for updating
   sprite_id->bmX = sprite_id->xPrev;
   sprite_id->bmY = sprite_id->yPrev;

   sprite_id->bmWidth  = bm.bmWidth;
   sprite_id->bmHeight = bm.bmHeight;

   if(sprite_id->hbmImage)
      DeleteObject(sprite_id->hbmImage);
   
   hbmBlit = CreateCompatibleBitmap(hdc, sprite_id->bmWidth, 
                                         sprite_id->bmHeight);

   hbmPrev = SelectObject(hdcMem, hbmBlit);
   SelectObject(hdcBlit, hbm);

   SetStretchBltMode(hdcMem, BLACKONWHITE);

   StretchBlt(hdcMem, 0, 0, sprite_id->bmWidth, sprite_id->bmHeight, 
              hdcBlit, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

   sprite_id->hbmImage = hbmBlit;

   // Tidy up
   SelectObject(hdcMem, hbmPrev);
   SelectObject(hdcBlit, hbmPrev); 

   DeleteDC(hdcMem);
   DeleteDC(hdcBlit);
   }



void DeleteSprite(SPRITE *sprite_id)
   {
   if(sprite_id->hbmImg_id)
      DeleteObject(sprite_id->hbmImg_id);
   if(sprite_id->hbmImage)
      DeleteObject(sprite_id->hbmImage);
   if(sprite_id->hbmBkg)
      DeleteObject(sprite_id->hbmBkg);
   
   sprite_id->hbmImg_id=0;
   sprite_id->hbmImage=0;
   sprite_id->hbmBkg=0;
   }



void EraseSprite(HDC hdc, SPRITE *sprite_id)
   {
   HDC     hdcMem;
   HBITMAP hbmPrev;

   hdcMem = CreateCompatibleDC(hdc);

   // Select bitmap
   hbmPrev = SelectObject(hdcMem, sprite_id->hbmBkg);
    
   // Draw the background in the window
   BitBlt(hdc, sprite_id->bmX, sprite_id->bmY, sprite_id->bmWidth,
      sprite_id->bmHeight, hdcMem, 0, 0, SRCCOPY);

   // Tidy up
   SelectObject(hdcMem, hbmPrev);
   DeleteDC(hdcMem);
   }



void DrawSprite(HDC hdc, SPRITE *sprite_id)
   {
   BlitSpriteBitmap(hdc, sprite_id->hbmImage, sprite_id->bmX, sprite_id->bmY, 
                         sprite_id->bmWidth, sprite_id->bmHeight, RGB(255,0,255));
   }




void GetSprite(HDC hdc, SPRITE *sprite_id)
   {
   HDC hdcMem;
   HBITMAP hbmPrev, hbmNew;

   hdcMem = CreateCompatibleDC(hdc);

   if(sprite_id->hbmBkg)
      DeleteObject(sprite_id->hbmBkg);

//   if(hbmNew)
//      DeleteObject(hbmNew);

   // Create and select a new bitmap to store our background
   hbmNew  = CreateCompatibleBitmap(hdc, sprite_id->bmWidth, 
                                         sprite_id->bmHeight);
   hbmPrev = SelectObject(hdcMem, hbmNew);

   // Get the background from the screen
   BitBlt(hdcMem, 0, 0, sprite_id->bmWidth, sprite_id->bmHeight, hdc,
      sprite_id->bmX, sprite_id->bmY, SRCCOPY);

   sprite_id->hbmBkg = hbmNew;

   // Tidy up
   SelectObject(hdcMem, hbmPrev);
   
   DeleteDC(hdcMem);
   }



BOOL IsSpriteSelected(SPRITE *sprite_id, POINT *point)
   {
   RECT  rect;    // RECT data structure
   BOOL  ret_val;
   POINT pt;      // POINT data structure2
   
   // Current bitmap position
   rect.left   = (int)(sprite_id->bmX);
   rect.top    = (int)(sprite_id->bmY);
   rect.right  = (int)(sprite_id->bmX + sprite_id->bmWidth);
   rect.bottom = (int)(sprite_id->bmY + sprite_id->bmHeight);

   pt.x = point->x;
   pt.y = point->y;
   
   ret_val = PtInRect(&rect, pt);

   // Return TRUE if pt in rect of sprite
   return ret_val;
   }



void MoveSprite(HWND hWnd, SPRITE *sprite_id, POINT *point)
   {
   COLORREF rgbBk;
   HDC hdc, hdcTemp, hdcNewBkg, hdcOldBkg, hdcMask, hdcCache, hdcBack; 
   HBITMAP hbmNew,  hbmNPrev, hbmOPrev, hbmPrev, hbmTemp, hbmMask,
           hbmBack, hbmPrevMask, hbmCache, hbmPrevCache, hbmPrevBack;
   int dx, dy, Xpos, Ypos, Width, Height;

   // Calculate delta x and delta y
   dx = sprite_id->xPrev - point->x;
   dy = sprite_id->yPrev - point->y;
   // Save previous mouse position
   sprite_id->xPrev = point->x;
   sprite_id->yPrev = point->y;
   // Update sprite's position
   sprite_id->bmX -= dx;
   sprite_id->bmY -= dy;
   // Set some often used values
   Xpos   = sprite_id->bmX;
   Ypos   = sprite_id->bmY;
   Width  = sprite_id->bmWidth;
   Height = sprite_id->bmHeight;
      
   // Get window DC
   hdc = GetDC(hWnd);
   hdcNewBkg = CreateCompatibleDC(hdc);
   hdcOldBkg = CreateCompatibleDC(hdc);
   hdcBack   = CreateCompatibleDC(hdc);
   hdcTemp   = CreateCompatibleDC(hdc);
   hdcMask   = CreateCompatibleDC(hdc);
   hdcCache  = CreateCompatibleDC(hdc);
   // Create a temp bitmap for our new background
   hbmMask  = CreateBitmap(Width, Height, 1, 1, NULL);
   hbmBack  = CreateBitmap(Width, Height, 1, 1, NULL);
   hbmNew   = CreateCompatibleBitmap(hdc, Width, Height);
   hbmCache = CreateCompatibleBitmap(hdc, Width, Height);
   // Select our bitmaps
   hbmPrevBack  = SelectObject(hdcBack,   hbmBack);
   hbmPrev      = SelectObject(hdcTemp,   sprite_id->hbmImage);
   hbmOPrev     = SelectObject(hdcOldBkg, sprite_id->hbmBkg);
   hbmNPrev     = SelectObject(hdcNewBkg, hbmNew);
   hbmPrevMask  = SelectObject(hdcMask,   hbmMask);
   hbmPrevCache = SelectObject(hdcCache,  hbmCache);

   // Set proper mapping mode.
   SetMapMode(hdcTemp, GetMapMode(hdc));
   rgbBk = SetBkColor(hdcTemp, RGB(255, 0, 255));
   // Create the bitmap mask & set the inverse of the mask
   BitBlt(hdcMask, 0, 0, Width, Height, hdcTemp, 0, 0, SRCCOPY);
   BitBlt(hdcBack, 0, 0, Width, Height, hdcMask, 0, 0, NOTSRCCOPY);
   SetBkColor(hdcTemp, rgbBk);

   // Copy screen to new background
   BitBlt(hdcNewBkg, 0, 0, Width, Height, hdc, Xpos, Ypos, SRCCOPY);
   // Replace part of new bkg with old background
   BitBlt(hdcNewBkg, dx, dy, Width, Height, hdcOldBkg, 0, 0, SRCCOPY);
   // Copy sprite to old background 
   BitBlt(hdcOldBkg, -dx, -dy, Width, Height, hdcMask, 0, 0, SRCAND);
   BitBlt(hdcTemp,    0,    0, Width, Height, hdcBack, 0, 0, SRCAND);
   BitBlt(hdcOldBkg, -dx, -dy, Width, Height, hdcTemp, 0, 0, SRCPAINT);
   // Copy sprite to screen without flicker
   BitBlt(hdcCache, 0, 0, Width, Height, hdcNewBkg, 0, 0, SRCCOPY);
   BitBlt(hdcCache, 0, 0, Width, Height, hdcMask,   0, 0, SRCAND);
   BitBlt(hdcCache, 0, 0, Width, Height, hdcTemp,   0, 0, SRCPAINT);
   BitBlt(hdc, Xpos, Ypos, Width, Height, hdcCache, 0, 0, SRCCOPY);
   // Copy old background to screen
   BitBlt(hdc, Xpos+dx, Ypos+dy, Width, Height, hdcOldBkg, 0, 0, SRCCOPY);

   // Tidy up
   SelectObject(hdcTemp,   hbmPrev);
   SelectObject(hdcBack,   hbmPrevBack);
   SelectObject(hdcMask,   hbmPrevMask);
   SelectObject(hdcCache,  hbmPrevCache);
   SelectObject(hdcNewBkg, hbmNPrev);
   SelectObject(hdcOldBkg, hbmOPrev);
   // Swap old with new background
   hbmTemp = sprite_id->hbmBkg;
   sprite_id->hbmBkg = hbmNew;
   hbmNew = hbmTemp;
   // Free resources
   DeleteObject(hbmNew);
   DeleteObject(hbmBack);
   DeleteObject(hbmMask);
   DeleteObject(hbmCache);
   // Tidy up some more
   DeleteDC(hdcTemp);
   DeleteDC(hdcBack);
   DeleteDC(hdcMask);
   DeleteDC(hdcCache);
   DeleteDC(hdcNewBkg);
   DeleteDC(hdcOldBkg);
   ReleaseDC(hWnd, hdc);
   }




void BeginSpriteAnimation(HWND hWnd, SPRITE *sprite_id, POINT *point)
   {
   // Get all mouse messages
   SetCapture(hWnd);

   // Save previous mouse position
   sprite_id->xPrev = point->x;
   sprite_id->yPrev = point->y; 
   
   }



void FinishSpriteAnimation(HWND hWnd, SPRITE *sprite_id, POINT *point)
   {
   POINT   pt, ptend;
   int     step_factor = 10;      

   step_factor = 11;
      
   if(sprite_id->bmWidth > 50)
      step_factor += 10;
   else 
      if(sprite_id->bmHeight > 50)
         step_factor +=10;
        
      {
      float slope, b, x_test;

      double length;

      int optimize;
       
      pt.x     = point->x;
      pt.y     = point->y;
      point->x = sprite_id->xPrev;
      point->y = sprite_id->yPrev;
      
      if(point->x == pt.x)
         MoveSprite(hWnd, sprite_id, &pt);
      
      else
         {
         if(point->x < pt.x)
            {
/* x1 = point->x,    y1 = point->y    = current location   */
/* x2 = pt.x,        y2 = pt.y        = destination point  */

            slope = ((pt.y - point->y) / (float)(max((pt.x - point->x), 1.0)));
            b = (point->y - (slope * point->x));

            length = max((sqrt(pow(pt.x - point->x, 2)
               + pow(pt.y - point->y, 2)) / step_factor), 1.0);

            optimize = pt.x - point->x;
            
            for(x_test = (float)point->x; x_test < pt.x;
               x_test += (float)(optimize / length))
               {
               ptend.x = (int)x_test;
               ptend.y = (int)((slope * x_test) + b);
               
               MoveSprite(hWnd, sprite_id, &ptend);
               }
            }
         else
            {
/* x1 = pt.x,        y1 = pt.y     = hole going towards */
/* x2 = point->x,    y2 = point->y = current location   */

            slope = ((point->y - pt.y) / (float)(max((point->x - pt.x), 1.0)));
            b = (point->y - (slope * point->x));

            length = max((sqrt(pow(point->x - pt.x, 2)
               + pow(point->y - pt.y, 2)) / step_factor), 1.0);

            optimize = point->x - pt.x;

            for(x_test = (float)point->x; x_test > pt.x;
               x_test -= (float)(optimize / length))
               {
               ptend.x = (int)x_test;
               ptend.y = (int)((slope * x_test) + b);
               
               MoveSprite(hWnd, sprite_id, &ptend);
               }
            }
         }   
      }  
      
   // Reset previous mouse position
   sprite_id->xPrev = sprite_id->bmX;
   sprite_id->yPrev = sprite_id->bmY;

   // Release mouse capture
   ReleaseCapture();

   }


void AnimateSprite(HWND hWnd, SPRITE *sprite_id, POINT *ptFrom, POINT *ptTo)
   {
   BeginSpriteAnimation(hWnd, sprite_id, ptFrom);
   FinishSpriteAnimation(hWnd, sprite_id, ptTo);
   }



// *** A SLOW BUT GOOD SPRITE BITMAP DRAWER ***   

void DrawSpriteBitmap(HDC hdc, HBITMAP hBitmap, POINT pt)
   {
   SPRITE sprite_temp;

   // initialize important elements
   sprite_temp.hbmImage = 0;
   sprite_temp.hbmBkg   = 0;
   // set the parametered memebers
   sprite_temp.xPrev     = pt.x;
   sprite_temp.yPrev     = pt.y;
   sprite_temp.hbmImg_id = hBitmap;
   // initialise the sprite
   InitSpriteInfo(hdc, sprite_temp.hbmImg_id, &sprite_temp);
   GetSprite(hdc, &sprite_temp);
   // and then draw it
   DrawSprite(hdc, &sprite_temp);
   // now free everything
   DeleteObject(sprite_temp.hbmImage);
   DeleteObject(sprite_temp.hbmBkg);
   }

