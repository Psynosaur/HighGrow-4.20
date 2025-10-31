// =======================================================
// LOGEXPORT.C -- Exporting & Printing History Log info
// =======================================================

#include <windows.h>
#include <commdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include "resource.h"
#include "global.h"
#include "plantmem.h"
#include "calc.h"
#include "registry.h"

// =======================================================
// EXPORTING LOG FILE DATA
// =======================================================
// GETTING THE NAME OF THE TEXT FILE TO SAVE INTO
// -------------------------------------------------------
  
static OPENFILENAME ofn ;

BOOL LEFileSaveDlg (HWND hwnd, LPSTR lpstrFileName)
     {
     char szStartPath[250] = "\0";
     static char szFilter[] = "HighGrow Growth Log Files (*.log)\0*.log\0" \
                              "All Files (*.*)\0*.*\0\0" ;
     // read the name of his startup directory
     REReadRegistryKey("Startup in", (LPCTSTR)szStartPath, 250);
     // now fill the OPENFILENAME struct elements
     ofn.lStructSize       = sizeof (OPENFILENAME) ;
     ofn.hwndOwner         = hwnd ;
     ofn.hInstance         = NULL ;
     ofn.lpstrFilter       = szFilter ;
     ofn.lpstrCustomFilter = NULL ;
     ofn.nMaxCustFilter    = 0 ;
     ofn.nFilterIndex      = 0 ;
     ofn.lpstrFile         = lpstrFileName ;
     ofn.nMaxFile          = _MAX_PATH ;
     ofn.lpstrFileTitle    = NULL ;
     ofn.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
     ofn.lpstrInitialDir   = szStartPath ;
     ofn.lpstrTitle        = "Export History Log" ;
     ofn.Flags             = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR ;
     ofn.nFileOffset       = 0 ;
     ofn.nFileExtension    = 0 ;
     ofn.lpstrDefExt       = "log" ;
     ofn.lCustData         = 0L ;
     ofn.lpfnHook          = NULL ;
     ofn.lpTemplateName    = NULL ;
     // now get the save filename
     return GetSaveFileName (&ofn) ;
     }

// -------------------------------------------------------
// OPENING, CLOSING & WRITING LOG DATA TO A TEXT FILE
// -------------------------------------------------------

FILE *LogStream=NULL;

BOOL LEOpenFile(LPSTR lpFileName)
   {
   if((LogStream = fopen(lpFileName, "w"))!=NULL)  return TRUE;
   return FALSE;
   }

BOOL LEWriteLine(char* szLine)
   {
   if(LogStream==NULL) return FALSE;
   if(fputs(szLine, LogStream)==0) return TRUE;
   return FALSE;
   }

void LECloseFile(void)
   {
   if(LogStream!=NULL) fclose(LogStream);
   }

// -------------------------------------------------------
// GETTING THE HEADING AND HISTORY LOG DATA (FOR EXPORTING)
// -------------------------------------------------------

// MessageBox(NULL, ofn.lpstrFile, "FileName", MB_OK);

void LEGetExportHeadingData(HINSTANCE hInst, char* szLineBuf)
	{
	int  i;
    char szText[64]="\0";     // place to store some text.
    for (i=0; i<10;i++)
        {
        LoadString(hInst, IDH_ST01+i, szText, sizeof(szText));
		strcat(szLineBuf, szText);
		strcat(szLineBuf, "\t"); // add a tab character
		if(i==1||i==4)  strcat(szLineBuf, "\t");  // some fields get two tabs
        }
	strcat(szLineBuf, "\n\n");
	}


