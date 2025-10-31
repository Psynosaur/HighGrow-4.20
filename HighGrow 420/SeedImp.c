
// =====================================================================================
//
//  SEEDIMP.C - IMPORTING AND EXPORTING SEED FILES
//
// =====================================================================================

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include "resource.h"
#include "global.h"
#include "SeedImp.h"
#include "SeedList.h"
#include "PlantSeed.h"
#include "DIBitmap.h"
#include "Registry.h"
#include "SeedLog.h"
#include "Toolbar.h"

// -----------------------------------------------------------------------
//   FILESEED STRUCTURE
// -----------------------------------------------------------------------

PFILESEED P_FSeed;               // pointer to global memory

// -----------------------------------------------------------------------
//   SOME CONSTANTS
// -----------------------------------------------------------------------

char      szSeedFile[255] = "\0";  // seed file name string
OFSTRUCT  OfStruct;                // information from OpenFile()
HGLOBAL   hSeedMem=0;              // handle to our global memory
int       hSeedFile=0;             // currently opened file handle


// -----------------------------------------------------------------------
//   ALLOCATING & HANDLING GLOBAL MEMORY
// -----------------------------------------------------------------------

void SISeedMemoryFree(void)
   {
   if(!hSeedMem) return;
   GlobalFree(hSeedMem);
   hSeedMem = 0;
   }


BOOL SISeedMemoryInit(void)
   {
   SISeedMemoryFree();
   hSeedMem = GlobalAlloc(GPTR, (DWORD)(sizeof(FILESEED)));
   if(!hSeedMem) return FALSE;
   return TRUE;
   }


// -----------------------------------------------------------------------
//  SEEDFILE OPENING AND CLOSING ROUTINE
// -----------------------------------------------------------------------


BOOL SISeedFileOpen(void)
   {
   // first check for the failure case
   if(!szSeedFile[0]) return FALSE;
   // Open the file and get its handle
   hSeedFile=OpenFile(szSeedFile, (LPOFSTRUCT)&OfStruct, OF_READWRITE);
   if(!hSeedFile)  return FALSE;
   return TRUE;
   }


BOOL SISeedFileOpenNew(void)
   {
   // first check for the failure case
   if(!szSeedFile[0]) return FALSE;
   // Open the file and get its handle
   hSeedFile=OpenFile(szSeedFile,(LPOFSTRUCT)&OfStruct,OF_READWRITE|OF_CREATE);
   if(!hSeedFile)  return FALSE;
   return TRUE;
   }


void SISeedFileClose(void)
   {
   if(!hSeedFile)  return;
   _lclose(hSeedFile);
   hSeedFile = 0;
   }


// -----------------------------------------------------------------------
//  SEEDFILE READING AND WRITING ROUTINES
// -----------------------------------------------------------------------


BOOL SISeedFileRead(void)
   {  // file must be open
   char szText[200]="\0";
   LONG IOresult;    // result of a file read
   DWORD dw;
   if(!hSeedFile) return FALSE;
   // Allocate buffer to the size of the file
   dw = GetFileSize((HANDLE)hSeedFile, NULL);
   // lock memory and read file
   P_FSeed   = (PFILESEED)GlobalLock(hSeedMem);
   IOresult = _hread(hSeedFile, P_FSeed, sizeof(FILESEED));
   // # bytes read must equal to file size - header size
   if(IOresult != (LONG)(sizeof(FILESEED)))
       {
       GlobalUnlock(hSeedMem);
       return FALSE;
       }
   // here we can unlock our memory again
   GlobalUnlock(hSeedMem);
   return TRUE;
   }


BOOL SISeedFileWrite(void)
   { // file must be open
   LONG IOresult;    // result of a file write
   // lock the memory
   P_FSeed = (PFILESEED)GlobalLock(hSeedMem);
   // now we can save the Global buffer to a file
   IOresult = _hwrite(hSeedFile, (LPCSTR)P_FSeed, sizeof(FILESEED));
   // unlock the memory again
   GlobalUnlock(hSeedMem);
   // check the results status
   if(IOresult!=(LONG)sizeof(FILESEED))
      return FALSE;
   return TRUE;
   }


// =============================================================
// EXPORTING SEEDS
// =============================================================
// GETTING THE NAME OF THE SEED FILE TO SAVE INTO
// -------------------------------------------------------
  
