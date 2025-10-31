/*****************************************************************************\
*
*  PLANTLOG.C - HANDLING GLOBAL MEMORY AND SAVING & READING THE PLANT LOG FILE
*
\*****************************************************************************/

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include "resource.h"
#include "global.h"
#include "harvest.h"
#include "dibitmap.h"
#include "seedlist.h"
#include "registry.h"
   
/*****************************************************************************\
*  SOME CONSTANTS
\*****************************************************************************/
    
 LPCSTR     lpLogFile ="PlantLog.hgp";
 OFSTRUCT   LogOfStruct;  // information from OpenFile()
 HGLOBAL    hPlantLog=0;  // handle to our global memory
 int        hPLogFile=0;  // currently opened file handle
 PPLANTLOG  P_Log;        // pointer to global memory
 PLANTLOG   PG_Plant[gMaxLogEntries]; // array of log entries

/*****************************************************************************\
*  ALLOCATING & HANDLING GLOBAL MEMORY
/*****************************************************************************/


void PGPlantLogMemFree(void)
   {
   if(!hPlantLog) return;
   GlobalFree(hPlantLog);
   hPlantLog = 0;
   }


BOOL PGPlantLogMemInit(void)
   {
   PGPlantLogMemFree();
   hPlantLog = GlobalAlloc(GPTR, (DWORD)(sizeof(PG_Plant)));
   if(!hPlantLog) return FALSE;
   return TRUE;
   }


/*****************************************************************************\
*  THE MAIN FILE OPEN ROUTINE
\*****************************************************************************/


BOOL PGPlantLogFileOpen(void)
   {
   // Open the file and get its handle
   hPLogFile=OpenFile(lpLogFile, (LPOFSTRUCT)&LogOfStruct, OF_READWRITE);
   if(!hPLogFile)  return FALSE;
   return TRUE;
   }

BOOL PGPlantLogFileOpenNew(void)
   {
   // Open the file and get its handle
   hPLogFile=OpenFile(lpLogFile, (LPOFSTRUCT)&LogOfStruct,
                      OF_READWRITE|OF_CREATE);
   if(!hPLogFile)  return FALSE;
   return TRUE;
   }

void PGPlantLogFileClose(void)
   {
   if(!hPLogFile)  return;
   _lclose(hPLogFile);
   hPLogFile = 0;
   }


BOOL PGPlantLogFileRead(void)
   {  // file must be open
   LONG IOresult;    // result of a file read
   DWORD dw;

   if(!hPLogFile) return FALSE;
   // Allocate Plant buffer to the size of the file
   dw = GetFileSize((HANDLE)hPLogFile, NULL);

   // lock memory and read file
   P_Log   = (PPLANTLOG)GlobalLock(hPlantLog);
   IOresult = _hread(hPLogFile, P_Log, sizeof(PG_Plant));

   // # bytes read must equal to file size - header size
   if(IOresult != (LONG)(sizeof(PG_Plant)))
       {
       GlobalUnlock(hPlantLog);
       return FALSE;
       }
   GlobalUnlock(hPlantLog);
   return TRUE;
   }


BOOL PGPlantLogFileWrite(void)
    { // file must be open
    LONG IOresult;    // result of a file write
    // lock the memory
    P_Log   = (PPLANTLOG)GlobalLock(hPlantLog);
    // and save the Global Plant buffer
    IOresult = _hwrite(hPLogFile, (LPCSTR)P_Log, sizeof(PG_Plant));
    // unlock the memory again
    GlobalUnlock(hPlantLog);
    // check the results status
    if(IOresult!=(LONG)(sizeof(PG_Plant)))
        return FALSE;
    return TRUE;
    }


// ********************************************************************
// * OPENS & READS PLANTLOG FILE INTO GLOBAL MEMORY
// ********************************************************************

BOOL PGLogFileToMem(void)
   { // opens the plant log, reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hPlantLog. If fails, we get null.
   BOOL bSuccess = FALSE;
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist((LPSTR)lpLogFile)) return 0;
   // now set up the memory required
   if(!PGPlantLogMemInit()) return 0;
   // if the file exists, open it
   if(!PGPlantLogFileOpen())   return 0;
   // then read the file into our memory
   bSuccess = PGPlantLogFileRead();
   // free our memory if we failed to read the file
   if(!bSuccess) PGPlantLogMemFree();
   // and close it again
   PGPlantLogFileClose();

   return bSuccess;
   }


