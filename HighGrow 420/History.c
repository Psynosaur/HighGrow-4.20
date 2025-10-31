/******************************************************************************\
*
*  PROGRAM:     History.c
*
*  PURPOSE:     Showing Plant History in a ListView control
*
\******************************************************************************/
  
#include <windows.h>
#include <commctrl.h>
#include "stdio.h"
#include "resource.h"
#include "global.h"
#include "history.h"
#include "calc.h"
#include "plantmem.h"
#include "seedlist.h"
#include "toolbar.h"
#include "dibitmap.h"
#include "logexport.h"
      
/******************************************************************************\
*  FUNCTION TO: Create ListView and attach icon images
\******************************************************************************/

#define ID_LISTVIEW  1 // id of the listview window
#define NUM_COLUMNS 10 // number of listview columns
#define MAX_ITEMLEN 64 // maximum bytes of text

HIMAGELIST hSmall;     // Handles to image lists for small icons.

/******************************************************************************\
*  FUNCTION TO: Handle the WM_NOTIFY message
\******************************************************************************/


LRESULT HINotifyHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
    LV_DISPINFO *pLvdi = (LV_DISPINFO *)lParam;
    NM_LISTVIEW *pNm   = (NM_LISTVIEW *)lParam;
    PGROWDAY     pDay  = (PGROWDAY)(pLvdi->item.lParam);
    static char szText[10];

    if(wParam!=IDH_LV01)  return 0L;
    switch(pLvdi->hdr.code)
       {
       case LVN_GETDISPINFO:
          // Display the appropriate item, getting the text or number
          // from the HOUSEINFO structure I set up.
          switch(pLvdi->item.iSubItem)
             {
             case 0:     // visited?
                wsprintf(szText, "%i", (int)pLvdi->item.iItem+1);
                pLvdi->item.pszText = szText;
                break;

             case 1:     // Date
                GLNextShortFormatDate((char*)P_Plant->PI_Plant.szStartDate, 
                                      (char*)pLvdi->item.pszText,(int)pLvdi->item.iItem);
                break;

             case 2:     // Health
                if(pDay->cHealth)
                     wsprintf(szText, "%i", (int)pDay->cHealth);
                else szText[0]=0;
                pLvdi->item.pszText = szText;
                break;

             case 3:     // Height
                if(pDay->iHeight)
                     wsprintf(szText, "%i", pDay->iHeight);
                else szText[0]=0;
                pLvdi->item.pszText = szText;
                break;

             case 4:     // Mass
                if(pDay->iMass)
                     wsprintf(szText, "%i", pDay->iMass);
                else szText[0]=0;
                pLvdi->item.pszText = szText;
                break;

             case 5:     // Potency
                if(pDay->iPotency)
                     sprintf(szText, "%1.2f", (int)pDay->iPotency/100.00);
                else szText[0]=0;
                pLvdi->item.pszText = szText;
                break;

             case 6:     // Moisture
                if(pDay->iMoisture)
                     wsprintf(szText, "%i", (int)pDay->iMoisture);
                else szText[0]=0;
                pLvdi->item.pszText = szText;
                break;

             case 7:     // Fertilizer
                if(pDay->bFertilized)
                    {
                    int i,j=0;
                    for(i=0; i<4; i++) j=j+pDay->szNutrients[i];
                    sprintf(szText, "%1.1f", j/10.0);
                    pLvdi->item.pszText = szText;
                    }
                break;

             case 8:     // Soil pH
                if(pDay->cSoilPH)
                     sprintf(szText, "%1.1f", pDay->cSoilPH/10.0);
                else szText[0]=0;
                pLvdi->item.pszText = szText;
                break;

             case 9:     // Pruned
                if(pDay->cTipsPruned)
                     wsprintf(szText, "%i", (int)pDay->cTipsPruned);
                else szText[0]=0;
                    pLvdi->item.pszText = szText;
                break;

             default: break;
             }
             break;

       default: break;
       }
    return 0L;
    }


