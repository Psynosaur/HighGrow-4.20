#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <math.h>
#include "resource.h"
#include "global.h"
#include "toolbar.h"
#include "calc.h"
#include "seedlist.h"
#include "plantmem.h"
#include "growroom.h"
#include "nodecalc.h"
#include "dibitmap.h"
#include "Leaves.h"
#include "Buds.h"
#include "Robbie.h"
#include "versinfo.h"
#include "password.h"
#include "dibapi.h"
#include "clock.h"
#include "activity.h"
#include "jpgview.h"
#include "crypt.h"
#include "midi.h"

/******************************************************************************\
*  GROWROOM.C - The best place to be !!!
\******************************************************************************/

#define    gPotWidth   86
#define    gPotHeight  95

#define    gLampWidth  145
#define    gLampHeight  48

HWND       hGrowRoomWindow = NULL;
HDC        hdcOffScreen;     // handle to our compatible DC
HFONT      hNameFont;        // used to show plant name on lamp
HFONT      hVacModeFont;     // used to show Vacation Mode Enabled text
HCURSOR    hArrowCursor;     // normal growroom arrow cursor
HCURSOR    hPruneCursor;     // shows a scissors if he's over a tip
HCURSOR    hOHandCursor;     // shows a open hand if he's over the light
HCURSOR    hCHandCursor;     // shows a closed hand if he's clicked on the light
HCURSOR    hBHandCursor;     // shows a pointing hand for clicking on the boombox
HIMAGELIST hLLightImage;     // handle to image list for lamp light
HIMAGELIST hBlackImage;      // handle to image list for black dark bitmap
HBITMAP    hDarknessBmp;     // handle to black bitmap for alpha-blending
DRAWPLANT  DP_Plant[3];      // for drawing our 3 plants
TIPNODE    TN_Plant[3];      // holds node numbers for our 3 plant's tips
HANDLE     hStemPens[5];     // handles to our four brown stem pens
HANDLE     hCordPen;         // to draw the lamp power-cord
HBITMAP    hGrowRoomBmp;     // handle to background bitmap
HPALETTE   hGrowRoomPal;     // handle to background palette
HBITMAP    hPotListBmp;      // handle to a pots bitmap
HPALETTE   hPotListPal;      // handle to a pots palette
HBITMAP    hLightListBmp;    // handle to a lights bitmap
HPALETTE   hLightListPal;    // handle to a lights palette
HBITMAP    hBoomBoxBmp;    // handle to a lights bitmap
HPALETTE   hBoomBoxPal;    // handle to a lights palette
HBITMAP    hLightMHBmp;      // handle to bitmap for Metal Halide light shine
HBITMAP    hLightHPSBmp;     // handle to bitmap for High Pressure Sodium shine
POINT      PT_ClipPoints[17];// array of points for our darkness clipping region
RECT       RC_DrawPlant[3];  // holds calculated growroom area for our plants

RECT       rcCurMouseLight;  // holds current position of lamp while dragging it
int        giCurMouseTipNode;// holds node number of the tip his mouse moves over

HMENU      hContextMenu=0;

/******************************************************************************\
*  CLOSING THE GROWROOM WINDOW
\******************************************************************************/

void GRExitGrowRoom(void)
    {
    if(hGrowRoomWindow)
        DestroyWindow(hGrowRoomWindow);
    hGrowRoomWindow = 0;
    }

/******************************************************************************\
*  CREATING THE GROW ROOM WINDOW AND DRAWING THE BACKGROUND AND PLANTS
\******************************************************************************/

void GREnterGrowRoom(HWND hwnd, HINSTANCE hInst)
    {
    RECT rcWin; 
    int  i = TBToolbarHeight();
    // ******* prepare the plants for drawing
    GRInitGrowRoomDrawPlants();
    // **************************************
    GetClientRect(hwnd, &rcWin);
    if(!hGrowRoomWindow)
        {
        hGrowRoomWindow =CreateWindowEx(WS_EX_CLIENTEDGE, 
                                        "GrowRoom", NULL, WS_CHILD, 0, i,
                                        rcWin.right-rcWin.left, rcWin.bottom-rcWin.top-i,     
                                        hwnd, NULL, hInst, NULL);
        }
    }

/******************************************************************************\
*  CHECK IF WE MUST REDRAW THE GROWROOM WHEN HE CHANGES THE ROOM COMBOBOX
\******************************************************************************/

void GRCheckToRedrawGrowRoom(HWND hwnd, HINSTANCE hInst)
    { 
    // when the user changes the growroom combobox, we check to see if
    // he is currently in any of the growrooms. If he is, we'll close the 
    // current room and enter the selected room
    if(hGrowRoomWindow)
        {
        LockWindowUpdate(hwnd);
        GRExitGrowRoom();
        GREnterGrowRoom(hwnd, hInst);
        LockWindowUpdate(NULL);
        }
    }

/******************************************************************************\
*  HERE WE RETURN THE OFFSET FROM THE PLANTSTARTY TO DRAW EACH PLANT IN THE POT
\******************************************************************************/

int GRGetPlantOffset(int iGrowroom)
    { // option based 1 growroom
    int iofs = 0;
    int iPotNumber = GR_Room[iGrowroom-1].iPotType;
    switch (iPotNumber)
        {
        case 0:
            {
            iofs = 12;
            break;
            }
        case 1:
            {
            iofs = 4;
            break;
            }
        case 2:
            {
            iofs = -4;
            break;
            }
        case 3:
            {
            iofs = 5;
            break;
            }
        case 4:
            {
            iofs = -25;
            break;
            }
        case 5:
            {
            iofs = -1;
            break;
            }
        case 6:
            {
            iofs = -17;
            break;
            }
        case 7:
            {
            iofs = -14;
            break;
            }
        case 8:
            { // rectangular
            iofs = 10;
            break;
            }
        case 9:
            { // rectangular
            iofs = 4;
            break;
            }
        case 10:
            { // rectangular
            iofs = -24;
            break;
            }
        case 11:
            { 
            iofs = 4;
            break;
            }
        case 12:
            { 
            iofs = 18;
            break;
            }
        case 13:
            { // rectangular
            iofs = 8;
            break;
            }
        case 14:
            {
            iofs = -2;
            break;
            }
        case 15:
            {
            iofs = -4;
            break;
            }
        case 16:
            {
            iofs = -2;
            break;
            }
        case 17:
            {
            iofs = 7;
            break;
            }
        case 18:
            {
            iofs = 22;
            break;
            }
        case 19:
            {
            iofs = 3;
            break;
            }
        case 20:
            {
            iofs = 11;
            break;
            }
        case 21:
            { // rectangular
            iofs = -9;
            break;
            }
        case 22:
            {
            iofs = -2;
            break;
            }
        case 23:
            {
            iofs = -1;
            break;
            }
        case 24:
            {
            iofs = 8;
            break;
            }
        case 25:
            {
            iofs = -3;
            break;
            }
        case 26:
            {
            iofs = 4;
            break;
            }
        case 27:
            {
            iofs = -2;
            break;
            }
        case 28:
            {
            iofs = -27;
            break;
            }
        case 29:
            {
            iofs = 5;
            break;
            }
        }
    return iofs;
    }


/******************************************************************************\
*  CALCULATING TOTAL NUMBER OF DRAWNODES
\******************************************************************************/

int GRCalcNumDrawNodes(int iPlant)
    { // add up how many drawnodes we have for this plant
    int i;     // total number of nodes
    BOOL bTrue=FALSE; 
    // first we calculate how many nodes we have so far
    for(i=0;(i<gMaxNodes && bTrue==FALSE);i++)
        { // if no length, then it's not a valid node
        if(DP_Plant[iPlant].DN_Plant[i].cDNThickness==0)
            bTrue=TRUE;
        }  
    return i-1;
    }


/******************************************************************************\
*  SETS THE RECT WITHIN GROWROOM WINDOW WHICH SPANS THE GIVEN PLANT
\******************************************************************************/

BOOL GRGetPlantRect(int iPlant, LPRECT lpRect)
    {
    RECT       rcPlant; // our local calculating rect
    PDRAWPLANT pdPlant = &DP_Plant[iPlant]; 
    int  i;
    // first we set the default calc rect for this plant
    rcPlant.left   = DP_Plant[iPlant].DN_Plant[0].ptDNStart.x;
    rcPlant.right  = rcPlant.left;
    rcPlant.top    = gPlantStartY;
    rcPlant.bottom = gPlantStartY;
    if(giGrowingNodes != 0)
        { // now we check the area that this plant occupies
        for(i=0;i<giGrowingNodes;i++)
            { 
            // here we check the minimum X value
            if(pdPlant->DN_Plant[i].ptDNEnd.x < rcPlant.left)
                rcPlant.left  = pdPlant->DN_Plant[i].ptDNEnd.x;
            // here we check the maximum X value
            if(pdPlant->DN_Plant[i].ptDNEnd.x > rcPlant.right)
                rcPlant.right = pdPlant->DN_Plant[i].ptDNEnd.x;
            // here we check the maximum Y value
            if(pdPlant->DN_Plant[i].ptDNEnd.y < rcPlant.top)
                rcPlant.top   = pdPlant->DN_Plant[i].ptDNEnd.y;
            }
        }
    // now we must fill his rect with our calculated values
    // we'll add 10 pixels left, right and top for extra room
    lpRect->left   = max(0, rcPlant.left-10); // we'll max this for safety
    lpRect->top    = max(0, rcPlant.top -10); // this one too...
    lpRect->right  = rcPlant.right +10;
    lpRect->bottom = rcPlant.bottom;
    // everything's ok, so tell him
    return TRUE;
    }


/******************************************************************************\
*  PREPARES OUR TIPNODE STRUCTURE FOR THE GIVEN PLANT
\******************************************************************************/

int GRInitTipNodes(PNODE noPlant, int iPlant)
    { // uses the plant node array to fill our TIPNODE struct
    // **giGrowingNodes MUST be correct**
    int i, iTips = 0; // offset into our PRUNETIP struct array

    for(i=0;(i<giGrowingNodes && iTips<gMaxTipNodes);i++)
        { // we must fill our TIPNODE structure for all tips (max 200)
        if(noPlant[i].bGrowingTip == TRUE)
            { // if the node is a tip, we'll add it to our array
            TN_Plant[iPlant].iTipNodes[iTips] = i; // "i" is our current node
            // now increment our TIPNODE struct array pointer
            iTips += 1;
            }
        }
    TN_Plant[iPlant].iTipNodes[iTips] = i; // zeros the next one for safety
    // return the number of tips added to anyone that's interested in this
    return iTips;
    }
                             
/******************************************************************************\
*  CALCULATES CURRENT PLANT NODE DRAWING POSITIONS
\******************************************************************************/

