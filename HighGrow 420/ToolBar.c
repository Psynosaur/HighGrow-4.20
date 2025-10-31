/****************************************************************************
* PROGRAM: Toolbar.c
****************************************************************************/

#include <windows.h>    // includes basic windows functionality
#include <commctrl.h>   // includes the common control header
#include <io.h>
#include "stdio.h"
#include "toolbar.h"
#include "global.h"
#include "activity.h"
#include "history.h"
#include "resource.h"
#include "chart.h"
#include "seedlist.h"
#include "growedit.h"
#include "growroom.h"
#include "harvest.h"
#include "robbiewin.h"
#include "registry.h"
#include "password.h"
#include "midi.h"
#include "versinfo.h"
#include "vacation.h"
#include "highgrow.h"
          
#define ID_TOOLBAR      2
#define ID_PROGRESS     4
#define ID_TSTATIC      5
#define ID_TCLOCK       6
#define ID_COOLBAR      10
#define NUM_TOOLBUTTONS 10
#define NUM_TOOLSEPS    34
#define NUM_COMBOSEPS   34
#define NUM_TOOLS       NUM_TOOLBUTTONS + NUM_TOOLSEPS -1

HWND hwndRebar    = NULL;
HWND hWndToolbar  = NULL;      
HWND hWndCombo    = NULL;
HWND hWndGrowCombo= NULL;
HWND hWndProgress = NULL;
HWND hWndStatic   = NULL;
HWND hWndClock    = NULL;

HFONT hClockFont;

// Window procedures for combo box subclassing
WNDPROC lpfnDefCombo;

HCURSOR hNormCursor;
HCURSOR hWaitCursor;

/******************************************************************************\
*  SHOWS, HIDES, AND UPDATES THE CLOCK STATIC WINDOW (GROWROOM MODE ONLY)
\******************************************************************************/

void TBShowClock(BOOL bShow)
    { // shows or hides the clock static toolbar window
    if(hWndClock==NULL) return;

    if(bShow==TRUE)
         ShowWindow(hWndClock, SW_SHOW);
    else ShowWindow(hWndClock, SW_HIDE);
    }

void TBUpdateClock(void)
    {
    char szTime[15];
    // getout if the static is not visible or the window wasn't created
    if(IsWindowVisible(hWndClock)==FALSE) return;
    if(hWndClock==NULL) return;
    // now get the local time
    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, NULL,
                  "hh':'mm':'ss tt",(LPTSTR)&szTime, 14);
    // and update the static window's text
    SendMessage(hWndClock, WM_SETTEXT, 0, (LPARAM)szTime);
    }


/******************************************************************************\
*  INITIALIZES AND MAINTAINS GROWTH CALCULATION PROGRESS WINDOW
\******************************************************************************/

int giCurToolProgress = 0;
BOOL bClockHidden = FALSE; // true if we've had to hide the clock window

void TBInitProgress(int iMax, LPCTSTR szText)
    { // we do this before we call the growth calculation function
    if(!hWndProgress)     return;
    if(giCurToolProgress) return; // getout if still busy with another
    // reset this current progress to zero 
    giCurToolProgress = 0;
    // first we show our wait cursor
    SetCursor(hWaitCursor);
    // now we must check if the clock static is visible.
    // if it is, we must hide it and set our flag, so that we know
    // to unhide it again after progress window is hidden again
    if(IsWindowVisible(hWndClock)==TRUE)
         {
         ShowWindow(hWndClock, SW_HIDE);
         bClockHidden = TRUE;
         }
    else bClockHidden = FALSE;
    // we first show our progress and static controls
    ShowWindow(hWndStatic, SW_SHOW);
    ShowWindow(hWndProgress, SW_SHOW);
    // now we set the supplied text for the progress status
    SendMessage(hWndStatic, WM_SETTEXT, 0, (LPARAM)szText);
    // now we set the min (zero) and max positions for our progress window
    SendMessage(hWndProgress, PBM_SETRANGE, 0, MAKELPARAM(0, iMax));
    // force the painting now, as we're still starting up the program
    UpdateWindow(hWndStatic);
    UpdateWindow(hWndProgress);
    }

void TBShowNextProgress(void)
    {
    if(!hWndProgress) return;
    giCurToolProgress += 1;
    // now we set the current position for our progress window
    SendMessage(hWndProgress, PBM_SETPOS, giCurToolProgress, 0);
    }

void TBShowProgress(int iCurProgress)
    {
    if(!hWndProgress) return;
    // now we set the current position for our progress window
    SendMessage(hWndProgress, PBM_SETPOS, iCurProgress, 0);
    }

void TBEndProgress(void)
    { // we do this after we're done with growth calculations
    // we hide our progress and static controls
    ShowWindow(hWndStatic, SW_HIDE);
    ShowWindow(hWndProgress, SW_HIDE);
    // if we hid the clock static, we must unhide it again
    if(bClockHidden==TRUE)
       ShowWindow(hWndClock, SW_SHOW);
    // finally we unset our wait cursor
    SetCursor(hNormCursor);
    // reset this current progress to zero 
    giCurToolProgress = 0;
    }


/******************************************************************************\
*  CHECKS WHICH PLANT IS CURRENTLY SELECTED IN THE COMBOBOX
\******************************************************************************/


int TBGetComboPlant(void)
    { // gets the OB1 number of the selected plant in the combo
	int i = 0;
    int iRoomOfs = (TBGetComboGrowroom()-1)*3;
    // returns 0 if there aren't any
    char szName[40];
    // first read the selected plant's name
    SendMessage(hWndCombo, WM_GETTEXT, 40, (LPARAM)szName);
    // now check which plant number this belongs to
	for(i=0;i<3;i++)
		{
		if(strcmp(szName, PI_PlantSeed[i+iRoomOfs].szPlantName)==0) 
			return i+iRoomOfs+1;
		}
    // there aren't any plants, so return false
    return 0;
    }

/******************************************************************************\
*  REMOVES THE GIVEN PLANT (OB0) FROM THE COMBOBOX
\******************************************************************************/

void TBRemoveCurPlant(void)
    { // assumes giCurPlant is set when called
    int  i;
    int  iRoomOfs = (TBGetComboGrowroom()-1)*3;
    char  szFile[28]="\0";
    BOOL  bAnyRemaining;
    HINSTANCE hInst = (HINSTANCE)GetWindowLong(ghwndMain, GWL_HINSTANCE);

    sprintf(szFile, "Plant%02i.hgp", giCurPlant);
    if(GLDoesFileExist(szFile))
        { // find the current plant in the combobox
        i = (int)SendMessage(hWndCombo, CB_FINDSTRING, (WPARAM)-1, 
                             (LPARAM)PI_PlantSeed[giCurPlant-1].szPlantName);
        // if we've found it, remove it and set the comboitem to 0
        if(i>=0) 
            {
            SendMessage(hWndCombo, CB_DELETESTRING, i, 0);
            SendMessage(hWndCombo, CB_INSERTSTRING, i, (LPARAM)"");
            SendMessage(hWndCombo, CB_SETCURSEL, i, 0);
            }
        // now erase this plant in our plantseed array
        PI_PlantSeed[giCurPlant-1].cSeedChoice    = 0;
        PI_PlantSeed[giCurPlant-1].szStartDate[0] = 0;
        PI_PlantSeed[giCurPlant-1].szPlantName[0] = 0;
        // delete the file
        DeleteFile(szFile);
        // now we check if we have any plants left in the growroom
        bAnyRemaining = FALSE;
        for(i=0;i<3;i++)
            {
            if(PI_PlantSeed[i+iRoomOfs].szStartDate[0]) 
                bAnyRemaining = TRUE;
            }
        // remove the plant from the DrawPlant struct and re-draw the growroom
        i = giCurPlant - iRoomOfs - 1;
        GRRemoveDrawPlant(i);
        // reset the oldest growday global to recalc it next time the game is started
        giOldestGrowDay = 0; // saves in registry on close
        // if we're currently in the growroom, AND we have NO MORE plants
        if(IsWindow(hGrowRoomWindow))
            { // if no more plants, get out of the growroom
            if(bAnyRemaining==FALSE)
                { 
                // starting the growroom also exits the growroom 
                TBStartGrowRoom(ghwndMain, hInst);
                // now enable or disable the menu items and toolbar buttons
                TBQueryEnableButtons();
                }
            else // otherwise, we'll redraw the growroom and remaining plants
                {
                GRReRenderGrowRoom(); 
                }
            }
        // now update the combobox windows to reflect the changes
        InvalidateRect(hWndGrowCombo, NULL, TRUE);
        InvalidateRect(hWndCombo, NULL, TRUE);
        // finally, we fix the plant menus
        GLInitGrowMenus(ghwndMain);
        }
    }

