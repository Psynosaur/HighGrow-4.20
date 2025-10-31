
#include <windows.h>
#include <string.h>
#include <shellapi.h>
#include "resource.h"
#include "stdio.h"
#include "Global.h"
#include "plantmem.h"
#include "plantlog.h"
#include "dibitmap.h"  // for handling 256 color bitmaps
#include "calc.h"
#include "registry.h"
#include "password.h"
#include "toolbar.h"
#include "seedimp.h"
#include "seedlog.h"
   

/****************************************************************************\
* ABOUT DIALOG
\****************************************************************************/
   
HFONT    hAboutBigFont;
HFONT    hAboutMedFont;
HBITMAP  hSlickBitmap;
HBITMAP  hSlickPalette;
HCURSOR  hLinkCursor;

void HAInitAboutDialog(HWND hDlg, HWND hParent, HINSTANCE dhInst)
    {
    long lp=MAKELPARAM(TRUE, 0);
    // center dialog in window (only if the parent is a valid window)
    if(hParent) DlgCenter(hDlg);
    // load our URL link cursor
    hLinkCursor  = LoadCursor(dhInst, "linkhand");
    // load our bud imagelist (MUST USE THIS METHOD FOR 256 COLOUR BITMAPS)
    hSlickBitmap = DILoadResourceBitmap(dhInst, (LPSTR)"Slick", (HPALETTE FAR*)&hSlickPalette);
    // now we'll create some bold fonts
    hAboutBigFont = GLCreateDialogFont(24,0,FW_BOLD);
    hAboutMedFont = GLCreateDialogFont(18,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hAboutBigFont)
        {
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hAboutBigFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST09,WM_SETFONT,(WPARAM)hAboutBigFont,lp);
        }
    // if we created the seed text font, set it to all the heading controls
    if(hAboutMedFont) 
        {
        SendDlgItemMessage(hDlg,IDC_ST03,WM_SETFONT,(WPARAM)hAboutMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST04,WM_SETFONT,(WPARAM)hAboutMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST10,WM_SETFONT,(WPARAM)hAboutMedFont,lp);
        }
    // change some things if we're registered
    if(gbRegistered)
        {
        SendDlgItemMessage(hDlg,IDC_ST03,WM_SETTEXT, 0, (LPARAM)"Freeware Version 4.20");
        SendDlgItemMessage(hDlg,IDC_BT01,WM_SETTEXT, 0, (LPARAM)"&OK");
        }
    }


void HADestroyAboutDialog(HWND hDlg)
    {
    if(hAboutBigFont) DeleteObject(hAboutBigFont);
    if(hAboutMedFont) DeleteObject(hAboutMedFont);
    if(hLinkCursor)   DestroyCursor(hLinkCursor);
    DIFreeBitmap(hSlickBitmap, (HPALETTE)hSlickPalette);
    }

  
