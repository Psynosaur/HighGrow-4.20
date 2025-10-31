/******************************************************************************\
*
*  PROGRAM:     SeedList.c
*
*  PURPOSE:     Choosing and Planting the Seeds
*
\******************************************************************************/

#include <windows.h>
#include "stdio.h"
#include "resource.h"
#include "global.h"
#include "toolbar.h"   // to enable toolbar buttons if they were disabled
#include "highgrow.h"  // for our recalculate when he plants a seed
#include "plantmem.h"  // for creating a new plant file
#include "dibitmap.h"  // for handling 256 color bitmaps
#include "growroom.h"  // for rendering our growroom after planting
#include "harvest.h"   // for showing our unavailable dialog
#include "seedlist.h"
#include "plantseed.h"
#include "seedimp.h"
#include "seedlog.h"
       
HFONT      hSeedNameFont;
HFONT      hSeedTextFont;
HBITMAP    hSeedBitmap;
HPALETTE   hSeedPalette;
int        giSeedsPlanted;
int        iLogSeeds;

/******************************************************************************\
*  CHECKING WHICH PLANTS ARE GROWING, & INIT THE "PI_PlantSeed" PLANTINFO ARRAY
\******************************************************************************/

void SLInitPlantSeed(PPLANT plPlant, int iSeed)
    { // * fills our PI_PlantSeed struct array
    int i;

    strcpy(PI_PlantSeed[iSeed].szPlantName, plPlant->PI_Plant.szPlantName);
    for(i=0 ;i<3; i++)
       PI_PlantSeed[iSeed].szStartDate[i] = plPlant->PI_Plant.szStartDate[i];
    
    PI_PlantSeed[iSeed].cSeedChoice    = plPlant->PI_Plant.cSeedChoice;
    PI_PlantSeed[iSeed].bRegistered    = plPlant->PI_Plant.bRegistered;
    PI_PlantSeed[iSeed].dwFileVersion  = plPlant->PI_Plant.dwFileVersion;
    PI_PlantSeed[iSeed].cGrowthStage   = plPlant->PI_Plant.cGrowthStage;
    }


void SLUpdatePlantSeedGrowthStage(PPLANT plPlant, int iSeed)
    {
    // reset the growthstage for this plant after calculating
    PI_PlantSeed[iSeed].cGrowthStage   = plPlant->PI_Plant.cGrowthStage;
    }

/******************************************************************************\
*  FUNCTIONS TO: Checking if a seed in the list has been planted yet
\******************************************************************************/

int SLIsSeedPlanted(int iIndex)
    { // if seed is planted, returns the ob1 seed number for the ob0 listbox index
    // returns zero if seed is not planted
    int i;
    for(i=0;i<gMaxPlants;i++)
        { 
        if(PI_PlantSeed[i].cSeedChoice-1 == iIndex)
            return i+1;
        }
    return FALSE;
    }

int SLIsGrowroomFull(HWND hwnd)
    { // checking if the given growroom has all three pots planted
    int i = 0;
	int j = 3*(max(0, SendDlgItemMessage(hwnd, IDC_GRCOMBO, CB_GETCURSEL, 0, 0L)));
    BOOL bFull   = TRUE;
    // check each pot in this growroom
    for(i=0;i<3;i++)
        {
        if(PI_PlantSeed[i+j].szStartDate[0] == 0)
            bFull = FALSE;
        }
    return bFull;
    }

/******************************************************************************\
*  FUNCTIONS TO: Changing the text when he changes the seedlist
\******************************************************************************/

int SLGetSuggestedPotNumber(HWND hDlg)
    { // returns the OB0 next suggested pot number
    int i;
	int j = 3*(max(0, SendDlgItemMessage(hDlg, IDC_GRCOMBO, CB_GETCURSEL, 0, 0L)));
    // check plantseed struct for a non-zero pot choice
    for(i=0;i<3;i++)
        {
        if(PI_PlantSeed[i+j].cSeedChoice==0)  
            return i;
        }
    // if we didn't have a free pot, return zero
    return 0;
    }

