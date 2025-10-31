/******************************************************************************\
*
*  PROGRAM:     GrowEdit
*
*  PURPOSE:     Editing the growroom
*
\******************************************************************************/
 
#include <windows.h>
#include "stdio.h"
#include "resource.h"
#include "global.h"
#include "toolbar.h"   // to enable toolbar buttons if they were disabled
#include "dibitmap.h"  // for handling 256 color bitmaps
#include "growroom.h"  // for rendering our growroom after planting
#include "harvest.h"   // for the unavailble dialog
#include "growedit.h"
#include "dibapi.h"
#include "vacation.h"
#include "jpgview.h"
#include "crypt.h"
#include "internet.h"
#include "password.h"
#include "highgrow.h"
       
#define gMaxPots    30  // allow for 30 pots
#define gMaxLights  10  // allow for 10 reflectors

HFONT      hGENameFont;
HFONT      hGETextFont;
HBITMAP    hPotListBitmap;
HPALETTE   hPotListPalette;
HBITMAP    hLightListBitmap;
HPALETTE   hLightListPalette;
// HBITMAP    hPreviewRoomBitmap;
// HPALETTE   hPreviewRoomPalette;

/*****************************************************************************\
*  SOME CONSTANTS
\*****************************************************************************/
    
LPCSTR     lpRoomFile ="GrowRoom.hgg";
OFSTRUCT   RoomOfStruct;  // information from OpenFile()
HGLOBAL    hRoomMem=0;    // handle to our global memory
int        hRoomFile=0;   // currently opened file handle
int        iNumRoomFiles = 0;
PGROWROOM  G_Room;        // pointer to our growroom structure

/*****************************************************************************\
*  ALLOCATING & HANDLING GLOBAL MEMORY
/*****************************************************************************/


void GERoomMemFree(void)
   {
   if(!hRoomMem) return;
   GlobalFree(hRoomMem);
   hRoomMem = 0;
   }

BOOL GERoomMemInit(void)
   {
   GERoomMemFree();
   hRoomMem = GlobalAlloc(GPTR, (DWORD)(sizeof(GR_Room)));
   if(!hRoomMem) return FALSE;
   return TRUE;
   }

/*****************************************************************************\
*  THE MAIN FILE OPEN ROUTINE
\*****************************************************************************/


BOOL GERoomFileOpen(void)
   {
   // Open the file and get its handle
   hRoomFile=OpenFile(lpRoomFile, (LPOFSTRUCT)&RoomOfStruct, OF_READWRITE);
   if(!hRoomFile)  return FALSE;
   return TRUE;
   }

BOOL GERoomFileOpenNew(void)
   {
   // Open the file and get its handle
   hRoomFile=OpenFile(lpRoomFile, (LPOFSTRUCT)&RoomOfStruct, OF_READWRITE|OF_CREATE);
   if(!hRoomFile)  return FALSE;
   return TRUE;
   }

void GERoomFileClose(void)
   {
   if(!hRoomFile)  return;
   _lclose(hRoomFile);
   hRoomFile = 0;
   }


BOOL GERoomFileRead(void)
   {  // file must be open
   LONG IOresult;    // result of a file read
   DWORD dw;

   if(!hRoomFile) return FALSE;
   // Allocate Plant buffer to the size of the file
   dw = GetFileSize((HANDLE)hRoomFile, NULL);

   // lock memory and read file
   G_Room   = (PGROWROOM)GlobalLock(hRoomMem);
   IOresult = _hread(hRoomFile, G_Room, sizeof(GR_Room));

   // # bytes read must equal to file size - header size
   if(IOresult != (LONG)(sizeof(GR_Room)))
       {
       GlobalUnlock(hRoomMem);
       return FALSE;
       }
   GlobalUnlock(hRoomMem);
   return TRUE;
   }


BOOL GERoomFileWrite(void)
    { // file must be open
    LONG IOresult;    // result of a file write
    // lock the memory
    G_Room   = (PGROWROOM)GlobalLock(hRoomMem);
    // and save the Global Plant buffer
    IOresult = _hwrite(hRoomFile, (LPCSTR)G_Room, sizeof(GR_Room));
    // unlock the memory again
    GlobalUnlock(hRoomMem);
    // check the results status
    if(IOresult!=(LONG)(sizeof(GR_Room)))
        return FALSE;
    return TRUE;
    }


// ********************************************************************
// * OPENS & READS PLANTLOG FILE INTO GLOBAL MEMORY
// ********************************************************************

BOOL GERoomFileToMem(void)
   { // opens the growroom log, reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hRoomMem. If fails, we get null.
   BOOL bSuccess = FALSE;
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist((LPSTR)lpRoomFile)) return 0;
   // now set up the memory required
   if(!GERoomMemInit()) return 0;
   // if the file exists, open it
   if(!GERoomFileOpen())   return 0;
   // then read the file into our memory
   bSuccess = GERoomFileRead();
   // free our memory if we failed to read the file
   if(!bSuccess) GERoomMemFree();
   // and close it again
   GERoomFileClose();

   return bSuccess;
   }

// ********************************************************************
// * OPENS & WRITES GROWROOM FILE FROM GLOBAL MEMORY
// ********************************************************************

BOOL GERoomMemToFile(void)
   { // opens the growroom log, reads the contents into global
   // memory. If the function succeeds, we get the handle to our
   // global memory in the variable hRoomMem. If fails, we get null.
   BOOL bSuccess = FALSE;
   // if the file doesn't exist, we do nothing
   if(!GLDoesFileExist((LPSTR)lpRoomFile)) return 0;
   // get out if we don't have global memory required
   if(!hRoomMem)            return 0;
   // if the file exists, open it
   if(!GERoomFileOpen()) return 0;
   // save the global memory into the file
   bSuccess = GERoomFileWrite();
   // free our global memory
   GERoomMemFree();
   // and close the file again
   GERoomFileClose();
   return bSuccess;
   }

/******************************************************************************\
*  THE GROWROOM FILE DATA SAVING ROUTINE
\******************************************************************************/


BOOL GERoomSaveFileDetails(void)
    { 
    int i=0;
    // if the global memory hasn't been created, getout
    if(!hRoomMem) return FALSE;
    // lock the memory first
    G_Room = (PGROWROOM)GlobalLock(hRoomMem);
    // now copy the memory from our struct to our file
    CopyMemory(G_Room, &GR_Room, sizeof(GR_Room));
    // finally unlock our memory again
    GlobalUnlock(hRoomMem);
    return TRUE;
    }