static OPENFILENAME ofnSeed ;

BOOL SIFileSaveDlg (HWND hwnd, LPSTR lpstrFileName)
     {
     char szStartPath[250] = "\0";
     static char szFilter[] = "HighGrow Seed Files (*.hgs)\0*.hgs\0\0";
     // read the name of his startup directory
     REReadRegistryKey("Startup in", (LPCTSTR)szStartPath, 250);
     // now fill the OPENFILENAME struct elements
     ofnSeed.lStructSize       = sizeof (OPENFILENAME) ;
     ofnSeed.hwndOwner         = hwnd ;
     ofnSeed.hInstance         = NULL ;
     ofnSeed.lpstrFilter       = szFilter ;
     ofnSeed.lpstrCustomFilter = NULL ;
     ofnSeed.nMaxCustFilter    = 0 ;
     ofnSeed.nFilterIndex      = 0 ;
     ofnSeed.lpstrFile         = lpstrFileName ;
     ofnSeed.nMaxFile          = _MAX_PATH ;
     ofnSeed.lpstrFileTitle    = NULL ;
     ofnSeed.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
     ofnSeed.lpstrInitialDir   = szStartPath ;
     ofnSeed.lpstrTitle        = "Export New Hybrid" ;
     ofnSeed.Flags             = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR ;
     ofnSeed.nFileOffset       = 0 ;
     ofnSeed.nFileExtension    = 0 ;
     ofnSeed.lpstrDefExt       = "hgs" ;
     ofnSeed.lCustData         = 0L ;
     ofnSeed.lpfnHook          = NULL ;
     ofnSeed.lpTemplateName    = NULL ;
     // now get the save filename
     return GetSaveFileName (&ofnSeed) ;
     }

// -----------------------------------------------------------------------
//  THE FILESEEDFILE CREATION ROUTINE (WHEN HE PLANTS HIS SEED)
// -----------------------------------------------------------------------

BOOL SINewSeedFile(HWND hWnd,
                   PPLANTSEED piSeed,    LPCSTR szGrowerName,  
                   LPCSTR szSeedVariety, LPCSTR szSeedOrigin,  
                   LPCSTR szSeedDescr,   LPCSTR szSeedHint)

    {
    int  i, iCheckSum;
    char szBuffer[sizeof(FILESEED)]="\0";
    // saves the contents of the PlantSeed struct into a
    // new seedfile for the given seed. This function also
    // sets default values for the rest of the file
    // first set up our filename
    sprintf(szSeedFile, "%s.hgs", szSeedVariety);
    // now get the filename to save it under
    // return false if he didn't enter any name
    if(!SIFileSaveDlg(hWnd, szSeedFile)) return 0;
    // if the file already exists, we'll delete it first
    if(GLDoesFileExist((LPSTR)szSeedFile))
        DeleteFile(szSeedFile);
    // now set up the memory required
    if(!SISeedMemoryInit())              return 0;
    // create the new plant file
    SISeedFileOpenNew();
    // if the global memory hasn't been created, getout
    if(!hSeedMem)                        return 0;
    // first we must lock the memory
    P_FSeed = (PFILESEED)GlobalLock(hSeedMem);
    // now we can initialize this seed's info
    strcpy(P_FSeed->szFileType, "HighGrow Marijuana Seed File");
    strcpy(P_FSeed->szFileVersion, "3.00");
    // first copy the text-based Struct elements
    strcpy(P_FSeed->szGrowerName,  szGrowerName);
    strcpy(P_FSeed->szSeedVariety, szSeedVariety);
    strcpy(P_FSeed->szSeedOrigin,  szSeedOrigin);
    strcpy(P_FSeed->szSeedDescr,   szSeedDescr);
    strcpy(P_FSeed->szSeedHint,    szSeedHint);
    // now copy the PlantSeed Struct elements
    P_FSeed->PS_SeedInfo.bSeedGender   = piSeed->bSeedGender;
    P_FSeed->PS_SeedInfo.bSeedIndica   = piSeed->bSeedIndica;
    for(i=0;i<gMaxWeeks;i++)
       P_FSeed->PS_SeedInfo.szHealthWeeks[i]    = piSeed->szHealthWeeks[i];
    for(i=0;i<gMaxWeeks;i++)
       P_FSeed->PS_SeedInfo.szPotencyWeeks[i]   = piSeed->szPotencyWeeks[i];
    for(i=0;i<gMaxWeeks;i++)
       P_FSeed->PS_SeedInfo.szWaterWeeks[i]     = piSeed->szWaterWeeks[i];
    for(i=0;i<gMaxWeeks;i++)
       P_FSeed->PS_SeedInfo.szFertWeeks[i]      = piSeed->szFertWeeks[i];
    for(i=0;i<10;i++)
       P_FSeed->PS_SeedInfo.szSplitNodes[i]     = piSeed->szSplitNodes[i];
    for(i=0;i<8;i++)
       P_FSeed->PS_SeedInfo.szGrowthStageDay[i] = piSeed->szGrowthStageDay[i];
    for(i=0;i<8;i++)
       P_FSeed->PS_SeedInfo.szNodeGrowth[i]     = piSeed->szNodeGrowth[i];
    for(i=0;i<8;i++)
       P_FSeed->PS_SeedInfo.szTipsGrowth[i]     = piSeed->szTipsGrowth[i];
    // also zero our spare int
    P_FSeed->iSpareSeedInt = 0;
    // now calculate the checksum for this seed
    CopyMemory(&szBuffer, P_FSeed, sizeof(FILESEED));
    // initialize our checksum to the doper's magic number
    iCheckSum = 420;
    for(i=4;i<sizeof(FILESEED);i++)
        iCheckSum += szBuffer[i];
    // and save it in the buffer
    P_FSeed->iSeedFileCheckSum = iCheckSum;
    // finally we can unlock our memory again
    GlobalUnlock(hSeedMem);
    // now save the global memory into the file
    SISeedFileWrite();
    // and close the file again
    SISeedFileClose();
    // now free the memory again
    SISeedMemoryFree();
    return TRUE;
    }


