/****************************************************************************
* PROGRAM: Toolbar.h
****************************************************************************/
//extern HWND hWndToolbar;      

void TBShowClock(BOOL bShow);
void TBUpdateClock(void);

void TBInitProgress(int iMax, LPCTSTR szText);
void TBShowProgress(int iCurProgress);
void TBShowNextProgress(void);
void TBEndProgress(void);

BOOL TBStartActivitySheet(HWND hwnd, HINSTANCE hInst, int idButton);
BOOL TBStartGrowthLog(HWND hwnd, HINSTANCE hInst);
BOOL TBStartGrowthChart(HWND hwnd, HINSTANCE hInst);
BOOL TBStartHarvestDialog(HWND hwnd, HINSTANCE hInst);
BOOL TBStartObjectivesDialog(HWND hwnd, HINSTANCE hInst);
BOOL TBStartPlantingDialog(HWND hwnd, HINSTANCE hInst);
BOOL TBStartEditGrowRoomDialog(HWND hwnd, HINSTANCE hInst);
BOOL TBStartGrowRoom(HWND hwnd, HINSTANCE hInst);
void TBQueryEnableButtons(void);
void TBSetGrowroomButtonState(BOOL bEnabled);
BOOL TBToolbarNotify(HINSTANCE hInst, LPARAM lParam);
int  TBToolbarHeight(void);
// void TBSizeToolbar(HWND hWnd);
void TBInitGrowroomCombo(HWND hComboWnd);
void TBODrawComboPlantItem(LPDRAWITEMSTRUCT lpDIS);
void TBODrawComboGrowroomItem(LPDRAWITEMSTRUCT lpDIS);
BOOL TBMeasureGrowComboItem(LPMEASUREITEMSTRUCT lpMIS);
int TBGetComboPlant(void);
int TBGetComboGrowroom(void);
int TBGetComboGrowroomCount(void);
void TBResetGrowroomCombo(BOOL bNewRoomAdded);
BOOL TBGetComboGrowroomName(LPTSTR lpName);
void TBUpdatePlantCombo(void);
void TBUpdateGrowroomCombo(void);
void TBSetComboPlant(int iPlant);
// BOOL TBCreateToolbar(HWND hWnd, HINSTANCE hInst);
void TBDestroyToolbar(HWND hWnd);
BOOL TBStartRobbieAlarmWalk(HWND hwnd, HINSTANCE hInst);
BOOL TBStartRobbieToonfile(HWND hwnd, LPCSTR szToonFile);
BOOL TBStartRobbieRules(HWND hwnd);
void TBCheckEndRulesPlayback(HWND hwnd);
//void TBPlayNextMidiFile(HWND hwnd);
BOOL TBPlayMusicFile(HWND hwnd, LPSTR lpMusicFile);
void TBStartStopMusicPlayback(HWND hwnd);
BOOL TBPlayNextRandomMusicFile(HWND hwnd);
void TBToggleSoundOption(HWND hwnd, HINSTANCE hInst);
void TBToggleAutoLoadOption(HWND hwnd);
void TBToggleAutoGrowroomOption(HWND hwnd);
void TBStartVacationEnabledDialog(HWND hwnd, HINSTANCE ghInst);
void TBStartAlarmSetDialog(HWND hwnd, HINSTANCE ghInst);

void TBGrowroomMusicPlayback(HWND hwnd);
void TBRemoveCurPlant(void);


void TBSizeCoolbar(HWND hWnd, LPARAM lParam);
BOOL TBCreateCoolbar(HWND hwndParent, HINSTANCE g_hInst);
