
// *****************************************
// **              MIDI.H                 **
// *****************************************

DWORD MIOpenStartMidiPlayback(HWND hWndNotify, LPSTR lpFileName);
DWORD MIStopCloseMidiPlayback(void);
int GetMidiFileCount(void);

extern UINT wMidiDeviceID;
extern BOOL bMidiPlaying;

DWORD MIOpenStartMP3Playback(HWND hWndNotify, LPSTR lpFileName);
DWORD MIStopCloseMP3Playback(void);
int GetMP3FileCount(void);

extern BOOL bMP3Playing;
extern UINT wMP3DeviceID;

BOOL MIShowSoundEnableDialog(HWND hwnd, HINSTANCE hInst);