// ********************************************************************
// * OPENS & WRITES PLANTLOG LOG FILE FROM GLOBAL MEMORY
// ********************************************************************

BOOL PGLogMemToFile(void)
   { // opens the plant log, reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hPlantLog. If fails, we get null.
   BOOL bSuccess = FALSE;
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist((LPSTR)lpLogFile)) return 0;
   // get out if we don't have global memory required
   if(!hPlantLog)            return 0;
   // if the file exists, open it
   if(!PGPlantLogFileOpen()) return 0;
   // save the global memory into the file
   bSuccess = PGPlantLogFileWrite();
   // free our global memory
   PGPlantLogMemFree();
   // and close the file again
   PGPlantLogFileClose();
   return bSuccess;
   }


/******************************************************************************\
*  THE PLANTLOG DATA SAVING ROUTINE
\******************************************************************************/

BOOL PGPlantLogSaveDetails(void)
    { // save the harvest values for the next entry in the logfile
    int i=0;
    // if the global memory hasn't been created, getout
    if(!hPlantLog) return FALSE;
    // lock the memory first
    P_Log = (PPLANTLOG)GlobalLock(hPlantLog);
    // now adjust the memory pointer to the next free entry
    while((P_Log->PI_Plant.szPlantName[0] != 0)&(i<gMaxLogEntries-1))
          { // if the first byte of the name is set, then we have an entry
          P_Log += 1;  // jump to the next plant
          i     += 1;  // this is just incase we are full
          }       
    // first store the grower's name and harvest date
    REReadRegistryKey("Grower's Name", (LPCTSTR)P_Log->szGrowerName, 30);
    GLDateNow((char*)&P_Log->szHarvestDate);
    // now copy the PlantInfo Struct elements
    strcpy(P_Log->PI_Plant.szPlantName, PI_CurPlant.szPlantName);
    for(i=0 ;i<3; i+=1)
        P_Log->PI_Plant.szStartDate[i] = PI_CurPlant.szStartDate[i];
    P_Log->PI_Plant.cGrowthStage  = PI_CurPlant.cGrowthStage;
    P_Log->PI_Plant.cSeedChoice   = PI_CurPlant.cSeedChoice;
    P_Log->PI_Plant.bRegistered   = PI_CurPlant.bRegistered;
    P_Log->PI_Plant.dwFileVersion = PI_CurPlant.dwFileVersion;
    // now copy the harvest day's Growday Struct elements
    P_Log->GD_Plant.bVisited     =  GD_CurPlant.bVisited;
    P_Log->GD_Plant.bWatered     =  GD_CurPlant.bWatered;
    P_Log->GD_Plant.bFertilized  =  GD_CurPlant.bFertilized;
    P_Log->GD_Plant.iMass        =  GD_CurPlant.iMass;
    P_Log->GD_Plant.iHeight      =  GD_CurPlant.iHeight;
    P_Log->GD_Plant.iPotency     =  GD_CurPlant.iPotency;
    P_Log->GD_Plant.iMoisture    =  GD_CurPlant.iMoisture;
    P_Log->GD_Plant.cHealth      =  GD_CurPlant.cHealth;
    P_Log->GD_Plant.cSoilPH      =  GD_CurPlant.cSoilPH;
    P_Log->GD_Plant.cLightOn     =  GD_CurPlant.cLightOn;
    P_Log->GD_Plant.cLightOff    =  GD_CurPlant.cLightOff;
    P_Log->GD_Plant.cLightHeight =  GD_CurPlant.cLightHeight;
    P_Log->GD_Plant.cTipsPruned  =  GD_CurPlant.cTipsPruned;
    for(i=0; i<4; i+=1)
        P_Log->GD_Plant.szNutrients[i] = GD_CurPlant.szNutrients[i];
    // finally unlock our memory again
    GlobalUnlock(hPlantLog);
    return TRUE;
    }