/******************************************************************************\
* RESIZES ALL THE CONTROLS IN THE HISTORY DIALOG
\******************************************************************************/

void HIFitHistoryDialog(HWND hdlg)
    {
    RECT rcDlg;
    int  iBWidth  = 130; // button width
    int  iBHeight = 22;  // button height
    int  iEdge    = 8;   // edge width
    int  i;

    // Resizes the dialog inside the HighGrow Program window. 
    GLSizeDialogToFit(hdlg);
    // let's read the size and pos of the dialog
    GetClientRect(hdlg, &rcDlg);
    // let's move the static window first
    MoveWindow(GetDlgItem(hdlg, IDH_LV01), iEdge, iEdge, 
                      rcDlg.right-rcDlg.left-(iEdge*2),
                      rcDlg.bottom-rcDlg.top-iBHeight-(iEdge*3), FALSE);
    // also move our radio button plant name controls
    for(i=0;i<3;i++)
        {
        MoveWindow(GetDlgItem(hdlg,IDC_RB01+i), 
                   (iEdge*2)+(i*iBWidth), rcDlg.bottom-iEdge-iBHeight, 
                   iBWidth, iBHeight, FALSE);
        }
    // also move our print and export buttons
    for(i=0;i<2;i++)
        {
        MoveWindow(GetDlgItem(hdlg,IDC_BT01+i), 
                   (iEdge*2)+((3*iBWidth)+(i*iBWidth/2)), 
                   rcDlg.bottom-iEdge-iBHeight, 
                   (iBWidth/2)-10, iBHeight, FALSE);
        }
    // now we move our cancel button
    MoveWindow(GetDlgItem(hdlg,IDCANCEL), rcDlg.right-iEdge-(iBWidth/2),
               rcDlg.bottom-iEdge-iBHeight, (iBWidth/2), iBHeight, FALSE);

    }


/******************************************************************************\
*  FILLING THE LISTVIEW WITH DATA
\******************************************************************************/

void HIFillListView(HWND hWndList)
    {
    LV_ITEM    lvI;        // List view item structure.
    int        index;      // Index used in for loops.
    int        iSubItem;   // Index for inserting subitems.

    // first we delete everything that already exists
    ListView_DeleteAllItems(hWndList);
    // Fill in the LV_ITEM structure for each of the items to add to the list.
    // The mask specifies that the .pszText, .iImage, .lParam and .state
    // members of the LV_ITEM structure are valid.
    lvI.mask  = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
    lvI.state = LVIS_SELECTED|LVIS_FOCUSED;
    lvI.stateMask = 0;  

    for(index=1; index<giGrowDay+1; index++)
        {
        lvI.iItem      = index;
        lvI.iSubItem   = 0;
        // The parent window is responsible for storing the text. The list view
        // window will send an LVN_GETDISPINFO when it needs the text to display.
        lvI.pszText    = LPSTR_TEXTCALLBACK; 
        lvI.cchTextMax = MAX_ITEMLEN;
        if(!P_Plant->GD_Plant[index].bVisited)
             lvI.iImage     = (int)hSmall;
        else lvI.iImage     = 0;
        lvI.lParam     = (LPARAM)&P_Plant->GD_Plant[index];
        if(ListView_InsertItem(hWndList, &lvI)==-1)  return;
        for(iSubItem=1; iSubItem<NUM_COLUMNS; iSubItem++)
           ListView_SetItemText(hWndList, index, iSubItem, LPSTR_TEXTCALLBACK);
        }
    ListView_EnsureVisible(hWndList, giGrowDay-1, 0);

    }


/******************************************************************************\
*  REFILLING THE LISTVIEW FOR A DIFFERENT PLANT
\******************************************************************************/


