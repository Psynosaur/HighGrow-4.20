#include <windows.h>
#include <commctrl.h>

/******************************************************************************\
*
*  GLOBAL.H    -   defines structs and functions used by all other source files
*
\******************************************************************************/

        
/******************************************************************************\
*  GLOBAL STRUCT DECLARATIONS
\******************************************************************************/

#define gMaxRooms      30            // allow for 30 growrooms (maximum EVER!!!)
#define gMaxPlants     gMaxRooms * 3 // allow for 3 plants per growroom
#define gMaxPots       30            // allow for 30 pots
#define gMaxLights     10            // allow for 10 reflectors

#define gMaxWeeks      40            // allow for 40 weeks maximum
#define gMaxDays       (gMaxWeeks*7) // allow for 40 weeks maximum
#define gMaxNodes      1000          // allow for 1000 nodes maximum
#define gMaxTipNodes   100           // allow for 100 tips maximum
#define gMaxLogEntries 20            // allow for 20 plants maximum

typedef struct
   { // Growroom Info Structure
   char  szRoomName[50];    // user may enter this when he edits the growroom
   int   iShadeType;        // type of growshade for this room
   int   iLampType;         // type of growlamp for this room
   int   iLampStrength;     // strength of growlamp for this room
   int   iPotType;          // type of pot for this room
   int   iSoilType;         // type of potting medium for this room
   BOOL  bClockGauge;       // true to display a clock gauge
   BOOL  bTempGauge;        // true to display a thermometer gauge
   BOOL  bHygroGauge;       // true to display a hygrometer gauge
   BOOL  bZoomGauge;        // true to display a magnifting glass
   BOOL  bVacLights;        // true for vacation mode light setting
   BOOL  bVacWater;         // true for vacation mode Watering
   BOOL  bVacFert;          // true for vacation mode Fertilizing
   int   iVacLightMM;       // mm height of light above plant
   int   iVacWaterML;       // ml of water per watering
   int   iVacWaterDays;     // number of days between waterings
   int   iVacFertWaters;    // number of waterings per fertilizing
   int   iVacFertN;         // amount of N to add
   int   iVacFertP;
   int   iVacFertK;
   int   iVacFertCa;
   } GROWROOM;

typedef struct
   { // Plant Info Structure
    char  szPlantName[40]; // user may enter this when he plants his seed
    char  szStartDate[3];  // Day/Month/Year of plant start date
    char  cGrowthStage;    // seedling, vegetative, flowering etc.
    char  cSeedChoice;     // 0-11 seed chosen (check seedgender[] for sex)
    BOOL  bRegistered;     // untill true, growth at half speed
    DWORD dwFileVersion;   // current version = 1.1 
   } PLANTINFO;

typedef struct
   { // Grow Day Structure
    BOOL  bVisited;        // set if he viewed his plants today
    BOOL  bWatered;        // set if he watered his plants today
    BOOL  bFertilized;     // set if he fertilized today
    int   iMass;           // current weight of wet plant in grams
    int   iHeight;         // current height of wet plant in mm
    int   iPotency;        // current potency of plant (%THC of dry mass)
    int   iMoisture;       // amount water in plant in ml
    char  cHealth;         // current health (used to apply day's growth)
    char  cSoilPH;         // current pH of the soil (based on fertilized)
    char  cLightOn;        // in quarter hours intervals from midnight
    char  cLightOff;       // in quarter hours intervals from midnight
    char  cLightHeight;    // height of light from ground in 125mm units
    char  szNutrients[4];  // NPK & Ca mix in each byte
    char  cTipsPruned;     // number of growing tips pruned, (max of 100)
   } GROWDAY;

typedef struct
   { // Node Structure
	int   iStartNode;      // start of node for this node
    BOOL  bGrowingTip;     // true if this is a growing tip
    BOOL  bNodeSplit;      // true if tip was pruned or must split naturally
    char  cNextNodeDays;   // number of days before a tip gets a new node
    char  cNodeLength;     // pixels 
    char  cNodeAngle;      // between -90 and +90 degrees. 0 = up
    char  cNodeLeafType;   // small, big, dead, or buds at different stages
    char  cNodeLeafID;     // 0 to however-many ID of leaf in Imagelist
   } NODE; 

typedef struct
   { // Plant Structure
   PLANTINFO PI_Plant;
   GROWDAY   GD_Plant[gMaxDays];
   NODE      NO_Plant[gMaxNodes];
   } PLANT;

typedef struct
   { // DrawNode Structure
   POINT ptDNStart;     // mm x and y of node start
   POINT ptDNEnd;       // mm x and y of node end
   char  cDNThickness;  // between 1 and 5 pixels thick
   char  cDNLeafType;   // small, big, dead, or buds at different stages
   char  cDNLeafID;     // 0 to however-many ID of leaf in Imagelist
   } DRAWNODE; 