void SLUpdateButtonStates(HWND hdlg, int iSeed)
    {
    BOOL bFull;
    BOOL bState;
    int iIndex = 0;

    // first check if this growroom is full of plants
    bFull = SLIsGrowroomFull(hdlg);
    // now check if this seed has been planted yet
    iSeed = SLIsSeedPlanted(iSeed);
    // if we've planted this seed, we show it's name
    if(iSeed>0)
        { 
        strcpy(strbuf, PI_PlantSeed[iSeed-1].szPlantName);
        iIndex = iSeed - ((int)((iSeed-1)/3)*3) - 1;
        SendDlgItemMessage(hdlg, IDC_POTCOMBO, CB_SETCURSEL, iIndex, 0L);
        bState = FALSE;
        }
    else  // we haven't planted this seed yet
        { // set it's name to null
        strbuf[0] = 0;
        // if he's already planted all his seeds
        // or if this growroom is entirely filled
        if((giSeedsPlanted==gMaxPlants)||(bFull==TRUE)) 
             bState = FALSE; // we'll disable all input controls
        else 
            {
            iIndex = SLGetSuggestedPotNumber(hdlg);
            SendDlgItemMessage(hdlg, IDC_POTCOMBO, CB_SETCURSEL, iIndex, 0L);
            bState = TRUE;  // else we'll enable them
            }
        }
    // show or blank the plant name and en/disable the planting button
    SendDlgItemMessage(hdlg, IDC_ED01, WM_SETTEXT, 0, (LPARAM)strbuf);
    EnableWindow(GetDlgItem(hdlg, IDC_ED01), bState);
    EnableWindow(GetDlgItem(hdlg, IDC_BT01), bState);
    EnableWindow(GetDlgItem(hdlg, IDC_POTCOMBO), bState);
    }


void SLSeedListBoxChange(HWND hdlg, HINSTANCE hInst)
    {
    int  iIndex;
    char szType[40]="\0";
    char szOrig[200]="\0";
    char szDesc[250]="\0";
    char szHint[200]="\0";
    // if he selects another seed from the seed listbox...
    // first check which seed is currently selected
    iIndex= SendDlgItemMessage(hdlg, IDC_LB01, LB_GETCURSEL, 0,0);
    // load and display the NAME text for this seed
    if(iIndex<30)
        {
        LoadString(hInst, (iIndex+IDS_BT01),   szType, 40);
        // load and display the ORIGIN text for this seed
        LoadString(hInst, (iIndex+IDS_SIO_01), szOrig, 200);
        // load and display the GENERAL text for this seed
        LoadString(hInst, (iIndex+IDS_SIG_01), szDesc, 250);
        // load and display the TIP text for this seed
        LoadString(hInst, (iIndex+IDS_SIT_01), szHint, 200);
        }
    else
        { // get the details from the logfile
        SGGetHybridLogDetails(iIndex-30, (char*)&szType, (char*)&szOrig,
                                         (char*)&szDesc, (char*)&szHint);
        }
    // now set the text into the static controls
    SendDlgItemMessage(hdlg, IDC_ST01, WM_SETTEXT, 0, (LPARAM)szType);
    SendDlgItemMessage(hdlg, IDC_ST03, WM_SETTEXT, 0, (LPARAM)szOrig);
    SendDlgItemMessage(hdlg, IDC_ST05, WM_SETTEXT, 0, (LPARAM)szDesc);
    SendDlgItemMessage(hdlg, IDC_ST07, WM_SETTEXT, 0, (LPARAM)szHint);
    // now update the enabled state of the child controls
    SLUpdateButtonStates(hdlg, iIndex);
    }



/******************************************************************************\
*  FUNCTIONS TO: Plant a new seed by clicking the button
\******************************************************************************/

int SLGetNextPlantNumber(HWND hDlg)
    { // returns the OB1 plant number for the next growfile
    int i = max(0, SendDlgItemMessage(hDlg, IDC_POTCOMBO, CB_GETCURSEL, 0, 0L));
	int j = 3*(max(0, SendDlgItemMessage(hDlg, IDC_GRCOMBO, CB_GETCURSEL, 0, 0L)));
    // check plantseed struct for a non-zero seed choice
    if(PI_PlantSeed[i+j].cSeedChoice==0)  return i+j+1;
    // if we didn't have a free pot, return zero
    return 0;
    }

