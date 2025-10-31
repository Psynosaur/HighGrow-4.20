// =====================================================================
//  PROGRAM:     Chart.c
//  PURPOSE:     Showing Plant History in a Graph form
// =====================================================================

#include <windows.h>
#include "stdio.h"
#include "resource.h"
#include "global.h"
#include "toolbar.h"
#include "calc.h"
#include "plantmem.h"
#include "dibitmap.h"
      
// --------------------------------------------------------------------
// FUNCTION TO: Create and Delete all the drawing objects we require
// --------------------------------------------------------------------
            
HFONT hCHHNameFont; // font for horizontal axis descriptor
HFONT hCHVNameFont; // font for vertical axis descriptor
HFONT hCHTicFont;  // font for Tic Number on Chart

HPEN  hGridPen;           
HPEN  hRedChartPen;       HBRUSH hRedChartBrush;
HPEN  hBlueChartPen;      HBRUSH hBlueChartBrush;
HPEN  hGreenChartPen;     HBRUSH hGreenChartBrush;
HPEN  h3DTopPen;          HBRUSH h3DTopBrush;
HPEN  h3DFacePen;         HBRUSH h3DFaceBrush;
HPEN  h3DBottomPen;       HBRUSH h3DBottomBrush;
HBRUSH h3DLFaceBrush;

#define LTGREEN   (RGB(128,192,128))

void CHCreateDrawObjects(void)
    {
    hCHHNameFont = GLCreateDialogFont(18,0,FW_BOLD);
    hCHVNameFont = GLCreateDialogFont(18,900,FW_BOLD);
    hCHTicFont   = GLCreateDialogFont(12,0,FW_NORMAL);

    hGridPen       = CreatePen(PS_DOT,   1, RGB(192,255,192));
    hRedChartPen   = CreatePen(PS_SOLID, 1, RGB(255,0,0));
    hBlueChartPen  = CreatePen(PS_SOLID, 1, RGB(0,0,255));
    hGreenChartPen = CreatePen(PS_SOLID, 1, RGB(0,255,0));
    h3DTopPen      = CreatePen(PS_SOLID, 1, RGB(0,192,0));
    h3DFacePen     = CreatePen(PS_SOLID, 1, LTGREEN);
    h3DBottomPen   = CreatePen(PS_SOLID, 1, RGB(0,128,0));

    hRedChartBrush   = CreateSolidBrush(RGB(255,0,0));
    hBlueChartBrush  = CreateSolidBrush(RGB(0,0,255));
    hGreenChartBrush = CreateSolidBrush(RGB(0,255,0));
    h3DTopBrush      = CreateSolidBrush(LTGREEN);
    h3DFaceBrush     = CreateSolidBrush(LTGREEN);
    h3DLFaceBrush    = CreateSolidBrush(RGB(255,255,255));
    h3DBottomBrush   = CreateSolidBrush(RGB(0,128,0));
    }


void CHFreeDrawObjects(void)
    {
    if(hCHHNameFont)     DeleteObject(hCHHNameFont);
    if(hCHVNameFont)     DeleteObject(hCHVNameFont);
    if(hCHTicFont)       DeleteObject(hCHTicFont);

    if(hGridPen)         DeleteObject(hGridPen);
    if(hRedChartPen)     DeleteObject(hRedChartPen);
    if(hBlueChartPen)    DeleteObject(hBlueChartPen);
    if(hGreenChartPen)   DeleteObject(hGreenChartPen);
    if(h3DTopPen)        DeleteObject(h3DTopPen);
    if(h3DFacePen)       DeleteObject(h3DFacePen);
    if(h3DBottomPen)     DeleteObject(h3DBottomPen);

    if(hRedChartBrush)   DeleteObject(hRedChartBrush);
    if(hBlueChartBrush)  DeleteObject(hBlueChartBrush);
    if(hGreenChartBrush) DeleteObject(hGreenChartBrush);
    if(h3DTopBrush)      DeleteObject(h3DTopBrush);
    if(h3DFaceBrush)     DeleteObject(h3DFaceBrush);
    if(h3DLFaceBrush)    DeleteObject(h3DLFaceBrush);
    if(h3DBottomBrush)   DeleteObject(h3DBottomBrush);
    }



// --------------------------------------------------------------------
//  FUNCTION TO: Managing the different graph types
// --------------------------------------------------------------------

// IDC_RB01 = Health
// IDC_RB02 = Height
// IDC_RB03 = Mass
// IDC_RB04 = Potency
// IDC_RB05 = Moisture
// IDC_RB06 = Soil pH

int giTicLength = 5;
int giChartEdge = 15;
#define gMaxDayTic 150 // after this number we must show weekly tic markings

