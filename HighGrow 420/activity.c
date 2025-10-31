#include <windows.h>
#include <commctrl.h>
#include "stdio.h"
#include "prsht.h"
#include "resource.h"
#include "global.h"
#include "plantmem.h"
#include "growroom.h"
#include "nodecalc.h"
#include "toolbar.h"
#include "calc.h"
// #include "plantseed.h"
    
/******************************************************************************\
*  GLOBAL VARIABLES FOR THIS FILE
\******************************************************************************/

BOOL gbPrunedNow = FALSE; // our one global variable in this file

/******************************************************************************\
*  FUNCTION DEFININTIONS USED GENERALLY IN THIS FILE
\******************************************************************************/

BOOL ACActivityInitGrowInfo(int iPlant)
    {
    char szFile[128]="\0";
    BOOL bSuccess=FALSE;

    giCurPlant = iPlant;
    sprintf(szFile, "Plant%02i.hgp", iPlant);
    if(GLDoesFileExist(szFile))
        {
        if(PMPlantFileToMem(iPlant-1))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            // now fill our current growday structs
            CACalculatePlant(P_Plant, iPlant-1);
            GLInitPlantInfo(&P_Plant->PI_Plant);
            bSuccess = GLInitGrowDay(&P_Plant->GD_Plant[giGrowDay]);
            // we can unlock our memory again
            GlobalUnlock(hPlantMem);
            // and then free our memory again
            PMPlantMemoryFree();
            }
        }
    return bSuccess;
    }
 
BOOL ACActivitySaveGrowDay(HWND hDlg)
    { // ** SAVE THE DAY'S CHANGES
    char szFile[128]="\0";
    BOOL bSuccess=FALSE;
    int iRoomOfs = (TBGetComboGrowroom()-1)*3;

    sprintf(szFile, "Plant%02i.hgp", giCurPlant);
    if(GLDoesFileExist(szFile))
        {
        if(PMPlantFileToMem(giCurPlant-1))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            // now fill our current growday structs
            bSuccess = GLSaveGrowDay(&P_Plant->GD_Plant[giGrowDay]);
            // if he pruned now, remove some nodes
            if(gbPrunedNow == TRUE)
                 NOPruneRandomTips(P_Plant, giCurPlant-1-iRoomOfs, giCurPlant-1, giPrunedTips); 
            else // we alway's recalculate our lamp drawing positions
                 // UNLESS we pruned, because NOPruneRandomTips recalcs it too
                 GRCalcDrawLamp((PPLANT)P_Plant, giCurPlant-1-iRoomOfs, TRUE);
            // we must always reset our prunednow bool variable
            gbPrunedNow = FALSE;
            // we can unlock our memory again
            GlobalUnlock(hPlantMem);
            // finally, save our changes back
            PMPlantMemToFile(giCurPlant-1);
            }
        }
    // *** UPDATE THE LAST VISITED GLOBAL ***
    giLastVisited = giGrowDay;
    // *** SEND UPDATE MESSAGE TO ALL PAGES ***
    PropSheet_QuerySiblings(GetParent(hDlg), 0, 0);

    return bSuccess;
    }

