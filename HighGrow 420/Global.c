#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <time.h>
#include "resource.h"
#include "global.h"
#include "calc.h"
#include "toolbar.h"
#include "PlantMem.h"
#include "PlantLog.h"
#include "Password.h"
#include "htmlhelp.h"
#include "Toolbar.h"
#include "Midi.h"
#include "clock.h"
#include "dibapi.h"
#include "crypt.h"
   
/******************************************************************************\
*
*  GLOBAL.C    -   defines functions used by all other source files
*
\******************************************************************************/
    
/******************************************************************************\
*  GLOBAL TO ALL FILES CONSTANT DEFINITIONS
\******************************************************************************/

GROWROOM  GR_Room[gMaxRooms];         // holds growroom info
PLANTINFO PI_PlantSeed[gMaxPlants];   // holds plant info for ALL our plants
char      strbuf[200]="\0";           // holds temporary text (used in all files)

// ****** MUST BE FALSE WITH SHAREWARE VERSION *****
BOOL      gbRegistered=TRUE;    // true if this is a registered version
// ****** MUST BE FALSE WITH SHAREWARE VERSION *****
BOOL      gbPaid      =TRUE;    // true if this is a paid-for registered version
// ****** START OFF WITH NO ALPHA BLENDING *****
BOOL gbAlphaBlending  =FALSE;   // true if we can perform alpha-blending of bitmaps

LPCSTR lpSecurePassword ="HG42BETA";

/******************************************************************************\
*  FUNCTIONS TO HANDLE BLOWPIPE DEBUGGER
\******************************************************************************/

#define WM_BLOWPIPE  WM_USER+10000

void Blow(LPCTSTR lpBlowText)
    {
    ATOM atom = 0;
    HWND hwnd = FindWindow("BlowPipe", "BlowPipe Debugger");
    if(hwnd)
        {
        atom = GlobalAddAtom(lpBlowText);
        if(atom)
            {
            SendMessage(hwnd, WM_BLOWPIPE, 0, (LPARAM)atom);
            }
        }
    }



/******************************************************************************\
*  FUNCTIONS TO HANDLE: displays a help screen for a HTML help file
\******************************************************************************/

HWND GLHelpContext(HWND hparent, LPCSTR lpHelpFile, int iContext)
    {
    return HtmlHelp(hparent, lpHelpFile, HH_HELP_FINDER, iContext);
    }

           
/******************************************************************************\
*  FUNCTIONS TO HANDLE: initialize and save the daily actitvities
\******************************************************************************/

PLANTINFO PI_CurPlant;   // filled with ACInitPlantInfo
GROWDAY   GD_CurPlant;   // only holds ONE growday. filled by ACInitGrowDay

    

BOOL GLInitPlantInfo(PPLANTINFO piPlant)
    {
    int i;
    strcpy(PI_CurPlant.szPlantName, piPlant->szPlantName);
    for(i=0; i<3; i+=1)
        PI_CurPlant.szStartDate[i] = piPlant->szStartDate[i];
    PI_CurPlant.cGrowthStage  = piPlant->cGrowthStage;
    PI_CurPlant.cSeedChoice   = piPlant->cSeedChoice;
    PI_CurPlant.bRegistered   = piPlant->bRegistered;
    PI_CurPlant.dwFileVersion = piPlant->dwFileVersion;

    return TRUE;
    }

BOOL GLInitGrowDay(PGROWDAY gdPlant)
    { 
    int i;
    GD_CurPlant.bVisited     = gdPlant->bVisited;
    GD_CurPlant.bWatered     = gdPlant->bWatered;
    GD_CurPlant.bFertilized  = gdPlant->bFertilized;
    GD_CurPlant.iMass        = gdPlant->iMass;
    GD_CurPlant.iHeight      = gdPlant->iHeight;
    GD_CurPlant.iPotency     = gdPlant->iPotency;
    GD_CurPlant.iMoisture    = gdPlant->iMoisture;
    GD_CurPlant.cHealth      = gdPlant->cHealth;
    GD_CurPlant.cSoilPH      = gdPlant->cSoilPH;
    GD_CurPlant.cLightOn     = gdPlant->cLightOn;
    GD_CurPlant.cLightOff    = gdPlant->cLightOff;
    GD_CurPlant.cLightHeight = gdPlant->cLightHeight;
    GD_CurPlant.cTipsPruned  = gdPlant->cTipsPruned;
    for(i=0; i<4; i+=1)
        GD_CurPlant.szNutrients[i] = gdPlant->szNutrients[i];
    
    return TRUE;
    }

                              
BOOL GLSavePlantInfo(PPLANTINFO piPlant)
    {
    int i;
    strcpy(piPlant->szPlantName, PI_CurPlant.szPlantName);
    for(i=0; i<3; i+=1)
        piPlant->szStartDate[i] = PI_CurPlant.szStartDate[i];
    piPlant->cGrowthStage  = PI_CurPlant.cGrowthStage;
    piPlant->cSeedChoice   = PI_CurPlant.cSeedChoice;
    piPlant->bRegistered   = PI_CurPlant.bRegistered;
    piPlant->dwFileVersion = PI_CurPlant.dwFileVersion;

    return TRUE;
    }