BOOL GERoomLoadFileDetails(void)
    { 
    int i=0;
    // if the global memory hasn't been created, getout
    if(!hRoomMem) return FALSE;
    // lock the memory first
    G_Room = (PGROWROOM)GlobalLock(hRoomMem);
    // now copy the memory from our struct to our file
    CopyMemory(&GR_Room, G_Room, sizeof(GR_Room));
    // finally unlock our memory again
    GlobalUnlock(hRoomMem);
    return TRUE;
    }

BOOL GERoomInitFileDetails(void)
    { 
    int i=0;
    // if the global memory hasn't been created, getout
    if(!hRoomMem) return FALSE;
    // lock the memory first
    G_Room = (PGROWROOM)GlobalLock(hRoomMem);
    // now set default values for all the growrooms
    for(i=0; i<gMaxRooms; i+=1)
        {
        G_Room->szRoomName[0]  = 0;
        G_Room->iLampType      = 0; 
        G_Room->iShadeType     = 0;
        G_Room->iLampStrength  = 3;
        G_Room->iPotType       = 0; 
        G_Room->iSoilType      = 0;
        G_Room->bClockGauge    = TRUE;
        G_Room->bTempGauge     = TRUE;
        G_Room->bHygroGauge    = TRUE;
        G_Room->bZoomGauge     = TRUE;
        G_Room->bVacLights     = TRUE;     
        G_Room->bVacWater      = TRUE;
        G_Room->bVacFert       = TRUE;
        G_Room->iVacLightMM    = 100;
        G_Room->iVacWaterML    = 600;  
        G_Room->iVacWaterDays  = 3;
        G_Room->iVacFertWaters = 3;   
        G_Room->iVacFertN      = 3;   
        G_Room->iVacFertP      = 2;
        G_Room->iVacFertK      = 2;
        G_Room->iVacFertCa     = 0;
        G_Room += 1;
        }
    // finally unlock our memory again
    GlobalUnlock(hRoomMem);
    return TRUE;
    }

/******************************************************************************\
*  THE GROWROOM FILE CREATION ROUTINE (AT FIRST LOAD OF GAME)
\******************************************************************************/

BOOL GECreateNewRoomFile(void)
    { 
    BOOL bCreated = FALSE;
    // now set up the memory required
    if(!GERoomMemInit())     return 0;
    // still going?, create the new plant log file
    if(!GERoomFileOpenNew()) return 0;
    // set the room's info
    if(GERoomInitFileDetails()) 
        {
        // load the file details into our growroom struct array
        GERoomLoadFileDetails();
        // save the global memory into the file
        GERoomFileWrite();
        bCreated = TRUE;
        }
    // close the file again
    GERoomFileClose();
    // and free the memory again
    GERoomMemFree();
    // now set our file to be hidden
    SetFileAttributes(lpRoomFile, FILE_ATTRIBUTE_HIDDEN);
    return bCreated;
    }

/******************************************************************************\
*  THE GROWROOM FILE LOAD AND SAVE ROUTINES
\******************************************************************************/

BOOL GESaveExistRoomFile(void)
    {
    if(!GLDoesFileExist((LPSTR)lpRoomFile)) return 0;
    if(!GERoomFileToMem())                  return 0;
    if(!GERoomSaveFileDetails())           
        {
        // free the memory again
        GERoomMemFree();
        return 0;
        }
    if(!GERoomMemToFile())  return 0;
    return TRUE;
    }

BOOL GELoadExistRoomFile(void)
    {
    if(!GLDoesFileExist((LPSTR)lpRoomFile)) return 0;
    if(!GERoomFileToMem())                  return 0;
    // load the file details into our growroom struct array
    GERoomLoadFileDetails();
    // no save required, just free the memory again
    GERoomMemFree();
    // if we're still here, we return assume success
    return TRUE;
    }

/******************************************************************************\
*  CHECKING IF THE GROWROOM LOGFILE SIZE IS VALID AND DELETE IT IF IT ISN'T
\******************************************************************************/

BOOL GECheckValidRoomFile(void)
    { 
    // checks if the growroom file is the wrong size. 
    // Deletes it and recreates if it is incorrect
    int hFile = 0;
    DWORD dw  = 0;
    OFSTRUCT ofs;
    // ----------
    // open the file into our open file struct
    if((hFile = OpenFile(lpRoomFile, &ofs, OF_READ)) != -1)
        { // check the size of the growroom file
        dw = GetFileSize((HANDLE)hFile, NULL);
        // close the open file again
        _lclose(hFile);
        // check if size is valid for the growroom struct size
        if(dw != sizeof(GROWROOM)*gMaxRooms)
            { // delete the room file now
            DeleteFile(lpRoomFile);
            // now create a new one of the correct size
            return GECreateNewRoomFile();
            }
        return TRUE;
        }
    return FALSE;
    }

/******************************************************************************\
*  THE GROWROOM FILE API FUNCTIONS FOR SAVING ROOM DETAILS AFTER EDITING
\******************************************************************************/


BOOL GESaveRoomDetailsToFile(void)
    {
    if(GLDoesFileExist((LPSTR)lpRoomFile))
        {
        if(GECheckValidRoomFile())
            return GESaveExistRoomFile();
        else 
            return FALSE;
        }
    else
        return GECreateNewRoomFile();
    }


BOOL GELoadRoomDetailsFromFile(void)
    {
    if(GLDoesFileExist((LPSTR)lpRoomFile))
        {
        if(GECheckValidRoomFile())
            return GELoadExistRoomFile();
        else
            return FALSE;
        }
    else
        {
        return GECreateNewRoomFile();
        }
    }


/******************************************************************************\
*  THE GROW ROOM DELETE ROUTINE 
\******************************************************************************/

