      
/******************************************************************************\
*
*  PROGRAM:     HighGrow.C
*
*  PURPOSE:     Growing Plants.
*
\******************************************************************************/
#include <windows.h>
#include <time.h>
#include "stdio.h"
#include "resource.h"
#include "highgrow.h"
#include "global.h"
#include "calc.h"
#include "activity.h"
#include "plantmem.h"
#include "seedlist.h"
#include "history.h"
#include "growroom.h"
#include "toolbar.h"
#include "plantseed.h"
#include "Robbie.h"
#include "RobbieWin.h"
#include "Comments.h"
#include "Password.h"
#include "DiBitmap.h"
#include "Harvest.h"
#include "Plantlog.h"
#include "Midi.h"
#include "trayicon.h"             
#include "clock.h"                
#include "growedit.h"                
#include "htmlhelp.h"
#include "jpgview.h"
#include "internet.h"
#include "seedimp.h"    // **** only required for testing seed exporting ****
    
/******************************************************************************\
*  GLOBAL VARS: For this file only
\******************************************************************************/

#define HG_CHECKTIMER 101 // id of our conditions checking timer

HANDLE hAccel=0;

BOOL bAutoLoadEnabled=FALSE; // set if \A is found on the command line
BOOL bAutoGrowEnabled=FALSE; // set if \G is found on the command line

char gszCommandLine[MAX_PATH];
         
