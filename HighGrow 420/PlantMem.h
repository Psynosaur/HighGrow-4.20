
// ********************************************************************
// PLANTMEM.H
// ********************************************************************

extern PPLANT  P_Plant;     // pointer to global memory
extern HGLOBAL hPlantMem;   // handle to our global memory

BOOL PMPlantFileToMem(int iCurPlant);
BOOL PMPlantMemToFile(int iCurPlant);

BOOL PMPlantFileToMem(int iCurPlant);
void PMPlantMemoryFree(void);
BOOL PMNewPlantFile(PPLANTINFO piPlant, int iCurPlant);
