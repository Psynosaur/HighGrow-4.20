/******************************************************************************\
*
*  Password.c - setting and check for the growroom password
*
\******************************************************************************/

#include <windows.h>
#include <commctrl.h>
#include "stdio.h"
#include "resource.h"
#include "password.h"
#include "registry.h"
#include "global.h"
#include "seedlist.h"
#include "dibitmap.h"
#include "htmlhelp.h"
#include "clock.h"
   
int giOldestGrowDay; // oldest plant's growday

int        giPasswordStyle;   // current style we're dealing with
BOOL       gbChangePassword; // true if he wants to change his password
         
char       gszCurPassword[20]; // the password protecting this program
//LPCSTR     lpRegiserCode ="273-8472561-326";
BOOL       gbGodMode=FALSE;     // true if his password is "XXXXXX"

HFONT      hPassFont;
HFONT      hCheatFont;
HIMAGELIST hPassLeafImage;  // handle to image list for password leaf
HIMAGELIST hNoEntryImage;   // handle to image list for no-entry symbol


/******************************************************************************\
*  Initializing the password dialog before showing it
\******************************************************************************/

void PAInitDialogControls(HWND hDlg)
    {
    LPCSTR lpTitle, lpOK, lpCancel;

    if(giPasswordStyle==IDC_EDCHECK)
        {
        lpTitle  ="Enter Your Password";
        lpOK     ="&Check Password";
        SetFocus(GetDlgItem(hDlg, IDC_EDCHECK));
        ShowWindow(GetDlgItem(hDlg, IDC_EDCHECK), SW_SHOW);
        ShowWindow(GetDlgItem(hDlg, IDC_EDSET),   SW_HIDE);
        }
    else
        {
        lpTitle  ="Enter New Password";
        lpOK     ="&Set Password";
        SetFocus(GetDlgItem(hDlg, IDC_EDSET));
        ShowWindow(GetDlgItem(hDlg, IDC_EDCHECK), SW_HIDE);
        ShowWindow(GetDlgItem(hDlg, IDC_EDSET),   SW_SHOW);
        // if he want's to change his password, change the cancel button's text
        if(gbChangePassword)
            {
            lpCancel ="&Remove Password";
            SendDlgItemMessage(hDlg, IDCANCEL, WM_SETTEXT, 0, (LPARAM)lpCancel);
            }
        }
    SendDlgItemMessage(hDlg, giPasswordStyle, EM_LIMITTEXT, 18, 0);
    SendDlgItemMessage(hDlg, giPasswordStyle, WM_SETTEXT, 0, (LPARAM)NULL);
    SendDlgItemMessage(hDlg, IDC_ST01, WM_SETTEXT, 0, (LPARAM)lpTitle);
    SendDlgItemMessage(hDlg, IDC_ST03, WM_SETTEXT, 0, (LPARAM)lpTitle);
    SendDlgItemMessage(hDlg, IDOK,     WM_SETTEXT, 0, (LPARAM)lpOK);
    }


void PAInitPasswordDialog(HWND hDlg, HINSTANCE hInst)
    {
    DlgCenter(hDlg);
    InitCommonControls();

    hPassFont = GLCreateDialogFont(24,0,FW_BOLD);
    SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hPassFont,MAKELPARAM(TRUE, 0));

    hPassLeafImage = ImageList_LoadBitmap(hInst,"PassLeaf",245,0,RGB(255,255,255));
    hNoEntryImage  = ImageList_LoadBitmap(hInst,"NoEntry", 175,0,RGB(255,255,255));
    // now show the required controls in this dialog
    PAInitDialogControls(hDlg);
    }


/******************************************************************************\
*  Reading the password from the registry & unscrambling it into our global var
\******************************************************************************/


BOOL PAGetRegistryPassword(void)
    { 
    // this function gets the password and unscrambles it.
    // returns true if a password was read and unscrambled
    int i, iLen; // length of his password
    // get the password he entered in the edit control
    gszCurPassword[0]=0;
    REReadRegistryKey("Password", (LPCTSTR)gszCurPassword, 20);
    iLen = strlen(gszCurPassword);
    for(i=0;i<iLen;i++) // here we subtract one from every character
        gszCurPassword[i] = gszCurPassword[i] -1; 
    // now return a true if we have any length
    return (BOOL)iLen;
    }


/******************************************************************************\
*  Scrambling and Saving the new password and in the registry
\******************************************************************************/

BOOL PASaveRegistryPassword(HWND hDlg)
    { // here we save the password if it's longer than 4 characters long
    int i, iLen; // length of his password
    // get the password he entered in the edit control
    iLen = SendDlgItemMessage(hDlg, IDC_EDSET, WM_GETTEXT, 20, (LPARAM)gszCurPassword);
    // now check if it's a valid length
    if(iLen < 4)
        { // tell him that he should re-enter it
        MessageBox(hDlg, "Passwords should be at least four characters long!!", 
                   "Warning", MB_OK|MB_ICONEXCLAMATION);
        // erase his previous password and set the 
        // focus back to the relevant edit control
        SendDlgItemMessage(hDlg, giPasswordStyle, WM_SETTEXT, 0, (LPARAM)NULL);
        SetFocus(GetDlgItem(hDlg, giPasswordStyle));
        return FALSE;
        }
    // ok, it's a valid length, scamble and save it
    for(i=0;i<iLen;i++) // but we'll add one to every other character
       gszCurPassword[i] = gszCurPassword[i] + 1; 
    // now save this slightly scrambled password in the registry
    RESaveRegistryKey("Password", gszCurPassword);
    // here we reset the password again by subtracting one
    for(i=0;i<iLen;i++) // here we subtract one from every character
        gszCurPassword[i] = gszCurPassword[i] -1; 
    // and return true
    return TRUE;
    }