void GRCalcDrawNodes(PNODE noPlant, PDRAWPLANT dpPlant, int iPlant, int iStyle)
    { // prepares the current plant's DrawNode structure, in preparation for
    // drawing this plant. The size of our plant is based on the absolute
    // maximum of 2500mm high spread over giPlantPixels screen height
    // If iStyle is CDN_MM we calculate in mm or CND_PIX for pixels
    int i;             // used for our loops
    int xOfs, yOfs;    // for calculating horizontal & vertical growth offsets
    int iAngle;        // for calculating horizontal & vertical growth offsets
    double dRadians;   // holds our angle in radians
    double dPPix;

    // getout if we have an invalid plant
    if(iPlant<0||iPlant>2) return;

    if(iStyle==CDN_MM)
         dPPix = 1.00;        // for mm calculations
    else dPPix = gPlantStartY/2500.00; // for pixel calculations

    for(i=0;i<giGrowingNodes;i++)
        {
        // first we must set our node's growth in x and y direction
        if(noPlant[i].cNodeAngle) // are we growing at any angle?
            { // then we must calculate our horizontal and vertical components
            iAngle  =(90 - abs(noPlant[i].cNodeAngle)); // get positive angle
            dRadians=iAngle * 0.0174533; // convert degrees to radians
            xOfs= (int)ceil((noPlant[i].cNodeLength*cos(dRadians))*dPPix);
            yOfs= (int)ceil((noPlant[i].cNodeLength*sin(dRadians))*dPPix);
            }
        else // if we're growing upwards, there's no horizontal component
            {
            xOfs= 0;
            yOfs= (int)ceil(noPlant[i].cNodeLength*dPPix);
            }
        // now we set the node's START X and Y position
        if(i==0) // if this is our start node
            {
            int iX;
            switch(iPlant)
                {
                case 0: iX=155; break;
                case 1: iX=305; break;
                case 2: iX=460; break;
                }
            dpPlant->DN_Plant[i].ptDNStart.x = iX;  // calculated
            dpPlant->DN_Plant[i].ptDNStart.y = gPlantStartY; // static
            }
        else // we must find our start from our predeccessor's end
            {
            dpPlant->DN_Plant[i].ptDNStart.x 
                = dpPlant->DN_Plant[noPlant[i].iStartNode].ptDNEnd.x;

            dpPlant->DN_Plant[i].ptDNStart.y 
                = dpPlant->DN_Plant[noPlant[i].iStartNode].ptDNEnd.y;
            }
        // now we decrement the node's END Y position (negative = up)
        dpPlant->DN_Plant[i].ptDNEnd.y 
             = dpPlant->DN_Plant[i].ptDNStart.y - yOfs;
        // and now we set the node's END X position (negative or positive)
        if(noPlant[i].cNodeAngle>=0) // are we growing towards the right
             dpPlant->DN_Plant[i].ptDNEnd.x = dpPlant->DN_Plant[i].ptDNStart.x + xOfs;
        else dpPlant->DN_Plant[i].ptDNEnd.x = dpPlant->DN_Plant[i].ptDNStart.x - xOfs;

        // here we set the other more obvious elements
        dpPlant->DN_Plant[i].cDNLeafID    = noPlant[i].cNodeLeafID;
        dpPlant->DN_Plant[i].cDNLeafType  = noPlant[i].cNodeLeafType;
        dpPlant->DN_Plant[i].cDNThickness = 1;
        }
    }


/******************************************************************************\
*  PREPARES EACH NODE'S STEM THICKNESS IN OUR DRAWPLANT STRUCT
\******************************************************************************/

BOOL GRCheckStemThickness(PNODE noPlant, int iPlant)
    { // here we calculate the node's stem thickness
    int i, j, k=0;

    // if we're not yet vegetating, exit with true
    if(giGrowthStage<3) return TRUE;

    for(i=0;i<giGrowingNodes;i++)
        { // j = percentage of total nodes
        j = ((i*100)/giGrowingNodes); 

        k = 1;
        if(j<35) k +=1; // add 1 if less than 35% of total nodes
        if(j<15) k +=1; // add 1 if less than 8% of total nodes 
        if(giGrowthStage>3 && j<7)   k +=1; // if preflowering or more
        if(noPlant[i].cNodeAngle==0) k +=1; // if growing straight up

        DP_Plant[iPlant].DN_Plant[i].cDNThickness = min(5, max(1, k));
        }
    return TRUE;
    }

/******************************************************************************\
*  PREPARES THE NUMBER OF DAYS THIS PLANT HAS BEEN FLOWERING (IF ANY)
\******************************************************************************/


void GRInitFloweringDays(PPLANT plPlant, int iPlant)
    {
    // if flowering or later, we calculate number of days of flowering
    // this is done by calculating the number of days of less than 12 hours
    // of light and then subtracting 12 days (for preflowering etc) to arrive
    // at the number of days since the start of the flowering display
    int  i, j;
    BOOL bIsTrue;
    // if we're not yet preflowering, we set flowering days to zero and return
    if(giGrowthStage<4)
        {
        DP_Plant[iPlant].iFlowerDays = 0;
        return;
        }
    i = giGrowDay-1;
    bIsTrue = TRUE;
    while((i>0)&(bIsTrue==TRUE))
        { // calculate the photoperiod
        j= GLGrowDayPhotoperiod(plPlant, i);
        if(j>12*4) // if more than 12 hours light, set false
            bIsTrue = FALSE;
        i -= 1; // decrement our day counter
        }
    DP_Plant[iPlant].iFlowerDays = max(0, (giGrowDay-i-12));
    }


/******************************************************************************\
*  INITIALIZES THE GROWROOM FOR PLANT DRAWING
\******************************************************************************/


// BOOL GRCheckIfLightIsOn(int iPlant, int iQuarters)
//     {
//     // first we assume that the light is off
//     DP_Plant[iPlant].bIsLightOn = FALSE;
//     // now we check if it should be on
//     if(iQuarters>=DP_Plant[iPlant].cLightOn 
//     && iQuarters< DP_Plant[iPlant].cLightOff)
//          { // if it's on, set our light drawing flag
//          DP_Plant[iPlant].bIsLightOn = TRUE;
//          return TRUE;
//          }
//     return FALSE;
//     }

BOOL GRCheckIfLightIsOn(int iPlant, int iQuarters)
    {
    int iStart = DP_Plant[iPlant].cLightOn;
    int iEnd   = DP_Plant[iPlant].cLightOff;
    // first we assume that the light is off
    DP_Plant[iPlant].bIsLightOn = FALSE;
    // now we check if it should be on
    if(iStart<=iEnd)
         {
         if((iQuarters>=iStart)&&(iQuarters<iEnd))
             { // if it's on, set our light drawing flag
             DP_Plant[iPlant].bIsLightOn = TRUE;
             return TRUE;
             }
         }
    else {
         if((iQuarters>=iStart)||(iQuarters<iEnd))
             { // if it's on, set our light drawing flag
             DP_Plant[iPlant].bIsLightOn = TRUE;
             return TRUE;
             }
         }
    return FALSE;
    }


void GRCalcDrawLamp(PPLANT plPlant, int iPlant, BOOL bIsGrowing)
    { // prepare this lamp for drawing
    int    iX, iMin;
    int    iHeight; 
    int    iRoom = TBGetComboGrowroom();
    int    iYofs = GRGetPlantOffset(iRoom);
    double dPPix = gPlantStartY/2500.00;
//    char sztext[50];
    // ----------
    if(bIsGrowing == TRUE)
        { // set the lamp height from the top
        // ****** DICEY CODE THIS *********************
        iMin    = 400 - 130 + iYofs - gLampHeight + 15;
        // ********************************************
        iHeight = (int)(plPlant->GD_Plant[giGrowDay].cLightHeight*125*dPPix);
        // set the switch On and Off times
        DP_Plant[iPlant].cLightOn   = plPlant->GD_Plant[giGrowDay].cLightOn;
        DP_Plant[iPlant].cLightOff  = plPlant->GD_Plant[giGrowDay].cLightOff;
        // check if we must show the lamp to be on
        GRCheckIfLightIsOn(iPlant, GLGetTimeNowQuarters());
        }
    else 
        { 
        // no seed planted yet, raise lamp to top
        iHeight = gPlantStartY;
        // our lamp can also not be switched on
        DP_Plant[iPlant].cLightOn  = 0;
        DP_Plant[iPlant].cLightOff = 0;
        DP_Plant[iPlant].bIsLightOn = FALSE;
        }

    // now we calculate the X position for the relevant plant
    switch(iPlant)
       { 
       case 0: iX=80;  break;
       case 1: iX=235; break;
       case 2: iX=390; break;
       }
    // now we set the light top-left X drawing position
    DP_Plant[iPlant].ptLightOrigin.x = iX;
    // now we set the light top-left Y drawing position
    DP_Plant[iPlant].ptLightOrigin.y 
        = max(0-gLampHeight+5, (gPlantStartY-gLampHeight-5-iHeight));
    // ---
//    wsprintf(sztext, "iMin=%i, iMax=%i, LightY=%i", iMin, iMax, DP_Plant[iPlant].ptLightOrigin.y);
//    Blow(sztext);
    // ---
    DP_Plant[iPlant].ptLightOrigin.y = min(iMin, DP_Plant[iPlant].ptLightOrigin.y);
    }

                             
void GRInitDrawPlant(PPLANT plPlant, int iPlant, BOOL bIsGrowing)
    {
    // first erase the entire drawplant struct for this plant
    ZeroMemory(&DP_Plant[iPlant], (DWORD)sizeof(DRAWPLANT));
    // now fill the drawplant struct for this plant
    if(bIsGrowing == TRUE)
        {
        // calculate the drawing positions for all the nodes in this plant
        GRCalcDrawNodes(plPlant->NO_Plant, &DP_Plant[iPlant], iPlant, CDN_PIX);
        // now calculate the stem stickness for all our nodes
        GRCheckStemThickness(plPlant->NO_Plant, iPlant);
        // now calculate the growroom screen rect for this plant
        GRGetPlantRect(iPlant, &RC_DrawPlant[iPlant]);
        // and fill our tipnode struc for this plant too
        GRInitTipNodes(plPlant->NO_Plant, iPlant);
        // finally check if this plant has been flowering
        GRInitFloweringDays(plPlant, iPlant);
        }
    // prepare the lamp for drawing
    GRCalcDrawLamp(plPlant, iPlant, bIsGrowing);
    }


void GRInitGrowRoomDrawPlants(void)
	{
    char szFile[256] ="\0";
    int i            = 0;
    int iRoomOfs     = (TBGetComboGrowroom()-1)*3;
    // now initialise our global temperature value
    giRoomTemp = CACalcRoomTemperature();
    // now perform plant's each calculation, showing our progress
    for(i=0;i<3;i++)
        {
        giCurPlant = i+1+iRoomOfs;
        sprintf(szFile, "Plant%02i.hgp", i+1+iRoomOfs);
        if(GLDoesFileExist(szFile))
            {
            if(PMPlantFileToMem(i+iRoomOfs))
                { // first lock our global memory
                P_Plant = (PPLANT)GlobalLock(hPlantMem);
                // calculate the plant info
                CACalculatePlant(P_Plant, i+iRoomOfs);
                GLInitPlantInfo(&P_Plant->PI_Plant);
                GLInitGrowDay(&P_Plant->GD_Plant[giGrowDay]);
                // calc plant's temperature influence
                giRoomTemp += CACalcPlantTemperature((PPLANT)P_Plant);
                // now prepare for drawing our plant
                GRInitDrawPlant((PPLANT)P_Plant, i, TRUE);
                // we can unlock our memory again
                GlobalUnlock(hPlantMem);
                // finally, save our changes back
                PMPlantMemToFile(i+iRoomOfs);
				}
            }
        else
            { // set our light positions etc.
            GRInitDrawPlant((PPLANT)P_Plant, i, FALSE);
            }
        }
    // ensure we're within range
    giRoomTemp = max(15,min(40,giRoomTemp));
    // after calculating room temperature, we can calculate our humidity
    CACalcRoomHumidity();
	}

/******************************************************************************\
*  HERE WE REMOVE A PLANT FROM THE GROWROOM'S DRAWPLANT STRUCT 
\******************************************************************************/


void GRRemoveDrawPlant(int iPlant)
    { // remove the OB0 plant from the DRAWPLANT struct
    DP_Plant[iPlant].bIsLightOn      = FALSE;
    DP_Plant[iPlant].cLightOn        = 0;
    DP_Plant[iPlant].cLightOff       = 0;
    DP_Plant[iPlant].ptLightOrigin.y = 5-gLampHeight;
    DP_Plant[iPlant].DN_Plant[0].ptDNStart.x = 0;
    DP_Plant[iPlant].DN_Plant[0].ptDNStart.y = 0;
    }


/******************************************************************************\
*  HERE WE DARKEN THE ENTIRE ROOM 
\******************************************************************************/


