#include<windows.h>
#include<wininet.h>
#include<iostream.h>
#include "resource.h"
#include "dibitmap.h"  
#include "global.h"  
#include "crypt.h"  

//******************************************************************************
//*  GLOBAL VARIABLES
//******************************************************************************

char szRoomName[50]="\0";

//******************************************************************************
//*  DOWNLOAD SERVER AND GROWROOM INFORMATION
//******************************************************************************

// **** beta code requirements ***
//#include "password.h"  

// *** ROOM DOWNLOAD LINK ***
// BETA 1 - www.rsrclan.com/highgrow/picserv/
//        - zx{0suueobr0dqp1kjkjhtry2qmetgux2
// BETA 2 - www.highgrow.us/newer/picserv/
//        - zx{0ikjjjssy/wv1qf{gs1skftitw1
// FINAL  - 66.221.101.79/picserv/
//        - 972433133229:1skftitw1

LPCSTR glpHighPicServer = "972433133229:1skftitw1";


//LPCSTR glpHighPicLog = "/imgdownload.php?img=";
LPCSTR glpHighPicLog   = "2jqieqzpopef/rkrBjqi>";


// *** ROOM PAGE LINK ***
// BETA 1 - http://www.rsrclan.com/highgrow/rooms.php
//        - kuxr;12yzx2tttfndo2epo2jlhlisqz1
// BETA 2 - http://www.highgrow.us/newer/rooms.php
//        - kuxr;12yzx2jjikiup{0vu2phxit0trqpt2rir
// FINAL  - http://www.highgrow.us/rooms.php
//        - kuxr;12yzx2jjikiup{0vu2trpqu/rkr

LPCSTR glpHighRoomServer ="kuxr;12yzx2jjikiup{0vu2trpqu/rkr";

//******************************************************************************
//*  DOWNLOAD COMPLETED SUCCESSFULLY DIALOG
//******************************************************************************