/******************************************************************************\
*  THE PLANTLOG CREATION ROUTINE (WHEN HE HARVESTS HIS FIRST PLANT)
\******************************************************************************/


BOOL PGSaveNewPlantLogFile(void)
    { // saves the harvest time details of the Plant into a
    // new plantlog for the give plant. This function also
    // sets default values for day 0 of plant growth.
    // now set up the memory required
    if(!PGPlantLogMemInit())     return 0;
    // create the new plant log file
    if(!PGPlantLogFileOpenNew()) return 0;
    // set the last GrowDay info
    if(!PGPlantLogSaveDetails()) 
        {
        // we must close the file again
        PGPlantLogFileClose();
        // also free the memory again
        PGPlantLogMemFree();
        return 0;
        }
    // save the global memory into the file
    if(!PGPlantLogFileWrite())   return 0;
    // and close the file again
    PGPlantLogFileClose();
    // we'll always return true
    PGPlantLogMemFree();
    // now set our file to be hidden
    SetFileAttributes(lpLogFile, FILE_ATTRIBUTE_HIDDEN);
    return TRUE;
    }


/******************************************************************************\
*  THE PLANTLOG UPDATE ROUTINE (ONCE THE FIRST PLANT'S BEEN HARVESTED)
\******************************************************************************/


BOOL PGSaveExistPlantLogFile(void)
    {
    if(!GLDoesFileExist((LPSTR)lpLogFile)) return 0;
    if(!PGLogFileToMem())                  return 0;
    if(!PGPlantLogSaveDetails())           
        {
        // free the memory again
        PGPlantLogMemFree();
        return 0;
        }
    if(!PGLogMemToFile())  return 0;
    return TRUE;
    }


/******************************************************************************\
*  THE PLANTLOG API WORD FOR SAVING PLANT DETAILS AT HARVEST TIME
\******************************************************************************/


BOOL PGAddCurPlantToLogFile(void)
    {
    // saves the harvest time details of the Plant into the logfile
    // if the logfile doesn't exist yet, create it and store this plant's
    // details at log position 0.
    // If the file does exist, we add it after the previous entry in the log
    if(GLDoesFileExist((LPSTR)lpLogFile))
         return PGSaveExistPlantLogFile();
    else return PGSaveNewPlantLogFile();
    }



/******************************************************************************\
*  THE PLANTLOG HISTORY DIALOG
\******************************************************************************/

HFONT    hPLogBigFont;
HFONT    hPLogMedFont;
HBITMAP  hPlantLogBmp;
HPALETTE hPlantLogPal;
int      iLogPlants;


