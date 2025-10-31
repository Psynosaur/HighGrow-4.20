/******************************************************************************\
*
*  PROGRAM:     Leaves.c
*
*  PURPOSE:     handles setting, loading, drawing, and freeing of leaf images
*
\******************************************************************************/

#include <windows.h>
#include <commctrl.h>
#include "stdio.h"
#include "global.h"
#include "dibitmap.h"

/******************************************************************************\
*  FUNCTIONS TO: Load and Free the Leaf Imagelists
\******************************************************************************/

/*

HIMAGELIST hLeafImages[10]; // holds imagelist for 4 left, 4 right. and 2 tiny

void LFFreeLeafImages(void)
    {
    int i;
    for(i=0;i<10;i++)
        ImageList_Destroy(hLeafImages[i]);
    }


void LFLoadLeafImages(HINSTANCE hInst)
    { // Load 16 color leaf images
    COLORREF crMaskWhite = RGB(255,255,255);
    // INIT THE COMMON CONTROLS
    InitCommonControls();
    // FIRST CREATE THE IMAGE LISTS
    // create imagelists for the tiny leaves
    hLeafImages[0]= ImageList_LoadBitmap(hInst,"LFEU",16,0,crMaskWhite);
    hLeafImages[1]= ImageList_LoadBitmap(hInst,"LFED",16,0,crMaskWhite);
    // create imagelists for the small leaves
    hLeafImages[2]= ImageList_LoadBitmap(hInst,"LFLD",24,0,crMaskWhite);
    hLeafImages[3]= ImageList_LoadBitmap(hInst,"LFRD",24,0,crMaskWhite);
    // create imagelists for the medium leaves
    hLeafImages[4]= ImageList_LoadBitmap(hInst,"LFLC",30,0,crMaskWhite);
    hLeafImages[5]= ImageList_LoadBitmap(hInst,"LFRC",30,0,crMaskWhite);
    // create imagelists for the big leaves
    hLeafImages[6]= ImageList_LoadBitmap(hInst,"LFLB",45,0,crMaskWhite);
    hLeafImages[7]= ImageList_LoadBitmap(hInst,"LFRB",45,0,crMaskWhite);
    // create imagelists for the really big leaves
    hLeafImages[8]= ImageList_LoadBitmap(hInst,"LFLA",60,0,crMaskWhite);
    hLeafImages[9]= ImageList_LoadBitmap(hInst,"LFRA",60,0,crMaskWhite);
    }
*/


/******************************************************************************\
*  FUNCTIONS TO: Load and Free the Leaf Bitmaps
\******************************************************************************/

HBITMAP    hLeafBitmap[8];  // holds imagelist for 4 left, 4 right. and 2 tiny
HPALETTE   hLeafPalette[8]; // holds imagelist for 4 left, 4 right. and 2 tiny
HIMAGELIST hLeafImages[2]; // holds imagelist for 2 tiny 16 colour images

void LFFreeLeafImages(void)
    {
    int i;
    for(i=0;i<8;i++)
        {
        if(hLeafBitmap[i]) DIFreeBitmap(hLeafBitmap[i], hLeafPalette[i]);
        }
    if(hLeafImages[0])  ImageList_Destroy(hLeafImages[0]);
    if(hLeafImages[1])  ImageList_Destroy(hLeafImages[1]);
    }

void LFLoadLeafImages(HINSTANCE hInst)
    { // Load 256 color leaf images
    hLeafImages[0]= ImageList_LoadBitmap(hInst,"LFEU",16,0,RGB(255,255,255));
    hLeafImages[1]= ImageList_LoadBitmap(hInst,"LFED",16,0,RGB(255,255,255));
    // load bitmaps for the small leaves
    hLeafBitmap[0]= DILoadResourceBitmap(hInst,"LFLD",&hLeafPalette[0]);
    hLeafBitmap[1]= DILoadResourceBitmap(hInst,"LFRD",&hLeafPalette[1]);
    // load bitmaps for the medium leaves
    hLeafBitmap[2]= DILoadResourceBitmap(hInst,"LFLC",&hLeafPalette[2]);
    hLeafBitmap[3]= DILoadResourceBitmap(hInst,"LFRC",&hLeafPalette[3]);
    // load bitmaps for the big leaves
    hLeafBitmap[4]= DILoadResourceBitmap(hInst,"LFLB",&hLeafPalette[4]);
    hLeafBitmap[5]= DILoadResourceBitmap(hInst,"LFRB",&hLeafPalette[5]);
    // load bitmaps for the really big leaves
    hLeafBitmap[6]= DILoadResourceBitmap(hInst,"LFLA",&hLeafPalette[6]);
    hLeafBitmap[7]= DILoadResourceBitmap(hInst,"LFRA",&hLeafPalette[7]);
    }
                                   