/******************************************************************************\
*  FUNCTIONS TO HANDLE: The TEMPORARY calculated property sheet page
\******************************************************************************/
/*
// IDT_LB01 = Day number
// IDT_RB01 = Plant A
// IDT_RB02 = Plant B
// IDT_RB03 = Plant C
// IDT_BT01 = Save changes
// IDT_CB01 = Visited?
// IDT_ED01 = Health
// IDT_ED02 = Mass
// IDT_ED03 = Height
// IDT_ED04 = Moisture
// IDT_ED05 = Potency
// IDT_ED06 = Soil pH

void ACTempShowDay(HWND hDlg)
    {
    // *** VISITED ***
    if(GD_CurPlant.bVisited==TRUE)
         SendDlgItemMessage(hDlg, IDT_CB01, BM_SETCHECK, BST_CHECKED,   0);
    else SendDlgItemMessage(hDlg, IDT_CB01, BM_SETCHECK, BST_UNCHECKED, 0);
    // *** HEALTH % ***
    GLSetNumText(GD_CurPlant.cHealth, hDlg, IDT_ED01);
    // *** GROWING MASS ***
    GLSetNumText(GD_CurPlant.iMass, hDlg, IDT_ED02);
    // *** HEIGHT IN MM ***
    GLSetNumText(GD_CurPlant.iHeight, hDlg, IDT_ED03);
    // *** MOISTURE IN ML ***
    GLSetNumText(GD_CurPlant.iMoisture, hDlg, IDT_ED04);
    // *** POTENCY THC ***
    sprintf(strbuf, "%1.2f", (GD_CurPlant.iPotency/100.00));
    SendDlgItemMessage(hDlg, IDT_ED05, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** SOIL PH ***
    sprintf(strbuf, "%1.1f", (GD_CurPlant.cSoilPH/10.0));
    SendDlgItemMessage(hDlg, IDT_ED06, WM_SETTEXT, 0, (LPARAM)strbuf);
    }

void ACInitTempDialog(HWND hDlg)
    {
    int i;
    for(i=1;i<gMaxDays;i++)
        {
        sprintf(strbuf, "%03i", i);
        SendDlgItemMessage(hDlg, IDT_LB01, LB_ADDSTRING, 0, (LPARAM)strbuf);
        }
    SendDlgItemMessage(hDlg, IDT_LB01, LB_SETCURSEL, giGrowDay-1, 0);
    SendDlgItemMessage(hDlg, IDT_RB01, BM_SETCHECK, BST_CHECKED,  0);

    ACTempShowDay(hDlg);
    }


void ACTempChangeDay(HWND hDlg, WPARAM wParam)
    {
    if(LOWORD(wParam)==IDT_LB01)
        {
        giGrowDay = (SendDlgItemMessage(hDlg, IDT_LB01, LB_GETCURSEL, 0, 0)+1);
        ACActivityInitGrowInfo();
        ACTempShowDay(hDlg);
        }
    }


void ACTempSaveDlg(HWND hDlg)
    {
    // *** VISITED ***
    if(SendDlgItemMessage(hDlg, IDT_CB01, BM_GETCHECK, 0, 0)==BST_CHECKED)
         GD_CurPlant.bVisited=TRUE;
    else GD_CurPlant.bVisited=FALSE;
    // *** HEALTH % ***
    GD_CurPlant.cHealth   = GLGetNumText(hDlg, IDT_ED01);
    // *** GROWING MASS ***
    GD_CurPlant.iMass     = GLGetNumText(hDlg, IDT_ED02);
    // *** HEIGHT IN MM ***
    GD_CurPlant.iHeight   = GLGetNumText(hDlg, IDT_ED03);
    // *** MOISTURE IN ML ***
    GD_CurPlant.iMoisture = GLGetNumText(hDlg, IDT_ED04);
    // *** POTENCY THC ***
    GD_CurPlant.iPotency  = GLGetNumText(hDlg, IDT_ED05);
    // *** SOIL PH ***
    GD_CurPlant.cSoilPH   = GLGetNumText(hDlg, IDT_ED06);

    ACActivitySaveGrowDay(hDlg);
    }


BOOL CALLBACK ACTempDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   
    switch (message)
      {
      case WM_INITDIALOG:
           ACInitTempDialog(hDlg);
      return (TRUE);

      case PSM_QUERYSIBLINGS:
           // *** MOISTURE IN ML ***
           GLSetNumText(GD_CurPlant.iMoisture, hDlg, IDT_ED04);
      return (TRUE);

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDT_BT01: ACTempSaveDlg(hDlg);           return FALSE;
            case IDT_LB01: ACTempChangeDay(hDlg, wParam); return FALSE;
            }  
          return FALSE;
      return (FALSE);
      }
    return (FALSE);  
   }                          
   
*/

/******************************************************************************\
*  FUNCTIONS TO HANDLE: The STATISTICS property sheet page
\******************************************************************************/

// IDS_ST01 = Date Planted
// IDS_ST02 = Growth Stage
// IDS_ST03 = Gender (sex)
// IDS_ST04 = Photoperiod
// IDS_ST05 = Last Visited
// IDS_ST06 = Last Watered
// IDS_ST07 = Last Fertilized
// IDS_ST08 = Last Pruned
// IDS_ST09 = Height (mm)
// IDS_ST10 = Health (%)
// IDS_ST11 = Potency THC
// IDS_ST12 = Moisture (ml)
// IDS_ST13 = Soild pH
// IDS_ST14 = Number of Growing Tips
// IDS_ST15 = Growing Mass (grams)
// IDS_ST16 = Pruned Mass (grams)
// IDS_ST17 = Plant Name