BOOL CALLBACK INDownloadedProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   static HFONT hHdrFont=0;
   static HFONT hBigFont=0;
   static HBITMAP  hBkgndBmp=0;
   static HPALETTE hBkgndPal=0;

    switch (message)
      {
      case WM_INITDIALOG:
            {
            // ---
            DlgCenter(hDlg);
            hHdrFont = GLCreateDialogFont(24,0,FW_BOLD);
            hBigFont = GLCreateDialogFont(20,0,FW_BOLD);
            SendDlgItemMessage(hDlg, IDC_ST01, WM_SETFONT, (WPARAM)hHdrFont, 0);
            SendDlgItemMessage(hDlg, IDC_ST02, WM_SETFONT, (WPARAM)hBigFont, 0);
            SendDlgItemMessage(hDlg, IDC_ST02, WM_SETTEXT, 0, (LPARAM)szRoomName);
            hBkgndBmp = DILoadResourceBitmap(dhInst, "dlgleaves", &hBkgndPal);
            }
      return (TRUE);

      case WM_DESTROY:
           if(hHdrFont)  DeleteObject(hHdrFont);
           if(hBigFont)  DeleteObject(hBigFont);
           if(hBkgndBmp) DIFreeBitmap(hBkgndBmp, hBkgndPal);
      return (TRUE);

      case WM_ERASEBKGND:
          if((-1==GetDeviceCaps((HDC)wParam, NUMCOLORS))&&(hBkgndBmp!=0))
              { // if we're in high colour, we'll draw the light leaf background
              RECT rc;
              // --
              GetClientRect(hDlg, &rc);
              DITileBitmap((HDC)wParam, rc, hBkgndBmp, hBkgndPal);
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
          switch (wParam)
            {
            case IDOK:
            case IDCANCEL:
              EndDialog(hDlg, TRUE); 
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE);
   }                          


void INDownloadCompleteDialog(HWND hwnd, HINSTANCE hInst)
    {
    DialogBox(hInst, "DOWNLOADED", hwnd, INDownloadedProc);
    }



//******************************************************************************
//*  CALCULATION PROGRESS MODELESS DIALOG
//******************************************************************************

HWND hDownloadDlg=0;
int iDownloadProgress =0;

BOOL CALLBACK INModelessDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   static HFONT     hHdrFont=0;
   const  HINSTANCE hInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   static HBITMAP  hBkgndBmp=0;
   static HPALETTE hBkgndPal=0;

   switch (message)
      {
      case WM_INITDIALOG:
            {
            char szfile[MAX_PATH];
            // first we show our wait cursor
            SetCursor(LoadCursor(NULL, IDC_WAIT));
            DlgCenter(hDlg);
            hHdrFont = GLCreateDialogFont(18,0,FW_BOLD);
            hBkgndBmp = DILoadResourceBitmap(hInst, "dlgleaves", &hBkgndPal);
            SendDlgItemMessage(hDlg, IDC_ST01, WM_SETFONT, (WPARAM)hHdrFont, 0);
            SendDlgItemMessage(hDlg, IDC_PR01, PBM_SETRANGE, 0, MAKELPARAM(0, (int)lParam));
            // here we show which file we're downloading
            wsprintf(szfile, "Downloading Grow Room Image..... %s", szRoomName);
            SendDlgItemMessage(hDlg, IDS_ST01, WM_SETTEXT, 0, (LPARAM)szfile);
            UpdateWindow(hDlg);
            }
      return (TRUE);

      case WM_DESTROY:
           SetCursor(LoadCursor(NULL, IDC_ARROW));
           if(hHdrFont) 
                {
                DeleteObject(hHdrFont);
                hHdrFont = 0;
                }
           if(hBkgndBmp) DIFreeBitmap(hBkgndBmp, hBkgndPal);
      return (TRUE);

      case WM_ERASEBKGND:
             if((-1==GetDeviceCaps((HDC)wParam, NUMCOLORS))&&(hBkgndBmp!=0))
                  { // if we're in high colour, we'll draw the light leaf background
                  RECT rc;
                  // --
                  GetClientRect(hDlg, &rc);
                  DITileBitmap((HDC)wParam, rc, hBkgndBmp, hBkgndPal);
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


void INCreateModelessDialog(HWND hWnd, HINSTANCE hInst)
    {
    hDownloadDlg = CreateDialogParam(hInst, "calc", hWnd, (DLGPROC)INModelessDlgProc, 10);
    iDownloadProgress = 0;
    }

void INSetProgressText(LPCSTR lpText)
    {
    if(hDownloadDlg)
        {
        SendDlgItemMessage(hDownloadDlg, IDS_ST01, WM_SETTEXT, 0, (LPARAM)lpText);
        UpdateWindow(hDownloadDlg);
        }
    }

void INShowNextProgress(void)
    {
    if(hDownloadDlg)
        {
        SendDlgItemMessage(hDownloadDlg, IDC_PR01, PBM_SETPOS, iDownloadProgress, 0);
        UpdateWindow(hDownloadDlg);
        }
    iDownloadProgress += 1; 
    }

void INEndModelessDialog(void)
    {
    if(hDownloadDlg) DestroyWindow(hDownloadDlg);
    }


//******************************************************************************
//*  DOWNLOADING A ROOM IMAGE FILE FROM THE SERVER
//******************************************************************************

//  INGetInternetFile(hDlg, dhInst, "www.amazes.us", "wildlife/h1.jpg", "c:\\temp\\Hawk.jpg");

BOOL INGetInternetFile(HWND hwnd, HINSTANCE hInst, 
                       char *szServer, char *szFileNumber, char *szLocalFile)
    {
    HINTERNET hSession;
    HINTERNET hConnection;
    HINTERNET hRequest;
    DWORD dwContentLen;
    DWORD dwBufLen = sizeof(dwContentLen);
    char *pData = 0;
    DWORD dwReadSize = 0;
    DWORD cReadCount = 0;
    DWORD dwBytesRead = 0;
    char *pCopyPtr = 0;
    OFSTRUCT OfStruct;  
    int  hFile=0;   // currently opened file handle
    LPVOID lpMem;
    BOOL breturn = FALSE;
    char szLogRequest[50];
    // -----
    // now we can open our internet session
    hSession = InternetOpen("HighGrow Grow Room Image Download",
                            INTERNET_OPEN_TYPE_PRECONFIG,
                            NULL, NULL, 0);
    
    hConnection = InternetConnect(hSession, 
                                 szServer,  // Server
                                 INTERNET_DEFAULT_HTTP_PORT,
                                 NULL,     // Username
                                 NULL,     // Password
                                 INTERNET_SERVICE_HTTP,
                                 0,        // Synchronous
                                 (DWORD)NULL);    // No Context

    if(hConnection==0) 
        {
        InternetCloseHandle(hSession);
        return FALSE;
        }

    // *** HERE WE DO OUR DOWNLOAD LOGGING REQUEST ***
    strcpy(szLogRequest, glpHighPicLog);
    GLUnscrambleText((LPSTR)&szLogRequest);
    strcat(szLogRequest, szFileNumber);

    // ****** HERE WE DO OUR DOWNLOAD FILE REQUEST ******
    // here we send our DOWNLOAD FILE request to the server
    hRequest = HttpOpenRequest(hConnection, 
                               "GET",
                                szLogRequest,
                                NULL,    // Default HTTP Version
                                NULL,    // No Referer
                                (const char**)"*/*\0", // Accept anything
                                0,       // Flags
                                (DWORD)NULL);   // No Context
    // getout if our download request failed
    if(hRequest==0) 
        {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnection);
        InternetCloseHandle(hSession);
        return FALSE;
        }
    
    // here we send our START DOWNLOAD request to the server
    if(HttpSendRequest(hRequest, NULL, 0, NULL, 0))      
        {
        // first we create our modeless progress dialog
        INCreateModelessDialog(hwnd, hInst);
        if (HttpQueryInfo(hRequest, 
                          HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
                          (LPVOID)&dwContentLen,
                          &dwBufLen,
                          0))
            {
            // You have a content length so you can calculate percent complete
            pData = (char*)GlobalAlloc(GMEM_FIXED, dwContentLen + 1);
            dwReadSize = dwContentLen / 10;   // We will read 10% of data
                                                // with each read.

            pCopyPtr = pData;
            for (cReadCount = 0; cReadCount < 10; cReadCount++)
                {
                InternetReadFile(hRequest, pCopyPtr, dwReadSize, &dwBytesRead);
                pCopyPtr = pCopyPtr + dwBytesRead;
                INShowNextProgress();
                }
            // extra read to account for integer division round off
            InternetReadFile(hRequest,
                             pCopyPtr,
                             dwContentLen - (pCopyPtr - pData),
                             &dwBytesRead);
            // Null terminate data
            pData[dwContentLen] = 0;

            // now we save everything to a file on his hard disk
            INSetProgressText("Saving Downloaded Room Image File");
            hFile=OpenFile(szLocalFile, (LPOFSTRUCT)&OfStruct, OF_READWRITE|OF_CREATE);
            if(hFile)
                {
                lpMem = GlobalLock(pData);
                _hwrite(hFile, lpMem, dwContentLen + 1);
                _lclose(hFile);
                GlobalUnlock(pData);
                breturn = TRUE;
                }
            // free our global memort now
            GlobalFree(pData);
            }
        // here we kill the progress dialog again
        INEndModelessDialog();
        // tell him that we were successful
        }

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnection);
    InternetCloseHandle(hSession);
    // now we return our flag
    return breturn;
    }


//******************************************************************************
//*  DOWNLOADING A ROOM IMAGE FILE FROM THE GIVEN URL
//******************************************************************************

// ************************************************************
// *** This routine can download any of the following links ***
// ************************************************************
// *** http://www.rsrclan.com/highgrow/picserv/4.jpg        ***
// *** highgrow://www.rsrclan.com/highgrow/picserv/4.jpg    ***
// *** highgrow://4/                                        ***
// *** highgrow://4~Slick's Garage                          ***
// ************************************************************


BOOL INGetInternetRoomImageFile(HWND hwnd, HINSTANCE hInst, char *szFileURL)
    {
    char szCopyURL[MAX_PATH];
    char szServer[MAX_PATH];
    char szImageFile[MAX_PATH];
    char szFileName[MAX_PATH];
    char szFileNumber[50];
    char szpath[MAX_PATH];
    int  iptr = 0;
    char* szfind;
    BOOL breturn = FALSE;
    // ------
    // getout if we were passed a null string
    if(strlen(szFileURL) == 0) return FALSE;
    // copy the URL to our test string and lowercase it
    strcpy(szCopyURL, szFileURL);
    strlwr(szCopyURL);
    // first we check for the "highgrow://" sequence
    if(strncmp(szCopyURL, "highgrow://", 11)==0)  iptr = 11;
    // now we check if a server name is the next part supplied
    strcpy(szServer, glpHighPicServer);
    GLUnscrambleText((LPSTR)&szServer);
    // now we add the filename portion to this server string
    strcat(szServer, szFileURL+iptr);
    // if the last character is a forward slash, we must remove it
    // *** for testing purposes, there may be NO room name attached ***
    if(szServer[strlen(szServer)-1] == '/')
         {  // now we remove the last forward slash 
         szServer[strlen(szServer)-1] = 0;
         }
    // now we look backwards to find our room name
    // find the pointer to the previous tilde character
    szfind = 0;
    szfind = strrchr(szServer, '~');
    if(szfind)
        { // the portion after the tilde is the room name
        strcpy(szRoomName, szfind+1);
        // chop the room name from the server name
        szServer[szfind-szServer] = 0;
        }
    // now we search for the first forward slash character
    szfind = 0;
    szfind = strstr(szServer, "/");
    if(szfind) 
        {
        szServer[szfind-szServer] = 0;
        strcpy(szFileName, szfind+1);
        strcpy(szImageFile, szfind+1);
        }
    // now we search for the last backslash to get the image file name
    szfind = 0;
    szfind = strrchr(szFileName, '/');
    if(szfind) 
        {
        strcpy(szImageFile, szfind+1);
        strcpy(szFileNumber, szfind+1);
        GetCurrentDirectory(MAX_PATH, (LPTSTR)&szpath);
        strcat(szpath, "\\");
        strcat(szpath, szfind+1);
        strcpy(szImageFile, szpath);
        }
    // also check our other failure conditions
    if(strlen(szServer)    == 0) return FALSE;
    if(strlen(szImageFile) == 0) return FALSE;
    if(strlen(szRoomName)  == 0) return FALSE;
    // add the relevant extensions
    strcat(szImageFile, ".jpg");
    // *******************
//    Blow(szServer);
//    Blow(szFileNumber);
//    Blow(szImageFile);
//    Blow(szRoomName);
    // ******************
    // if this is a valid JPG file that we're downloading, we can start now
    breturn = INGetInternetFile(hwnd, hInst, szServer,  szFileNumber, szImageFile);
    if(breturn) 
        {
        // if we downloaded the JPG file, we must convert and encrypt it now
        breturn = CRConvertAndEncryptImageFile(szImageFile, szRoomName, TRUE);
        }
    // now we must delete the JPG image file regardless of whether we encrypted it
    DeleteFile(szImageFile);

    // now we tell him whether the download was successful or not
    if(breturn)
        {
        INDownloadCompleteDialog(hwnd, hInst);
        }
    else
        {
        MessageBox(hwnd, 
                  "Unable to Download the Requested Grow Room Image File\nfrom the HighGrow web-site.",
                  "Download Problem Encountered", MB_OK);
        }
    return breturn;
    }


//******************************************************************************
//*  SCRAMBLING AND UNSCRAMBLING URLS
//******************************************************************************

BOOL HIScrambleURL(HWND hDlg)
    {
    char szText[MAX_PATH];
    int  ilen;
    // ---------
    // read the text from the input edit control
    ilen = SendDlgItemMessage(hDlg, IDC_ED01, WM_GETTEXT, MAX_PATH, (LPARAM)&szText);
    if(ilen == 0) return FALSE;
    // now scramble the text string
    GLScrambleText((LPSTR)&szText);
    SendDlgItemMessage(hDlg, IDC_ED02, WM_SETTEXT, 0, (LPARAM)szText);
    return TRUE;
    }

BOOL HIUnscrambleURL(HWND hDlg, HINSTANCE hInst)
    {
    char szText[MAX_PATH];
    int  ilen;
    // ---------
    // read the text from the scrambled text edit control
    ilen = SendDlgItemMessage(hDlg, IDC_ED02, WM_GETTEXT, MAX_PATH, (LPARAM)&szText);
    if(ilen == 0) return FALSE;
    EnableWindow(GetDlgItem(hDlg, IDC_BT02), FALSE);
    // now unscramble the text string
    GLUnscrambleText((LPSTR)&szText);
    return INGetInternetRoomImageFile(hDlg, hInst, szText);
    }

//******************************************************************************
//*  BETA TESTING DIALOG FOR DOWNLOADING A ROOM IMAGE FILE FROM THE SERVER
//******************************************************************************

BOOL CALLBACK INDownloadDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   static HFONT     hHdrFont=0;
   const  HINSTANCE hInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);

   switch (message)
      {
      case WM_INITDIALOG:
            {
//            LPSTR lpURL = "http://www.amazes.us/wildlife/images/crackerandme_jpg.jpg";
            // first we show our wait cursor
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            DlgCenter(hDlg);
            hHdrFont = GLCreateDialogFont(20,0,FW_BOLD);
            SendDlgItemMessage(hDlg, IDC_ST01, WM_SETFONT, (WPARAM)hHdrFont, MAKELPARAM(TRUE, 0));
//            SendDlgItemMessage(hDlg, IDC_ED01, WM_SETTEXT, 0, (LPARAM)lpURL);
            EnableWindow(GetDlgItem(hDlg, IDC_BT02), FALSE);           
            } 
      return (TRUE);
      case WM_DESTROY:
           if(hHdrFont) DeleteObject(hHdrFont);
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
          UINT ui = GetDlgCtrlID((HWND)lParam);
          if((ui==IDC_ST01)||(ui==IDC_ED02))
              return FALSE;
          else
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_BT01: 
                {
                if(HIScrambleURL(hDlg))
                   EnableWindow(GetDlgItem(hDlg, IDC_BT02), TRUE);           
                }
            return (FALSE);
 
            case IDC_BT02: 
                {
                EnableWindow(GetDlgItem(hDlg, IDCANCEL), FALSE);           
                EnableWindow(GetDlgItem(hDlg, IDC_BT01), FALSE);           
                if(HIUnscrambleURL(hDlg, hInst))
                    EndDialog(hDlg,TRUE); 
                else
                    {
                    EnableWindow(GetDlgItem(hDlg, IDCANCEL), TRUE);           
                    EnableWindow(GetDlgItem(hDlg, IDC_BT01), TRUE);           
                    }
                }
            return (FALSE);

            case IDCANCEL: 
                  EndDialog(hDlg,FALSE); 
            return (TRUE);
            }  
      return (FALSE);

      }
    return (FALSE); 
   }                          


BOOL INShowDownloadDialog(HWND hDlg, HINSTANCE dhInst)
    {
//    return DialogBox(dhInst, "DOWNLOAD", hDlg, INDownloadDlgProc);
    return FALSE;
    }
