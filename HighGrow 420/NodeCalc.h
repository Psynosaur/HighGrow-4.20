
/******************************************************************************\
*  NODECALC.H - Calculating new node growth
\******************************************************************************/

// *** FILE MUST BE OPEN AND IN MEMORY ***
void NOCalculateNodes(PPLANT plPlant, int iPlant);
void NOReCalculateNodes(PPLANT plPlant, int iPlant);
BOOL NOPruneRandomTips(PPLANT plPlant, int iPot, int iPlant, int iNumTips);

// *** LOADS FILE INTO MEMORY FIRST ***
BOOL NOMousePrune(int iPlant, int iPot, int iTipNode);