void LEGetExportLineData(char* lpLineBuffer, int iLine)
	{
    PGROWDAY pDay  = (PGROWDAY)&P_Plant->GD_Plant[iLine+1];
    int i,j=0;
	char szDate[50]    ="\0";
	char szPotency[50] ="\0";
	char szNutrient[50]="\0";
	char szSoilPH[50]  ="\0";
    // first null the buffer
	lpLineBuffer[0]='0';
    // Date
    GLNextShortFormatDate((char*)P_Plant->PI_Plant.szStartDate, (char*)szDate, iLine);
    // Potency
    sprintf(szPotency, "%1.2f", (int)pDay->iPotency/100.00);
    // Fertilizer
    for(i=0; i<4; i++) j=j+pDay->szNutrients[i];
    sprintf(szNutrient, "%1.1f", j/10.0);
    // Soil pH
    sprintf(szSoilPH, "%1.1f", pDay->cSoilPH/10.0);
	// Now add them all together
	//  day# / date / health / height / mass / potency / moisture / fertilizer / pH / pruned
    wsprintf(lpLineBuffer, "%i\t%s\t%i\t\t%i\t\t%i\t\t%s\t\t%i\t\t%s\t\t%s\t\t%i\n", 
			 iLine+1, szDate, (int)pDay->cHealth, pDay->iHeight, pDay->iMass, 
			 szPotency, pDay->iMoisture, szNutrient, szSoilPH, (int)pDay->cTipsPruned);
	}


// -------------------------------------------------------
// EXPORTING HISTORY LOG DATA TO A TEXT FILE
// -------------------------------------------------------


void LEExportHistoryData(void)
	{
	int  iDay;
	char szLineBuf[250]="\0";

    for(iDay=0; iDay<giGrowDay; iDay++)
		{
		// get this line of plant history
		LEGetExportLineData((char*)szLineBuf, iDay);
	    // and write the string to the longfile
	    LEWriteLine(szLineBuf);
		}
	}


void LEExportHeadingData(HINSTANCE hInst, LPSTR lpPlantName)
	{
	char szTemp[150]="\0";
	char szLineBuf[250]="\0";
    // identify the plant
    wsprintf(szTemp, "HighGrow Growth Log:  %s\n\n", lpPlantName);
	LEWriteLine(szTemp);
    // get the heading line
	LEGetExportHeadingData(hInst, (char*)&szLineBuf);
    // and write the string to the longfile
	LEWriteLine(szLineBuf);
	}

// -------------------------------------------------------
// MAIN HISTORY LOG EXPORTING FUNCTION
// -------------------------------------------------------

void LEExportHistoryLog(HWND hwnd, HINSTANCE hInst, LPSTR lpPlantName, int iPlant)
    { // Export the History Log for the current plant
	char szPlant[100]="\0";
    giCurPlant = iPlant;
    sprintf(strbuf, "Plant%02i.hgp", iPlant);
    if(GLDoesFileExist(strbuf))
        { // show the export dialog now
        if(PMPlantFileToMem(iPlant-1))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            CACalculatePlant(P_Plant, iPlant-1);
            // copy the plant's name, as openfilename will change it
			strcpy(szPlant, lpPlantName);
            if(LEFileSaveDlg(hwnd, lpPlantName))
				{
                if(LEOpenFile(ofn.lpstrFile))
                    {
					LEExportHeadingData(hInst, szPlant);
					LEExportHistoryData();
                    LECloseFile();
                    }
				else MessageBox(hwnd, "Error Writing to Log File", 
						        "Error", MB_APPLMODAL | MB_OK);

				}
            GlobalUnlock(hPlantMem);
            PMPlantMemoryFree();
            }
        }
    }


// =======================================================
// PRINTING LOG FILE DATA
// =======================================================
// THE ABORT PROCEDURE
// -------------------

BOOL CALLBACK LEAbortProc(HDC hDC, int Error) 
    { 
    MSG   msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
    return TRUE;
    } 

// ------------------------
// GETTING THE PRINTER'S DC
// ------------------------

HDC LEGetPrinterDC(void) 
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

// -------------------------------------------------------
// GETTING THE HEADING AND HISTORY LOG DATA (FOR PRINTING)
// -------------------------------------------------------

// MessageBox(NULL, ofn.lpstrFile, "FileName", MB_OK);

void LEGetPrintHeadingData(HINSTANCE hInst, char* szLineBuf)
	{
	int  i;
    char szText[64]="\0";     // place to store some text.
    for (i=0; i<10;i++)
        {
        LoadString(hInst, IDH_ST01+i, szText, sizeof(szText));
		strcat(szLineBuf, szText);
		strcat(szLineBuf, "\t"); // add a tab character
        }
	}