BOOL GEDeleteGrowRoom(HWND hDlg, HINSTANCE dhInst)
    {
    int  i, j, imax;
    int  iReturn;
    BOOL bReturn;
    char szmsg[200];
    char szFile[MAX_PATH] ="\0";
    char szDest[MAX_PATH] ="\0";
    int  iRoom    = TBGetComboGrowroom()-1;
    int  iRoomOfs = iRoom * 3;
    // ---------    
    wsprintf(szmsg, "%s\n%s\n\n%s", 
            "Are you sure that you want to delete this Grow",
            "Room and all the plants that are growing here?",
            "Note: Your Room Image file will NOT be deleted.");

    iReturn = MessageBox(hDlg, szmsg, "Delete Grow Room", MB_YESNO|MB_ICONQUESTION);
    if(iReturn==IDYES)
        { 
        imax = gMaxRooms-1;
        // first we move everything back by one in the growroom array
        for(i=iRoom;i<(imax);i++)
            {
            strcpy((LPSTR)GR_Room[i].szRoomName, (LPSTR)GR_Room[i+1].szRoomName);
            GR_Room[i].bClockGauge    = GR_Room[i+1].bClockGauge;
            GR_Room[i].bTempGauge     = GR_Room[i+1].bTempGauge;
            GR_Room[i].bHygroGauge    = GR_Room[i+1].bHygroGauge;
            GR_Room[i].bZoomGauge     = GR_Room[i+1].bZoomGauge;
            GR_Room[i].iShadeType     = GR_Room[i+1].iShadeType;
            GR_Room[i].iPotType       = GR_Room[i+1].iPotType;
            GR_Room[i].iLampType      = GR_Room[i+1].iLampType;
            GR_Room[i].iLampStrength  = GR_Room[i+1].iLampStrength;
            GR_Room[i].iSoilType      = GR_Room[i+1].iSoilType;
            GR_Room[i].bVacLights     = GR_Room[i+1].bVacLights;
            GR_Room[i].bVacWater      = GR_Room[i+1].bVacWater;
            GR_Room[i].bVacFert       = GR_Room[i+1].bVacFert;
            GR_Room[i].iVacLightMM    = GR_Room[i+1].iVacLightMM;
            GR_Room[i].iVacWaterML    = GR_Room[i+1].iVacWaterML;
            GR_Room[i].iVacWaterDays  = GR_Room[i+1].iVacWaterDays;
            GR_Room[i].iVacFertWaters = GR_Room[i+1].iVacFertWaters;
            GR_Room[i].iVacFertN      = GR_Room[i+1].iVacFertN;
            GR_Room[i].iVacFertP      = GR_Room[i+1].iVacFertP;
            GR_Room[i].iVacFertK      = GR_Room[i+1].iVacFertK;
            GR_Room[i].iVacFertCa     = GR_Room[i+1].iVacFertCa;
            }
        // now we reset the last entry in the array to our default values
        GR_Room[imax].szRoomName[0]  = 0;
        GR_Room[imax].bClockGauge    = TRUE;
        GR_Room[imax].bTempGauge     = TRUE;
        GR_Room[imax].bHygroGauge    = TRUE;
        GR_Room[imax].bZoomGauge     = TRUE;
        GR_Room[imax].iShadeType     = 0;
        GR_Room[imax].iPotType       = 0;
        GR_Room[imax].iLampType      = 0;
        GR_Room[imax].iLampStrength  = 3;
        GR_Room[imax].iSoilType      = 0;
        GR_Room[imax].bVacLights     = TRUE;
        GR_Room[imax].bVacWater      = TRUE;
        GR_Room[imax].bVacFert       = TRUE;
        GR_Room[imax].iVacLightMM    = 100;
        GR_Room[imax].iVacWaterML    = 600;
        GR_Room[imax].iVacWaterDays  = 3;
        GR_Room[imax].iVacFertWaters = 3;
        GR_Room[imax].iVacFertN      = 3;
        GR_Room[imax].iVacFertP      = 2;
        GR_Room[imax].iVacFertK      = 2;
        GR_Room[imax].iVacFertCa     = 0;
        // now open the growroom details file and save the changes
        bReturn = GESaveRoomDetailsToFile();
        // now we delete and rename our plant files
        if(bReturn)
            { // if we successfully saved our growroom file, we can delete the plants
            for(i=0;i<3;i++)
                {
                wsprintf(szFile, "Plant%02i.hgp", i+1+iRoomOfs);
                if(GLDoesFileExist(szFile))
                    {
                    DeleteFile(szFile);
                    }
                }
            // now we must rename all the plant files from this room onwards
            for(i=iRoom;i<=gMaxPlants;i++)
                {
                wsprintf(szFile, "Plant%02i.hgp", i+3);
                wsprintf(szDest, "Plant%02i.hgp", i);
                if(GLDoesFileExist(szFile))
                    MoveFile(szFile, szDest);
                }
            }
        // first reset our plantseed struct
        for(i=0;i<gMaxPlants;i++)
            {
            PI_PlantSeed[i].szPlantName[0]=0;
            for(j=0 ;j<3; j++)
               PI_PlantSeed[i].szStartDate[j] = 0;
            }
        // now we recalculate all our plants to init everything again
        HGCalculatePlants(hDlg, dhInst, TRUE);
        // now we redisplay the current growroom's combobox contents
        TBResetGrowroomCombo(FALSE);
        // now check if we must draw the next grow room
        GRCheckToRedrawGrowRoom(GetParent(hDlg), dhInst);
        // return our success flag
        return bReturn;
        }
    return FALSE;
    }

/******************************************************************************\
*  HERE WE RETURN THE OFFSET FROM THE PLANTSTARTY TO DRAW EACH PLANT IN THE POT
\******************************************************************************/

DWORD GEGetPotSize(int iPotNumber)
    { // get size (in cubic centimeters) for the given pot (OB0) 
    DWORD dwsize = 0;
    // -----------
    switch (iPotNumber)
        {
        case 0:
            {
            dwsize = (DWORD)((4538.2857 + 3118.5)*60);
            break;
            }
        case 1:
            {
            dwsize = (DWORD)((5947.0714 + 2291.1428)*72);
            break;
            }
        case 2:
            {
            dwsize = (DWORD)((5412.7857 + 2291.1428)*78);
            break;
            }
        case 3:
            {
            dwsize = (DWORD)((5811.1428 + 3740.7857)*68);
            break;
            }
        case 4:
            {
            dwsize = (DWORD)((5676.7857 + 1386)*103);
            break;
            }
        case 5:
            {
            dwsize = (DWORD)((5676.7857 + 4780.2857)*67);
            break;
            }
        case 6:
            {
            dwsize = (DWORD)((5811.1428 + 2124.5714)*94);
            break;
            }
        case 7:
            {
            dwsize = (DWORD)((4780.2857 + 5155.0714)*80);
            break;
            }
        case 8:
            { // rectangular
            dwsize = (DWORD)((5625 + 2116)*57);
            break;
            }
        case 9:
            { // rectangular
            dwsize = (DWORD)((4489 + 4489)*59);
            break;
            }
        case 10:
            { // rectangular
            dwsize = (DWORD)((5476 + 2209)*91);
            break;
            }
        case 11:
            { 
            dwsize = (DWORD)((5676.7857 + 2291.1428)*62);
            break;
            }
        case 12:
            { 
            dwsize = (DWORD)((5283.1428 + 3218.2857)*54);
            break;
            }
        case 13:
            { // rectangular
            dwsize = (DWORD)((4096 + 2500)*57);
            break;
            }
        case 14:
            {
            dwsize = (DWORD)((6650.2857 + 2923.6428)*79);
            break;
            }
        case 15:
            {
            dwsize = (DWORD)((5947.0714 + 1521.1428)*77);
            break;
            }
        case 16:
            {
            dwsize = (DWORD)((4903.6428 + 2464)*74);
            break;
            }
        case 17:
            {
            dwsize = (DWORD)((6223.6428 + 2124.5714)*66);
            break;
            }
        case 18:
            {
            dwsize = (DWORD)((5947.0714 + 3319.6428)*51);
            break;
            }
        case 19:
            {
            dwsize = (DWORD)((6364.2857 + 3319.6428)*73);
            break;
            }
        case 20:
            {
            dwsize = (DWORD)((5544 + 2376.7857)*66);
            break;
            }
        case 21:
            { // rectangular
            dwsize = (DWORD)((8281 + 1936)*81);
            break;
            }
        case 22:
            {
            dwsize = (DWORD)((5544 + 2124.5714)*78);
            break;
            }
        case 23:
            {
            dwsize = (DWORD)((5676.7857 + 2207.0714)*76);
            break;
            }
        case 24:
            {
            dwsize = (DWORD)((6942.5714 + 2376.7857)*66);
            break;
            }
        case 25:
            {
            dwsize = (DWORD)((5544 + 1386)*70);
            break;
            }
        case 26:
            {
            dwsize = (DWORD)((5028.5714 + 5028.5714)*57);
            break;
            }
        case 27:
            {
            dwsize = (DWORD)((6506.5 + 2464)*70);
            break;
            }
        case 28:
            {
            dwsize = (DWORD)((6942.5714 + 1964.2857)*88);
            break;
            }
        case 29:
            {
            dwsize = (DWORD)((4419.6428 + 3740.7857)*63);
            break;
            }
        }
    return dwsize/2;
    }


