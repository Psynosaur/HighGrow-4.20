/******************************************************************************\
*  GROWROOM.H - The best place to be !!!
\******************************************************************************/

#define gPlantStartY  312 // pixel position of plant start

extern HWND hGrowRoomWindow;

void GRCalcDrawNodes(PNODE noPlant, PDRAWPLANT dpPlant, int iPlant, int iStyle);
void GRCalcDrawLamp(PPLANT plPlant, int iPlant, BOOL bIsGrowing);
void GRInitDrawPlant(PPLANT plPlant, int iPlant, BOOL bIsGrowing);
void GRInitGrowRoomDrawPlants(void);
void GRExitGrowRoom(void);
void GREnterGrowRoom(HWND hwnd, HINSTANCE hInst);
void GRCheckToRedrawGrowRoom(HWND hwnd, HINSTANCE hInst);
BOOL GRReRenderGrowRoom(void);
void GRRecreateGrowroomGauges(HINSTANCE hInst, int iRoom);
void GRReloadGrowroomImage(HWND hWnd, HINSTANCE hInst, int iRoom);
void GRUpdateGrowRoom(HWND hwnd);
void GRRealizePalette(void);
void GRRemoveDrawPlant(int iPlant);
int GRGetPlantOffset(int iGrowroom);

LRESULT CALLBACK GrowRoomWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
