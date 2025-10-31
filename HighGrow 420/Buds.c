/******************************************************************************\
*
*  PROGRAM:     Buds.c
*
*  PURPOSE:     handles setting, loading, drawing, and freeing of bud images
*
\******************************************************************************/

#include <windows.h>
#include <commctrl.h>
#include "stdio.h"
#include "global.h"

         
/******************************************************************************\
*  FUNCTIONS TO: Load and Free the Bud Imagelists
\******************************************************************************/

HIMAGELIST hBudImages[2]; // holds imagelist for males and females

void BDFreeBudImages(void)
    {
    ImageList_Destroy(hBudImages[0]);
    ImageList_Destroy(hBudImages[1]);
    }


void BDLoadBudImages(HINSTANCE hInst)
    { // Load 16 color bud images
    // create imagelists for the male and female buds
    hBudImages[0]= ImageList_LoadBitmap(hInst,"BDM",5,0,RGB(0,0,255));
    hBudImages[1]= ImageList_LoadBitmap(hInst,"BDF",5,0,RGB(0,0,255));
    }
                                   
/******************************************************************************\
*  FUNCTIONS TO: Draws Bud Images at each node's end point
\******************************************************************************/

void BDDrawBudImage(HDC hdc, POINT ptNode, BOOL bBudFemale, int iBudDays)
    {
    int i, x, iBudID, iBudType = 0;
    POINT ptBud;

    if(bBudFemale) // if we're a female plant...
        iBudType = 1; // change our array pointer
    // make a copy of the given point parameters
    ptBud.x = ptNode.x-3;
    ptBud.y = ptNode.y-5;
    x = 0;
    // ** Now draw the bud from the correct imagelist type
    for(i=0;i<iBudDays;i+=5)
        { // first choose a random bud image
        iBudID = Random(0,32);
        // now draw at the current node position
        ImageList_Draw(hBudImages[iBudType], iBudID, hdc, 
                       ptBud.x, ptBud.y, ILD_TRANSPARENT);
        // now adjust the node position for the next draw
        ptBud.x = ptNode.x += x;
        ptBud.y -= 2;
        // now we adjust the x value to stagger the buds horizotally
        if(x>=0) x = -(iBudDays/7);
        else     x =  (iBudDays/7);
        }
    }