BOOL GLSaveGrowDay(PGROWDAY gdPlant)
    { 
    int i;
    gdPlant->bVisited     = TRUE;
    gdPlant->bWatered     = GD_CurPlant.bWatered;
    gdPlant->bFertilized  = GD_CurPlant.bFertilized;
    gdPlant->iMass        = GD_CurPlant.iMass;
    gdPlant->iHeight      = GD_CurPlant.iHeight;
    gdPlant->iPotency     = GD_CurPlant.iPotency;
    gdPlant->iMoisture    = GD_CurPlant.iMoisture;
    gdPlant->cHealth      = GD_CurPlant.cHealth;
    gdPlant->cSoilPH      = GD_CurPlant.cSoilPH;
    gdPlant->cLightOn     = GD_CurPlant.cLightOn;
    gdPlant->cLightOff    = GD_CurPlant.cLightOff;
    gdPlant->cLightHeight = GD_CurPlant.cLightHeight;
    gdPlant->cTipsPruned  = GD_CurPlant.cTipsPruned;
    for(i=0; i<4; i+=1)
        gdPlant->szNutrients[i] = GD_CurPlant.szNutrients[i];
    
    return TRUE;
    }

/******************************************************************************\
*  SAVING AND LOADING OUR GROWTH VARIABLES FOR THE CURRENT PLANT
\******************************************************************************/

void GLSaveGrowthVariables(PPLANT plPlant)
    { // this we must calculate each visit for every day of growth
    plPlant->PI_Plant.cGrowthStage            = giGrowthStage;
    plPlant->PI_Plant.bRegistered             = gbRegistered;
    plPlant->GD_Plant[giGrowDay].iHeight      = giHeight;
    plPlant->GD_Plant[giGrowDay].iPotency     = giPotency;   
    plPlant->GD_Plant[giGrowDay].iMoisture    = giMoisture;    
    plPlant->GD_Plant[giGrowDay].cHealth      = giHealth;   
    plPlant->GD_Plant[giGrowDay].cSoilPH      = giSoilpH;    
    plPlant->GD_Plant[giGrowDay].cLightOn     = giLightOn;   
    plPlant->GD_Plant[giGrowDay].cLightOff    = giLightOff;   
    plPlant->GD_Plant[giGrowDay].cLightHeight = giLightHeight;    
    plPlant->GD_Plant[giGrowDay].iMass        = giGrowingMass;  
    plPlant->GD_Plant[giGrowDay].cTipsPruned  = giPrunedTips;
    }

void GLLoadGrowthVariables(PPLANT plPlant)
    { // this we must calculate each visit for every day of growth
    giGrowthStage = plPlant->PI_Plant.cGrowthStage;
    giHeight      = plPlant->GD_Plant[giGrowDay].iHeight;
    giPotency     = plPlant->GD_Plant[giGrowDay].iPotency;
    giMoisture    = plPlant->GD_Plant[giGrowDay].iMoisture;
    giHealth      = plPlant->GD_Plant[giGrowDay].cHealth;
    giSoilpH      = plPlant->GD_Plant[giGrowDay].cSoilPH;
    giLightOn     = plPlant->GD_Plant[giGrowDay].cLightOn;
    giLightOff    = plPlant->GD_Plant[giGrowDay].cLightOff;
    giLightHeight = plPlant->GD_Plant[giGrowDay].cLightHeight;
    giGrowingMass = plPlant->GD_Plant[giGrowDay].iMass;
    giPrunedTips  = plPlant->GD_Plant[giGrowDay].cTipsPruned;
    }

/******************************************************************************\
*  GROWTH STAGE AS A TEXT STRING
\******************************************************************************/


void GLGetGrowthStageString(LPSTR szBuffer, int iStage)
    {
    switch (iStage)
      {
      case 0: strcpy(szBuffer, "Seed");         break;
      case 1: strcpy(szBuffer, "Germination");  break;
      case 2: strcpy(szBuffer, "Seedling");     break;
      case 3: strcpy(szBuffer, "Vegetative");   break;
      case 4: strcpy(szBuffer, "Preflowering"); break;
      case 5: strcpy(szBuffer, "Flowering");    break;
      case 6: strcpy(szBuffer, "Seed Set");     break;
      case 7: strcpy(szBuffer, "Dead");         break;
      case 8: strcpy(szBuffer, "Harvested");    break;
      strcpy(szBuffer, "Unknown");
      } 
    }


/******************************************************************************\
*  GENERATING A RANDOM NUMBER
\******************************************************************************/

int Random(int minrand, int maxrand)
   { // returns a random number from minrand to maxrand in value
   int t = maxrand+1;
   while((t>maxrand)|(t<minrand))
         t = rand()/10;
   return t;
   }

/******************************************************************************\
*  CREATING THE FONT THAT WE USE FOR ALL OUR DIALOGS
\******************************************************************************/

HFONT GLCreateDialogFont(int iSize, int iAngle, int iStyle)
    {
    HFONT hfont = CreateFont(iSize,0,iAngle,0,iStyle,0,0,0,0,0,0,0,0,"Trebuchet");
    if(hfont)  return hfont;
    return CreateFont(iSize,0,iAngle,0,iStyle,0,0,0,0,0,0,0,0,"Times New Roman");
    }

/******************************************************************************\
*  SHOWING A NUMBER IN A CONTROL'S STATIC TEXT STRING
\******************************************************************************/


void GLSetNumText(int num, HWND hwnd, int ID)
    {
    char strbuf[20]="\0";

    _ltoa(num, strbuf, 10);
    SendDlgItemMessage(hwnd, ID, WM_SETTEXT, 0, (LPARAM)strbuf);
    }