typedef struct
   { // DrawPlant Structure
   int      iFlowerDays;         // days this plant has been flowering
   char     cLightOn;            // in quarter hours intervals from midnight
   char     cLightOff;           // in quarter hours intervals from midnight
   BOOL     bIsLightOn;          // true if we must show the light on
   POINT    ptLightOrigin;       // position of top-left of light in pixels
   DRAWNODE DN_Plant[gMaxNodes]; // holds the positions of all our nodes
   } DRAWPLANT;

typedef struct
   { // Pruning Growing Nodes
   int iCurNode;  // current node or growing tip
   int iHeight;   // the Y position of current tip's end point
   } PRUNETIP;

typedef struct
   { // TipNode Structure holds node numbers of a plant's growing tips
   int iTipNodes[gMaxTipNodes]; // allow for 100 tips per plant
   } TIPNODE;

typedef struct
   { // PlantSeed structure holds seed's characteristics
   BOOL bSeedGender;                 // True = Female
   BOOL bSeedIndica;                 // True = Indica, False = Sativa
   int  szGrowthStageDay[8];         // day numbers for growth stages
   char szHealthWeeks[gMaxWeeks+1];  // average health in % per week ending 
   char szPotencyWeeks[gMaxWeeks+1]; // average potency per week ending
   char szWaterWeeks[gMaxWeeks+1];   // average water consumption per week ending
   char szFertWeeks[gMaxWeeks+1];    // average fertilizer requirement 
   char szSplitNodes[10];            // node numbers of natural splitting nodes
   char szNodeGrowth[8];             // mm/day/growthstage of all non-tip nodes
   char szTipsGrowth[8];             // mm/day/growthstage of all growing tips
   } PLANTSEED;

typedef struct
   { // PlantLog Structure holds harvested plants information
   char      szGrowerName[30];  // user may enter this when he harvests
   char      szHarvestDate[3];  // Day/Month/Year of plant harvest date
   PLANTINFO PI_Plant; // info about plant seed selected
   GROWDAY   GD_Plant; // holds last growday's statistics
   } PLANTLOG;

/******************************************************************************\
*  GLOBAL CONSTANTS AND VARIABLES DECLARATIONS
\******************************************************************************/


typedef GROWROOM  FAR* PGROWROOM;   // one struct allocated per room
typedef PLANTINFO FAR* PPLANTINFO;  // one struct allocated per plant
typedef GROWDAY   FAR* PGROWDAY;    // 300 structs per plant (one per day)
typedef PLANT     FAR* PPLANT;      // one struct allocated per plant
typedef NODE      FAR* PNODE;       // maximum of 1000. depends on health
typedef DRAWNODE  FAR* PDRAWNODE;   // maximum of 1000. depends on health
typedef DRAWPLANT FAR* PDRAWPLANT;  // maximum of 1000. depends on health
typedef TIPNODE   FAR* PTIPNODE;    // maximum of 200 tips per plant.
typedef PLANTSEED FAR* PPLANTSEED;  // maximum of 12 seeds
typedef PLANTLOG  FAR* PPLANTLOG;   // maximum of 12 plants in the logfile

extern DRAWPLANT DP_Plant[3];    // for drawing our 3 plants
extern TIPNODE   TN_Plant[3];    // holds node numbers for our 3 plant's tips
extern PLANTSEED PS_Plant[gMaxPlants];    // holds info for all his seeds
extern PLANTLOG  PG_Plant[gMaxLogEntries]; // holds harvest plants
extern PLANTINFO PI_PlantSeed[gMaxPlants];   // holds plant info for ALL our plants

extern GROWROOM  GR_Room[gMaxRooms]; // holds growroom info

#define CDN_MM   0 // style for calculating nodes in mm
#define CDN_PIX  1 // style for calculating nodes in pixels

extern BOOL gbGodMode;        // true if his password is "IAMGOD"
extern BOOL gbRegistered;     // true if this is a registered version
extern BOOL gbPaid;           // true if this is a paid-for version
extern BOOL gbAlphaBlending;  // true if we can perform alpha-blending of bitmaps

extern int giCurPlant; // current plant for any operations
extern int giGrowDay;  // current day of growth
extern int giOldestGrowDay; // oldest plant's growday
extern int giLastVisited;
extern int giLastWatered;
extern int giLastPruned;
extern int giLastFertilized;
extern int giGrowthStage;
extern int giGender;
extern int giHeight;       // millimeters
extern int giHealth;
extern int giPotency;
extern int giMoisture;     // milliliters
extern int giSoilpH;
extern int giLightOn;  
extern int giLightOff;
extern int giLightHeight;
extern int giGrowingNodes;
extern int giGrowingTips;
extern int giGrowingMass;
extern int giPrunedTips;
extern double gdPrunedMass;

