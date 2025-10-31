
#define IDS_PLAY_WAV	       100
#define IDS_WINDOWS_PLAY_WAV   200

BOOL IsSoundEnabled(LPSTR lpFrameText);
void CloseSoundDevice(void);
BOOL PlayToonWave(HWND hWnd, char* lpWavefile);
BOOL TestToonWave(HWND hWnd, char* lpWavefile);
BOOL PlayToonWaveResource(LPCSTR lpWaveRes, HINSTANCE hInst);