void LEGetPrintLineData(char* lpLineBuffer, int iLine)
	{
    PGROWDAY pDay  = (PGROWDAY)&P_Plant->GD_Plant[iLine+1];
    int i,j=0;
	char szDate[50]    ="\0";
	char szPotency[50] ="\0";
	char szNutrient[50]="\0";
	char szSoilPH[50]  ="\0";

    // Date
    GLNextShortFormatDate((char*)P_Plant->PI_Plant.szStartDate, (char*)szDate, iLine);
    // Potency
    sprintf(szPotency, "%1.2f", (int)pDay->iPotency/100.00);
    // Fertilizer
    for(i=0; i<4; i++) j=j+pDay->szNutrients[i];
    sprintf(szNutrient, "%1.1f", j/10.0);
    // Soil pH
    sprintf(szSoilPH, "%1.1f", pDay->cSoilPH/10.0);
	// Now add them all together
	//  day# / date / health / height / mass / potency / moisture / fertilizer / pH / pruned
    wsprintf(lpLineBuffer, "%i\t%s\t%i\t%i\t%i\t%s\t%i\t%s\t%s\t%i\0", 
			 iLine+1, szDate, (int)pDay->cHealth, pDay->iHeight, pDay->iMass, 
			 szPotency, pDay->iMoisture, szNutrient, szSoilPH, (int)pDay->cTipsPruned);
	}


// ------------------------
// THE PAGE PRINTING LOOP
// ------------------------