DWORD GEGetCurRoomPotSize(int iRoom)
    { // get the pot size (in cubic centimeters) for the current growroom 
    return GEGetPotSize(GR_Room[iRoom].iPotType);
    }


/******************************************************************************\
*  FUNCTIONS TO: Changing the text when he changes the listbox
\******************************************************************************/

void GEPotListBoxChange(HWND hdlg, HINSTANCE hInst)
    {
    int   iIndex;
    DWORD dwSize;
    char  szSize[40];
    // first check which pot is currently selected
    iIndex= SendDlgItemMessage(hdlg, IDC_LB02, LB_GETCURSEL, 0,0);
    // display the SIZE text for this pot
    if(iIndex == LB_ERR) return;
    // now get the size of the pots in the current growroom
    dwSize = GEGetPotSize(iIndex);
    sprintf(szSize, "%1.1f dm%c", dwSize/1000.0, 179);
    // now set the text into the static control
    SendDlgItemMessage(hdlg, IDC_ST02, WM_SETTEXT, 0, (LPARAM)szSize);
    }


void GELightListBoxChange(HWND hdlg, HINSTANCE hInst)
    {
    int  iIndex;
    // first check which pot is currently selected
    iIndex= SendDlgItemMessage(hdlg, IDC_LB01, LB_GETCURSEL, 0,0);
    // load and display the NAME text for this seed
    // now set the text into the static controls
//    SendDlgItemMessage(hdlg, IDC_ST01, WM_SETTEXT, 0, (LPARAM)szType);
    }



/******************************************************************************\
*  FUNCTIONS TO: Initialize the dialog before showing it
\******************************************************************************/

void GEFillLightListBox(HWND hDlg)
    {
    int i=0;
    char szNumLights[50]="\0";

    SendDlgItemMessage(hDlg, IDC_LB01, LB_RESETCONTENT, 0, 0L);
    for(i=0; i<gMaxLights; i++)
        SendDlgItemMessage(hDlg, IDC_LB01, LB_ADDSTRING, 0, 0L);
    SendDlgItemMessage(hDlg, IDC_LB01, LB_SETCURSEL, 0, 0L);
    // now change the listbox's groupbox text
    wsprintf(szNumLights, "%i Reflectors", gMaxLights);
    SendDlgItemMessage(hDlg, IDC_ST01, WM_SETTEXT, 0, (LPARAM)szNumLights);
    }


void GEFillPotListBox(HWND hDlg)
    {
    int i=0;
    char szNumPots[50]="\0";

    SendDlgItemMessage(hDlg, IDC_LB02, LB_RESETCONTENT, 0, 0L);
    for(i=0; i<gMaxPots; i++)
        SendDlgItemMessage(hDlg, IDC_LB02, LB_ADDSTRING, 0, 0L);
    SendDlgItemMessage(hDlg, IDC_LB02, LB_SETCURSEL, 0, 0L);
    // now change the listbox's groupbox text
    wsprintf(szNumPots, "%i Pots", gMaxPots);
    SendDlgItemMessage(hDlg, IDC_ST04, WM_SETTEXT, 0, (LPARAM)szNumPots);
    }