int GRGetLightShineAngle(int iShadeType)
    {
    int iAngle =75;

    switch (iShadeType)
        {
        case 0:
            {
            iAngle =55;
            break;
            }
        case 1:
            {
            iAngle =75;
            break;
            }
        case 2:
            {
            iAngle =35;
            break;
            }
        case 3:
            {
            iAngle =35;
            break;
            }
        case 4:
            {
            iAngle =60;
            break;
            }
        case 5:
            {
            iAngle =30;
            break;
            }
        case 6:
            {
            iAngle =72;
            break;
            }
        case 7:
            {
            iAngle =80;
            break;
            }
        case 8:
            {
            iAngle =45;
            break;
            }
        case 9:
            {
            iAngle =60;
            break;
            }
        }
    return iAngle;
    }



void GRTileDarknessAndLightness(HDC hdc, RECT rc, int iRoom)
    { // fill the growroom rect with our darkness and lightness images
    int   i, ion, xOfs;
    int   iWidth, iHeight, iLength;
    HRGN  hLightRegion[3]; // regions for each of our lights
    HRGN  hLCircRegion[3]; // regions for circular bottoms of the light area
    HRGN  hDarkRegion;     // darkness region
    HRGN  hShineRegion;    // lightshine region
    POINT ptClipPoints[5];
    HBITMAP hbmp = 0;
    int    iAngle = GRGetLightShineAngle(GR_Room[iRoom].iShadeType);
    double dRadians=iAngle*0.0174533; // convert 60 degrees to radians
    // ------------
    // getout if we don't have our darkness image loaded
//    if(hBlackImage==0) return;
    // set these locals for ease of use
    iWidth  = rc.right-rc.left;
    iHeight = rc.bottom-rc.top;
    // assume all the lights are on
    ion = 3;
    // first we create a regions for the entire screen
    hDarkRegion   = CreateRectRgn(0, 0, iWidth, iHeight);
    hShineRegion  = CreateRectRgn(0, 0, iWidth, iHeight);
    // we also create a region for the light area of each plant
    for(i=0;i<3;i++)
        { // if a valid plant, subtract it's light region from our dark region
        if(DP_Plant[i].bIsLightOn == TRUE)
            {
            iLength = iHeight - (DP_Plant[i].ptLightOrigin.y + gLampHeight-5);
            xOfs    = (int)ceil(iLength*cos(dRadians));

            ptClipPoints[0].x=DP_Plant[i].ptLightOrigin.x;
            ptClipPoints[0].y=DP_Plant[i].ptLightOrigin.y + gLampHeight-5;

            ptClipPoints[1].x=ptClipPoints[0].x - xOfs;
            ptClipPoints[1].y=iHeight;

            ptClipPoints[2].x=ptClipPoints[0].x + gLampWidth + xOfs;
            ptClipPoints[2].y=iHeight;

            ptClipPoints[3].x=ptClipPoints[0].x + gLampWidth;
            ptClipPoints[3].y=ptClipPoints[0].y;

            ptClipPoints[4].x=ptClipPoints[0].x;
            ptClipPoints[4].y=ptClipPoints[0].y;
            // here we create each light region
            hLightRegion[i]=CreatePolygonRgn((POINT*)&ptClipPoints, 5, WINDING);
            // here we create and combine each light circle region
            hLCircRegion[i]=CreateEllipticRgn(ptClipPoints[1].x-300, 
                                ptClipPoints[0].y-800, ptClipPoints[2].x+300, 
                                ptClipPoints[2].y+(xOfs/4));
            CombineRgn(hLightRegion[i], hLightRegion[i], hLCircRegion[i], RGN_AND);
            // now we combine our light region into our Screen Region
            CombineRgn(hDarkRegion, hDarkRegion, hLightRegion[i], RGN_DIFF);
            // decrease the count of lights on
            ion -= 1;
            }
        }
    // we select our dark region
    if(hDarkRegion) SelectClipRgn(hdc, hDarkRegion);
    // now we can tile the darkness image
    if(gbAlphaBlending && (hDarknessBmp!=0))
        DITileBlendBitmap(hdc, rc, hDarknessBmp, 50+(ion*20));
    else 
        GLTileImage(hdc, rc, hBlackImage);

    if(GR_Room[iRoom].iLampType == 0)
         hbmp = hLightMHBmp;
    else hbmp = hLightHPSBmp;
    // here we combine the three light regions and alphablend the shine
    if(gbAlphaBlending && (hbmp!=0))
        {
        SetRectRgn(hShineRegion, 0, 0, 0, 0);
        for(i=0;i<3;i++)
            { // if a valid plant, subtract it's light region from our dark region
            if(DP_Plant[i].bIsLightOn == TRUE)
                CombineRgn(hShineRegion, hShineRegion, hLightRegion[i], RGN_OR);
            }
        if(hShineRegion) SelectClipRgn(hdc, hShineRegion);
        DITileBlendBitmap(hdc, rc, hbmp, 10+(GR_Room[iRoom].iLampStrength)*5);
        }

    // we must reset our clipping region to fullscreen
    SelectClipRgn(hdc, NULL);
    // we must also delete these clipping regions
    for(i=0;i<3;i++)  DeleteObject(hLightRegion[i]); // regions for each of our lights
    for(i=0;i<3;i++)  DeleteObject(hLCircRegion[i]); // regions for our light circles
    if(hDarkRegion)   DeleteObject(hDarkRegion);     // darkness region
    if(hShineRegion)  DeleteObject(hShineRegion);    // lightshine region
    }

/*
void GRFillRectWithDarkness(HDC hdc, RECT rc)
    { // fill the given rect with our darkness bitmap
    int   i, ion;
    // assume all the lights are on
    if(gbAlphaBlending && (hDarknessBmp!=0))
        {
        ion = 3;
        for(i=0;i<3;i++)
            { // if a valid plant, subtract it's light region from our dark region
            if(DP_Plant[i].bIsLightOn == TRUE)
                // decrease the count of lights on
                ion -= 1;
            }
        // now we can tile the darkness image
        DITileBlendBitmap(hdc, rc, hDarknessBmp, 0+(ion*20));
        }
    }
*/

/******************************************************************************\
*  HERE WE DRAW EACH PLANT'S NODES
\******************************************************************************/

BOOL GRDrawPlantNodes(HDC hdc, int iPlant, int iMaxNodes, int iYadj)
    {
    LPPOINT lpPoint=NULL;
    HANDLE  hPrevObject; // previous pen
    int i, iOfs;

    // getout if we have an invalid plant
    if(iPlant<0||iPlant>2)         return FALSE;
    // getout if we somehow haven't filled our DrawNode structure    
    if(DP_Plant[iPlant].DN_Plant[0].ptDNStart.y==0) return FALSE;

    for(i=0;i<iMaxNodes;i++)
        { // draw each node from start to end point
        if(i==0)
             iOfs = ((DP_Plant[iPlant].DN_Plant[i].cDNThickness+1)/2)+1;
        else iOfs = 0;
        MoveToEx(hdc, DP_Plant[iPlant].DN_Plant[i].ptDNStart.x, 
                      DP_Plant[iPlant].DN_Plant[i].ptDNStart.y+iYadj-iOfs, lpPoint);
        // select a brown pen of the correct stem thickness
        hPrevObject= SelectObject(hdc, 
                        hStemPens[DP_Plant[iPlant].DN_Plant[i].cDNThickness-1]);
        // and draw this node
        LineTo(hdc, DP_Plant[iPlant].DN_Plant[i].ptDNEnd.x, 
                    DP_Plant[iPlant].DN_Plant[i].ptDNEnd.y+iYadj);
        // reselect this DC's previous pen
        SelectObject(hdc, hPrevObject);
        }
    
    return TRUE;
    }


/******************************************************************************\
*  HERE WE DRAW EACH PLANT'S LEAVES
\******************************************************************************/
 
BOOL GRDrawPlantLeaves(HDC hdc, int iPot, int iPlant, int iMaxNodes, int iYadj)
    {
    int i, j;
    POINT ptend;

    // getout if we have an invalid plant
    if(iPot<0||iPot>2)         return FALSE;
    // getout if we somehow haven't filled our DrawNode structure    
    if(DP_Plant[iPot].DN_Plant[0].ptDNStart.y==0) return FALSE;
    // getout if this plant is dead
    if(PI_PlantSeed[iPlant].cGrowthStage>=7) return FALSE;

    j=0;
    if(iMaxNodes>=5)   j=1;
    if(iMaxNodes>=10)  j=2;
    if(iMaxNodes>=25)  j=3;
    if(iMaxNodes>=100) j=4;
    for(i=j;i<iMaxNodes;i++)
        { 
        // make the adjustment for the pot height
        ptend.x = DP_Plant[iPot].DN_Plant[i].ptDNEnd.x;
        ptend.y = DP_Plant[iPot].DN_Plant[i].ptDNEnd.y + iYadj;
        // now draw a leaf at the node's end point
        LFDrawLeafImage(hdc, ptend, DP_Plant[iPot].DN_Plant[i].cDNLeafType,
                                    DP_Plant[iPot].DN_Plant[i].cDNLeafID);
        }
    
    return TRUE;
    }


/******************************************************************************\
*  HERE WE DRAW EACH PLANT'S BUDS (FLOWERS)
\******************************************************************************/


BOOL GRDrawPlantBuds(HDC hdc, int iPot, int iPlant, int iMaxNodes, int iYadj)
    {
    int i, iType;
    POINT ptend;

    // ****** TEMP STUFF *******
//    DP_Plant[iPlant].iFlowerDays=30;
    // ****** END TEMP STUFF *******

    // getout if we have an invalid plant
    if(iPot<0||iPot>2)         return FALSE;
    // getout if we somehow haven't filled our DrawNode structure    
    if(DP_Plant[iPot].DN_Plant[0].ptDNStart.y==0) return FALSE;
    // getout if we haven't got any flowering days
    if(DP_Plant[iPot].iFlowerDays==0) return FALSE;
    // getout if this plant is dead
    if(PI_PlantSeed[iPlant].cGrowthStage>=7) return FALSE;

    for(i=0;i<iMaxNodes;i+=3)
        { // if the leaf type is small or medium (no tips)
        iType = DP_Plant[iPot].DN_Plant[i].cDNLeafType; 
        if((iType>1)&(iType<7))
            {
            // make the adjustment for the pot height
            ptend.x = DP_Plant[iPot].DN_Plant[i].ptDNEnd.x;
            ptend.y = DP_Plant[iPot].DN_Plant[i].ptDNEnd.y + iYadj;
            // here we draw the bud image
            BDDrawBudImage(hdc, ptend, PS_Plant[iPot].bSeedGender, 
                                       DP_Plant[iPot].iFlowerDays);
            }
        }
    return TRUE;
    }


/******************************************************************************\
*  HERE WE DRAW THE PLANT HEIGHT MEASUREMENT LINES
\******************************************************************************/

/*
void GRDrawHeightMeasureLines(HWND hwnd, HDC hdc)
    { // these lines show the visitor how tall is plants are
    double dPPix = gPlantStartY/2500.00;
    LPPOINT lpPoint=NULL;
    HPEN    hPrevPen; // previous pen
    int  i, iY;
    RECT rc;
    // get the growroom size for our line's X start and end points
    GetClientRect(hwnd, &rc);
    // now select a black pen 
    hPrevPen = SelectObject(hdc, GetStockObject(BLACK_PEN));

    iY = (int)(gPlantStartY-(500*dPPix));
    for(i=0;i<4;i++)
        {
        // set the line's drawing origin and draw it
        MoveToEx(hdc, rc.left, iY, lpPoint);
        LineTo(hdc, rc.right, iY);
        // decrement our Y position for another 500mm
        iY -= (int)(500*dPPix);
        }
    // finally, reselect our previous pen
    SelectObject(hdc, hPrevPen);
    }
*/

/******************************************************************************\
*  HERE WE DRAW EACH PLANT'S LAMP SHADE
\******************************************************************************/


