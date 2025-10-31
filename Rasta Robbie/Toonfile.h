
#include "windows.h"

// STRUCTURES

typedef struct
   { // Toon Step file Header structure
   char  szProductName[30];
   DWORD dwFileVersion;     // ToonFrameFile's version number
   char  szCaption[50];     // Window Caption
   DWORD dwWindowSize;      // Toon playback Window size
   UINT  uiToonFileFrames;  // Number of frames in ToonFile
   UINT  uiPlaybackSpeed;   // Initial Toon Playback speed
   UINT  uiBackTexture;   // 1=Robbie...
   } TOONSTEPHEADER;

typedef TOONSTEPHEADER FAR* PTOONSTEPHEADER;

// GLOBAL VARIABLES

#define iMaxPoses 24             // maximum number of poses in this version
extern  HBITMAP hRobbieBmps[20+iMaxPoses];

extern HCURSOR hHourGlass;  // handle to hourglass cursor
extern HCURSOR hSaveCursor;  // handle to previous cursor

extern DWORD dwWinSize;
extern char FileName[128];

extern HGLOBAL hFrameMem;
extern int     MaxToonSteps;
extern int     gnWalkSpeed;
extern int     CurToonStep;

// FUNCTIONS

void FreeFrameMemory(void);
BOOL AdjustPlayWindowPoint(HWND hwnd, POINT *ptWin);
BOOL SaverFileOpen(HWND hWnd, LPCSTR szSaverFile);
BOOL GetCurToonStep(int CurFrame, TOONSTEP *pTStep);
BOOL LoadToonResource(HWND hWnd, HINSTANCE ghInst, LPCSTR szToonRes);