/******************************************************************************\
*  FUNCTIONS TO: Set Leaf Image type for each node
\******************************************************************************/

void LFCalcNodeLeafTypes(PNODE noPlant, int iNodes, int iGStage)
    { 
    int  i, j;  // percentage of total nodes
    int  iType, iID, iImage;

    for(i=0;i<iNodes;i++)
        { 
        if(noPlant[i].cNodeLeafType != -1) // if we haven't lost this leaf?
            {
            // first we set the leaf type based on the growthstage and total
            // number of nodes.
            if((noPlant[i].bGrowingTip)|
              ((iNodes<4)&(giLastPruned==0))) // growing tip node?
                {iType = 1; goto checkit;} // set to tiny 

            j = ((i*100)/iNodes);
//            wsprintf(strbuf, "i = %i, iNodes = %i, j = %i", i, iNodes, j);
//            MessageBox(NULL, strbuf, "Message", MB_APPLMODAL | MB_OK);
            if(iGStage==2) // if we're seedling
                {
                if((j>=10)|(i<3))
                     {iType = 2;} // tiny
                else {iType = 3;} // small
                goto checkit;
                }
            if(iGStage==3) // if we're vegetating
                {
                if(j>=45) {iType = 2; goto checkit;} // tiny
                if((j>=30)|(giGrowDay<41)) 
                          {iType = 3; goto checkit;} // small
                if((j>=10)|(giGrowDay<56)) 
                          {iType = 4; goto checkit;} // medium
                else      {iType = 5; goto checkit;} // big
                }
            if(iGStage>=4) // preflowering or later
                {
                if(j>=55) {iType = 2; goto checkit;} // tiny
                if(j>=40) {iType = 3; goto checkit;} // small
                if(j>=25) {iType = 4; goto checkit;} // medium
                if(j>=10) {iType = 5; goto checkit;} // big
                else      {iType = 6; goto checkit;} // really big
                }
checkit:
            // now, if the type has changed, we must select a random leaf ID
            // from our leaf imagelists
            // first we must convert the Type for left/right duality
            if(iType <= 2) iImage = iType;  // tiny (up or down) is always tiny
            else  // if not a tiny leaftype, we must check it's growth direction
                { // ...and select from the left or right imagelists
                // 2 Images/LeafType  (from 2 to 10)
                iImage  = (((iType-2)*2)+1); 
                // growing left? use left imagelists
                if(noPlant[i].cNodeAngle>=0) 
                    { // if node% of total nodes is NOT divisable by 7, we change
                    if(j-((int)(j/7)*7)!=0) iImage += 1; 
                    }
                else
                    { // if node% of total nodes IS divisable by 7, we change
                    if(j-((int)(j/7)*7)==0) iImage += 1; 
                    }
                }
            // if the leaf type changed, we must calculate a new leaf ID
           if(noPlant[i].cNodeLeafType != iImage) 
                { // set a new leaf number for this leaf type
                if(iType==1) {iID = Random(0,29);} // tiny 2leaf up
                if(iType==2) {iID = Random(0,29);} // tiny down
                if(iType==3) {iID = Random(0,19);} // small
                if(iType==4) {iID = Random(0,19);} // medium
                if(iType==5) {iID = Random(0,9);} // big
                if(iType==6) {iID = Random(0,4);} // really big
                // this is done for speed?
                noPlant[i].cNodeLeafID   = iID;
                noPlant[i].cNodeLeafType = iImage;
                }
            }
        }
    }

/******************************************************************************\
*  FUNCTIONS TO: Set Lose leaves based on health decline
\******************************************************************************/

// 0 = Seed           1 = Germination   2 = Seedling   3 = Vegetative
// 4 = Preflowering   5 = Flowering     6 = Seed Set   7 = Dying