void GEInitGrowEditDialog(HWND hDlg, HINSTANCE hInst)
    {
    char szBuff[50] ="\0";
    char szRoom[50] ="\0";
//    char szText[50] ="\0";
    int iSel  = 0;
    int iRoom = TBGetComboGrowroom();
    long lp=MAKELPARAM(TRUE, 0);
    UINT uiRes = IDR_EMPTYROOM;
    // first position that damn thing
    DlgCenter(hDlg);
    // set the caption to reflect which growroom
    TBGetComboGrowroomName(szBuff);
	wsprintf(szRoom, "Edit Grow Room - %s", szBuff);
    SetWindowText(hDlg, szRoom);
    // create our special dialog fonts
    hGENameFont = GLCreateDialogFont(22,0,FW_BOLD);
    hGETextFont = GLCreateDialogFont(17,0,FW_BOLD);
    // if we created the seed name font, set it 
//    if(hGENameFont) 
//        SendDlgItemMessage(hDlg,IDC_ST08,WM_SETFONT,(WPARAM)hGENameFont,lp);
    // if we created the seed text font, set it to all the heading controls
    if(hGETextFont) 
        {
        SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hGETextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST02,WM_SETFONT,(WPARAM)hGETextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST04,WM_SETFONT,(WPARAM)hGETextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST07,WM_SETFONT,(WPARAM)hGETextFont,lp);
        SendDlgItemMessage(hDlg,IDC_ST08,WM_SETFONT,(WPARAM)hGETextFont,lp);
        }
    // load our room bitmap for stretching into the preview window
    if((iRoom <= 3))
        {
        // read the name of the standard image
//        TBGetComboGrowroomName(szRoom);
        // load the standard room image from the dll
        switch (iRoom)
          {
          case 1:
              uiRes = IDR_BEDROOM;
          break;
          case 2:
              uiRes = IDR_BASEMENT;
          break;
          case 3:
              uiRes = IDR_ATTIC;
          break;
          }
        HGILoadRoomEditImage(hInst, uiRes);
        }
    else
        { // else we load our empty room image in case he has deleted all this image file
        HGILoadRoomEditImage(hInst, uiRes);
        }
    // load our light images
    hLightListBitmap = DILoadResourceBitmap(hInst, "Small_Lights", &hLightListPalette);
    // load our pot images
    hPotListBitmap = DILoadResourceBitmap(hInst, "Small_Pots", &hPotListPalette);
    // now fill the list box with dummy strings for our LIGHTS
    GEFillLightListBox(hDlg);
    // now we can show this light's info in the dialog
    GELightListBoxChange(hDlg, hInst);
    // now fill the list box with dummy strings for our POTS
    GEFillPotListBox(hDlg);
    // if this is a standard room, we show the
    if((iRoom > 3))
        {
        // Show our growroom image file combo
        iNumRoomFiles = GLFillGrowFileNamesCombo(hDlg, IDC_CM01);
        // disable the combobox if no images were found
        if(iNumRoomFiles==0)
            {
            strcpy(szBuff, "No Images Found");
            // now display the name of this room
            SendDlgItemMessage(hDlg, IDC_CM01, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)szBuff);
            EnableWindow(GetDlgItem(hDlg, IDC_CM01), FALSE);
            iSel = 0;
            }
        else
            {
            iSel = SendDlgItemMessage(hDlg, IDC_CM01, CB_FINDSTRING, -1, (LPARAM)szBuff);
 	        if(iSel==CB_ERR)  iSel = 0;
            // ****** TEMP CODE ***************************
//            EnableWindow(GetDlgItem(hDlg, IDC_CM01), FALSE);
            // ********************************************
            }
        }
    else 
        {
        SendDlgItemMessage(hDlg, IDC_CM01, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)szBuff);
        EnableWindow(GetDlgItem(hDlg, IDC_CM01), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDC_BT01), FALSE);
        iSel = 0;
        }
    SendDlgItemMessage(hDlg, IDC_CM01, CB_SETCURSEL, iSel, 0L);
    // fill the lamp type combobox
    SendDlgItemMessage(hDlg, IDC_CM02, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Metal Halide");
    SendDlgItemMessage(hDlg, IDC_CM02, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"High Pressure Sodium");
    SendDlgItemMessage(hDlg, IDC_CM02, CB_SETCURSEL, GR_Room[iRoom-1].iLampType, 0L);
    // fill the lamp strength combobox
    SendDlgItemMessage(hDlg, IDC_CM03, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"400 Watts");
    SendDlgItemMessage(hDlg, IDC_CM03, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"600 Watts");
    SendDlgItemMessage(hDlg, IDC_CM03, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"800 Watts");
    SendDlgItemMessage(hDlg, IDC_CM03, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"1000 Watts");
    SendDlgItemMessage(hDlg, IDC_CM03, CB_SETCURSEL, GR_Room[iRoom-1].iLampStrength, 0L);
    // fill the pot size combobox
//    SendDlgItemMessage(hDlg, IDC_CM04, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Not Working Yet");
//    SendDlgItemMessage(hDlg, IDC_CM04, CB_SETCURSEL, 0, 0L);
//    EnableWindow(GetDlgItem(hDlg, IDC_CM04), FALSE);
    // fill the soil type combobox
    SendDlgItemMessage(hDlg, IDC_CM04, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Loamy Soil");
    SendDlgItemMessage(hDlg, IDC_CM04, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Chalky Soil");
    SendDlgItemMessage(hDlg, IDC_CM04, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Peaty Soil");
    SendDlgItemMessage(hDlg, IDC_CM04, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Clay/Silty Soil");
    SendDlgItemMessage(hDlg, IDC_CM04, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"Sandy/Stony Soil");
    SendDlgItemMessage(hDlg, IDC_CM04, CB_SETCURSEL, GR_Room[iRoom-1].iSoilType, 0L);
    // set all our gauge checkboxes
    SendDlgItemMessage(hDlg,IDC_LB01,LB_SETCURSEL,GR_Room[iRoom-1].iShadeType, 0L);
    SendDlgItemMessage(hDlg,IDC_LB02,LB_SETCURSEL,GR_Room[iRoom-1].iPotType, 0L);
    SendDlgItemMessage(hDlg,IDC_CB01,BM_SETCHECK,GR_Room[iRoom-1].bClockGauge,0L);
    SendDlgItemMessage(hDlg,IDC_CB02,BM_SETCHECK,GR_Room[iRoom-1].bTempGauge,0L);
    SendDlgItemMessage(hDlg,IDC_CB03,BM_SETCHECK,GR_Room[iRoom-1].bHygroGauge,0L);
    SendDlgItemMessage(hDlg,IDC_CB04,BM_SETCHECK,GR_Room[iRoom-1].bZoomGauge,0L);
    // now we can show this pot's info in the dialog
    GEPotListBoxChange(hDlg, hInst);
    }


BOOL GESaveGrowEditDialog(HWND hDlg)
    {
    BOOL bReturn = FALSE;
    int  i, iSel, iReturn = 0;
    long lp    = MAKELPARAM(TRUE, 0);
    int  iRoom = TBGetComboGrowroom()-1;
    HWND hCombo = GetDlgItem(hDlg, IDC_CM01);
    char szRoom[50] ="\0";
    char* szfind = 0;
    // -------------
    if(iRoom >= gMaxRooms) return bReturn;
	// read the growroom combo to see which one was selected
	iSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0L);
    // return growroom 1 if error occurred
	if(iSel==CB_ERR)    return bReturn;
	// read the selected combo item's text
	iReturn = SendMessage(hCombo, CB_GETLBTEXT, iSel, (LPARAM)&szRoom);
	// if we failed to read the text, return a false
    if(iReturn==CB_ERR) return bReturn;
    // remove the room number from the combo box text
    szfind = strstr(szRoom, "(");
    if(szfind) szRoom[szfind-szRoom-1] = 0;
    // now we check we already have a room with this name
    for(i=0;i<gMaxRooms;i++)
        {
        if(i != iRoom)
            {
            iReturn = strcmp((LPSTR)GR_Room[i].szRoomName, szRoom);
            if(iReturn==0)
                {
                MessageBox(hDlg, "You already have a Grow Room with this name!", 
                                 "Hey Dude", MB_OK);
                return bReturn;
                }
            }
        }
    // now check if he's saving and has no image file for this room
    iReturn = strcmp("No Images Found", szRoom);
    if(iReturn!=0)
         { // now we can update our growroom image file name struct item
         strcpy((LPSTR)GR_Room[iRoom].szRoomName, szRoom);
         }

    // read all the checkboxes
    GR_Room[iRoom].bClockGauge   = SendDlgItemMessage(hDlg,IDC_CB01,BM_GETCHECK,0,0L);
    GR_Room[iRoom].bTempGauge    = SendDlgItemMessage(hDlg,IDC_CB02,BM_GETCHECK,0,0L);
    GR_Room[iRoom].bHygroGauge   = SendDlgItemMessage(hDlg,IDC_CB03,BM_GETCHECK,0,0L);
    GR_Room[iRoom].bZoomGauge    = SendDlgItemMessage(hDlg,IDC_CB04,BM_GETCHECK,0,0L);
    // read the listboxes
    GR_Room[iRoom].iShadeType    = max(0, SendDlgItemMessage(hDlg,IDC_LB01,LB_GETCURSEL,0,0L));
    GR_Room[iRoom].iPotType      = max(0, SendDlgItemMessage(hDlg,IDC_LB02,LB_GETCURSEL,0,0L));
    // read the lamp info comboboxes
    GR_Room[iRoom].iLampType     = max(0, SendDlgItemMessage(hDlg,IDC_CM02,CB_GETCURSEL,0,0L));
    GR_Room[iRoom].iLampStrength = max(0, SendDlgItemMessage(hDlg,IDC_CM03,CB_GETCURSEL,0,0L));
    // read the soil info combobox
    GR_Room[iRoom].iSoilType     = max(0, SendDlgItemMessage(hDlg,IDC_CM04,CB_GETCURSEL,0,0L));
    // now open the growroom details file and save the changes
    bReturn = GESaveRoomDetailsToFile();
    // finally, we redisplay the current growroom's combobox contents
    TBResetGrowroomCombo(FALSE);
    // return our success flag
    return bReturn;
    }

/******************************************************************************\
*  FUNCTIONS TO: Draw the current growroom image in the status window
\******************************************************************************/
/*
void GEDrawPreviewRoomBmp(HWND hwnd)
     {
     HANDLE hdib = 0;
     // he has no image files, so show the default empty room image
     hdib = BitmapToDIB(hPreviewRoomBitmap, hPreviewRoomPalette);
     if(hdib)
         {
         DIB_DrawWindowDIB(hwnd, hdib);
         DestroyDIB(hdib);
         }
     }
*/

void GEDrawStaticWindow(HWND hDlg, HWND hstat, HDC hdc, int iRoom)
    {
    RECT   rcwin;
    char szpath[MAX_PATH];
    char szfile[MAX_PATH];
    char* szfind = 0;
    int isel  = 0;
    // --------------
    GetClientRect(hstat, &rcwin);
    if(iRoom <= 3)
        {
        HGIRenderRoomEditImage(hdc, rcwin);
        }
    else
        {
        isel = SendDlgItemMessage(hDlg, IDC_CM01, CB_GETCURSEL, 0, 0L);
        if(isel >= 0)
           {
           GetCurrentDirectory(MAX_PATH, (LPTSTR)&szpath);
           strcat(szpath, "\\");
           SendDlgItemMessage(hDlg, IDC_CM01, CB_GETLBTEXT, (WPARAM)isel, (LPARAM)&szfile);
           // remove the room number if we find it as part of the combo text
           szfind = strstr(szfile, "(");
           if(szfind) szfile[szfind-szfile-1] = 0;
           strcat(szpath, szfile);
           strcat(szpath, ".hgb");
           // -- check if the file exists
           if(GLDoesFileExist(szpath))
               { // fill the background with grey 
               char szDFile[MAX_PATH];
               // -----
               // first fill the window with grey
               FillRect(hdc, &rcwin, GetStockObject(GRAY_BRUSH));
               // now setup a name for the destination file
               GLGetTempFilePathAndName(MAX_PATH, szDFile);
               // now try to convert the image file into the jpg format
               if(CRConvertAndDecryptImageFile(szpath, szDFile))
                   { // try to draw the image file
                   if(HGIDrawFileWindow(hstat, hdc, (LPCTSTR)szDFile)==FALSE)
                       { // he has an invalid image file, so show the default empty room image
                       HGIRenderRoomEditImage(hdc, rcwin);
                       }
                   DeleteFile(szDFile);
                   }
               else
                   { // he has an invalid image file, so show the default empty room image
                   HGIRenderRoomEditImage(hdc, rcwin);
                   }
               }
           else
               {
               // he has no image files, so show the default empty room image
               HGIRenderRoomEditImage(hdc, rcwin);
               }
           }
        }
    }


/******************************************************************************\
*  FUNCTIONS TO: Winproc for our Seed List dialog
\******************************************************************************/

BOOL CALLBACK GEGrowEditDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   LPDRAWITEMSTRUCT    lpDIS;
   LPMEASUREITEMSTRUCT lpMIS;
   
    switch (message)
      {
      case WM_INITDIALOG:
           GEInitGrowEditDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
            if(hGENameFont) DeleteObject(hGENameFont);
            if(hGETextFont) DeleteObject(hGETextFont);
            DIFreeBitmap(hLightListBitmap,   hLightListPalette);
            DIFreeBitmap(hPotListBitmap,     hPotListPalette);
//            if(hPreviewRoomBitmap)
//                {
//                DIFreeBitmap(hPreviewRoomBitmap, hPreviewRoomPalette);
//                hPreviewRoomBitmap = 0;
//                }
            // free the JPEG image resource if we loaded one
            HGIFreeRoomEditImage();
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
          if(i==IDC_STROOM)
              {
              int iRoom = TBGetComboGrowroom();
              GEDrawStaticWindow(hDlg, (HWND)lParam, (HDC)wParam, iRoom);
              return (int)GetStockObject(NULL_BRUSH);
              }
          if((i==IDC_CB01)||(i==IDC_CB02)||(i==IDC_CB03)||(i==IDC_CB04)||(i==IDC_ST02))
              return FALSE;
          else
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus
 
      case WM_DRAWITEM:
          {
          lpDIS = (LPDRAWITEMSTRUCT)lParam;

          FillRect(lpDIS->hDC, &lpDIS->rcItem, GetStockObject(WHITE_BRUSH));
          if(wParam==IDC_LB01)
             {
             DIDrawImageEx(lpDIS->hDC, min(lpDIS->itemID, gMaxLights),
                           90, lpDIS->rcItem.left+10,lpDIS->rcItem.top+13, 
                           hLightListBitmap, hLightListPalette);
             }
          else
             {
             if(wParam==IDC_LB02)
                 {
                 DIDrawImageEx(lpDIS->hDC, min(lpDIS->itemID, gMaxPots),
                                64, lpDIS->rcItem.left+12,lpDIS->rcItem.top+2, 
                                hPotListBitmap, hPotListPalette);
                 }
              }
          
          InflateRect(&lpDIS->rcItem, -2, -2);
          if(lpDIS->itemState & ODS_SELECTED)
              FrameRect(lpDIS->hDC, &lpDIS->rcItem, GetStockObject(BLACK_BRUSH));
          if(lpDIS->itemState & ODS_FOCUS)
              DrawFocusRect(lpDIS->hDC, &lpDIS->rcItem);
          }
      return (TRUE);  // only done because we're redirecting the focus

      case WM_MEASUREITEM:
           {
           lpMIS = (LPMEASUREITEMSTRUCT)lParam;
           if(wParam==IDC_LB01)
              {
              lpMIS->itemHeight = 62;
              return(TRUE);
              }
           if(wParam==IDC_LB02)
              {
              lpMIS->itemHeight = 86;
              return(TRUE);
              }
           }
      return (TRUE); 

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_LB02:
               if(HIWORD(wParam)==LBN_SELCHANGE)
                 GEPotListBoxChange(hDlg, dhInst);
            break;

            case IDC_CM01:
                {
                if(HIWORD(wParam)==CBN_SELCHANGE)
                    {
                    InvalidateRect(GetDlgItem(hDlg, IDC_STROOM), NULL, TRUE);
                    }
                }
            break;

            case IDC_BT02:
                {
                int iRoom= SendDlgItemMessage(hDlg, IDC_CM01, CB_GETCURSEL, 0, 0L);
                if(iRoom == CB_ERR) return FALSE;
                VAShowVacationDialog(hDlg, dhInst, iRoom);
                InvalidateRect(GetDlgItem(hDlg, IDC_STROOM), NULL, TRUE);
                }
            break;

            case IDC_BT01:
                { 
                if(GEDeleteGrowRoom(hDlg, dhInst))
                    EndDialog(hDlg,FALSE); 
                }
            break;

            case IDOK: 
                  // save changes to the room file
                  if(GESaveGrowEditDialog(hDlg))
                      EndDialog(hDlg,TRUE); 
            return (TRUE);

            case IDCANCEL: 
                  EndDialog(hDlg,FALSE); 
            return (TRUE);
            }  
      return (FALSE);

      }
   return (FALSE);
   }                          



/******************************************************************************\
*  FUNCTIONS TO: DIALOG FOR ADDING A NEW GROWROOM
\******************************************************************************/

HCURSOR hGELinkCursor = NULL;


void GEInitGrowAddDialog(HWND hDlg, HINSTANCE hInst)
    {
    int iReturn, iSel = 0;
    char szBuff[50] ="\0";
    long lp=MAKELPARAM(TRUE, 0);
    int  iCount = TBGetComboGrowroomCount();
    // first position that damn thing
    DlgCenter(hDlg);
    hGELinkCursor  = LoadCursor(hInst, "linkhand");
    // create our special dialog fonts
    hGETextFont = GLCreateDialogFont(15,0,FW_BOLD);
    SendDlgItemMessage(hDlg,IDC_ST01,WM_SETFONT,(WPARAM)hGETextFont,lp);
    SendDlgItemMessage(hDlg,IDC_ST03,WM_SETFONT,(WPARAM)hGETextFont,lp);
    SendDlgItemMessage(hDlg,IDC_ST04,WM_SETFONT,(WPARAM)hGETextFont,lp);
    SendDlgItemMessage(hDlg,IDC_ST05,WM_SETFONT,(WPARAM)hGETextFont,lp);
    // show number of rooms
    if(iCount < gMaxRooms)
        {
        wsprintf(szBuff, "Grow Room Number:  %i", iCount+1);
        }
    else
        { // if he's reached the maximum number of rooms, disable the OK button
        EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
        strcpy(szBuff, "No More Grow Rooms");
        }
    // show the next room number or disable the Add Room button
    SendDlgItemMessage(hDlg,IDC_ST04,WM_SETTEXT, 0, (LPARAM)szBuff);
    // load our empty room image in case he has deleted all image files
//    hPreviewRoomBitmap = DILoadResourceBitmap(hInst, "emptyroom", &hPreviewRoomPalette);
    HGILoadRoomEditImage(hInst, IDR_EMPTYROOM);
    // Show our standard growroom names
    iNumRoomFiles = GLFillGrowFileNamesCombo(hDlg, IDC_CM01);
    // disable the combobox if no images were found
    if(iNumRoomFiles==0)
        {
        SendDlgItemMessage(hDlg, IDC_CM01, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)"No Images Found");
        EnableWindow(GetDlgItem(hDlg, IDC_CM01), FALSE);
        EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
        }
    else 
        {
        iReturn = SendDlgItemMessage(hDlg, IDC_CM01, CB_FINDSTRING, (WPARAM)-1, (LPARAM)"Empty Room");
        if(iReturn!=CB_ERR) 
            iSel = iReturn;
        }
    SendDlgItemMessage(hDlg, IDC_CM01, CB_SETCURSEL, iSel, 0L);
    }


