// *************************************************
// ********          JPGView.h            **********
// *************************************************


BOOL HGIDrawResourceWindow(HWND hwnd, HINSTANCE hInst, HDC hdc, UINT uiRes);
BOOL HGIDrawFileWindow(HWND hwnd, HDC hdc, LPCTSTR lpFileName);

BOOL HGILoadGrowroomResourceImage(HINSTANCE hInst, UINT uiRes);
BOOL HGILoadGrowroomFileImage(LPCTSTR lpFileName);
void HGIFreeGrowroomImage(void);
void HGIRenderGrowroomImage(HDC hdc, RECT rc);

BOOL HGILoadRoomEditImage(HINSTANCE hInst, UINT uiRes);
void HGIFreeRoomEditImage(void);
void HGIRenderRoomEditImage(HDC hdc, RECT rc);


