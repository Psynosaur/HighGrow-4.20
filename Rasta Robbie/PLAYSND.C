#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <io.h>
#include "playsnd.h"


BOOL IsSoundEnabled(LPSTR lpFrameText)
   { // returns a true if the text stored in the toonstep structure
     // actually represents a pre-recorded wav file.
   // reverse the text and check for a .wav file extension

   _strrev(lpFrameText);
   if(_strnicmp(lpFrameText, "vaw.", 4)==0) 
      {
      _strrev(lpFrameText);
      if((_strnicmp(lpFrameText, "WWF:", 4)==0)||
         (_strnicmp(lpFrameText, "WWR:", 4)==0)) 
         return TRUE;
      }
   else 
      _strrev(lpFrameText);
   
   return FALSE;
   }

void ErrorProc(DWORD dwResult, char* lpWaveFile)
   {
   HANDLE hMem;
   BOOL bRes=TRUE;
   LPSTR lpStringBuff;
	 
   hMem=GlobalAlloc(GHND,80);
   if(hMem)
     {
	  lpStringBuff=GlobalLock(hMem);
	   
	  if(lpStringBuff)
	     {
	     if(!mciGetErrorString(dwResult,lpWaveFile,70))
            bRes=FALSE;

		 GlobalUnlock(hMem);
		 }
	  else 
	     bRes=FALSE;

	  GlobalFree(hMem);
	  }
   else 
      bRes=FALSE;
   
   if(!bRes)
      strcpy(lpWaveFile, "A rather weird sound playback error just occurred!?");
 	}

// ****************************************************************************
WORD wDeviceID=0;

void CloseSoundDevice(void)
    {
    if(wDeviceID)
       {
       mciSendCommand(wDeviceID,MCI_CLOSE,0,(DWORD)NULL);
       wDeviceID=0;
       }
    }

BOOL OpenPlaySound(HWND hWnd, DWORD dwPlayFlags, char* lpWavefile)
	{ // this wave file MUST be in the form WTF:xxx.wav to be played here
    // NOTE: You must call CloseSoundDevice() with the MM_MCINOTIFY message
    MCI_OPEN_PARMS mciopen;
    MCI_PLAY_PARMS mciplay;
    DWORD dwRes;
    DWORD dwFlags;
    struct _finddata_t wtf_file;
    long hFile;

    if(wDeviceID) return FALSE;

    if(!waveOutGetNumDevs())
       {
       strcpy(lpWavefile, "I can't speak until you get a sound system!");
       return FALSE;
       }

    if((hFile = _findfirst(lpWavefile+4, &wtf_file)) == -1L)
       {
       strcpy(lpWavefile, "Hey, someone stole this frame's sound file!");
       return FALSE;
       }
    _findclose(hFile);

    mciopen.lpstrDeviceType=(LPSTR)MCI_DEVTYPE_WAVEFORM_AUDIO;
    mciopen.lpstrElementName=(LPSTR)lpWavefile+4;

    dwFlags=MCI_OPEN_TYPE_ID | MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;

    dwRes=mciSendCommand(0,MCI_OPEN,dwFlags,(DWORD)(LPSTR)&mciopen);

    if(dwRes)
       {
	   ErrorProc(dwRes, lpWavefile);
	   return FALSE;
	   }

    wDeviceID=mciopen.wDeviceID;
    mciplay.dwFrom=0;
    mciplay.dwCallback=(DWORD)hWnd;
    dwFlags=MCI_FROM | dwPlayFlags;
    dwRes=mciSendCommand(wDeviceID,MCI_PLAY,dwFlags,(DWORD)(LPSTR)&mciplay);

    if(dwRes)
	   {
	   ErrorProc(dwRes, lpWavefile);
	   return FALSE;
	   }
    return TRUE;
	}

BOOL PlayToonWave(HWND hWnd, char* lpWavefile)
    { // Opens the wave device. and plays sound.
    // NOTE: You must call CloseSoundDevice() with the MM_MCINOTIFY message
    return OpenPlaySound(hWnd, MCI_NOTIFY, lpWavefile);
    }


BOOL TestToonWave(HWND hWnd, char* lpWavefile)
    { // Opens the wave device. plays sound and then closes it again
    if(OpenPlaySound(hWnd, MCI_WAIT, lpWavefile))
       {
       CloseSoundDevice();
       return TRUE;
       }
    return FALSE;
    }

BOOL PlayToonWaveResource(LPCSTR lpWaveRes, HINSTANCE hInst)
    { 
    return PlaySound(lpWaveRes, hInst, SND_RESOURCE | SND_ASYNC | SND_NOWAIT);
    }