BOOL GESaveGrowAddDialog(HWND hDlg, HINSTANCE hInst)
    {
    BOOL bReturn = FALSE;
    int  i, iSel, iReturn = 0;
    char* szfind = 0;
    char szRoom[50] ="\0";
    HWND hCombo = GetDlgItem(hDlg, IDC_CM01);
    int  iCount = TBGetComboGrowroomCount();
    // ------------------------------------
    // return growroom 1 if error occurred
	if(iCount==0)        return bReturn;
    // getout if we somehow don't have a growroom name combo
    if(hCombo==0)        return bReturn;
    // do nothing if we don't have any images to use
    if(iNumRoomFiles == 0) 
        {
        HAUnavailableDialog(hDlg, hInst);
        return  FALSE;
        }
	// read the growroom combo to see which one was selected
	iSel = SendMessage(hCombo, CB_GETCURSEL, 0, 0L);
    // return growroom 1 if error occurred
	if(iSel==CB_ERR)    return  bReturn;
	// read the selected combo item's text
	iReturn = SendMessage(hCombo, CB_GETLBTEXT, iSel, (LPARAM)&szRoom);
	// if we failed to read the text, return a false
    if(iReturn==CB_ERR) return  bReturn;
    // remove the room number from the combo box text
    szfind = strstr(szRoom, "(");
    if(szfind)  szRoom[szfind-szRoom-1] = 0;
    // now we check we already have a room with this name
    for(i=0;i<gMaxRooms;i++)
        {
        iReturn = strcmp((LPSTR)GR_Room[i].szRoomName, szRoom);
        if(iReturn==0)
            {
            MessageBox(hDlg, "You already have a Grow Room with this name!", 
                             "Hey Dude", MB_OK);
            return FALSE;
            }
        }
    // now we can update our growroom image file name struct item
    strcpy((LPSTR)GR_Room[iCount].szRoomName, szRoom);
    // now open the growroom details file and save the changes
    bReturn = GESaveRoomDetailsToFile();
    // now we redisplay the new growroom combobox contents
    TBResetGrowroomCombo(TRUE);
    // here we check to update the growroom window
    GRCheckToRedrawGrowRoom(GetParent(hDlg), hInst);
    // return our success flag
    return bReturn;
    }