void HIRedoPlantHistory(HWND hDlg, int iPot)
    {
    // Here we'll recalculate and redisplay the history log for the
    // selected plant as called from the history dialog's radio button
    HWND hWndList = GetDlgItem(hDlg, IDH_LV01);
    int iRoomOfs  = (TBGetComboGrowroom()-1)*3;
    int iPlant    = iPot + iRoomOfs;

    if(giCurPlant != iPot)
        {
        // first unlock and free the existing plant's memory
        GlobalUnlock(hPlantMem);
        PMPlantMemoryFree();
        // now we set up our new plant's history
        giCurPlant = iPlant;
        // set the combo in our toolbar to show the chosen plant
        TBSetComboPlant(iPot-1);
        // here we load and calculate the new plant
        sprintf(strbuf, "Plant%02i.hgp", iPlant);
        if(GLDoesFileExist(strbuf))
            {
            if(PMPlantFileToMem(iPlant-1))
                { // first lock our global memory
                P_Plant = (PPLANT)GlobalLock(hPlantMem);
                CACalculatePlant(P_Plant, iPlant-1);
                HIFillListView(hWndList);
                }
            }
        }
    }

/******************************************************************************\
*  EXPORTING & PRINTING THE CURRENT PLANT'S HISTORY LOG DATA
\******************************************************************************/

int HIGetSelectedPlant(HWND hDlg)
	{
    if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_RB01, BM_GETCHECK, 0, 0))
         return 1;
    else {
         if(BST_CHECKED==SendDlgItemMessage(hDlg, IDC_RB02, BM_GETCHECK, 0, 0))
              return 2;
         else return 3;
         }
	}


void HIExportHistoryLog(HWND hDlg, HINSTANCE hInst)
    {
    char lpPlantName[100]="\0";
    int  iPlant = HIGetSelectedPlant(hDlg);

    SendDlgItemMessage(hDlg, IDC_RB01+iPlant-1, WM_GETTEXT, (WPARAM)100, (LPARAM)lpPlantName);  
    LEExportHistoryLog(hDlg, hInst, lpPlantName, iPlant);
    }


void HIPrintHistoryLog(HWND hDlg, HINSTANCE hInst)
	{
    char lpPlantName[100]="\0";
    int  iPlant = HIGetSelectedPlant(hDlg);
    SendDlgItemMessage(hDlg, IDC_RB01+iPlant-1, WM_GETTEXT, (WPARAM)100, (LPARAM)lpPlantName);  
	LEPrintHistoryLog(hDlg, hInst, lpPlantName, iPlant);
	}

/******************************************************************************\
*  HISTORY LIST DIALOG WINPROC
\******************************************************************************/

// IDH_LV01  - History ListView Control

