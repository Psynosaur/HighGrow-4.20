/*****************************************************************************\
*
*  SEEDLOG.C - HANDLING GLOBAL MEMORY AND SAVING & READING THE SEED LOG FILE
*
\*****************************************************************************/

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include "resource.h"
#include "global.h"
#include "seedlog.h"
   
/*****************************************************************************\
*  SOME CONSTANTS
\*****************************************************************************/
    
 LPCSTR     lpSeedLogFile ="SeedLog.hgp";
 OFSTRUCT   SeedLogOFS;  // information from OpenFile()
 HGLOBAL    hSeedLog=0;  // handle to our global memory
 int        hSLogFile=0;  // currently opened file handle
 PSEEDLOG   P_SeedLog;        // pointer to global memory
 SEEDLOG    FS_SeedLog[100]; // array of log entries

/*****************************************************************************\
*  ALLOCATING & HANDLING GLOBAL MEMORY
/*****************************************************************************/


void SGSeedLogMemFree(void)
   {
   if(!hSeedLog) return;
   GlobalFree(hSeedLog);
   hSeedLog = 0;
   }


BOOL SGSeedLogMemInit(void)
   {
   SGSeedLogMemFree();
   hSeedLog = GlobalAlloc(GPTR, (DWORD)(sizeof(FS_SeedLog)));
   if(!hSeedLog) return FALSE;
   return TRUE;
   }


/*****************************************************************************\
*  THE MAIN FILE OPEN ROUTINE
\*****************************************************************************/


BOOL SGSeedLogFileOpen(void)
   {
   // Open the file and get its handle
   hSLogFile=OpenFile(lpSeedLogFile, (LPOFSTRUCT)&SeedLogOFS, OF_READWRITE);
   if(!hSLogFile)  return FALSE;
   return TRUE;
   }

BOOL SGSeedLogFileOpenNew(void)
   {
   // Open the file and get its handle
   hSLogFile=OpenFile(lpSeedLogFile, (LPOFSTRUCT)&SeedLogOFS,
                      OF_READWRITE|OF_CREATE);
   if(!hSLogFile)  return FALSE;
   return TRUE;
   }

void SGSeedLogFileClose(void)
   {
   if(!hSLogFile)  return;
   _lclose(hSLogFile);
   hSLogFile = 0;
   }


BOOL SGSeedLogFileRead(void)
   {  // file must be open
   LONG IOresult;    // result of a file read
   DWORD dw;

   if(!hSLogFile) return FALSE;
   // Allocate Plant buffer to the size of the file
   dw = GetFileSize((HANDLE)hSLogFile, NULL);

   // lock memory and read file
   P_SeedLog   = (PSEEDLOG)GlobalLock(hSeedLog);
   IOresult = _hread(hSLogFile, P_SeedLog, sizeof(FS_SeedLog));

   // # bytes read must equal to file size - header size
   if(IOresult != (LONG)(sizeof(FS_SeedLog)))
       {
       GlobalUnlock(hSeedLog);
       return FALSE;
       }
   GlobalUnlock(hSeedLog);
   return TRUE;
   }


BOOL SGSeedLogFileWrite(void)
    { // file must be open
    LONG IOresult;    // result of a file write
    // lock the memory
    P_SeedLog   = (PSEEDLOG)GlobalLock(hSeedLog);
    // and save the Global Plant buffer
    IOresult = _hwrite(hSLogFile, (LPCSTR)P_SeedLog, sizeof(FS_SeedLog));
    // unlock the memory again
    GlobalUnlock(hSeedLog);
    // check the results status
    if(IOresult!=(LONG)(sizeof(FS_SeedLog)))
        return FALSE;
    return TRUE;
    }


// ********************************************************************
// * OPENS & READS SEEDLOG FILE INTO GLOBAL MEMORY
// ********************************************************************

BOOL SGLogFileToMem(void)
   { // opens the seed log, reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hSeedLog. If fails, we get null.
   BOOL bSuccess = FALSE;
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return 0;
   // now set up the memory required
   if(!SGSeedLogMemInit())    return 0;
   // if the file exists, open it
   if(!SGSeedLogFileOpen())   return 0;
   // then read the file into our memory
   bSuccess = SGSeedLogFileRead();
   // free our memory if we failed to read the file
   if(!bSuccess) SGSeedLogMemFree();
   // and close it again
   SGSeedLogFileClose();

   return bSuccess;
   }


// ********************************************************************
// * OPENS & WRITES SEEDLOG LOG FILE FROM GLOBAL MEMORY
// ********************************************************************