BOOL CALLBACK GEGrowAddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   const HWND hParent     = GetParent(hDlg);
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
   const HCURSOR hArrow   = LoadCursor((HINSTANCE)NULL, IDC_ARROW);
   static BOOL bMouseDown = FALSE;  
   
    switch (message)
      {
      case WM_INITDIALOG:
           GEInitGrowAddDialog(hDlg, dhInst);
      return (TRUE);

      case WM_DESTROY:
            if(hGETextFont) DeleteObject(hGETextFont);
            if(hGELinkCursor) DestroyCursor(hGELinkCursor);
//            DIFreeBitmap(hPreviewRoomBitmap, hPreviewRoomPalette);
            // free the JPEG image resource if we loaded one
            HGIFreeRoomEditImage();
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
//          if((i==IDC_STROOM)&&(hPreviewRoomBitmap!=0))
          if(i==IDC_STROOM)
              {
              GEDrawStaticWindow(hDlg, (HWND)lParam, (HDC)wParam, 4);
              return (int)GetStockObject(NULL_BRUSH);
              }

          if(i==IDC_ST02)
              {
              if(bMouseDown)
                   SetTextColor((HDC)wParam, RGB(255,0,0));
              else SetTextColor((HDC)wParam, RGB(0,0,255));
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }

          if((i==IDC_CB01)||(i==IDC_CB02)||(i==IDC_CB03)||(i==IDC_CB04))
              return FALSE;
          else
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);  // only done because we're redirecting the focus

      case WM_MOUSEMOVE:
           {
           RECT  rc;
           POINT pt;
           GetWindowRect(GetDlgItem(hDlg, IDC_ST02), &rc);
           pt.x = LOWORD(lParam);
           pt.y = HIWORD(lParam);
           ClientToScreen(hDlg, &pt);
           if(PtInRect(&rc, pt))
                SetClassLong(hDlg, GCL_HCURSOR, (LONG)hGELinkCursor);
           else SetClassLong(hDlg, GCL_HCURSOR, (LONG)hArrow);
           }
      return (FALSE);

      case WM_LBUTTONDOWN:
          { 
           char    szURL[MAX_PATH]="\0";
           HCURSOR hCurCursor = (HCURSOR)GetClassLong(hDlg, GCL_HCURSOR);
           HWND    hStatic    = GetDlgItem(hDlg, IDC_ST02);
           if(hCurCursor == hGELinkCursor)
               {  
               // BETA 1 - http://www.rsrclan.com/highgrow/rooms.php
               // BETA 2 - http://www.highgrow.us/newer/rooms.php
               // FINAL  - http://www.highgrow.us/rooms.php
               // ********** 
//               strcpy(szURL, "http://www.highgrow.us/newer/rooms.php");
//               GLScrambleText((LPSTR)&szURL);
//               Blow(szURL);
//               strcpy(szURL,"www.highgrow.us/newer/picserv/");
//               GLScrambleText((LPSTR)&szURL);
//               Blow(szURL);
               // ********** 
               strcpy(szURL, glpHighRoomServer);
               GLUnscrambleText((LPSTR)&szURL);
               // now show that he's clicked the link
               bMouseDown = TRUE;
               InvalidateRect(hStatic, NULL, FALSE);
               UpdateWindow(hStatic);
               ShellExecute(hDlg, "open", szURL, NULL, NULL, SW_SHOWDEFAULT);
               bMouseDown = FALSE;
               InvalidateRect(GetDlgItem(hDlg, IDC_ST05), NULL, FALSE);
               UpdateWindow(hStatic);
               // now we close our dialog
               EndDialog(hDlg,FALSE); 
//               // show the beta tester's download dialog
//               if(INShowDownloadDialog(hDlg, dhInst))
//                   { // if we downloaded an image file, we refresh our list of images
//                   GLFillGrowFileNamesCombo(hDlg, IDC_CM01);
//                   InvalidateRect(GetDlgItem(hDlg, IDC_STROOM), NULL, TRUE);
//                   }
               }
           }
      return (FALSE);

   
      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_CM01:
                {
                if(HIWORD(wParam)==CBN_SELCHANGE)
                    {
                    InvalidateRect(GetDlgItem(hDlg, IDC_STROOM), NULL, TRUE);
                    }
                }
            break;

            case IDOK: 
                  // save changes to the room file
                  if(GESaveGrowAddDialog(hDlg, dhInst))
                      EndDialog(hDlg,TRUE); 
            return (TRUE);

            case IDCANCEL: 
                  EndDialog(hDlg,FALSE); 
            return (TRUE);
            }  
      return (FALSE);

      }
   return (FALSE);
   }                          