int GLGetNumText(HWND hwnd, int ID)
    {
    char strbuf[20]="\0";

    SendDlgItemMessage(hwnd, ID, WM_GETTEXT, 20, (LPARAM)strbuf);
    return atoi(strbuf);
    }

/******************************************************************************\
*  GETTING A TEMPORARY FILENAME (WITH TEMP PATH ATTACHED)
\******************************************************************************/

int GLGetTempFilePathAndName(DWORD dlen, LPTSTR lpBuffer)
    {
    int ilen = 0;
    DWORD dw = 0;
    char szpath[MAX_PATH];
    // -----
    dw = GetTempPath(MAX_PATH, szpath);
    if(dw)
        {
        return GetTempFileName(szpath, "hgb", 0, lpBuffer);
        }
    return 0;
    }

//******************************************************************************
//*  SCRAMBLING AND UNSCRAMBLING URLS
//******************************************************************************

BOOL GLScrambleText(LPSTR lpText)
    {
    char szOutput[MAX_PATH];
    char szScramble[10];
    int  i, ilen, iptr;
    // ---------
    // check the text length
    ilen = strlen(lpText);
    if(ilen == 0) return FALSE;
    // now setup the 8 byte scramble string
    wsprintf(szScramble, "%d", 31421232);
    iptr = 0;
    for(i=0;i<=ilen;i++)
        {
        szOutput[i] = lpText[i] + (szScramble[iptr]-48);
        iptr += 1;
        if(iptr >= 8) iptr = 0;
        }
    szOutput[ilen] = 0;
    strcpy(lpText, szOutput);
    return TRUE;
    }

BOOL GLUnscrambleText(LPSTR lpText)
    {
    char szOutput[MAX_PATH];
    char szScramble[10];
    int  i, ilen, iptr;
    // ---------
    // check the text length
    ilen = strlen(lpText);
    if(ilen == 0) return FALSE;
    // now setup the 8 byte scramble string
    wsprintf(szScramble, "%d", 31421232);
    iptr = 0;
    for(i=0;i<=ilen;i++)
        {
        szOutput[i] = lpText[i] - (szScramble[iptr]-48);
        iptr += 1;
        if(iptr >= 8) iptr = 0;
        }
    szOutput[ilen] = 0;
    strcpy(lpText, szOutput);
    return TRUE;
    }

/******************************************************************************\
*  DATE CALCULATION AND FORMATTING FUNCTIONS
\******************************************************************************/

//struct tm {
//        int tm_sec;     /* seconds after the minute - [0,59] */
//        int tm_min;     /* minutes after the hour - [0,59] */
//        int tm_hour;    /* hours since midnight - [0,23] */
//        int tm_mday;    /* day of the month - [1,31] */
//        int tm_mon;     /* months since January - [0,11] */
//        int tm_year;    /* years since 1900 */
//        int tm_wday;    /* days since Sunday - [0,6] */
//        int tm_yday;    /* days since January 1 - [0,365] */
//        int tm_isdst;   /* daylight savings time flag */
//        };

BOOL GLNextDate(char *szDateBuf, int dayoffset)
    { 
    // the 3byte character array must hold the date to be converted
    // in the day/month/year format. When the function returns, the
    // new date will be placed in the same day/month/year array elements
    struct tm then;
    time_t    now;

    time(&now);
    then=*localtime(&now);
 
    then.tm_mday=szDateBuf[0];
    then.tm_mon=szDateBuf[1];
    then.tm_year=szDateBuf[2];

    if(dayoffset>0)
        then.tm_mday=then.tm_mday+dayoffset;

    mktime(&then);

    szDateBuf[0]=then.tm_mday;
    szDateBuf[1]=then.tm_mon;
    szDateBuf[2]=then.tm_year;

    return TRUE;
    }


BOOL GLPreviousDate(char *szDateBuf, int dayoffset)
    { 
    // the 3byte character array must hold the date to be converted
    // in the day/month/year format. When the function returns, the
    // new date will be placed in the same day/month/year array elements
    struct tm then;
    time_t    now;

    time(&now);
    then=*localtime(&now);
 
    then.tm_mday=szDateBuf[0];
    then.tm_mon=szDateBuf[1];
    then.tm_year=szDateBuf[2];
    
    if(dayoffset>0)
        then.tm_mday=then.tm_mday-dayoffset;

    mktime(&then);

    szDateBuf[0]=then.tm_mday;
    szDateBuf[1]=then.tm_mon;
    szDateBuf[2]=then.tm_year;

    return TRUE;
    }


BOOL GLFormatDate(char* szDateBuf, char* szResult)
    { 
    // szResult buffer must be at least 20 bytes long
    // the 3byte szDateBuf character array must hold the date to be converted
    // in the day/month/year format. When the function returns, the
    // formatted date will be placed in the szResult.
    SYSTEMTIME stThen;

    stThen.wDay   = szDateBuf[0];
    stThen.wMonth = szDateBuf[1]+1;
    stThen.wYear  = szDateBuf[2]+1900;

    if(!GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, (SYSTEMTIME *)&stThen, 
                      "ddd',' dd MMM yyyy", (LPTSTR)szResult, 20))
         return FALSE;
    else return TRUE;
    }