void GRDrawLamp(HDC hdc, int iPlant)
    {
    int xpos, ypos;
    int ilamp;
    BITMAP bm;
    LPPOINT lpPoint=NULL;
    HANDLE  hPrevPen; // previous pen
    int    iRoom = TBGetComboGrowroom();
    // draw the lamp, if we've got the Image
    if(hLightListBmp!=0)
        { // first we set some locals
        ilamp = min(GR_Room[iRoom-1].iShadeType, gMaxLights-1);
        xpos = DP_Plant[iPlant].ptLightOrigin.x+(gLampWidth/2);
        ypos = DP_Plant[iPlant].ptLightOrigin.y;
        // get the offsets to the top and left of the lamp for the string
        // if this lamp is not at the very top
        if(ypos>0)
            {
            // select the black cord pen 
            hPrevPen = SelectObject(hdc, hCordPen);
            // set the line drawing origin & draw the down line
            MoveToEx(hdc, xpos, 0, lpPoint);
            LineTo(hdc, xpos, ypos-35);
            LineTo(hdc, xpos-15, ypos-5);
            MoveToEx(hdc, xpos, ypos-35, lpPoint);
            LineTo(hdc, xpos+15, ypos-5);
            // and reselect our previous pen
            SelectObject(hdc, hPrevPen);
            }
        // calculate the bitmap size
        GetObject(hLightListBmp, sizeof(BITMAP), (LPSTR)&bm);
        // now draw the pot bitmap
        DIDrawTransparentSection(hdc, hLightListBmp, hLightListPal,
                                 ilamp*gLampWidth, 0, gLampWidth, bm.bmHeight,
                                 (short)(xpos - gLampWidth/2), (short)ypos-5, 
                                 RGB(0,0,255));
        }
    }


/******************************************************************************\
*  HERE WE DRAW THE EASTER EGG BOOMBOX IMAGE IF HE'S FOUND IT
\******************************************************************************/


void GRDrawBoomBox(HWND hwnd, HDC hdc)
    {
    BITMAP bm;
    LPPOINT lpPoint=NULL;
    RECT    rc;
    // --------
    // draw the lamp, if we've got the Image
    if(hBoomBoxBmp!=0)
        { // first we set some locals
        GetClientRect(hwnd, &rc);
        // calculate the bitmap size
        GetObject(hBoomBoxBmp, sizeof(BITMAP), (LPSTR)&bm);
        // now draw the pot bitmap
        DIDrawTransparentBitmap(hdc, hBoomBoxBmp, hBoomBoxPal,
                               (short)(rc.right - bm.bmWidth - 5), 
                               (short)(rc.bottom - bm.bmHeight + 2), 
                               RGB(0,0,255));
        }
    }


/******************************************************************************\
*  HERE WE DRAW EACH PLANT'S LAMP SHADE
\******************************************************************************/

void GRDrawPotShadow(HDC hdc, int iPot, int iPlant, int iPotX, int iPotW, int iWinH)
    {
    // here we draw the required (OB0) pot's shadow
    RECT rcshadow;
    int ish, isw, iofs;
    int iblend = 110;
    int ixofs = 0;
    int iyofs = 0;
    int ilighty;
//    char sztext[50];
    // calc the shadow settings for this pot shape
    // first set some defaults
    isw  = 0;
    ish  = 15;
    iofs = 1;
    switch (iPlant)
        {
        case 0:
            {
            isw = 5; ish = 16; iofs = 0;
            break;
            }
        case 1:
            {
            isw = 7; ish = 17; iofs = 0;
            break;
            }
        case 2:
            {
            isw = 8; ish = 18; iofs = 0;
            break;
            }
        case 3:
            {
            isw = 9; ish = 29; iofs = -2;
            break;
            }
        case 4:
            {
            isw = 10; ish = 32; iofs = -4;
            break;
            }
        case 5:
            {
            isw = 7; ish = 28; iofs = -1;
            break;
            }
        case 6:
            {
            isw = 9; ish = 30; iofs = -2;
            break;
            }
        case 7:
            {
            isw = 10; ish = 33; iofs = -3;
            break;
            }
        case 8:
            { // rectangular
            isw = 4; ish = 28; iofs = -1, ixofs = 22, iyofs = 6;
            break;
            }
        case 9:
            { // rectangular
            isw = 10; ish = 32; iofs = -1, ixofs = 25, iyofs = 6;
            break;
            }
        case 10:
            { // rectangular
            isw = 0; ish = 30; iofs = -4, ixofs = 0, iyofs = 0;
            break;
            }
        case 11:
            { 
            isw = 7; ish = 31; iofs = -3;
            break;
            }
        case 12:
            { 
            isw = 5; ish = 30; iofs = -3;
            break;
            }
        case 13:
            { // rectangular
            isw = 15; ish = 37; iofs = -1, ixofs = 35, iyofs = 10;
            break;
            }
        case 14:
            {
            isw = 9; ish = 23; iofs = -2;
            break;
            }
        case 15:
            {
            isw = 8; ish = 18; iofs = -1;
            break;
            }
        case 16:
            {
            isw = 9; ish = 21; iofs = -1;
            break;
            }
        case 17:
            {
            isw = 5; ish = 16; iofs = 0;
            break;
            }
        case 18:
            {
            isw = 7; ish = 17; iofs = 0;
            break;
            }
        case 19:
            {
            isw = 7; ish = 19; iofs = -2;
            break;
            }
        case 20:
            {
            isw = 5; ish = 16; iofs = 0;
            break;
            }
        case 21:
            { // rectangular
            isw = 0; ish = 28; iofs = -4, ixofs = 0, iyofs = 0;;
            break;
            }
        case 22:
            {
            isw = 6; ish = 21; iofs = -1;
            break;
            }
        case 23:
            {
            isw = 6; ish = 19; iofs = 0;
            break;
            }
        case 24:
            {
            isw = 6; ish = 24; iofs = -4;
            break;
            }
        case 25:
            {
            isw = 8; ish = 25; iofs = -4;
            break;
            }
        case 26:
            {
            isw = 14; ish = 28; iofs = -2;
            break;
            }
        case 27:
            {
            isw = 6; ish = 16; iofs = 0;
            break;
            }
        case 28:
            {
            isw = 8; ish = 13; iofs = -1;
            break;
            }
        case 29:
            {
            isw = 4; ish = 15; iofs = 0;
            break;
            }
        }
        
    // fill the rect for this pot
    rcshadow.left   = iPotX - (iPotW/2) + 10 - isw;
    rcshadow.right  = iPotX + (iPotW/2) - 10 + isw;
    rcshadow.top    = iWinH - ish - iofs;
    rcshadow.bottom = iWinH - iofs;
    // now draw the pot's shadow
    if((gbAlphaBlending)&&(hDarknessBmp!=0))
        { // lighten the shadow if the light is switched off
        if(DP_Plant[iPot].bIsLightOn == TRUE) 
            {
            // here we increase the size of the shadow the closer the light gets
            ilighty = min(13, (DP_Plant[iPot].ptLightOrigin.y + 50)/30);
            InflateRect(&rcshadow, ilighty, ilighty/3);
            }
        else
            {
            iblend = 50;
            }
        // now we turn the xywh rect to xyxy
        rcshadow.right  += rcshadow.left;
        rcshadow.bottom += rcshadow.top;
        // finally, we can blend the shadow bitmap
        if((iPlant==8)||(iPlant==9)||(iPlant==10)||(iPlant==13)||(iPlant==21))
            {
            DIAlphaBlendOffsetRect(hdc, rcshadow, hDarknessBmp, iblend, ixofs, iyofs);
            }
        else
            {
            DIAlphaBlendEllipse(hdc, rcshadow, hDarknessBmp, iblend);
            }
        }
    else 
        {
        // no alpha-blending, then do the crappy dithered shadow
        if(iPlant==8)
            {
            DIShadeRoundRect(hdc, &rcshadow);
            }
        else
            {
            DIShadeEllipse(hdc, &rcshadow);
            }
        }
    }


void GRDrawPot(HWND hwnd, HDC hdc, int iPot)
    {
    BITMAP bm;
    RECT   rcwnd;
    int    iplant;
    int    ipotwid;
    int    ipoty;
    int    ipotx = DP_Plant[iPot].ptLightOrigin.x + (gLampWidth/2);
    int    iRoom = TBGetComboGrowroom();
    // draw the pot if we've got the image
    if(hPotListBmp != 0)
        {
        iplant = GR_Room[iRoom-1].iPotType;
        // calculate some sizes
        GetObject(hPotListBmp, sizeof(BITMAP), (LPSTR)&bm);
        GetClientRect(hwnd, &rcwnd);
        ipotwid = bm.bmWidth/gMaxPots;
        ipoty   = rcwnd.bottom - bm.bmHeight - 6;
        // draw this pot's shadow
        GRDrawPotShadow(hdc, iPot, iplant, ipotx, ipotwid, rcwnd.bottom);
        // now draw the pot bitmap
        DIDrawTransparentSection(hdc, hPotListBmp, hPotListPal,
                                 iplant*ipotwid, 0, ipotwid, bm.bmHeight,
                                 (short)(ipotx - ipotwid/2), (short)ipoty, 
                                 RGB(255,255,255));
        }
    }

/******************************************************************************\
*  HERE WE DRAW EACH PLANT'S NAME
\******************************************************************************/


void GRDrawPlantName(HDC hdc, int iPlant, int iRoom)
    {
    int iRoomOfs = (iRoom-1)*3;
    // ----
    // if we have a plant growing here...
    if(PI_PlantSeed[iPlant+iRoomOfs].szPlantName[0])
        { 
        RECT   rcName;
        // first calculate the position of the name rectangle
        rcName.left   = DP_Plant[iPlant].ptLightOrigin.x+25;
        rcName.top    = DP_Plant[iPlant].ptLightOrigin.y+20-gLampHeight;
        rcName.right  = rcName.left+95;
        rcName.bottom = rcName.top+19;
        // now select the objects we'll require
        SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        SelectObject(hdc, GetStockObject(BLACK_PEN));
        SelectObject(hdc, hNameFont);
        SetBkMode(hdc, TRANSPARENT);
        // first draw the rectangle
        RoundRect(hdc, rcName.left, rcName.top, rcName.right, rcName.bottom, 6, 6);
        // now draw the plant's name in the rectangle
        DrawTextEx(hdc, PI_PlantSeed[iPlant+iRoomOfs].szPlantName, 
                   strlen(PI_PlantSeed[iPlant+iRoomOfs].szPlantName), &rcName, 
                   DT_SINGLELINE|DT_CENTER|DT_END_ELLIPSIS|DT_VCENTER, NULL);
        }
    }

/******************************************************************************\
*  HERE WE DRAW THE VACATION MODE ENABLED TEXT IN THE TOP RIGHT HAND CORNER
\******************************************************************************/


void GRDrawBackgroundLeafRect(HDC hdc, RECT rc)
    {
    RECT rcPanel;
    // ----------
    CopyRect(&rcPanel, &rc);
    // draw the embossed rectangle
    Rectangle3D(hdc, rcPanel, 
                GetStockObject(BLACK_PEN), GetStockObject(LTGRAY_BRUSH),
                GetStockObject(BLACK_PEN), GetStockObject(WHITE_PEN));
    // now draw the leaf tiled background
    if(-1==GetDeviceCaps(hdc, NUMCOLORS))
         { // if we're in high colour, we'll draw the light leaf background
         InflateRect(&rcPanel, -2, -2);
         DIRectTileDlgLeafBitmap(hdc, rcPanel);
         }
    }