/******************************************************************************\
*  STARTS UP THE SELECTED ACTIVITY PROPERTY SHEET FOR THE SELECTED COMBOPLANT
\******************************************************************************/

BOOL TBStartActivitySheet(HWND hwnd, HINSTANCE hInst, int idButton)
    { // shows the property sheet dialog for the selected toolbar button
    int iIndex;
    int iState;
    // getout if it's an invalid id
    if((idButton>=IDM_STATS && idButton<=IDM_PRUNE) == FALSE)  
        return FALSE;
    // check which plant is selected in the combobox
    iIndex = TBGetComboPlant();
    // getout if we have an invalid combo plant selection
    if(iIndex == 0) return FALSE;
    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, idButton, (LPARAM)iState);
    // now show the property sheet dialog
    ACShowActivitySheet(hwnd, hInst, iIndex, (idButton-IDM_STATS));
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, idButton, (LPARAM)iState);
    return TRUE;
    }


/******************************************************************************\
*  STARTS UP THE HISTORY LOG DIALOG FOR THE SELECTED COMBOPLANT
\******************************************************************************/

BOOL TBStartGrowthLog(HWND hwnd, HINSTANCE hInst)
    {
    int iIndex;
    int iState;

    // check which plant is selected in the combobox
    iIndex = TBGetComboPlant();
    // getout if we have an invalid combo plant selection
    if(iIndex == 0) return FALSE;
    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GLOG, (LPARAM)iState);
    // and show the history dialog for this plant
    HIShowHistoryDialog(hwnd, hInst, iIndex);
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GLOG, (LPARAM)iState);
    
    return TRUE;
    }


/******************************************************************************\
*  STARTS UP THE HISTORY CHART DIALOG FOR THE SELECTED COMBOPLANT
\******************************************************************************/

BOOL TBStartGrowthChart(HWND hwnd, HINSTANCE hInst)
    {
    int iState;

    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWCHART, (LPARAM)iState);
    // and show the chart dialog for this plant
    CHShowChartDialog(hwnd, hInst);
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWCHART, (LPARAM)iState);
   
    return TRUE;
    }


/******************************************************************************\
*  STARTS UP THE OBJECTIVES DIALOG
\******************************************************************************/

BOOL TBStartObjectivesDialog(HWND hwnd, HINSTANCE hInst)
    {
    int iState;
    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_OBJECTIVES, (LPARAM)iState);
    // and show the objectives dialog
    DialogBox(hInst, "Object", hwnd, ObjectivesDlgProc);
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_OBJECTIVES, (LPARAM)iState);
   
    return TRUE;
    }

/******************************************************************************\
*  STARTS UP THE SEEDLIST PLANTING DIALOG
\******************************************************************************/

BOOL TBStartPlantingDialog(HWND hwnd, HINSTANCE hInst)
    {
    int iState;
    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_PLANTING, (LPARAM)iState);
    // and show the planting dialog
    DialogBox(hInst, "Planting", hwnd, SLSeedDlgProc);
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_PLANTING, (LPARAM)iState);
   
    return TRUE;
    }

/******************************************************************************\
*  STARTS UP THE GROWROOM EDITING DIALOG
\******************************************************************************/

BOOL TBStartEditGrowRoomDialog(HWND hwnd, HINSTANCE hInst)
    {
    int iState;
    int iReturn;
    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_EDITGROW, (LPARAM)iState);
    // the room editor dialog
    iReturn = GEStartEditGrowRoomDialog(hwnd, hInst);
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_EDITGROW, (LPARAM)iState);
    return iReturn;
    }

/******************************************************************************\
*  STARTS UP THE GROWROOM WINDOW
\******************************************************************************/

BOOL TBStartGrowRoom(HWND hwnd, HINSTANCE hInst)
    {
    HMENU hMenu = GetMenu(hwnd);
    MENUITEMINFO mii, mi2;

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask  = MIIM_TYPE|MIIM_STATE;
    mii.fType  = MFT_STRING;
    mi2.cbSize = mii.cbSize;
    mi2.fMask  = MIIM_STATE;
    if(GetMenuState(hMenu, IDM_GROWROOM, MF_BYCOMMAND) != MF_CHECKED)
         { // enter the growroom, and check the menu item
         GREnterGrowRoom(hwnd, hInst);
         TBSetGrowroomButtonState(TRUE);
         TBShowClock(TRUE);
         mii.dwTypeData = "&Exit Grow Room..\tCtrl+G";
         mii.fState     = MFS_CHECKED;
         mi2.fState     = MFS_ENABLED;
         }
    else
         { // exit the growroom, and uncheck the menu item
         TBShowClock(FALSE);
         TBSetGrowroomButtonState(FALSE);
         GRExitGrowRoom();
         mii.dwTypeData = "&Enter Grow Room..\tCtrl+G";
         mii.fState     = MFS_UNCHECKED;
         mi2.fState     = MFS_GRAYED;
         }
    mii.cch = strlen((LPCSTR)mii.dwTypeData);
    SetMenuItemInfo(hMenu, IDM_GROWROOM, 0, &mii);
    return TRUE;
    }

/******************************************************************************\
*  STARTS OR STOPS THE RASTA ROBBIE'S ALARM CLOCK TEXT
\******************************************************************************/

BOOL bRobbieRulesPlaying = FALSE;