typedef struct
   { // Graph ChartInfo Structure holds info about our graph axes
	int  iYMinValue;     // minimum value on the vertical axis
	int  iYMaxValue;     // maximum value on the vertical axis
	int  iYTicPels;      // pixels per tic on the vertical axis
	int  iYTicUnit;      // graph units per tic on the vertical axis
    int  iYTicDecs;      // number of zeroes to show after decimals
    int  iYTextOfs;      // number of pixels to allow for horizontal tic text height
    int  iXTics;         // number of tics on the horizontal axis
	int  iXTicPels;      // pixels per tic on the horizontal axis
	int  iXTicUnit;      // graph units per tic on the horizontal axis
    int  iXTicDecs;      // number of zeroes to show after decimals
    int  iXTextOfs;      // number of pixels to allow for vertical tic text width
   } CHARTINFO;

typedef struct
   { // GraphPlot Struct holds an array of points to draw polyline with
   POINT ptPlot[gMaxDayTic+2]; // all the plotted graph values
   int   iMaxPlotDay;   // giGrowDay for this plant
   int   iCurDayValue;  // current value for this plant
   char  szPlotName[40]; // plant's name
   } GRAPHPLOT;

CHARTINFO CI_Chart;    // holds chartinfo for one graph
GRAPHPLOT GP_Plant[3]; // holds graphplot point array for each plant


// --------------------------------------------------------------------
//  FUNCTION TO: Set the Axes Rect withing our static Chart Window
// --------------------------------------------------------------------

void CHSetChartRect(HWND hwnd, RECT* rcWin)
    {
    GetClientRect(hwnd, rcWin);
    InflateRect(rcWin, -giChartEdge, -giChartEdge);
    rcWin->left   += CI_Chart.iXTextOfs +15;
    rcWin->bottom -= CI_Chart.iYTextOfs +15;
    }


int CHGetCheckedButton(HWND hdlg)
    {
    int i;
    // first find out which radio button is selected
    for(i=0;i<6;i++)
        { // if the button is check, set our local and quit loop
        if((IsDlgButtonChecked(hdlg, IDC_RB01+i)==BST_CHECKED))
            return i;
        }
    return 0;
    }


// --------------------------------------------------------------------
//  FUNCTION TO: Initialize the graphplot struct for our three plants
// --------------------------------------------------------------------

void CHInitPlantGraph(HWND hStatic, PPLANT plPlant, int iPlant, int iStyle)
    {
    int    i, j, k, xVal, yVal, yOfs;
    double dCurVal, dEndVal, dAvVal;
    PGROWDAY gDay;
    RECT   rc;
    // setup the drawing region
    CHSetChartRect(hStatic, &rc);
    // now plot the relevant graph
    j=0;
    for(i=0;i<=giGrowDay;i=i+CI_Chart.iXTicUnit)
        {
        dAvVal = 0; // initialize this now
        for(k=0;(k<CI_Chart.iXTicUnit)&(i+k<giGrowDay);k++)
            {
            gDay = &plPlant->GD_Plant[i+k+1];
            switch(iStyle)
                {
                case 0: dCurVal = gDay->cHealth;   break;
                case 1: dCurVal = gDay->iHeight;   break;
                case 2: dCurVal = gDay->iMass;     break;
                case 3: dCurVal = gDay->iPotency;  break;
                case 4: dCurVal = gDay->iMoisture; break;
                case 5: dCurVal = gDay->cSoilPH;   break;
                }
//            if(dCurVal) dEndVal = dCurVal; // if a value, set the end one
            dEndVal = dCurVal; // set the end value
            dAvVal  +=  dEndVal; // now add it to the average
            }
        dAvVal = dAvVal/CI_Chart.iXTicUnit;
        xVal = ((i+k)/CI_Chart.iXTicUnit)*CI_Chart.iXTicPels;
        yVal = (int)((dAvVal/CI_Chart.iYTicUnit)*CI_Chart.iYTicPels);
        yOfs = (CI_Chart.iYMinValue/CI_Chart.iYTicUnit)*CI_Chart.iYTicPels;
        if(j==0)
            { // if this is day 0, we check it's Y value
            GP_Plant[iPlant].ptPlot[j].x = rc.left;
            if(iStyle == 5) // special case for the pH
                 GP_Plant[iPlant].ptPlot[j].y = rc.bottom + yOfs - yVal;
            else GP_Plant[iPlant].ptPlot[j].y = rc.bottom + yOfs;
            j=j+1; // increment this local counter
            }
        GP_Plant[iPlant].ptPlot[j].x = rc.left   + xVal;
        GP_Plant[iPlant].ptPlot[j].y = rc.bottom - yVal + yOfs;
        j=j+1; // increment this local counter
        }
    GP_Plant[iPlant].iCurDayValue= (int)dEndVal;
    // this last setting signals the end of our graph for this plant
    GP_Plant[iPlant].ptPlot[j+1].x = 0;
    GP_Plant[iPlant].ptPlot[j+1].y = 0;
    // also set this very usefull struct element
    GP_Plant[iPlant].iMaxPlotDay = giGrowDay;
    // also hold this plant's name
    strcpy(GP_Plant[iPlant].szPlotName, plPlant->PI_Plant.szPlantName);
    }


