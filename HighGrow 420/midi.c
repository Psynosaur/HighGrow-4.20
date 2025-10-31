#include <windows.h>
#include <io.h>
#include <mmsystem.h>
#include "midi.h"
#include "resource.h"
#include "dibitmap.h"
#include "password.h"
#include "global.h"

// ************************************************************
// ************************************************************
// ******************* PLAYING MIDI FILES *********************
// ************************************************************
// ************************************************************

BOOL bMidiPlaying=FALSE;
UINT wMidiDeviceID;
MCI_PLAY_PARMS mciPlayParms;

DWORD MIStopCloseMidiPlayback(void)
    {
    // getout if he can't play any midi files
    if(midiOutGetNumDevs()==0) return (0L);

    if(bMidiPlaying)
        mciSendCommand(wMidiDeviceID, MCI_STOP, MCI_NOTIFY, (DWORD)(LPVOID) &mciPlayParms);

    mciSendCommand(wMidiDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
    bMidiPlaying=FALSE;
    return (0L);
    }

 
DWORD MIOpenStartMidiPlayback(HWND hWndNotify, LPSTR lpFileName)
    {
    DWORD dwError;
    MCI_OPEN_PARMS   mciOpenParms;
    MCI_STATUS_PARMS mciStatusParms;
    // getout if he can't play any midi files
    if(midiOutGetNumDevs()==0) return (0L);
    // Open the device by specifying the device and filename.
    // MCI will attempt to choose the MIDI mapper as the output port.
    mciOpenParms.lpstrDeviceType = "sequencer";
    mciOpenParms.lpstrElementName = lpFileName;
    if (dwError = mciSendCommand((MCIDEVICEID)NULL, MCI_OPEN, 
                                  MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,
                                  (DWORD)(LPVOID) &mciOpenParms))
        {
        // Failed to open device. Don't close it; just return error.
        return (dwError);
        }
    // The device opened successfully; get the device ID.
    wMidiDeviceID = mciOpenParms.wDeviceID;

    // Check if the output port is the MIDI mapper.
    mciStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
    if (dwError = mciSendCommand(wMidiDeviceID, MCI_STATUS, 
        MCI_STATUS_ITEM, (DWORD)(LPVOID) &mciStatusParms))
        {
        mciSendCommand(wMidiDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
        return (dwError);
        }

    // Begin playback. The window procedure function for the parent 
    // window will be notified with an MM_MCINOTIFY message when 
    // playback is complete. At this time, the window procedure closes 
    // the device.
    mciPlayParms.dwCallback = (DWORD) hWndNotify;
    if (dwError = mciSendCommand(wMidiDeviceID, MCI_PLAY, MCI_NOTIFY, 
        (DWORD)(LPVOID) &mciPlayParms))
        {
        mciSendCommand(wMidiDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
        return (dwError);
        }
    bMidiPlaying=TRUE;
    return (0L);
    }


int GetMidiFileCount(void)
   {  // Count number of midi music files in current directory
   return GLGetFileCount("MIDI\\*.mid");
   }

// ************************************************************
// ************************************************************
// ******************* PLAYING MP3 FILES  *********************
// ************************************************************
// ************************************************************

BOOL bMP3Playing=FALSE;
UINT wMP3DeviceID;
MCI_PLAY_PARMS mciPlayMP3Parms;


DWORD MIStopCloseMP3Playback(void)
    {
    if(bMP3Playing)
        mciSendCommand(wMP3DeviceID, MCI_STOP, MCI_NOTIFY, (DWORD)(LPVOID) &mciPlayMP3Parms);

    mciSendCommand(wMP3DeviceID, MCI_CLOSE, 0, (DWORD)NULL);
    bMP3Playing=FALSE;
    return (0L);
    }

 
DWORD MIOpenStartMP3Playback(HWND hWndNotify, LPSTR lpFileName)
    {
    DWORD dwError;
    MCI_OPEN_PARMS mciOpenParms;
    // Open the device by specifying the device and filename.
    mciOpenParms.lpstrDeviceType = "mpegvideo";
    mciOpenParms.lpstrElementName = lpFileName;
    if (dwError = mciSendCommand((MCIDEVICEID)NULL, MCI_OPEN, 
                                  MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,
                                  (DWORD)(LPVOID) &mciOpenParms))
        {
        // Failed to open device. Don't close it; just return error.
        return (dwError);
        }
    // The device opened successfully; get the device ID.
    wMP3DeviceID = mciOpenParms.wDeviceID;

    // Begin playback. The window procedure function for the parent 
    // window will be notified with an MM_MCINOTIFY message when 
    // playback is complete. At this time, the window procedure closes 
    // the device.
    mciPlayMP3Parms.dwCallback = (DWORD) hWndNotify;
    if (dwError = mciSendCommand(wMP3DeviceID, MCI_PLAY, MCI_NOTIFY, 
        (DWORD)(LPVOID) &mciPlayMP3Parms))
        {
        mciSendCommand(wMP3DeviceID, MCI_CLOSE, 0, (DWORD)NULL);
        return (dwError);
        }
    bMP3Playing=TRUE;
    return (0L);
    }


int GetMP3FileCount(void)
   {  // Count number of .mp3 music files in current directory
   return GLGetFileCount("MP3\\*.mp3");
   }


/****************************************************************************\
* VACATION MODE ENABLE MODE DIALOG BOX AND WINPROC
\****************************************************************************/

HFONT hSndEnableBigFont;
HFONT hSndEnableMedFont;
BOOL  bMIMusicPlaying = FALSE;
BOOL  bMidi = TRUE;


void MIStopPlayingSoundFile(HWND hDlg)
    {
    // stop the playback
    if(bMIMusicPlaying) 
        {
        if(bMidi)
            MIStopCloseMidiPlayback();
        else
            MIStopCloseMP3Playback();
        // unset our global
        bMIMusicPlaying = FALSE;
        SendDlgItemMessage(hDlg,IDC_BT01,WM_SETTEXT,0,(LPARAM)"Play &Song");
        }
    }


void MIPlayStopSelectedSoundFile(HWND hDlg)
    {
    char szname[MAX_PATH];
    char szcopy[MAX_PATH];
    UINT uiSel;
    // ---
    // first we read the selected text
    uiSel = SendDlgItemMessage(hDlg,IDC_LB01,LB_GETCURSEL,0,0L); 
    if(uiSel == LB_ERR) return ;
    SendDlgItemMessage(hDlg,IDC_LB01,LB_GETTEXT,uiSel,(LPARAM)szname);
    if(strlen(szname)==0) return;
    // now check if the MIDI file type was selected
    if(SendDlgItemMessage(hDlg,IDC_MIDI,BM_GETCHECK,0,0L)==BST_CHECKED)
        {
        wsprintf(szcopy, "MIDI\\%s.mid", szname);
        strcpy(szname, szcopy);
        bMidi = TRUE;
        }
    else
        {
        wsprintf(szcopy, "MP3\\%s.mp3", szname);
        strcpy(szname, szcopy);
        bMidi = FALSE;
        }
    // now we start or stop the playback
    if(bMIMusicPlaying) 
        MIStopPlayingSoundFile(hDlg);
    else
        {
        if(bMidi)
            MIOpenStartMidiPlayback(hDlg, szname);
        else
            MIOpenStartMP3Playback(hDlg, szname);
        // unset our global
        SendDlgItemMessage(hDlg,IDC_BT01,WM_SETTEXT,0,(LPARAM)"&Stop Playing");
        bMIMusicPlaying = TRUE;
        }
    }

void MIFillMusicFilesListbox(HWND hDlg)
    {
    struct _finddata_t music_file;
    long hFile;
    int  i = 0;
    char szname[100];
    char szspec[50];
    char sztype[10];
    BOOL bMidi = FALSE;
    // -------
    if(SendDlgItemMessage(hDlg,IDC_MIDI,BM_GETCHECK,0,0L)==BST_CHECKED)
        {
        strcpy(szspec, "MIDI\\*.mid");
        strcpy(sztype, "MIDI");
        bMidi = TRUE;
        }
    else
        {
        strcpy(szspec, "MP3\\*.mp3");
        strcpy(sztype, "MP3");
        bMidi = FALSE;
        }
    // always reset the listbox first
    SendDlgItemMessage(hDlg, IDC_LB01, LB_RESETCONTENT, 0,0L);
    // Find first music file in current directory 
    if((hFile = _findfirst((char*)szspec, &music_file)) != -1L)
        {
        i = 1;
        strcpy(szname, music_file.name);
        szname[strlen(szname)-4] = 0;
        SendDlgItemMessage(hDlg,IDC_LB01,LB_INSERTSTRING,-1,(LPARAM)szname);
        while(_findnext(hFile, &music_file)==0)
            {
            strcpy(szname, music_file.name);
            szname[strlen(szname)-4] = 0;
            SendDlgItemMessage(hDlg,IDC_LB01,LB_INSERTSTRING,-1,(LPARAM)szname);
            i += 1;
            }
        _findclose(hFile);
        }
    // if we found no files, we'll grey the play button
    if(i==0)
        EnableWindow(GetDlgItem(hDlg, IDC_BT01), FALSE);
    else         
        EnableWindow(GetDlgItem(hDlg, IDC_BT01), TRUE);

    // select the first music file
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCURSEL, 0, 0L);
    // show how many files we found
    wsprintf(szspec, "%i %s Music Files Found", i, sztype);
    SendDlgItemMessage(hDlg,IDC_ST03,WM_SETTEXT,0,(LPARAM)szspec);
    }


void MIInitSoundEnableDialog(HWND hDlg, HWND hParent)
    {
    long lp = MAKELPARAM(TRUE, 0);
    // center dialog in window (only if the parent is a valid window)
    if(hParent) DlgCenter(hDlg);
    // here we set the dialog box text
    // now we'll create some bold fonts
    hSndEnableBigFont   = GLCreateDialogFont(20,0,FW_BOLD);
    hSndEnableMedFont   = GLCreateDialogFont(15,0,FW_BOLD);
    // if we created the seed name font, set it 
    if(hSndEnableBigFont)
        SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hSndEnableBigFont,lp);
    // if we created the seed text font, set it to all the heading controls
    if(hSndEnableMedFont) 
        {
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hSndEnableMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST03,WM_SETFONT,(WPARAM)hSndEnableMedFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST04,WM_SETFONT,(WPARAM)hSndEnableMedFont,lp);
        }
    // set the current status of the sound mode
    if(PACheckSoundEnabled())
         SendDlgItemMessage(hDlg,IDC_ST02,WM_SETTEXT,0,(LPARAM)"Currently Enabled");
    else SendDlgItemMessage(hDlg,IDC_ST02,WM_SETTEXT,0,(LPARAM)"Currently Disabled");
    // here we set the current sound file type radio button
    if(PACheckMIDISoundFileType())
         SendDlgItemMessage(hDlg,IDC_MIDI,BM_SETCHECK,BST_CHECKED,0L);
    else SendDlgItemMessage(hDlg,IDC_MP3, BM_SETCHECK,BST_CHECKED,0L);

    // here we fill the music file names listbox
    MIFillMusicFilesListbox(hDlg);
	// and select the first music file
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCURSEL, 0, 0L);
    }


