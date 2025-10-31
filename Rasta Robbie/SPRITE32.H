/****************************************************************************\
*                                                                            *
* Sprites, by Slick Software 1995                                            *
*                                                                            *
* SPRITE32.C - This module handles all the sprite dragging and animation.     *
*                                                                            *
\****************************************************************************/

// Some header files...
#include <WINDOWS.H>

#ifndef RC_INVOKED
#include <MATH.H>
#include <STRING.H>
#include <TIME.H>
#include <STDLIB.H>
#include <STDIO.H>
#endif

/* System */

typedef struct
   {
   int     xPrev;         // Previous X location
   int     yPrev;         // Previous Y location
   int     bmX;           // Bitmap origin
   int     bmY;           // Bitmap origin
   int     bmWidth;       // Bitmap width
   int     bmHeight;      // Bitmap height
   BOOL    bSelected;     // Is ball selected
   HBITMAP hbmImg_id;     // Original bitmap
   HBITMAP hbmImage;      // Sprite's bitmap
   HBITMAP hbmBkg;        // What's behind our sprite
   } SPRITE;

/* SPRITE32.C */  
void InitSpriteInfo(HDC hdc, HBITMAP hbm, SPRITE *sprite_id);
void DeleteSprite(SPRITE *sprite_id);
void GetSprite(HDC hdc, SPRITE *sprite_id);
BOOL IsSpriteSelected(SPRITE *sprite_id, POINT *point);
void EraseSprite(HDC hdc, SPRITE *sprite_id);
void DrawSprite(HDC hdc, SPRITE *sprite_id);
void MoveSprite(HWND hWnd, SPRITE *sprite_id, POINT *point);
void BeginSpriteAnimation(HWND hWnd, SPRITE *sprite_id, POINT *point);
void FinishSpriteAnimation(HWND hWnd, SPRITE *sprite_id, POINT *point);
void AnimateSprite(HWND hWnd, SPRITE *sprite_id, POINT *ptFrom, POINT *ptTo);
void DrawSpriteBitmap(HDC hdc, HBITMAP hBitmap, POINT pt);