BOOL GLNextFormatDate(char* szDateBuf, char* szResult, int dayoffset)
    { // FORMAT = Mon, 5 June 1997
    // calculates and formats a date with a given offset from
    // the given start date.
    // szResult buffer must be at least 20 bytes long
    // the 3byte szDateBuf character array must hold the date to be converted
    // in the day/month/year format. When the function returns, the
    // formatted date will be placed in the szResult.
    char resbuf[3];
    int  i;

    for(i=0; i<3; i+=1) 
        resbuf[i] = szDateBuf[i];
    resbuf[1]-=1;  // subtract one from months makes it relative to 0

    GLNextDate((char*)&resbuf, dayoffset);

    return GLFormatDate((char*)&resbuf, (char*)szResult);
    }

BOOL GLNextShortFormatDate(char* szDateBuf, char* szResult, int dayoffset)
    { // FORMAT = DD/MM/YYYY
    // calculates and formats a date with a given offset from
    // the given start date.
    // szResult buffer must be at least 8 bytes long
    // the 3byte szDateBuf character array must hold the date to be converted
    // in the day/month/year format. When the function returns, the
    // formatted date will be placed in the szResult.
    char resbuf[3];
    int  i;

    for(i=0; i<3; i+=1) 
        resbuf[i] = szDateBuf[i];
    resbuf[1]-=1;  // subtract one from months makes it relative to 0
    GLNextDate((char*)&resbuf, dayoffset);

    wsprintf(szResult, "%02i/%02i/%04i", 
            (int)resbuf[0], (int)resbuf[1]+1, (int)resbuf[2]+1900);

    return TRUE;
    }


BOOL GLDateNow(char *szDateBuf)
    { 
    // When this function returns, the new date will be placed in the 
    // szDateBuf's day/month/year array elements
    struct tm then;
    time_t    now;

    time(&now);
    then=*localtime(&now);
 
    mktime(&then);

    szDateBuf[0]=then.tm_mday;
    szDateBuf[1]=then.tm_mon+1;
    szDateBuf[2]=then.tm_year;

    return TRUE;
    }


BOOL GLFormatDateNow(char* szResult)
    { // FORMAT = Monday, 5 June 1997
    char szDate[60]="\0";

    if(GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, NULL, 
                     "ddd',' dd MMM yyyy", (LPTSTR)&szDate, 60))
         {
         strcpy(szResult, szDate);  
         return TRUE;
         }
    return FALSE;
    }


int GLDiffDate(char *szDate1, char *szDate2)
    { // calculate the difference in days between two dates
    int  i,   iDays1,  iDays2;
    struct tm tmDate1, tmDate2;
    time_t    t_Date1, t_Date2;

    // first initialise the tm structure for Date 1
    time(&t_Date1);
    tmDate1 = *localtime(&t_Date1);
    tmDate1.tm_mday=szDate1[0];
    tmDate1.tm_mon =szDate1[1]-1;
    tmDate1.tm_year=szDate1[2];
    mktime(&tmDate1);
    // now initialise the tm structure for Date 2
    time(&t_Date2);
    tmDate2 = *localtime(&t_Date2);
    tmDate2.tm_mday=szDate2[0];
    tmDate2.tm_mon =szDate2[1]-1;
    tmDate2.tm_year=szDate2[2];
    mktime(&tmDate2);
    // calculate number of days since 01/01/80 for Date 1
    iDays1 = tmDate1.tm_yday;
    for(i=80; i<szDate1[2]; i++)
        {
        if((i-(i/4)*4)==0) // was this year a leap year?
             iDays1 += 366; 
        else iDays1 += 365;
        }
    // calculate number of days since 01/01/80 for Date 2
    iDays2 = tmDate2.tm_yday;
    for(i=80; i<szDate2[2]; i++)
        {
        if((i-(i/4)*4)==0) // was this year a leap year?
             iDays2 += 366;
        else iDays2 += 365;
        }
    // and finally return the difference between the dates
    return (iDays2 - iDays1);
    }


int GLGetTimeNowQuarters(void)
    { 
    // returns the number of quarter hour intervals passed since
    // midnight last night
    struct tm TimeNow;
    time_t    now;
    int       iQuarters;
    // first we calculate the exact time now
    time(&now);
    TimeNow=*localtime(&now);
    mktime(&TimeNow);
    // now we get the number of 15min intervals passed
    iQuarters  = (TimeNow.tm_hour * 4);
    iQuarters += (TimeNow.tm_min/15);
    // this is what we'll return then
    return iQuarters;
    }

int GLGetSecsSinceMidnight(void)
    {
    // returns the number of seconds passed since midnight last night
    struct tm TimeNow;
    time_t    now;
    int       iSeconds;
    // first we calculate the exact time now
    time(&now);
    TimeNow=*localtime(&now);
    mktime(&TimeNow);
    // now we get the number of seconds passed
    iSeconds  = (TimeNow.tm_hour * 3600); // seconds per hour since midnight
    iSeconds += (TimeNow.tm_min * 60); // add seconds per minute since midnight
    iSeconds += (TimeNow.tm_sec);
    // this is what we'll return then
    return iSeconds;
    }

/******************************************************************************\
*  PHOTOPERIOD CALC - returns the photoperiod in quarter of an hour intervals
\******************************************************************************/

