// -----------------------------------------------------
//  CCSCODE.c
// -----------------------------------------------------

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mapi.h>
#include <htmlhelp.h>
#include "vcombobx.h"


// ---------------------------------------------------
// --- displays a help screen for a HTML help file ---
// ---------------------------------------------------

HWND CCSHelpContext(HWND hparent, LPCSTR lpHelpFile, int iContext)
    {
    return HtmlHelp(hparent, lpHelpFile, HH_HELP_CONTEXT, iContext);
    }

   
// -------------------------------
// --- RANDOM NUMBER GENERATOR ---
// -------------------------------

int RandSeed = 0;

int _stdcall CRandom(void)
	{
	if(RandSeed==0)
		srand((unsigned)time(NULL));

	RandSeed = rand();

	return RandSeed;
	}


  
// --------------------------------
// --- VIRTUAL COMBOBOX CONTROL ---
// --------------------------------

HWND _stdcall CreateVirtualCombobox(HWND hParent, UINT nIDCombo, DWORD dwStyle, UINT x, UINT y, UINT w, UINT h, UINT cItems)
    {
    HWND hWndVCombo;
    HINSTANCE hInst = (HINSTANCE)GetWindowLong(hParent, GWL_HINSTANCE);
    
    if(InitVirtualComboBox(hInst))
        {
    	hWndVCombo = CreateWindowEx(0,WC_VCOMBOBOX,NULL,dwStyle,
	    						    x,y,w,h,hParent,(HMENU)nIDCombo,hInst,NULL);

	    if(NULL == hWndVCombo)	return FALSE;

	    // Specify the count of items in the combo box.
	    // We use the message cracker macro for VCBM_SETCOUNT.
	    VirtualComboBox_SetCount(hWndVCombo, cItems);

	    return hWndVCombo;
        }
    else 
        return FALSE;
    }
    
