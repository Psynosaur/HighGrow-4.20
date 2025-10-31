#include <windows.h>
#include <string.h>
#include "resource.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "Global.h"
#include "DIBitmap.h"
#include "toolbar.h"
#include "password.h"
#include "growedit.h"
  
/****************************************************************************\
* VACATION MODE DIALOG
\****************************************************************************/
   
HFONT    hVacBigFont;
HFONT    hVacMedFont;
int      iVacRoom; // OB0 room number for this setting

/****************************************************************************\
* MANAGING THE LIGHTING CONTROLS
\****************************************************************************/

void VAQueryEnableLighting(HWND hDlg)
    {
    LONG result;
    BOOL bEnable = FALSE;
    // check the state of our checkbox
    result = SendDlgItemMessage(hDlg,IDC_CB01,BM_GETCHECK,0,0L);
    if(result==BST_CHECKED) bEnable = TRUE;
    // now set the enabled state of the relevant controls
    EnableWindow(GetDlgItem(hDlg, IDC_ST01), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST02), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ED01), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP01), bEnable);
    }

void VASaveLightingControls(HWND hDlg, int iRoom)
    {
    BOOL bState;
    // first read the lighting checkbox
    bState = SendDlgItemMessage(hDlg, IDC_CB01, BM_GETCHECK, 0, 0L);
    if(bState == BST_CHECKED)
         GR_Room[iRoom].bVacLights = TRUE;
    else GR_Room[iRoom].bVacLights = FALSE;
    // now read the height to maintain the light above the plant
    GR_Room[iRoom].iVacLightMM = max(min(1000, GLGetNumText(hDlg, IDC_ED01)), 25);
    }


void VAInitLightingControls(HWND hDlg, int iRoom)
    {
    long lrange = 0;
    BOOL bState = BST_UNCHECKED;
    // first set the lighting checkbox
    if(GR_Room[iRoom].bVacLights) bState = BST_CHECKED;
    SendDlgItemMessage(hDlg, IDC_CB01, BM_SETCHECK, bState, 0L);
    // now set the height to maintain the light above the plant
    GLSetNumText(GR_Room[iRoom].iVacLightMM, hDlg, IDC_ED01);
    // now setup the spinner controls
    lrange = MAKELONG((short) 1000, (short) 50);
    SendDlgItemMessage(hDlg, IDC_SP01, UDM_SETRANGE, 0, lrange);
    // now enable or disable the relevant controls 
    VAQueryEnableLighting(hDlg);
    }

/****************************************************************************\
* MANAGING THE WATERING CONTROLS
\****************************************************************************/

void VAQueryEnableWatering(HWND hDlg)
    {
    LONG result;
    BOOL bEnable = FALSE;
    // check the state of our checkbox
    result = SendDlgItemMessage(hDlg,IDC_CB02,BM_GETCHECK,0,0L);
    if(result==BST_CHECKED) bEnable = TRUE;
    // now set the enabled state of the relevant controls
    EnableWindow(GetDlgItem(hDlg, IDC_ED02), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ED03), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP02), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP03), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST04), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST11), bEnable);
    }


void VASaveWateringControls(HWND hDlg, int iRoom)
    {
    BOOL bState;
    // first read the watering checkbox
    bState = SendDlgItemMessage(hDlg, IDC_CB02, BM_GETCHECK, 0, 0L);
    if(bState == BST_CHECKED)
         GR_Room[iRoom].bVacWater = TRUE;
    else GR_Room[iRoom].bVacWater = FALSE;
    // now read the amount to water
    GR_Room[iRoom].iVacWaterML = max(min(GLGetNumText(hDlg, IDC_ED02), 2000), 100);
    // now read the watering days
    GR_Room[iRoom].iVacWaterDays = max(min(GLGetNumText(hDlg, IDC_ED03), 10), 1);
    }