void MISaveSoundEnableDialog(HWND hDlg)
    {
    // first save the music file type
    if(SendDlgItemMessage(hDlg,IDC_MIDI,BM_GETCHECK,0,0L)==BST_CHECKED)
         PASetSoundFileType(TRUE);
    else PASetSoundFileType(FALSE);
    // now set the sound option to on
    PASetSoundEnabled(TRUE);
    }


BOOL CALLBACK MISoundEnableDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hDlg, GWL_HINSTANCE);
   const HWND hParent     = GetParent(hDlg);

    switch (message)
      {
      case WM_INITDIALOG:
           MIInitSoundEnableDialog(hDlg, hParent);
      return (TRUE);

      case WM_DESTROY:
            {
            MIStopPlayingSoundFile(hDlg);
            if(hSndEnableBigFont)   DeleteObject(hSndEnableBigFont);
            if(hSndEnableMedFont)   DeleteObject(hSndEnableMedFont);
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
          if((i==IDC_ST03)||(i==IDC_ST04))
              return FALSE;

          SetBkMode((HDC)wParam, TRANSPARENT);
          return (int)GetStockObject(NULL_BRUSH);
          }
       return (FALSE);

      case MM_MCINOTIFY:
          {
          if(wParam==MCI_NOTIFY_SUCCESSFUL)
              {
              if(((lParam==(LONG)wMP3DeviceID)&&(bMP3Playing==TRUE))||
                  (lParam==(LONG)wMidiDeviceID)&&(bMidiPlaying==TRUE))
                  {
                  MIStopPlayingSoundFile(hDlg);
                  }
              }
          }
      break;
    
      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_BT01:
                MIPlayStopSelectedSoundFile(hDlg);
            return TRUE;

            case IDC_MIDI:
            case IDC_MP3:
                {
                MIStopPlayingSoundFile(hDlg);
                MIFillMusicFilesListbox(hDlg);
                }
            return TRUE;

            case IDOK:
                {
                // first save the relevant setings
                MISaveSoundEnableDialog(hDlg);
                // now end the dialog
                EndDialog(hDlg, TRUE); 
                }
            return TRUE;

            case IDCANCEL:
                {
                // first save the relevant setings
                if(PACheckSoundEnabled())
                    PAToggleSoundOption();
                // now end the dialog
                EndDialog(hDlg, FALSE); 
                }
            return TRUE;
            }  
      return (FALSE);
      }
    return (FALSE);               
   }                          


BOOL MIShowSoundEnableDialog(HWND hwnd, HINSTANCE hInst)
    {
    return DialogBox(hInst, "MUSIC", hwnd, MISoundEnableDlgProc);
    }

