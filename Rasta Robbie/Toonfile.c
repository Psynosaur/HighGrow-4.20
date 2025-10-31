
//**************************************************************
//***************EDITING TOOLHELP FRAME FILES*******************
//**************************************************************
#include <stdlib.h>
#include <io.h>
#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "memory.h"
#include "playsnd.h"
#include "Robbie.h"
#include "toonfile.h"
#include <commdlg.h>
#include <sys\stat.h>

#define DemoSteps 1

TOONSTEPHEADER RobbieToonStepHeader;
TOONSTEP       RobbieToonStep;
int            gnWalkSpeed=3;
int            CurToonStep;
int            MaxToonSteps;
int            tstepPose=0;      // current frame's pose

// ******* ALLOCATING & HANDLING LOCAL MEMORY ***********

HGLOBAL   hFrameMem;
PTOONSTEP pFrameBuf;
BOOL bChanges = FALSE;       // TRUE if the file is changed

void FreeFrameMemory(void)
   {
   if(hFrameMem) 
      {
      GlobalFree(hFrameMem);
      hFrameMem = 0;
      }
   }
   
BOOL InitFrameMemory(int NumFrames)
   {
   FreeFrameMemory();
   
   hFrameMem = GlobalAlloc(GHND, (DWORD)(NumFrames*sizeof(TOONSTEP)));
   
   if(hFrameMem) 
      return TRUE;
   else
      return FALSE;   
   }
   
BOOL GetCurToonStep(int CurFrame, TOONSTEP *pTStep)
   {
   if(hFrameMem==0) 
      return FALSE;

   if(CurFrame>=MaxToonSteps)
      return FALSE;
   
   pFrameBuf = (PTOONSTEP)GlobalLock(hFrameMem);
   
   if(pFrameBuf)
      {
      pFrameBuf = pFrameBuf+CurFrame;
      pTStep->itXpos = pFrameBuf->itXpos;
      pTStep->itYpos = pFrameBuf->itYpos;
      pTStep->itPose = pFrameBuf->itPose;
      pTStep->uiWait = pFrameBuf->uiWait;
      pTStep->btWink = pFrameBuf->btWink;
      strcpy(pTStep->sztText, pFrameBuf->sztText);   
      GlobalUnlock(hFrameMem);
      return TRUE;
      }
   else
      return FALSE;   
   }
   
// ********************************************************
// ************ FILE HANDLING FUNCTIONS *******************
// ********************************************************

HCURSOR hHourGlass;  // handle to hourglass cursor
HCURSOR hSaveCursor;          // current cursor handle
int hFile;                   // file handle
OFSTRUCT OfStruct;           // information from OpenFile()
DWORD dwWinSize;   // toon display window size
char  FileName[128];
char  str[255];


//************TOONSTEP FILE HEADER INIT****************

BOOL AdjustPlayWindowPoint(HWND hwnd, POINT *ptWin)
    { 
    // Adjusts the given Point for new position in the playback window.
    // This must be done in case the file is being played back on a 
    // screen with different dimentions.
    RECT rc;

    // here we check for failure conditions
    if(RobbieToonStepHeader.dwWindowSize==MAKELONG(0,0))
    return FALSE;

    // here we calculate the window size
    GetClientRect(hwnd, &rc);

    // Here we calculate the new point positions
    ptWin->x = ptWin->x*(rc.right-rc.left)/
               LOWORD(RobbieToonStepHeader.dwWindowSize);
    
    ptWin->y = ptWin->y*(rc.bottom-rc.top)/
               HIWORD(RobbieToonStepHeader.dwWindowSize);
    
    return TRUE;
    }

BOOL CheckToonFileVersion(HWND hWnd, LPCSTR lpFileName)
   {
   if(RobbieToonStepHeader.dwFileVersion!=MAKELONG(1, 2))
      {
      sprintf(str, "Invalid ToonFile Version %s.", lpFileName);
      MessageBox(hWnd, str, NULL, MB_OK | MB_ICONEXCLAMATION);
      return FALSE;
      }
   return TRUE;
   }

BOOL ReadToonFileHeader(HWND hWnd, HFILE hToonFile)
   {
   LONG IOStatus;    // result of a file write 

   // read the toonfile header from the file
   IOStatus = _hread(hToonFile, &RobbieToonStepHeader, 
                       (LONG)sizeof(TOONSTEPHEADER));
   // check for write failure case
   if(IOStatus!=(LONG)(sizeof(TOONSTEPHEADER))) 
      return FALSE;
   
   // check major value of file version
   if(!CheckToonFileVersion(hWnd, FileName))
      return FALSE;

   MaxToonSteps = RobbieToonStepHeader.uiToonFileFrames;
   gnWalkSpeed  = RobbieToonStepHeader.uiPlaybackSpeed;
   return TRUE;
   }
      
//******************************************************
//*******************OPENING TOONFILES******************
//******************************************************