BOOL TBStartRobbieTextWalk(HWND hwnd, LPCSTR szToonText)
    {
    int i, iState, iMenuState, iGRState;
    HWND hPlayWin = hwnd; // set play window to main window
    POINT pt={150,340};
    char  szText[150];

    // if we have a growroom window up, play in that one
    if(hGrowRoomWindow) // this global is only set if we have one
        hPlayWin = hGrowRoomWindow;

    if(bRobbieRulesPlaying)
        {
        RRStopWindowToon(hPlayWin);
        if(hGrowRoomWindow) 
             iGRState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
        else iGRState = MAKELONG(TBSTATE_ENABLED, 0);
        iState     = MAKELONG(TBSTATE_ENABLED, 0);
        iMenuState = MF_ENABLED|MF_BYPOSITION;
        SendMessage(hWndToolbar, TB_SETSTATE, IDM_ROBBIE, (LPARAM)iState);
        EnableWindow(hWndCombo, TRUE);
        EnableWindow(hWndGrowCombo, TRUE);
        bRobbieRulesPlaying = FALSE;
        }
    else // set the toolbar button state to down
        {
        iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
        SendMessage(hWndToolbar, TB_SETSTATE, IDM_ROBBIE, (LPARAM)iState);
        EnableWindow(hWndCombo, FALSE);
        EnableWindow(hWndGrowCombo, FALSE);
        if(hGrowRoomWindow) 
             iGRState = MAKELONG(TBSTATE_INDETERMINATE|TBSTATE_PRESSED, 0);
        else iGRState = MAKELONG(TBSTATE_INDETERMINATE, 0);
        iState     = MAKELONG(TBSTATE_INDETERMINATE, 0);
        iMenuState = MF_GRAYED|MF_BYPOSITION;

        // if he's asked for the the reg code file, we'll 
        // tell him the code now...
        strcpy(szText, szToonText);
        RRPlayWindowFrame(hPlayWin, pt, 1, TRUE, szText);
        bRobbieRulesPlaying = TRUE;
        }
    for(i=0;i<8;i++)
        SendMessage(hWndToolbar,TB_SETSTATE,IDM_STATS+i,   (LPARAM)iState);
    for(i=0;i<5;i++)
        EnableMenuItem(GetMenu(hwnd), i, iMenuState);
    DrawMenuBar(hwnd);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWROOM,    (LPARAM)iGRState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_EDITGROW,    (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWCHART,   (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_OBJECTIVES,  (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_PLANTING,    (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_HELPCONTENTS,(LPARAM)iState);

//    SendMessage(hWndToolbar, TB_SETSTATE, IDM_SOUND,    (LPARAM)iState);
    return TRUE;
    }


BOOL TBStartRobbieAlarmWalk(HWND hwnd, HINSTANCE hInst)
    {
    char szToonText[150];
    // ---
    // first load the text setting from the registry
    PALoadAlarmText(szToonText);
    // whistle three times
    PlaySound("ALARM", hInst, SND_RESOURCE|SND_SYNC);
    PlaySound("ALARM", hInst, SND_RESOURCE|SND_SYNC);
    PlaySound("ALARM", hInst, SND_RESOURCE|SND_SYNC);
    // if we loaded any text, then get him to say it
    if(strlen(szToonText))
        {
        TBStartRobbieTextWalk(hwnd, szToonText);
        return TRUE;
        }
    return FALSE;
    }


/******************************************************************************\
*  STARTS OR STOPS THE WEIRD ROBBIE'S 10 GOLDEN RULES TOONFILE
\******************************************************************************/


BOOL TBStartRobbieToonfile(HWND hwnd, LPCSTR szToonFile)
    {
    int i, iState, iMenuState, iGRState;
    HWND hPlayWin = hwnd; // set play window to main window

    // if we have a growroom window up, play in that one
    if(hGrowRoomWindow) // this global is only set if we have one
        hPlayWin = hGrowRoomWindow;

    if(bRobbieRulesPlaying)
        {
        RRStopWindowToon(hPlayWin);
        if(hGrowRoomWindow) 
             iGRState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
        else iGRState = MAKELONG(TBSTATE_ENABLED, 0);
        iState     = MAKELONG(TBSTATE_ENABLED, 0);
        iMenuState = MF_ENABLED|MF_BYPOSITION;
        SendMessage(hWndToolbar, TB_SETSTATE, IDM_ROBBIE, (LPARAM)iState);
        EnableWindow(hWndCombo, TRUE);
        EnableWindow(hWndGrowCombo, TRUE);
        bRobbieRulesPlaying = FALSE;
        }
    else // set the toolbar button state to down
        {
        iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
        SendMessage(hWndToolbar, TB_SETSTATE, IDM_ROBBIE, (LPARAM)iState);
        EnableWindow(hWndCombo, FALSE);
        EnableWindow(hWndGrowCombo, FALSE);
        if(hGrowRoomWindow) 
             iGRState = MAKELONG(TBSTATE_INDETERMINATE|TBSTATE_PRESSED, 0);
        else iGRState = MAKELONG(TBSTATE_INDETERMINATE, 0);
        iState     = MAKELONG(TBSTATE_INDETERMINATE, 0);
        iMenuState = MF_GRAYED|MF_BYPOSITION;

        // if he's asked for the the reg code file, we'll 
        // tell him the code now...
        if(strcmp(szToonFile, "OBEgg")==0)
            {
            POINT pt={150,340};
            char  szText[150];
//            char  szFiles[10];
//            int iFiles =0;
            // --- check if we find jpg files to convert
//            iFiles = GLConvertGrowroomImageFiles();
//            if(iFiles)
//                {
//                if(iFiles ==1)
//                    strcpy(szFiles, "file");
//                else
//                    strcpy(szFiles, "files");
//                wsprintf(szText, "Hey Dude, I've found and converted %i jpeg image %s!!", iFiles, szFiles);
//                }
//            else
            strcpy(szText, "Well Done Dude!!\nYou've found this version's Easter Egg!!");
            RRPlayWindowFrame(hPlayWin, pt, 1, TRUE, szText);
            }
        else
            RRPlayWindowToon(hPlayWin, szToonFile);

        bRobbieRulesPlaying = TRUE;
        }
    for(i=0;i<8;i++)
        SendMessage(hWndToolbar,TB_SETSTATE,IDM_STATS+i,   (LPARAM)iState);
    for(i=0;i<5;i++)
        EnableMenuItem(GetMenu(hwnd), i, iMenuState);
    DrawMenuBar(hwnd);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWROOM,    (LPARAM)iGRState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_EDITGROW,    (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWCHART,   (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_OBJECTIVES,  (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_PLANTING,    (LPARAM)iState);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_HELPCONTENTS,(LPARAM)iState);

//    SendMessage(hWndToolbar, TB_SETSTATE, IDM_SOUND,    (LPARAM)iState);
    return TRUE;
    }


/******************************************************************************\
*  STARTS OR STOPS THE WEIRD ROBBIE'S 10 GOLDEN RULES TOONFILE
\******************************************************************************/

BOOL TBStartRobbieRules(HWND hwnd)
    {
    return TBStartRobbieToonfile(hwnd, "OBRules");
    }

/******************************************************************************\
*  CHECKS WHETHER ROBBIE IS DOING RULES ANIMATION & END IT 
\******************************************************************************/

void TBCheckEndRulesPlayback(HWND hwnd)
    {
    if(bRobbieRulesPlaying) // if we're playing golden rules...
        {
        TBStartRobbieRules(hwnd); // calling the start function also ends it
        return;
        }
    }



/******************************************************************************\
*  STARTS AND STOPS THE MIDI SOUND PLAYBACK
\******************************************************************************/

BOOL bTBMusicFilePlaying = FALSE;
int  iLastFilePlayed;

void TBStopMusicFile(void)
    { 
    // stop the playback and zero our global
    if(gbMusicFileType == TYPE_MP3)
        MIStopCloseMP3Playback();
    else
        MIStopCloseMidiPlayback();
    // zero the global 
    bTBMusicFilePlaying = FALSE;
    }

BOOL TBPlayMusicFile(HWND hwnd, LPSTR lpMusicFile)
    {
    char szname[MAX_PATH];
    // ---
    // if we're currently playing a random file, stop it
    if(bTBMusicFilePlaying) TBStopMusicFile();
    // try to play a music file
    if(gbMusicFileType == TYPE_MP3)
        {
        wsprintf(szname, "MP3\\%s", lpMusicFile);
        if(MIOpenStartMP3Playback(hwnd, szname)!=0L)
        return FALSE; // if we failed, return false
        }
    else
        {
        wsprintf(szname, "MIDI\\%s", lpMusicFile);
        if(MIOpenStartMidiPlayback(hwnd, szname)!=0L)
        return FALSE; // if we failed, return false
        }

    bTBMusicFilePlaying = TRUE;
    return TRUE; // we started successfully, so return true
    }

// sprintf(strbuf, "%i", j);
// MessageBox(hwnd, strbuf, "Music File", MB_OK|MB_ICONEXCLAMATION);
   
BOOL TBPlayRandomMusicFile(HWND hwnd)
   {  
   struct _finddata_t music_file;
   long hFile;
   int  i, j, numFiles=0;
   char szext[50];
   // -----
   if(gbMusicFileType == TYPE_MP3)
       {
       numFiles = GetMP3FileCount();
       strcpy(szext, "MP3\\*.mp3");
       }
   else
       {
       numFiles = GetMidiFileCount();
       strcpy(szext, "MIDI\\*.mid");
       }
   // getout if we have no music files
   if(numFiles==0)  return FALSE;
   if(numFiles>1)
       { // never choose the same one we just played
       j=Random(1,numFiles);
       if(j==iLastFilePlayed)
           {
           while(j==iLastFilePlayed)
                  j=Random(1,numFiles);
           }
       iLastFilePlayed=j;
       }
   else j=1;
   if((hFile = _findfirst(szext, &music_file)) == -1L)
      return FALSE;
   if(j>1)
      {
      for(i=1; i<j;i+=1)
         _findnext(hFile, &music_file);
      }
   _findclose(hFile);
   return TBPlayMusicFile(hwnd, (LPSTR)music_file.name);
   }

void TBMusicPlayback(HWND hwnd, BOOL bStart)
    {
    UINT uiState;
    // first check if we're playing or not
    if(bStart)
        {
        uiState = MF_UNCHECKED;
        strcpy(strbuf, "Start &Music Playback..\tCtrl+M");
        SendMessage(hWndToolbar, TB_SETSTATE, IDM_SOUND, 
                            (LPARAM)MAKELONG(TBSTATE_ENABLED, 0));
        TBStopMusicFile();
        }
    else 
        {
        if(TBPlayRandomMusicFile(hwnd))
            {
            uiState = MF_CHECKED;
            strcpy(strbuf, "Stop &Music Playback..\tCtrl+M");
            SendMessage(hWndToolbar, TB_SETSTATE, IDM_SOUND, 
                       (LPARAM)MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0));
            }
        }
    ModifyMenu(GetMenu(hwnd),IDM_SOUND,MF_BYCOMMAND|MF_STRING,IDM_SOUND, strbuf);
    CheckMenuItem(GetMenu(hwnd),IDM_SOUND,MF_BYCOMMAND|uiState);
    }


BOOL TBPlayNextRandomMusicFile(HWND hwnd)
    { 
    TBStopMusicFile();
    return TBPlayRandomMusicFile(hwnd);
    }


void TBStartStopMusicPlayback(HWND hwnd)
    { // called by the toolbar, menu item, and/or accellerator key to
    // start or stop the random MIDI sound file playback.
    TBMusicPlayback(hwnd, bTBMusicFilePlaying);
    }


void TBGrowroomMusicPlayback(HWND hwnd)
    {
    BOOL bStart=FALSE;
    if(PACheckSoundEnabled()==TRUE)
        {
        if((bMidiPlaying==TRUE)||(bMP3Playing==TRUE)) return;
        if(SendMessage(hWndToolbar, TB_GETSTATE, IDM_GROWROOM, 0)==
                                (LPARAM)MAKELONG(TBSTATE_ENABLED, 0))
            {
            bStart = TRUE;
            }
        TBMusicPlayback(hwnd, bStart);
        }
    }


void TBToggleSoundOption(HWND hwnd, HINSTANCE hInst)
    { // sets or unsets the sound enabled option
    UINT uiState;
    // ---
    // first stop whatever music is playing
    TBMusicPlayback(hwnd, TRUE);
    // now we can show the sound dialog
    MIShowSoundEnableDialog(hwnd, hInst);
    // now set the menu item as checked or unchecked
    if(PACheckSoundEnabled())
         uiState = MF_CHECKED;
    else uiState = MF_UNCHECKED;
    CheckMenuItem(GetMenu(hwnd),IDM_SOUNDON,MF_BYCOMMAND|uiState);
    }

/******************************************************************************\
*  TOGGLES THE PROGRAM AUTO-LOAD OPTION
\******************************************************************************/

void TBToggleAutoLoadOption(HWND hwnd)
    { // sets or unsets the Auto-Load option
    BOOL bSetKey;
    UINT uiState;
    PAToggleAutoLoadOption();
    if(PACheckAutoLoad())
         {
         uiState = MF_CHECKED;
         bSetKey = TRUE;
         }
    else {
         uiState = MF_UNCHECKED;
         bSetKey = FALSE;
         }
    CheckMenuItem(GetMenu(hwnd),IDM_AUTOLOAD,MF_BYCOMMAND|uiState);
    RESetProgramAutoLoad(bSetKey);
    }

/******************************************************************************\
*  TOGGLES THE PROGRAM AUTO-GROWROOM ENTRY OPTION
\******************************************************************************/

void TBToggleAutoGrowroomOption(HWND hwnd)
    { // sets or unsets the Auto-Load option
    UINT uiState;
    PAToggleAutoGrowroomOption();
    if(PACheckAutoGrowroom())
         uiState = MF_CHECKED;
    else uiState = MF_UNCHECKED;
    CheckMenuItem(GetMenu(hwnd),IDM_AUTOGROW,MF_BYCOMMAND|uiState);
    }

/******************************************************************************\
*  TOGGLES THE VACATION MODE OPTION
\******************************************************************************/

void TBStartVacationEnabledDialog(HWND hwnd, HINSTANCE ghInst)
    { // sets or unsets the clock option
    UINT uiState;
    VAShowVacationEnableDialog(hwnd, ghInst);
    if(PACheckVacModeEnabled())
         uiState = MF_CHECKED;
    else uiState = MF_UNCHECKED;
    CheckMenuItem(GetMenu(hwnd),IDM_VACMODEON,MF_BYCOMMAND|uiState);
    }

/******************************************************************************\
*  TOGGLES THE VACATION MODE OPTION
\******************************************************************************/

void TBStartAlarmSetDialog(HWND hwnd, HINSTANCE ghInst)
    { // sets or unsets the clock option
    UINT uiState;
    VAShowAlarmSetDialog(hwnd, ghInst);
    if(PACheckAlarmSet())
         uiState = MF_CHECKED;
    else uiState = MF_UNCHECKED;
    CheckMenuItem(GetMenu(hwnd),IDM_ALARMSET,MF_BYCOMMAND|uiState);
    }
          
          

/******************************************************************************\
*  STARTS UP THE HARVEST DIALOG FOR THE SELECTED COMBOPLANT
\******************************************************************************/

BOOL TBStartHarvestDialog(HWND hwnd, HINSTANCE hInst)
    {
    int iIndex;
    int iState;

    // check which plant is selected in the combobox
    iIndex = TBGetComboPlant();
    // getout if we have an invalid combo plant selection
    if(iIndex == 0) return FALSE;
    // set the toolbar button state to down
    iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_HARVEST, (LPARAM)iState);
    // and show the chart dialog for this plant
    HAShowPlantHarvestDialog(hwnd, hInst, iIndex);
    // and set the toolbar button state to up again
    iState = MAKELONG(TBSTATE_ENABLED, 0);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_HARVEST, (LPARAM)iState);
   
    return TRUE;
    }


/******************************************************************************\
*  SETS STATE OF ALL PLANT RELATED BUTTONS (*DONE WHEN COMBO SELECTION CHANGES*)
\******************************************************************************/

void TBQueryEnableButtons(void)
    { // checks the combo plant and enables/disables plant related buttons
    BOOL bState=FALSE;
    long lState;
    int  i, iofs;
    // getout if we somehow don't have a combo
    if(hWndCombo==0 || hWndToolbar==0) return;
    // gets the currently selected item
    iofs = (TBGetComboGrowroom()-1)*3;
    for(i=0;i<3;i++)
        {
        if(PI_PlantSeed[i+iofs].szStartDate[0]!=0) 
            bState = TBSTATE_ENABLED;
        }
    // enable or disable the growroom and lock buttons
    lState = MAKELONG(bState, 0);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_GROWCHART,(LPARAM)lState);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_GLOG,     (LPARAM)lState);
    SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_STATS,    (LPARAM)lState);
    // if system can't play sounds, or we don't have MIDI files, disable button