/******************************************************************************\
*  Checking if he can continue by pressing the ok button
\******************************************************************************/


BOOL PAExitDialogCheck(HWND hDlg)
    {
    int iLen;
    char szPassword[20]; 
    // if setting new password, we check for a valid length and save it if ok
    if(giPasswordStyle==IDC_EDSET)
        return PASaveRegistryPassword(hDlg);
    // not the setting dialog, so check the password that he entered
    iLen= SendDlgItemMessage(hDlg, IDC_EDCHECK, WM_GETTEXT, 20, (LPARAM)szPassword);
    // compare his password with the registry on and return false if different
    if(strcmp(szPassword, gszCurPassword)!=0)
        {
        MessageBox(hDlg, "Invalid Password Entered!!", 
                   "Warning", MB_OK|MB_ICONEXCLAMATION);
        SendDlgItemMessage(hDlg, IDC_EDCHECK, WM_SETTEXT, 0, (LPARAM)NULL);
        SetFocus(GetDlgItem(hDlg, IDC_EDCHECK));
        return FALSE;
        }
    // we passed all the tests, return a true to exit
    return TRUE;
    }


/******************************************************************************\
*  Drawing the Password Leaf and noentry images
\******************************************************************************/


void PADrawPasswordImage(HWND hwnd, HDC hdc)
    {
    RECT rc;
    int  iW, iH;
    // first get the size of our window
    GetClientRect(hwnd, &rc);
    // now fill our window with his button colour, to erase any previous image
    FillRect(hdc, &rc, GetSysColorBrush(COLOR_3DFACE));
    // now we draw the correct images
    iW = ((rc.right-rc.left)/2);
    iH = ((rc.bottom-rc.top)/2);
    if(hPassLeafImage)
        {
        ImageList_Draw(hPassLeafImage, 0, hdc, 
                        iW-(245/2), iH-(230/2), ILD_TRANSPARENT);
        }
    if(giPasswordStyle==IDC_EDCHECK && hNoEntryImage != NULL)
        {
        ImageList_Draw(hNoEntryImage, 0, hdc,
                        iW-(175/2), iH-(175/2), ILD_TRANSPARENT);
        }
    }


/******************************************************************************\
*  The Password Dialog Proc
\******************************************************************************/

BOOL CALLBACK PasswordDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   
    switch (message)
      {
      case WM_INITDIALOG:
           PAInitPasswordDialog(hDlg, dhInst);
      return (FALSE);  // only because we redirect the focus to the edit

      case WM_DESTROY:
            if(hPassFont)      DeleteObject(hPassFont);
            if(hPassLeafImage) ImageList_Destroy(hPassLeafImage);
            if(hNoEntryImage)  ImageList_Destroy(hNoEntryImage);
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
          if(GetDlgCtrlID((HWND)lParam) == IDC_ST02)
              {
              PADrawPasswordImage((HWND)lParam, (HDC)wParam);
              return (FALSE);
              }
          SetBkMode((HDC)wParam, TRANSPARENT);
          return (int)GetStockObject(NULL_BRUSH);
          }
      return (FALSE);

      case WM_COMMAND:
          switch (wParam)
            {
            case IDOK:
              if(PAExitDialogCheck(hDlg)==TRUE)
                  EndDialog(hDlg, TRUE); 
            return TRUE;

            case IDCANCEL:
                // if he want's to remove his password, we'll do it now
                if(gbChangePassword==TRUE)
                    { 
                    RESaveRegistryKey("Password", ""); // zero his existing password
                    gszCurPassword[0]= 0; // and initialize our global variable
                    }
                 EndDialog(hDlg, FALSE); 
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE); 
   }                          

/******************************************************************************\
*  Checks or Sets the Sound Option
\******************************************************************************/


BOOL PACheckSoundEnabled(void)
    { // returns true if he has the sound enabled
    char szSound[10];
    REReadRegistryKey("Sound Option", (LPCTSTR)szSound, 10);
    if(strcmp(szSound, "Off")==0) 
        return FALSE;
    return TRUE;
    }


void PASetSoundEnabled(BOOL bEnabled)
    { // sets or unsets the sound option
    char szSound[10];
    if(bEnabled)
         strcpy(szSound, "On");
    else strcpy(szSound, "Off");
    RESaveRegistryKey("Sound Option", (LPCTSTR)szSound);
    }


void PAToggleSoundOption(void)
    {
    if(PACheckSoundEnabled())
         PASetSoundEnabled(FALSE);
    else PASetSoundEnabled(TRUE);
    }


/******************************************************************************\
*  Checking and setting the type of sound file (MIDI or MP3)
\******************************************************************************/

BOOL gbMusicFileType; // zero = midi, 1=mp3 (default is midi)

void PASetSoundFileType(BOOL bMIDI)
    { // sets or unsets the sound option
    char szType[10];
    if(bMIDI)
        {
        strcpy(szType, "MIDI");
        gbMusicFileType = 0;
        }
    else
        {
        gbMusicFileType = TYPE_MP3;
        strcpy(szType, "MP3");
        }
    RESaveRegistryKey("Music File Type", (LPCTSTR)szType);
    }