void GRDrawVacationModeText(HWND hwnd, HDC hdc, int iRoom)
    {
    RECT rcwnd;
    RECT rctext;
    HFONT  hfont;
    HPEN   hpen;
    int imode = 0;
    int iRoomOfs = (iRoom-1)*3;
    // getout if vacation mode is not enabled
    if(PACheckVacModeEnabled()==FALSE) return;
    // now, if we have any plants growing in this room...
    if((PI_PlantSeed[iRoomOfs+0].szPlantName[0])||
       (PI_PlantSeed[iRoomOfs+1].szPlantName[0])||
       (PI_PlantSeed[iRoomOfs+2].szPlantName[0]))
        {
        GetClientRect(hwnd, &rcwnd);
        hfont  = SelectObject(hdc, hVacModeFont);
        hpen   = SelectObject(hdc, GetStockObject(BLACK_PEN));
        imode  = SetBkMode(hdc, TRANSPARENT);
        // first draw the text to measure it
        DrawTextEx(hdc, "Vacation Mode", -1, &rctext,
                   DT_SINGLELINE|DT_CENTER|DT_CALCRECT, NULL);
        // now set the rect for our growroom window
        rcwnd.left   = rcwnd.right - (rctext.right - rctext.left) - 10;
        rcwnd.bottom = rcwnd.top +   (rctext.bottom - rctext.top) + 8;
        // now draw the 3d rectangle
        GRDrawBackgroundLeafRect(hdc, rcwnd);
        // now draw the text
        InflateRect(&rcwnd, -5, -4);
        DrawTextEx(hdc, "Vacation Mode", -1, &rcwnd, DT_SINGLELINE|DT_CENTER, NULL);
        // clean up by reselecting our objetcs
        SelectObject(hdc, hfont);
        SelectObject(hdc, hpen);
        SetBkMode(hdc, imode);
        }
    }

/******************************************************************************\
*  HERE WE RENDER THE ENTIRE GROWROOM OFFSCREEN AND THEN BITBLT IT ONSCREEN
\******************************************************************************/

void GRDrawEggRect(HDC hdc)
    {
    RECT rc;

    // draw the easter egg activation rect
    SetRect(&rc, 590, 170, 620, 200);
    FillRect(hdc, &rc, GetStockObject(BLACK_BRUSH));
    // draw the boombox switching and menu rect
    SetRect(&rc, 535, 340, 625, 395);
    FillRect(hdc, &rc, GetStockObject(BLACK_BRUSH));
    }


void GRRenderPlantsOffscreen(HWND hwnd, int iRoom)
    { // here we draw the growroom, the pots, plants, and lights
    // NOTE: we will do this offscreen, to prevent flicker
    static HANDLE hObject;
    RECT   rcWin;
    RECT   rcPanel ={-1,-1,61,171};
    int    iWidth, iHeight;
    int    i;
    int    iNodes;
    int    iPotYadj = 0;
    int    iRoomAdj = (iRoom-1)*3;
    HINSTANCE hInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
    // ------------
    // getout if we don't have an offscreen dc
    if(hdcOffScreen==0)  return;
    // light up the toolbar progress indicator
    TBInitProgress(24, "Rendering Grow Room..");
    TBShowNextProgress();
    // now read the growroom window's size
    GetClientRect(hwnd, &rcWin);
    // set these locals for ease of use
    iWidth  = rcWin.right-rcWin.left;
    iHeight = rcWin.bottom-rcWin.top;
    // if this is the test room we paint the whole dc light grey
    FillRect(hdcOffScreen, &rcWin, GetStockObject(LTGRAY_BRUSH));
    // also draw the height measurement horizontal lines
//    GRDrawHeightMeasureLines(hwnd, hdcOffScreen);
    // now draw the growroom bitmap image
    if(hGrowRoomBmp)
        {
        DIDrawBitmap(hdcOffScreen, 0, 0, hGrowRoomBmp, hGrowRoomPal, SRCCOPY);
        }
    else
        {
        HGIRenderGrowroomImage(hdcOffScreen, rcWin);
        }
    // ---
    TBShowNextProgress();
    // now we must draw the lamp images
    for(i=0;i<3;i++)
        { // Now we draw the lamp image
        GRDrawLamp(hdcOffScreen, i);
        TBShowNextProgress();
        }
    // now we must draw the plant names
    for(i=0;i<3;i++)
        { // if we have a plant growing, draw it's name
        GRDrawPlantName(hdcOffScreen, i, iRoom);
        TBShowNextProgress();
        }
    // here we draw each pot and shadow
    for(i=0;i<3;i++)
        { // draw each offscreen pot and pot shadow
        GRDrawPot(hwnd, hdcOffScreen, i);
        TBShowNextProgress();
        }
    // here we draw the easter egg boombox in the bottom right corner
    if(gbBoomBox)
        GRDrawBoomBox(hwnd, hdcOffScreen);
    // now we can draw the plants over everything else
    for(i=0;i<3;i++)
        {
        iPotYadj = GRGetPlantOffset(iRoom);
        iNodes   = GRCalcNumDrawNodes(i);
        TBShowNextProgress();
        // now we draw the node stems
        if(iNodes) GRDrawPlantNodes(hdcOffScreen, i, iNodes, iPotYadj);
        TBShowNextProgress();
        // and now we draw the node leaves
        if(iNodes) GRDrawPlantLeaves(hdcOffScreen, i, iRoomAdj+i, iNodes, iPotYadj);
        TBShowNextProgress();
        // also draw the flowers at flowering time
        if(iNodes) GRDrawPlantBuds(hdcOffScreen, i, iRoomAdj+i, iNodes, iPotYadj);
        TBShowNextProgress();
        }
    // here we fill the growroom with the required darkness
    GRTileDarknessAndLightness(hdcOffScreen, rcWin, iRoom-1);
    TBShowNextProgress();
    // add up the number of gauges required
    i=0;
    if(GR_Room[iRoom-1].bClockGauge==TRUE) i+=1;
    if(GR_Room[iRoom-1].bTempGauge ==TRUE) i+=1;
    if(GR_Room[iRoom-1].bHygroGauge==TRUE) i+=1;
    if(GR_Room[iRoom-1].bZoomGauge ==TRUE) i+=1;
    // if we have any gauges, draw the rect
    if(i>=0)
        { // set the bottom edge of the rect
        rcPanel.bottom = i*56+2;
        // draw the embossed rectangle
        GRDrawBackgroundLeafRect(hdcOffScreen, rcPanel);
        }
    // draw the vacation mode text if required
    GRDrawVacationModeText(hwnd, hdcOffScreen, iRoom);
    // draw the easter egg valid double-click rectangle
//    GRDrawEggRect(hdcOffScreen);
    // hide our calculation progress control again
    TBEndProgress();
    }


/******************************************************************************\
*  RE-RENDERING AND DRAWING THE GROWROOM WINDOW
\******************************************************************************/

BOOL GRReRenderGrowRoom(void)
    { // sends an Invalidate if we've got a growroom window
    int iRoom = TBGetComboGrowroom();
    // getout if not in a growroom
    if(!hGrowRoomWindow) return FALSE;
    // first render the plants
    GRRenderPlantsOffscreen(hGrowRoomWindow, iRoom);
    // and now update the entire window
    return InvalidateRect(hGrowRoomWindow, NULL, TRUE);
    }


/******************************************************************************\
*  CONDITIONS CHECKING TIMER ACTIONS
\******************************************************************************/

void GRUpdateGrowRoom(HWND hwnd)
    { 
    // called during the timer message of the main window
    // to check if we must redraw the plants based on the lamp
    // on/off status
    int  i;
    BOOL bPrevStatus; 
    BOOL bIsTrue   = FALSE;
    if(hGrowRoomWindow);
        {
        int iQuarters = GLGetTimeNowQuarters();
        // first update the time shown on our clock
        TBUpdateClock();
        // now check the lighting situation for each plant
        for(i=0;i<3;i++)
            {
            bPrevStatus = DP_Plant[i].bIsLightOn;
            if(bPrevStatus != GRCheckIfLightIsOn(i, iQuarters))
                  bIsTrue = TRUE;
             }
        if(bIsTrue == TRUE)
             GRReRenderGrowRoom();
        }
    }

/******************************************************************************\
*  HERE WE BITBLT OUR OFFSCREEN DC ONSCREEN
\******************************************************************************/

void GRDrawPlants(HDC hdc, RECT rc)
    { // here we draw the growroom, the pots, plants, and lights
    // first select the correct 256 colour palette
    HPALETTE hPal = SelectPalette(hdc,hGrowRoomPal,TRUE);
    // now we must realize it into the DC
    RealizePalette(hdc);
    // we must bitblt the offscreen DC onto the screen
    BitBlt(hdc, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, 
           hdcOffScreen, rc.left, rc.top, SRCCOPY);
    // and reselect our previous palette
    SelectPalette(hdc,hPal,FALSE);
    }

/******************************************************************************\
*  RESELECTING AND REALISING OUR PALETTE
\******************************************************************************/


void GRRealizePalette(void)
    { 
    // after a toolbar property sheet, we must realize our growroom palette 
    // again for some reason
    HDC      hdc  = GetDC(hGrowRoomWindow);
    HPALETTE hPal = SelectPalette(hdc,hGrowRoomPal,FALSE);
    // now we must realize it into the DC
    RealizePalette(hdc);
    // and reselect our previous palette
    SelectPalette(hdc,hPal,FALSE);
    // and release our temp dc
    ReleaseDC(hGrowRoomWindow, hdc);
    }


/******************************************************************************\
*  CHECKING WHICH PLANT HE CLICKED ON (OR MOVED THE MOUSE OVER)
\******************************************************************************/

int GRCheckPlantClick(HWND hwnd, int xPos)
    {
    RECT rcWin;                   // rectangle of client window
    int  iWidth = 0;              // width of each plant rectangle
    // first get the client area size
    GetClientRect(hwnd, &rcWin);
    // now set the width of each of 3 valid rectangles
    iWidth = (rcWin.right-rcWin.left)/3;
    // if he's clicked in the first third
    if(xPos <= iWidth)       return 0; // set plant 1
    else  // not in the first third
        { // has he clicked in the second third?
        if(xPos <= iWidth*2) return 1; // set to plant 2 (ob0)
        }
    return 2; // it must be this one then
    }


/******************************************************************************\
*  DRAWING THE FRAME WHILE HE'S DRAGGING THE LIGHT WITH HIS MOUSE
\******************************************************************************/


void GRUnDrawLightDragRect(HWND hwnd)
    {
    // if he's been dragging the light and he releases the mouse button,
    // we undraw the drag rectangle, and zero it for next time.
    // we must also unset the mouse cursor to an open hand again
    HDC    hdc;
    HPEN   hPrevPen;
    HBRUSH hPrevBrush;
    int    iPrevROP;
    // if we have a bounding rectangle
    if(rcCurMouseLight.left!=0) 
        {
        hdc        = GetDC(hwnd); // gets the DC to draw into
        hPrevPen   = SelectObject(hdc, GetStockObject(BLACK_PEN));
        hPrevBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        iPrevROP   = SetROP2(hdc, R2_NOT);
        // now undraw the rect
        RoundRect(hdc, rcCurMouseLight.left,  rcCurMouseLight.top, 
                       rcCurMouseLight.right, rcCurMouseLight.bottom, 10, 10);
        // now we can reset our previous drawing mode, pen and brush
        SetROP2(hdc, iPrevROP);
        SelectObject(hdc, hPrevPen);
        SelectObject(hdc, hPrevBrush);
        // finally we can release our DC again
        ReleaseDC(hwnd, hdc);
        // reset the rect left pos to indicate NO rect
        rcCurMouseLight.left = 0; 
        }
    }


