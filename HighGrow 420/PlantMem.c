
/******************************************************************************\
*
*  PLANTMEM.C - HANDLING GLOBAL MEMORY AND SAVING AND READING HGPLANT FILES
*
\******************************************************************************/

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include "global.h"
#include "toolbar.h"

/******************************************************************************\
*  SOME CONSTANTS
\******************************************************************************/
    
char     FileName[128] = "\0";
OFSTRUCT OfStruct;      // information from OpenFile()
HGLOBAL  hPlantMem=0;   // handle to our global memory
int      hPlantFile=0;  // currently opened file handle
PPLANT   P_Plant;       // pointer to global memory

// ********************************************************
// ******** ALLOCATING & HANDLING GLOBAL MEMORY ***********
// ********************************************************

void PMPlantMemoryFree(void)
   {
   if(!hPlantMem) return;
   GlobalFree(hPlantMem);
   hPlantMem = 0;
   }
   
BOOL PMPlantMemoryInit(void)
   {
   PMPlantMemoryFree();
   hPlantMem = GlobalAlloc(GPTR, (DWORD)(sizeof(PLANT)));
   if(!hPlantMem) return FALSE;   
   return TRUE;
   }


/******************************************************************************\
*  THE MAIN FILE OPEN ROUTINE
\******************************************************************************/


BOOL PMPlantFileOpen(void)
   {  
   // first check for the failure case
   if(!FileName[0]) return FALSE;
   // Open the file and get its handle
   hPlantFile=OpenFile(FileName, (LPOFSTRUCT)&OfStruct, OF_READWRITE);
   if(!hPlantFile)  return FALSE;
   return TRUE;
   }

BOOL PMPlantFileOpenNew(void)
   {  
   // first check for the failure case
   if(!FileName[0]) return FALSE;
   // Open the file and get its handle
   hPlantFile=OpenFile(FileName, (LPOFSTRUCT)&OfStruct, OF_READWRITE|OF_CREATE);
   if(!hPlantFile)  return FALSE;
   return TRUE;
   }

void PMPlantFileClose(void)
   {
   if(!hPlantFile)  return;
   _lclose(hPlantFile);
   hPlantFile = 0;
   }


BOOL PMPlantFileRead(void)
   {  // file must be open
   LONG IOresult;    // result of a file read
   DWORD dw;

   if(!hPlantFile) return FALSE;
   // Allocate Plant buffer to the size of the file 
   dw = GetFileSize((HANDLE)hPlantFile, NULL);

   // lock memory and read file
   P_Plant   = (PPLANT)GlobalLock(hPlantMem);
   IOresult = _hread(hPlantFile, P_Plant, sizeof(PLANT));

   // # bytes read must equal to file size - header size
   if(IOresult != (LONG)(sizeof(PLANT)))
       {
       GlobalUnlock(hPlantMem);
       return FALSE;
//       return TRUE;
       }
   GlobalUnlock(hPlantMem);
   return TRUE;
   }


BOOL PMPlantFileWrite(void)
    { // file must be open
    LONG IOresult;    // result of a file write 
    // lock the memory
    P_Plant   = (PPLANT)GlobalLock(hPlantMem);
    // and save the Global Plant buffer      
    IOresult = _hwrite(hPlantFile, (LPCSTR)P_Plant, sizeof(PLANT));
    // unlock the memory again
    GlobalUnlock(hPlantMem);
    // check the results status
    if(IOresult!=(LONG)(sizeof(PLANT)))
        return FALSE;
    return TRUE;
    }


// ********************************************************************
// * OPENS & READS PLANT FILE INTO GLOBAL MEMORY
// ********************************************************************

// *** MUST FREE MEMORY OR CALL PMPlantMemToFile AFTER CALLING THIS ***

BOOL PMPlantFileToMem(int iCurPlant)
   { // opens the plant file (0-2), reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hPlantMem. If fails, we get null.
   int i=min(gMaxPlants-1, max(0, iCurPlant));
   BOOL bSuccess = FALSE;
   // first set up our filename
   sprintf(FileName, "Plant%02i.hgp", i+1);
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist(FileName)) return 0;
   // now set up the memory required
   if(!PMPlantMemoryInit()) return 0;
   // if the file exists, open it
   if(!PMPlantFileOpen())   return 0;
   // then read the file into our memory
   bSuccess = PMPlantFileRead();
   // free our memory if we failed to read the file
   if(!bSuccess) PMPlantMemoryFree();
   // and close it again
   PMPlantFileClose();

   return bSuccess;
   }