BOOL SGLogMemToFile(void)
   { // opens the seed log, reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hSeedLog. If fails, we get null.
   BOOL bSuccess = FALSE;
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return 0;
   // get out if we don't have global memory required
   if(!hSeedLog)            return 0;
   // if the file exists, open it
   if(!SGSeedLogFileOpen()) return 0;
   // save the global memory into the file
   bSuccess = SGSeedLogFileWrite();
   // free our global memory
   SGSeedLogMemFree();
   // and close the file again
   SGSeedLogFileClose();
   return bSuccess;
   }


/******************************************************************************\
*  THE SEEDLOG DATA SAVING ROUTINE
\******************************************************************************/


BOOL SGSeedLogSaveDetails(PPLANTSEED piSeed,    LPCSTR szGrowerName,  
                          LPCSTR szSeedVariety, LPCSTR szSeedOrigin,  
                          LPCSTR szSeedDescr,   LPCSTR szSeedHint)
    { // save the harvest values for the next entry in the logfile
    int i = 0; 
    int iCheckSum = 0;
    char szBuffer[sizeof(SEEDLOG)]="\0";
    // if the global memory hasn't been created, getout
    if(!hSeedLog) return FALSE;
    // lock the memory first
    P_SeedLog = (PSEEDLOG)GlobalLock(hSeedLog);
    // now adjust the memory pointer to the next free entry
    while((P_SeedLog->iSeedLogCheckSum != 0)&(i<100))
          { // if the first byte of the name is set, then we have an entry
          P_SeedLog += 1;  // jump to the next seed
          i     += 1;  // this is just incase we are full
          }       
    // now we can initialize this seed's info
    strcpy(P_SeedLog->szFileVersion, "2.00");
    // first copy the text-based Struct elements
    strcpy(P_SeedLog->szGrowerName,  szGrowerName);
    strcpy(P_SeedLog->szSeedVariety, szSeedVariety);
    strcpy(P_SeedLog->szSeedOrigin,  szSeedOrigin);
    strcpy(P_SeedLog->szSeedDescr,   szSeedDescr);
    strcpy(P_SeedLog->szSeedHint,    szSeedHint);
    // now copy the PlantSeed Struct elements
    P_SeedLog->PS_SeedInfo.bSeedGender   = piSeed->bSeedGender;
    P_SeedLog->PS_SeedInfo.bSeedIndica   = piSeed->bSeedIndica;
    for(i=0;i<gMaxWeeks;i++)
       P_SeedLog->PS_SeedInfo.szHealthWeeks[i]    = piSeed->szHealthWeeks[i];
    for(i=0;i<gMaxWeeks;i++)
       P_SeedLog->PS_SeedInfo.szPotencyWeeks[i]   = piSeed->szPotencyWeeks[i];
    for(i=0;i<gMaxWeeks;i++)
       P_SeedLog->PS_SeedInfo.szWaterWeeks[i]     = piSeed->szWaterWeeks[i];
    for(i=0;i<gMaxWeeks;i++)
       P_SeedLog->PS_SeedInfo.szFertWeeks[i]      = piSeed->szFertWeeks[i];
    for(i=0;i<10;i++)
       P_SeedLog->PS_SeedInfo.szSplitNodes[i]     = piSeed->szSplitNodes[i];
    for(i=0;i<8;i++)
       P_SeedLog->PS_SeedInfo.szGrowthStageDay[i] = piSeed->szGrowthStageDay[i];
    for(i=0;i<8;i++)
       P_SeedLog->PS_SeedInfo.szNodeGrowth[i]     = piSeed->szNodeGrowth[i];
    for(i=0;i<8;i++)
       P_SeedLog->PS_SeedInfo.szTipsGrowth[i]     = piSeed->szTipsGrowth[i];
    // also zero our spare int
    P_SeedLog->iSpareSeedInt = 0;
    // now calculate the checksum for this seed
    CopyMemory(&szBuffer, P_SeedLog, sizeof(SEEDLOG));
    // initialize our checksum to the doper's magic number
    iCheckSum = 420;
    for(i=4;i<sizeof(SEEDLOG);i++)
        iCheckSum += szBuffer[i];
    // and save it in the buffer
    P_SeedLog->iSeedLogCheckSum = iCheckSum;
   
    // finally unlock our memory again
    GlobalUnlock(hSeedLog);
    return TRUE;
    }


/******************************************************************************\
*  THE SEEDLOG API FUNCTION FOR SAVING SEED DETAILS AT HARVEST TIME
\******************************************************************************/


