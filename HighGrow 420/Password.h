/******************************************************************************\
*
*  Password.h - setting and check for the growroom password
*
\******************************************************************************/

extern char gszCurPassword[20]; // the password protecting this program

BOOL CALLBACK CheatDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void PASetPassword(HWND hwnd, HINSTANCE hInst);
void PASaveEndCheatChecks(void);
//BOOL PACodeRegistration(HWND hwnd, HINSTANCE hInst);
BOOL PASetGrowerName(HWND hDlg, HINSTANCE hInst, int id);
BOOL PACheckSoundEnabled(void);
void PASetSoundEnabled(BOOL bEnabled);
void PAToggleSoundOption(void);
void PASetSoundFileType(BOOL bMIDI);
BOOL PACheckMIDISoundFileType(void);

#define TYPE_MP3  1
extern BOOL gbMusicFileType; // zero = midi, 1=mp3 (default is midi)


void PACheckGodMode(void);
//void PACheckRegistered(void);
BOOL PACheckProgramAccess(HWND hwnd, HINSTANCE ghInst);
//void PACheckShareWarning(HWND hwnd, HINSTANCE ghInst);
// combines: BOOL PACheckPassword(HWND hwnd, HINSTANCE hInst);
//      and: BOOL PAStartCheatCheck(HWND hwnd);

void PACheckStartupDirectory(void);
BOOL PAGetRegistryPassword(void);

//void PAUnscrambleCode(char* szCode);

BOOL PACheckAutoLoad(void);
void PASetAutoLoad(BOOL bEnabled);
void PAToggleAutoLoadOption(void);
void PAToggleAutoGrowroomOption(void);
BOOL PACheckAutoGrowroom(void);

BOOL PACheckVacModeEnabled(void);
void PASetVacModeEnabled(BOOL bEnabled);
void PAToggleVacModeOption(void);

BOOL PACheckAlarmSet(void);
BOOL PACheckAlarmTime(int iSecs);
void PASetAlarmClock(BOOL bEnabled);
void PAToggleAlarmClockOption(void);
void PASaveAlarmSettings(LPSTR sztext, int ihrs, int imin, int ibn, BOOL bSet);
void PALoadAlarmSettings(LPSTR sztext, int* ihrs, int* imin, int* ibn, int* bSet);
void PALoadAlarmText(LPSTR sztext);

extern BOOL gbBoomBox; // visible or not
void PASetBoomBox(BOOL bEnabled);
void PACheckBoomBox(void);