// ********************************************************************
// * OPENS & WRITES PLANT FILE FROM GLOBAL MEMORY
// ********************************************************************

// **** MUST CREATE MEMORY WITH PMPlantFileToMem BEFORE CALLING THIS ****

BOOL PMPlantMemToFile(int iCurPlant)
   { // opens the plant file (0-2), reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hPlantMem. If fails, we get null.
   int  i = min(gMaxPlants-1, max(0, iCurPlant));
   BOOL bSuccess = FALSE;
   // first set up our filename
   sprintf(FileName, "Plant%02i.hgp", i+1);
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist(FileName)) return 0;
   // get out if we don't have global memory required
   if(!hPlantMem)                 return 0;
   // if the file exists, open it
   if(!PMPlantFileOpen())         return 0;
   // save the global memory into the file
   bSuccess = PMPlantFileWrite();
   // free our global memory
   PMPlantMemoryFree();
   // and close the file again
   PMPlantFileClose();
   return bSuccess;
   }


/******************************************************************************\
*  THE PLANTFILE CREATION ROUTINE (WHEN HE PLANTS HIS SEED)
\******************************************************************************/

void PMInitFilePlantInfo(PPLANTINFO piPlant)
    { 
    int i;
    // if the global memory hasn't been created, getout
    if(!hPlantMem) return;
    // lock the memory first
    P_Plant = (PPLANT)GlobalLock(hPlantMem);
    // now copy the PlantInfo Struct elements
    strcpy(P_Plant->PI_Plant.szPlantName, piPlant->szPlantName);
    for(i=0 ;i<3; i+=1)
         P_Plant->PI_Plant.szStartDate[i] = piPlant->szStartDate[i];
    P_Plant->PI_Plant.cSeedChoice  = piPlant->cSeedChoice;
    P_Plant->PI_Plant.bRegistered  = gbRegistered;
    P_Plant->PI_Plant.dwFileVersion=MAKELONG(1, 1);  // version = 1.1 
    // now set some default values for growday 0
    P_Plant->GD_Plant[0].bVisited      = TRUE;
    P_Plant->GD_Plant[0].bWatered      = FALSE;
    P_Plant->GD_Plant[0].bFertilized   = FALSE;
    P_Plant->GD_Plant[0].cLightOn      = 28;
    P_Plant->GD_Plant[0].cLightOff     = 88;
    P_Plant->GD_Plant[0].cLightHeight  = 17;
    P_Plant->GD_Plant[0].iMoisture     = 0;
    P_Plant->GD_Plant[0].cSoilPH       = 70;
    // set today's growth statistics now too
    P_Plant->GD_Plant[1].bVisited      = TRUE;
    P_Plant->GD_Plant[1].cLightOn      = 28;
    P_Plant->GD_Plant[1].cLightOff     = 88;
    P_Plant->GD_Plant[1].cLightHeight  = 17;
    P_Plant->GD_Plant[1].iMoisture     = 0;
    P_Plant->GD_Plant[1].cSoilPH       = 70;

    // finally unlock our memory again
    GlobalUnlock(hPlantMem);
    }


BOOL PMNewPlantFile(PPLANTINFO piPlant, int iCurPlant)
    {
    // saves the contents of the PlantInfo struct into a
    // new plantfile for the give plant. This function also
    // sets default values for day 0 of plant growth.
    int i=min(gMaxPlants-1, max(0, iCurPlant));
    // first set up our filename
    sprintf(FileName, "Plant%02i.hgp", i+1);
    // if the file already exists, we do nothing
    if(GLDoesFileExist(FileName)) return 0;
    // now set up the memory required
    if(!PMPlantMemoryInit()) return 0;
    // create the new plant file
    PMPlantFileOpenNew();
    // set the day 0 GrowDay info
    PMInitFilePlantInfo(piPlant);
    // save the global memory into the file
    PMPlantFileWrite();
    // and close the file again
    PMPlantFileClose();
    // we'll always return true
    PMPlantMemoryFree();
    // now set our file to be hidden
    SetFileAttributes(FileName, FILE_ATTRIBUTE_HIDDEN);
    return TRUE;
    }