void VAInitWateringControls(HWND hDlg, int iRoom)
    {
    long lrange = 0;
    BOOL bState = BST_UNCHECKED;
    // first set the watering checkbox
    if(GR_Room[iRoom].bVacWater) bState = BST_CHECKED;
    SendDlgItemMessage(hDlg, IDC_CB02, BM_SETCHECK, bState, 0L);
    // now set the amount to water
    GLSetNumText(GR_Room[iRoom].iVacWaterML, hDlg, IDC_ED02);
    // now set the watering days
    GLSetNumText(GR_Room[iRoom].iVacWaterDays, hDlg, IDC_ED03);
    // now setup the spinner controls
    lrange = MAKELONG((short) 2000, (short) 100); 
    SendDlgItemMessage(hDlg, IDC_SP02, UDM_SETRANGE, 0, lrange);
    lrange = MAKELONG((short) 10, (short) 1); 
    SendDlgItemMessage(hDlg, IDC_SP03, UDM_SETRANGE, 0, lrange);
    // now enable or disable the relevant controls 
    VAQueryEnableWatering(hDlg);
    }


/****************************************************************************\
* MANAGING THE FERTILIZING CONTROLS
\****************************************************************************/

void VAQueryEnableFertilizing(HWND hDlg)
    {
    LONG result;
    BOOL bEnable = FALSE;
    // check the state of our checkbox
    result = SendDlgItemMessage(hDlg,IDC_CB03,BM_GETCHECK,0,0L);
    if(result==BST_CHECKED) bEnable = TRUE;
    // now set the enabled state of the relevant controls
    EnableWindow(GetDlgItem(hDlg, IDC_ED04), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ED05), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ED06), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ED07), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ED08), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP04), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP05), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP06), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP07), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_SP08), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST06), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST08), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST09), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST10), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST13), bEnable);
    EnableWindow(GetDlgItem(hDlg, IDC_ST14), bEnable);
    }


void VASaveFertilizingControls(HWND hDlg, int iRoom)
    {
    BOOL bState;
    char szText[50]="\0";
    // first read the fertilizing checkbox
    bState = SendDlgItemMessage(hDlg, IDC_CB03, BM_GETCHECK, 0, 0L);
    if(bState == BST_CHECKED)
         GR_Room[iRoom].bVacFert = TRUE;
    else GR_Room[iRoom].bVacFert = FALSE;
    // now set the number of waterings per fertilizing
    GR_Room[iRoom].iVacFertWaters = max(min(GLGetNumText(hDlg, IDC_ED04), 10), 1);
    // now set the amount of N to add
    SendDlgItemMessage(hDlg, IDC_ED05, WM_GETTEXT, 50, (LPARAM)(LPCTSTR)&szText);
    GR_Room[iRoom].iVacFertN = (int)(atof(szText)*10);
    // now set the amount of N to add
    SendDlgItemMessage(hDlg, IDC_ED06, WM_GETTEXT, 50, (LPARAM)(LPCTSTR)&szText);
    GR_Room[iRoom].iVacFertP = (int)(atof(szText)*10);
    // now set the amount of N to add
    SendDlgItemMessage(hDlg, IDC_ED07, WM_GETTEXT, 50, (LPARAM)(LPCTSTR)&szText);
    GR_Room[iRoom].iVacFertK = (int)(atof(szText)*10);
    // now set the amount of N to add
    SendDlgItemMessage(hDlg, IDC_ED08, WM_GETTEXT, 50, (LPARAM)(LPCTSTR)&szText);
    GR_Room[iRoom].iVacFertCa = (int)(atof(szText)*10);
    }