/******************************************************************************\
*
*  FUNCTION:    WinMain (standard WinMain INPUTS/RETURNS)
*
*  GLOBAL VARS: ghwndMain - handle of main app window
*
*  LOCAL VARS:  msg - msg to get/dispatch
*
\******************************************************************************/
                
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow)
{
  MSG  msg;
  WNDCLASS wc;
  WNDCLASS wcGr;
  char lpCommandLine[MAX_PATH]="\0";
  HWND hCheckWnd=NULL;

//  wsprintf(lpCommandLine, "Hello Paul = %s", lpCmdLine);
//  Blow(lpCommandLine);
   // first we copy the command line into a global buffer for checking at create time
   strcpy(gszCommandLine, lpCmdLine); // copy command line string
   // here we copy the command line so that we can adjust and check it
   strcpy(lpCommandLine, lpCmdLine); // copy command line string
   _strlwr(lpCommandLine);        // and lowercase it (for comparisons)
  // now check if we're already running
  hCheckWnd = FindWindow("HighGrow", NULL);
  if(hCheckWnd)
      {  // if we're already running
      // first check and change to this program's original startup directory
      PACheckStartupDirectory();
      // now we show the window & reset the focus
      ShowWindow(hCheckWnd, SW_SHOWNORMAL);
      SetForegroundWindow(hCheckWnd);
      // now we'll attempt to download the image
      if(strncmp(lpCommandLine, "highgrow://", 11)==0)
          { // start the file download and encryption immediately
          INGetInternetRoomImageFile(hCheckWnd, hInstance, gszCommandLine);
          }
      return 0;
      }

  if(!hPrevInstance)
    {
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = (WNDPROC)MainWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance, "HIGHGROW");
    wc.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject (WHITE_BRUSH);
    wc.lpszMenuName  = (LPCTSTR) "Menu";
    wc.lpszClassName = (LPCTSTR) "HighGrow";

    if(!RegisterClass (&wc))
        {
        MessageBox(NULL, "RegisterClass() function failed", 
                   "HighGrow", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
        }

    wcGr.style         = CS_DBLCLKS|CS_OWNDC;
    wcGr.lpfnWndProc   = (WNDPROC)GrowRoomWndProc;
    wcGr.cbClsExtra    = 0;
    wcGr.cbWndExtra    = 0;
    wcGr.hInstance     = hInstance;
    wcGr.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcGr.hCursor       = NULL;
    wcGr.hbrBackground = GetStockObject (WHITE_BRUSH);
    wcGr.lpszMenuName  = NULL;
    wcGr.lpszClassName = (LPCTSTR) "GrowRoom";

    if(!RegisterClass (&wcGr))
        {
        MessageBox(NULL, "RegisterClass() function failed", 
                   "GrowRoom", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
        }

    // we'll also try to register the Analog clock window's class
    CLRegisterClockClass(hInstance);
    }

   // check if /A auto-load parameter switch was found on command line
   if(strstr(lpCommandLine,"/a")!=0)
      bAutoLoadEnabled = TRUE;
   // check if /G auto-growroom parameter switch was found on command line
   if(strstr(lpCommandLine,"/g")!=0)
      bAutoGrowEnabled = TRUE;

   if(!(ghwndMain = CreateWindowEx (0L, "HighGrow", NULL,
                                    WS_BORDER|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
                                    (GetSystemMetrics(SM_CXSCREEN)-640)/2,
                                    (GetSystemMetrics(SM_CYSCREEN)-480)/2,
                                    640, 480,
                                    NULL, NULL, hInstance, NULL)))
    return (0);

//  ShowWindow (ghwndMain, nCmdShow);

  while (GetMessage (&msg, NULL, 0, 0))
      {
      if(!TranslateAccelerator(ghwndMain, hAccel, &msg))
          {
          TranslateMessage (&msg);
          DispatchMessage  (&msg);
          }
      }

  return (msg.wParam);
}

/******************************************************************************\
*  SHOWING THE DATE IN THE WINDOW CAPTION TEXT
\******************************************************************************/

void HGSetWindowCaption(HWND hwnd)
    { // sets the current date in the caption bar of the main window
    char strdate[60];
    char strbuff[150];

    GetDateFormat(LOCALE_SYSTEM_DEFAULT,0,NULL,"dddd',' dd MMMM yyyy",(LPTSTR)&strdate, 60);
    sprintf(strbuff, "HighGrow (Freeware Version 4.20) - %s", strdate);

    SetWindowText(hwnd, strbuff);
    }

/******************************************************************************\
*  CALCULATION PROGRESS MODELESS DIALOG
\******************************************************************************/

HWND hModelessDlg=0;

BOOL CALLBACK HGModelessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   static HFONT     hHdrFont=0;
   const  HINSTANCE hInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);

   switch (message)
      {
      case WM_INITDIALOG:
            {
            // first we show our wait cursor
            SetCursor(hWaitCursor);
            DlgCenter(hDlg);
            hHdrFont = GLCreateDialogFont(18,0,FW_BOLD);
            SendDlgItemMessage(hDlg, IDC_ST01, WM_SETFONT, (WPARAM)hHdrFont, 0);
            SendDlgItemMessage(hDlg, IDC_PR01, PBM_SETRANGE, 0, MAKELPARAM(0, (int)lParam));
            UpdateWindow(hDlg);
            }
      return (TRUE);

      case WM_DESTROY:
           SetCursor(hNormCursor);
           if(hHdrFont) 
                {
                DeleteObject(hHdrFont);
                hHdrFont = 0;
                }
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
          SetBkMode((HDC)wParam, TRANSPARENT);
          return (int)GetStockObject(NULL_BRUSH);
          }
      return (FALSE);  // only done because we're redirecting the focus
      }
    return (FALSE); 
   }                          

void HGCreateModelessDialog(HWND hWnd, HINSTANCE hInst)
    {
    hModelessDlg = CreateDialogParam(hInst, "calc", hWnd, 
                                     (DLGPROC)HGModelessDlgProc, 18);
    }

void HGShowProgress(HWND hWnd, int iCurProgress, BOOL bVisible)
    {
    if(!bVisible)
         {
         SendDlgItemMessage(hModelessDlg, IDC_PR01, PBM_SETPOS, iCurProgress, 0);
         UpdateWindow(hModelessDlg);
         }
    else TBShowProgress(iCurProgress);
    iCurProgress += 1; 
    }

void HGEndModelessDialog(BOOL bVisible)
    {
    if(bVisible)     TBEndProgress();
    if(hModelessDlg) DestroyWindow(hModelessDlg);
    }

/******************************************************************************\
* ALPHA-BLENDING THE STAR BITMAP
\******************************************************************************/

int AlphaCallbackCount = 0;
int AlphaTimerID       = 0;

int iInsertX           = 450;
int iInsertY           = 145;

HBITMAP  hStarBitmap   = 0;
HPALETTE hStarPalette  = 0;

// ----------------------------------------------------------
// THE FREEWARE STAR DRAWING ALPHA-BLENDING CALLBACK FUNTION
// ----------------------------------------------------------