void SLPlantSelectedSeed(HWND hDlg, HINSTANCE hInst)
    {
    int iIndex, iPlant;
    // first check if he's entered a name
    if(!SendDlgItemMessage(hDlg, IDC_ED01, WM_GETTEXT, 40, (LPARAM)&strbuf))
        { // tell him to enter a name first, and then getout
        char szError[100];
        LoadString(hInst, IDSERR_NAME_SEED, szError, 100);
        MessageBox(hDlg, szError, "HighGrow Message", MB_OK);
        // set the focus back to the edit control to help him
        SetFocus(GetDlgItem(hDlg, IDC_ED01));
        return;
        }
    // check the ID number of this new plant
    iPlant = SLGetNextPlantNumber(hDlg);
    if(iPlant==0)
        { // tell him to enter a name first, and then getout
        MessageBox(hDlg, "You already have a seed planted in this pot.", 
                         "HighGrow Message", MB_OK);
        // set the focus back to the edit control to help him
        SetFocus(GetDlgItem(hDlg, IDC_ED01));
        return;
        }
    // get the current listbox seed and set our 
    iIndex = SendDlgItemMessage(hDlg, IDC_LB01, LB_GETCURSEL, 0,0);
    PI_PlantSeed[iPlant-1].cSeedChoice=iIndex+1;
    // he's entered a name, so we'll record this and today's date
    strcpy(PI_PlantSeed[iPlant-1].szPlantName, strbuf);
    GLDateNow((char*)&PI_PlantSeed[iPlant-1].szStartDate);
    // now update the enabled state of the child controls
    SLUpdateButtonStates(hDlg, iIndex);
    // here we redraw the growroom combo to show the change
    InvalidateRect(GetDlgItem(hDlg, IDC_GRCOMBO), NULL, FALSE);
    // also set the focus back to the listbox
    SetFocus(GetDlgItem(hDlg, IDC_LB01));
    // now we create a new plant file and set default values
    PMNewPlantFile(&PI_PlantSeed[iPlant-1], iPlant-1);
    // enable our Main window's menu and the toolbar buttons
    GLInitGrowMenus(GetParent(hDlg));
    TBUpdateGrowroomCombo();
    // now recalculate all the plant's (including this one)
    HGCalculatePlants(GetParent(hDlg), hInst, TRUE);
    GRInitGrowRoomDrawPlants();
    GRReRenderGrowRoom();
    // increase this to indicate we've planted another
    giSeedsPlanted += 1;
    }

/******************************************************************************\
*  FUNCTIONS TO: Initialize the dialog before showing it
\******************************************************************************/

void SLFillSeedListBox(HWND hDlg)
    {
    int i, iLogSeeds=0;
    char szNumSeeds[50]="\0";

    if(gbRegistered) iLogSeeds = SGCountSeedLogEntries();
    SendDlgItemMessage(hDlg, IDC_LB01, LB_RESETCONTENT, 0, 0L);
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCOUNT, 30+iLogSeeds, 0L);
    
    for(i=0; i<30+iLogSeeds; i++)
        SendDlgItemMessage(hDlg, IDC_LB01, LB_ADDSTRING, 0, 0L);
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCURSEL, 0, 0L);
    // now change the listbox's groupbox text
    wsprintf(szNumSeeds, "%i Seeds", 30+iLogSeeds);
    SendDlgItemMessage(hDlg, IDC_ST09, WM_SETTEXT, 0, (LPARAM)szNumSeeds);
    }


void SLInitSeedDialog(HWND hDlg, HINSTANCE hInst)
    {
    int i;
    long lp=MAKELPARAM(TRUE, 0);
    // first position that damn thing
    DlgCenter(hDlg);
    // create our special dialog fonts
    hSeedNameFont = GLCreateDialogFont(24,0,FW_BOLD);
    hSeedTextFont = GLCreateDialogFont(18,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hSeedNameFont) 
        SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hSeedNameFont,lp);
    // if we created the seed text font, set it to all the heading controls
    if(hSeedTextFont) 
        {
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST04,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST06,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST08,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST09,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST10,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST11,WM_SETFONT,(WPARAM)hSeedTextFont,lp);
        }
    // load our seed imagelist (MUST USE THIS METHOD FOR 256 COLOUR BITMAPS)
    hSeedBitmap = DILoadResourceBitmap(hInst, "SeedList", &hSeedPalette);
    // now fill the list box with dummy strings for our seeds
    SLFillSeedListBox(hDlg);
    // now fill the grow room combobox with room names
    TBInitGrowroomCombo(GetDlgItem(hDlg, IDC_GRCOMBO));
    SendDlgItemMessage(hDlg, IDC_GRCOMBO, CB_SETCURSEL, TBGetComboGrowroom()-1, 0L);
    // now fill the plant pot combobox items
    SendDlgItemMessage(hDlg, IDC_POTCOMBO, CB_INSERTSTRING, -1, (LPARAM)"Left");
    SendDlgItemMessage(hDlg, IDC_POTCOMBO, CB_INSERTSTRING, -1, (LPARAM)"Center");
    SendDlgItemMessage(hDlg, IDC_POTCOMBO, CB_INSERTSTRING, -1, (LPARAM)"Right");
    SendDlgItemMessage(hDlg, IDC_POTCOMBO, CB_SETCURSEL, 0, 0L);
    // limit the amount typable into the plant name edit control
    SendDlgItemMessage(hDlg, IDC_ED01, EM_LIMITTEXT, 39, 0);
    // here we count how many seeds he's already planted
    giSeedsPlanted = 0; // first set this to zero
    for(i=0;i<gMaxPlants;i++)
        { // check plantseed struct for a non-zero seed choice
        if(PI_PlantSeed[i].cSeedChoice!=0) 
           giSeedsPlanted += 1; 
        }
    // now we can show this seed's info in the dialog
    SLSeedListBoxChange(hDlg, hInst);
    }