void VAInitFertilizingControls(HWND hDlg, int iRoom)
    {
    long lrange = 0;
    char szText[50]="\0";
    BOOL bState = BST_UNCHECKED;
    // first set the fertilizing checkbox
    if(GR_Room[iRoom].bVacFert) bState = BST_CHECKED;
    SendDlgItemMessage(hDlg, IDC_CB03, BM_SETCHECK, bState, 0L);
    // now set the number of waterings per fertilizing
    GLSetNumText(GR_Room[iRoom].iVacFertWaters, hDlg, IDC_ED04);
    // now set the amount of N to add
    sprintf(szText, "%1.1f", (GR_Room[iRoom].iVacFertN/10.00));
    SendDlgItemMessage(hDlg, IDC_ED05, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)&szText);
    // now set the amount of N to add
    sprintf(szText, "%1.1f", (GR_Room[iRoom].iVacFertP/10.00));
    SendDlgItemMessage(hDlg, IDC_ED06, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)&szText);
    // now set the amount of N to add
    sprintf(szText, "%1.1f", (GR_Room[iRoom].iVacFertK/10.00));
    SendDlgItemMessage(hDlg, IDC_ED07, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)&szText);
    // now set the amount of N to add
    sprintf(szText, "%1.1f", (GR_Room[iRoom].iVacFertCa/10.00));
    SendDlgItemMessage(hDlg, IDC_ED08, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)&szText);
    // now setup the spinner controls
    lrange = MAKELONG((short) 10, (short) 1); 
    SendDlgItemMessage(hDlg, IDC_SP04, UDM_SETRANGE, 0, lrange);
    lrange = MAKELONG((short) 5, (short) 0); 
    SendDlgItemMessage(hDlg, IDC_SP05, UDM_SETRANGE, 0, lrange);
    SendDlgItemMessage(hDlg, IDC_SP06, UDM_SETRANGE, 0, lrange);
    SendDlgItemMessage(hDlg, IDC_SP07, UDM_SETRANGE, 0, lrange);
    SendDlgItemMessage(hDlg, IDC_SP08, UDM_SETRANGE, 0, lrange);
    // now enable or disable the relevant controls 
    VAQueryEnableFertilizing(hDlg);
    }


/****************************************************************************\
* CREATING AND DESTROYING THE VACATION MODE DIALOG
\****************************************************************************/

void VAInitVacationDialog(HWND hDlg, HWND hParent, HINSTANCE dhInst)
    {
    long lp    = MAKELPARAM(TRUE, 0);
    char sztext[100] ="\0";
    // center dialog in window (only if the parent is a valid window)
    if(hParent) DlgCenter(hDlg);
    // here we set the dialog box text
    TBGetComboGrowroomName(strbuf);
	wsprintf(sztext, "Vacation Mode Setup - %s", strbuf);
    SetWindowText(hDlg, sztext);

    // now we'll create some bold fonts
    hVacBigFont = GLCreateDialogFont(20,0,FW_BOLD);
    hVacMedFont = GLCreateDialogFont(15,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hVacBigFont)
        {
        SendDlgItemMessage(hDlg,IDC_STITLE,WM_SETFONT,(WPARAM)hVacBigFont,lp);
        }
    // if we created the seed text font, set it to all the heading controls
    if(hVacMedFont) 
        {
        SendDlgItemMessage(hDlg,IDC_CB01,WM_SETFONT,(WPARAM)hVacMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_CB02,WM_SETFONT,(WPARAM)hVacMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_CB03,WM_SETFONT,(WPARAM)hVacMedFont,lp);
        }
    // now enable or disable the relevant controls
    VAInitLightingControls(hDlg, iVacRoom);
    VAInitWateringControls(hDlg, iVacRoom);
    VAInitFertilizingControls(hDlg, iVacRoom);
    }


void VASaveVacationDialog(HWND hDlg)
    {
    // save all our settings to the growroom struct
    VASaveLightingControls(hDlg, iVacRoom);
    VASaveWateringControls(hDlg, iVacRoom);
    VASaveFertilizingControls(hDlg, iVacRoom);
    // now open the growroom details file and save the changes
    GESaveRoomDetailsToFile();
    }

/****************************************************************************\
* HANDLING SPINNER CONTROL NOTIFICATIONS
\****************************************************************************/

void VAAdjustFertSpinner(HWND hDlg, WPARAM CtrlId, int iDelta)
    {
    int i;
    char szText[50]="\0";
    // ---
    SendDlgItemMessage(hDlg, CtrlId, WM_GETTEXT, 50, (LPARAM)(LPCTSTR)&szText);
    i = (int)(atof(szText)*10);
    if(iDelta >= 0)
         i += 1;
    else i -= 1;
    i = min(max(i, 0), 15);
    sprintf(szText, "%1.1f", (i/10.00));
    SendDlgItemMessage(hDlg, CtrlId, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)&szText);
    }
  