BOOL SGAddSeedToLogFile(PPLANTSEED piSeed,    LPCSTR szGrowerName,  
                        LPCSTR szSeedVariety, LPCSTR szSeedOrigin,  
                        LPCSTR szSeedDescr,   LPCSTR szSeedHint)
    {
    // saves the harvest time details of the SEED into the logfile
    // if the logfile doesn't exist yet, create it and store this SEED's
    // details at log position 0.
    // If the file does exist, we add it after the previous entry in the log
    if(GLDoesFileExist((LPSTR)lpSeedLogFile))
        {
        if(!SGLogFileToMem()) return 0;
        if(!SGSeedLogSaveDetails(piSeed,       szGrowerName, szSeedVariety,
                                 szSeedOrigin, szSeedDescr,  szSeedHint)) 
            {
            // free the memory and fail
            SGSeedLogMemFree();
            return 0;
            }
        if(!SGLogMemToFile()) return 0;
        }
    else
        { // saves the harvest time details of the SEED into a
        // new SEEDlog for the give seed. This function also
        // sets default values for day 0 of seed growth.
        // now set up the memory required
        if(!SGSeedLogMemInit())     return 0;
        // create the new seed log file
        if(!SGSeedLogFileOpenNew()) return 0;
        // Save all the details of this new seed
        if(!SGSeedLogSaveDetails(piSeed,       szGrowerName, szSeedVariety,
                                 szSeedOrigin, szSeedDescr,  szSeedHint)) 
            {
            // we must close the file again
            SGSeedLogFileClose();
            // free the memory and fail
            SGSeedLogMemFree();
            return 0;
            }
        // save the global memory into the file
        if(!SGSeedLogFileWrite())   return 0;
        // and close the file again
        SGSeedLogFileClose();
        // we'll always return true
        SGSeedLogMemFree();
        }
    // now set our file to be hidden
    SetFileAttributes(lpSeedLogFile, FILE_ATTRIBUTE_HIDDEN);
    return TRUE;
    }


/******************************************************************************\
*  API FUNCTION FOR CHECKING HOW MANY SEEDS IN OUR SEEDLOG
\******************************************************************************/


int SGCountSeedLogEntries(void)
    { // counts the number of harvested seeds in the seed log
    int i=0;
    // if the seedlog doesn't exist, we'll return zero
    if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return 0;
    // if we couldn't read the log file, we also return zero
    if(!SGLogFileToMem())  return 0;
    // if the global memory hasn't been created, getout too
    if(!hSeedLog) return 0;
    // although fixed, lock the global memory first
    P_SeedLog = (PSEEDLOG)GlobalLock(hSeedLog);
    // now adjust the memory pointer to the next free entry
    while((P_SeedLog->iSeedLogCheckSum != 0)&(i<100))
          { // if the first byte of the name is set, then we have an entry
          P_SeedLog += 1;  // jump to the next seed
          i     += 1;  // this is just incase we are full
          }
    // we're done, so unlock the global memory again
    GlobalUnlock(hSeedLog);
    // now also free the memory
    SGSeedLogMemFree();
    // and return the harvested seed count
    return i;
    }


/******************************************************************************\
*  API FUNCTION FOR GETTING THE GIVEN SEED'S HYBRID NAME
\******************************************************************************/


BOOL SGGetHybridName(int iSeed, HINSTANCE hInst, char* szHybridName, int iLen)
    { // ** BUFFER MUST BE AT LEAST iLen BYTES LONG **
    // ** gets the hybrid's name for the Option Based 0 seed.
    // if the seed is one of our standard seeds, we'll look it up in the 
    // string table, but if it's a bred hybrid, we'll look it's name up
    // in the seed log
    BOOL bSuccess;
    char szBuffer[100]="\0";
    // first check if this is a predefined seed
    if(iSeed < 30)
        {
        return (BOOL)LoadString(hInst, (IDS_BT01+iSeed), szHybridName, iLen);
        }
    else
        {
        if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return 0;
        if(!SGLogFileToMem())  return 0;
        // if the global memory hasn't been created, getout
        if(!hSeedLog)          return 0;
        // although fixed, lock the global memory first
        P_SeedLog  = (PSEEDLOG)GlobalLock(hSeedLog);
        // jump to our entry in the seedlog
        P_SeedLog += max(0, min((iSeed - 30),100));
        // if our checksum failed, set our flag to false
        if(P_SeedLog->iSeedLogCheckSum)
            {
            strcpy(szHybridName, P_SeedLog->szSeedVariety);
            bSuccess = TRUE;
            }
        else
            {
            bSuccess = FALSE;
            }
        // we're done, so unlock the global memory again
        GlobalUnlock(hSeedLog);
        // now also free the memory
        SGSeedLogMemFree();
        // here we return our success flag
        return bSuccess;
        }
    }