BOOL CALLBACK HAAboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   const HCURSOR hArrow   = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
   static BOOL bMouseDown = FALSE;  

    switch (message)
      {
      case WM_INITDIALOG:
           HAInitAboutDialog(hDlg, hParent, dhInst);
      return (TRUE);

      case WM_DESTROY:
           HADestroyAboutDialog(hDlg);
      return (TRUE);

      case WM_MOUSEMOVE:
           {
           RECT  rc;
           POINT pt;
           GetWindowRect(GetDlgItem(hDlg, IDC_ST05), &rc);
           pt.x = LOWORD(lParam);
           pt.y = HIWORD(lParam);
           ClientToScreen(hDlg, &pt);
           if(PtInRect(&rc, pt))
                SetClassLong(hDlg, GCL_HCURSOR, (LONG)hLinkCursor);
           else SetClassLong(hDlg, GCL_HCURSOR, (LONG)hArrow);
           }
      return (FALSE);

      case WM_LBUTTONDOWN:
           {
           char    szURL[100]="\0";
           HCURSOR hCurCursor = (HCURSOR)GetClassLong(hDlg, GCL_HCURSOR);
           HWND    hStatic    = GetDlgItem(hDlg, IDC_ST05);
           if(hCurCursor == hLinkCursor)
               {
               bMouseDown = TRUE;
               InvalidateRect(hStatic, NULL, FALSE);
               UpdateWindow(hStatic);
               SendMessage(hStatic, WM_GETTEXT, 100, (LPARAM)szURL);
               ShellExecute(hDlg, "open", szURL, NULL, NULL, SW_SHOWDEFAULT);
               bMouseDown = FALSE;
               InvalidateRect(GetDlgItem(hDlg, IDC_ST05), NULL, FALSE);
               UpdateWindow(hStatic);
               }
           }
      return (FALSE);

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
          if(i==IDC_ST01)
              {
              RECT rc;
              DIDrawImage((HDC)wParam, 0, 0, 0, hSlickBitmap, (HPALETTE)hSlickPalette);
              SetBkMode((HDC)wParam, TRANSPARENT);
              SetTextColor((HDC)wParam,RGB(255,255,255));
              SelectObject((HDC)wParam,hAboutBigFont);
              GetClientRect((HWND)lParam, &rc);
              rc.left   += 15;
              rc.bottom -= 8;
              DrawText((HDC)wParam, "Slick", 5, &rc, DT_BOTTOM|DT_LEFT|DT_SINGLELINE);
              }
          else
              {
              if(i==IDC_ST05)
                  {
                  if(bMouseDown)
                       SetTextColor((HDC)wParam, RGB(255,0,0));
                  else SetTextColor((HDC)wParam, RGB(0,0,255));
                  }
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
       return (FALSE);

      case WM_COMMAND:
          switch (wParam)
            {
            case IDC_BT01:
                if(gbRegistered)
                     {
                     EndDialog(hDlg, TRUE); 
                     return TRUE;
                     }
            return FALSE;

            case IDCANCEL:
              EndDialog(hDlg, TRUE); 
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE);               /* Didn't process a message    */
   }                          


void HAShowAboutDialog(HWND hwnd, HINSTANCE hInst)
    {
    DialogBox(hInst, "About", hwnd, HAAboutDlgProc);
    }


void HAUnavailableDialog(HWND hwnd, HINSTANCE hInst)
    {
    DialogBox(hInst, "ShareWare", hwnd, HAAboutDlgProc);
    }

/*===============================*/
/* The Abort Procudure           */
/* ==============================*/

BOOL CALLBACK AbortProc(HDC hDC, int Error) 
    { 
    MSG   msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
    return TRUE;
    } 

/*===============================*/
/* Initialize DOCINFO structure  */
/* ==============================*/

void InitDocStruct(DOCINFO* di, char* docname)
    { 
    // Always zero it before using it
    memset(di, 0, sizeof(DOCINFO));
    // Fill in the required members
    di->cbSize = sizeof(DOCINFO);
    di->lpszDocName = docname;
    } 

/*===============================*/
/* Obtain printer device context */
/* ==============================*/

HDC GetPrinterDC(void) 
    { 
    PRINTDLG pdlg;
    // Initialize the PRINTDLG structure
    memset(&pdlg, 0, sizeof(PRINTDLG));
    pdlg.lStructSize = sizeof(PRINTDLG);
    // Set the flag to return printer DC
//    pdlg.Flags = PD_RETURNDEFAULT | PD_RETURNDC;
    pdlg.Flags = PD_RETURNDC;
    // Invoke the printer dialog box
    PrintDlg(&pdlg);
    // hDC member of the PRINTDLG structure contains
    // the printer DC
    return pdlg.hDC;
    } 

/*===============================*/
/* Drawing on the Paper DC       */
/* ==============================*/

void DrawOnPaper(HDC hdc, HWND hWnd, BOOL bFinal)
    { // This is the function that does draws on a given DC
    int    j, k;
    RECT   rcPage, rcTop, rcBot, rcStats, rcTemp;
    HPEN   hTopPen, hBotPen;
    HPEN   hBlackPen = GetStockObject(BLACK_PEN);
    HBRUSH hFaceBrush, hBackBrush;
    HFONT  hNameFont, hStatsFont, hPrevFont;
    char   szText[250];
    char   szName[50];
    char   szDateThen[20];
    char   szDateNow[20];
    HENHMETAFILE hemf =0;
    HBITMAP    hSmLeaf=0;
    HINSTANCE  hInst  = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
    // set the size based on where we're drawing
    SetRect(&rcPage, 0, 0, 
            GetDeviceCaps(hdc, HORZRES),
            GetDeviceCaps(hdc, VERTRES));

    hTopPen  = CreatePen(PS_SOLID, 5, RGB(0,168,0)); // lighter green
    hBotPen  = CreatePen(PS_SOLID, 5, RGB(0,128,0)); // darker green
    hFaceBrush = CreateSolidBrush(RGB(128,255,160)); // bright green
    hBackBrush = CreateSolidBrush(RGB(128,128,128)); // dark grey

    SetBkMode(hdc, TRANSPARENT);

    CopyRect(&rcTop, &rcPage);
    for(j=0;j<5;j++)
        {
        InflateRect(&rcTop, -10, -10);
        Frame3D(hdc, rcTop, hBlackPen, hBlackPen);
        }

    hemf = GetEnhMetaFile("BigLeaf.dat");
    if(hemf)
        { // now we define the area for the top metafile
        CopyRect(&rcTop, &rcPage);
        InflateRect(&rcTop, -100, -100);
        rcTop.bottom = rcTop.top + (rcTop.right-rcTop.left);
        // then play and delete the metafile 
        PlayEnhMetaFile(hdc, hemf, &rcTop);
        DeleteEnhMetaFile(hemf);
        }

    hemf = GetEnhMetaFile("Harvest.dat");
    if(hemf)
        { // now we define the area for the top metafile
        CopyRect(&rcTop, &rcPage);
        InflateRect(&rcTop, -100, -100);
        rcTop.bottom = rcTop.top + (rcTop.right-rcTop.left);
        // then play and delete the metafile 
        PlayEnhMetaFile(hdc, hemf, &rcTop);
        DeleteEnhMetaFile(hemf);
        }
    // now we draw the credits
    CopyRect(&rcBot, &rcPage);
    InflateRect(&rcBot, -120, -120);
    rcBot.top = 50 + rcBot.top + (rcBot.right-rcBot.left);
    OffsetRect(&rcBot, 40, 40);
    Rect3D(hdc, rcBot, GetStockObject(BLACK_PEN), hBackBrush);
    OffsetRect(&rcBot, -40, -40);
    Rect3D(hdc, rcBot, hBotPen, hFaceBrush);
    InflateRect(&rcBot, -120, -120);

    CopyRect(&rcStats, &rcBot);
    k = ((rcStats.bottom-rcStats.top)/4)/2;
    hNameFont = CreateFont(k,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0,"Lucida Casual");
    hPrevFont = SelectObject(hdc, hNameFont);

    LoadString(hInst, IDS_HV01, strbuf, sizeof(strbuf));
    if(bFinal) // now load and print the left hand statistics
         {
         REReadRegistryKey("Grower's Name", (LPCTSTR)szName, 30),
         sprintf(szText, strbuf, szName, PI_CurPlant.szPlantName);
         }
    else sprintf(szText, strbuf, "Test Print", "Test Print");
    DrawText(hdc, szText, strlen(szText), &rcStats, DT_CENTER|DT_WORDBREAK); 

    CopyRect(&rcStats, &rcBot);
    DrawText(hdc, szText, strlen(szText), &rcBot, 
             DT_CENTER|DT_WORDBREAK|DT_CALCRECT); 
    rcStats.top = rcBot.bottom+20;
    InflateRect(&rcStats, -30, -30);
    Frame3D(hdc, rcStats, hTopPen, hBotPen);
    InflateRect(&rcStats, -10, -10);
    Frame3D(hdc, rcStats, hTopPen, hBotPen);

    InflateRect(&rcStats, -100, -10);
    k = ((rcStats.bottom-rcStats.top)/6);
    hStatsFont = CreateFont(k,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0,"Lucida Casual");
    SelectObject(hdc, hStatsFont);
    // first measure the text height and make an adjustment to our rect
    LoadString(hInst, IDS_HV02, strbuf, sizeof(strbuf));
    CopyRect(&rcTemp, &rcStats);
    DrawText(hdc,szText,strlen(szText),&rcTemp,DT_LEFT|DT_WORDBREAK|DT_CALCRECT); 
    rcStats.top += (((rcStats.bottom-rcStats.top)-(rcTemp.bottom-rcTemp.top))/4);

    // now load and print the left hand statistics
    if(bFinal) 
         {
         SGGetHybridName(PI_CurPlant.cSeedChoice-1, hInst, (char*)&szName, 40);
         sprintf(szText, strbuf, szName, giGrowDay, 
                 GD_CurPlant.cHealth, GD_CurPlant.iHeight);
         }
    else sprintf(szText, strbuf, "Test Print", 0, 0, 0);
    DrawText(hdc, szText, strlen(szText), &rcStats, DT_LEFT|DT_WORDBREAK); 
    // now load and print the right hand statistics
    LoadString(hInst, IDS_HV03, strbuf, sizeof(strbuf));
    if(bFinal) 
         {
         GLFormatDate((char*)&PI_CurPlant.szStartDate, (char*)&szDateThen);
         GLFormatDateNow((char*)&szDateNow);
         sprintf(szText, strbuf, szDateThen, szDateNow,
                 (GD_CurPlant.iPotency/100.00), GD_CurPlant.iMass);
         }
    else sprintf(szText, strbuf, "Test Print", "Test Print", 0.00, 0);
    DrawText(hdc, szText, strlen(szText), &rcStats, DT_RIGHT|DT_WORDBREAK); 
    // now we can delete our font again
    if(hStatsFont) DeleteObject(hStatsFont);

    SelectObject(hdc, hPrevFont);
    if(hNameFont)  DeleteObject(hNameFont);
    if(hTopPen)    DeleteObject(hTopPen);
    if(hBotPen)    DeleteObject(hBotPen);
    if(hFaceBrush) DeleteObject(hFaceBrush);
    if(hBackBrush) DeleteObject(hBackBrush);
    } 

/*===============================*/
/* Drawing on the Window DC      */
/* ==============================*/

void HADrawInWindow(HDC hdc, HWND hWnd)
    { // This is the function that does draws on a given DC of a window
    char szText[250];
    HENHMETAFILE hemf =0;
    HBITMAP    hSmLeaf=0;
    BITMAP     bm;
    HPALETTE   hLeafPal;
    HFONT      hPrevFont;
    RECT   rcPage, rcTop, rcBot;
    HPEN   hTopPen = CreatePen(PS_SOLID, 1, RGB(128,255,0)); // light green
    HPEN   hBotPen = CreatePen(PS_SOLID, 1, RGB(0,128,0)); // dark green
    HPEN   hBlackPen = GetStockObject(BLACK_PEN);
    HBRUSH hFaceBrush = CreateSolidBrush(RGB(128,255,128)); // bright green
    HBRUSH hBackBrush = CreateSolidBrush(RGB(128,128,128)); // dark grey
    HFONT  hFont = CreateFont(-11,0,0,0,FW_NORMAL,0,0,0,0,0,0,0,0,"Lucida Casual");
    HINSTANCE  hInst  = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
    // set the size rect for where we're drawing into
    GetClientRect(hWnd, &rcPage);
    InflateRect(&rcPage, -1, -1);

    SetBkMode(hdc, TRANSPARENT);
    hPrevFont = SelectObject(hdc, hFont);

    Rectangle3D(hdc, rcPage, hBlackPen, GetStockObject(WHITE_BRUSH), 
                GetStockObject(WHITE_PEN), hBlackPen);

    hSmLeaf = DILoadResourceBitmap(hInst,"Leaf256",&hLeafPal);
    if(hSmLeaf)
        {
        GetObject(hSmLeaf, sizeof(bm), &bm);
        rcTop.left   = rcPage.left+((rcPage.right-rcPage.left-bm.bmWidth)/2);
        rcTop.top    = rcPage.top+((rcPage.bottom-rcPage.top-bm.bmHeight)/3);
        rcTop.right  = rcTop.left+bm.bmWidth;
        rcTop.bottom = rcTop.top+bm.bmHeight;
        DIDrawBitmap(hdc, rcTop.left, rcTop.top, hSmLeaf, hLeafPal, SRCCOPY);
        DIFreeBitmap(hSmLeaf, hLeafPal);
        }

    hemf = GetEnhMetaFile("Harvest.dat");
    if(hemf)
        { // now we define the area for the top metafile
        CopyRect(&rcTop, &rcPage);
        InflateRect(&rcTop, -10, -10);
        rcTop.bottom = rcTop.top + (rcTop.right-rcTop.left);
        // then play and delete the metafile 
        PlayEnhMetaFile(hdc, hemf, &rcTop);
        DeleteEnhMetaFile(hemf);
        }
    // now we draw the credits
    CopyRect(&rcBot, &rcPage);
    InflateRect(&rcBot, -10, -10);
    rcBot.top = 5 + rcBot.top + (rcBot.right-rcBot.left);
    OffsetRect(&rcBot, 2, 2);
    Rect3D(hdc, rcBot, GetStockObject(BLACK_PEN), hBackBrush);
    OffsetRect(&rcBot, -2, -2);
    Rect3D(hdc, rcBot, hBotPen, hFaceBrush);
    InflateRect(&rcBot, -5, -5);
    LoadString(hInst, IDS_HV01, strbuf, sizeof(strbuf));
    wsprintf(szText, strbuf, ".........", ".....");
    InflateRect(&rcBot, -10, 0);
    DrawText(hdc, szText, strlen(szText), &rcBot, DT_CENTER|DT_WORDBREAK); 

    SelectObject(hdc, hPrevFont);
    if(hFont)      DeleteObject(hFont);
    if(hTopPen)    DeleteObject(hTopPen);
    if(hBotPen)    DeleteObject(hBotPen);
    if(hFaceBrush) DeleteObject(hFaceBrush);
    if(hBackBrush) DeleteObject(hBackBrush);
    } 


/*==============================================*/
/* Sample code :  Typical printing process      */
/* =============================================*/

BOOL PrintStuff(HWND hWnd, BOOL bFinal)
    { 
    HDC        hDC;
    DOCINFO    di;
    // Need a printer DC to print to
    hDC = GetPrinterDC();
    // Did you get a good DC?
    if(!hDC)  return FALSE;
    // You always have to use an AbortProc()
    if(SetAbortProc(hDC, AbortProc) == SP_ERROR)
        {
        MessageBox(hWnd, "Error setting up AbortProc", "Error", MB_APPLMODAL | MB_OK);
        return FALSE;
        }
    // Init the DOCINFO and start the document
    InitDocStruct(&di, "MyDoc");
    StartDoc(hDC, &di);
    StartPage(hDC);
    DrawOnPaper(hDC, hWnd, bFinal);
    EndPage(hDC);
    EndDoc(hDC);
    DeleteDC(hDC);
    SetForegroundWindow(hWnd);
    return TRUE;
    } 



/*==============================================*/
/* Harvest Dialog handler                       */
/* =============================================*/

HFONT  hPrintBigFont;
HFONT  hPrintMedFont;

void HAInitPrintDialog(HWND hDlg, HINSTANCE hInst)
    {
    long lp=MAKELPARAM(TRUE, 0);
    // first center the dialog in our window
    DlgCenter(hDlg);
    hPrintBigFont = GLCreateDialogFont(24,0,FW_BOLD);
    hPrintMedFont = GLCreateDialogFont(18,0,FW_BOLD);
    SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hPrintBigFont,lp);
    SendDlgItemMessage(hDlg,IDS_ST01,WM_SETFONT,(WPARAM)hPrintMedFont,lp);
    SendDlgItemMessage(hDlg,IDS_ST02,WM_SETFONT,(WPARAM)hPrintMedFont,lp);
    REReadRegistryKey("Grower's Name", (LPCTSTR)strbuf, 30);
    SendDlgItemMessage(hDlg,IDC_ED01,WM_SETTEXT,0,(LPARAM)strbuf);
    }