BOOL VACheckSpinnerAction(HWND hDlg, WPARAM CtrlId, LPNMUPDOWN lpnmud)
    {
    int i;
    if(lpnmud->hdr.code == UDN_DELTAPOS)
        {
        switch (lpnmud->hdr.idFrom)
            {
            case IDC_SP01:
                {
                 i = GLGetNumText(hDlg, IDC_ED01);
                 if(lpnmud->iDelta >= 0)
                      i += 50;
                 else i -= 50;
                 GLSetNumText(min(max(i, 50), 1000), hDlg, IDC_ED01);
                }
            return TRUE;

            case IDC_SP02:
                {
                 i = GLGetNumText(hDlg, IDC_ED02);
                 if(lpnmud->iDelta >= 0)
                      i += 100;
                 else i -= 100;
                 GLSetNumText(min(max(i, 100), 2000), hDlg, IDC_ED02);
                }
            return TRUE;

            case IDC_SP05:
                 VAAdjustFertSpinner(hDlg, IDC_ED05, lpnmud->iDelta);
            return TRUE;

            case IDC_SP06:
                 VAAdjustFertSpinner(hDlg, IDC_ED06, lpnmud->iDelta);
            return TRUE;

            case IDC_SP07:
                 VAAdjustFertSpinner(hDlg, IDC_ED07, lpnmud->iDelta);
            return TRUE;

            case IDC_SP08:
                 VAAdjustFertSpinner(hDlg, IDC_ED08, lpnmud->iDelta);
            return TRUE;
            }
        }
    return FALSE;
    }


/****************************************************************************\
* VACATION MODE DIALOG WINPROC
\****************************************************************************/
  
BOOL CALLBACK VAVacationDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);

    switch (message)
      {
      case WM_INITDIALOG:
           VAInitVacationDialog(hDlg, hParent, dhInst);
      return (TRUE);

      case WM_DESTROY:
            {
            if(hVacBigFont) DeleteObject(hVacBigFont);
            if(hVacMedFont) DeleteObject(hVacMedFont);
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
          int i = GetDlgCtrlID((HWND)lParam);
          // do nothing if it's a disabled edit control we're painting
          if((i==IDC_ED01)||(i==IDC_ED02)||(i==IDC_ED03)||(i==IDC_ED04)||
             (i==IDC_ED05)||(i==IDC_ED06)||(i==IDC_ED07)||(i==IDC_ED08)||
             (i==IDC_CB01)||(i==IDC_CB02)||(i==IDC_CB03))
             return FALSE;
          // otherwise return a transparent setting
          SetBkMode((HDC)wParam, TRANSPARENT);
          return (int)GetStockObject(NULL_BRUSH);
          }
       return (FALSE);

      case WM_NOTIFY:
          {
          switch (wParam)
            { // lighting above plant
            case IDC_SP01:
            // watering amount
            case IDC_SP02:
            // nutrient spinners
            case IDC_SP05:
            case IDC_SP06:
            case IDC_SP07:
            case IDC_SP08:
            // check the relevant action
            return VACheckSpinnerAction(hDlg, wParam, (LPNMUPDOWN)lParam);
            }
          }
       return (FALSE);


      case WM_COMMAND:
          switch (wParam)
            {
            case IDC_CB01:
                 VAQueryEnableLighting(hDlg);
            return FALSE;

            case IDC_CB02:
                 VAQueryEnableWatering(hDlg);
            return FALSE;

            case IDC_CB03:
                 VAQueryEnableFertilizing(hDlg);
            return FALSE;

            case IDOK:
                {
                // first save the relevant setings
                VASaveVacationDialog(hDlg);
                // now end the dialog
                EndDialog(hDlg, TRUE); 
                }
            return TRUE;

            case IDCANCEL:
                EndDialog(hDlg, FALSE); 
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE);               
   }                          


void VAShowVacationDialog(HWND hwnd, HINSTANCE hInst, int iRoom)
    {
    iVacRoom = max(min(iRoom, gMaxRooms), 0);
    DialogBox(hInst, "Vac_Edit", hwnd, VAVacationDlgProc);
    }