extern char gszStartDate[3]; // holds day/month/year of plant start
extern char strbuf[200];      // holds temporary text (used in all files)

extern HCURSOR hNormCursor;
extern HCURSOR hWaitCursor;

extern LPCSTR lpSecurePassword;


/******************************************************************************\
*  GLOBAL FUNCTION DECLARATIONS
\******************************************************************************/

extern PLANTINFO PI_CurPlant;   // filled with GLInitPlantInfo for this plant
extern GROWDAY   GD_CurPlant;   // only holds ONE growday. filled by GLInitGrowDay

BOOL GLInitGrowDay(PGROWDAY gdPlant);      // before showing the day
BOOL GLInitPlantInfo(PPLANTINFO piPlant);  // before the activities dialog
BOOL GLSaveGrowDay(PGROWDAY gdPlant);       // to saving the day's changes
BOOL GLSavePlantInfo(PPLANTINFO piPlant);   // to saving the day's changes

void GLLoadGrowthVariables(PPLANT plPlant);
void GLSaveGrowthVariables(PPLANT plPlant);
void GLGetGrowthStageString(LPSTR szBuffer, int iStage);

// use this to debug with
void Blow(LPCTSTR lpBlowText);

// use this to calculate random numbers
int Random(int minrand, int maxrand);

// use this to get a temporary filename and path
int GLGetTempFilePathAndName(DWORD dlen, LPTSTR lpBuffer);

// use this to scramble and unscramble text
BOOL GLScrambleText(LPSTR lpText);
BOOL GLUnscrambleText(LPSTR lpText);

// use this date function to return a date in the form DMY
BOOL GLFormatDate(char* szDateBuf, char* szResult);
// use this date function to return today's date in the form DMY
BOOL GLDateNow(char *szDateBuf);
// Use this format today's date in form Mon, 15 June 97
BOOL GLNextFormatDate(char* szDateBuf, char* szResult, int dayoffset);
// Use this format today's date in form Monday, 15 June 1997
BOOL GLFormatDateNow(char* szResult);
// use this one to return next date in form DD/MM/YY
BOOL GLNextShortFormatDate(char* szDateBuf, char* szResult, int dayoffset);
// use this one to calculate the difference in days between 2 dates
int GLDiffDate(char* szDate1, char* szDate2);
// use this to calculate number of 15min intervals passed since midnight
int GLGetTimeNowQuarters(void);
// use this to calculate number of seconds passed since midnight
int GLGetSecsSinceMidnight(void);
// returns the photoperiod in quarter of an hour intervals
int GLGrowDayPhotoperiod(PPLANT plPlant, int iGrowDay);
int GLCurPlantPhotoperiod(void);

// use this function to create our dialog fonts
HFONT GLCreateDialogFont(int iSize, int iAngle, int iStyle);

// Use these function to read and write a number into a static text control
void GLSetNumText(int num, HWND hwnd, int ID);
int  GLGetNumText(HWND hwnd, int ID);

// void GLDrawBitmap(HDC hdc, RECT rc, HBITMAP hBitmap);
void GLTileBitmap(HDC hdc, RECT rc, HBITMAP hBitmap);
void GLTileImage(HDC hdc, RECT rcWin, HIMAGELIST hImage);
void Frame3D(HDC hdc, RECT rc, HANDLE hTopPen, HANDLE hBottomPen);
void Rect3D(HDC hdc, RECT rc, HANDLE hPen, HANDLE hBrush);
void FrameRect3D(HDC hdc, RECT rc, HPEN hFacePen, HPEN hTopPen, HPEN hBotPen);
void Rectangle3D(HDC hdc, RECT rc, HPEN hFPen, HBRUSH hFBrush, HPEN hTPen, HPEN hBPen);

// Use this function to paint the gauge controls
// void GLDisplayProgress(HWND hDlg, int ID, int cur, int max);
void GLDisplayProgress(HWND hDlg, int ID, HBRUSH hBrush, int cur, int max);

// Center the current dialog on the screen.
void DlgCenter(HWND hDlg);
// Resizes the dialog inside the HighGrow Program window.
void GLSizeDialogToFit(HWND hdlg);

// Checks to see if the file exists
BOOL GLDoesFileExist(LPSTR lpPlantFile);

// counts the number of files matching the given spec
int GLGetFileCount(LPCSTR szFileSpec);

// converts bitmap files to the HighGrow private growroom image format
int GLConvertGrowroomImageFiles(void);
// filling a combobox with the names of the growroom image files
int GLFillGrowFileNamesCombo(HWND hDlg, UINT ComboID);
// filling a listbox with the names of the growroom image files
void GLFillGrowroomNamesListbox(HWND hDlg, UINT ListID);

// greys or ungreys menu items
void GLInitGrowMenus(HWND hwnd);

