// =================================================================
// ----------------------- TRAYICON.C ------------------------------
// =================================================================

#include <windows.h>
#include "resource.h"
#include "toolbar.h"
#include "registry.h"
#include "harvest.h"
#include "trayicon.h"
#include "highgrow.h"
#include "htmlhelp.h"

HMENU hTrayMenu = NULL;
HICON hTrayIcon = NULL;
BOOL  gbIsMainWindowHidden=FALSE; // global flag shows hidden status

// -----------------------------------------------------------------
// MyTaskBarAddIcon - adds an icon to the taskbar status area.
// Returns TRUE if successful, or FALSE otherwise.
// hwnd - handle to the window to receive callback messages.
// uID - identifier of the icon.
// hicon - handle to the icon to add.
// lpszTip - ToolTip text.
// -----------------------------------------------------------------
      
BOOL TRTaskBarAddIcon(HWND hwnd, HINSTANCE hInst, LPSTR lpszTip)
    {
    BOOL res;
    NOTIFYICONDATA tnid;

    hTrayIcon = LoadIcon(hInst, "highgrow");
    hTrayMenu = GetSubMenu(LoadMenu(hInst, "TrayMenu"), 0);

    tnid.cbSize = sizeof(NOTIFYICONDATA);
    tnid.hWnd = hwnd;
    tnid.uID = IDI_HIGHGROW;
    tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    tnid.uCallbackMessage = MYWM_NOTIFYICON;
    tnid.hIcon = hTrayIcon;

    if(lpszTip)
         lstrcpyn(tnid.szTip, lpszTip, sizeof(tnid.szTip));
    else tnid.szTip[0] = '\0';

    res = Shell_NotifyIcon(NIM_ADD, &tnid);

    if (hTrayIcon)  DestroyIcon(hTrayIcon);
    return res;
    }


// -----------------------------------------------------------------
// MyTaskBarDeleteIcon - deletes an icon from the taskbar
//     status area.
// Returns TRUE if successful, or FALSE otherwise.
// hwnd - handle to the window that added the icon.
// uID - identifier of the icon to delete.
// -----------------------------------------------------------------


BOOL TRTaskBarDeleteIcon(HWND hwnd)
    {
    BOOL res;
    NOTIFYICONDATA tnid;

    if(hTrayIcon)  DeleteObject(hTrayIcon);
    if(hTrayMenu)  DeleteObject(hTrayMenu);

    tnid.cbSize = sizeof(NOTIFYICONDATA);
    tnid.hWnd = hwnd;
    tnid.uID = IDI_HIGHGROW;

    res = Shell_NotifyIcon(NIM_DELETE, &tnid);
    return res;
    }

// --------------------------------
// MENU ACTION ACTION
// --------------------------------

void TRShowHideMainWin(HWND hWnd, HINSTANCE hInst)
    {
    UINT uiState;
    uiState = GetMenuState(GetMenu(hWnd), IDM_GROWROOM, MF_BYCOMMAND);

    if(!IsWindowVisible(hWnd))
         { // if the main window is currently hidden...
         ShowWindow(hWnd, SW_SHOW);
         UpdateWindow(hWnd);
         if(uiState != MF_CHECKED)
              DoCommand(hWnd, MAKEWPARAM(0, IDM_GROWROOM), hInst);
         }
    else ShowWindow(hWnd, SW_HIDE);
    }

// -----------------------------------------------------------------
// On_MYWM_NOTIFYICON -processes callback messages for taskbar icons
// wParam - first message parameter of the callback message.
// lParam - second message parameter of the callback message.
// -----------------------------------------------------------------

void TR_MYWM_NOTIFYICON(HWND hWnd, HINSTANCE hInst, WPARAM wParam, LPARAM lParam)
    {
    UINT uID;
    UINT uMouseMsg;
    UINT uiState;

    uID       = (UINT) wParam;
    uMouseMsg = (UINT) lParam;
    uiState   = GetMenuState(GetMenu(hWnd),IDM_GROWROOM,MF_BYCOMMAND);

    if((uMouseMsg == WM_LBUTTONDOWN) || (uMouseMsg == WM_RBUTTONDOWN))
        {
        if(uID==IDI_HIGHGROW)
            {
            POINT pt;
            UINT  uMID;
            MENUITEMINFO mmi;
            char  szMenuText[100]="\0";

            // first we'll read the mouse cursor position
            GetCursorPos(&pt);

            // now we adjust the text on the IDM_GROWROOM menu item
            if(IsWindowVisible(hWnd))
                 strcpy(szMenuText, "&Exit");
            else strcpy(szMenuText, "&Enter");
            strcat(szMenuText, " Grow Room");
            // first we fill out the menu item struct info
            mmi.cbSize = sizeof(MENUITEMINFO);
            mmi.fMask  = MIIM_TYPE|MIIM_STATE;
            mmi.fType  = MFT_STRING;
            mmi.dwTypeData = szMenuText;
            if(uiState != MF_GRAYED)
                 mmi.fState = MFS_ENABLED;
            else mmi.fState = MFS_GRAYED;
            mmi.fState = mmi.fState|MFS_DEFAULT;

            // now we can change the menu's text
            SetMenuItemInfo(hTrayMenu, IDM_GROWROOM, 0, &mmi);

            // now we can show the menu which returns the selected item
            uMID = TrackPopupMenu(hTrayMenu,
                               TPM_RIGHTALIGN|TPM_BOTTOMALIGN|
                               TPM_RETURNCMD|TPM_LEFTBUTTON,
                               pt.x, pt.y, 0, hWnd, NULL);
            // here we perform the action for the selected menu item
            switch (uMID)
                {
                case IDM_GROWROOM:
                     TRShowHideMainWin(hWnd, hInst);
                return;
                case IDM_HELPCONTENTS:
                     HtmlHelp(hWnd, "HighGrow.chm", HH_DISPLAY_TOPIC, (DWORD)"Contents.htm");
                return;
                case IDM_ABOUT:
                     HAShowAboutDialog(GetDesktopWindow(), hInst);
                return;
                case IDM_EXIT:
                     PostMessage(hWnd, WM_CLOSE, 0, 0);
                return;
                }
            }
         }
     }