void CHInitGraphPlot(HWND hStatic, int iButton, int iRoomOfs)
    { 
    // prepares an array of points for each plant to 
    // use to draw the graph with. 
    int i;
    // now fill the graphlot struct for each plant
    for(i=0;i<3;i++)
        {
        // first erase the entire graphplot struct for this plant
        ZeroMemory(&GP_Plant[i], (DWORD)sizeof(GRAPHPLOT));
        // now load the plant file and init the graph
        sprintf(strbuf, "Plant%02i.hgp", i+1+iRoomOfs);
        if(GLDoesFileExist(strbuf))
            {
            if(PMPlantFileToMem(i+iRoomOfs))
                { // first lock our global memory
                P_Plant = (PPLANT)GlobalLock(hPlantMem);
                CACalculatePlant(P_Plant, i+iRoomOfs);
                CHInitPlantGraph(hStatic, P_Plant, i, iButton);
                GlobalUnlock(hPlantMem);
                PMPlantMemoryFree();
                }
            }
        }
    }


int CHCalcOldestPlotDay(int iRoomOfs)
    { 
    // prepares an array of points for each plant to 
    // use to draw the graph with. 
    int i;
    int iDay     = 0;
    // now fill the graphlot struct for each plant
    for(i=0;i<3;i++)
        {
        // now load the plant file and init the graph
        sprintf(strbuf, "Plant%02i.hgp", i+1+iRoomOfs);
        if(GLDoesFileExist(strbuf))
            {
            if(PMPlantFileToMem(i+iRoomOfs))
                { // first lock our global memory
                giCurPlant = i+1;
                P_Plant = (PPLANT)GlobalLock(hPlantMem);
                CACalculatePlant(P_Plant, i+iRoomOfs);
                iDay = max(iDay, giGrowDay);
                GlobalUnlock(hPlantMem);
                PMPlantMemoryFree();
                }
            }
        }
    return iDay;
    }


// --------------------------------------------------------------------
//  FUNCTION TO: Initialize the chart info struct
// --------------------------------------------------------------------

void CHInitChartPlotInfo(HWND hStatic, HDC hdc)
    {
    HWND hdlg = GetParent(hStatic);
    RECT rc, rcTemp;
    int  iUnits;
    char szStr[10];
    int  iButton;
    int  iOldestDay;
    int  iRoomOfs = (TBGetComboGrowroom()-1)*3;
    // first erase the entire graphplot struct for this plant
    ZeroMemory(&CI_Chart, (DWORD)sizeof(CHARTINFO));
    // first find out which radio button is selected
    iButton = CHGetCheckedButton(hdlg);
    // ********* not very elegant *************
    iOldestDay = CHCalcOldestPlotDay(iRoomOfs);
    // first we calculate the vertical axes information
 	CI_Chart.iYMinValue = 0;

    if(iButton==0)
        { // if this is the HEALTH radio button?
        CI_Chart.iYTicDecs  = 0;      
     	CI_Chart.iYMaxValue = 100;         
	    CI_Chart.iYTicUnit  = 10;
        goto InitEnd;
        }
    if(iButton==1)
        { // if this is the HEIGHT radio button?
        CI_Chart.iYTicDecs  = 0;      
	    CI_Chart.iYMaxValue = 2500;         
	    CI_Chart.iYTicUnit  = 250;      
        goto InitEnd;
        }
    if(iButton==2)
        { // if this is the MASS radio button?
        CI_Chart.iYTicDecs  = 0;      
	    CI_Chart.iYMaxValue = 1000;         
	    CI_Chart.iYTicUnit  = 100;      
        goto InitEnd;
        }
    if(iButton==3)
        { // if this is the POTENCY radio button?
        CI_Chart.iYTicDecs  = 2;      
	    CI_Chart.iYMaxValue = 450;         
	    CI_Chart.iYTicUnit  = 50;      
        goto InitEnd;
        }
    if(iButton==4)
        { // if this is the MOISTURE radio button?
        CI_Chart.iYTicDecs  = 0;      
	    CI_Chart.iYMaxValue = 1500;         
	    CI_Chart.iYTicUnit  = 100;      
        goto InitEnd;
        }
    if(iButton==5)
        { // if this is the SOIL PH radio button?
        CI_Chart.iYTicDecs  = 1;      
     	CI_Chart.iYMinValue = 40;
	    CI_Chart.iYMaxValue = 100;         
	    CI_Chart.iYTicUnit  = 10;      
        goto InitEnd;
        }
InitEnd:
    // here we measure the length of the maximum length string
    sprintf(szStr, "%i", 2500);
    DrawText(hdc, szStr, -1, &rcTemp, DT_CALCRECT);
    // and set this important offset position
    CI_Chart.iXTextOfs = rcTemp.right-rcTemp.left;      
    CI_Chart.iYTextOfs = rcTemp.bottom-rcTemp.top;      
    // *** now we can calculate our graph plotting area
    CHSetChartRect(hStatic, &rc);

    // now we finish off the common vertical axis info
    iUnits= (CI_Chart.iYMaxValue-CI_Chart.iYMinValue)/CI_Chart.iYTicUnit;
  	CI_Chart.iYTicPels = (rc.bottom-rc.top)/iUnits;

    // *** now we can calculate all our constant horizontal axes information
    CI_Chart.iXTics    = (int)(((iOldestDay/7)+1)*7); // to the nearest next 7 days
    CI_Chart.iXTicDecs = 0; // horizontal growdays/weeks have no decimals
    if(CI_Chart.iXTics<=gMaxDayTic) // enough space to show daily tics?
         CI_Chart.iXTicUnit = 1; // set tics to days
    else CI_Chart.iXTicUnit = 7; // set tics to weeks

    iUnits= CI_Chart.iXTics/CI_Chart.iXTicUnit;
    CI_Chart.iXTicPels = (rc.right-rc.left)/iUnits;

    // here we setup our graph plotting points
    CHInitGraphPlot(hStatic, iButton, iRoomOfs);
    }