// =====================================================================================
//  DISPLAYING AND INITIALIZING THE SEED BREEDING DIALOG
// =====================================================================================

HFONT    hBreedHdrFont;
HFONT    hBreedTxtFont;
HBITMAP  hBreedBitmap;
HPALETTE hBreedPalette;
/*
void SIShowComboValues(HWND hDlg, PPLANTSEED PPS_Seed)
    {
    // ========================= START DEBUG CODE ==================================
    // ================= (CALL BREEDING1 DIALOG TEMPLATE) ==========================
    char szValue[50]="\0";
    int  i;
    int  iSelect  = SendDlgItemMessage(hDlg, IDC_CM01, CB_GETCURSEL, 0, 0L);
    HWND hListbox = GetDlgItem(hDlg, IDC_LB01);

    // first remove everything that's currently beiong displayed
    SendMessage(hListbox, LB_RESETCONTENT, 0, 0L);

    // now we check what we must display
    if(iSelect == 0)
       { // here we show the plant health
       for(i=0;i<gMaxWeeks;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szHealthWeeks[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 1)
       { // here we show the plant potency
       for(i=0;i<gMaxWeeks;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szPotencyWeeks[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 2)
       { // here we show the plant water requirements
       for(i=0;i<gMaxWeeks;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szWaterWeeks[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 3)
       { // here we show the plant nutrient requirements
       for(i=0;i<gMaxWeeks;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szFertWeeks[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 4)
       { // here we show the plant split node days
       for(i=0;i<10;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szSplitNodes[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 5)
       { // here we show the plant growth stage days
       for(i=0;i<8;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szGrowthStageDay[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 6)
       { // here we show the plant node growth
       for(i=0;i<8;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szNodeGrowth[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    if(iSelect == 7)
       { // here we show the plant tip growth
       for(i=0;i<8;i++)
           {
           wsprintf(szValue, "%i", PPS_Seed->szTipsGrowth[i]);
           SendMessage(hListbox, LB_ADDSTRING, 0, (LPARAM)szValue);
           }
       }
    // now select the first listbox item
    SendMessage(hListbox, LB_SETCURSEL, 0, 0L);
    // ========================= END DEBUG CODE ==================================
    }
*/