BOOL PACheckMIDISoundFileType(void)
    { // sets or unsets the sound option
    char szType[10];
    // -------------
    gbMusicFileType = 0;
    REReadRegistryKey("Music File Type", (LPCTSTR)szType, 10);
    if(strcmp(szType, "MP3")==0) 
        {
        gbMusicFileType = TYPE_MP3;
        return FALSE;
        }
    return TRUE;
    }

/******************************************************************************\
*  Checking and setting the BoomBox easter egg image
\******************************************************************************/

BOOL gbBoomBox; // visible or not

void PASetBoomBox(BOOL bEnabled)
    { // sets or unsets the sound option
    char szText[10];
    if(bEnabled)
        {
        strcpy(szText, "Visible");
        gbBoomBox = TRUE;
        }
    else
        {
        strcpy(szText, "Hidden");
        gbBoomBox = FALSE;
        }
    RESaveRegistryKey("BoomBox", (LPCTSTR)szText);
    }

void PACheckBoomBox(void)
    { // sets or unsets the sound option
    char szText[10];
    // -------------
    gbBoomBox = FALSE;
    REReadRegistryKey("BoomBox", (LPCTSTR)szText, 10);
    if(strcmp(szText, "Visible")==0) 
        {
        gbBoomBox = TRUE;
        }
    }



/******************************************************************************\
*  Checks if auto-load has been enabled
\******************************************************************************/

BOOL PACheckAutoLoad(void)
    {
    char szAutoLoad[10];
    REReadRegistryKey("Auto-Load", (LPCTSTR)szAutoLoad, 10);
    if(strcmp(szAutoLoad, "Yes")==0) 
        return TRUE;
    return FALSE;
    }

void PASetAutoLoad(BOOL bEnabled)
    { // sets or unsets the Auto-load option
    char szAutoLoad[10];
    if(bEnabled)
         strcpy(szAutoLoad, "Yes");
    else strcpy(szAutoLoad, "No");
    RESaveRegistryKey("Auto-Load", (LPCTSTR)szAutoLoad);
    }

void PAToggleAutoLoadOption(void)
    {
    if(PACheckAutoLoad())
         PASetAutoLoad(FALSE);
    else PASetAutoLoad(TRUE);
    }


/******************************************************************************\
*  Checks if auto-growroom has been enabled
\******************************************************************************/

void PASetAutoGrowroom(BOOL bEnabled)
    { // sets or unsets the Auto-load option
    char szAutoGrowroom[10];
    if(bEnabled)
         strcpy(szAutoGrowroom, "Yes");
    else strcpy(szAutoGrowroom, "No");
    RESaveRegistryKey("Auto-Growroom", (LPCTSTR)szAutoGrowroom);
    }

BOOL PACheckAutoGrowroom(void)
    {
    BOOL bFlag=FALSE;
    char szAutoGrowroom[10];
    REReadRegistryKey("Auto-Growroom", (LPCTSTR)szAutoGrowroom, 10);
    if(strcmp(szAutoGrowroom, "Yes")==0) 
        bFlag = TRUE;
    // now we must set this, because our setup program checks this
    // to see if we have version 2.0. If he never toggles it, it
    // never gets set
    PASetAutoGrowroom(bFlag);
    return bFlag;
    }

void PAToggleAutoGrowroomOption(void)
    {
    if(PACheckAutoGrowroom())
         PASetAutoGrowroom(FALSE);
    else PASetAutoGrowroom(TRUE);
    }


/******************************************************************************\
*  Checks if the vacation mode has been enabled
\******************************************************************************/

BOOL PACheckVacModeEnabled(void)
    {
    char szMode[10];
    REReadRegistryKey("Vacation Mode", (LPCTSTR)szMode, 10);
    if(strcmp(szMode, "Yes")==0) 
        return TRUE;
    return FALSE;
    }

void PASetVacModeEnabled(BOOL bEnabled)
    { // sets or unsets the Auto-load option
    char szMode[10];
    if(bEnabled)
         strcpy(szMode, "Yes");
    else strcpy(szMode, "No");
    RESaveRegistryKey("Vacation Mode", (LPCTSTR)szMode);
    }

void PAToggleVacModeOption(void)
    {
    if(PACheckVacModeEnabled())
         PASetVacModeEnabled(FALSE);
    else PASetVacModeEnabled(TRUE);
    }

/******************************************************************************\
*  Checks if the alarm clock has been set
\******************************************************************************/

int  giAlarmMinute = 0;
BOOL bAlarmSet = FALSE;

void PASetAlarmMinute(void)
    {
    char szMode[200];
    int ihrs, imin, ibn;
    // ---
    // first ensure our global is zeroed
    giAlarmMinute = 0;
    // read the number of hours value
    REReadRegistryKey("Alarm Hours", (LPCTSTR)szMode, 20);
    ihrs = atoi(szMode); // convert the string to a number
    // read the number of minutes value
    REReadRegistryKey("Alarm Minutes", (LPCTSTR)szMode, 20);
    imin = atoi(szMode); // convert the string to a number
    // read the AM/PM value value
    REReadRegistryKey("Alarm AMPM", (LPCTSTR)szMode, 20);
    ibn = atoi(szMode); // convert the string to a number
    // adjust hours if in afternoon
    if(ibn) ihrs += 12;
    // now set our global minutes
    giAlarmMinute = (ihrs*60)+imin;
    }


BOOL PACheckAlarmTime(int iSecs)
    { // check if we must sound the Alarm Clock
    if(bAlarmSet)
        { // if the Alarm is enabled, and not yet sounded
        int iAlarmSecs = giAlarmMinute*60;
        // if we're within 2 secs after our intended time
        if((iAlarmSecs <= iSecs)&&(iAlarmSecs+2 >= iSecs))
            {
            bAlarmSet = FALSE;
            return TRUE;
            }
        }
    return FALSE;
    }