void GRDrawLightDragRect(HWND hwnd, LPARAM lParam, int iRoom)
    { 
    // if he's clicked and dragging the lamp, we must show a bounding
    // rectangle indicating the lamp height when he releases the button
    HDC    hdc  = GetDC(hwnd); // gets the DC to draw into
    HPEN   hPrevPen   = SelectObject(hdc, GetStockObject(BLACK_PEN));
    HBRUSH hPrevBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    int    iPlant   = GRCheckPlantClick(hwnd, LOWORD(lParam));
    int    iPrevROP = SetROP2(hdc, R2_NOT);
    int    iYofs    = GRGetPlantOffset(iRoom);
    POINT  ptMouse;
    if(rcCurMouseLight.left==0) 
        { // if we have already started dragging, undraw our rect
        rcCurMouseLight.left  = DP_Plant[iPlant].ptLightOrigin.x;
        rcCurMouseLight.right = rcCurMouseLight.left + gLampWidth;
        }
    else
        { // else we must undraw the bounding rect
        RoundRect(hdc, rcCurMouseLight.left,  rcCurMouseLight.top, 
                       rcCurMouseLight.right, rcCurMouseLight.bottom, 10, 10);
        }
    // now calculate and draw the new bounding rectangle
    // we use current mouse Y pos as the bottom of our bounding rect
    // and limit it between the top of the plant and the roof
    rcCurMouseLight.bottom = HIWORD(lParam);
    rcCurMouseLight.top    = rcCurMouseLight.bottom-gLampHeight;
    // draw the new bounding rect
    RoundRect(hdc, rcCurMouseLight.left,  rcCurMouseLight.top, 
                   rcCurMouseLight.right, rcCurMouseLight.bottom, 10, 10);

    // we must ensure than the mouse cursor X-pos stays within the
    // valid lamp area (ie. above the plant)
    ptMouse.x = LOWORD(lParam);
    ptMouse.y = min(RC_DrawPlant[iPlant].top+iYofs, HIWORD(lParam));
    if(HIWORD(lParam)<10)
        {
        ptMouse.y = 10;
        ClientToScreen(hwnd, &ptMouse);
        SetCursorPos(ptMouse.x, ptMouse.y);
        }
    if(HIWORD(lParam)>(RC_DrawPlant[iPlant].top+iYofs))
        {
        ClientToScreen(hwnd, &ptMouse);
        SetCursorPos(ptMouse.x, ptMouse.y);
        }
    if(LOWORD(lParam)<rcCurMouseLight.left)
        {
        ptMouse.x = rcCurMouseLight.left;
        ClientToScreen(hwnd, &ptMouse);
        SetCursorPos(ptMouse.x, ptMouse.y);
        }
    if(LOWORD(lParam)>rcCurMouseLight.right)
        {
        ptMouse.x = rcCurMouseLight.right;
        ClientToScreen(hwnd, &ptMouse);
        SetCursorPos(ptMouse.x, ptMouse.y);
        }
    // now we can reset our previous drawing mode, pen and brush
    SetROP2(hdc, iPrevROP);
    SelectObject(hdc, hPrevPen);
    SelectObject(hdc, hPrevBrush);
    // finally we can release our DC again
    ReleaseDC(hwnd, hdc);
    }


/******************************************************************************\
*  CHECKING IF HE STARTED THE EASTER EGG (SHIFT+CTRL+DBLCLICK ON THE FAN CENTER)
\******************************************************************************/


BOOL GRStartEasterEgg(HWND hwnd, HINSTANCE hInst, POINT ptClick)
    { // if he shift-double-clicked on the fan, 
    // start up the easter egg toonfile
    RECT rc, rcWin;
    // first set our double click area
    GetClientRect(hwnd, &rcWin);
    SetRect(&rc, rcWin.left+590, rcWin.top+170, 
                 rcWin.left+620, rcWin.top+200);
    // if he did click there...
    if(PtInRect(&rc, ptClick))
        {
        if((GetKeyState(VK_SHIFT)<0)
          &&(GetKeyState(VK_CONTROL)<0))
            { // enable the boombox in the registry
            PASetBoomBox(TRUE);
            // redraw the growroom to show the boombox
            GRReRenderGrowRoom();
            return TBStartRobbieToonfile(hwnd, "OBEgg");
            }
        }
    return FALSE;
    }


/******************************************************************************\
*  CHECKING IF HE RIGHT-CLICKED ON THE BOOMBOX IMAGE
\******************************************************************************/


BOOL GRCheckBoomBoxAction(HWND hwnd, HINSTANCE hInst, POINT ptClick, BOOL bLeftClick)
    { // if he shift-double-clicked on the fan, 
    // start up the easter egg toonfile
    RECT rc, rcWin;
    HMENU hBoomMenu;
    char szResult[20];
    UINT uiState;
    // ------------
    if(gbBoomBox == FALSE) return FALSE;
    // first set our right click area
    GetClientRect(hwnd, &rcWin);
    SetRect(&rc, rcWin.left +535, rcWin.top    +340, 
                 rcWin.right+625, rcWin.bottom +395);
    // if he did click there...
    if(PtInRect(&rc, ptClick))
        {
        if(bLeftClick)
            {
            PlaySound("BOOMCLICK", hInst, SND_RESOURCE|SND_SYNC);
            TBStartStopMusicPlayback(GetParent(hwnd));
            }
        else
            { // first we load the boombox context menu
            hBoomMenu = LoadMenu(hInst, "BOOMBOXMENU");
            if(hBoomMenu)    
                {
                HMENU hSubMenu = GetSubMenu(hBoomMenu, 0);
                // first we must change the text of the playback menu item
                if((bMidiPlaying==TRUE)||(bMP3Playing==TRUE)) 
                    {
                    strcpy(szResult, "Stop Music Playback");
                    uiState = MF_CHECKED;
                    }
                else
                    {
                    strcpy(szResult, "Start Music Playback");
                    uiState = MF_UNCHECKED;
                    }
                ModifyMenu(hSubMenu,IDM_SOUND,MF_BYCOMMAND|MF_STRING,IDM_SOUND, szResult);
                CheckMenuItem(hSubMenu,IDM_SOUND,MF_BYCOMMAND|uiState);
                // now we show the popup menu        
                ClientToScreen(hwnd, &ptClick);
                TrackPopupMenu(hSubMenu, TPM_BOTTOMALIGN|TPM_LEFTALIGN,
                                ptClick.x, ptClick.y, 0, hwnd, NULL);
                // now we can delete the menu again
                DeleteObject(hBoomMenu);
                }
            }
        return TRUE;
        }
    return FALSE;
    }


BOOL GRCheckBoomBoxMouseMove(HWND hwnd, POINT ptMouse)
    {
    RECT rc, rcWin;
    // ------------
    if(gbBoomBox == FALSE) return FALSE;
    // first set our right click area
    GetClientRect(hwnd, &rcWin);
    SetRect(&rc, rcWin.left +535, rcWin.top    +340, 
                 rcWin.right+625, rcWin.bottom +395);
    // if he did click there...
    return PtInRect(&rc, ptMouse);
    }


/******************************************************************************\
*  MOUSE RIGHT BUTTON ACTIONS
\******************************************************************************/

void GRCheckRightButtonDownAction(HWND hwnd, HINSTANCE hInst, LPARAM lParam)
    { // Here we handle the left button single click
    // We will set the clicked plant in our toolbar combo
    int  iRoom    = TBGetComboGrowroom();
    int  iRoomOfs = (iRoom-1)*3;
    int  iPot     = GRCheckPlantClick(hwnd, LOWORD(lParam));
    int  iPlant   = iPot+iRoomOfs;
    POINT  ptMouse;          // current cursor position over window
    // first we'll convert the lParam to a point position
    ptMouse.x = LOWORD(lParam); // horizontal position of cursor
    ptMouse.y = HIWORD(lParam); // vertical position of cursor

    // the easter egg is hidden in the Attic, don't bother checking if not there
    if(iRoom==2)
        { // if he shift-double-clicked on the fan, start up the easter egg toonfile
        if(GRStartEasterEgg(hwnd, hInst, ptMouse)) return;
        }
    // now we check if he right-clicked on the boombox
    if(GRCheckBoomBoxAction(hwnd, hInst, ptMouse, FALSE)) return;
    // now we check which plant he clicked on
    ClientToScreen(hwnd, &ptMouse);
    // now we exit if this plant is actually growing yet
    if(PI_PlantSeed[iPlant].szStartDate[0]==0) return;
    // ok, he's clicked on a valid plant, so change the toolbar's combo
    TBSetComboPlant(iPlant);
    // if we have loaded our context menu, we show it now
    if(hContextMenu)
        {
        TrackPopupMenu(GetSubMenu(hContextMenu, 0), TPM_BOTTOMALIGN|TPM_LEFTALIGN,
                       ptMouse.x, ptMouse.y, 0, hwnd, NULL);
        }
    }


/******************************************************************************\
*  MOUSE LEFT BUTTON ACTIONS
\******************************************************************************/


void GRCheckLeftButtonDownAction(HWND hwnd, HINSTANCE hInst, LPARAM lParam)
    { // Here we handle the left button single click
    // We will set the clicked plant in our toolbar combo
    int  iWidth   = 0;              // width of each plant rectangle
    int  iRoom    = TBGetComboGrowroom();
    int  iRoomOfs = (iRoom-1)*3;
    int  iPot     = GRCheckPlantClick(hwnd, LOWORD(lParam));
    int  iPlant   = iPot+iRoomOfs;
    POINT  ptMouse;          // current cursor position over window
    RECT rcLight;

    // first we'll convert the lParam to a point position
    ptMouse.x = LOWORD(lParam); // horizontal position of cursor
    ptMouse.y = HIWORD(lParam); // vertical position of cursor
    // now we check if he left-clicked on the boombox
    if(GRCheckBoomBoxAction(hwnd, hInst, ptMouse, TRUE)) return;
    // now we exit if this plant is actually growing yet
    if(PI_PlantSeed[iPlant].szStartDate[0]==0) return;
    // now we exit if this plant is already dead
//    if(PI_PlantSeed[iPlant].cGrowthStage>=7) return;
    // ok, he's clicked on a valid plant, so change the toolbar's combo
    TBSetComboPlant(iPlant);
    // now we check if he wanted to prune a tip
    if(giCurMouseTipNode) // if this is set, he's clicked on a tip
        { // yes, try to prune this tip now
        if(NOMousePrune(iPlant, iPot, giCurMouseTipNode)==TRUE);
            { // if we managed to correctly prune this tip...
            // ...re-render the plants to show missing tip
            GRReRenderGrowRoom();
            }
        return;
        }
    // so, he didn't want to prune the tip, must we change the 
    // cursor to a closed hand if he's on the lower part of the light
    // first setup the valid rect for the bottom 10 pixels
    rcLight.left   = DP_Plant[iPot].ptLightOrigin.x;
    rcLight.top    = DP_Plant[iPot].ptLightOrigin.y+gLampHeight-10;
    rcLight.right  = rcLight.left + gLampWidth;
    rcLight.bottom = rcLight.top  + 25;
    // change the cursor if he's in the rect
    if(PtInRect(&rcLight, ptMouse))
        { // if he's finger's on the button, we close the hand
        SetCursor(hCHandCursor);
        // and draw the bounding rect
        GRDrawLightDragRect(hwnd, lParam, iRoom);
        return;
        }
    }


void GRCheckLeftDoubleAction(HWND hwnd, HINSTANCE hInst, LPARAM lParam)
    { // Here we handle the left button double click
    // If he clicks on any unplanted seeds, we show our planting dialog
    RECT  rcClick;    // current area we're concerned with
    POINT ptClick;    // point of the mouseclick
    int  iRoom    = TBGetComboGrowroom();
    int  iRoomOfs = (iRoom-1)*3;
    int  iPot     = GRCheckPlantClick(hwnd, LOWORD(lParam));
    int  iPlant   = iPot+iRoomOfs;

    ptClick.x = LOWORD(lParam); // mouse click X position
    ptClick.y = HIWORD(lParam); // mouse click Y position

    // first we check if this plant is actually growing yet
    if(PI_PlantSeed[iPlant].szStartDate[0]==0) // if not growing..
        { // show the seed planting dialog
        DialogBox(hInst, "Planting", hwnd, SLSeedDlgProc);
        return;
        }

    // now we can check if he double-clicked on the plant
    if(PtInRect(&RC_DrawPlant[iPot], ptClick) == TRUE)
        { // we'll show him the Information Activity Page
        TBStartActivitySheet(hwnd, hInst, IDM_STATS);
        return;
        }
    // now we check if he double-clicked on the pot
    // first we set the rect where this pot is
    rcClick.left   = DP_Plant[iPot].DN_Plant[0].ptDNStart.x - 60;
    rcClick.top    = gPlantStartY;
    rcClick.right  = rcClick.left + 120;
    rcClick.bottom = gPlantStartY + 80;
    // now if he double-clicked within the pot rect...
    if(PtInRect(&rcClick, ptClick) == TRUE)
        { // ...we'll show him the Watering Activity Page
        TBStartActivitySheet(hwnd, hInst, IDM_WATER);
        return;
        }
    // now we check if he double-clicked on the light
    // first we set the rect where this light is
    rcClick.left   = DP_Plant[iPot].ptLightOrigin.x;
    rcClick.top    = DP_Plant[iPot].ptLightOrigin.y;
    rcClick.right  = rcClick.left + gLampWidth;
    rcClick.bottom = rcClick.top  + gLampHeight;
    // now if he double-clicked within the light rect...
    if(PtInRect(&rcClick, ptClick) == TRUE)
        { // ...we'll show him the Lighting Activity Page
        TBStartActivitySheet(hwnd, hInst, IDM_LIGHT);
        return;
        }

/*
    sprintf(strbuf, "X=%i, Y=%i, X=%i, Y=%i and clicked at X=%i, Y=%i", 
            rcClick.left, rcClick.top, rcClick.right, rcClick.bottom,
            ptClick.x, ptClick.y);
    MessageBox(NULL, (LPCSTR)strbuf, "Message", MB_OK|MB_ICONEXCLAMATION);
*/    
    }