int GLGrowDayPhotoperiod(PPLANT plPlant, int iGrowDay)
    {
    int i;
    int iStart = plPlant->GD_Plant[iGrowDay].cLightOn;
    int iEnd   = plPlant->GD_Plant[iGrowDay].cLightOff;
    // ---------
    if(iStart <= iEnd)
         i = iEnd - iStart;
    else i = (96 - iStart) + iEnd;
    // ------
    return i;
    }


int GLCurPlantPhotoperiod(void)
    {
    int i;
    int iStart = GD_CurPlant.cLightOn;
    int iEnd   = GD_CurPlant.cLightOff;
    // ---------
    if(iStart <= iEnd)
         i = iEnd - iStart;
    else i = (96 - iStart) + iEnd;
    // ------
    return i;
    }

/******************************************************************************\
*  FUNCTION:    Various Bitmap drawing functions
\******************************************************************************/
/*
void GLDrawBitmap(HDC hdc, RECT rc, HBITMAP hBitmap)
   {
   BITMAP   bm;
   HDC      hdcMem;
   POINT ptSize, ptOrg;
   HANDLE hPrevObject;
   
   hdcMem = CreateCompatibleDC(hdc);
   hPrevObject = SelectObject(hdcMem, hBitmap);
   SetMapMode(hdcMem, GetMapMode(hdc));
   
   GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
   ptSize.x = rc.right-rc.left;
   ptSize.y = rc.bottom-rc.top;
   DPtoLP(hdc, &ptSize, 1);
   
   ptOrg.x = 0;
   ptOrg.y = 0;
   DPtoLP(hdcMem, &ptOrg, 1);
   
   BitBlt(hdc, rc.left, rc.top, ptSize.x, ptSize.y, 
         hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);
         
   SelectObject(hdcMem, hPrevObject);
   DeleteDC(hdcMem);
   }
*/

void GLTileBitmap(HDC hdc, RECT rc, HBITMAP hBitmap)
   {
   BITMAP  bm;
   HDC     hdcMem;
   POINT   ptSize, ptOrg, ptStart;
   HANDLE  hPrevObject;
    
   hdcMem = CreateCompatibleDC(hdc);
   hPrevObject = SelectObject(hdcMem, hBitmap);
   SetMapMode(hdcMem, GetMapMode(hdc));
    
   GetObject(hBitmap, sizeof(BITMAP), (void FAR*) &bm);
   ptSize.x = bm.bmWidth;
   ptSize.y = bm.bmHeight;
   DPtoLP(hdc, &ptSize, 1);
    
   ptOrg.x = 0;
   ptOrg.y = 0;
   DPtoLP(hdcMem, &ptOrg, 1);

   ptStart.x = 0;
   ptStart.y = 0;
   DPtoLP(hdcMem, &ptStart, 1);

   while(ptStart.y < (rc.bottom-rc.top))
    {   while(ptStart.x < (rc.right-rc.left))
        {   BitBlt(hdc, ptStart.x, ptStart.y, ptSize.x, ptSize.y, 
            hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);
            ptStart.x = ptStart.x + ptSize.x;
        }   ptStart.x = 0;
        ptStart.y = ptStart.y + ptSize.y;
    } 
   SelectObject(hdcMem, hPrevObject);
   DeleteDC(hdcMem);
}
   

/******************************************************************************\
*  FUNCTION:    TileImage - tiles the image zero from the given imagelist
\******************************************************************************/

void GLTileImage(HDC hdc, RECT rcWin, HIMAGELIST hImage)
    {
    // now draw the darkness into our offscreen DC
    POINT ptStart, ptSize;
    int   iWidth  = rcWin.right-rcWin.left;
    int   iHeight = rcWin.bottom-rcWin.top;

    ptSize.x = 32;
    ptSize.y = 32;
    ptStart.x = 0;
    ptStart.y = 0;

    while(ptStart.y < iHeight)
         {   while(ptStart.x < iWidth)
             {   ImageList_Draw(hImage, 0, hdc, 
                                ptStart.x, ptStart.y, ILD_TRANSPARENT);
                 ptStart.x = ptStart.x + ptSize.x;
             }   ptStart.x = 0;
             ptStart.y = ptStart.y + ptSize.y;
         } 
    }


/******************************************************************************\
*  FUNCTION TO: Draw 3D rectangles and lines
\******************************************************************************/

void Frame3D(HDC hdc, RECT rc, HANDLE hTopPen, HANDLE hBottomPen)
   {
   HANDLE hPrevPen;
   // select the top pen and draw
   hPrevPen = SelectObject(hdc, hTopPen);
   MoveToEx(hdc, rc.left, rc.bottom, NULL);
   LineTo(hdc, rc.left, rc.top);
   LineTo(hdc, rc.right, rc.top);
   // select the bottom pen and draw
   SelectObject(hdc, hBottomPen);
   MoveToEx(hdc, rc.right, rc.top, NULL);
   LineTo(hdc, rc.right, rc.bottom);
   LineTo(hdc, rc.left, rc.bottom);
   SelectObject(hdc, hPrevPen);
   }

void Rect3D(HDC hdc, RECT rc, HANDLE hPen, HANDLE hBrush)
   {
   HANDLE hPrevPen, hPrevBrush;
   // select the top pen and draw
   hPrevPen   = SelectObject(hdc, hPen);
   hPrevBrush = SelectObject(hdc, hBrush);
   Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
   SelectObject(hdc, hPrevPen);
   SelectObject(hdc, hPrevBrush);
   }