BOOL PACheckAlarmSet(void)
    {
    char szMode[10];
    REReadRegistryKey("Alarm Clock", (LPCTSTR)szMode, 10);
    if(strcmp(szMode, "Yes")==0) 
        {
        bAlarmSet = TRUE;
        PASetAlarmMinute();
        return TRUE;
        }
    bAlarmSet = FALSE;
    return FALSE;
    }

void PASetAlarmClock(BOOL bEnabled)
    { // sets or unsets the Alarm clock
    char szMode[10];
    bAlarmSet = bEnabled;
    if(bEnabled)
        strcpy(szMode, "Yes");
    else strcpy(szMode, "No");
    RESaveRegistryKey("Alarm Clock", (LPCTSTR)szMode);
    }

void PASaveAlarmSettings(LPSTR sztext, int ihrs, int imin, int ibn, BOOL bSet)
    {
    char szMode[200];
    // ------
    // first set the alarm on/off
    if(bSet)
         strcpy(szMode, "Yes");
    else strcpy(szMode, "No");
    RESaveRegistryKey("Alarm Clock", (LPCTSTR)szMode);
    // now set Rasta Robbie's text
    RESaveRegistryKey("Alarm Text", (LPCTSTR)sztext);
    // now set Hours's value
    wsprintf(szMode, "%i", ihrs);
    RESaveRegistryKey("Alarm Hours", (LPCTSTR)szMode);
    // now set Minutes's value
    wsprintf(szMode, "%i", imin);
    RESaveRegistryKey("Alarm Minutes", (LPCTSTR)szMode);
    // now set the Before Noon value
    wsprintf(szMode, "%i", ibn);
    RESaveRegistryKey("Alarm AMPM", (LPCTSTR)szMode);
    }


void PALoadAlarmSettings(LPSTR sztext, int* ihrs, int* imin, int* ibn, int* iset)
    {
    char szMode[200];
    // ------
    // check if the alarm is set
    *iset = PACheckAlarmSet();
    // read Robbie's words of wisdom
    REReadRegistryKey("Alarm Text", (LPCTSTR)szMode, 200);
    strcpy(sztext, szMode);
    // read the number of hours value
    REReadRegistryKey("Alarm Hours", (LPCTSTR)szMode, 20);
    *ihrs = atoi(szMode); // convert the string to a number
    // read the number of minutes value
    REReadRegistryKey("Alarm Minutes", (LPCTSTR)szMode, 20);
    *imin = atoi(szMode); // convert the string to a number
    // read the AM/PM value value
    REReadRegistryKey("Alarm AMPM", (LPCTSTR)szMode, 20);
    *ibn = atoi(szMode); // convert the string to a number
    }


void PALoadAlarmText(LPSTR sztext)
    {
    char szText[150];
    // read Robbie's words of wisdom
    REReadRegistryKey("Alarm Text", (LPCTSTR)szText, 150);
    strcpy(sztext, szText);
    }


void PAToggleAlarmClockOption(void)
    {
    if(PACheckAlarmSet())
         PASetAlarmClock(FALSE);
    else PASetAlarmClock(TRUE);
    }



/******************************************************************************\
*  Read the password from registry set the gbGodMode flag
\******************************************************************************/
/*
void PACheckGodMode(void)
    { // if he has a password set, we'll check if it's the godmode password
    // we must do this before doing any growth calculations
    if(PAGetRegistryPassword())
        { // check if he's set his password to "420024"
        if((gszCurPassword[0]==52)&&(gszCurPassword[1]==50)&&
           (gszCurPassword[2]==48)&&(gszCurPassword[3]==48)&&
           (gszCurPassword[4]==50)&&(gszCurPassword[5]==52))
              gbGodMode    = TRUE;
        }
    }
*/
/******************************************************************************\
*  Changing the coded password with an unscrambling technique
\******************************************************************************/
/*
void PAUnscrambleCode(char* szCode)
    { // 273-8472561-326 becomes 162-8472561-104, and then the middle 7 digits
    // are adjusted to become Alphabetic, by adding an adjustment value of
    // between 1 and 17 to each digit and then adding it to 65 (ASCII A)
    // The serial number or volume label is used to get a constant but varying
    // number to use. (My development machine the code is 162-PLOJMNI-104)
    int i;
    int iAdjustment=0;
    char szVolumeBuf[255]="\0";
    char szSerialBuf[255]="\0";
    char szRootPath[255]="\0";
    DWORD   dwSerial;
    LPDWORD lpDW = &dwSerial;

    strcpy(szCode, "273-8472561-326");
    // first check the drive where we're growing from
    REReadRegistryKey("Startup in", (LPCTSTR)szRootPath, 255);
    szRootPath[2]=0; // we only require the drive letter
    strcat(szRootPath, "\\"); // and 2 backslashes
    // now let's get some information unique to this drive
    GetVolumeInformation((LPCTSTR)&szRootPath, (LPTSTR)&szVolumeBuf, 255, lpDW, 
                        (LPDWORD)NULL, (LPDWORD)NULL, (LPTSTR)NULL, 0);
    if(dwSerial != 0) // if we read his serial number correctly..
        { // first turn the number into a string
        wsprintf(szSerialBuf, "%d", dwSerial);
        for(i=0;i<(int)strlen(szSerialBuf);i++) 
             { // now add up all the numbers in the string
             iAdjustment += szSerialBuf[i] - 48;
             }
        }
    else  // we didn't read his serial number
        { // so we'll check his drive's volume lable
        if(strlen(szVolumeBuf)!=0)
             { // if we read his volume label..
             for(i=0;i<(int)strlen(szVolumeBuf);i++) 
                 { // now add up all the numbers in the string
                 iAdjustment += szVolumeBuf[i] - 65;
                 }
             }
        else { // no volume label either, let's add 1
             iAdjustment = 1;
             }
        }
    // now we must check that this number does not exceed our limit
    // as we're converting numbers to text, we cannot exceed 17 (9+17=26)
    while(iAdjustment >= 17) 
        iAdjustment = iAdjustment/2;
    // also check it's not less than 1
    iAdjustment = max(1, iAdjustment);
    // ** now we can start adjusting our code
    // first subtract 1 from the first section
    for(i=0;i<3;i++)
        szCode[i] = szCode[i] - 1;
    // now add our adjustment value to each of the second section
    for(i=4;i<11;i++)
        szCode[i] = szCode[i] + iAdjustment + 16;
    // now subtract 2 from the last section
    for(i=12;i<15;i++)
        szCode[i] = szCode[i] - 2;
    // and add a null at the end
    szCode[15]=0;
    }
*/
/******************************************************************************\
*  Check if he's entered the correct registration unlocking code
\******************************************************************************/
/*
void PACheckRegistered(void)
    { // if he has a password set, we'll check if it's the Registration code
    // we must do this before doing any growth calculations
    gszCurPassword[0]=0;
    REReadRegistryKey("Registration Code", (LPCTSTR)gszCurPassword, 20);
    if(strlen(gszCurPassword))
        { // first we'll scramble the coded password
        char szCode[20];
        PAUnscrambleCode(szCode);
        // now check if he's set his password to our valid registration code
        if(strcmp(gszCurPassword, szCode)==0)
            gbRegistered = TRUE;
        }
    }
*/
/******************************************************************************\
*  Read the password from registry & calls password entry dialog if there is one
\******************************************************************************/