/******************************************************************************\
*  API FUNCTION FOR GETTING THE GIVEN SEED'S TEXT DETAILS;
\******************************************************************************/


BOOL SGGetHybridLogDetails(int iSeed, 
                           char* szHybridName, char* szSeedOrigin, 
                           char* szSeedDescr,  char* szSeedHint)

    { 
    // ** gets the hybrid's details for the Option Based 0 seed.
    int i;
    BOOL bSuccess;
    int iCheckSum = 0;
    char szBuffer[sizeof(SEEDLOG)]="\0";

    if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return 0;
    if(!SGLogFileToMem())  return 0;
    // if the global memory hasn't been created, getout
    if(!hSeedLog)          return 0;
    // although fixed, lock the global memory first
    P_SeedLog  = (PSEEDLOG)GlobalLock(hSeedLog);
    // jump to our entry in the seedlog
    P_SeedLog += max(0, min(iSeed,99));
    // if our checksum failed, set our flag to false
    if(P_SeedLog->iSeedLogCheckSum)
        {
        // now calculate the checksum for this seed
        CopyMemory(&szBuffer, P_SeedLog, sizeof(SEEDLOG));
        // initialize our checksum to the doper's magic number
        iCheckSum = 420;
        for(i=4;i<sizeof(SEEDLOG);i++)
            iCheckSum += szBuffer[i];
        // now check if it's a valid checksum
        if(P_SeedLog->iSeedLogCheckSum == iCheckSum)
            { // yes, it's valid
            strcpy(szHybridName, P_SeedLog->szSeedVariety);
            strcpy(szSeedOrigin, P_SeedLog->szSeedOrigin);
            strcpy(szSeedDescr,  P_SeedLog->szSeedDescr);
            strcpy(szSeedHint,   P_SeedLog->szSeedHint);
            bSuccess = TRUE;
            }
        else
            {
            LPCSTR lpInvalid = "Invalid Seed Log Entry";
            strcpy(szHybridName, lpInvalid);
            strcpy(szSeedOrigin, lpInvalid);
            strcpy(szSeedDescr,  lpInvalid);
            strcpy(szSeedHint,   lpInvalid);
            bSuccess = FALSE;
            }
        }
    else
        {
        LPCSTR lpInvalid = "Seed Log Entry Not Found";
        strcpy(szHybridName, lpInvalid);
        strcpy(szSeedOrigin, lpInvalid);
        strcpy(szSeedDescr,  lpInvalid);
        strcpy(szSeedHint,   lpInvalid);
        bSuccess = FALSE;
        }
    // we're done, so unlock the global memory again
    GlobalUnlock(hSeedLog);
    // now also free the memory
    SGSeedLogMemFree();
    // here we return our success flag
    return bSuccess;
    }


/******************************************************************************\
*  API FUNCTION FOR CHECK IF THE GIVEN SEED'S HYBRID NAME ALREADY EXISTS
\******************************************************************************/


BOOL SGDoesHybridNameExist(HINSTANCE hInst, LPCSTR szHybridName)
    { // ** BUFFER MUST BE AT LEAST iLen BYTES LONG **
    // ** checks if the hybrid's name already exists, by checking the
    // stringtable and the seedlog
    int  i;
    BOOL bExists = FALSE;
    char szBuffer[50]="\0";
    // first check if this is a predefined seed
    for(i=0;i<30;i++)
        { // load the string from our stringtable
        LoadString(hInst, IDS_BT01+i, szBuffer, 50);
        // if it matches the given name, we getout
        if(strcmp(szBuffer, szHybridName)==0) 
            return TRUE;
        }
    if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return 0;
    if(!SGLogFileToMem())  return 0;
    // if the global memory hasn't been created, getout
    if(!hSeedLog)          return 0;
    // although fixed, lock the global memory first
    P_SeedLog  = (PSEEDLOG)GlobalLock(hSeedLog);
    // now check for a match, while the checksum is non-zero
    i = 0; // set this counter
    while((P_SeedLog->iSeedLogCheckSum != 0)&(i<100))
          { // if the first byte of the name is set, then we have an entry
          strcpy(szBuffer, P_SeedLog->szSeedVariety);
          if(strcmp(szBuffer, szHybridName)==0) 
              { // we've found a matching name
              bExists = TRUE; // we'll return this flag
              i       = 100;  // forces our loop exit
              }
          else
              {
              P_SeedLog += 1; // jump to the next seed
              i         += 1; // this is just incase we are full
              }
          }
    // we're done, so unlock the global memory again
    GlobalUnlock(hSeedLog);
    // now also free the memory
    SGSeedLogMemFree();
    // here we return our success flag
    return bExists;
    }