void FrameRect3D(HDC hdc, RECT rc, HPEN hFacePen, HPEN hTopPen, HPEN hBotPen)
   {
   // first adjust the rect and draw the top highlight rect
   OffsetRect(&rc, -1, -1);
   Rect3D(hdc, rc, hTopPen, hTopPen);
   // now adjust and draw the bottom lowlight rect
   OffsetRect(&rc, 2, 2);
   Rect3D(hdc, rc, hBotPen, hBotPen);
   // now readjust the rect and draw the face 
   OffsetRect(&rc, -1, -1);
   Rect3D(hdc, rc, hFacePen, hFacePen);
   }

void Rectangle3D(HDC hdc, RECT rc, HPEN hFPen, HBRUSH hFBrush, HPEN hTPen, HPEN hBPen)
   {
   // first reduce his rect a little, as we'll draw outside it
   InflateRect(&rc, -1, -1);
   // first adjust the rect and draw the top highlight rect
   OffsetRect(&rc, -1, -1);
   Rect3D(hdc, rc, hTPen, hFBrush);
   // now adjust and draw the bottom lowlight rect
   OffsetRect(&rc, 2, 2);
   Rect3D(hdc, rc, hBPen, hFBrush);
   // now readjust the rect and draw the face 
   OffsetRect(&rc, -1, -1);
   Rect3D(hdc, rc, hFPen, hFBrush);
   }



/******************************************************************************\
*  SHOWING COLOURED PERCENTAGE WINDOWS IN THE ACTIVITY TABBED DIALOGS
\******************************************************************************/


void GLDisplayProgress(HWND hDlg, int ID, HBRUSH hBrush, int cur, int max)
    {
    RECT   rcWin;
    int    i, j;
    HWND   hwnd = GetDlgItem(hDlg, ID);
    HDC    hdc  = GetDC(hwnd);

    GetClientRect(hwnd, &rcWin);
    j = min(cur, max);
    i = ((rcWin.bottom-rcWin.top)*j)/max;
    rcWin.top = (rcWin.bottom-rcWin.top)-i;

    FillRect(hdc, &rcWin, hBrush);
    ReleaseDC(hwnd, hdc);
    }

/******************************************************************************\
*  CENTERING DIALOGS IN THE CLIENT AREA
\******************************************************************************/


void DlgCenter(HWND hDlg)
   // Center the current dialog on the screen. Must be called from
   // the dialog message handler during wm_initdialog processing.
   {
   RECT rcDlg, rcParent;  // scratch rect 
   POINT ptWin;   
   
   GetWindowRect(hDlg, &rcDlg);
   GetClientRect(GetParent(hDlg), &rcParent);
   // make an adjustment for the toolbar
   rcParent.top += TBToolbarHeight();

   ptWin.x = ((rcParent.right-rcParent.left)-(rcDlg.right-rcDlg.left))/2;
   ptWin.y = ((rcParent.bottom-rcParent.top)-(rcDlg.bottom-rcDlg.top))/2;

   ClientToScreen(GetParent(hDlg), &ptWin);

   MoveWindow(hDlg, ptWin.x, ptWin.y+TBToolbarHeight(), 
             rcDlg.right-rcDlg.left, rcDlg.bottom-rcDlg.top, TRUE);
   }


/******************************************************************************\
* Resizes the dialog to fit inside our Main Program window
\******************************************************************************/

void GLSizeDialogToFit(HWND hdlg)
    {
    // Resizes the dialog inside the HighGrow Program window. Must be called
    // fram the dialog message handler during wm_initdialog processing.
    int   iEdge   = 8;
    POINT ptPOrgn, ptPSize;
    HWND  hParent = GetParent(hdlg);
    RECT  rcParent;
    int   iSideOfs= GetSystemMetrics(SM_CYSIZEFRAME);
    int   iTopOfs = TBToolbarHeight() + 10 + GetSystemMetrics(SM_CYMENU)
                  + GetSystemMetrics(SM_CYSMCAPTION)
                  + GetSystemMetrics(SM_CYSIZEFRAME);
    
    // first we get the size of our parent window
    GetWindowRect(hParent, &rcParent);
    InflateRect(&rcParent, -iEdge, -iEdge);
    ptPOrgn.x = rcParent.left+iSideOfs;
    ptPOrgn.y = rcParent.top+iTopOfs;
    ptPSize.x = rcParent.right-rcParent.left-(iSideOfs*2);
    ptPSize.y = rcParent.bottom-rcParent.top-iTopOfs-iSideOfs;
    MoveWindow(hdlg, ptPOrgn.x, ptPOrgn.y, ptPSize.x, ptPSize.y, TRUE);

    }


/******************************************************************************\
*  THE FILE EXISTS ROUTINE
\******************************************************************************/

BOOL GLDoesFileExist(LPSTR lpPlantFile)
   {
   struct _finddata_t hgp_file;
   long hFile;

   if((hFile = _findfirst(lpPlantFile, &hgp_file)) != -1L)
      {
      _findclose(hFile);
      return TRUE;
      }
   return FALSE;
   }


/******************************************************************************\
*  THE FILE COUNTING ROUTINE
\******************************************************************************/

int GLGetFileCount(LPCSTR szFileSpec)
   {  // Count number of toonfiles in current directory
   struct _finddata_t midi_file;
   long hFile;
   int  i = 0;
   // Find first toonfile in current directory 
   if((hFile = _findfirst((char*)szFileSpec, &midi_file)) != -1L)
      {
      i = 1;
      while(_findnext(hFile, &midi_file)==0)
         i += 1;
      _findclose(hFile);
      }
    return i;
   }

