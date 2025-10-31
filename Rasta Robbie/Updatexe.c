#include "windows.h"
#include "stdio.h"

void UpdateExecutableString(HWND hwnd, LPSTR szExeFile)
    {
    HANDLE hResLoad;     // handle to loaded resource 
    HANDLE hExe;        // handle to existing .EXE file 
    HANDLE hRes;         // handle/ptr. to res. info. in hExe 
    HANDLE hUpdateRes;  // update resource handle 
    char *lpResLock;    // pointer to resource data 
    BOOL result; 
 
    // Load the .EXE file that contains the dialog box you want to copy. 
    hExe = LoadLibrary(szExeFile); 
 
    if(!hExe) 
        MessageBox(hwnd, "Could not load exe.", "Error!", MB_OK); 
 
    // Locate the dialog box resource in the .EXE file. 
    hRes = FindResource(hExe, "AboutBox", RT_DIALOG); 
 
    if(!hRes) 
        MessageBox(hwnd, "Could not locate dialog box.", "Error!", MB_OK);  
 
    // Load the dialog box into global memory. 
    hResLoad = LoadResource(hExe, hRes); 
 
    if(!hResLoad) 
        MessageBox(hwnd, "Could not load dialog box.", "Error!", MB_OK); 
 
    // Lock the dialog box into global memory. 
    lpResLock = LockResource(hResLoad); 
 
    if(!lpResLock) 
        MessageBox(hwnd, "Could not lock dialog box.", "Error!", MB_OK);  
 
    // Open the file to which you want to add the dialog box resource. 
    hUpdateRes = BeginUpdateResource(szExeFile, FALSE); 
 
    if(!hUpdateRes) 
        MessageBox(hwnd, "Could not open file for writing.", "Error!", MB_OK); 
 
    // Add the dialog box resource to the update list. 
    result = UpdateResource(hUpdateRes,       // update resource handle 
                            RT_DIALOG,        // change dialog box resource 
                            "AboutBox",       // dialog box name 
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
                            lpResLock,        // ptr to resource info 
                            SizeofResource(hExe, hRes)); // size of resource info. 
 
    if(!result) 
        MessageBox(hwnd, "Could not add resource.", "Error!", MB_OK); 
 
    // Write changes to FOOT.EXE and then close it. 
    if(!EndUpdateResource(hUpdateRes, FALSE)) 
        MessageBox(hwnd, "Could not write changes to file.", "Error!", MB_OK); 
 
    // Clean up. 
    if(!FreeLibrary(hExe)) 
        MessageBox(hwnd, "Could not free executable.", "Error!", MB_OK); 
    }

