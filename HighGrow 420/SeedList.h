/******************************************************************************\
*
*  PROGRAM:     SeedList.h
*
*  PURPOSE:     Choosing and Planting the Seeds
*
\******************************************************************************/

extern PLANTINFO PI_PlantSeed[30*3];

void SLInitPlantSeed(PPLANT plPlant, int iSeed);
void SLUpdatePlantSeedGrowthStage(PPLANT plPlant, int iSeed);

BOOL CALLBACK SLSeedDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