/******************************************************************************\
*  THE BACKGROUND IMAGE FILE CONVERTING ROUTINE
\******************************************************************************/

int GLConvertGrowroomImageFiles(void)
   {  // Encrypt any JPG images found in the password directory
   struct _finddata_t hgb_file;
   long hFile;
   int  i = 0;
   BOOL bSecure = TRUE;
   char szFilePath[MAX_PATH];
   char szFileSpec[MAX_PATH];
   char szRoomName[50];
   // -----------------------
   // Set the file spec to search for
   GetCurrentDirectory(MAX_PATH, (LPTSTR)&szFilePath);
   strcat(szFilePath, "\\");
   // check if he has a password set
   if(strlen(gszCurPassword)) 
       { // if his password is our global encryption password
       if(!strcmp(lpSecurePassword, gszCurPassword))
           { // add the password to the file path
           // NOTE: free image files are created in a folder with the same
           // name as the global encryption password
           strcat(szFilePath, gszCurPassword);
           strcat(szFilePath, "\\");
           bSecure = FALSE;
           }
       }
   strcpy(szFileSpec, szFilePath);
   strcat(szFileSpec, "*.jpg");
   // Find first toonfile in current directory 
   if((hFile = _findfirst((char*)szFileSpec, &hgb_file)) != -1L)
      {
      i = 1;
      strcpy(szFileSpec, szFilePath);
      strcat(szFileSpec, hgb_file.name);
      strcpy(szRoomName, hgb_file.name);
      szRoomName[strlen(szRoomName)-4] = 0;
      CRConvertAndEncryptImageFile((LPSTR)szFileSpec, szRoomName, TRUE);
      while(_findnext(hFile, &hgb_file)==0)
          {
          i += 1;
          strcpy(szFileSpec, szFilePath);
          strcat(szFileSpec, hgb_file.name);
          strcpy(szRoomName, hgb_file.name);
          szRoomName[strlen(szRoomName)-4] = 0;
          CRConvertAndEncryptImageFile((LPSTR)szFileSpec, szRoomName, TRUE);
          }
      _findclose(hFile);
      }
    return i;
   }

/******************************************************************************\
*  FILLING A COMBOBOX WITH BACKGROUND IMAGE FILE NAMES
\******************************************************************************/

int GLCheckRoomInUse(LPSTR szRoomName)
    { // returns the OB1 room number if the given room name is already in use
    int i, iReturn;
    // now we check we already have a room with this name
    for(i=0;i<gMaxRooms;i++)
        {
        iReturn = strcmp((LPSTR)GR_Room[i].szRoomName, szRoomName);
        if(iReturn==0)    return i+1;
        }
    return 0;
    }


int GLFillGrowFileNamesCombo(HWND hDlg, UINT ComboID)
   {  // Count number of toonfiles in current directory
   struct _finddata_t hgb_file;
   long hFile;
   int  i = 0;
   int  iUse;
   char szFileSpec[MAX_PATH];
   char szRoomName[MAX_PATH];
   char szComboText[MAX_PATH];
   // -----------------------
   // we must always empty the contents first
   SendDlgItemMessage(hDlg, ComboID, CB_RESETCONTENT, 0, 0);
   // Set the file spec to search for
   GetCurrentDirectory(MAX_PATH, (LPTSTR)&szFileSpec);
   strcat(szFileSpec, "\\*.hgb");
   // Find first toonfile in current directory 
   if((hFile = _findfirst((char*)szFileSpec, &hgb_file)) != -1L)
      {
      i = 1;
      strcpy(szRoomName, hgb_file.name);
      szRoomName[strlen(hgb_file.name)-4] = 0;
      iUse = GLCheckRoomInUse(szRoomName);
      if(iUse) wsprintf(szComboText, "%s (%i)", szRoomName, iUse);
      else     strcpy(szComboText, szRoomName);
      SendDlgItemMessage(hDlg, ComboID, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)szComboText);
      while(_findnext(hFile, &hgb_file)==0)
          {
          i += 1;
          strcpy(szRoomName, hgb_file.name);
          szRoomName[strlen(hgb_file.name)-4] = 0;
          iUse = GLCheckRoomInUse(szRoomName);
          if(iUse) wsprintf(szComboText, "%s (%i)", szRoomName, iUse);
          else     strcpy(szComboText, szRoomName);
          SendDlgItemMessage(hDlg, ComboID, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)szComboText);
          }
      _findclose(hFile);
      }
   SendDlgItemMessage(hDlg, ComboID, CB_SETCURSEL, 0, 0);
   return i;
   }


/******************************************************************************\
*  FILLING A LISTBOX WITH BACKGROUND IMAGE FILE NAMES
\******************************************************************************/
 
