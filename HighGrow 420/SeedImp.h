// -------------------------------------------------------------------------------------
//
//  SEEDIMP.H - IMPORTING AND EXPORTING SEED FILES
//
// -------------------------------------------------------------------------------------

typedef struct
   {
   int       iSeedFileCheckSum;  // checksum containing sub of all characters
   int       iSpareSeedInt;      // available space for a future version
   char      szFileType[30];     // description for editors
   char      szFileVersion[5];   // version number in string form
   char      szGrowerName[30];   // user may enter this when he harvests
   char      szSeedVariety[50];  // type of seed. ie skunk, DP etc
   char      szSeedOrigin[200];  // holds the origin of the seed
   char      szSeedDescr[244];   // holds grower's description of the seed
   char      szSeedHint[200];    // holds grower's hint/tip for this seed
   PLANTSEED PS_SeedInfo;        // holds all the seed's characteristics
   } FILESEED;

typedef FILESEED FAR* PFILESEED; // one struct allocated per seed

void SISeedBreedDialog(HWND hwnd, HINSTANCE hInst);
BOOL SIImportSeedFile(HWND hWnd, HINSTANCE hInst);
BOOL SIExportSeedFile(HWND hwnd, HINSTANCE hInst, int iSeed);

                           