void CALLBACK HGAlphaBlendCallback(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
    {
    if(AlphaCallbackCount<50)
        {
        HDC hdc = GetDC((HWND)dwUser);
        DIAlphaBlendBitmap(hdc, hStarBitmap, iInsertX, iInsertY, AlphaCallbackCount);
        ReleaseDC((HWND)dwUser, hdc);
        AlphaCallbackCount += 1;
        }
    else 
        {
        timeKillEvent(uTimerID);
        AlphaTimerID = 0;
        }
    }

 
/******************************************************************************\
*  CHECK WHICH PLANTS ARE GROWING, & CALCULATE ALL GROWTH SINCE LAST VISIT
\******************************************************************************/

int HGCalculatePlants(HWND hWnd, HINSTANCE hInst, BOOL bVisible)
    { 
    // * checks which plant files are around, and then updates
    //   each plant's nodes since his last visit
    int i, iCalculated =0;
    int iCurProgress   =0;
    BOOL bAnyPlants    =FALSE; // we don't calculate none plants
    char szFile[256]   ="\0";
    // first we check if we even have any plants to calculate
    // so as not to calculate and show progress for no plants
    for(i=0;i<gMaxPlants;i++)
        {
        sprintf(szFile, "Plant%02i.hgp", i+1);
        if(GLDoesFileExist(szFile))
            bAnyPlants = TRUE;
        }
    // now we getout if we still have no plants
    if(bAnyPlants == FALSE) return 0;
    // now check if we must initialise the progress window
    if(bVisible) TBInitProgress((5*gMaxRooms), "Calculating New Growth..");
    // now perform plant's each calculation, showing our progress
    for(i=0;i<gMaxPlants;i++)
        {
        giCurPlant = i+1; // stupid global
        sprintf(szFile, "Plant%02i.hgp", i+1);
        if(GLDoesFileExist(szFile))
            {
            iCurProgress += 1; HGShowProgress(hWnd, iCurProgress, bVisible);
            if(PMPlantFileToMem(i))
                { // first lock our global memory
                P_Plant = (PPLANT)GlobalLock(hPlantMem);
                // first we fill this seed's growth characteristics
                PSInitPlantSeeds(i, P_Plant->PI_Plant.cSeedChoice-1);
                iCurProgress += 1; HGShowProgress(hWnd, iCurProgress, bVisible);
                // now initialize our plant seed info struct
                SLInitPlantSeed((PPLANT)P_Plant, i);
                iCurProgress += 1; HGShowProgress(hWnd, iCurProgress, bVisible);
                // now we calculate all the growth factors
                CACalculatePlant((PPLANT)P_Plant, i);
                SLUpdatePlantSeedGrowthStage((PPLANT)P_Plant, i);
                // also set our oldest plant's growday
                giOldestGrowDay = max(giOldestGrowDay, giGrowDay);
                iCurProgress += 1; HGShowProgress(hWnd, iCurProgress, bVisible);
                // we can unlock our memory again
                GlobalUnlock(hPlantMem);
                // finally, save our changes back
                PMPlantMemToFile(i);
                iCurProgress += 1; HGShowProgress(hWnd, iCurProgress, bVisible);
                }
            else 
                {
                iCurProgress += 4; HGShowProgress(hWnd, iCurProgress, bVisible);
                }
            // increase our seeds planted count
            iCalculated += 1;
            }
        else
            { // set our light positions etc.
            iCurProgress += 5; HGShowProgress(hWnd, iCurProgress, bVisible);
            }
        }
    // hide our growth calculation progress control
    HGEndModelessDialog(bVisible);
    // and return the number of plants we calculated
    return iCalculated;
    }


/******************************************************************************\
*
*  FUNCTION:    DoCommand (handles windows command messages)
*
\******************************************************************************/

BOOL bDontPaint = FALSE;

BOOL DoCommand(HWND hwnd, WPARAM wParam, HINSTANCE ghInst)
   {
   int iofs = (TBGetComboGrowroom()-1)*3;

   switch (LOWORD(wParam))
      {
      case IDM_OBJECTIVES:
//           GEDeleteRoom();
           TBStartObjectivesDialog(hwnd, ghInst);
      return TRUE;

      case IDM_PLANTING:
           TBStartPlantingDialog(hwnd, ghInst);
      return TRUE;

      case IDM_NEWGROW:
            GEStartAddGrowRoomDialog(hwnd, ghInst);
      return TRUE;

      case IDM_EDITGROW:
           TBStartEditGrowRoomDialog(hwnd, ghInst);
      return TRUE;

      case IDC_TBGRCOMBO:
           if(HIWORD(wParam)==CBN_SELCHANGE)
              {
              bDontPaint = TRUE;
              TBUpdateGrowroomCombo();
              TBUpdatePlantCombo();
              GLInitGrowMenus(hwnd);
              GRCheckToRedrawGrowRoom(hwnd, ghInst);
              UpdateWindow(hwnd);
              bDontPaint = FALSE;
              }
      break;

      case IDC_TBCOMBO:
           if(HIWORD(wParam)==CBN_SELCHANGE)
              {
              TBUpdatePlantCombo();
              }
      break;

      case IDM_GROWROOM:
           { 
           // if star is still alphablending, kill the timer
	        if(AlphaTimerID) 
                {
                timeKillEvent(AlphaTimerID);
                AlphaTimerID = 0;
                }
           // enter the currently selected growroom
           TBStartGrowRoom(hwnd, ghInst);
           // this shows the growroom without flashing
           UpdateWindow(hwnd); 
           // now start playing that midi file
           TBGrowroomMusicPlayback(hwnd);
           }
      return TRUE;

      case IDM_GROWCHART:
          if(PI_PlantSeed[0+iofs].szStartDate[0]|
             PI_PlantSeed[1+iofs].szStartDate[0]|
             PI_PlantSeed[2+iofs].szStartDate[0])
              { // if at least one plant is growing
              TBStartGrowthChart(hwnd, ghInst);
              }
      return TRUE;

      case IDM_ROBBIE:
           TBStartRobbieRules(hwnd);
      return TRUE;

      case IDM_ACT01:
           if(PI_PlantSeed[0+iofs].szStartDate[0])
              ACShowActivitySheet(hwnd, ghInst, 1+iofs, 0);
      return TRUE;

      case IDM_ACT02:
           if(PI_PlantSeed[1+iofs].szStartDate[0])
              ACShowActivitySheet(hwnd, ghInst, 2+iofs, 0);
      return TRUE;

      case IDM_ACT03:
           if(PI_PlantSeed[2+iofs].szStartDate[0])
              ACShowActivitySheet(hwnd, ghInst, 3+iofs, 0);
      return TRUE;

      case IDM_HIST01:
           if(PI_PlantSeed[0+iofs].szStartDate[0])
               {
               TBSetComboPlant(0);
               HIShowHistoryDialog(hwnd, ghInst, 1);
               }
      return TRUE;

      case IDM_HIST02:
               {
               TBSetComboPlant(1);
               HIShowHistoryDialog(hwnd, ghInst, 1);
               }
      return TRUE;

      case IDM_HIST03:
               {
               TBSetComboPlant(2);
               HIShowHistoryDialog(hwnd, ghInst, 1);
               }
      return TRUE;

      case IDM_HARV01:
           if(PI_PlantSeed[0+iofs].szStartDate[0])
               HAShowPlantHarvestDialog(hwnd, ghInst, 1+iofs);
      return TRUE;

      case IDM_HARV02:
           if(PI_PlantSeed[1+iofs].szStartDate[0])
               HAShowPlantHarvestDialog(hwnd, ghInst, 2+iofs);
      return TRUE;

      case IDM_HARV03:
           if(PI_PlantSeed[2+iofs].szStartDate[0])
               HAShowPlantHarvestDialog(hwnd, ghInst, 3+iofs);
      return TRUE;

      case IDM_STATS:
           TBStartActivitySheet(hwnd, ghInst, IDM_STATS);
      return TRUE;

      case IDM_GLOG:
            TBStartGrowthLog(hwnd, ghInst);
      return TRUE;

      case IDM_PLOG:
            PGStartPlantLog(hwnd, ghInst);
      return TRUE;

      case IDM_HARVEST:
            TBStartHarvestDialog(hwnd, ghInst);
      return TRUE;

      case IDM_LOCK:
           PASetPassword(hwnd, ghInst);
      return TRUE;

      case IDM_HELPCONTENTS:
             HtmlHelp(hwnd, "HighGrow.chm", HH_DISPLAY_TOPIC, (DWORD)"Contents.htm");
      return TRUE;

      case IDM_HELPTIPS:
             HtmlHelp(hwnd, "HighGrow.chm", HH_DISPLAY_TOPIC, (DWORD)"Hints_and_tips_for_better_growth.htm");
      return TRUE;

      case IDM_SOUND:
           TBStartStopMusicPlayback(hwnd);
      return TRUE;

      case IDM_SOUNDON:
           TBToggleSoundOption(hwnd, ghInst);
      return TRUE;

      case IDM_VACMODEON:
           { // set the vacation mode to either on or off
           TBStartVacationEnabledDialog(hwnd, ghInst);
           // also redraw the growroom to show the change
           GRReRenderGrowRoom();
           }
      return TRUE;

      case IDM_ALARMSET:
           TBStartAlarmSetDialog(hwnd, ghInst);
      return TRUE;

      case IDM_AUTOLOAD:
           TBToggleAutoLoadOption(hwnd);
      return TRUE;

      case IDM_AUTOGROW:
           TBToggleAutoGrowroomOption(hwnd);
      return TRUE;

      case IDM_ABOUT:
           HAShowAboutDialog(hwnd, ghInst);
      return TRUE;
                  
      case IDM_EXIT:
           DestroyWindow(hwnd);
      return TRUE;
      }
   return FALSE;
   }
       
/******************************************************************************\
*
*  FUNCTION:    MainWndProc (standard window procedure INPUTS/RETURNS)
*
\******************************************************************************/

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
  BOOL   bAutoLoad;
  BOOL   bAutoGrow;
  static DWORD    dwtime=0;
  static HBITMAP  hBackground;
  static HPALETTE hPalette;
  static HBITMAP  hStarBmp;
  static BOOL     bIsProgramRunning; // so that we don't repaint on startup
  static int     iPrevSecs;       // used to check when we must recalculate
  // static int     iLastCalcedSecs; // number of seconds since we last recalculated
  HINSTANCE ghInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

  switch (message)
    {
    case WM_CREATE:
         // first check how we've been started up (ie AutoLoad?)
         bAutoLoad = (PACheckAutoLoad() & bAutoLoadEnabled);
         // now check if he wants to enter the growroom on startup
         bAutoGrow = (PACheckAutoGrowroom() | bAutoGrowEnabled);
         // Show an alternative splashbox, with the calculate progress control
         if(bAutoLoad)  HGCreateModelessDialog(hwnd, ghInst);
         srand((unsigned)time(NULL)); // initialize random number generation
         // add the icon to the system tray
         TRTaskBarAddIcon(hwnd, ghInst, "HighGrow - Legal Marijuana Growing");
         // before calculating, check if godmode password is set
//         PACheckGodMode();
         // now check and change to this program's original startup directory
         PACheckStartupDirectory();
         // before calculating, check if registration unlocking code is set
//         PACheckRegistered();
         // check if he's found the Easter egg BoomBox
         PACheckBoomBox();
         // here we initialise all the growroom info from our growroom file
         GELoadRoomDetailsFromFile();
         // now create the tool and cool bars
         TBCreateCoolbar(hwnd, ghInst); // creates our toolbar
         // show today's date in our caption
         HGSetWindowCaption(hwnd);
         // load some resources
         hNormCursor = LoadCursor(NULL, IDC_ARROW);
         hWaitCursor = LoadCursor(NULL, IDC_WAIT);
         hAccel      = LoadAccelerators(ghInst, "HOTKEYS");
         // load the title image
         hBackground = DILoadResourceBitmap(ghInst, "Title", &hPalette);
         // ALSO load the dialog background image (used often)
         DILoadDlgLeafBitmap(ghInst);
         // now that we've loaded our resources, we may show the window
         if(!bAutoLoad)
             { // if he doesn't want to auto-load, we'll show it immediately
             ShowWindow(hwnd, SW_SHOW);
             UpdateWindow(hwnd); // ensured immediate painting
             }
//       GLInitGrowroomInfo();
         // now that we're in the correct directory, calculate our plants
         HGCalculatePlants(hwnd, ghInst, !bAutoLoad);
//         iLastCalcedSecs = 0; 
         // now we check if he's got a password, or
         // if he's cheated in any way
         if(PACheckProgramAccess(hwnd, ghInst)==FALSE)
             { // if he failed, we close our program
             PostMessage(hwnd, WM_CLOSE, 0, 0L);
             break;
             }
         // load the freeware star bitmap
         hStarBitmap = DILoadResourceBitmap(ghInst, "FreeStar", &hStarPalette);
         // do the bitmap alpha-blending check over here
         // set the star blending timer callback going
         gbAlphaBlending = DILoadAlphaBlendLibrary();
         if((gbAlphaBlending)&&(!bAutoGrow  ))
               {
                AlphaCallbackCount = 0;
                AlphaTimerID = timeSetEvent(25, 0, HGAlphaBlendCallback, (DWORD)hwnd, 
                                              TIME_PERIODIC|TIME_CALLBACK_FUNCTION);
                }
         // read the type of music file we're dealing with
         PACheckMIDISoundFileType();
         // now we can refresh our menu and toolbar
         TBUpdateGrowroomCombo();
         TBQueryEnableButtons();
         // setup the menus after the shareware warning (incase he registers)
         GLInitGrowMenus(hwnd);
         // He's allowed access, set the light timer to check every second
         SetTimer(hwnd, HG_CHECKTIMER, 1000, NULL);
         // set our variable to allow child windows to realize plaettes
         bIsProgramRunning = TRUE;
         // warn him of the shareware remaining plant growth days
//         PACheckShareWarning(hwnd, ghInst);
         // now we go into the growroom if the /G parameter switch was found
         if(!bAutoLoad & bAutoGrow)
             { // we'll simply force our command button (protection)
             DoCommand(hwnd, MAKEWPARAM(IDM_GROWROOM,0), ghInst);
             }
         // now we check if our path includes the http:// or highgrow:// protocols
         if((strncmp(gszCommandLine, "highgrow://", 11)==0)||
            (strncmp(gszCommandLine, "http://",      7)==0))
             { // start the file download and encryption immediately
             INGetInternetRoomImageFile(hwnd, ghInst, gszCommandLine);
             }
    break;

    case MYWM_NOTIFYICON:
         TR_MYWM_NOTIFYICON(hwnd, ghInst, wParam, lParam);
    break;

    case WM_COMMAND:
         DoCommand(hwnd, wParam, ghInst);
    break;

    case WM_NOTIFY:
         TBToolbarNotify(ghInst, lParam);
    break;

    case MM_MCINOTIFY:
        {
        DWORD dw = GetTickCount();
        if(wParam==MCI_NOTIFY_SUCCESSFUL)
            {
            if(((lParam==(LONG)wMP3DeviceID)&&(bMP3Playing==TRUE))||
                (lParam==(LONG)wMidiDeviceID)&&(bMidiPlaying==TRUE))
                {
                if(dw >= (dwtime + 10000))
                    {
                    TBPlayNextRandomMusicFile(hwnd);
                    dwtime = dw;
                    }
                }
            }
        }
    break;
    
    case WM_PAINT:
         {
         PAINTSTRUCT ps;
         HDC hdc;
         // -----
         hdc = BeginPaint(hwnd, &ps);
         if(bDontPaint == FALSE)
             {
             DIDrawBitmap(hdc, 0,25, hBackground, hPalette, SRCCOPY);
             DIDrawBitmap(hdc, iInsertX, iInsertY, hStarBitmap, hStarPalette, SRCCOPY);
             }
         EndPaint(hwnd, &ps);
         }
    break;

    case WM_PALETTECHANGED:
         { // to redraw window when a child dialogs displays 256 colours
         if(bIsProgramRunning)  // if we're already visible
             {
             RECT rc;
             GetClientRect(hwnd, &rc);
             rc.top += TBToolbarHeight()-1;
             InvalidateRect(hwnd, &rc, 0);
             }
         }
    break;

    case WM_SIZE:
        TBSizeCoolbar(hwnd, lParam);
    break;

    case WM_TIMECHANGE:
         { // the Windows date or time have been reset
         // first we must recalculate the plants
         HGCalculatePlants(hwnd, ghInst, TRUE);
//         iLastCalcedSecs = 0; 
         // now do the cheat count check
         if(PACheckProgramAccess(hwnd, ghInst)==FALSE)
             { // if he failed, we close our program
             PostMessage(hwnd, WM_CLOSE, 0, 0L);
             break;
             }
         }
    break;

    case WM_TIMER:
         if(wParam==HG_CHECKTIMER)
             { // first get seconds elapsed since midnight
             int iSecs = GLGetSecsSinceMidnight();
             // now if it's less than before, we could have passed midnight
             if(iSecs<iPrevSecs)
                 { 
                 // if it's not within one minute of midnight,
                 // he could be cheating by setting the time back
//                 if(iSecs >= 60)
//                     { // check if he's cheating by adjusting the time
//                     // if it's not been readjusted by daylight savings time
//                     if(iLastCalcedSecs <= (3*60*60)) // calc'ed within 3 hours?
//                         { // we'll tell him that he's cheated and close
//                         KillTimer(hwnd, HG_CHECKTIMER); // must kill timer first
//                         DialogBox(ghInst, "Cheat", (HWND)hwnd, (DLGPROC)CheatDlgProc);
//                         PostMessage(hwnd, WM_CLOSE, 0, 0L);
//                         return FALSE;
//                         }
//                     }
                 // if he hasn't cheated, we'll continue recalculating
                 HGCalculatePlants(hwnd, ghInst, IsWindowVisible(hwnd));
//                 iLastCalcedSecs = 0; // initialize this to zero again
                 HGSetWindowCaption(hwnd); // show new date in caption
                 if(GetDlgItem(hwnd, 0)) // if we have a growroom...
                    GRReRenderGrowRoom(); // ...we show new growth
                 }
             // always update the prevsecs variable
             iPrevSecs = iSecs;
             // also increment our last calculated seconds count
//             iLastCalcedSecs += 1;
             // now if we have a growroom, redraw the clock 
             if(GetDlgItem(hwnd, 0))
                GRUpdateGrowRoom(hwnd);
             // now we check if we must sound our alarm clock
             if(PACheckAlarmTime(iSecs))
                 TBStartRobbieAlarmWalk(hwnd, ghInst);
             }
    break;

    case WM_DRAWITEM:
          if(wParam==IDC_TBCOMBO)
             TBODrawComboPlantItem((LPDRAWITEMSTRUCT)lParam);
          if(wParam==IDC_TBGRCOMBO)
             TBODrawComboGrowroomItem((LPDRAWITEMSTRUCT)lParam);
    break;

    case WM_MEASUREITEM:
          if((wParam==IDC_TBGRCOMBO)||(wParam==IDC_TBCOMBO))
              TBMeasureGrowComboItem((LPMEASUREITEMSTRUCT)lParam);
    break;

    case WM_CLOSE:
         ShowWindow(hwnd, SW_HIDE);
         DestroyWindow(hwnd);
    break;

    case WM_ENDPLAYBACK:
         TBCheckEndRulesPlayback(hwnd);
    break;

    case WM_DESTROY:
        TBDestroyToolbar(hwnd);
        GRExitGrowRoom();
//        DestroyAnimationWindow();
        RRFreeRobbieLibrary(hwnd);
        COFreeCommentLibrary();
        KillTimer(hwnd, HG_CHECKTIMER);
        if(gbAlphaBlending)
            {
            // free the alpha-blending DLL
            DIFreeAlphaBlendLibrary();
            // set this global back to zero
            gbAlphaBlending = FALSE;
            // kill the timer if still going
	        if(AlphaTimerID) timeKillEvent(AlphaTimerID);
            }
        if(bIsProgramRunning)  // if we started up successfully before closing
           PASaveEndCheatChecks(); // save critical cheat info in the registry
        if(hAccel)        DestroyAcceleratorTable(hAccel);
        if(hNormCursor)   DestroyCursor(hNormCursor);
        if(hWaitCursor)   DestroyCursor(hWaitCursor);
        DIFreeBitmap(hStarBitmap, hStarPalette);
        DIFreeBitmap(hBackground, hPalette);
        DIFreeDlgLeafBitmap();
        TRTaskBarDeleteIcon(hwnd);
        PostQuitMessage(0);
    break;

    default:
        return (DefWindowProc(hwnd, message, wParam, lParam));
    }
  return (0);
}