/******************************************************************************\
*  MOUSE MOVING OVER GROWING TIPS OR DRAGGING THE LIGHT ACTIONS
\******************************************************************************/

BOOL GRCheckMouseMoveAction(HWND hwnd, WPARAM wParam, LPARAM lParam)
    { // Here we handle the mousemove messages
    // If he moves over any tips, we'll show the pruning cursor and set
    // the giCurTipNode global (in case he clicks to prune this tip)
    POINT  ptMouse;          // current cursor position over window
    RECT   rcTip, rcLight;   // valid area around the tip he's over
    int    i, j, k, iTip;    // simple little locals
    int  iRoom     = TBGetComboGrowroom();
    int  iRoomOfs  = (iRoom-1)*3;
    int  iPlantOfs = GRGetPlantOffset(iRoom);
    int  iPot      = GRCheckPlantClick(hwnd, LOWORD(lParam));
    int  iPlant    = iPot+iRoomOfs;

    // first we'll convert the lParam to a point position
    ptMouse.x = LOWORD(lParam); // horizontal position of cursor
    ptMouse.y = HIWORD(lParam); // vertical position of cursor

    // now we check if he left-clicked on the boombox
    if(GRCheckBoomBoxMouseMove(hwnd, ptMouse)) 
        {
        SetCursor(hBHandCursor);
        return FALSE;
        }
    
    // now we exit if this plant is actually growing yet
    if(PI_PlantSeed[iPlant].szStartDate[0]==0) 
        {
        SetCursor(hArrowCursor);
        return FALSE;
        }
    // now we exit if this plant is already dead
    if(PI_PlantSeed[iPlant].cGrowthStage>=7)
        {
        SetCursor(hArrowCursor);
        return FALSE;
        }
   
    // now we must check if he's over the bottom part of the light
    // first setup the valid rect for the bottom 10 pixels
    rcLight.left   = DP_Plant[iPot].ptLightOrigin.x;
    rcLight.top    = DP_Plant[iPot].ptLightOrigin.y+gLampHeight-10;
    rcLight.right  = rcLight.left + gLampWidth;
    rcLight.bottom = rcLight.top  + 15;
    // change the cursor if he's in the rect OR he's dragging
    if(PtInRect(&rcLight, ptMouse)||rcCurMouseLight.left!=0)
        { // if he's finger's on the button, we close the hand
        if(wParam == MK_LBUTTON)
            {
            GRDrawLightDragRect(hwnd, lParam, iRoom);
            SetCursor(hCHandCursor);
            }
        else SetCursor(hOHandCursor);
        return TRUE;
        }
    // now we check if he's within our plant's rect
    if(PtInRect(&RC_DrawPlant[iPot], ptMouse) == TRUE)
        { // so he's moving over the plant, but is it over a tip?
        for(i=0;i<gMaxTipNodes;i++)
            { // we'll check each tip in our TIPNODE struct
            if(TN_Plant[iPot].iTipNodes[i] > 0) // tip can't be node 0
                { 
                iTip = TN_Plant[iPot].iTipNodes[i];
                // now we must set up the rect for this tip
                j = DP_Plant[iPot].DN_Plant[iTip].ptDNStart.x;
                k = DP_Plant[iPot].DN_Plant[iTip].ptDNEnd.x;
                rcTip.left   = min(j, k)-1; // ensures correct order
                rcTip.right  = max(j, k)+1; // ensures correct order
                rcTip.top    = iPlantOfs+DP_Plant[iPot].DN_Plant[iTip].ptDNEnd.y-1;
                rcTip.bottom = iPlantOfs+DP_Plant[iPot].DN_Plant[iTip].ptDNStart.y+1;
                if(PtInRect(&rcTip, ptMouse))
                    {
                    // set the cursor to an arrow
                    SetCursor(hPruneCursor);
                    // and set the node number global for this tip
                    giCurMouseTipNode = iTip;
                    return TRUE;
                    }
                }
            }
        }
    // reset this if he's not over it
    giCurMouseTipNode = 0; 
    // else set the cursor to an arrow
    SetCursor(hArrowCursor);
   
    return FALSE;
    }


/******************************************************************************\
*  MOUSE BUTTON UP - CHECK IF HE'S DRAGGED THE LAMP AND REDRAW IF HE HAS
\******************************************************************************/


BOOL GRMouseChangeLight(int iPot, int iPlant, int iHeight)
    { // adjust the light for the given plant after dragging the lamp
    char szFile[128]="\0";
    BOOL bSuccess=FALSE;

    giCurPlant = iPlant+1;
    sprintf(szFile, "Plant%02i.hgp", giCurPlant);
    if(GLDoesFileExist(szFile))
        {
        if(PMPlantFileToMem(iPlant))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            // now load and fill our current growday variables
            CACalculatePlant(P_Plant, iPlant);
            // let's adjust the light height for this plant then
            giLightHeight = iHeight;
            // save our new current growday structs
            GLSaveGrowthVariables(P_Plant);
            // calculate it's new position on the screen
            GRCalcDrawLamp(P_Plant, iPot, TRUE);
            // and then free our memory again
            PMPlantMemToFile(iPlant);
            bSuccess = TRUE;
            }
        }
    return bSuccess;
    }


void GRCheckLeftButtonUpAction(HWND hwnd, LPARAM lParam)
    { // Here we handle the left button single click
    // We will set the clicked plant in our toolbar combo
    int  iWidth = 0; // width of each plant rectangle
    int  iHeight= 0; // new height of light if he moved it
    POINT ptMouse;          // current cursor position over window
    RECT  rcLight;
    int   iRoom    = TBGetComboGrowroom();
    int   iRoomOfs = (iRoom-1)*3;
    int   iPot     = GRCheckPlantClick(hwnd, LOWORD(lParam));
    int   iPlant   = iPot+iRoomOfs;

    // first we'll convert the lParam to a point position
    ptMouse.x = LOWORD(lParam); // horizontal position of cursor
    ptMouse.y = HIWORD(lParam); // vertical position of cursor
    // now we exit if this plant is actually growing yet
    if(PI_PlantSeed[iPlant].szStartDate[0]==0) return;

    // if he's been dragging the light, we must open his hand, 
    // undraw the bounding rect and rerender the growroom
    if(rcCurMouseLight.left!=0) 
        {
        // only if he's moved the rect enough, we'll recalc lamp position
        if((rcCurMouseLight.top > DP_Plant[iPot].ptLightOrigin.y+10)
         ||(rcCurMouseLight.top < DP_Plant[iPot].ptLightOrigin.y-10))
            { // calculate the new height
            double dPPix=gPlantStartY/2500.00;
            iHeight = 19 - (int)(rcCurMouseLight.bottom/dPPix/125);
            // now do all the redrawing stuff
            GRUnDrawLightDragRect(hwnd);
            GRMouseChangeLight(iPot, iPlant, iHeight);
            GRReRenderGrowRoom();
            }
        else // he hasn't moved more than 10 pixels up or down...
            { // .. so just undraw the rect..
            GRUnDrawLightDragRect(hwnd);
            // show the open hand cursor again
            SetCursor(hOHandCursor);
            }
        return;
        }
    // ok, so he didn't just finish dragging the lamp
    // we must change the cursor to an open  hand if he's on the 
    // lower part of the light
    // first setup the valid rect for the bottom 10 pixels
    rcLight.left   = DP_Plant[iPot].ptLightOrigin.x;
    rcLight.top    = DP_Plant[iPot].ptLightOrigin.y+gLampHeight-10;
    rcLight.right  = rcLight.left + gLampWidth;
    rcLight.bottom = rcLight.top  + 15;
    // change the cursor if he's in the rect
    if(PtInRect(&rcLight, ptMouse))
        { // if he's finger's on the button, we close the hand
        SetCursor(hOHandCursor);
        return;
        }
    }

/******************************************************************************\
*  CREATING AND DESTROYING GROWROOM CLOCKS
\******************************************************************************/

HWND hClockWnd; // analog clock window handle
HWND hThermWnd; // analog thermometer window handle
HWND hHygroWnd; // analog hygrometer window handle
HWND hZoomWnd;  // zoomer window handle

void GRCreateGrowroomGauges(HWND hwnd, HINSTANCE hInst, int iRoom)
    {
    RECT rcClock = {5,0,50,50};
    rcClock.top = 5;
    if(GR_Room[iRoom].bClockGauge==TRUE)       
         { // true to display a clock gauge
         hClockWnd    = CLCreateClockWindow(hwnd, hInst, rcClock, STYLE_CLOCK);
         rcClock.top += rcClock.bottom+5;
         }
    if(GR_Room[iRoom].bTempGauge==TRUE)       
         { // true to display a thermometer gauge
         hThermWnd    = CLCreateClockWindow(hwnd, hInst, rcClock, STYLE_THERM);
         rcClock.top += rcClock.bottom+5;
         }
    if(GR_Room[iRoom].bHygroGauge==TRUE)       
         { // true to display a hygrometer gauge
         hHygroWnd    = CLCreateClockWindow(hwnd, hInst, rcClock, STYLE_HYGRO);
         rcClock.top += rcClock.bottom+5;
         }
    if(GR_Room[iRoom].bZoomGauge==TRUE)       
         { // true to display a hygrometer gauge
         hZoomWnd    = CLCreateClockWindow(hwnd, hInst, rcClock, STYLE_ZOOM);
         rcClock.top += rcClock.bottom+5;
         }
    }

void GRDestroyGrowroomGauges(void)
    {
    if(hClockWnd) DestroyWindow(hClockWnd); 
    if(hThermWnd) DestroyWindow(hThermWnd); 
    if(hHygroWnd) DestroyWindow(hHygroWnd); 
    if(hZoomWnd)  DestroyWindow(hZoomWnd); 
    hClockWnd = 0;
    hThermWnd = 0;
    hHygroWnd = 0;
    hZoomWnd  = 0;
    }

void GRRecreateGrowroomGauges(HINSTANCE hInst, int iRoom)
    { // reloads the OB1 growroom guages
    // getout if we're not in the growroom
    if(!hGrowRoomWindow) return;
    // first we destroy any gauges that are presently there
    GRDestroyGrowroomGauges();
    // get room number OB0
    GRCreateGrowroomGauges(hGrowRoomWindow, hInst, iRoom-1);
    }

/******************************************************************************\
*  RELOADING THE GROWROOM BACKGROUND IMAGE AFTER HE'S EDITED A PRIVATE GROWROOM
\******************************************************************************/


