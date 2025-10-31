/******************************************************************************\
*
*  PROGRAM:     Leaves.h
*
\******************************************************************************/

void LFFreeLeafImages(void);
void LFLoadLeafImages(HINSTANCE hInst);
void LFDrawLeafImage(HDC hdc, POINT ptNode, int iLeafType, int iLeafID);

void LFCalcLeafGrowth(PPLANT plPlant, int iNodes, int iGStage);