BOOL PACheckPassword(HWND hwnd, HINSTANCE hInst)
    { // if he has a password set, we'll ask for it here.
    // we'll return true if no password set, or he successfully cracked it
    if(PAGetRegistryPassword())
        { // check if he's set his password to 
//        if((gszCurPassword[0]==52)&&(gszCurPassword[1]==50)&&
//           (gszCurPassword[2]==48)&&(gszCurPassword[3]==48)&&
//           (gszCurPassword[4]==50)&&(gszCurPassword[5]==52))
//              {
//              gbGodMode = TRUE;
//              MessageBox(hwnd, "God Mode Successfully Enabled!!", 
//                               "Oh Master...", MB_OK|MB_ICONEXCLAMATION);
//              return TRUE;
//              }
        // if he's not god, we ask him for his password
        gbChangePassword = FALSE;
        giPasswordStyle  = IDC_EDCHECK;  // the id of the password entry edit control
        return DialogBox(hInst, "Password", hwnd, (DLGPROC)PasswordDlgProc);
        }
    // he has no password set, let him continue
    return TRUE;
    }


/******************************************************************************\
*  Setting a Password, after entering any existing passwords
\******************************************************************************/

void PASetPassword(HWND hwnd, HINSTANCE hInst)
    {
    if(PAGetRegistryPassword())
         gbChangePassword = TRUE;
    else gbChangePassword = FALSE;

    giPasswordStyle = IDC_EDSET;  // the id of the new password edit control
    DialogBox(hInst, "Password", hwnd, (DLGPROC)PasswordDlgProc);
    }



/******************************************************************************\
*  Showing and managing the Cheat Dialog
\******************************************************************************/

int giCheatType = 0; // see next lines for details
// 1=start prior to last startup
// 2=start within 60secs of new day
// 3=Date change while program running

void PAInitCheatDialog(HWND hDlg, HINSTANCE hInst)
    {
    char szDate[20];  // holds last valid date
    char szText[128]; // holds next valid date message
    char szCheat[255]; // holds alternative error message

    DlgCenter(hDlg);
    InitCommonControls();
    if(giCheatType==1) // started prior to prev startup?
        {
        // read the next valid entry date
        REReadRegistryKey("Last Entered", (LPCTSTR)szDate, 20);
        }
    else // else his cheat check has run out
        {
        char szDateBuf[4];
        GLDateNow((char*)&szDateBuf);
        GLNextFormatDate((char*)&szDateBuf, (char*)&szDate, 1);
        }
    // check which type of cheating he's doing
    if(giCheatType==1) // we'll only alter text for this type
        {
        LoadString(hInst, IDS_CH01, szCheat, 254);
        SendDlgItemMessage(hDlg, IDC_ST03, WM_SETTEXT, 0, (LPARAM)szCheat);
        }
    // now show this date on the OK button
    wsprintf(szText, "Next Valid Entry on %s", szDate);
    SendDlgItemMessage(hDlg, IDOK, WM_SETTEXT, 0, (LPARAM)szText);
    // set the style for drawing the noentry image
    giPasswordStyle  = IDC_EDCHECK;  // the id of the password entry edit control
    // create and set a bolder font to the warning message
    hCheatFont = GLCreateDialogFont(18,0,FW_BOLD);
    SendDlgItemMessage(hDlg,IDC_ST03,WM_SETFONT,(WPARAM)hCheatFont,MAKELPARAM(TRUE, 0));
    // load our images into our imagelist
    hPassLeafImage = ImageList_LoadBitmap(hInst,"PassLeaf",245,0,RGB(255,255,255));
    hNoEntryImage  = ImageList_LoadBitmap(hInst,"NoEntry", 175,0,RGB(255,255,255));
    }