/******************************************************************************\
*  FUNCTIONS TO: Winproc for our Seed List dialog
\******************************************************************************/

BOOL CALLBACK SLSeedDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   LPDRAWITEMSTRUCT    lpDIS;
   LPMEASUREITEMSTRUCT lpMIS;
   
    switch (message)
      {
      case WM_INITDIALOG:
           SLInitSeedDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
            if(hSeedNameFont) DeleteObject(hSeedNameFont);
            if(hSeedTextFont) DeleteObject(hSeedTextFont);
            DIFreeBitmap(hSeedBitmap, hSeedPalette);
      return (FALSE);  // only done because we're redirecting the focus

      case WM_ERASEBKGND:
          if(-1==GetDeviceCaps((HDC)wParam, NUMCOLORS))
              { // if we're in high colour, we'll draw the light leaf background
              DITileDlgLeafBitmap(hDlg, (HDC)wParam);
              return (TRUE);
              }
      return (FALSE);

      case WM_CTLCOLORSTATIC:
          {
          int i = GetDlgCtrlID((HWND)lParam);
          if((i==IDC_ST02)||(i==IDC_ST04)||(i==IDC_ST06)||
             (i==IDC_ST08)||(i==IDC_ST10)||(i==IDC_ST11)||(i==IDC_STATIC))
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_DRAWITEM:
          {
          lpDIS = (LPDRAWITEMSTRUCT)lParam;

          if(wParam==IDC_GRCOMBO)
              {
              TBODrawComboGrowroomItem(lpDIS);
              return FALSE;
              }

          FillRect(lpDIS->hDC, &lpDIS->rcItem, GetStockObject(WHITE_BRUSH));

          DIDrawImage(lpDIS->hDC, min(lpDIS->itemID, 30),
                        lpDIS->rcItem.left+10,lpDIS->rcItem.top+5, 
                        hSeedBitmap, hSeedPalette);
          InflateRect(&lpDIS->rcItem, -2, -2);
          if(lpDIS->itemState & ODS_SELECTED)
              FrameRect(lpDIS->hDC, &lpDIS->rcItem, GetStockObject(BLACK_BRUSH));
          if(lpDIS->itemState & ODS_FOCUS)
              DrawFocusRect(lpDIS->hDC, &lpDIS->rcItem);
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_MEASUREITEM:
           lpMIS = (LPMEASUREITEMSTRUCT)lParam;
           if(wParam==IDC_LB01)
               {
               lpMIS->itemHeight = 60;
               return(TRUE);
               }
           else
               {
               if(wParam==IDC_GRCOMBO)
                    return lpMIS->itemHeight = 15;
               }

      return (FALSE);  // only done because we're redirecting the focus

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_LB01:
               if(HIWORD(wParam)==LBN_SELCHANGE)
                 SLSeedListBoxChange(hDlg, dhInst);
            break;

            case IDC_BT01:
                 SLPlantSelectedSeed(hDlg, dhInst);
            break;

            case IDC_BT02:
                 if(gbPaid)
                     { // if this version if paid for, we'll allow importing
                     if(SIImportSeedFile(hDlg, dhInst))
                        SLFillSeedListBox(hDlg);
                     }
                 else
                     HAUnavailableDialog(hDlg, dhInst);
            break;

            case IDC_BT03:
                 if(gbPaid)
                     { // if this version if paid for, we'll allow importing
                     int i=SendDlgItemMessage(hDlg,IDC_LB01,LB_GETCURSEL,0,0L);
                     SIExportSeedFile(hDlg, dhInst, i);
                     }
                 else
                     HAUnavailableDialog(hDlg, dhInst);
            break;

            case IDCANCEL: EndDialog(hDlg,FALSE); 
            return (TRUE);
            }  
      return (FALSE);

      }
   return (FALSE);
   }                          