void SIInitBreedDialog(HWND hDlg, HINSTANCE hInst, PPLANTSEED PPS_Seed)
    {
    int  i, iMale, iSeedMale, iSeedFemale;
    int iRoomOfs = (TBGetComboGrowroom()-1)*3;
    char szNameFemale[40]="\0";
    char szNameMale[40]="\0";
    char szTempBuffer[200]="\0";
    char szOriginText[200]="\0";
    long lp=MAKELPARAM(TRUE, 0);

    // first position the damn dialog
    DlgCenter(hDlg);

    // first we must find the female and male seeds
    iSeedFemale = PI_CurPlant.cSeedChoice; // only females get this dialog
    for(i=0;i<3;i++)
        { // check for a male, and if found we set our int
        if(PS_Plant[i+iRoomOfs].bSeedGender==FALSE)  
            { // we'll getout with the first male found
            iMale = i+iRoomOfs; // found a male
            i     = 3; // forces the loop exit
            }
        }
    // now we set our male plant's seed choice
    iSeedMale = PI_PlantSeed[iMale].cSeedChoice;
//    iSeedMale = 9;

    // here we limit the amount typable into the edit controls
    SendDlgItemMessage(hDlg, IDC_ED01, EM_LIMITTEXT, 39, 0);
    SendDlgItemMessage(hDlg, IDC_ED03, EM_LIMITTEXT, 243, 0);
    SendDlgItemMessage(hDlg, IDC_ED04, EM_LIMITTEXT, 199, 0);
    // create our special dialog fonts
    hBreedHdrFont = GLCreateDialogFont(24,0,FW_BOLD);
    hBreedTxtFont = GLCreateDialogFont(18,0,FW_BOLD);
    // if we created the seed text font, set it to all the heading controls
    if(hBreedHdrFont)
        {
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hBreedHdrFont,lp);
        }
    if(hBreedTxtFont)
        {
        SendDlgItemMessage(hDlg,IDC_ST03,WM_SETFONT,(WPARAM)hBreedTxtFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST05,WM_SETFONT,(WPARAM)hBreedTxtFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST06,WM_SETFONT,(WPARAM)hBreedTxtFont,lp);
        }
    // load our seed imagelist (MUST USE THIS METHOD FOR 256 COLOUR BITMAPS)
    hBreedBitmap = DILoadResourceBitmap(hInst, "SeedList", &hBreedPalette);

    // here we display the new seed's origin
    REReadRegistryKey("Grower's Name", (LPCTSTR)strbuf, 30);
    SGGetHybridName(iSeedFemale-1, hInst, (char*)&szNameFemale, 40);
    SGGetHybridName(iSeedMale-1,   hInst, (char*)&szNameMale,   40);
    LoadString(hInst, (IDS_BREED_01+Random(0, 9)), szTempBuffer, 200);
    wsprintf(szOriginText, szTempBuffer, strbuf, szNameFemale, szNameMale);
    SendDlgItemMessage(hDlg, IDC_ST01, WM_SETTEXT, 0, (LPARAM)szOriginText);
    // also display the dialog header text
    szTempBuffer[0]=0;
    wsprintf(szTempBuffer, "%s has Seeds", PI_CurPlant.szPlantName);
    SendDlgItemMessage(hDlg, IDC_ST02, WM_SETTEXT, 0, (LPARAM)szTempBuffer);

    // here we'll combine the two seeds and create a new one
    PSCombinePlantSeeds(iSeedMale, iSeedFemale, PPS_Seed);

    // ========================= START DEBUG CODE ==================================
    // ================= (CALL BREEDING1 DIALOG TEMPLATE) ==========================
    // now fill the combo box with out genetic debugging info
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_RESETCONTENT, 0, 0L);
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Health");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Potency");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Watering");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Fertilizing");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Split Nodes");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Growth Stage");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Node Growth");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_ADDSTRING, 0, (LPARAM)"Tip Growth");
//    SendDlgItemMessage(hDlg, IDC_CM01, CB_SETCURSEL, 0, 0L);
//    SIShowComboValues(hDlg, PPS_Seed);
    // ========================= END DEBUG CODE ==================================

    }



