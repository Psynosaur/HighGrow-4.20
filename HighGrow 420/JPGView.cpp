// *************************************************
// ********          JPGView.cpp          **********
// *************************************************

#include "StdAfx.h"
#include "Picture.h"

extern "C" { BOOL HGIDrawResourceWindow(HWND hwnd, HINSTANCE hInst, HDC hdc, UINT uiRes); }
extern "C" { BOOL HGIDrawFileWindow(HWND hwnd, HDC hdc, LPCTSTR lpFileName); }

// *******************************************************************
// *** Blowpipe Stuff
// *******************************************************************

#define WM_BLOWPIPE  WM_USER+10000

void Blow(LPCTSTR lpBlowText)
    {
    ATOM atom = 0;
    HWND hwnd = FindWindow("BlowPipe", "BlowPipe Debugger");
    if(hwnd)
        {
        atom = GlobalAddAtom(lpBlowText);
        if(atom)
            {
            SendMessage(hwnd, WM_BLOWPIPE, 0, (LPARAM)atom);
            }
        }
    }


// *******************************************************************
// *** Load & Draw an image file
// *******************************************************************

CPicture JPG_file;

BOOL HGIDrawFileWindow(HWND hwnd, HDC hdc, LPCTSTR lpFileName)
    {
    BOOL bLoad = 0;
    RECT rc;
    CDC  mydc;
    // -----
    GetClientRect(hwnd, &rc);
    // load the JPG image
    bLoad = JPG_file.Load(lpFileName);
    if(bLoad)
        { // if we loaded it, we can render it
        mydc.Attach(hdc);
        JPG_file.Render(&mydc, &rc, NULL);
        JPG_file.Free();
        return TRUE;
        }
    return FALSE;
    }


// *******************************************************************
// *** Load & Draw an image Resource
// *******************************************************************
/*
CPicture JPG_reso;

BOOL HGIDrawResourceWindow(HWND hwnd, HINSTANCE hInst, HDC hdc, UINT uiRes)
    {
    BOOL bLoad;
    RECT rc;
    CDC  mydc;
    // -----
    GetClientRect(hwnd, &rc);
    // load the JPG image
    bLoad = JPG_reso.Load(hInst, uiRes);
    if(bLoad)
        { // if we loaded it, we can render it
        mydc.Attach(hdc);
        JPG_reso.Render(&mydc, &rc, NULL);
        JPG_reso.Free();
        mydc.Detach();
        return TRUE;
        }
    return FALSE;
    }
*/

// *******************************************************************
// *** Loading & Drawing the Growroom JPEG Image Resource
// *******************************************************************

extern "C" { BOOL HGILoadGrowroomResourceImage(HINSTANCE hInst, UINT uiRes); }
extern "C" { BOOL HGILoadGrowroomFileImage(LPCTSTR lpFileName); }
extern "C" { void HGIFreeGrowroomImage(void); }
extern "C" { void HGIRenderGrowroomImage(HDC hdc, RECT rc); }

CPicture JPG_Growroom;
BOOL bGrowroomLoaded = FALSE;


BOOL HGILoadGrowroomResourceImage(HINSTANCE hInst, UINT uiRes)
    {
    if(bGrowroomLoaded) return FALSE;
    bGrowroomLoaded = JPG_Growroom.Load(hInst, uiRes);
    return bGrowroomLoaded;
    }


BOOL HGILoadGrowroomFileImage(LPCTSTR lpFileName)
    {
    if(bGrowroomLoaded) return FALSE;
    bGrowroomLoaded = JPG_Growroom.Load(lpFileName);
    return bGrowroomLoaded;
    }


void HGIFreeGrowroomImage(void)
    {
    if(bGrowroomLoaded)
        {
        JPG_Growroom.Free();
        bGrowroomLoaded = FALSE;
        }
    }


void HGIRenderGrowroomImage(HDC hdc, RECT rc)
    {
    CDC  mydc;
    // ------
    if(bGrowroomLoaded)
        {
        mydc.Attach(hdc);
        JPG_Growroom.Render(&mydc, &rc, NULL);
        mydc.Detach();
        }
    }


// *******************************************************************
// *** Loading & Drawing the Growroom Editor JPEG Image Resource
// *******************************************************************

extern "C" { BOOL HGILoadRoomEditImage(HINSTANCE hInst, UINT uiRes); }
extern "C" { void HGIFreeRoomEditImage(void); }
extern "C" { void HGIRenderRoomEditImage(HDC hdc, RECT rc); }

CPicture JPG_RoomEdit;
BOOL bRoomEditLoaded = FALSE;


BOOL HGILoadRoomEditImage(HINSTANCE hInst, UINT uiRes)
    {
    if(bRoomEditLoaded) return FALSE;
    bRoomEditLoaded = JPG_RoomEdit.Load(hInst, uiRes);
    return bRoomEditLoaded;
    }


void HGIFreeRoomEditImage(void)
    {
    if(bRoomEditLoaded)
        {
        JPG_RoomEdit.Free();
        bRoomEditLoaded = FALSE;
        }
    }


void HGIRenderRoomEditImage(HDC hdc, RECT rc)
    {
    CDC  mydc;
    // ------
    if(bRoomEditLoaded)
        {
        mydc.Attach(hdc);
        JPG_RoomEdit.Render(&mydc, &rc, NULL);
        mydc.Detach();
        }
    }