BOOL CALLBACK CheatDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);
   
    switch (message)
      {
      case WM_INITDIALOG:
           PAInitCheatDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
            if(hCheatFont)     DeleteObject(hCheatFont);
            if(hPassLeafImage) ImageList_Destroy(hPassLeafImage);
            if(hNoEntryImage)  ImageList_Destroy(hNoEntryImage);
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
          if(GetDlgCtrlID((HWND)lParam) == IDC_ST02)
              {
              PADrawPasswordImage((HWND)lParam, (HDC)wParam);
              return (FALSE);
              }
          SetBkMode((HDC)wParam, TRANSPARENT);
          return (int)GetStockObject(NULL_BRUSH);
          }

      case WM_COMMAND:
          switch (wParam)
            {
            case IDOK:
                 EndDialog(hDlg, TRUE); 
            return TRUE;

            case IDC_BT01:
                 HtmlHelp(hDlg, "HighGrow.chm", HH_DISPLAY_TOPIC, 
                         (DWORD)"Essential_things_to_remember.htm");
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE); 
   }                          

/******************************************************************************\
*  Writing important cheat checks into the registry at the start and end
\******************************************************************************/

void PASaveEndCheatChecks(void)
    { // setting cheat checks into the registry at program termination
    char szKeyVal[20];
    // first set the current growday, for our backwards clock check
    wsprintf(szKeyVal, "%i", giOldestGrowDay);
    RESaveRegistryKey("Oldest Plant GrowDay", szKeyVal);
    // now set the seconds since midnight, for our day skip check
    wsprintf(szKeyVal, "%i", GLGetSecsSinceMidnight());
    RESaveRegistryKey("Seconds Elapsed", szKeyVal);
    }


BOOL PAStartCheatCheck(HWND hwnd, HINSTANCE hInst)
    { // ** THIS MUST BE DONE AFTER CALCULATING THE PLANTS **
    // reading the registry and checking cheat conditions at program startup
    // If he's cheating we put up our cheat dialog and pass a false flag back
    char szKeyCheck[20]; // holds anything we read
    char szFile[128];    // holds filename spec
    int  i, iSecs, iSecCheck, iDayCheck; // holds our numbers
    int iCheats; // number of times he's already cheated
    BOOL bAnyPlants=FALSE; // we reset cheats if we have no plants
    // if godmode is enabled, we wont do any cheat checks
//    if(gbGodMode==TRUE) return TRUE; 
    // first reset this global
    giCheatType = 0;
    // first check if he's cheated yet
    szKeyCheck[0]=0; // zero this incase
    REReadRegistryKey("Cheat Count", (LPCTSTR)szKeyCheck, 20);
    iCheats = atoi(szKeyCheck); // convert the string to a number
    szKeyCheck[0]=0; // zero this incase
    // first we check if we even have any plants to calculate
    // so as not to calculate and show progress for no plants
    for(i=1;i<gMaxPlants+1;i++)
        {
        sprintf(szFile, "Plant%02i.hgp", i);
        if(GLDoesFileExist(szFile)) bAnyPlants = TRUE;
        }
    // now if we have no plants, we must reset some stuff
    if(bAnyPlants==FALSE)
        {
        wsprintf(szKeyCheck, "%i", 0);
        RESaveRegistryKey("Cheat Count", szKeyCheck);
        RESaveRegistryKey("Oldest Plant GrowDay", szKeyCheck);
        return TRUE;
        }
    // first read the oldest plant growday value from the regitry
    REReadRegistryKey("Oldest Plant GrowDay", (LPCTSTR)szKeyCheck, 20);
    if(strlen(szKeyCheck)==0) // this can only happen the very first time we..
        return TRUE;          // ..startup so, pass a true and getout
    // now set our last stored oldest day value
    iDayCheck = atoi(szKeyCheck);
    // now check if he's started up earlier than the oldest day
    if(iDayCheck > giOldestGrowDay)
        { // we tell him to pissoff and return a false
        giCheatType = 1; // set this to indicate which text to display
        DialogBox(hInst, "Cheat", hwnd, (DLGPROC)CheatDlgProc);
        return FALSE;
        }
    // if this is after our last started date, we'll check the seconds elapsed
    if(iDayCheck < giOldestGrowDay)
        { // read the registry for when he last ended
        // check the time difference between last ended and now
        REReadRegistryKey("Seconds Elapsed", (LPCTSTR)szKeyCheck, 20);
        iSecCheck = atoi(szKeyCheck); // convert the string to a number
        iSecs     = GLGetSecsSinceMidnight();
        // he's started up the next day within 1 minute
        if((iSecs>iSecCheck) && iSecs<(iSecCheck+60))
            { // we must check how many times he's done this
            // read how many times he's already cheated this way
            wsprintf(szKeyCheck, "%i", iCheats+1);
            RESaveRegistryKey("Cheat Count", szKeyCheck);
            if(iCheats >= 3) // if he's already cheated three times this way
                {
                giCheatType = 2; // set this to indicate which text to display
                DialogBox(hInst, "Cheat", hwnd, (DLGPROC)CheatDlgProc);
                return FALSE;
                }
            return TRUE;
            }
        else  // everytime he waits one minute between starts, we'll decrease 
            { // the cheat count
            wsprintf(szKeyCheck, "%i", iCheats-1);
            RESaveRegistryKey("Cheat Count", szKeyCheck);
            return TRUE;
            }
        }
    else
        {
        if(iCheats >= 5) // if he's already cheated five times this way
            {
            DialogBox(hInst, "Cheat", hwnd, (DLGPROC)CheatDlgProc);
            return FALSE;
            }
        }
    return TRUE;
    }