void GRReloadGrowroomImage(HWND hWnd, HINSTANCE hInst, int iRoom)
    { // reloads the OB1 growroom image
    char szRoomName[50]   ="\0";
    char szpath[MAX_PATH] ="\0";
    char szDestFile[MAX_PATH]="\0";
    UINT uiRes = IDR_EMPTYROOM;
    // ---
    // free the existing image if we've loaded one
    if(hGrowRoomBmp) DIFreeBitmap(hGrowRoomBmp, hGrowRoomPal);
    HGIFreeGrowroomImage();
    // zero the globals in case the next load fails
    hGrowRoomBmp  = 0;  
    hGrowRoomPal  = 0;
    // get the name of this room from the combo
    TBGetComboGrowroomName(szRoomName);
    // load standard image in the first 3 rooms
    if(iRoom <= 3)
        { 
        switch (iRoom)
          {
          case 1:
              uiRes = IDR_BEDROOM;
          break;
          case 2:
              uiRes = IDR_BASEMENT;
          break;
          case 3:
              uiRes = IDR_ATTIC;
          break;
          }
        HGILoadGrowroomResourceImage(hInst, uiRes);
        }
    else
        {
        GetCurrentDirectory(MAX_PATH, (LPTSTR)&szpath);
        strcat(szpath, "\\");
        strcat(szpath, szRoomName);
        strcat(szpath, ".hgb");
        if(GLDoesFileExist(szpath))
            { // Try to convert and load this Grow Room Image file
            // first setup a name for the destination file
            GLGetTempFilePathAndName(MAX_PATH, szDestFile);
            // now try to convert the image file into the jpg format
            if(CRConvertAndDecryptImageFile(szpath, szDestFile))
                {
                HGILoadGrowroomFileImage(szDestFile);
                DeleteFile(szDestFile);
                }
            else
                { // else we load our empty room image in case he has deleted this image file
                HGILoadGrowroomResourceImage(hInst, uiRes);
                }
            }
        else
            { // else we load our empty room image in case he has deleted this image file
            HGILoadGrowroomResourceImage(hInst, uiRes);
            }
        }
    }


/******************************************************************************\
*  THE GROWROOM WINPROC
\******************************************************************************/
 
LRESULT CALLBACK GrowRoomWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
  static HANDLE hCompatibleBmp;
  HINSTANCE ghInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

  switch (message)
    {
    case WM_CREATE:
         {
         int  i;
         int  iRoom = TBGetComboGrowroom();  // OB1 
         HDC  hdc;
         RECT rc; // window is created at the correct size
         COLORREF crStemBrown = RGB(128,128,0);
         COLORREF crMaskWhite = RGB(255,255,255);
         // -----------------
         // lets load our leaf and bud images early on
         LFLoadLeafImages(ghInst);
         BDLoadBudImages(ghInst);
         // now load our context menu
         hContextMenu = LoadMenu(ghInst, "CONTEXTMENU");
         // now create those cursors
         hArrowCursor = LoadCursor(NULL, IDC_ARROW);
         hPruneCursor = LoadCursor(ghInst, "scissors");
         hOHandCursor = LoadCursor(ghInst, "openhand");
         hCHandCursor = LoadCursor(ghInst, "closedhand");
         for(i=0;i<5;i++)
             hStemPens[i]=CreatePen(PS_SOLID, i+1, crStemBrown);
     
         hLLightImage = ImageList_LoadBitmap(ghInst,"LLight",240,0,crMaskWhite);
         // get the name of this room image resource or file and load it
         GRReloadGrowroomImage(hwnd, ghInst, iRoom);
         // here we load our pot images
         hPotListBmp   = DILoadResourceBitmap(ghInst, "Big_Pots", &hPotListPal);
         // now we load our light images
         hLightListBmp   = DILoadResourceBitmap(ghInst, "Big_Lights", &hLightListPal);
         // now we load our light background image
         hLightMHBmp  = LoadBitmap(ghInst, "Back_MH");
         hLightHPSBmp = LoadBitmap(ghInst, "Back_HPS");
         // we'll also load our darkness images
         hBlackImage   = ImageList_LoadBitmap(ghInst, "Black", 32, 0, crMaskWhite);
         hDarknessBmp  = LoadBitmap(ghInst, "darkness");
         // now we can load our easter egg boombox image and cursor
         hBoomBoxBmp   = DILoadResourceBitmap(ghInst, "BoomBox", &hBoomBoxPal);
         hBHandCursor  = LoadCursor(ghInst, "BoomHand");
         // we also create a black pen to draw our light electrical cord
         hCordPen     = CreatePen(PS_SOLID, 2, RGB(0,0,0));
         // create the font to draw the plant names with
         hNameFont    = CreateFont(14,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0,"MS Sans Serif");
         hVacModeFont = GLCreateDialogFont(16,0,FW_BOLD);
         // first create an offscreen dc and a compatible sized bitmap
         hdc = GetDC(hwnd);
         hdcOffScreen = CreateCompatibleDC(hdc);
         // get the window's size for our offscreen bitmap
         GetClientRect(hwnd, &rc);
         hCompatibleBmp = CreateCompatibleBitmap(hdc, rc.right-rc.left, rc.bottom-rc.top);
         SelectObject(hdcOffScreen, hCompatibleBmp);
         ReleaseDC(hwnd, hdc);
         // now we must render our plants into our offscreen DC
         GRRenderPlantsOffscreen(hwnd, iRoom);
         // now we'll also create the clock-type windows
         GRCreateGrowroomGauges(hwnd, ghInst, iRoom-1);
         // finally, we may show our window
         ShowWindow(hwnd, SW_SHOW);
         }
    break;

    case WM_COMMAND:
        {
        int iPlant = TBGetComboPlant();
        switch (LOWORD(wParam))
          {
          // --- plant menu commands
          case IDM_ACTINFO:
                   ACShowActivitySheet(hwnd, ghInst, iPlant, 0);
          return TRUE;
          case IDM_ACTLIGHTS:
                   ACShowActivitySheet(hwnd, ghInst, iPlant, 1);
          return TRUE;
          case IDM_ACTWATER:
                   ACShowActivitySheet(hwnd, ghInst, iPlant, 2);
          return TRUE;
          case IDM_ACTFERT:
                   ACShowActivitySheet(hwnd, ghInst, iPlant, 3);
          return TRUE;
          case IDM_ACTPRUNE:
                   ACShowActivitySheet(hwnd, ghInst, iPlant, 4);
          return TRUE;
          // --- boombox menu commands
          case IDM_SOUND:
               TBStartStopMusicPlayback(GetParent(hwnd));
          return TRUE;
          case IDM_HIDEBOOM:
              { // hide the boom box music player
              PASetBoomBox(FALSE);
              // redraw the growroom to show the boombox
              GRReRenderGrowRoom();
              }
          return TRUE;
          }  
        return (FALSE);
        }

    case WM_INITMENUPOPUP:
        { // we can assume he clicked on a valid plant
        HMENU hmenu = (HMENU) wParam;
        MENUITEMINFO mmi;
        // ---
        if(HIWORD(lParam)==FALSE)
            {
            int iPlant = TBGetComboPlant();
            char sztext[100];
            char szname[100];
            // ---
            strcpy(szname, PI_PlantSeed[iPlant-1].szPlantName);
            // ---
            mmi.cbSize     = sizeof(MENUITEMINFO);
            mmi.fMask      = MIIM_TYPE|MIIM_ID;
            mmi.fType      = MFT_STRING;
            mmi.fState     = MFS_ENABLED;
            mmi.hSubMenu   = hmenu;
            // ---
            mmi.wID        = IDM_ACTINFO;
            wsprintf(sztext, "Information for %s", szname);
            mmi.dwTypeData = sztext;
            SetMenuItemInfo(hmenu, mmi.wID, FALSE, &mmi);
            // ---
            mmi.wID        = IDM_ACTLIGHTS;
            wsprintf(sztext, "Lighting for %s", szname);
            mmi.dwTypeData = sztext;
            SetMenuItemInfo(hmenu, mmi.wID, FALSE, &mmi);
            // ---
            mmi.wID        = IDM_ACTWATER;
            wsprintf(sztext, "Watering for %s", szname);
            mmi.dwTypeData = sztext;
            SetMenuItemInfo(hmenu, mmi.wID, FALSE, &mmi);
            // ---
            mmi.wID        = IDM_ACTFERT;
            wsprintf(sztext, "Fertilizing for %s", szname);
            mmi.dwTypeData = sztext;
            SetMenuItemInfo(hmenu, mmi.wID, FALSE, &mmi);
            // ---
            mmi.wID        = IDM_ACTPRUNE;
            wsprintf(sztext, "Pruning for %s", szname);
            mmi.dwTypeData = sztext;
            SetMenuItemInfo(hmenu, mmi.wID, FALSE, &mmi);
            }
        }
    break;

    case WM_RBUTTONDOWN:
         GRCheckRightButtonDownAction(hwnd, ghInst, lParam);
    break;

    case WM_LBUTTONDOWN:
         GRCheckLeftButtonDownAction(hwnd, ghInst, lParam);
    break;

    case WM_LBUTTONUP:
         GRCheckLeftButtonUpAction(hwnd, lParam);
    break;

    case WM_LBUTTONDBLCLK:
         GRCheckLeftDoubleAction(hwnd, ghInst, lParam);
    break;

    case WM_MOUSEMOVE:
         GRCheckMouseMoveAction(hwnd, wParam, lParam);
    break;

    case WM_PAINT:
          {
          PAINTSTRUCT ps;
          RECT rc;
          HDC hdc = BeginPaint(hwnd, &ps);
          GetClientRect(hwnd, &rc);
          GRDrawPlants(hdc, rc);
          EndPaint(hwnd, &ps);
          }
    break;

    case WM_CLOSE:
         ShowWindow(hwnd, SW_HIDE);
         DestroyWindow(hwnd);
    break;

    case WM_PALETTECHANGED:
         GRRealizePalette();
    break;

    case WM_ENDPLAYBACK:
         TBCheckEndRulesPlayback(GetParent(hwnd));
    break;

    case WM_DESTROY:
        {
        int i;
        LFFreeLeafImages(); // free our leaf images
        BDFreeBudImages();  // free our bud images
        // first destroy the analog clock windows
        GRDestroyGrowroomGauges();
        // now delete all our objects
        if(hLLightImage)    ImageList_Destroy(hLLightImage);
        if(hBlackImage)     ImageList_Destroy(hBlackImage);
        if(hPruneCursor)    DestroyCursor(hPruneCursor);
        if(hArrowCursor)    DestroyCursor(hArrowCursor);
        if(hOHandCursor)    DestroyCursor(hOHandCursor);
        if(hCHandCursor)    DestroyCursor(hCHandCursor);
        if(hBHandCursor)    DestroyCursor(hBHandCursor);
        if(hDarknessBmp)    DeleteObject(hDarknessBmp);
        if(hCompatibleBmp)  DeleteObject(hCompatibleBmp);
        if(hLightMHBmp)     DeleteObject(hLightMHBmp);
        if(hLightHPSBmp)    DeleteObject(hLightHPSBmp);
        if(hCordPen)        DeleteObject(hCordPen);
        if(hNameFont)       DeleteObject(hNameFont);
        if(hVacModeFont)    DeleteObject(hVacModeFont);
        if(hdcOffScreen)    DeleteDC(hdcOffScreen);
        if(hContextMenu)    DeleteObject(hContextMenu);
        for(i=0;i<5;i++)
            if(hStemPens[i]) DeleteObject(hStemPens[i]);
        if(hGrowRoomBmp)    DIFreeBitmap(hGrowRoomBmp, hGrowRoomPal);
        if(hPotListBmp)     DIFreeBitmap(hPotListBmp,  hPotListPal);
        if(hLightListBmp)   DIFreeBitmap(hLightListBmp,hLightListPal);
        if(hBoomBoxBmp)     DIFreeBitmap(hBoomBoxBmp,  hBoomBoxPal);

        HGIFreeGrowroomImage();

        hGrowRoomBmp  = 0;  hGrowRoomPal  = 0;
        hPotListBmp   = 0;  hPotListPal   = 0;
        hLightListBmp = 0;  hLightListPal = 0;
        }
    break;

    default:
        return (DefWindowProc(hwnd, message, wParam, lParam));
    }
  return (0);
  }