/******************************************************************************\
*  EXPOSED FUNCTIONS TO START THE ADDING/EDITING A NEW GROWROOM DIALOGS
\******************************************************************************/

BOOL GEStartAddGrowRoomDialog(HWND hwnd, HINSTANCE hInst)
    { 
    return DialogBox(hInst, "RoomAdd", hwnd, GEGrowAddDlgProc);
    }

BOOL GEStartEditGrowRoomDialog(HWND hwnd, HINSTANCE hInst)
    { 
    BOOL bReturn = FALSE;
    // ---------
    // display the room editor dialog box
    bReturn = DialogBox(hInst, "RoomEdit", hwnd, GEGrowEditDlgProc);
    // if he's in the growroom, redraw showing changes
    if((bReturn)&&(hGrowRoomWindow))
        { // recalc plant and light origins and redraw plants
        int iRoom = TBGetComboGrowroom();
        // ---
        GRInitGrowRoomDrawPlants();
        GRReloadGrowroomImage(hwnd, hInst, iRoom);
        GRReRenderGrowRoom();
        GRRecreateGrowroomGauges(hInst, iRoom);
        }
    // if he pressed the Add New Room button
//    if(bReturn)
//        { // start the add new room dialog box
//        GEStartAddGrowRoomDialog(hwnd, hInst);
//        // when he's added a new room, we restart this routine
//        bReturn = GEStartEditGrowRoomDialog(hwnd, hInst);
//        }
    return bReturn;
    }

/******************************************************************************\
*  CAN BE CALLED TO DELETE THE CURRENTLY SELECTED ROOM NAME IN THE ROOM FILE
\******************************************************************************/

BOOL GEDeleteRoom(void)
    {
    int iRoom = TBGetComboGrowroom()-1;
    GR_Room[iRoom].szRoomName[0] = 0;
    return GESaveRoomDetailsToFile();
    }