/******************************************************************************\
*  API FUNCTION FOR LOADING ALL THE CURRENT SEED'S SEEDLOG GROW DATA
\******************************************************************************/


void SGZeroCurrentLogSeed(PPLANTSEED PPS_Plant)
    {
    int i;
    PPS_Plant->bSeedGender = 0;
    PPS_Plant->bSeedIndica = 0; 
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szHealthWeeks[i]   = 0;
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szPotencyWeeks[i]  = 0;
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szWaterWeeks[i]    = 0;
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szFertWeeks[i]     = 0;
    for(i=0;i<10;i++)        PPS_Plant->szSplitNodes[i]    = 0;
    for(i=0;i<8;i++)         PPS_Plant->szGrowthStageDay[i]= 0;
    for(i=0;i<8;i++)         PPS_Plant->szNodeGrowth[i]    = 0;
    for(i=0;i<8;i++)         PPS_Plant->szTipsGrowth[i]    = 0;
    }


void SGSetCurrentLogSeed(PPLANTSEED PPS_Plant, PSEEDLOG P_SeedLog)
    {
    int i;
    PPS_Plant->bSeedGender = P_SeedLog->PS_SeedInfo.bSeedGender;
    PPS_Plant->bSeedIndica = P_SeedLog->PS_SeedInfo.bSeedIndica; 
    for(i=0;i<gMaxWeeks;i++) 
        PPS_Plant->szHealthWeeks[i]   = P_SeedLog->PS_SeedInfo.szHealthWeeks[i];
    for(i=0;i<gMaxWeeks;i++) 
        PPS_Plant->szPotencyWeeks[i]  = P_SeedLog->PS_SeedInfo.szPotencyWeeks[i];
    for(i=0;i<gMaxWeeks;i++) 
        PPS_Plant->szWaterWeeks[i]    = P_SeedLog->PS_SeedInfo.szWaterWeeks[i];
    for(i=0;i<gMaxWeeks;i++) 
        PPS_Plant->szFertWeeks[i]     = P_SeedLog->PS_SeedInfo.szFertWeeks[i];
    for(i=0;i<10;i++)        
        PPS_Plant->szSplitNodes[i]    = P_SeedLog->PS_SeedInfo.szSplitNodes[i];
    for(i=0;i<8;i++)         
        PPS_Plant->szGrowthStageDay[i]= P_SeedLog->PS_SeedInfo.szGrowthStageDay[i];
    for(i=0;i<8;i++)         
        PPS_Plant->szNodeGrowth[i]    = P_SeedLog->PS_SeedInfo.szNodeGrowth[i];
    for(i=0;i<8;i++)         
        PPS_Plant->szTipsGrowth[i]    = P_SeedLog->PS_SeedInfo.szTipsGrowth[i];
    }


void SGInitLogSeed(PPLANTSEED PPS_Plant, int iSeed)
    {
    // sets the hybrid's details for the Option Based 0 seed.
    int i;
    int iCheckSum = 0;
    char szBuffer[sizeof(SEEDLOG)]="\0";

    if(!GLDoesFileExist((LPSTR)lpSeedLogFile)) return;
    if(!SGLogFileToMem())  return;
    // if the global memory hasn't been created, getout
    if(!hSeedLog)          return;
    // although fixed, lock the global memory first
    P_SeedLog  = (PSEEDLOG)GlobalLock(hSeedLog);
    // jump to our entry in the seedlog
    P_SeedLog += max(0, min(iSeed,99));
    // if our checksum failed, set our flag to false
    if(P_SeedLog->iSeedLogCheckSum)
        {
        // now calculate the checksum for this seed
        CopyMemory(&szBuffer, P_SeedLog, sizeof(SEEDLOG));
        // initialize our checksum to the doper's magic number
        iCheckSum = 420;
        for(i=4;i<sizeof(SEEDLOG);i++)
            iCheckSum += szBuffer[i];
        // now check if it's a valid checksum
        if(P_SeedLog->iSeedLogCheckSum == iCheckSum)
             SGSetCurrentLogSeed(PPS_Plant, P_SeedLog);
        else SGZeroCurrentLogSeed(PPS_Plant);
        }
    // we're done, so unlock the global memory again
    GlobalUnlock(hSeedLog);
    // now also free the memory
    SGSeedLogMemFree();
    }