void LFCalcLeafLoss(PPLANT plPlant, int iNodes, int iGStage)
    { // here we check our plant's health and possibly lose a leaf from 
    // a random node on this plant
    // **NOTE** giGrowDay must be set
    int i, iNode;

    // getout if we've already visited today.
    if(plPlant->GD_Plant[giGrowDay].bVisited == TRUE) return;
    if(iGStage<3) return; // if we're still a seedling, we won't lose leaves
    if(iNodes<25) return; // we chose a random leaf from zero to iNodes-20

    // if this plant's health has reached zero, we'll show dead leaves
    if((plPlant->GD_Plant[giGrowDay].cHealth==0)&&(gbGodMode==0))
        { // unless he's in god mode, set every leafed node to dead
        for(i=0;i<iNodes;i++)
            { // if we have a leaf at this node, kill it
            if(plPlant->NO_Plant[i].cNodeLeafType != -1) 
                plPlant->NO_Plant[i].cNodeLeafType = 11; 
            }
        return ;
        }
    
    // if he's fertilized some P or K in the last 5 days we won't lose any
    for(i=giGrowDay-1;i>=max(0, giGrowDay-5);i=i-1)
        { // if he's given any P or K, we getout
        if(plPlant->GD_Plant[i].szNutrients[1]!=0 
        || plPlant->GD_Plant[i].szNutrients[2]!=0)
            return;
        } 
    
    // if we've lost health compared to yesterday, we'll lose a random leaf
    if(plPlant->GD_Plant[giGrowDay].cHealth 
        < plPlant->GD_Plant[giGrowDay-1].cHealth)
        {
        iNode = Random(0,iNodes-20);
        // now that we've found a leaf, lets lose it and getout
        plPlant->NO_Plant[iNode].cNodeLeafType = -1; 
        return;
        }
    // ALSO...
    // if we've lost potency compared to yesterday, we lose a random leaf
    if(plPlant->GD_Plant[giGrowDay].iPotency 
        < plPlant->GD_Plant[giGrowDay-1].iPotency)
        {
        iNode=0; // set our random found leaf to 0
        while(iNode==0) // keep looking untill...
            { // we find a random leaf that we can lose
            if((iNode = Random(0,iNodes-20)) == -1)
                iNode = 0;
            }
        // now that we've found a leaf, lets lose it and getout
        plPlant->NO_Plant[iNode].cNodeLeafType = -1; 
        return;
        }
     }


/******************************************************************************\
*  FUNCTIONS TO: CALCULATE LEAF DEVELOPMENT AND LOSSES
\******************************************************************************/

void LFCalcLeafGrowth(PPLANT plPlant, int iNodes, int iGStage)
    {
    // *** CALCULATE LEAF TYPES ***
    LFCalcNodeLeafTypes(plPlant->NO_Plant, giGrowingNodes, giGrowthStage);
    // *** CALCULATE LEAF LOSS BASED ONE HEALTH ***
    LFCalcLeafLoss(plPlant, giGrowingNodes, giGrowthStage);
    }

/******************************************************************************\
*  FUNCTIONS TO: Draws Leaf Images at each node's end point
\******************************************************************************/

//  1 = Tiny two leaf growing UP
//  2 = Tiny growing DOWN
//  3 = Small Left
//  4 = Small Right
//  5 = Medium Left
//  6 = Medium Right
//  7 = Big Left
//  8 = Big Right
//  9 = Really Big Left
// 10 = Really Big Right

void LFDrawLeafImage(HDC hdc, POINT ptNode, int iLeafType, int iLeafID)
    {
    // valid LeafTypes are from 1 to 10
    // valid LeafID depends on number of images in leaftype
    BITMAP bm;
    POINT ptEnd;
    // ---------
    // first we getout if the leaf has dropped off (type= -1)
    if(iLeafType <= 0) return;
    // we also getout if the leaf type is invalid
    if(iLeafType > 10)  return;

    // now we must set up the leaf origin from the node end point
    switch(iLeafType)
        {
        case  1: ptEnd.x=ptNode.x- 7; ptEnd.y=ptNode.y- 7; break;
        case  2: ptEnd.x=ptNode.x- 8; ptEnd.y=ptNode.y- 4; break;
        case  3: ptEnd.x=ptNode.x-13; ptEnd.y=ptNode.y- 6; break;
        case  4: ptEnd.x=ptNode.x- 8; ptEnd.y=ptNode.y- 7; break;
        case  5: ptEnd.x=ptNode.x-17; ptEnd.y=ptNode.y- 7; break;
        case  6: ptEnd.x=ptNode.x- 8; ptEnd.y=ptNode.y- 8; break;
        case  7: ptEnd.x=ptNode.x-29; ptEnd.y=ptNode.y- 7; break;
        case  8: ptEnd.x=ptNode.x- 9; ptEnd.y=ptNode.y- 8; break;
        case  9: ptEnd.x=ptNode.x-37; ptEnd.y=ptNode.y-10; break;
        case 10: ptEnd.x=ptNode.x-14; ptEnd.y=ptNode.y-12; break;
        }
    if((iLeafType ==1)||(iLeafType ==2))
        {
        // Now draw the 16 colour leaf from the correct imagelist type
        ImageList_Draw(hLeafImages[iLeafType-1], iLeafID, hdc, 
                       ptEnd.x, ptEnd.y, ILD_TRANSPARENT);
        }
    else
        {
        // now draw the 256 colour leaf from the correct bitmap type
        GetObject(hLeafBitmap[iLeafType-3], sizeof(BITMAP), (LPSTR)&bm);
        DIDrawTransparentSection(hdc, hLeafBitmap[iLeafType-3], hLeafPalette[iLeafType-3],
                                 iLeafID*bm.bmHeight, 0, bm.bmHeight, bm.bmHeight,
                                 (short)ptEnd.x, (short)ptEnd.y, RGB(255,255,255));
        }
    }