// --------------------------------------------------------------------
//  FUNCTION TO: Drawing the chart's axes
// --------------------------------------------------------------------

void CHDrawChartAxes(HWND hwnd, HDC hdc, BOOL bPaper)
   {
   RECT rcWin;

   CHSetChartRect(hwnd, &rcWin);
   if(bPaper)
        FrameRect(hdc, &rcWin, h3DFaceBrush);
   else Rectangle3D(hdc, rcWin,   GetStockObject(BLACK_PEN), h3DLFaceBrush, 
                    h3DBottomPen, GetStockObject(WHITE_PEN));
   }


// --------------------------------------------------------------------
//  FUNCTION TO: Drawing the chart's axes descriptions
// --------------------------------------------------------------------

void CHDrawChartName(HWND hwnd, HDC hdc)
   {
   int iButton;
   char szText[50];
   RECT rcWin, rcChart, rcVer, rcHor;
   HPEN  hPen = SelectObject(hdc, GetStockObject(BLACK_PEN));
   HFONT hFont= SelectObject(hdc, hCHHNameFont);
   // first setup rectangles that we need for all our other rect calcs
   GetClientRect(hwnd, &rcWin);
   CHSetChartRect(hwnd, &rcChart);
   // now setup the rect we need for our horizontal X axis descriptor
   rcHor.left   = rcChart.left;
   rcHor.bottom = rcWin.bottom-giChartEdge+5;
   rcHor.top    = rcHor.bottom-20;
   rcHor.right  = rcChart.right;
   // now draw the horizontal descriptor
   if(CI_Chart.iXTicUnit == 1)
        strcpy(szText, "Plant Growth (Days)");
   else strcpy(szText, "Plant Growth (Weeks)");
   DrawText(hdc, szText, -1, &rcHor, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
   // now setup the rect we need for our vertical Y axis descriptor
   rcVer.left   = giChartEdge-5;
   rcVer.top    = rcChart.top;
   rcVer.right  = rcVer.left + 120;
   rcVer.bottom = rcChart.bottom -10;
   // select the vertical font and draw the text
   SelectObject(hdc, hCHVNameFont);
   // sind out which radio button is checked
   iButton = CHGetCheckedButton(GetParent(hwnd));
   // find out the text on the radio button
   GetDlgItemText(GetParent(hwnd),IDC_RB01+iButton,strbuf,40);
   sprintf(szText, "Plant %s", strbuf);
   DrawText(hdc, szText, -1, &rcVer, DT_BOTTOM|DT_SINGLELINE);

   SelectObject(hdc, hPen);
   SelectObject(hdc, hFont);
   }


// --------------------------------------------------------------------
//  FUNCTION TO: Drawing the chart's grid lines
// --------------------------------------------------------------------

void CHDrawChartGrid(HWND hwnd, HDC hdc)
    {
    int  i, j, k;
    int  iCur, iXStep;
    RECT rc;
    HPEN hPen = SelectObject(hdc, hGridPen);
    // first set the rect we can draw within
    CHSetChartRect(hwnd, &rc);

    // first calculate our horizontal text step value
    if(CI_Chart.iXTicPels>20)
         iXStep = 1;
    else iXStep = (20/CI_Chart.iXTicPels)+1;

    // here we draw the vertical grid lines on the horizontal axis
    iCur = rc.left;
    while(iCur < rc.right)
        { // if this is not the first one (on axis line)
        if(iCur > rc.left)
            { // draw the grid line
            MoveToEx(hdc, iCur, rc.bottom-2, NULL);
            LineTo(hdc, iCur, rc.top+2);
            }
        iCur += CI_Chart.iXTicPels*iXStep;
        }
    // now let's draw the horizontal grid on the vertical axis
    j = CI_Chart.iYMinValue/CI_Chart.iYTicUnit;
    k = CI_Chart.iYMaxValue/CI_Chart.iYTicUnit;
    iCur = rc.bottom-CI_Chart.iYTicPels;
    for(i=j;i<k; i++)
        {
        MoveToEx(hdc, rc.left+2, iCur, NULL);
        LineTo(hdc, rc.right-2, iCur);
        iCur -= CI_Chart.iYTicPels;
        }
    // finally we must reselect that ol' pen!
    SelectObject(hdc, hPen);
    }
       


// --------------------------------------------------------------------
//  FUNCTION TO: Drawing the chart's tic marks
// --------------------------------------------------------------------


void CHSetYTicText(char* szText, int iValue)
    {
    char szStr[10];
    switch (CI_Chart.iYTicDecs)
        {
        case 0:  sprintf(szStr, "%i",    iValue);       break;
        case 1:  sprintf(szStr, "%1.1f", iValue/10.0);  break;
        case 2:  sprintf(szStr, "%1.2f", iValue/100.00);break;
        }
    strcpy(szText, szStr);
    }


void CHDrawChartTics(HWND hwnd, HDC hdc)
   {
   RECT  rcWin, rcTemp;
   POINT ptTic;
   char  szStr[30];
   int   iXStep;
   int   i, j, k;
   HPEN  hPrevPen  = SelectObject(hdc, GetStockObject(BLACK_PEN));
   HFONT hPrevFont = SelectObject(hdc, hCHTicFont);
   // set our text forground and background colours
   SetTextColor(hdc, RGB(0,0,0)); // text colouur to black
//   SetBkColor(hdc, GetSysColor(COLOR_3DFACE)); // background to face
   SetBkColor(hdc, LTGREEN); // background to face
   // now, set our tic drawing outside rect
   CHSetChartRect(hwnd, &rcWin);
   // first calculate our horizontal text step value
   if(CI_Chart.iXTicPels>20)
        iXStep = 1;
   else iXStep = (20/CI_Chart.iXTicPels)+1;
   // now let's draw the horizontal tics
   ptTic.x=rcWin.left;
   ptTic.y=rcWin.bottom;
   i = 0;
   j = CI_Chart.iXTics/CI_Chart.iXTicUnit;
   while(ptTic.x < rcWin.right)
       {
       MoveToEx(hdc, ptTic.x, ptTic.y, NULL);
       // now if this is a text tic, draw it differently
       if(((int)(i/iXStep)*iXStep)==i)
           { // first draw the tic line at full length
           LineTo(hdc, ptTic.x, ptTic.y+giTicLength+1);
           // now write the text in the space provided
           sprintf(szStr, "%i", i);
//           sprintf(szStr, "%i", i*CI_Chart.iXTicUnit);
           rcTemp.left   = ptTic.x - 10;
           rcTemp.top    = ptTic.y + giTicLength + 1;
           rcTemp.right  = ptTic.x + 10;
           rcTemp.bottom = rcTemp.top + 15;
           DrawText(hdc, szStr, -1, &rcTemp, DT_CENTER);
           }
       else // we draw the tic line a little shorter
           LineTo(hdc, ptTic.x, ptTic.y+giTicLength-1);
       // adjust the x postion of the next tic
       ptTic.x += CI_Chart.iXTicPels;
       // we must also increment our tic counter
       i += 1;
       }
   // now let's draw the vertical tics
   ptTic.x=rcWin.left;
   ptTic.y=rcWin.bottom;
   j = CI_Chart.iYMinValue/CI_Chart.iYTicUnit;
   k = CI_Chart.iYMaxValue/CI_Chart.iYTicUnit;
   for(i=j;i<=k; i++)
       {
       MoveToEx(hdc, ptTic.x, ptTic.y, NULL);
       LineTo(hdc, ptTic.x-giTicLength, ptTic.y);

       rcTemp.left   = ptTic.x - giTicLength - 3 - 100;
       rcTemp.top    = ptTic.y - 6;
       rcTemp.right  = rcTemp.left + 100;
       rcTemp.bottom = rcTemp.top + 12;

       CHSetYTicText(szStr, i*CI_Chart.iYTicUnit);
       DrawText(hdc, szStr, -1, &rcTemp, DT_RIGHT);

       ptTic.y -= CI_Chart.iYTicPels;
       }
   // finally, we reselect our old objects
   SelectObject(hdc, hPrevPen);
   SelectObject(hdc, hPrevFont);
   }


// --------------------------------------------------------------------
//  FUNCTION TO: Drawing each plant's growth graph
// --------------------------------------------------------------------
/*
typedef struct
   { // GraphPlot Struct holds an array of points to draw polyline with
   POINT ptPlot[gMaxDayTic+2];
   } GRAPHPLOT;

CHARTINFO CI_Chart;    // holds chartinfo for one graph
GRAPHPLOT GP_Plant[3]; // holds graphplot point array for each plant
*/

void CHDrawChartGraphs(HWND hwnd, HDC hdc, BOOL bPaper)
   {
   int  iTics, iPlant;
   HPEN hPlotPen[3];
   HPEN hPrevPen = SelectObject(hdc, hRedChartPen);
   RECT rcClip;
   HRGN hClipRgn;
   // first set a clipping rect to ensure we don't draw outside our chart

   CHSetChartRect(hwnd, &rcClip);
   if(!bPaper) 
        { // if we're not drawing to paper, we'll set a clip rect
        rcClip.bottom -=1;
        hClipRgn = CreateRectRgn(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom);
        SelectClipRgn(hdc, hClipRgn);
        }
   hPlotPen[0] = hRedChartPen;
   hPlotPen[1] = hBlueChartPen;
   hPlotPen[2] = hGreenChartPen;
   for(iPlant=0;iPlant<3;iPlant++)
       { // if we have a valid plant to draw
       if(GP_Plant[iPlant].szPlotName[0] != 0)
           {
           iTics=0;
           while(GP_Plant[iPlant].ptPlot[iTics].x != 0)
                {
                iTics += 1;
                }
           SelectObject(hdc, hPlotPen[iPlant]);
           Polyline(hdc, (POINT*)&GP_Plant[iPlant], iTics-1);
           }
       }
   SelectObject(hdc, hPrevPen);
   }

// --------------------------------------------------------------------
//  FUNCTION TO: Draw the plant colour legend boxes
// --------------------------------------------------------------------

void CHDrawLegend(HWND hwnd, HDC hdc)
    { // draws the legend boxes showing graph colours
    HPEN   hCPen;
    HBRUSH hCBrush;
    RECT rcWin;
    char szStr[20];
    int  iDay, iVal;
    int  iID = GetDlgCtrlID(hwnd);
    int  iPlant = (iID-IDC_STC01);
    // if the plant isn't growing yet, do nothing
    if(GP_Plant[iPlant].szPlotName[0] == 0) return;
    // otherwise we, show the correct graph colour
    switch(iID)
        {
        case IDC_STC01:
            hCPen   = hRedChartPen;
            hCBrush = hRedChartBrush;
        break;
        case IDC_STC02:
            hCPen   = hBlueChartPen;
            hCBrush = hBlueChartBrush;
        break;
        case IDC_STC03:
            hCPen   = hGreenChartPen;
            hCBrush = hGreenChartBrush;
        break;
        }
    // first show the graph colour for this plant
    GetClientRect(hwnd, &rcWin);
    FillRect(hdc, &rcWin, hCBrush);
    // now draw the area that we're going to write text on
    InflateRect(&rcWin, -3, -3);
    SelectObject(hdc, h3DFacePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    SelectObject(hdc, h3DLFaceBrush);
    Rectangle(hdc, rcWin.left, rcWin.top, rcWin.right, rcWin.bottom);
    // now we draw the text for this plant
    SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
    SetBkColor(hdc, RGB(255,255,255));
    iDay = GP_Plant[iPlant].iMaxPlotDay;
    iVal = GP_Plant[iPlant].iCurDayValue;
    // now format and draw the text
    OffsetRect(&rcWin, 0, 2);
    CHSetYTicText(szStr, iVal);
    sprintf(strbuf, "%s\n%s at day %i", GP_Plant[iPlant].szPlotName, szStr, iDay);
    DrawText(hdc, strbuf, -1, &rcWin, DT_CENTER);
    }


// --------------------------------------------------------------------
// FUNCTION TO: Draw the growchart in our dialog's static window
// --------------------------------------------------------------------

void CHDrawChart(HWND hwnd, HDC hdc)
   {
   HBRUSH hBrush = SelectObject(hdc, h3DFaceBrush);
   HPEN   hPen   = SelectObject(hdc, h3DFacePen);
   RECT rc;

   GetClientRect(hwnd, &rc);
   Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

   CHDrawChartAxes(hwnd, hdc, FALSE); // draw the graph's axes
   CHDrawChartTics(hwnd, hdc); // draw the graph's tic marks and tic text
   CHDrawChartName(hwnd, hdc); // draw the description of each axis
   CHDrawChartGrid(hwnd, hdc); // draw the graph's dash-dot-dot grid lines
   CHDrawChartGraphs(hwnd, hdc, FALSE); // now draw each plant's growth graph

   SelectObject(hdc, hPen);
   SelectObject(hdc, hBrush);
   }



// ======================================================================
// PRINTING GROWTH CHART DATA
// ======================================================================
// THE ABORT PROCEDURE
// --------------------------------------------------------------------

BOOL CALLBACK CHAbortProc(HDC hDC, int Error)
    {
    MSG   msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
    return TRUE;
    }

// --------------------------------------------------------------------
//  GETTING THE PRINTER'S DC
// --------------------------------------------------------------------

HDC CHGetPrinterDC(void)
    {
    PRINTDLG pdlg;
    // Initialize the PRINTDLG structure
    memset(&pdlg, 0, sizeof(PRINTDLG));
    pdlg.lStructSize = sizeof(PRINTDLG);
    // Set the flag to return printer DC
    pdlg.Flags = PD_RETURNDC;
    // Invoke the printer dialog box
    PrintDlg(&pdlg);
    // hDC member of the PRINTDLG structure contains
    // the printer DC
    return pdlg.hDC;
    }

// --------------------------------------------------------------------
//  PRINTING THE PAGE
// --------------------------------------------------------------------


void CHPrintHeader(HDC hdc, HINSTANCE hInst, LPRECT rcPg, LPRECT rcHdr)
    {
    char szDate[60] ="\0";
    char szLineBuf[250]="\0";

    Rectangle(hdc, rcHdr->left, rcHdr->top, rcHdr->right, rcHdr->bottom);
    rcHdr->left  += 50;
    rcHdr->right -= 50;

    GetDateFormat(LOCALE_SYSTEM_DEFAULT,0,NULL,"dddd',' dd MMMM yyyy",(LPTSTR)&szDate, 60);
    DrawText(hdc, "HighGrow Growth Chart", -1, rcHdr, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
    DrawText(hdc, szDate, -1, rcHdr, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
    FrameRect(hdc, rcPg, GetStockObject(BLACK_BRUSH));
    }



void CHPrintChartPage(HWND hWnd, HDC hdc, HINSTANCE hInst)
    { // This is the function that does draws on a given DC
    RECT   rcWin, rcPage, rcHeader;
    HWND   hChartWnd;
    SIZE   size1, size2;
    int    iOMode, iMapMode;
    HPEN   hOPen,  hBlackPen  = GetStockObject(BLACK_PEN);
    HBRUSH hOBrush,hGreyBrush = GetStockObject(LTGRAY_BRUSH);
    HFONT  hOFont, hHeaderFont;
    char   szLineBuf[250]="\0";

    // first create and select all our objects
    hHeaderFont = GLCreateDialogFont(80,0,FW_BOLD);
    hOFont  = SelectObject(hdc, hHeaderFont);
    hOPen   = SelectObject(hdc, hBlackPen);
    hOBrush = SelectObject(hdc, hGreyBrush);
    iOMode  = SetBkMode(hdc, TRANSPARENT);
    // set the rects based on where we're drawing
    SetRect(&rcPage, 0, 0,
            GetDeviceCaps(hdc, HORZRES),
            GetDeviceCaps(hdc, VERTRES));
    CopyRect(&rcHeader, &rcPage);
    rcHeader.bottom=200;
    rcPage.top     =200;
    // print the page border and header
    SelectObject(hdc, hHeaderFont);
    CHPrintHeader(hdc, hInst, &rcPage, &rcHeader);
    // now we can print our chart
    hChartWnd = GetDlgItem(hWnd, IDC_CHART);
    GetClientRect(hChartWnd, &rcWin);

    iMapMode = SetMapMode(hdc, MM_ISOTROPIC) ;
    SetWindowExtEx(hdc, rcWin.right-rcWin.left,  rcWin.bottom-rcWin.top, &size1) ;
    SetViewportExtEx(hdc, rcPage.right-rcPage.left, rcPage.bottom-rcPage.top, &size2);
    SetWindowOrgEx(hdc, 0, -20, NULL);
    CHDrawChartAxes(hChartWnd, hdc, TRUE); // draw the graph's axes
    CHDrawChartTics(hChartWnd, hdc); // draw the graph's tic marks and tic text
    CHDrawChartName(hChartWnd, hdc); // draw the description of each axis
    CHDrawChartGrid(hChartWnd, hdc); // draw the graph's dash-dot-dot grid lines
    CHDrawChartGraphs(hChartWnd, hdc, TRUE); // now draw each plant's growth graph
    SetMapMode(hdc, iMapMode);
    SetWindowExtEx(hdc, size1.cx, size1.cy, NULL);
    SetViewportExtEx(hdc, size2.cx, size2.cy, NULL);

    // finally we reselect all our objects
    SelectObject(hdc, hOPen);
    SelectObject(hdc, hOBrush);
    SelectObject(hdc, hOFont);
    SetBkMode(hdc, iOMode);
    if(hHeaderFont) DeleteObject(hHeaderFont);
    }


// -------------------------------------------------------
//  PRINTING THE PLANT GROWTH CHART REPORT
// -------------------------------------------------------


BOOL CHPrintGrowthChart(HWND hWnd, HINSTANCE hInst)
    {
    DOCINFO di;
    HDC     hDC;
    // Need a printer DC to print to
    hDC = CHGetPrinterDC();
    // Did we get a good DC?
    if(!hDC) return FALSE;
    // Here we set an AbortProc()
    if(SetAbortProc(hDC, CHAbortProc) == SP_ERROR)
        {
        MessageBox(hWnd, "Error setting up AbortProc", "Error", MB_APPLMODAL | MB_OK);
        return FALSE;
        }
    // Init the DOCINFO and start the document
    memset(&di, 0, sizeof(DOCINFO));
    di.cbSize    = sizeof(DOCINFO);
    di.lpszDocName = "Growth Chart Printing";
    // start the document printing
    StartDoc(hDC, &di);
    // Start the Page Printing cycle
    StartPage(hDC);
    // now print all the pages for this log
    CHPrintChartPage(hWnd, hDC, hInst);
    // now end the page printing cycle
    EndPage(hDC);
    // end the printing and clean up
    EndDoc(hDC);
    DeleteDC(hDC);
    SetForegroundWindow(hWnd);
    return TRUE;
    }


// --------------------------------------------------------------------
//  RESIZES ALL THE CONTROLS INTO THE CHART DIALOG
// --------------------------------------------------------------------


void CHFitChartDialog(HWND hdlg)
    {
    RECT rcDlg;
    int  i, iX, iW;
    int  iBHeight = 40;  // button height
    int  iEdge    = 8;   // edge width

    // Resizes the dialog inside the HighGrow Program window. 
    GLSizeDialogToFit(hdlg);
    // let's read the size and pos of the dialog
    GetClientRect(hdlg, &rcDlg);
    // let's move the static window first
    MoveWindow(GetDlgItem(hdlg, IDC_CHART), iEdge, (iBHeight/2)+(iEdge*2), 
                      rcDlg.right-rcDlg.left-(iEdge*2),
                      rcDlg.bottom-rcDlg.top-iBHeight-(iBHeight/2)-(iEdge*4), FALSE);
    // now we'll move all our graph type radio buttons
    iX = iEdge*3;
    iW = ((rcDlg.right-rcDlg.left-(iEdge*2))/6)-iEdge;
    for(i=0;i<6;i++)
        {
        MoveWindow(GetDlgItem(hdlg,IDC_RB01+i), iX, iEdge, iW, iBHeight/2, FALSE);
        iX += iW+iEdge;
        }
    // now we'll move our plant legend static controls
    iX = iEdge;
    iW = ((rcDlg.right-rcDlg.left-(iEdge*2))/4)-iEdge;
    for(i=0;i<3;i++)
        {
        MoveWindow(GetDlgItem(hdlg,IDC_STC01+i), iX, 
                    rcDlg.bottom-iEdge-iBHeight, iW, iBHeight, FALSE);
        iX += iW+iEdge;
        }
    // now we move our print button
    MoveWindow(GetDlgItem(hdlg,IDC_BT01), rcDlg.right-iEdge-145,
               rcDlg.bottom-iEdge-iBHeight, 70, iBHeight, FALSE);

    // now we move our cancel button
    MoveWindow(GetDlgItem(hdlg,IDCANCEL), rcDlg.right-iEdge-70,
               rcDlg.bottom-iEdge-iBHeight, 70, iBHeight, FALSE);

    }


void CHInitChartDialog(HWND hDlg, HINSTANCE hInst)
    {
    HDC hdc = GetDC(hDlg);
    char szBuff[50] ="\0";
    char szRoom[50] ="\0";
    // set the caption to reflect which growroom
    TBGetComboGrowroomName(szBuff);
	wsprintf(szRoom, "Growth Chart for - %s", szBuff);
    SetWindowText(hDlg, szRoom);
    // first check the health radio button
    SendDlgItemMessage(hDlg, IDC_RB01, BM_SETCHECK, BST_CHECKED, 0);
    // now fill the plotinfo struct for this chart
    CHInitChartPlotInfo(GetDlgItem(hDlg, IDC_CHART), hdc);
    ReleaseDC(hDlg, hdc);
    }


// --------------------------------------------------------------------
//  HISTORY CHART DIALOG WINPROC
// --------------------------------------------------------------------


void CHChangeChartType(HWND hdlg)
    {
    HWND hchart = GetDlgItem(hdlg, IDC_CHART);
    HDC  hdc    = GetDC(hchart);
    int  i;

    CHInitChartPlotInfo(hchart, hdc);
    CHDrawChart(hchart, hdc);
    ReleaseDC(hchart, hdc);
    // now force the static legends to repaint
    for(i=0;i<3;i++)
        InvalidateRect(GetDlgItem(hdlg, IDC_STC01+i), NULL, TRUE);
    }

  
BOOL CALLBACK CHChartDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   
    switch (message)
      {
      case WM_INITDIALOG:
           CHFitChartDialog(hDlg);
           CHCreateDrawObjects();
           CHInitChartDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
           CHFreeDrawObjects();
      break;

      case WM_ERASEBKGND:
      return DITileDlgLeafBitmap(hDlg, (HDC)wParam);

      case WM_CTLCOLORSTATIC:
          {
          int i = GetDlgCtrlID((HWND)lParam);
          if(i==IDC_CHART)
              CHDrawChart((HWND)lParam, (HDC)wParam);
          else
              {
              if((i==IDC_STC01)||(i==IDC_STC02)||(i==IDC_STC03))
                  CHDrawLegend((HWND)lParam, (HDC)wParam);
              else
                  {
                  if((i>=IDC_RB01)&&(i<=IDC_RB06))
                      {
                      SetBkMode((HDC)wParam, TRANSPARENT);
                      return (int)GetStockObject(NULL_BRUSH);
                      }
                  }
              }
          }
      return (FALSE);

      case WM_COMMAND:
          switch (wParam)
            {
            case IDC_RB01: 
            case IDC_RB02: 
            case IDC_RB03: 
            case IDC_RB04: 
            case IDC_RB05: 
            case IDC_RB06: 
                 CHChangeChartType(hDlg);
            return (FALSE);

            case IDC_BT01:
                  CHPrintGrowthChart(hDlg, dhInst);
            return (FALSE);

            case IDCANCEL: 
                EndDialog(hDlg, FALSE); 
            return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          
   


void CHShowChartDialog(HWND hwnd, HINSTANCE hInst)
    { 
    DialogBox(hInst, "Graphs", hwnd, CHChartDlgProc);
    }