BOOL SISaveBreedDialog(HWND hwnd, PPLANTSEED PPS_Seed, HINSTANCE hInst)
    {
    int  i;
    char szSeedVariety[50]="\0";
    char szGrowerName[30]="\0";
    char szSeedOrigin[200]="\0";
    char szSeedDescr[250]="\0";
    char szSeedHint[200]="\0";
    LPCSTR lpCaption="Incomplete Seed Log Entry";

    // first read the origin static's text
    SendDlgItemMessage(hwnd, IDC_ST01, WM_GETTEXT, 200, (LPARAM)szSeedOrigin);
    // now read the new hybrid's name edit control
    SendDlgItemMessage(hwnd, IDC_ED01, WM_GETTEXT,  50, (LPARAM)szSeedVariety);
    // if he didn't fill in the variety name, we cannot continue
    if(strlen(szSeedVariety)==0)
        {
        MessageBox(hwnd, "Please Enter a Name for the New Hybrid!", 
                         lpCaption, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(GetDlgItem(hwnd, IDC_ED01));
        return FALSE;
        }
    // now we'll check if the name already exists for this hybrid
    if(SGDoesHybridNameExist(hInst, szSeedVariety))
        {
        MessageBox(hwnd, "The HighGrow Seed List already has a Hybrid with this name!", 
                         lpCaption, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(GetDlgItem(hwnd, IDC_ED01));
        return FALSE;
        }

    // now read the hybrid General description edit control
    SendDlgItemMessage(hwnd, IDC_ED03, WM_GETTEXT, 250, (LPARAM)szSeedDescr);
    // if he didn't fill in a plant description, we cannot continue
    if(strlen(szSeedDescr)==0)
        {
        MessageBox(hwnd, "Please Enter a Description of this Seed!", 
                         lpCaption, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(GetDlgItem(hwnd, IDC_ED03));
        return FALSE;
        }
    // now read the grower's hint edit control
    SendDlgItemMessage(hwnd, IDC_ED04, WM_GETTEXT, 200, (LPARAM)szSeedHint);
    // if he didn't fill in a grower's hint, we cannot continue
    if(strlen(szSeedHint)==0)
        {
        MessageBox(hwnd, "Please Enter a Hint for Growers of this Seed!", 
                         lpCaption, MB_OK|MB_ICONEXCLAMATION);
        SetFocus(GetDlgItem(hwnd, IDC_ED04));
        return FALSE;
        }

    // here we save the dialog's details and create the new seed's file
    if(!SGAddSeedToLogFile(PPS_Seed,     szGrowerName, szSeedVariety,
                          szSeedOrigin, szSeedDescr,  szSeedHint))
        {
        MessageBox(hwnd, "Unable to add this new seed to the Seed Log!", 
                         "Error Encountered", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
        }
    // if he's paid us, we'll ask him to save a seed file
    if(gbPaid) 
        {
        i = MessageBox(hwnd, "New seed successfully Imported into the Seedlist\n\
Would you like to Export it as a HighGrow Seedfile?", 
                         "Export This Seed?", MB_YESNO|MB_ICONQUESTION);
        if(i ==IDYES)
            {
            if(SINewSeedFile(hwnd, PPS_Seed, szGrowerName, szSeedVariety,
                             szSeedOrigin,    szSeedDescr,  szSeedHint))
                MessageBox(hwnd, "HighGrow Seedfile Created Successfully!", 
                                "Seed Exported", MB_OK);
            else
                MessageBox(hwnd, "HighGrow Seedfile Not Created!", 
                                 "Seed Not Exported", MB_OK);
            }
        }
    return TRUE;
    }


// -----------------------------------------------------------------------
//  WINPROC FOR OUR SEED EXPORT DIALOG
// -----------------------------------------------------------------------


BOOL CALLBACK SIBreedDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   static PLANTSEED PS_NewSeed;        // holds info for our new combined seed

   switch (message)
      {
      case WM_INITDIALOG:
           SIInitBreedDialog(hDlg, dhInst, &PS_NewSeed);
      return (TRUE);

      case WM_DESTROY:
            if(hBreedHdrFont) DeleteObject(hBreedHdrFont);
            if(hBreedTxtFont) DeleteObject(hBreedTxtFont);
            DIFreeBitmap(hBreedBitmap, hBreedPalette);
      return (FALSE);  // only done because we're redirecting the focus

      case WM_ERASEBKGND:
          if(-1==GetDeviceCaps((HDC)wParam, NUMCOLORS))
              { // if we're in high colour, we'll draw the light leaf background
              DITileDlgLeafBitmap(hDlg, (HDC)wParam);
              return (TRUE);
              }
      return (FALSE);

      case WM_CTLCOLORSTATIC:
          {
          int i = GetDlgCtrlID((HWND)lParam);
          if((i>=IDC_ST02)||(i<=IDC_ST07))
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            // ============ START DEBUG CODE ===============
//            case IDC_CM01:
//               if(HIWORD(wParam)==CBN_SELCHANGE)
//                  SIShowComboValues(hDlg, &PS_NewSeed);
//            break;
            // ============= END DEBUG CODE ================

            case IDOK: // save to log and exit if he filled everything in
                 if(SISaveBreedDialog(hDlg, &PS_NewSeed, dhInst))
                     EndDialog(hDlg,TRUE);
            return (TRUE);

            case IDCANCEL: // save to log and exit if he filled everything in
                   EndDialog(hDlg,FALSE);
            return (TRUE);
            }
      return (FALSE);

      }
   return (FALSE);
   }


// -----------------------------------------------------------------------
//  MAIN SEED EXPORT DIALOG API WORD
// -----------------------------------------------------------------------

void SISeedBreedDialog(HWND hwnd, HINSTANCE hInst)
    { // we'll only allow breeding if there's a male plant around
    int  i, iMale=0;
    int iRoomOfs = (TBGetComboGrowroom()-1)*3;

    // first we must find the male plants
    for(i=0;i<3;i++)
        { // check for a male, and if found we set our int
        if(PS_Plant[i+iRoomOfs].bSeedGender==FALSE)  
            { // we'll getout if this male is more than flowering
            if(PI_PlantSeed[i+iRoomOfs].cGrowthStage>=5)
                {
                iMale = i+iRoomOfs; // found a male
                i     = 3; // forces the loop exit
                }
            }
        }
    if(iMale)
        DialogBox(hInst, "BREEDING", hwnd, SIBreedDlgProc);
    }


// =============================================================
// IMPORTING SEEDS
// =============================================================
// GETTING THE NAME OF THE SEED FILE TO SAVE INTO
// -------------------------------------------------------
  
static OPENFILENAME ofnImport;

BOOL SIFileOpenDlg(HWND hwnd, LPSTR lpstrFileName)
     {
     char szStartPath[250] = "\0";
     static char szFilter[] = "HighGrow Seed Files (*.hgs)\0*.hgs\0\0";
     // read the name of his startup directory
     REReadRegistryKey("Startup in", (LPCTSTR)szStartPath, 250);
     // now fill the OPENFILENAME struct elements
     ofnImport.lStructSize       = sizeof (OPENFILENAME) ;
     ofnImport.hwndOwner         = hwnd ;
     ofnImport.hInstance         = NULL ;
     ofnImport.lpstrFilter       = szFilter ;
     ofnImport.lpstrCustomFilter = NULL ;
     ofnImport.nMaxCustFilter    = 0 ;
     ofnImport.nFilterIndex      = 0 ;
     ofnImport.lpstrFile         = lpstrFileName ;
     ofnImport.nMaxFile          = _MAX_PATH ;
     ofnImport.lpstrFileTitle    = NULL ;
     ofnImport.nMaxFileTitle     = _MAX_FNAME + _MAX_EXT ;
     ofnImport.lpstrInitialDir   = szStartPath ;
     ofnImport.lpstrTitle        = "Import New Hybrid" ;
     ofnImport.Flags             = OFN_HIDEREADONLY|OFN_NOCHANGEDIR ;
     ofnImport.nFileOffset       = 0 ;
     ofnImport.nFileExtension    = 0 ;
     ofnImport.lpstrDefExt       = "hgs" ;
     ofnImport.lCustData         = 0L ;
     ofnImport.lpfnHook          = NULL ;
     ofnImport.lpTemplateName    = NULL ;
     // now get the open filename
     return GetOpenFileName (&ofnImport) ;
     }


// -----------------------------------------------------------------------
//  MAIN SEED IMPORT API FUNCTION
// -----------------------------------------------------------------------


BOOL SIImportSeedFile(HWND hWnd, HINSTANCE hInst)
    { 
    int  i, iCheckSum;
    BOOL bSuccess=FALSE;
    char szBuffer[sizeof(FILESEED)]="\0";
    // ask him which file to import
    if(!SIFileOpenDlg(hWnd, szSeedFile)) return 0;
    // if the file already exists, we do nothing
    if(!GLDoesFileExist(szSeedFile))     return 0;
    // if the file exists, open it
    if(!SISeedFileOpen())            return 0;
    // now set up the memory required
    if(!SISeedMemoryInit())          return 0;
    // read the file
    SISeedFileRead();
    // now we must lock the memory
    P_FSeed = (PFILESEED)GlobalLock(hSeedMem);
    // now check the checksum for this seed
    CopyMemory(&szBuffer, P_FSeed, sizeof(FILESEED));
    // initialize our checksum to the doper's magic number
    iCheckSum = 420;
    for(i=4;i<sizeof(FILESEED);i++)
        iCheckSum += szBuffer[i];
    // now check if our checksum matches the file checksum
    if(P_FSeed->iSeedFileCheckSum == iCheckSum)
        {
        // it matches, so now check if we already have this seed in our list
        if(SGDoesHybridNameExist(hInst, P_FSeed->szSeedVariety))
            {
            MessageBox(hWnd, "The HighGrow Seed List already has a Hybrid with this name!", 
                             "HighGrow Seed Import", MB_OK|MB_ICONEXCLAMATION);

            }
        else
            {
            if(SGAddSeedToLogFile((PPLANTSEED)&P_FSeed->PS_SeedInfo,   
                                  P_FSeed->szGrowerName,
                                  P_FSeed->szSeedVariety, P_FSeed->szSeedOrigin,
                                  P_FSeed->szSeedDescr,   P_FSeed->szSeedHint))
                {
                MessageBox(hWnd, "New seed successfully Imported into the Seedlist!", 
                                 "HighGrow Seed Import", MB_OK|MB_ICONEXCLAMATION);
                bSuccess = TRUE;
                }
            else
                MessageBox(hWnd, "Unable to add this new seed to the Seed Log!", 
                                 "Error Encountered", MB_OK|MB_ICONEXCLAMATION);
            }
        }
    else 
        {
        MessageBox(hWnd, "This is an Invalid HighGrow Seed File!",
                         "Error Encountered", MB_OK|MB_ICONEXCLAMATION);
        }
    // finally we can unlock our memory again
    GlobalUnlock(hSeedMem);
    // now free the memory again
    SISeedMemoryFree();
    // and close it again
    SISeedFileClose();

    return bSuccess;
    }

// -----------------------------------------------------------------------
//  SEEDLIST'S EXPORT BUTTON API FUNCTION
// -----------------------------------------------------------------------


BOOL SIExportSeedFile(HWND hwnd, HINSTANCE hInst, int iSeed)
    {
    char szType[40]="\0";
    char szOrig[200]="\0";
    char szDesc[250]="\0";
    char szHint[200]="\0";
    PLANTSEED PS_ExpSeed;  // holds info for our exported seed
    // first stop him from exporting a standard issue seed
    if(iSeed<30)
        {
        MessageBox(hwnd, "You may only export the seeds which you have\nadded \
to the HighGrow Seedlist yourself (either\nby breeding them or by importing them).",
                         "Unable to Export this Seed", MB_OK|MB_ICONEXCLAMATION);
        return FALSE;
        }
    // now get the log-details for this seed
    REReadRegistryKey("Grower's Name", (LPCTSTR)strbuf, 30);
    SGGetHybridLogDetails(iSeed-30, (char*)&szType, (char*)&szOrig,
                                    (char*)&szDesc, (char*)&szHint);

    // now try to save the details to a seed file, reporting our success
    if(SINewSeedFile(hwnd, &PS_ExpSeed, strbuf, szType, szOrig, szDesc, szHint))
        {
        MessageBox(hwnd, "The HighGrow Seedfile was created successfully!", 
                         "Seed Exported", MB_OK|MB_ICONEXCLAMATION);
        return TRUE;
        }
    else
        MessageBox(hwnd, "Could not create the HighGrow Seedfile for this seed!", 
                         "Seed Not Exported", MB_OK|MB_ICONEXCLAMATION);
    return FALSE;
    }