/****************************************************************************\
* VACATION MODE ENABLE MODE DIALOG BOX AND WINPROC
\****************************************************************************/

HFONT hVacEnableBigFont;
HFONT hVacEnableMedFont;
HFONT hVacEnableSmallFont;


void VERoomListBoxChange(HWND hDlg, HINSTANCE dhInst)
    {
    int  iRoom, iFTot;
    char szftot[10]  = "\0";
    char sztext[100] = "\0";
    // first check which seed is currently selected
    iRoom = SendDlgItemMessage(hDlg, IDC_LB01, LB_GETCURSEL, 0,0);
    if(iRoom == LB_ERR) return;
    // set the vacation mode settings text into the static controls
    // first set the lighting vacation mode text
    if(GR_Room[iRoom].bVacLights)
        {
        wsprintf(sztext, "%i mm above plants", GR_Room[iRoom].iVacLightMM);
        SendDlgItemMessage(hDlg, IDC_ST08, WM_SETTEXT, 0, (LPARAM)sztext);
        }
    else 
        SendDlgItemMessage(hDlg, IDC_ST08, WM_SETTEXT, 0, (LPARAM)"No Lighting Check");
    // now set the watering vacation mode text
    if(GR_Room[iRoom].bVacWater)
        {
        wsprintf(sztext, "%i ml every %i days", 
                  GR_Room[iRoom].iVacWaterML, GR_Room[iRoom].iVacWaterDays);
        SendDlgItemMessage(hDlg, IDC_ST09, WM_SETTEXT, 0, (LPARAM)sztext);
        }
    else 
        SendDlgItemMessage(hDlg, IDC_ST09, WM_SETTEXT, 0, (LPARAM)"No Vacation Watering");
    // now set the fertilizing vacation mode text
    if(GR_Room[iRoom].bVacFert)
        {
        iFTot = GR_Room[iRoom].iVacFertN + GR_Room[iRoom].iVacFertP +                 
                GR_Room[iRoom].iVacFertK + GR_Room[iRoom].iVacFertCa;
        sprintf(szftot, "%1.1f", (iFTot/10.00));
        wsprintf(sztext, "%s g every %i waterings", szftot, GR_Room[iRoom].iVacFertWaters);
        SendDlgItemMessage(hDlg, IDC_ST10, WM_SETTEXT, 0, (LPARAM)sztext);
        }
    else 
        SendDlgItemMessage(hDlg, IDC_ST10, WM_SETTEXT, 0, (LPARAM)"No Vacation Fertilizing");
    }



void VEInitVacEnableDialog(HWND hDlg, HWND hParent, HINSTANCE dhInst)
    {
    long lp    = MAKELPARAM(TRUE, 0);
    int  iRoom = 0;
    // center dialog in window (only if the parent is a valid window)
    if(hParent) DlgCenter(hDlg);
    // here we set the dialog box text
    // now we'll create some bold fonts
    hVacEnableBigFont   = GLCreateDialogFont(20,0,FW_BOLD);
    hVacEnableMedFont   = GLCreateDialogFont(15,0,FW_BOLD);
    hVacEnableSmallFont = GLCreateDialogFont(14,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hVacEnableBigFont)
        SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hVacEnableBigFont,lp);
    // if we created the seed text font, set it to all the heading controls
    if(hVacEnableMedFont) 
        {
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hVacEnableMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST03,WM_SETFONT,(WPARAM)hVacEnableMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST04,WM_SETFONT,(WPARAM)hVacEnableMedFont,lp);
        }
    if(hVacEnableSmallFont) 
        {
        SendDlgItemMessage(hDlg,IDC_ST05,WM_SETFONT,(WPARAM)hVacEnableSmallFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST06,WM_SETFONT,(WPARAM)hVacEnableSmallFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST07,WM_SETFONT,(WPARAM)hVacEnableSmallFont,lp);
        }
    // set the current status of the vacation mode
    if(PACheckVacModeEnabled())
         SendDlgItemMessage(hDlg,IDC_ST02,WM_SETTEXT,0,(LPARAM)"Currently Enabled");
    else SendDlgItemMessage(hDlg,IDC_ST02,WM_SETTEXT,0,(LPARAM)"Currently Disabled");

    // now fill the grow room combobox with room names
    // Show our standard growroom names
    GLFillGrowroomNamesListbox(hDlg, IDC_LB01);