/******************************************************************************\
*  Performing startup checks for allowing/preventing program access
\******************************************************************************/

BOOL PACheckProgramAccess(HWND hwnd, HINSTANCE hInst)
    {
    char szdatebuf[20];

    // first check if he's entered a password (or god?)
    if(PACheckPassword(hwnd, hInst)==FALSE)
       return FALSE;
    // now check if he's cheated in any way
    if(PAStartCheatCheck(hwnd, hInst)==FALSE)
        return FALSE;

    // set today's date in the registry as the last valid entered date
    GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, NULL,
                   "dd MMMM", (LPTSTR)&szdatebuf,20);
    RESaveRegistryKey("Last Entered", (LPCTSTR)szdatebuf);
    return TRUE;
    }

/******************************************************************************\
*  Performing startup checks for the first installed program directory
\******************************************************************************/

void PACheckStartupDirectory(void)
    {
    char szStartPath[150];
    LPCTSTR lpKey ="Startup in"; // name of registry key

    if(gbGodMode) return; // getout of we're in godmode

    REReadRegistryKey(lpKey, (LPCTSTR)szStartPath, 150);
    if(strlen(szStartPath)>1) // if we've read a startup directory..
        { // ..we set this as our current directory
        SetCurrentDirectory((LPCTSTR)szStartPath);
        }
    else  // this must be the first time we're being run, so...
        { // ..set the current directoy as our startup directory
        GetCurrentDirectory((DWORD)150, (LPTSTR)szStartPath);
        RESaveRegistryKey(lpKey, (LPCTSTR)szStartPath);
        }        
    }


/******************************************************************************\
* Shareware Code Registration dialog
\******************************************************************************/

HFONT   hCodeBigFont;
HFONT   hCodeMedFont;

BOOL PASetGrowerName(HWND hDlg, HINSTANCE hInst, int id)
    { // get the grower name entered in the edit control
    if(SendDlgItemMessage(hDlg, id, WM_GETTEXT, 30, (LPARAM)strbuf))
        { // check if he has entered his name
        RESaveRegistryKey("Grower's Name", (LPCTSTR)strbuf);
        return TRUE;
        }
    LoadString(hInst, IDS_CR01, strbuf, sizeof(strbuf));
    MessageBox(hDlg, strbuf, "Warning", MB_OK|MB_ICONEXCLAMATION);
    SetFocus(GetDlgItem(hDlg, id));
    return FALSE;
    }
/*
BOOL PACheckCodeRegistration(HWND hDlg, HINSTANCE hInst)
    {
    int i; // length of his registration code
    // get the name he entered in the edit control
    if(PASetGrowerName(hDlg, hInst, IDC_ED02)==FALSE)
       return FALSE;
    // now get the registration code he entered
    i = SendDlgItemMessage(hDlg, IDC_ED01, WM_GETTEXT, 20, (LPARAM)strbuf);
    // now check if it's a valid length
    if(i != 15)   // could this be our registration crack code?
        { // it's an invalid password
        MessageBox(hDlg, "Invalid Registration Code Entered !!", 
                    "Warning", MB_OK|MB_ICONEXCLAMATION);
        // focus back to the relevant edit control
        SetFocus(GetDlgItem(hDlg, IDC_ED01));
        return FALSE;
        }
    else
        { // compare his code with our valid code 162-9583672-104
        char szCode[20];
        PAUnscrambleCode(szCode);
        if(strcmp(strbuf, (LPCTSTR)&szCode)==0)
            {
            LoadString(hInst, IDS_CR02, strbuf, sizeof(strbuf));
            MessageBox(hDlg, strbuf, "Thanks", MB_OK|MB_ICONEXCLAMATION);
            RESaveRegistryKey("Registration Code", (LPCTSTR)szCode);
            gbRegistered = TRUE;
            return TRUE;
            }
        else
            {
            if(strcmp(strbuf, "273-8472561-326")==0)
                MessageBox(hDlg, "Nice try !!!\n\nNow let's see if you can\ncrack this version's code !!!", 
                            "Warning", MB_OK|MB_ICONEXCLAMATION);
            else
                MessageBox(hDlg, "Invalid Registration Code Entered !!", 
                            "Warning", MB_OK|MB_ICONEXCLAMATION);
            // focus back to the relevant edit control
            SetFocus(GetDlgItem(hDlg, IDC_ED01));
            return FALSE;
            }
        }
    return FALSE;
    }


void PAInitCodeDialog(HWND hDlg)
    {
    int i;
    long lp=MAKELPARAM(TRUE, 0);
    // first center the dialog in our window
    DlgCenter(hDlg);
    // now we'll create some bold fonts
    hCodeBigFont = GLCreateDialogFont(24,0,FW_BOLD);
    hCodeMedFont = GLCreateDialogFont(17,0,FW_BOLD);
    SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hCodeBigFont,lp);
    for(i=0;i<3;i++)
        SendDlgItemMessage(hDlg,IDS_ST01+i,WM_SETFONT,(WPARAM)hCodeMedFont,lp);
    }


BOOL CALLBACK PACodeDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);

    switch (message)
      {
      case WM_INITDIALOG:
            PAInitCodeDialog(hDlg);
      return (TRUE);

      case WM_DESTROY:
          {
          if(hCodeBigFont) DeleteObject(hCodeBigFont);
          if(hCodeMedFont) DeleteObject(hCodeMedFont);
          }
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
          if(GetDlgCtrlID((HWND)lParam) != IDC_ST01)
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }

      case WM_COMMAND:
          switch (wParam)
            {
            case IDC_BT01: 
                if(PACheckCodeRegistration(hDlg, dhInst))
                    {
                    EndDialog(hDlg, TRUE); 
                    return (TRUE);
                    }
            return (FALSE);

            case IDCANCEL: 
                EndDialog(hDlg, FALSE); 
            return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          



BOOL PACodeRegistration(HWND hwnd, HINSTANCE hInst)
    { // called if he calls up the Code Registration menu item or button
    if(gbRegistered)
        { // don't bother if already registered
        MessageBox(hwnd, "This version of HighGrow is already registered !!",
                   "Hey?", MB_APPLMODAL | MB_OK);
        return FALSE;
        }
    return DialogBox(hInst, "Code", hwnd, PACodeDlgProc);
    }
*/