void LEPrintHeader(HDC hdc, HINSTANCE hInst, LPRECT rcPg, LPRECT rcHdr, int* lpTabs, LPSTR lpName)
	{
	int  i;
	char szDate[60] ="\0";
	char szTemp[100]="\0";
	char szLineBuf[250]="\0";

	Rectangle(hdc, rcHdr->left, rcHdr->top, rcHdr->right, rcHdr->bottom);
	rcHdr->left  += 50;
	rcHdr->right -= 50;

    GetDateFormat(LOCALE_SYSTEM_DEFAULT,0,NULL,"dddd',' dd MMMM yyyy",(LPTSTR)&szDate, 60);
    wsprintf(szTemp, "HighGrow Growth Log:  %s", lpName);
	DrawText(hdc, szTemp, -1, rcHdr, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	DrawText(hdc, szDate, -1, rcHdr, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
	FrameRect(hdc, rcPg, GetStockObject(BLACK_BRUSH));
	// also draw the column lines
	for(i=0; i<10; i++)
		{
		MoveToEx(hdc, lpTabs[i]-100, 200, NULL);
		LineTo(hdc, lpTabs[i]-100, rcPg->bottom);
		}
    LEGetPrintHeadingData(hInst, (char*)&szLineBuf);
    TabbedTextOut(hdc, 50, 250, szLineBuf, strlen(szLineBuf), 10, lpTabs, 0);
	// now draw the heading underline
    MoveToEx(hdc, 0, 350, NULL);
	LineTo(hdc, rcPg->right, 350);
	// and reset the header rect for the next page
	rcHdr->left  -= 50;
	rcHdr->right += 50;
	}


void LEPrintHistoryPages(HWND hWnd, HDC hdc, HINSTANCE hInst, LPSTR lpPlantName)
    { // This is the function that does draws on a given DC
    RECT   rcPage, rcHeader;
	SIZE   sFontSize;
	int    lpTabStops[10];
	int    i, iY, iFontHeight, iOMode;
    HPEN   hOPen,  hBlackPen  = GetStockObject(BLACK_PEN);
    HBRUSH hOBrush,hGreyBrush = GetStockObject(LTGRAY_BRUSH);
	HFONT  hOFont, hHeaderFont, hTextFont;
	char   szLineBuf[250]="\0";

	// first create our fonts
    hHeaderFont = GLCreateDialogFont(80,0,FW_BOLD);
    hTextFont   = GLCreateDialogFont(80,0,FW_NORMAL);
	// first select all our objects
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
    // now setup all the locals for this page
    hOFont  = SelectObject(hdc, hTextFont);
	GetTextExtentPoint32(hdc, "W", 1, &sFontSize);
	iFontHeight = sFontSize.cy;
    // also setup the tabstop array positions
    lpTabStops[0] = (rcPage.right/10)/2+150;
	for(i=1; i<9; i++)
        lpTabStops[i] = (i+1)*(rcPage.right/10)+50;

	// Start the Page Printing cycle
    StartPage(hdc);
	// print the page border and header
    SelectObject(hdc, hHeaderFont);
    LEPrintHeader(hdc, hInst, &rcPage, &rcHeader, (int*)&lpTabStops, lpPlantName);
    SelectObject(hdc, hTextFont);
    // now print all the history data
    iY = 400;
	for(i=0; i<giGrowDay; i++)
		{
	    LEGetPrintLineData((char*)&szLineBuf, i);
	    TabbedTextOut(hdc, 50, iY, szLineBuf, strlen(szLineBuf), 10, lpTabStops, 0);
		// now, if we've hit the end, we'll getout of the loop
		if((iY+(2*iFontHeight)) > rcPage.bottom) 
			{ // if we cannot fit any more on this page, go to the next one
		    EndPage(hdc);
		    StartPage(hdc);
			// print the page border and header
			SelectObject(hdc, hHeaderFont);
			LEPrintHeader(hdc, hInst, &rcPage, &rcHeader, (int*)&lpTabStops, lpPlantName);
			SelectObject(hdc, hTextFont);
			// reset the current line's Y position on the page
	        iY  = 400;
			}
		else iY += iFontHeight;
		}

	// now end the page printing cycle
        EndPage(hdc);
	// finally we reselect all our objects
        SelectObject(hdc, hOPen);
        SelectObject(hdc, hOBrush);
        SelectObject(hdc, hOFont);
	SetBkMode(hdc, iOMode);
	if(hHeaderFont) DeleteObject(hHeaderFont);
	if(hTextFont)   DeleteObject(hTextFont);
	}


// -------------------------------------------------------
// PRINTING THE PLANT HISTORY LOG REPORT
// -------------------------------------------------------

BOOL LEPrintPlantHistory(HWND hWnd, HINSTANCE hInst, LPSTR lpPlantName, int iPlant)
    { 
    DOCINFO di;
    HDC     hDC;
    // Need a printer DC to print to
    hDC = LEGetPrinterDC();
    // Did we get a good DC?
    if(!hDC) return FALSE;
    // Here we set an AbortProc()
    if(SetAbortProc(hDC, LEAbortProc) == SP_ERROR)
        {
        MessageBox(hWnd, "Error setting up AbortProc", "Error", MB_APPLMODAL | MB_OK);
        return FALSE;
        }
    // Init the DOCINFO and start the document
    memset(&di, 0, sizeof(DOCINFO));
    di.cbSize    = sizeof(DOCINFO);
    di.lpszDocName = "History Log Printing";
	// start the document printing
    StartDoc(hDC, &di);
	// now print all the pages for this log
    LEPrintHistoryPages(hWnd, hDC, hInst, lpPlantName);
	// end the printing and clean up
    EndDoc(hDC);
    DeleteDC(hDC);
    SetForegroundWindow(hWnd);
    return TRUE;
    } 

// -------------------------------------------------------
// MAIN HISTORY LOG PRINTING FUNCTION
// -------------------------------------------------------

BOOL LEPrintHistoryLog(HWND hWnd, HINSTANCE hInst, LPSTR lpPlantName, int iPlant)
    { // Export the History Log for the current plant
    giCurPlant = iPlant;
    sprintf(strbuf, "Plant%02i.hgp", iPlant);
    if(GLDoesFileExist(strbuf))
        { // show the export dialog now
        if(PMPlantFileToMem(iPlant-1))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            CACalculatePlant(P_Plant, iPlant-1);
			// now print the log for this plant
			LEPrintPlantHistory(hWnd, hInst, lpPlantName, iPlant);
			// free the memory again after printing
            GlobalUnlock(hPlantMem);
            PMPlantMemoryFree();
			return TRUE;
            }
        }
    return FALSE;
    }