//    SendDlgItemMessage(hDlg, IDC_LB01, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Workshop");
//    SendDlgItemMessage(hDlg, IDC_LB01, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Basement");
//    SendDlgItemMessage(hDlg, IDC_LB01, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)"The Attic");
	// and select the first growroom
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCURSEL, 0, 0L);
    VERoomListBoxChange(hDlg, dhInst);
    }



BOOL CALLBACK VEVacEnableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);

    switch (message)
      {
      case WM_INITDIALOG:
           VEInitVacEnableDialog(hDlg, hParent, dhInst);
      return (TRUE);

      case WM_DESTROY:
            {
            if(hVacEnableBigFont)   DeleteObject(hVacEnableBigFont);
            if(hVacEnableMedFont)   DeleteObject(hVacEnableMedFont);
            if(hVacEnableSmallFont) DeleteObject(hVacEnableSmallFont);
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
          int i = GetDlgCtrlID((HWND)lParam);
          if((i==IDC_ST08)||(i==IDC_ST09)||(i==IDC_ST10))
              return FALSE;

          SetBkMode((HDC)wParam, TRANSPARENT);
          return (int)GetStockObject(NULL_BRUSH);
          }
       return (FALSE);

      case WM_DRAWITEM:
          {
          if(wParam==IDC_LB01)
              TBODrawComboGrowroomItem((LPDRAWITEMSTRUCT)lParam);
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_MEASUREITEM:
           if(wParam==IDC_LB01)
               {
               LPMEASUREITEMSTRUCT lpMIS = (LPMEASUREITEMSTRUCT)lParam;
               lpMIS->itemHeight = 18;
               return(TRUE);
               }
      return (FALSE);  // only done because we're redirecting the focus


      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_LB01:
                {
                if(HIWORD(wParam)==LBN_SELCHANGE)
                   VERoomListBoxChange(hDlg, dhInst);
                }
            break;

            case IDC_BT01:
                {
                int iRoom = SendDlgItemMessage(hDlg, IDC_LB01, LB_GETCURSEL, 0, 0L);
                if(iRoom == LB_ERR) return FALSE;
                // show the vacation settings edit for this room
                VAShowVacationDialog(hDlg, dhInst, iRoom);
                // now update the changes in the static controls
                VERoomListBoxChange(hDlg, dhInst);
                }
            return TRUE;


            case IDOK:
                {
                // first save the relevant setings
                PASetVacModeEnabled(TRUE);
                // now end the dialog
                EndDialog(hDlg, TRUE); 
                }
            return TRUE;

            case IDCANCEL:
                {
                // first save the relevant setings
                if(PACheckVacModeEnabled())
                    PAToggleVacModeOption();
                // now end the dialog
                EndDialog(hDlg, FALSE); 
                }
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE);               
   }                          


void VAShowVacationEnableDialog(HWND hwnd, HINSTANCE hInst)
    {
    DialogBox(hInst, "Vac_Enable", hwnd, VEVacEnableDlgProc);
    }


/****************************************************************************\
* SETTING OR UNSETTING THE HIGHGROW ALARM CLOCK
\****************************************************************************/


void VESaveAlarmClockDialog(HWND hDlg, HINSTANCE dhInst)
    {
    char sztext[150];
    int ihrs, imin, ibn;
    // ------------
    SendDlgItemMessage(hDlg, IDC_ROBTEXT, WM_GETTEXT, sizeof(sztext), (LPARAM)&sztext);
    // read the hours combobox
    ihrs = SendDlgItemMessage(hDlg, IDC_CM01, CB_GETCURSEL, 0, 0L);
    if(ihrs == CB_ERR) ihrs = 0;
    // read the minutes combobox
    imin = SendDlgItemMessage(hDlg, IDC_CM02, CB_GETCURSEL, 0, 0L);
    if(imin == CB_ERR) imin = 0;
    // read the AM/PM combobox
    ibn = SendDlgItemMessage(hDlg, IDC_CM03, CB_GETCURSEL, 0, 0L);
    if(ibn == CB_ERR) ibn = 0;
    // now we save everything in the registry
    PASaveAlarmSettings((LPSTR)&sztext, ihrs, imin, ibn, TRUE);
//    PASetAlarmClock(TRUE);
    }


