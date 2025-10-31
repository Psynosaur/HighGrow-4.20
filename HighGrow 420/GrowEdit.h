/******************************************************************************\
*
*  PROGRAM:     GrowEdit.h
*
*  PURPOSE:     Editing Growrooms
*
\******************************************************************************/

BOOL CALLBACK GEGrowEditDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

BOOL GESaveRoomDetailsToFile(void);
BOOL GELoadRoomDetailsFromFile(void);

DWORD GEGetCurRoomPotSize(int iRoom);

BOOL GEStartAddGrowRoomDialog(HWND hwnd, HINSTANCE hInst);
BOOL GEStartEditGrowRoomDialog(HWND hwnd, HINSTANCE hInst);
BOOL GEDeleteRoom(void);


