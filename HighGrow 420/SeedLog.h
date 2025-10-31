// =====================================================================================
//
//  SEEDLOG.H - ADDING SEEDS TO THE AVAILABLE SEED LOG FILE
//
// =====================================================================================

typedef struct
   {
   int       iSeedLogCheckSum;  // checksum containing sub of all characters
   int       iSpareSeedInt;      // available space for a future version
   char      szFileVersion[5];   // version number in string form
   char      szGrowerName[30];   // user may enter this when he harvests
   char      szSeedVariety[50];  // type of seed. ie skunk, DP etc
   char      szSeedOrigin[200];  // holds the origin of the seed
   char      szSeedDescr[244];   // holds grower's description of the seed
   char      szSeedHint[200];    // holds grower's hint/tip for this seed
   PLANTSEED PS_SeedInfo;        // holds all the seed's characteristics
   } SEEDLOG;

typedef SEEDLOG FAR* PSEEDLOG; // one struct allocated per seed

BOOL SGAddSeedToLogFile(PPLANTSEED piSeed,    LPCSTR szGrowerName,  
                        LPCSTR szSeedVariety, LPCSTR szSeedOrigin,  
                        LPCSTR szSeedDescr,   LPCSTR szSeedHint);
BOOL SGGetHybridLogDetails(int iSeed, 
                           char* szHybridName, char* szSeedOrigin, 
                           char* szSeedDescr, char* szSeedHint);
int  SGCountSeedLogEntries(void);
BOOL SGGetHybridName(int iSeed, HINSTANCE hInst, char* szHybridName, int iLen);
BOOL SGDoesHybridNameExist(HINSTANCE hInst, LPCSTR szHybridName);
void SGInitLogSeed(PPLANTSEED PPS_Plant, int iSeed);