void VEInitAlarmClockDialog(HWND hDlg, HWND hParent, HINSTANCE dhInst)
    {
    long lp    = MAKELPARAM(TRUE, 0);
    int  iRoom = 0;
    int  i;
    char sztext[150];
    int  ihrs, imin, ibn, iset;
    // first we load our alarm clock settings
    PALoadAlarmSettings((LPSTR)&sztext, (int*)&ihrs, (int*)&imin, (int*)&ibn, (int*)&iset);
    // center dialog in window (only if the parent is a valid window)
    if(hParent) DlgCenter(hDlg);
    // here we set the dialog box text
    // now we'll create some bold fonts
    hVacEnableBigFont   = GLCreateDialogFont(20,0,FW_BOLD);
    hVacEnableMedFont   = GLCreateDialogFont(15,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hVacEnableBigFont)
        SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hVacEnableBigFont,lp);
    // if we created the seed text font, set it to all the heading controls
    if(hVacEnableMedFont) 
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hVacEnableMedFont,lp);
    // set the current status of the vacation mode
    if(iset)
         SendDlgItemMessage(hDlg,IDC_ST02,WM_SETTEXT,0,(LPARAM)"Currently Set");
    else SendDlgItemMessage(hDlg,IDC_ST02,WM_SETTEXT,0,(LPARAM)"Currently Unset");
 
    // first show Robbies words, loaded from the registry
    SendDlgItemMessage(hDlg,IDC_ROBTEXT,WM_SETTEXT,0,(LPARAM)sztext);
    
    // Show our standard clock times
    for(i=0;i<12;i++)
        {
        wsprintf(sztext, "%02i", i);
        SendDlgItemMessage(hDlg, IDC_CM01, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)sztext);
        }
    for(i=0;i<60;i++)
        {
        wsprintf(sztext, "%02i", i);
        SendDlgItemMessage(hDlg, IDC_CM02, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)sztext);
        }
    SendDlgItemMessage(hDlg, IDC_CM03, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"AM");
    SendDlgItemMessage(hDlg, IDC_CM03, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"PM");
    // here we select the currently set time values
    SendDlgItemMessage(hDlg, IDC_CM01, CB_SETCURSEL, ihrs, 0L);
    SendDlgItemMessage(hDlg, IDC_CM02, CB_SETCURSEL, imin, 0L);
    SendDlgItemMessage(hDlg, IDC_CM03, CB_SETCURSEL, ibn, 0L);

    // and select the first growroom
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCURSEL, 0, 0L);
    }


BOOL CALLBACK VEAlarmClockDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);

    switch (message)
      {
      case WM_INITDIALOG:
           VEInitAlarmClockDialog(hDlg, hParent, dhInst);
      return (TRUE);

      case WM_DESTROY:
            {
            if(hVacEnableBigFont)   DeleteObject(hVacEnableBigFont);
            if(hVacEnableMedFont)   DeleteObject(hVacEnableMedFont);
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
       return (FALSE);

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDOK:
                {
                // first save the relevant setings
                VESaveAlarmClockDialog(hDlg, dhInst);
                // now end the dialog
                EndDialog(hDlg, TRUE); 
                }
            return TRUE;

            case IDCANCEL:
                {
                // first save the relevant setings
                if(PACheckAlarmSet())
                    PAToggleAlarmClockOption();
                // now end the dialog
                EndDialog(hDlg, FALSE); 
                }
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE);               
   }                          


void VAShowAlarmSetDialog(HWND hwnd, HINSTANCE hInst)
    {
    DialogBox(hInst, "ALARMSET", hwnd, VEAlarmClockDlgProc);
    }