/******************************************************************************\
* Warn him of the shareware remaining plant growth days
\******************************************************************************/
/*
HFONT    hWarnBigFont;
HFONT    hWarnMedFont;
HFONT    hWarnSmlFont;
HBITMAP  hWarningBmp;
HPALETTE hWarningPal;

void PAInitWarningDialog(HWND hDlg, HINSTANCE dhInst)
    {
    int i, j;
    long lp=MAKELPARAM(TRUE, 0);
    char szDateNow[3];
    // first center the dialog in our window
    DlgCenter(hDlg);
    // load our leaf bitmap
    hWarningBmp = DILoadResourceBitmap(dhInst,"Leaf256",&hWarningPal);
    // now we'll create some bold fonts
    hWarnBigFont = GLCreateDialogFont(24,0,FW_BOLD);
    hWarnMedFont = GLCreateDialogFont(18,0,FW_BOLD);
    hWarnSmlFont = GLCreateDialogFont(19,0,FW_NORMAL);
    SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hWarnBigFont,lp);
    SendDlgItemMessage(hDlg,IDC_ST13,WM_SETFONT,(WPARAM)hWarnMedFont,lp);
    for(i=0;i<3;i++)
        {
        SendDlgItemMessage(hDlg,IDC_ST10+i,WM_SETFONT,(WPARAM)hWarnSmlFont,lp);
        if(PI_PlantSeed[i].szStartDate[0])
            {
            GLDateNow(szDateNow);
            j = 59 - GLDiffDate(PI_PlantSeed[i].szStartDate, (char*)&szDateNow);
            if(j>0)
                 sprintf(strbuf, "%s\n has %i days\nremaining", 
                         PI_PlantSeed[i].szPlantName, j);
            else sprintf(strbuf, "%s\n has Expired", 
                         PI_PlantSeed[i].szPlantName);
            }
        else strbuf[0]=0;
        SendDlgItemMessage(hDlg,IDC_ST10+i,WM_SETTEXT,0,(LPARAM)strbuf);
        }
    }
             

void PADrawWarningLeafImage(HWND hwnd, HDC hdc, int iPlant)
    {
    if(PI_PlantSeed[iPlant].szStartDate[0])
        {
        DIDrawBitmap(hdc, 0, 0, hWarningBmp, hWarningPal, SRCCOPY);
        }
    }

  
BOOL CALLBACK PAWarningDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   
    switch (message)
      {
      case WM_INITDIALOG:
            PAInitWarningDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
          {
          if(hWarnBigFont) DeleteObject(hWarnBigFont);
          if(hWarnMedFont) DeleteObject(hWarnMedFont);
          if(hWarnSmlFont) DeleteObject(hWarnSmlFont);
          DIFreeBitmap(hWarningBmp, hWarningPal);
          }
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
          UINT uID = GetDlgCtrlID((HWND)lParam);
          if(uID == IDC_ST02)
              {
              PADrawWarningLeafImage((HWND)lParam, (HDC)wParam, 0);
              return FALSE;
              }
          if(uID == IDC_ST03)
              {
              PADrawWarningLeafImage((HWND)lParam, (HDC)wParam, 1);
              return FALSE;
              }
          if(uID == IDC_ST04)
              {
              PADrawWarningLeafImage((HWND)lParam, (HDC)wParam, 2);
              return FALSE;
              }
          if(((uID >= IDC_ST10)&&(uID <= IDC_ST13))||(uID == IDC_ST01))
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
       return (FALSE);

      case WM_COMMAND:
          switch (wParam)
            {
            case IDC_BT01:
                 WinHelp(hDlg, "HighGrow.hlp", HELP_KEY, 
                        (DWORD)"Shareware registration information");
            return (FALSE);

            case IDC_BT02: 
                {
                if(PACodeRegistration(hDlg, dhInst))
                    EndDialog(hDlg, TRUE); 
                }
            return (FALSE);

            case IDOK: 
            case IDCANCEL: 
                EndDialog(hDlg, FALSE); 
            return (TRUE);
            }  
      return (FALSE);
      }
   return (FALSE);
   }                          


void PACheckShareWarning(HWND hwnd, HINSTANCE ghInst)
    {
    // dont do anything if we're already registered
    if(gbRegistered) return;
    // if we have at least one growing plant, we show the dialog
    if(PI_PlantSeed[0].szStartDate[0]|
       PI_PlantSeed[1].szStartDate[0]|
       PI_PlantSeed[2].szStartDate[0])
        {
        DialogBox(ghInst, "Warning", hwnd, PAWarningDlgProc);
        }
    }
*/