void GLFillGrowroomNamesListbox(HWND hDlg, UINT ListID)
   {  // Count number of toonfiles in current directory
	int i;
    HWND hList=GetDlgItem(hDlg, ListID);
    // getout if there is no listbox
    if(hList == 0) return;
    // resets combo entries to 0
    SendMessage(hList, LB_RESETCONTENT, 0, 0); 
    // Show our standard growroom names
    SendMessage(hList, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Workshop");
    SendMessage(hList, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Basement");
    SendMessage(hList, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Attic");
    // fill the combo with the growroom names
    for(i=3;i<gMaxRooms;i++)
		{
        if(strlen(GR_Room[i].szRoomName))
            {
		    SendMessage(hList, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)GR_Room[i].szRoomName);
            }
		}
	// and select the first growroom
    SendMessage(hList, LB_SETCURSEL, 0, 0L);
	}

/******************************************************************************\
*  FUNCTION:    InitGrowMenus - handles greying and naming of menus
\******************************************************************************/
  

void GLInitGrowMenus(HWND hwnd)
    { 
    // * uses the info in the PI_PlantSeed array to check which of the three
    //   plants are already growing.
    // * If he has planted any seeds, the "growroom", "lock growroom" and the
    //   history and activities entries on the "Plants" menu are enabled.
    // * If no plants have been planted, then we do nothing, as these menu items
    //   are already greyed as resources.
    int   i, j, iState;
    BOOL  bPlanted   = FALSE;
    HMENU hPlantMenu = GetMenu(hwnd);
    int  iroomofs = (TBGetComboGrowroom()-1)*3;

    for(i=0;i<3;i++) // for all three plants
        { // if he's set the start date (day cannot = 0)
        j = i+iroomofs;
        if(PI_PlantSeed[j].szStartDate[0]) 
            {
            iState = MF_BYCOMMAND|MF_ENABLED;
            sprintf(strbuf, "%s..\tCtrl+%i", PI_PlantSeed[j].szPlantName, i+1);
            ModifyMenu(hPlantMenu,i+IDM_ACT01,MF_BYCOMMAND|MF_STRING,i+IDM_ACT01,strbuf);
            sprintf(strbuf, "%s..\tShft+Ctrl+%i", PI_PlantSeed[j].szPlantName, i+1);
            ModifyMenu(hPlantMenu,i+IDM_HIST01,MF_BYCOMMAND|MF_STRING,i+IDM_HIST01,strbuf);
            sprintf(strbuf, "%s..", PI_PlantSeed[j].szPlantName, i+1);
            ModifyMenu(hPlantMenu,i+IDM_HARV01,MF_BYCOMMAND|MF_STRING,i+IDM_HARV01,strbuf);
            bPlanted=TRUE; // set our flag, he's planted at least one
            }
        else
            {
            iState = MF_BYCOMMAND|MF_GRAYED;
            sprintf(strbuf, "Seed %i", i+1);
            ModifyMenu(hPlantMenu,i+IDM_ACT01, MF_BYCOMMAND|MF_STRING,i+IDM_ACT01, strbuf);
            ModifyMenu(hPlantMenu,i+IDM_HIST01,MF_BYCOMMAND|MF_STRING,i+IDM_HIST01,strbuf);
            ModifyMenu(hPlantMenu,i+IDM_HARV01,MF_BYCOMMAND|MF_STRING,i+IDM_HARV01,strbuf);
            }
        EnableMenuItem(hPlantMenu, i+IDM_ACT01,  iState);
        EnableMenuItem(hPlantMenu, i+IDM_HIST01, iState);
        EnableMenuItem(hPlantMenu, i+IDM_HARV01, iState);
        }
    // now, if he planted any seeds,
    if(bPlanted)  iState = MF_BYCOMMAND|MF_ENABLED;
    else          iState = MF_BYCOMMAND|MF_GRAYED;
    EnableMenuItem(hPlantMenu, IDM_GROWCHART,iState);

    // if the Harvested Plant Log file doesn't exist, grey it's menu item
    if((GLDoesFileExist((LPSTR)lpLogFile))&(gbRegistered))
        EnableMenuItem(hPlantMenu, IDM_PLOG, MF_BYCOMMAND|MF_ENABLED);
    // if he has no MIDI files, or no sound playback facilities, grey the sound item
    if((GetMP3FileCount()==0)&&(GetMidiFileCount()==0))
        {
        EnableMenuItem(hPlantMenu, IDM_SOUND,   MF_BYCOMMAND|MF_GRAYED);
        EnableMenuItem(hPlantMenu, IDM_SOUNDON, MF_BYCOMMAND|MF_GRAYED);
        }
    // check the registry if he wants music played
//    if(PACheckSoundEnabled()&(midiOutGetNumDevs()>0))
    if(PACheckSoundEnabled())
        CheckMenuItem(hPlantMenu,IDM_SOUNDON,MF_BYCOMMAND|MF_CHECKED);
    // check the registry if he wants Auto-load on windows startup
    if(PACheckAutoLoad())
        CheckMenuItem(hPlantMenu,IDM_AUTOLOAD,MF_BYCOMMAND|MF_CHECKED);
    // check the registry if he wants to enter the growroom on startup
    if(PACheckAutoGrowroom())
        CheckMenuItem(hPlantMenu,IDM_AUTOGROW,MF_BYCOMMAND|MF_CHECKED);
    // check the registry and see if he's enabled the vacation mode calculations
    if(PACheckVacModeEnabled()) // check if he's enabled the vacation mode
        CheckMenuItem(hPlantMenu,IDM_VACMODEON,MF_BYCOMMAND|MF_CHECKED);
    // check the registry and see if he's enabled the Alarm clock check
    if(PACheckAlarmSet()) // check if he's enabled the alarm
        CheckMenuItem(hPlantMenu,IDM_ALARMSET,MF_BYCOMMAND|MF_CHECKED);
    DrawMenuBar(hwnd);
    }