//    if((GetMidiFileCount()==0)|(midiOutGetNumDevs()==0))
    if((GetMP3FileCount()==0)&&(GetMidiFileCount()==0))
        SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_SOUND,  (LPARAM)FALSE);
    }
                     
/******************************************************************************\
*  SETS STATE OF THE GROWROOM TOOLBAR BUTTON
\******************************************************************************/

void TBSetGrowroomButtonState(BOOL bEnabled)
    { // sets the state of the growroom toolbar button
    int iState;
    if(hWndToolbar==0) return;

    if(bEnabled==TRUE)
         iState = MAKELONG(TBSTATE_ENABLED|TBSTATE_PRESSED, 0);
    else iState = MAKELONG(TBSTATE_ENABLED, 0);

    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GROWROOM, (LPARAM)iState);
    }


/******************************************************************************\
*  MANAGES THE GROWROOM COMBOBOX
\******************************************************************************/

void TBInitGrowroomCombo(HWND hComboWnd)
	{
	int i;
    // getout if we somehow don't have a combo
    if(hComboWnd ==0) return;
    // resets combo entries to 0
    SendMessage(hComboWnd, CB_RESETCONTENT, 0, 0); 
    // Show our standard growroom names
    SendMessage(hComboWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Workshop");
    SendMessage(hComboWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Basement");
    SendMessage(hComboWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Attic");
    // fill the combo with the growroom names
    for(i=3;i<gMaxRooms;i++)
		{
        if(strlen(GR_Room[i].szRoomName))
            {
		    SendMessage(hComboWnd, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)GR_Room[i].szRoomName);
            }
		}
	// and select the first growroom
    SendMessage(hComboWnd, CB_SETCURSEL, 0, 0L);
	}

void TBResetGrowroomCombo(BOOL bNewRoomAdded)
    { // redisplays the growroom combo items after adding a new room
    int iCount, iSel = 0;
    // getout if we somehow don't have a combo
    if(hWndGrowCombo ==0) return;
    // read the number of growrooms in the combo
    iCount = SendMessage(hWndGrowCombo, CB_GETCOUNT, 0, 0L);
	if(iCount==CB_ERR)   iCount = 0;
    // read the current combo selection
    iSel   = SendMessage(hWndGrowCombo, CB_GETCURSEL, 0, 0L);
	if(iSel==CB_ERR)     iSel = iCount;
    // now fill it with the growroom names
    TBInitGrowroomCombo(hWndGrowCombo);
    // now select the new room, or the existing room
    if(bNewRoomAdded)    iSel = iCount;
    SendMessage(hWndGrowCombo, CB_SETCURSEL, iSel, 0L);
    // show the blank seeds in the plant combo
    TBUpdateGrowroomCombo();
    }
 
int TBGetComboGrowroom(void)
	{ // returns the OB1 selection in the growroom combo
	int iSel=0;
    // getout if we somehow don't have a combo
    if(hWndGrowCombo ==0) return 1;
	// read the growroom combo to see which one was selected
	iSel = SendMessage(hWndGrowCombo, CB_GETCURSEL, 0, 0L);
    // return growroom 1 if error occurred
	if(iSel==CB_ERR)      return 1;
    // return the selected growroom (OB1)
	return iSel+1;
	}

int TBGetComboGrowroomCount(void)
	{ // returns the OB1 count in the growroom combo items
	int iCount=0;
    // getout if we somehow don't have a combo
    if(hWndGrowCombo ==0) return 0;
	// read the growroom combo count
	iCount = SendMessage(hWndGrowCombo, CB_GETCOUNT, 0, 0L);
    // return growroom 1 if error occurred
	if(iCount==CB_ERR)      return 0;
    // return the growroom count (OB1)
	return iCount;
	}

BOOL TBGetComboGrowroomName(LPTSTR lpName)
	{ 
    int iSel, iReturn = 0;
    char  szText[100];
    // getout if we somehow don't have a combo
    if(hWndGrowCombo ==0) return FALSE;
	// read the growroom combo to see which one was selected
	iSel = SendMessage(hWndGrowCombo, CB_GETCURSEL, 0, 0L);
    // return growroom 1 if error occurred
	if(iSel==CB_ERR)      return FALSE;
	// read the selected combo item's text
	iReturn = SendMessage(hWndGrowCombo, CB_GETLBTEXT, iSel, (LPARAM)&szText);
	// if we failed to read the text, return a false
    if(iReturn==CB_ERR)   return FALSE;
    // now copy our results to his string
    strcpy(lpName, szText);
    // if we're still going, return the true
    return TRUE;
	}

void TBODrawComboGrowroomItem(LPDRAWITEMSTRUCT lpDIS)
    {
    int i, j;
    RECT  rc;
    HFONT hfnt = 0;
    HBRUSH hbr, hobr, hselbr = 0;
    HPEN   hpn1, hpn2, hopn  = 0;
    char  szText[100];
    HDC   hdc = lpDIS->hDC;
    // we'll copy the rect first
    CopyRect(&rc, &lpDIS->rcItem);
    // first read the selected plant's name
    SendMessage(hWndGrowCombo, CB_GETLBTEXT, lpDIS->itemID, (LPARAM)szText);
    // first fill the rect with a white brush
    FillRect(hdc, &rc, GetStockObject(WHITE_BRUSH));
    // create some objects
    hselbr = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
    hbr    = CreateSolidBrush(RGB(0,255,0));
    hpn1 = CreatePen(PS_SOLID, 1, RGB(0,128,0));
    hpn2 = CreatePen(PS_SOLID, 1, RGB(192,192,192));
    hobr = SelectObject(hdc, hbr);
    hopn = SelectObject(hdc, hpn1);
    // draw the three plants
    rc.left += 3;
    j = 3 * lpDIS->itemID;
    for(i=0;i<3;i++)
         {
         if(PI_PlantSeed[i+j].szStartDate[0] != 0)
             {
             SelectObject(hdc, hpn1);
             SelectObject(hdc, hbr);
             }
         else {
             SelectObject(hdc, hpn2);
             SelectObject(hdc, GetStockObject(WHITE_BRUSH));
             }
       
         Rectangle(hdc, rc.left, rc.top+4, rc.left+6, rc.bottom-4);
         rc.left += 8;
         }
    rc.left += 1;
    if(lpDIS->itemState & ODS_SELECTED)
        {
        SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
        SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
        FillRect(hdc, &rc, hselbr);
        }
    else
        {
        SetBkColor(hdc, RGB(255,255,255));
        SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
        }
    SelectObject(hdc, hobr);
    SelectObject(hdc, hopn);
    DeleteObject(hbr);
    DeleteObject(hpn1);
    DeleteObject(hpn2);
    DeleteObject(hselbr);

    // set the font for drawing with
    hfnt = SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
    DrawTextEx(hdc, szText, -1, &rc, DT_LEFT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER, NULL);
    SelectObject(hdc, hfnt);
    // now draw the focus and selection rectangles
//    if(lpDIS->itemState & ODS_SELECTED)
//         FrameRect(hdc, &lpDIS->rcItem, GetStockObject(BLACK_BRUSH));
//    if(lpDIS->itemState & ODS_FOCUS)
//         DrawFocusRect(hdc, &lpDIS->rcItem);
    }

BOOL TBMeasureGrowComboItem(LPMEASUREITEMSTRUCT lpMIS)
    {
    lpMIS->itemHeight = 15;
    return TRUE;
    }


/******************************************************************************\
*  MANAGES THE PLANT NAMES COMBOBOX
\******************************************************************************/

void TBODrawComboPlantItem(LPDRAWITEMSTRUCT lpDIS)
    {
    RECT  rc;
    HFONT hfnt = 0;
    HBRUSH hbr, hobr, hselbr = 0;
    HPEN   hpn1, hpn2, hopn = 0;
    int   iplant = lpDIS->itemID;
    int   iRoomOfs = (TBGetComboGrowroom()-1)*3;
    char  szText[100] = "\0";
    HDC   hdc = lpDIS->hDC;
    // we'll copy the rect first
    CopyRect(&rc, &lpDIS->rcItem);
    // first read the selected plant's name
    SendMessage(hWndCombo, CB_GETLBTEXT, iplant, (LPARAM)szText);
    // first fill the rect with a white brush
    FillRect(hdc, &rc, GetStockObject(WHITE_BRUSH));
    // draw the three plants
    rc.left += 2;
    hselbr = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
    hbr  = CreateSolidBrush(RGB(192,192,0));
    hpn1 = CreatePen(PS_SOLID, 1, RGB(128,128,0));
    hpn2 = CreatePen(PS_SOLID, 1, RGB(192,192,192));
    hobr = SelectObject(hdc, hbr);
    hopn = SelectObject(hdc, hpn1);
    if(PI_PlantSeed[iplant+iRoomOfs].szStartDate[0] != 0)
         {
         SelectObject(hdc, hpn1);
         SelectObject(hdc, hbr);
         }
    else {
         SelectObject(hdc, hpn2);
         SelectObject(hdc, GetStockObject(WHITE_BRUSH));
         }
    Rectangle(hdc, rc.left, rc.top+4, rc.left+6, rc.bottom-4);
    rc.left += 8;

    if(lpDIS->itemState & ODS_SELECTED)
        {
        SetBkColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
        SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
        FillRect(hdc, &rc, hselbr);
        }
    else
        {
        SetBkColor(hdc, RGB(255,255,255));
        SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
        }
    
    // set the font for drawing with
    hfnt = SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
    DrawTextEx(hdc, szText, -1, &rc, DT_LEFT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER, NULL);
    SelectObject(hdc, hfnt);
    SelectObject(hdc, hobr);
    SelectObject(hdc, hopn);
    DeleteObject(hbr);
    DeleteObject(hpn1);
    DeleteObject(hpn2);
    DeleteObject(hselbr);
    // now draw the focus and selection rectangles
//    if(lpDIS->itemState & ODS_SELECTED)
//         FrameRect(hdc, &lpDIS->rcItem, GetStockObject(BLACK_BRUSH));
//    if(lpDIS->itemState & ODS_FOCUS)
//         DrawFocusRect(hdc, &lpDIS->rcItem);
    }

/******************************************************************************\
*  HANDLES TOOLBAR'S TOOLTIP NOTIFICATION MESSAGES
\******************************************************************************/

BOOL TBToolbarNotify(HINSTANCE hInst, LPARAM lParam)
    { // we must do this in our parent's WM_NOTIFY message
    LPTOOLTIPTEXT lpToolTipText = (LPTOOLTIPTEXT)lParam;
    int  idTip = lpToolTipText->hdr.idFrom;
    int  iState, iIndex; // combo box current selected index
    char szBuff[40];
    char szName[100];
    char szResult[80];
    // get the currently selected growroom's name
    TBGetComboGrowroomName(szName);
    // now check who needs the text
    if(((LPNMHDR)lParam)->code != TTN_NEEDTEXT) 
        return FALSE;

    if(idTip==IDM_SOUND)
        {
        iState = TBSTATE_PRESSED|TBSTATE_ENABLED;
        if(SendMessage(hWndToolbar, TB_GETSTATE, idTip, 0)==iState)
            strcpy(szResult, "Stop Music Playback");
        else strcpy(szResult, "Start Music Playback");
        strcpy(lpToolTipText->lpszText, (LPTSTR)szResult);
        lpToolTipText->uFlags   = TTF_IDISHWND;
        return TRUE;
        }

    if(idTip==IDM_PLANTING)
		{
        // add the growroom number
		sprintf(szResult, "Planting Seeds in %s", szName);
        // set the text for this toolbutton
		strcpy(lpToolTipText->lpszText, (LPTSTR)szResult);
	    lpToolTipText->uFlags  = TTF_IDISHWND;
	    return TRUE;
        }
    
    if(idTip==IDM_EDITGROW)
		{
        // add the growroom number
		sprintf(szResult, "Edit %s", szName);
        // set the text for this toolbutton
		strcpy(lpToolTipText->lpszText, (LPTSTR)szResult);
	    lpToolTipText->uFlags  = TTF_IDISHWND;
	    return TRUE;
        }

    if(idTip>=IDM_STATS && idTip<=IDM_GLOG)
        { // we must add our plant name, 
        // first we must load the string
        LoadString(hInst, idTip, szBuff, 80);
        // now get the selected combo plant
        iIndex = TBGetComboPlant();
        sprintf(szResult, "%s %s", szBuff, PI_PlantSeed[iIndex-1].szPlantName);
        // now set the text pointer
        strcpy(lpToolTipText->lpszText, (LPTSTR)szResult);
        }
    else
        {
        if(idTip==IDM_GROWROOM)
            {
            iState = TBSTATE_PRESSED|TBSTATE_ENABLED;
            if(SendMessage(hWndToolbar, TB_GETSTATE, idTip, 0)==iState)
                 strcpy(szResult, "Exit ");
            else strcpy(szResult, "Enter ");
            // add the growroom name text
			strcat(szResult, szName);
            // set the text for this toolbutton
			strcpy(lpToolTipText->lpszText, (LPTSTR)szResult);
            }
        else
            { // Display the simple ToolTip text.
            // return ID and hInstance and let the toolbar get the tip itself
            lpToolTipText->lpszText = (LPTSTR)idTip;
            lpToolTipText->hinst    = hInst;
            }
        }
    lpToolTipText->uFlags   = TTF_IDISHWND;
    return TRUE;
    }


/******************************************************************************\
*  RETURNS THE HEIGHT OF THE TOOLBAR WINDOW
\******************************************************************************/

int TBToolbarHeight(void)
    { // we must do this in our parent's size message
    RECT rcWin;
    // getout if we somehow have no toolbar
    if(hWndToolbar == 0) return 0;
    // calculate window size
    GetWindowRect(hWndToolbar, &rcWin);
    // and return the pixel height
    return (rcWin.bottom - rcWin.top);
    }


/******************************************************************************\
*  SIZES THE TOOLBAR WINDOW
\******************************************************************************/

void TBSizeToolbar(HWND hWnd)
    { // we must do this in our parent's size message
    SendMessage(GetDlgItem(hWnd, ID_TOOLBAR), TB_AUTOSIZE, 0L, 0L);
    }


/******************************************************************************\
*  UPDATES THE PLANT NAMES COMBOBOX (*DONE AFTER PLANTSEED[] FILLED*)
\******************************************************************************/

void TBUpdateGrowroomCombo(void)
    { // Resets the combo and adds plant names to the combo box    
    int  i, iRoom;
    int  ifirst = 0;
    char szBuff[60]  = "\0";
    char szFound[60] = "\0";
    BOOL bPlanted = FALSE;
    // getout if we somehow don't have a plant combo
    if(hWndCombo ==0) return;
    // get the growroom number from the growroom combo
    iRoom = TBGetComboGrowroom() - 1;
    // resets combo entries to 0
    SendMessage(hWndCombo, CB_RESETCONTENT, 0, 0); 
    for(i=0;i<gMaxPlants;i++)
        { // if this plant is in the selected room
        if((i/3) == iRoom)
            {
            // if he's planted this seed...
            if(PI_PlantSeed[i].szStartDate[0] != 0)
                { // ...we'll display it's name
                sprintf(szBuff, "%s", PI_PlantSeed[i].szPlantName);
                bPlanted = TRUE;
                // if we haven't found a plant yet, set the one we have
                if(ifirst==0)
                    {
                    strcpy(szFound, PI_PlantSeed[i].szPlantName);
                    ifirst = 1;
                    }
                }
            else
                strcpy(szBuff, "");
            
            SendMessage(hWndCombo, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)szBuff);
            }
        }
    // select the first plant
    if(strlen(szFound))
        {
        ifirst = SendMessage(hWndCombo, CB_FINDSTRING, -1, (LPARAM)szFound); 
        // set to zero if we somehow couldn't find it
        if(ifirst == CB_ERR) ifirst = 0;
        }
    // now select our first item
    SendMessage(hWndCombo, CB_SETCURSEL, ifirst, 0L); 
    // it never hurts to enable or disable tool buttons afterwards
    TBQueryEnableButtons();
    InvalidateRect(hWndGrowCombo, NULL, FALSE);
    // and finally set the focus
//    SetFocus(hWndCombo);
    }

/******************************************************************************\
*  UPDATES THE ACTIVITIES TOOL BUTTON & MENU IF THE PLANT NAME COMBO CHANGES
\******************************************************************************/

void TBUpdatePlantCombo(void)
    { // Resets the combo and adds plant names to the combo box    
    int ipot;
    int iroomofs = (TBGetComboGrowroom()-1)*3;
    int istate   = MAKELONG(TBSTATE_ENABLED, 0);
    // getout if we somehow don't have a plant combo
    if(hWndCombo ==0) return;
    ipot = SendMessage(hWndCombo, CB_GETCURSEL, 0, 0); 
    if(PI_PlantSeed[iroomofs+ipot].szStartDate[0] == 0)
        istate = 0;

    SendMessage(hWndToolbar, TB_SETSTATE, IDM_STATS, (LPARAM)istate);
    SendMessage(hWndToolbar, TB_SETSTATE, IDM_GLOG,  (LPARAM)istate);
    }

/******************************************************************************\
*  SETS THE GIVEN PLANT IN THE COMBO
\******************************************************************************/

void TBSetComboPlant(int iPlant)
    { // we set the selected plant in the combo and check the buttons
    // ** iPlant is OB0 **
    if(hWndCombo==0) return ;
    // first we set the combo
    SendMessage(hWndCombo, CB_SELECTSTRING, 0, 
                 (LPARAM)(LPCSTR)PI_PlantSeed[iPlant].szPlantName);
    // and now we enable the buttons if they were disabled
    TBQueryEnableButtons();
    }



/******************************************************************************\
*  SETS UP THE TBBUTTON STRUCT FOR ALL OUR BUTTONS
\******************************************************************************/

// Our new button order is as follows:
//      SEPARATOR
//  IDM_OBJECTIVES 
//  IDM_PLANTING
//  IDM_GROWROOM
//  IDM_GROWCHART
//  IDM_ROBBIE
//  IDM_SOUND
//      41? SEPARATORS = ID_COMBO
//  IDM_STATS
//  IDM_HARVEST
//  IDM_GLOG
//      SEPARATOR
//  IDM_HELPCONTENTS

void TBInitTBButtons(TBBUTTON* tbButtons)
    {
    int i, j, k;
    // first init all buttons to enabled separators
    for(i=0;i<NUM_TOOLS;i++)
        {
        tbButtons[i].iBitmap    = 0;
        tbButtons[i].idCommand  = 0;
        tbButtons[i].fsState    = TBSTATE_ENABLED;
        tbButtons[i].fsStyle    = TBSTYLE_SEP;
        tbButtons[i].dwData     = 0L;
        tbButtons[i].iString    = 0;
        }
    k = 0; // the next bitmap id

    tbButtons[1].fsStyle    = TBSTYLE_BUTTON;
	tbButtons[1].idCommand  = IDM_OBJECTIVES;
    tbButtons[1].iBitmap    = k;
    k += 1;

    tbButtons[2].fsStyle    = TBSTYLE_BUTTON;
	tbButtons[2].idCommand  = IDM_ROBBIE;
    tbButtons[2].iBitmap    = k;
    k += 1;

    tbButtons[4].fsStyle    = TBSTYLE_BUTTON;
	tbButtons[4].idCommand  = IDM_PLANTING;
    tbButtons[4].iBitmap    = k;
    k += 1;

    tbButtons[22].fsStyle    = TBSTYLE_BUTTON;
    tbButtons[22].idCommand  = IDM_GROWROOM;
    tbButtons[22].iBitmap    = k;
    k += 1;

    tbButtons[23].fsStyle    = TBSTYLE_BUTTON;
	tbButtons[23].idCommand  = IDM_EDITGROW;
    tbButtons[23].iBitmap    = k;
    k += 1;

    // now we setup the next 6 activity buttons 
    j = 3+NUM_COMBOSEPS;
    for(i=j;i<3+j;i++)
        { // first set bitmap offset and style
        tbButtons[i].iBitmap    = k;
        tbButtons[i].fsStyle    = TBSTYLE_BUTTON;
        k += 1;
        }
        // and now their command IDs
        tbButtons[j+0].idCommand = IDM_STATS;
        tbButtons[j+1].idCommand = IDM_GLOG;
        tbButtons[j+2].idCommand = IDM_GROWCHART;

    // finally setup the last help button
    j = 8+NUM_TOOLSEPS-1;
    tbButtons[j].iBitmap    = k;
    tbButtons[j].fsStyle    = TBSTYLE_BUTTON;
    tbButtons[j].idCommand  = IDM_HELPCONTENTS;
    }

/******************************************************************************\
*  HANDLES THE TOOLBAR COMBO'S TOOLTIP DISPLAY
\******************************************************************************/

// This function relays the mouse messages from the combo box
// to get the ToolTips to work.

LRESULT CALLBACK ComboWndProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
    {
    switch (uMessage)
        {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            {
            MSG msg;
            HWND hWndTTips;
            msg.lParam = lParam;
            msg.wParam = wParam;
            msg.message = uMessage;
            msg.hwnd = hWnd;
            hWndTTips = (HWND)SendMessage(hWndToolbar, TB_GETTOOLTIPS, 0,0);
            SendMessage(hWndTTips, TTM_RELAYEVENT, 0, (LPARAM)(LPMSG)&msg);
            break;
            }
        }
    return (CallWindowProc(lpfnDefCombo, hWnd, uMessage, wParam, lParam));
    }

/******************************************************************************\
*  CREATES THE TOOLBAR WINDOW AND THE PLANT NAMES COMBO BOX
\******************************************************************************/

BOOL TBCreateToolbar(HWND hWnd, HINSTANCE hInst)
    { // we must do this in our parent's WM_CREATE message
    TBBUTTON tbButtons[NUM_TOOLS];
    TOOLINFO lpToolInfo;
    HWND     hWndTTips;
    UINT     uiFlag;
    // first check that the common controls dll is loaded
    InitCommonControls();
    // now prepare our TBBUTTON struct for all our buttons
    TBInitTBButtons((TBBUTTON*)&tbButtons);

    // Create the toolbar control.
    if(VICoolBarsEnabled())
         uiFlag = TBSTYLE_FLAT;
    else uiFlag = WS_BORDER;

    hWndToolbar = CreateToolbarEx(hWnd, WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS|uiFlag,
                                  ID_TOOLBAR, NUM_TOOLBUTTONS, hInst, IDB_TOOLBMP, 
                                  (LPCTBBUTTON)&tbButtons, NUM_TOOLS, 
                                  16,16,16,16, sizeof(TBBUTTON));
    // if we didn't create it, getout now
    if(!hWndToolbar) return FALSE;

    // Create the plant name combo box and add it to the toolbar.
    hWndCombo = CreateWindowEx(0L, "COMBOBOX", NULL,
                               WS_CHILD|WS_BORDER|WS_VISIBLE|CBS_HASSTRINGS|
                               CBS_OWNERDRAWFIXED|CBS_DROPDOWNLIST,
                               275, 1, 94, 200, hWndToolbar,
                               (HMENU)IDC_TBCOMBO, hInst, NULL);
    if(hWndCombo)
        SendMessage(hWndCombo, WM_SETFONT, 
                    (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));

    // Create the growroom combo box and add it to the toolbar.
    hWndGrowCombo = CreateWindowEx(0L, "COMBOBOX", NULL,
                               WS_CHILD|WS_BORDER|WS_VISIBLE|WS_VSCROLL|CBS_HASSTRINGS|
                               CBS_OWNERDRAWVARIABLE|CBS_DROPDOWNLIST,
                               85, 1, 134, 420, hWndToolbar,
                               (HMENU)IDC_TBGRCOMBO, hInst, NULL);
	// if we created the growroom combo, we'll fill it now
	if(hWndGrowCombo)  
        {
        TBInitGrowroomCombo(hWndGrowCombo);
        // set the combobox font
        SendMessage(hWndGrowCombo, WM_SETFONT, 
                    (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
        }

    // if we didn't create it, kill the toolbar again and getout
    if(!hWndCombo) 
        {
        DestroyWindow(hWndToolbar);
        return FALSE;
        }

    // Set the window procedure for the combo box.
    lpfnDefCombo = (WNDPROC)GetWindowLong(hWndCombo, GWL_WNDPROC);
    SetWindowLong(hWndGrowCombo, GWL_WNDPROC, (LONG)ComboWndProc);
    SetWindowLong(hWndCombo,     GWL_WNDPROC, (LONG)ComboWndProc);

    // Create the progress bar and add it to the toolbar.
    hWndProgress = CreateWindowEx(0L, PROGRESS_CLASS, NULL, WS_CHILD,
                                  495, 14, 132, 9, hWndToolbar,
                                  (HMENU)ID_PROGRESS, hInst, NULL);

    // Create the progress static and add it to the toolbar.
    hWndStatic = CreateWindowEx(0L, "static", "Calculating..", WS_CHILD,
                                  495, 0, 132, 14, hWndToolbar,
                                  (HMENU)ID_TSTATIC, hInst, NULL);

    // Create the clock static and add it to the toolbar.
    hWndClock  = CreateWindowEx(WS_EX_WINDOWEDGE, "static", 
                                  NULL, WS_CHILD,
                                  495, 0, 132, 20, hWndToolbar,
                                  (HMENU)ID_TCLOCK, hInst, NULL);

    if(hWndStatic) // if we have a static control, set it's font
        SendMessage(hWndStatic, WM_SETFONT,
                   (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));
    if(hWndClock) // if we have a clock static control, create and set it's font
        {
        hClockFont = CreateFont(24,0,0,0,FW_BOLD,0,0,0,0,0,0,0,0,"MS Sans Serif");
        SendMessage(hWndClock, WM_SETFONT, (WPARAM)hClockFont, MAKELPARAM(TRUE, 0));
        }
    // Get the handle to the ToolTip window.
    hWndTTips = (HWND)SendMessage(hWndToolbar, TB_GETTOOLTIPS, 0, 0);
    if(hWndTTips)
       { // Fill out the TOOLINFO structure.
       lpToolInfo.cbSize   = sizeof(lpToolInfo);
       // The uID is the handle of the tool (the combo box).
       lpToolInfo.uFlags   = TTF_IDISHWND;
       // the string ID in the resource
       lpToolInfo.lpszText = (LPSTR)IDC_TBCOMBO;
       // the window that gets the ToolTip messages
       lpToolInfo.hwnd     = hWnd;
       // the tool
       lpToolInfo.uId      = (UINT)hWndCombo;
       // the instance that owns the string resource
       lpToolInfo.hinst    = hInst;
       // Set up the ToolTips for the combo box.
       SendMessage(hWndTTips, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&lpToolInfo);
       // the string ID in the resource
       lpToolInfo.lpszText = (LPSTR)IDC_TBGRCOMBO;
       // the tool
       lpToolInfo.uId      = (UINT)hWndGrowCombo;
       // Set up the ToolTips for the GROWROOM combo box.
       SendMessage(hWndTTips, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&lpToolInfo);
       }
    // set the focus to the growroom combobox
    SetFocus(hWndGrowCombo);
    // All's ok, return TRUE
    return TRUE;
    }


/******************************************************************************\
*  CREATES THE REBAR WINDOW ATTACHES THE TOOLBAR
\******************************************************************************/

BOOL TBCreateCoolbar(HWND hwndParent, HINSTANCE g_hInst)
    { 
    HWND hwndRebar = NULL; 
    INITCOMMONCONTROLSEX iccex; 

    iccex.dwSize = sizeof(INITCOMMONCONTROLSEX); 
    iccex.dwICC = ICC_COOL_CLASSES; 
    InitCommonControlsEx(&iccex);

    //add a band that contains a combobox 
    if(!TBCreateToolbar(hwndParent, g_hInst))   return FALSE;
    // do nothing if we didn't initialize our common control
    if(!VICoolBarsEnabled()) return FALSE;
    
    hwndRebar = CreateWindowEx(WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL, 
                               WS_VISIBLE | WS_BORDER |  WS_CHILD |  
                               WS_CLIPCHILDREN |  WS_CLIPSIBLINGS |  
                               RBS_VARHEIGHT, 0, 0, 0, 0,  hwndParent, 
                               (HMENU)ID_COOLBAR, g_hInst, NULL); 
 
    if(hwndRebar) 
       { 
       REBARINFO      rbi; 
       REBARBANDINFO  rbbi; 
       RECT           rc; 
 
       rbi.cbSize  = sizeof(rbi); 
       rbi.fMask   = RBIM_IMAGELIST; 
       SendMessage(hwndRebar, RB_SETBARINFO, 0, (LPARAM)&rbi); 
                 
       GetWindowRect(hWndToolbar, &rc); 
   
       ZeroMemory(&rbbi, sizeof(rbbi)); 
       rbbi.cbSize       = sizeof(REBARBANDINFO); 
       rbbi.fMask        = RBBIM_SIZE | RBBIM_CHILD |  RBBIM_CHILDSIZE |  
                           RBBIM_ID   | RBBIM_STYLE |  RBBIM_TEXT;
       rbbi.cxMinChild   = rc.right - rc.left; 
       rbbi.cyMinChild   = rc.bottom - rc.top; 
       rbbi.cx           = 0; 
       rbbi.fStyle       = RBBS_FIXEDBMP| RBBS_FIXEDSIZE;
       rbbi.wID          = ID_COOLBAR; 
       rbbi.hwndChild    = hWndToolbar; 
       rbbi.lpText       = NULL;
       rbbi.cch          = 0; 
 
       SendMessage(hwndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)(LPREBARBANDINFO)&rbbi); 
       return TRUE; 
       }
    return FALSE;
    } 
 

/******************************************************************************\
*  SIZES THE TOOLBAR AND THE REBAR WINDOW
\******************************************************************************/

void TBSizeCoolbar(HWND hWnd, LPARAM lParam)
    { // we must do this in our parent's size message
    TBSizeToolbar(hWnd);
    // now if we also have a coolbar window, we must size it too
    if(hwndRebar)
        MoveWindow(hwndRebar, 0,0, LOWORD(lParam), 25, TRUE);
    }


/******************************************************************************\
*  DESTROYS THE TOOLBAR WINDOW AND THE PLANT NAMES COMBO BOX
\******************************************************************************/

void TBDestroyToolbar(HWND hWnd)
    { // we must do this in our parent's WM_CLOSE message
    DestroyWindow(GetDlgItem(hWnd, ID_TOOLBAR));
    DestroyWindow(GetDlgItem(hWnd, ID_TSTATIC));
    DestroyWindow(GetDlgItem(hWnd, ID_TCLOCK));
    DestroyWindow(GetDlgItem(hWnd, ID_PROGRESS));
    DestroyWindow(GetDlgItem(hWnd, IDC_TBCOMBO));
    DestroyWindow(GetDlgItem(hWnd, IDC_TBGRCOMBO));
    DestroyWindow(GetDlgItem(hWnd, ID_COOLBAR));
    // destroy our toolbar font too
    if(hClockFont) DeleteObject(hClockFont);
    }