void PGPlantListBoxChange(HWND hDlg, HINSTANCE hInst)
    {
    int i, iPlant;
    // if he selects another plant from the listbox...
    // first check which plant is currently selected
    iPlant= SendDlgItemMessage(hDlg, IDG_LB01, LB_GETCURSEL, 0,0);
    // getout if invalid plant entry
    if(iPlant<0)           return;
    if(iPlant>=iLogPlants) return;
    // if the global memory hasn't been created, getout
    if(!hPlantLog)         return;
    // lock the memory first
    P_Log = (PPLANTLOG)GlobalLock(hPlantLog);
    // offset to the current plant
    P_Log += iPlant;
    // first we display the plant name
    SendDlgItemMessage(hDlg,IDG_ST01,WM_SETTEXT,0,
                        (LPARAM)P_Log->PI_Plant.szPlantName);
    // now we'll display the grower's name
    SendDlgItemMessage(hDlg,IDG_ST20,WM_SETTEXT,0,
                        (LPARAM)P_Log->szGrowerName);
    // load and display the seed variety text
    LoadString(hInst, (10+P_Log->PI_Plant.cSeedChoice), strbuf, 50);
    SendDlgItemMessage(hDlg, IDG_ST21, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the start date
    GLNextFormatDate((char*)&P_Log->PI_Plant.szStartDate, (char*)strbuf, 0);
    SendDlgItemMessage(hDlg, IDG_ST22, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the harvested date
    GLNextFormatDate((char*)&P_Log->szHarvestDate, (char*)strbuf, 0);
    SendDlgItemMessage(hDlg, IDG_ST23, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the days of growth
    i = GLDiffDate((char*)&P_Log->PI_Plant.szStartDate, 
                   (char*)&P_Log->szHarvestDate);
    GLSetNumText(i+1, hDlg, IDG_ST24);
    // get and display the harvest time growth stage
    GLGetGrowthStageString(strbuf, P_Log->PI_Plant.cGrowthStage);
    SendDlgItemMessage(hDlg, IDG_ST25, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the health at harvest time
    sprintf(strbuf, "%i %%", P_Log->GD_Plant.cHealth);
    SendDlgItemMessage(hDlg, IDG_ST26, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the potency at harvest time
    sprintf(strbuf, "%1.2f %%THC", (P_Log->GD_Plant.iPotency/100.00));
    SendDlgItemMessage(hDlg, IDG_ST27, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the height in mm at harvest time
    sprintf(strbuf, "%i mm", P_Log->GD_Plant.iHeight);
    SendDlgItemMessage(hDlg, IDG_ST28, WM_SETTEXT, 0, (LPARAM)strbuf);
    // get and display the harvested mass in grams
    sprintf(strbuf, "%i grams", P_Log->GD_Plant.iMass);
    SendDlgItemMessage(hDlg, IDG_ST29, WM_SETTEXT, 0, (LPARAM)strbuf);
    // finally unlock our memory again
    GlobalUnlock(hPlantLog);
    }


void PGInitPlantLogDialog(HWND hDlg, HINSTANCE dhInst)
    {
    int i;
    long lp=MAKELPARAM(TRUE, 0);
//    char szDateNow[3];
    // first center the dialog in our window
    DlgCenter(hDlg);
    // load our leaf bitmap
    hPlantLogBmp = DILoadResourceBitmap(dhInst,"Leaf256",&hPlantLogPal);
    // now we'll create some bold fonts
    hPLogBigFont = GLCreateDialogFont(24,0,FW_BOLD);
    hPLogMedFont = GLCreateDialogFont(19,0,FW_BOLD);
    SendDlgItemMessage(hDlg,IDG_ST01,WM_SETFONT,(WPARAM)hPLogBigFont,lp);
    for(i=0;i<10;i++)
        {
        SendDlgItemMessage(hDlg,IDG_ST02+i,WM_SETFONT,(WPARAM)hPLogMedFont,lp);
        }
    SendDlgItemMessage(hDlg, IDG_LB01, LB_RESETCONTENT, 0, 0L);
    for(i=0; i<iLogPlants; i++)
        SendDlgItemMessage(hDlg, IDG_LB01, LB_ADDSTRING, 0, 0L);
    SendDlgItemMessage(hDlg, IDG_LB01, LB_SETCURSEL, 0, 0L);
    // now show the current plant's harvest statistics
    PGPlantListBoxChange(hDlg, dhInst);
    }


 
BOOL CALLBACK PGPlantLogDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   LPDRAWITEMSTRUCT    lpDIS;
   LPMEASUREITEMSTRUCT lpMIS;
   
    switch (message)
      {
      case WM_INITDIALOG:
            PGInitPlantLogDialog(hDlg, dhInst);
      return (TRUE);

      case WM_ERASEBKGND:
          if(-1==GetDeviceCaps((HDC)wParam, NUMCOLORS))
              { // if we're in high colour, we'll draw the light leaf background
              DITileDlgLeafBitmap(hDlg, (HDC)wParam);
              return (TRUE);
              }
      return (FALSE);

      case WM_CTLCOLORSTATIC:
          {
          UINT uiID = GetDlgCtrlID((HWND)lParam);
          if((IDG_ST01 != uiID)&&
             (IDG_ST20 != uiID)&&
             (IDG_ST21 != uiID)&&
             (IDG_ST22 != uiID)&&
             (IDG_ST23 != uiID)&&
             (IDG_ST24 != uiID)&&
             (IDG_ST25 != uiID)&&
             (IDG_ST26 != uiID)&&
             (IDG_ST27 != uiID)&&
             (IDG_ST28 != uiID)&&
             (IDG_ST29 != uiID))
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_MEASUREITEM:
           if(wParam==IDG_LB01)
		     {
             lpMIS = (LPMEASUREITEMSTRUCT)lParam;
             lpMIS->itemHeight = 172;
             return(TRUE);
		     }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_DRAWITEM:
          {
          RECT  rcNumber;
          HFONT hFont;

          lpDIS = (LPDRAWITEMSTRUCT)lParam;
          // first fill with white
          FillRect(lpDIS->hDC, &lpDIS->rcItem, GetStockObject(WHITE_BRUSH));
          // now draw the leaf
          DIDrawBitmap(lpDIS->hDC, lpDIS->rcItem.left, lpDIS->rcItem.top, 
                        hPlantLogBmp, hPlantLogPal, SRCCOPY);
          InflateRect(&lpDIS->rcItem, -2, -2);
          // now draw the number
          CopyRect(&rcNumber, &lpDIS->rcItem);
          InflateRect(&rcNumber, -5, -5);
          sprintf(strbuf, "%i", lpDIS->itemID+1);
          SetBkMode(lpDIS->hDC, TRANSPARENT);
          hFont = SelectObject(lpDIS->hDC, hPLogMedFont);
          DrawText(lpDIS->hDC, strbuf, strlen(strbuf), &rcNumber, DT_LEFT);
          SelectObject(lpDIS->hDC, hFont);
          // now draaw the rectangles
          if(lpDIS->itemState & ODS_SELECTED)
              FrameRect(lpDIS->hDC, &lpDIS->rcItem, GetStockObject(BLACK_BRUSH));
          if(lpDIS->itemState & ODS_FOCUS)
              DrawFocusRect(lpDIS->hDC, &lpDIS->rcItem);
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_DESTROY:
          {
          if(hPLogBigFont) DeleteObject(hPLogBigFont);
          if(hPLogMedFont) DeleteObject(hPLogMedFont);
          DIFreeBitmap(hPlantLogBmp, hPlantLogPal);
          }
      break;

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDG_LB01:
               if(HIWORD(wParam)==LBN_SELCHANGE)
                 PGPlantListBoxChange(hDlg, dhInst);
            break;

            case IDOK: 
            case IDCANCEL: 
                EndDialog(hDlg, FALSE); 
            return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          



/******************************************************************************\
*  CHECKING IF WE CAN START THE PLANTLOG HISTORY DIALOG
\******************************************************************************/

int PGCountPlantLogEntries(HWND hwnd)
    { // counts the number of harvested plants in the plant log
    int i=0;

    // if the global memory hasn't been created, getout
    if(!hPlantLog) return 0;
    // although fixed, lock the global memory first
    P_Log = (PPLANTLOG)GlobalLock(hPlantLog);
    // now adjust the memory pointer to the next free entry
    while((P_Log->PI_Plant.szPlantName[0] != 0)&(i<gMaxLogEntries))
          { // if the first byte of the name is set, then we have an entry
          P_Log += 1;  // jump to the next plant
          i     += 1;  // this is just incase we are full
          }
    // we're done, so unlock the global memory again
    GlobalUnlock(hPlantLog);
    // and return the harvested plant count
    return i;
    }


BOOL PGStartPlantLog(HWND hwnd, HINSTANCE hInst)
    {
    // first check if he's using the registered version
    if(!gbRegistered) 
        {
        HAUnavailableDialog(hwnd, hInst);
        return FALSE;
        }
    // now check if the file exists
    if(!GLDoesFileExist((LPSTR)lpLogFile))
        { // tell him if it doesn't
        MessageBox(hwnd, "Harvested Plant Log file not found",
                   "HighGrow Message", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
        }
    // now read the file into global memory
    if(!PGLogFileToMem()) return FALSE;
    // count the number of entries in the log file
    iLogPlants = PGCountPlantLogEntries(hwnd);
    // if we have no entries, tell him and getout
    if(!iLogPlants)
        { // free the memory, give him a message and getout
        PGPlantLogMemFree();
        MessageBox(hwnd, "No plants found in the HighGrow Harvested Plant Log",
                   "HighGrow Message", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
        }
    // now show the plant log dialog
    DialogBox(hInst, "PlantLog", hwnd, PGPlantLogDlgProc);
    // free the global memory now
    PGPlantLogMemFree();
    return TRUE;
    }