UINT OpenWtfFile(HWND hDlg)
   {  // ***NOTE*** FileName MUST be set before calling function
   UINT NumFrames;   // number of toonstep frames in file
   LONG IOStatus;    // result of a file read
   DWORD dw;

   // first check for the failure case
   if(FileName[0]==0) return 0;

   // Open the file and get its handle
   hFile = OpenFile(FileName, (LPOFSTRUCT)&OfStruct, OF_READWRITE);
   if(!hFile)
      {
      MessageBox(hDlg, "Error opening file", NULL, MB_OK | MB_ICONHAND);
      return 0;
      }

   // Allocate Frame buffer to the size of the file - fileheader + 1 
   dw = GetFileSize((HANDLE)hFile, NULL);
   NumFrames = (UINT)(((dw-sizeof(TOONSTEPHEADER))+1)
                     /(sizeof(TOONSTEP)));

   InitFrameMemory(NumFrames);

   if(!hFrameMem) 
      {
      MessageBox(hDlg, "Not enough memory.", NULL, MB_OK | MB_ICONHAND);
      _lclose(hFile);
      return 0;
      }

   // read file header and if successfull, read toonstep frames
   if(ReadToonFileHeader(hDlg, hFile)==FALSE)
      {
      _lclose(hFile);
      return 0;
      }

   // lock memory & show hourglass cursor
   hSaveCursor = SetCursor(hHourGlass);
   pFrameBuf = (PTOONSTEP)GlobalLock(hFrameMem);
   
   IOStatus = _hread(hFile, pFrameBuf, (NumFrames*sizeof(TOONSTEP)));
      
   // # bytes read must equal to file size - header size
   SetCursor(hSaveCursor);  // restore the cursor 
   if(IOStatus != (LONG)(NumFrames*sizeof(TOONSTEP)))
      {
      sprintf(str, "Error reading %s.", FileName);
      MessageBox(hDlg, str, NULL, MB_OK | MB_ICONEXCLAMATION);
      }

   _lclose(hFile);
   GlobalUnlock(hFrameMem);

   return NumFrames;
   }


// ************************************************************
// ****************OPENING TOONFILES REMOTELY******************
// ************************************************************

BOOL IsToonFile(LPSTR lpFileName)
   {
   _strrev(lpFileName);
   if(_strnicmp(lpFileName, "ftw.", 4)==0) 
      {
      _strrev(lpFileName);
      return TRUE;
      }
   return FALSE;
   }

BOOL FindToonFile(LPSTR lpFileName)
   {  // Check if this toon file can be found
   struct _finddata_t wtf_file;
   long hFile;
   BOOL bSuccess=FALSE;

   if(lpFileName[0]!=0) 
      {
      if((hFile = _findfirst(lpFileName, &wtf_file)) != -1L)
         {
         _findclose(hFile);
         bSuccess = TRUE;
         }
      }
   return bSuccess;
   }


BOOL SaverFileOpen(HWND hWnd, LPCSTR szSaverFile)
   {
   UINT NumFrames;

   if(!IsToonFile((LPSTR)szSaverFile))          
      {
      sprintf(str, "%s - Invalid Toon File!", szSaverFile);
      MessageBox(hWnd, str, NULL, MB_OK | MB_ICONHAND);
      return FALSE;
      }

   if(!FindToonFile((LPSTR)szSaverFile))
      {
      sprintf(str, "%s - Toon File Not Found!", szSaverFile);
      MessageBox(hWnd, str, NULL, MB_OK | MB_ICONHAND);
      return FALSE;
      }

   // ensure we get the dialog box
   strcpy(FileName, szSaverFile);
   NumFrames = OpenWtfFile(hWnd);
   // check if we opened ok   
   if(NumFrames==0) return FALSE;
   // set the start and end frame      
   CurToonStep  = 0;
   MaxToonSteps = NumFrames;
   GetCurToonStep(CurToonStep, &RobbieToonStep);
   return TRUE;
   }


// ************************************************************
// ****************OPENING TOONFILES RESOURCES*****************
// ************************************************************

BOOL LoadToonResource(HWND hWnd, HINSTANCE ghInst, LPCSTR szToonRes)
   {
   DWORD  dw;         // size of our toon resource
   UINT   NumFrames;  // number of toonstep frames in file
   HANDLE hResInfo;
   HANDLE hToonRes;   // handles to resource memory
   LPSTR  lpToonRes;  // pointer to resource
   BOOL   bSuccess = FALSE;
   int    iHeadLen = sizeof(TOONSTEPHEADER);

   if(szToonRes[0]==0)  return FALSE;

   hResInfo = FindResource(ghInst, szToonRes, "TOONFILE");
   if(hResInfo)
      {
      hToonRes = LoadResource(ghInst, hResInfo);
      if(hToonRes)
         {
         lpToonRes = LockResource(hToonRes);
         dw        = SizeofResource(ghInst, hResInfo);
         NumFrames = (UINT)(((dw-iHeadLen)+1)/(sizeof(TOONSTEP)));

         InitFrameMemory(NumFrames);
         if(hFrameMem) 
            {
            memcpy(&RobbieToonStepHeader, lpToonRes, iHeadLen);
            if(CheckToonFileVersion(hWnd, szToonRes))
               {
               MaxToonSteps = RobbieToonStepHeader.uiToonFileFrames;
               gnWalkSpeed  = RobbieToonStepHeader.uiPlaybackSpeed;

               pFrameBuf = (PTOONSTEP)GlobalLock(hFrameMem);
               memcpy(pFrameBuf, (lpToonRes+iHeadLen), MaxToonSteps*(sizeof(TOONSTEP)));
               GlobalUnlock(hFrameMem);
               CurToonStep  = 0;
               GetCurToonStep(CurToonStep, &RobbieToonStep);
               bSuccess = TRUE;
               }
            }
         else
            {
            MessageBox(hWnd, "Not Enough Memory!", NULL, MB_OK | MB_ICONHAND);
            }
         UnlockResource(hToonRes);
         FreeResource(hToonRes);
         }
      else
         {
         sprintf(str, "Toon Resource %s Not Loaded!", szToonRes);
         MessageBox(hWnd, str, NULL, MB_OK | MB_ICONHAND);
         }
      }
   else
      {
      sprintf(str, "Toon Resource %s Not Found!", szToonRes);
      MessageBox(hWnd, str, NULL, MB_OK | MB_ICONHAND);
      }
   return bSuccess;
   }