void HIInitHistoryDialog(HWND hDlg, HINSTANCE hInst)
    {
    int        iWid;       // calculated width of one column
    HICON      hIcon;      // Handle to an icon.
    LV_COLUMN  lvC;        // List view column structure.
    char       szText[64]="\0"; // place to store some text.
    HWND       hWndList = GetDlgItem(hDlg, IDH_LV01);
//    COLORREF   crColor  = GetSysColor(COLOR_BTNFACE);
    COLORREF   crColor  = RGB(128,192,128);
    RECT       rcWin;
    char       szWidths[NUM_COLUMNS];
    char       szBuff[100]="\0";
    int        i;      // Index used in for loops.
    int        iRoomOfs = (TBGetComboGrowroom()-1)*3;

    for(i=0;i<4;i++)
        {
        if(PI_PlantSeed[i+iRoomOfs].szPlantName[0]==0)
            EnableWindow(GetDlgItem(hDlg, IDC_RB01+i), FALSE);
        else
            {
            sprintf(szBuff, "%s", PI_PlantSeed[i+iRoomOfs].szPlantName);
            SendDlgItemMessage(hDlg, IDC_RB01+i, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)szBuff);
            }
        }
    SendDlgItemMessage(hDlg, IDC_RB01+giCurPlant-1-iRoomOfs, BM_SETCHECK, TRUE, 0);

    // Ensure that the common window DLL is loaded.
    InitCommonControls();
    // set the background color
    SendMessage(hWndList, LVM_SETBKCOLOR,     0, (LPARAM)crColor);
    SendMessage(hWndList, LVM_SETTEXTBKCOLOR, 0, (LPARAM)crColor);

    // now we fill our widths array
    szWidths[0] = 45;
    szWidths[1] = 70;
    szWidths[2] = 50;
    GetClientRect(hWndList, &rcWin);
    iWid = ((rcWin.right-rcWin.left)-112)/(NUM_COLUMNS-2);
    for(i=3; i<NUM_COLUMNS; i++)
         szWidths[i]=iWid;
    // now reduce the last column by 5 pixels
    szWidths[NUM_COLUMNS-1]-=8;
    // Initialize the list view window.
    // First, initialize the image lists we will need
    // to create an image list for the small icons.
    hSmall = ImageList_Create(16, 16, FALSE, 1, 0 );
    // Load the icon and add it to the image list.
    hIcon = LoadImage(hInst, "HighGrow", IMAGE_ICON, 16, 16, 
                      LR_LOADMAP3DCOLORS|LR_LOADTRANSPARENT);

    // add our icon
    ImageList_AddIcon(hSmall, hIcon);
    // Make sure that all of the icons are in the lists.
    if(ImageList_GetImageCount(hSmall) < 1)  return;
    // Associate lists with list view window.
    ListView_SetImageList(hWndList, hSmall, LVSIL_SMALL);

    // Now initialize the columns we will need.
    // Initialize the LV_COLUMN structure.
    // The mask specifies that the .fmt, .ex, width, and .subitem members 
    // of the structure are valid.
    lvC.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvC.fmt     = LVCFMT_CENTER; // center the column text.
    lvC.pszText = szText;      // The text for the column.

    for (i=0; i<NUM_COLUMNS; i++)
        {
        lvC.iSubItem = i;
        lvC.cx       = szWidths[i]; // Width of the column, in pixels.
        LoadString( hInst, IDH_ST01 + i, szText, sizeof(szText));
        if (ListView_InsertColumn(hWndList, i, &lvC) == -1) return;
        }

    // Finally, let's add the actual items to the window.
    HIFillListView(hWndList);
    }

  
BOOL CALLBACK HIHistoryDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   
    switch (message)
      {
      case WM_INITDIALOG:
           HIFitHistoryDialog(hDlg);
           HIInitHistoryDialog(hDlg, dhInst);
      return (TRUE);

      case WM_ERASEBKGND:
      return DITileDlgLeafBitmap(hDlg, (HDC)wParam);

      case WM_CTLCOLORSTATIC:
          {
          int i= GetDlgCtrlID((HWND)lParam);
          if((i==IDC_RB01)||(i==IDC_RB02)||(i==IDC_RB03))
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_NOTIFY:
           HINotifyHandler(hDlg, message, wParam, lParam);
      break;

      case WM_COMMAND:
          switch (wParam)
            {
            case IDC_RB01: HIRedoPlantHistory(hDlg, 1);      return (FALSE);
            case IDC_RB02: HIRedoPlantHistory(hDlg, 2);      return (FALSE);
            case IDC_RB03: HIRedoPlantHistory(hDlg, 3);      return (FALSE);
            case IDC_BT01: HIExportHistoryLog(hDlg, dhInst); return (FALSE);
            case IDC_BT02: HIPrintHistoryLog(hDlg, dhInst);  return (FALSE);
            case IDCANCEL: EndDialog(hDlg, FALSE);           return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          
   

void HIShowHistoryDialog(HWND hwnd, HINSTANCE hInst, int iPlant)
    { // show the option based 0 plant
    char szfile[50] = "\0";
    sprintf(szfile, "Plant%02i.hgp", iPlant);
    if(GLDoesFileExist(szfile))
        {
        if(PMPlantFileToMem(iPlant-1))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            CACalculatePlant(P_Plant, iPlant-1);
            DialogBox(hInst, "History", hwnd, HIHistoryDlgProc);
            GlobalUnlock(hPlantMem);
            PMPlantMemoryFree();
            }
        }
    }