BOOL CALLBACK HAPrintDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   
    switch (message)
      {
      case WM_INITDIALOG:
            HAInitPrintDialog(hDlg, dhInst);
      return (TRUE);

      case WM_ERASEBKGND:
          if(-1==GetDeviceCaps((HDC)wParam, NUMCOLORS))
              { // if we're in high colour, we'll draw the light leaf background
              DITileDlgLeafBitmap(hDlg, (HDC)wParam);
              return (TRUE);
              }
      return (FALSE);

      case WM_CTLCOLORSTATIC:
           SetBkMode((HDC)wParam, TRANSPARENT);
      return (int)GetStockObject(NULL_BRUSH);
 
      case WM_DESTROY:
            {
            if(hPrintBigFont) DeleteObject(hPrintBigFont);
            if(hPrintMedFont) DeleteObject(hPrintMedFont);
            }
      break;

      case WM_COMMAND:
          switch (wParam)
            { 
            case IDC_BT01: // test print button
                if(PASetGrowerName(hDlg, dhInst, IDC_ED01))
                    PrintStuff(hDlg, FALSE);
            return (FALSE);

            case IDC_BT02: // Harvest without printing button
                if(PASetGrowerName(hDlg, dhInst, IDC_ED01))
                    { // if it's a female and with seeds...
                    if((PS_Plant[giCurPlant-1].bSeedGender)
                       &&(PI_CurPlant.cGrowthStage==6))
                            { // ..we'll allow him to breed
                            SISeedBreedDialog(hDlg, dhInst);
                            }
                    // now we add the plant to the log file
                    // and remove it from the growroom.
                    PGAddCurPlantToLogFile();
                    TBRemoveCurPlant();
                    EndDialog(hDlg, TRUE); 
                    }
            return (FALSE);

            case IDOK:  // final print button
                if(PASetGrowerName(hDlg, dhInst, IDC_ED01))
                    {
                    if(PrintStuff(hDlg, TRUE))
                        {
                        if((PS_Plant[giCurPlant-1].bSeedGender)
                           &&(PI_CurPlant.cGrowthStage==6))
                               { // ..we'll allow him to breed
                               SISeedBreedDialog(hDlg, dhInst);
                               }
                        PGAddCurPlantToLogFile();
                        TBRemoveCurPlant();
                        EndDialog(hDlg, TRUE); 
                        }
                    }
            return (TRUE);

            case IDCANCEL: 
                EndDialog(hDlg, FALSE); 
            return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          


BOOL HAPrintDialog(HWND hwnd, HINSTANCE hInst)
    {
    return DialogBox(hInst, "Print", hwnd, HAPrintDlgProc);
    }

/*==============================================*/
/* Pre-Harvest Dialog handler                   */
/* =============================================*/

HFONT    hHarvBigFont;
HFONT    hHarvMedFont;
HBITMAP  hBudBitmap;
HPALETTE hBudPalette;

void HAInitHarvestDialog(HWND hDlg, HINSTANCE dhInst)
    {
    int i;
    long lp=MAKELPARAM(TRUE, 0);
    char szName[40];
    // first center the dialog in our window
    DlgCenter(hDlg);
    // now we'll create some bold fonts
    hHarvBigFont = GLCreateDialogFont(24,0,FW_BOLD);
    hHarvMedFont = GLCreateDialogFont(18,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hHarvBigFont) 
        SendDlgItemMessage(hDlg,IDA_ST01,WM_SETFONT,(WPARAM)hHarvBigFont,lp);
    // if we created the seed text font, set it to all the heading controls
    if(hHarvMedFont) 
        {
        for(i=0;i<7;i++)
            SendDlgItemMessage(hDlg,IDA_ST11+i,WM_SETFONT,(WPARAM)hHarvMedFont,lp);
        }
    // *** PLANT NAME ***
    SGGetHybridName(PI_CurPlant.cSeedChoice-1, dhInst, (char*)&szName, 40);
    sprintf(strbuf, "Harvesting %s", PI_CurPlant.szPlantName);
    SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM)strbuf);
    SendDlgItemMessage(hDlg, IDA_ST01, WM_SETTEXT, 0, (LPARAM)strbuf);
    SendDlgItemMessage(hDlg, IDA_ST02, WM_SETTEXT, 0, (LPARAM)szName);
    // *** DAYS GROWTH ***
    GLSetNumText(giGrowDay, hDlg, IDA_ST03);
    // *** GROWTH STAGE ***
    GLGetGrowthStageString(strbuf, PI_CurPlant.cGrowthStage);
    SendDlgItemMessage(hDlg, IDA_ST04, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** HEALTH % ***
    sprintf(strbuf, "%i %%", GD_CurPlant.cHealth);
    SendDlgItemMessage(hDlg, IDA_ST05, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** POTENCY THC ***
    sprintf(strbuf, "%1.2f %%THC", (GD_CurPlant.iPotency/100.00));
    SendDlgItemMessage(hDlg, IDA_ST06, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** HEIGHT IN MM ***
    sprintf(strbuf, "%i mm", GD_CurPlant.iHeight);
    SendDlgItemMessage(hDlg, IDA_ST07, WM_SETTEXT, 0, (LPARAM)strbuf);
    // *** GROWING MASS ***
    sprintf(strbuf, "%i grams", GD_CurPlant.iMass);
    SendDlgItemMessage(hDlg, IDA_ST08, WM_SETTEXT, 0, (LPARAM)strbuf);
    }

void HADestroyHarvestDialog(HWND hDlg)
    {
    if(hHarvBigFont) DeleteObject(hHarvBigFont);
    if(hHarvMedFont) DeleteObject(hHarvMedFont);
    }

  
BOOL CALLBACK HAHarvestDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   
    switch (message)
      {
      case WM_INITDIALOG:
            HAInitHarvestDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
           HADestroyHarvestDialog(hDlg);
      break;

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
          if(i==IDA_STPIC)
              HADrawInWindow((HDC)wParam, (HWND)lParam);
          else
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_COMMAND:
          switch (wParam)
            {
            case IDOK: 
                if(gbRegistered)
                    {
                    if(HAPrintDialog(hDlg, dhInst))
                        {
                        EndDialog(hDlg, TRUE); 
                        return (TRUE);
                        }
                    }
                else HAUnavailableDialog(hDlg, dhInst);
            return (FALSE);

            case IDCANCEL: 
                EndDialog(hDlg, FALSE); 
            return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          


BOOL HAHarvestInitGrowInfo(int iPlant)
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


void HAShowPlantHarvestDialog(HWND hwnd, HINSTANCE hInst, int iPlant)
    { // show the option based 0 plant
    if(HAHarvestInitGrowInfo(iPlant))
        {
        if(DialogBox(hInst, "Harvest", hwnd, HAHarvestDlgProc))
            {
            EnableMenuItem(GetMenu(hwnd), IDM_PLOG, MF_BYCOMMAND|MF_ENABLED);
            }
        }
    }