void ACInitStatusDialog(HWND hDlg, BOOL bUpdate)
    {
    // This is called when the dialog is first shown and everytime we get a
    // PSN_QUERYSIBLINGS message after he's clicked the APPLY button.
    // If this is the first showing, we must do everything (bUpdate = FALSE),
    // but if we must apply new attributes, we show only those that change.
    int  i;
    double j=0;

    // *** FIRST THE THINGS WE MUST ALWAYS DO ***
    // *** PHOTOPERIOD ***
    i = GLCurPlantPhotoperiod();
    sprintf(strbuf, "%02i hrs %02i min", i/4, (i-4*(i/4))*15);
    SendDlgItemMessage(hDlg, IDS_ST04, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** LAST VISITED ***
    GLNextFormatDate((char*)PI_CurPlant.szStartDate, (char*)&strbuf, giLastVisited-1);
    SendDlgItemMessage(hDlg, IDS_ST05, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** LAST WATERED ***
    if(giLastWatered)
         GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&strbuf, giLastWatered-1);
    else strcpy(strbuf, "Unwatered");
    SendDlgItemMessage(hDlg, IDS_ST06, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** MOISTURE IN ML ***
    GLSetNumText(GD_CurPlant.iMoisture, hDlg, IDS_ST12);
    // *** LAST FERTILISED ***
    if(giLastFertilized)
         GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&strbuf, giLastFertilized-1);
    else strcpy(strbuf, "Unfertilized");
    SendDlgItemMessage(hDlg, IDS_ST07, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** LAST PRUNED ***
    if(giLastPruned)
       GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&strbuf, giLastPruned-1);
    else strcpy(strbuf, "Unpruned");
    SendDlgItemMessage(hDlg, IDS_ST08, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** HEIGHT IN MM ***
    GLSetNumText(GD_CurPlant.iHeight, hDlg, IDS_ST09);
    // *** HEALTH % ***
    GLSetNumText(GD_CurPlant.cHealth, hDlg, IDS_ST10);
    // *** POTENCY THC ***
    sprintf(strbuf, "%1.2f", (GD_CurPlant.iPotency/100.00));
    SendDlgItemMessage(hDlg, IDS_ST11, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** SOIL PH ***
    sprintf(strbuf, "%1.1f", (GD_CurPlant.cSoilPH/10.0));
    SendDlgItemMessage(hDlg, IDS_ST13, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** GROWING MASS ***
    GLSetNumText(GD_CurPlant.iMass, hDlg, IDS_ST15);
    // *** PRUNED MASS ***
    sprintf(strbuf, "%1.1f", gdPrunedMass);
    SendDlgItemMessage(hDlg, IDS_ST16, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** GETOUT IF WE GOT A PSN_QUERYSIBLINGS MESSAGE ***
    if(bUpdate) return;

    // *** CONTINUE WITH THE THINGS THAT WON'T CHANGE ***
    // *** START DATE ***
    GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)strbuf, 0);
    SendDlgItemMessage(hDlg, IDS_ST01, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** GROWTH STAGE ***
    GLGetGrowthStageString(strbuf, PI_CurPlant.cGrowthStage);
    SendDlgItemMessage(hDlg, IDS_ST02, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** GENDER ***
    if(PI_CurPlant.cGrowthStage>=4) 
        { // until we're preflowering, we don't show the sex
        if(PS_Plant[giCurPlant-1].bSeedGender == TRUE) 
             strcpy(strbuf, "Female");
        else strcpy(strbuf, "Male");
        }
    else strcpy(strbuf, "Unknown");
    SendDlgItemMessage(hDlg, IDS_ST03, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** GROWING TIPS ***
    _ltoa(giGrowingTips, strbuf, 10);
    SendDlgItemMessage(hDlg, IDS_ST14, WM_SETTEXT, 0, (LPARAM)strbuf);
    }


BOOL CALLBACK ACStatsDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   static BOOL bStatusUpdate;  // set when we must update our page 
   
    switch (message)
      {
      case WM_INITDIALOG:
           DlgCenter(hParent);
           ACInitStatusDialog(hDlg, FALSE);
      return (TRUE);

      case PSM_QUERYSIBLINGS:
           bStatusUpdate = TRUE;
      return (TRUE);

      case WM_NOTIFY:
          switch (((LPNMHDR)lParam)->code)
             {
             case PSN_SETACTIVE: 
                  if(bStatusUpdate)
                      {
                      ACInitStatusDialog(hDlg, TRUE);
                      bStatusUpdate = FALSE;
                      }
             return PSNRET_NOERROR;
             }
            return (FALSE);
      }
    return (FALSE);  
   }                          
   

/******************************************************************************\
*  FUNCTIONS TO HANDLE: The LIGHTING property sheet page
\******************************************************************************/

// IDL_ST01 = Lighting Tip
// IDL_ST02 = Photoperiod
// IDL_ST03 = Plant height guage
// IDL_CM01 = Switch ON combobox
// IDL_CM02 = Switch OFF combobox
// IDL_SL01 = Lamp height slider

BOOL bLightAdjusted=FALSE;

void ACInitLightDialog(HWND hDlg, HINSTANCE hInst, BOOL bUpdate)
    {
    // This is called when the dialog is first shown and everytime we get a
    // PSN_QUERYSIBLINGS message after he's clicked the APPLY button.
    // If this is the first showing, we must do everything (bUpdate = FALSE),
    // but if we must apply new attributes, we show only those that change.
    int  i, j;

    if(bUpdate==FALSE)
        {
        // *** SET THE LIGHT SLIDER RANGE ***
        SendDlgItemMessage(hDlg, IDL_SL01, TBM_SETRANGE, 0, MAKELONG(0, 20));
        // *** RESET AND FILL THE CLOCK COMBOBOXES ***
        SendDlgItemMessage(hDlg, IDL_CM01, CB_RESETCONTENT, 0, 0);
        SendDlgItemMessage(hDlg, IDL_CM02, CB_RESETCONTENT, 0, 0);
        for(i=0; i<24; i++)
            {
            for(j=0; j<60; j+=15)
                {
                sprintf(strbuf, "%02i h %02i ", i, j);
                SendDlgItemMessage(hDlg, IDL_CM01, CB_ADDSTRING, 0, (LPARAM)strbuf);
                SendDlgItemMessage(hDlg, IDL_CM02, CB_ADDSTRING, 0, (LPARAM)strbuf);
                }
            }
        }
    // *** CURRENT CLOCK SETTING
    SendDlgItemMessage(hDlg, IDL_CM01, CB_SETCURSEL, GD_CurPlant.cLightOn, 0);
    SendDlgItemMessage(hDlg, IDL_CM02, CB_SETCURSEL, GD_CurPlant.cLightOff, 0);
    // *** PHOTOPERIOD ***
    i = GLCurPlantPhotoperiod();
    sprintf(strbuf, "%02i hrs %02i min", i/4, (i-4*(i/4))*15);
    SendDlgItemMessage(hDlg, IDL_ST02, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** LIGHT HEIGHT ***
    SendDlgItemMessage(hDlg, IDL_SL01, TBM_SETPOS, TRUE, 
                       (LPARAM)(20-GD_CurPlant.cLightHeight));    
    // *** LIGHTING TIP ***
    i = giGrowDay/7;
    if(i>=33) i = Random(0, 32);
    LoadString(hInst, IDS_LIGHT_001+i, strbuf, 200); 
    SendDlgItemMessage(hDlg, IDL_ST01, WM_SETTEXT, 0, (LPARAM)strbuf);
    }


void ACUpdatePhotoperiod(HWND hDlg, WPARAM wParam)
    {
    int  i;

    if(HIWORD(wParam)!=CBN_SELCHANGE) return;

    if(LOWORD(wParam)==IDL_CM01)
         GD_CurPlant.cLightOn  =(char)SendDlgItemMessage(hDlg, IDL_CM01, CB_GETCURSEL, 0, 0);
    else GD_CurPlant.cLightOff =(char)SendDlgItemMessage(hDlg, IDL_CM02, CB_GETCURSEL, 0, 0);
    // now we calculate and display the photoperiod for this plant
    i = GLCurPlantPhotoperiod();
    sprintf(strbuf, "%02i hrs %02i min", i/4, (i-4*(i/4))*15);
    SendDlgItemMessage(hDlg, IDL_ST02, WM_SETTEXT, 0, (LPARAM)strbuf);
    // now wake up the APPLY button
    PropSheet_Changed(GetParent(hDlg), hDlg);
    bLightAdjusted = TRUE;
    }

void ACUpdateLightHeight(HWND hDlg)
    {
    int iCurHeight = (char)(20-SendDlgItemMessage(hDlg, IDL_SL01, TBM_GETPOS, 0, 0));
    // now we must ensure that the light is never lower than the plant
    GD_CurPlant.cLightHeight=max(iCurHeight, (GD_CurPlant.iHeight/125)+1);
    PropSheet_Changed(GetParent(hDlg), hDlg);
    bLightAdjusted = TRUE;
    }

void ACApplyLightDialog(HWND hDlg)
    {
    if(bLightAdjusted)
        {
        ACActivitySaveGrowDay(hDlg);
        GRReRenderGrowRoom();
        bLightAdjusted = FALSE;
        }
    }


BOOL CALLBACK ACLightDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const  HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const  HWND hParent     = GetParent(hDlg);
   static HBRUSH hGreenBrush;
   static BOOL bLightUpdate;  // set when we must update our page 

    switch (message)
      {
      case WM_INITDIALOG:
           DlgCenter(hParent);
           hGreenBrush = CreateSolidBrush(RGB(0,128,0));
           ACInitLightDialog(hDlg, dhInst, FALSE);
      return (TRUE);

      case PSM_QUERYSIBLINGS:
           bLightUpdate = TRUE;
      return (TRUE);

      case WM_DESTROY:
           DeleteObject(hGreenBrush);
      return (TRUE);

      case WM_CTLCOLORSTATIC:
          if((HWND)lParam==GetDlgItem(hDlg, IDL_ST03))
              GLDisplayProgress(hDlg, IDL_ST03, hGreenBrush, GD_CurPlant.iHeight, 2500);
      return (FALSE);

      case WM_NOTIFY:
          switch (((LPNMHDR)lParam)->code)
             {
             case PSN_APPLY:     
                  ACApplyLightDialog(hDlg); 
             return PSNRET_NOERROR;

             case PSN_SETACTIVE: 
                  if(bLightUpdate)
                      {
                      ACInitLightDialog(hDlg, dhInst, TRUE);
                      bLightUpdate = FALSE;
                      }
             return PSNRET_NOERROR;
             }
            return (FALSE);
        
      case WM_VSCROLL:
            ACUpdateLightHeight(hDlg);
      return FALSE;

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDL_CM01:
            case IDL_CM02:
                 ACUpdatePhotoperiod(hDlg, wParam);
            return FALSE;
            }  
          return FALSE;
      }
    return (FALSE);  
   }                          
   

/******************************************************************************\
*  FUNCTIONS TO HANDLE: The WATERING property sheet page
\******************************************************************************/

// IDW_ST01 = Watering Tip
// IDW_ST02 = Last Watered
// IDW_ST03 = Current Moisture Level
// IDW_ST04 = Moisture Level guage
// IDW_ST05 = Water to Apply Now
// IDW_ST06 = Nutrients Added Today
// IDW_SL01 = Water Amount slider

int iTempMoisture=0;

void ACInitWaterDialog(HWND hDlg, HINSTANCE hInst, BOOL bUpdate)
    {
    // This is called when the dialog is first shown and everytime we get a
    // PSN_QUERYSIBLINGS message after he's clicked the APPLY button.
    // If this is the first showing, we must do everything (bUpdate = FALSE),
    // but if we must apply new attributes, we show only those that change.
    // *** FIRST DO THE ONCE ONLY STUFF ***
    int i, j=0;

    if(bUpdate==FALSE)
        // *** SET THE WATER SLIDER RANGE ***
        SendDlgItemMessage(hDlg, IDW_SL01, TBM_SETRANGE, 0, MAKELONG(0, 20));
    // *** NOW DO THE ALWAYS STUFF ***
    // *** LAST WATERED ***
    if(giLastWatered)
         GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&strbuf, giLastWatered-1);
    else strcpy(strbuf, "Unwatered");
    SendDlgItemMessage(hDlg, IDW_ST02, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** MOISTURE IN ML ***
    sprintf(strbuf, "%i ml", GD_CurPlant.iMoisture);
    SendDlgItemMessage(hDlg, IDW_ST03, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** INIT THE DAY'S AMOUNT TO ZERO ***
    sprintf(strbuf, "%i ml", 0);
    SendDlgItemMessage(hDlg, IDW_ST05, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** RESET THE WATER SLIDER TO ZERO ***
    SendDlgItemMessage(hDlg, IDW_SL01, TBM_SETPOS, TRUE, 20);
    // *** SHOW NUTRIENTS ADDED TODAY ***
    for(i=0; i<4; i++) j=j+GD_CurPlant.szNutrients[i];
    sprintf(strbuf, "%1.1f g", j/10.0);
    SendDlgItemMessage(hDlg, IDW_ST06, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** WATERING TIP ***
    i = giGrowDay/7;
    if(i>=33) i = Random(0, 32);
    LoadString(hInst, IDS_WATER_001+i, strbuf, 200); 
    SendDlgItemMessage(hDlg, IDW_ST01, WM_SETTEXT, 0, (LPARAM)strbuf);
    }


void ACUpdateWaterAmount(HWND hDlg)
    {
    int i=SendDlgItemMessage(hDlg, IDW_SL01, TBM_GETPOS, 0, 0);
    iTempMoisture = 20-i;
    sprintf(strbuf, "%i ml", (20-i)*50);
    SendDlgItemMessage(hDlg, IDW_ST05, WM_SETTEXT, 0, (LPARAM)strbuf);
    PropSheet_Changed(GetParent(hDlg), hDlg);
    }


void ACApplyWaterDialog(HWND hDlg)
    {
    if(iTempMoisture)
        {
        // *** UPDATE THE MOISTURE LEVEL
        GD_CurPlant.iMoisture += iTempMoisture*50;
        GD_CurPlant.iMoisture  = min(GD_CurPlant.iMoisture, 2000);
        // *** AND REFRESH THE GUAGE ***
        InvalidateRect(GetDlgItem(hDlg, IDW_ST04), NULL, TRUE);
        // *** UPDATE THE LAST WATERED GLOBAL ***
        giLastWatered = giGrowDay;
        GD_CurPlant.bWatered = TRUE;
        // *** AND SEND THE UPDATE MESSAGE ***
        ACActivitySaveGrowDay(hDlg);
        iTempMoisture = 0;
        }
    }



BOOL CALLBACK ACWaterDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   static HBRUSH hBlueBrush;
   static BOOL bWaterUpdate;  // set when we must update our page 
   
    switch (message)
      {
      case WM_INITDIALOG:
           DlgCenter(hParent);
           hBlueBrush = CreateSolidBrush(RGB(0,0,128));
           ACInitWaterDialog(hDlg, dhInst, FALSE);
      return (TRUE);

      case PSM_QUERYSIBLINGS:
           bWaterUpdate = TRUE;
      return (TRUE);

      case WM_DESTROY:
           DeleteObject(hBlueBrush);
      return (TRUE);

      case WM_NOTIFY:
          switch (((LPNMHDR)lParam)->code)
             {
             case PSN_APPLY:
                  ACApplyWaterDialog(hDlg);                    
             return PSNRET_NOERROR;

             case PSN_SETACTIVE: 
                  if(bWaterUpdate)
                      {
                      ACInitWaterDialog(hDlg, dhInst, TRUE);
                      bWaterUpdate = FALSE;
                      }
             return PSNRET_NOERROR;
             }
            return (FALSE);
        
      case WM_CTLCOLORSTATIC:
          if((HWND)lParam==GetDlgItem(hDlg, IDW_ST04))
              GLDisplayProgress(hDlg, IDW_ST04, hBlueBrush, 
                                GD_CurPlant.iMoisture, 1000);
      return (FALSE);

      case WM_VSCROLL:
             ACUpdateWaterAmount(hDlg);
      return FALSE;
      }
    return (FALSE);  
   }                          
   

/******************************************************************************\
*  FUNCTIONS TO HANDLE: The FERTILISING property sheet page
\******************************************************************************/

// IDF_ST01 = Fertilizing Tip
// IDF_ST02 = Last Fertilized
// IDF_ST03 = Soil pH balance 
// IDF_ST04 = Nitrogen (N) Mix Indicator
// IDF_ST05 = Phosphorus (P) Mix Indicator
// IDF_ST06 = Potassium (K) Mix Indicator
// IDF_ST07 = Calcium (Ca) Mix Indicator
// IDF_ST08 = Static fertilizer amount
// IDF_ST09 = Total Nutrients added today
// IDF_BT01 = Apply fertiliser to Water Now button
// IDF_SL01 = Nitrogen (N) Mix slider
// IDF_SL02 = Phosphorus (P) Mix slider
// IDF_SL03 = Potassium (K) Mix slider
// IDF_SL04 = Calcium (Ca) Mix slider

char szMixToday[4]="\0";

void ACUpdateSliderText(HWND hDlg, int IDstatic, int position)
    {
    sprintf(strbuf, "%1.1f", (position/10.0));
    SendDlgItemMessage(hDlg, IDstatic, WM_SETTEXT, 0, (LPARAM)strbuf);
    }

void ACUpdateFertilizerStatics(HWND hDlg)
    {
    int  i, j;
    // *** READ THE CURRENT SLIDER VALUES ***
    szMixToday[0]=(char)(15-SendDlgItemMessage(hDlg, IDF_SL01, TBM_GETPOS, 0, 0));
    szMixToday[1]=(char)(15-SendDlgItemMessage(hDlg, IDF_SL02, TBM_GETPOS, 0, 0));
    szMixToday[2]=(char)(15-SendDlgItemMessage(hDlg, IDF_SL03, TBM_GETPOS, 0, 0));
    szMixToday[3]=(char)(15-SendDlgItemMessage(hDlg, IDF_SL04, TBM_GETPOS, 0, 0));
    // *** SHOW NUTRIENTS TO ADD NOW ***
    j=0; // init this first
    for(i=0; i<4; i++) j=j+szMixToday[i];
    sprintf(strbuf, "%1.1f g", j/10.0);
    SendDlgItemMessage(hDlg, IDF_ST08, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** SHOW NUTRIENTS ADDED TODAY ***
    j=0; // init this first
    for(i=0; i<4; i++) j=j+GD_CurPlant.szNutrients[i];
    sprintf(strbuf, "%1.1f g", j/10.0);
    SendDlgItemMessage(hDlg, IDF_ST09, WM_SETTEXT, 0, (LPARAM)strbuf);
    }


void ACInitFertilizerDialog(HWND hDlg, HINSTANCE hInst, BOOL bUpdate)
    {
    // This is called when the dialog is first shown and everytime we get a
    // PSN_QUERYSIBLINGS message after he's clicked the APPLY button.
    // If this is the first showing, we must do everything (bUpdate = FALSE),
    // but if we must apply new attributes, we show only those that change.
    // *** LAST FERTILISED ***
    int i;

    if(giLastFertilized)
         GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&strbuf, giLastFertilized-1);
    else strcpy(strbuf, "Unfertilized");
    SendDlgItemMessage(hDlg, IDF_ST02, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** SET THE PH AND SLIDER RANGE ***
    if(bUpdate==FALSE)
        {
        for(i=0; i<4; i++) 
            szMixToday[i] = GD_CurPlant.szNutrients[i];
        sprintf(strbuf, "%1.1f", (GD_CurPlant.cSoilPH/10.0));
        SendDlgItemMessage(hDlg, IDF_ST03, WM_SETTEXT, 0, (LPARAM)strbuf);
        SendDlgItemMessage(hDlg, IDF_SL01, TBM_SETRANGE, 0, MAKELONG(0, 15));
        SendDlgItemMessage(hDlg, IDF_SL02, TBM_SETRANGE, 0, MAKELONG(0, 15));
        SendDlgItemMessage(hDlg, IDF_SL03, TBM_SETRANGE, 0, MAKELONG(0, 15));
        SendDlgItemMessage(hDlg, IDF_SL04, TBM_SETRANGE, 0, MAKELONG(0, 15));
        }
    // *** RESET SLIDERS TO ZERO ***
    SendDlgItemMessage(hDlg, IDF_SL01, TBM_SETPOS, TRUE, 15);    
    SendDlgItemMessage(hDlg, IDF_SL02, TBM_SETPOS, TRUE, 15);    
    SendDlgItemMessage(hDlg, IDF_SL03, TBM_SETPOS, TRUE, 15);    
    SendDlgItemMessage(hDlg, IDF_SL04, TBM_SETPOS, TRUE, 15);    
    // *** AND SHOW THE SLIDER TEXT ***
    ACUpdateSliderText(hDlg, IDF_ST04, 0);
    ACUpdateSliderText(hDlg, IDF_ST05, 0);
    ACUpdateSliderText(hDlg, IDF_ST06, 0);
    ACUpdateSliderText(hDlg, IDF_ST07, 0);
    // *** SHOW THE NEW TOTALS ***
    ACUpdateFertilizerStatics(hDlg);
    // *** FERTILIZING TIP ***
    i = giGrowDay/7;
    if(i>=33) i = Random(0, 32);
    LoadString(hInst, IDS_FERT_001+i, strbuf, 200); 
    SendDlgItemMessage(hDlg, IDF_ST01, WM_SETTEXT, 0, (LPARAM)strbuf);
    }


void ACUpdateFertilizerMix(HWND hDlg, LPARAM lParam)
    {
    int i=(15-SendDlgItemMessage(hDlg, GetDlgCtrlID((HWND)lParam), TBM_GETPOS, 0, 0));

    switch (GetDlgCtrlID((HWND)lParam))
      {
      case IDF_SL01:
           ACUpdateSliderText(hDlg, IDF_ST04, i);
           ACUpdateFertilizerStatics(hDlg);
      break;
      case IDF_SL02:
           ACUpdateSliderText(hDlg, IDF_ST05, i);
           ACUpdateFertilizerStatics(hDlg);
      break;
      case IDF_SL03:
           ACUpdateSliderText(hDlg, IDF_ST06, i);
           ACUpdateFertilizerStatics(hDlg);
      break;
      case IDF_SL04:
           ACUpdateSliderText(hDlg, IDF_ST07, i);
           ACUpdateFertilizerStatics(hDlg);
      break;
      }
    PropSheet_Changed(GetParent(hDlg), hDlg);
    }


void ACApplyFertilizerDialog(HWND hDlg)
    {
    int  i, j=0;

    // *** ADD NUTRIENTS TO TODAY'S TOTAL ***
    for(i=0; i<4; i++) 
        GD_CurPlant.szNutrients[i]=min((szMixToday[i]+GD_CurPlant.szNutrients[i]), 25);

    // *** UPDATE THE LAST FERTILIZED GLOBAL ***
    for(i=0; i<4; i++)  
        j += szMixToday[i];
    if(j>0) 
        {
        giLastFertilized = giGrowDay;
        GD_CurPlant.bFertilized = TRUE;
        // *** AND APPLY ALL THE CHANGES ***
        ACActivitySaveGrowDay(hDlg);
        }
    }


BOOL CALLBACK ACFertDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   static BOOL bFertUpdate;  // set when we must update our page 
   
    switch (message)
      {
      case WM_INITDIALOG:
           DlgCenter(hParent);
           ACInitFertilizerDialog(hDlg, dhInst, FALSE);
      return (TRUE);

      case PSM_QUERYSIBLINGS:
           bFertUpdate = TRUE;
      return (TRUE);

      case WM_NOTIFY:
          switch (((LPNMHDR)lParam)->code)
             {
             case PSN_APPLY:
                  ACApplyFertilizerDialog(hDlg);                    
             return PSNRET_NOERROR;

             case PSN_SETACTIVE: 
                  if(bFertUpdate)
                      {
                      ACInitFertilizerDialog(hDlg, dhInst, TRUE);
                      bFertUpdate = FALSE;
                      }
             return PSNRET_NOERROR;
             }
            return (FALSE);
        
      case WM_VSCROLL:
             ACUpdateFertilizerMix(hDlg, lParam);
      return FALSE;
      }
    return (FALSE);  
   }                          
   

/******************************************************************************\
*  FUNCTIONS TO HANDLE: The PRUNING property sheet page
\******************************************************************************/

// IDP_ST01 = Pruning Tip
// IDP_ST02 = Date Last Pruned
// IDP_ST03 = Available growing tips
// IDP_ST04 = Amount to Prune now static
// IDP_ST05 = Amount Pruned today static
// IDP_BT01 = Prune Growing Tips Now button
// IDP_SL01 = Number of tips to prune slider

void ACInitPruneDialog(HWND hDlg, HINSTANCE hInst)
    {
    // This is called when the dialog is first shown and everytime we get a
    // PSN_QUERYSIBLINGS message after he's clicked the APPLY button.
    // If this is the first showing, we must do everything (bUpdate = FALSE),
    // but if we must apply new attributes, we show only those that change.
    // *** LAST PRUNED ***
    int i;
    // always zero the tips pruned global when we initialise
    giPrunedTips = 0;
    if(giLastPruned)
         GLNextFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&strbuf, giLastPruned-1);
    else strcpy(strbuf, "Unpruned");
    SendDlgItemMessage(hDlg, IDP_ST02, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** AVAILABLE GROWING TIPS ***
    GLSetNumText(giGrowingTips, hDlg, IDP_ST03);
    // *** SET PRUNE SLIDER RANGE ***
    SendDlgItemMessage(hDlg, IDP_SL01, TBM_SETRANGE, 0, MAKELONG(0, min(50, giGrowingTips)));
    // *** RESET PRUNE SLIDER TO ZERO ***
    SendDlgItemMessage(hDlg, IDP_SL01, TBM_SETPOS, TRUE, giGrowingTips);
    // *** UPDATE PRUNED TODAY STATIC ***
    sprintf(strbuf, "%i", GD_CurPlant.cTipsPruned);
    SendDlgItemMessage(hDlg, IDP_ST05, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** RESET PRUNE NOW STATIC ***
    sprintf(strbuf, "%i", 0);
    SendDlgItemMessage(hDlg, IDP_ST04, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** PRUNING TIP ***
    i = giGrowDay/7;
    if(i>=33) i = Random(0, 32);
    LoadString(hInst, IDS_PRUNE_001+i, strbuf, 200); 
    SendDlgItemMessage(hDlg, IDP_ST01, WM_SETTEXT, 0, (LPARAM)strbuf);
    }


void ACUpdatePruneStatic(HWND hDlg)
    {
    int i = SendDlgItemMessage(hDlg, IDP_SL01, TBM_GETPOS, 0, 0);

    giPrunedTips = min(50, giGrowingTips-i);
    sprintf(strbuf, "%i", (giPrunedTips));
    SendDlgItemMessage(hDlg, IDP_ST04, WM_SETTEXT, 0, (LPARAM)strbuf);
    sprintf(strbuf, "%i", GD_CurPlant.cTipsPruned);
    SendDlgItemMessage(hDlg, IDP_ST05, WM_SETTEXT, 0, (LPARAM)strbuf);
    PropSheet_Changed(GetParent(hDlg), hDlg);
    if(giPrunedTips) // if he's set any tips to prune...
        { // we set our global variable for pruning when saving
        gbPrunedNow = TRUE;
        }
    }

 
void ACApplyPruningDialog(HWND hDlg)
    {
    if(giPrunedTips)
        {
        giLastPruned = giGrowDay;
        GD_CurPlant.cTipsPruned += giPrunedTips;
        giGrowingTips           -= giPrunedTips;
        // *** AND APPLY ALL THE CHANGES ***
        ACActivitySaveGrowDay(hDlg);
        // re-render and redraw the growroom and plants
        GRReRenderGrowRoom();
        // now we reset our global to prevent more applies adjusting
        // our GD_CurPlant.cTipsPruned struct element
        giPrunedTips = 0;
        }
    }


BOOL CALLBACK ACPruneDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   static BOOL bPruneUpdate;  // set when we must update our page 
   
    switch (message)
      {
      case WM_INITDIALOG:
           DlgCenter(hParent);
           ACInitPruneDialog(hDlg, dhInst);
      return (TRUE);

      case PSM_QUERYSIBLINGS:
           bPruneUpdate = TRUE;
      return (TRUE);

      case WM_NOTIFY:
          switch (((LPNMHDR)lParam)->code)
             {
             case PSN_APPLY:
                  ACApplyPruningDialog(hDlg);                    
             return PSNRET_NOERROR;

             case PSN_SETACTIVE: 
                  if(bPruneUpdate)
                      {
                      ACInitPruneDialog(hDlg, dhInst);
                      bPruneUpdate = FALSE;
                      }
             return PSNRET_NOERROR;
             }
            return (FALSE);
        
      case WM_VSCROLL:
             ACUpdatePruneStatic(hDlg);
      return FALSE;
      }
    return (FALSE);  
   }                          
   


/******************************************************************************\
*  FUNCTION:    ShowActivityPropertySheet (handles windows command messages)
\******************************************************************************/


int ACShowActivitySheet(HWND hWndParent, HINSTANCE hInstance, int iPlant, int iSheet)
    {
	int i;
    char strbuff[100]="\0";
    char strtype[50] ="\0";
	PROPSHEETPAGE    psActivityPage[5];
	PROPSHEETHEADER  psActivityHeader;

    ACActivityInitGrowInfo(iPlant);

    LoadString(hInstance, (PI_CurPlant.cSeedChoice+IDS_BT01-1), strtype, 50);
    sprintf(strbuff, "%s - %s - Day %i", PI_CurPlant.szPlantName, strtype, giGrowDay);

    for(i=0; i<=4; i++)
	   {
	   psActivityPage[i].dwSize      = sizeof(PROPSHEETPAGE);
	   psActivityPage[i].dwFlags	 = PSP_USEICONID|PSP_USETITLE;
	   psActivityPage[i].hInstance	 = hInstance;
	   psActivityPage[i].pszIcon	 = "HighGrow";
	   psActivityPage[i].lParam		 = 0;
	   }

//    psActivityPage[0].pszTemplate = "T_Temp";
//    psActivityPage[0].pszTitle	  = "Calculated";
//    psActivityPage[0].pszIcon	  = "HighGrow";
//    psActivityPage[0].pfnDlgProc  = ACTempDlgProc;
    
    psActivityPage[0].pszTemplate = "T_Stats";
    psActivityPage[0].pszTitle	  = "Statistics";
    psActivityPage[0].pszIcon	  = "Stats";
    psActivityPage[0].pfnDlgProc  = ACStatsDlgProc;
    
    psActivityPage[1].pszTemplate = "T_Light";
    psActivityPage[1].pszTitle	  = "Lighting";
    psActivityPage[1].pszIcon	  = "Light";
    psActivityPage[1].pfnDlgProc  = ACLightDlgProc;
                   
    psActivityPage[2].pszTemplate = "T_Water";
    psActivityPage[2].pszTitle	  = "Watering";
    psActivityPage[2].pszIcon	  = "Water";
    psActivityPage[2].pfnDlgProc  = ACWaterDlgProc;

    psActivityPage[3].pszTemplate = "T_Fertil";
    psActivityPage[3].pszTitle	  = "Fertilizing";
    psActivityPage[3].pszIcon	  = "Fertil";
    psActivityPage[3].pfnDlgProc  = ACFertDlgProc;

    psActivityPage[4].pszTemplate = "T_Pruning";
    psActivityPage[4].pszTitle	  = "Pruning";
    psActivityPage[4].pszIcon	  = "Prune";
    psActivityPage[4].pfnDlgProc  = ACPruneDlgProc;

	psActivityHeader.dwSize       = sizeof(PROPSHEETHEADER);
	psActivityHeader.dwFlags      = PSH_USEICONID|PSH_PROPSHEETPAGE;
	psActivityHeader.hwndParent   = hWndParent;
	psActivityHeader.hInstance    = hInstance;
	psActivityHeader.pszIcon      = "HighGrow";
    psActivityHeader.nStartPage   = iSheet;
	psActivityHeader.pszCaption   = strbuff;
	psActivityHeader.nPages       = sizeof(psActivityPage)/sizeof(PROPSHEETPAGE);
	psActivityHeader.ppsp         = (LPCPROPSHEETPAGE) &psActivityPage;

	return PropertySheet(&psActivityHeader);
    }

