/******************************************************************************\
*  VERSINFO.C
\******************************************************************************/

#include <windows.h>    // includes basic windows functionality
#include <commctrl.h>   // includes the common control header
#include <shlwapi.h>

DWORD dwVersMajor=0;
DWORD dwVersMinor=0;


/******************************************************************************\
*  CHECKS THE VERSION OF THE COMMON CONTROLS DLL
\******************************************************************************/

// DLL Versions
// ------------
// Because of ongoing enhancements, different versions of Comctl32.dll and shell32.dll 
// implement different features. Throughout this document, programming elements are 
// marked with a version number. This version number indicates that the programming element 
// is implemented in that version and later versions of the DLLs. If no version number is 
// specified, the programming element is implemented in all versions. 

// The following table outlines the different DLL versions, and the distribution platform. 
    
// Version Distribution platform 
// 4.00 Microsoft® Windows® 95/Windows NT® 4.0 
// 4.70 Microsoft® Internet Explorer 3.x 
// 4.71 Microsoft® Internet Explorer 4.0 
// 4.72 Microsoft® Internet Explorer 4.01 
// 5.00 Microsoft® Windows NT 5.0 

// Version Description 
// 0x0200 
//   The application will be compatible with Comctl32.dll and shell32.dll 
//   version 4.00 and later. The application will not be able to implement features 
//   that were added after version 4.00 of Comctl32.dll. 

// 0x0300 
//   The application will be compatible with Comctl32.dll and shell32.dll 
//   version 4.70 and later. The application will not be able to implement features 
//   that were added after version 4.70 of Comctl32.dll. 

// 0x0400 
//   The application will be compatible with Comctl32.dll and shell32.dll 
//   version 4.71 and later. The application will not be able to implement features 
//   that were added after version 4.71 of Comctl32.dll. 

// 0x0401 
//   The application will be compatible with Comctl32.dll and shell32.dll 
//   version 4.72 and later. The application will not be able to implement features 
//   that were added after version 4.72 of Comctl32.dll. 

// 0x0500 
//   The application will be compatible with Comctl32.dll and shell32.dll 
//   version 5.0 and later. The application will not be able to implement features 
//   that were added after version 5.00 of Comctl32.dll. 

BOOL GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
    {
    HRESULT   hr;
    BOOL      bResult=TRUE; 
    HINSTANCE hComCtl;
    if(IsBadWritePtr(pdwMajor, sizeof(DWORD)) || 
       IsBadWritePtr(pdwMinor, sizeof(DWORD)))   
          return FALSE;
    hComCtl = LoadLibrary(TEXT("comctl32.dll"));
    if(hComCtl)   
        {
        DLLGETVERSIONPROC pDllGetVersion;      
        
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hComCtl, TEXT("DllGetVersion"));
        if(pDllGetVersion)      
            {      
            DLLVERSIONINFO  dvi;      
            ZeroMemory(&dvi, sizeof(dvi));      
            dvi.cbSize = sizeof(dvi);   
            hr = (*pDllGetVersion)(&dvi);            
            if(SUCCEEDED(hr))         
                {
                *pdwMajor = dvi.dwMajorVersion;         
                *pdwMinor = dvi.dwMinorVersion;
                }      
            else bResult = FALSE;         
            }   
        else bResult = FALSE;         
        }
    FreeLibrary(hComCtl);   
    return bResult;   
    }



HRESULT QueryReadVersion(void)
    {
    if(dwVersMajor==0) // if version number variable not filled, fill it
       return GetComCtlVersion(&dwVersMajor, &dwVersMinor);
    return E_FAIL;
    }

/******************************************************************************\
*  CHECKS VARIOUS VERSION RELATED CALLS
\******************************************************************************/

BOOL VICoolBarsEnabled(void)
    {
    if(!QueryReadVersion()) return FALSE;
    if(dwVersMajor>=5)      return TRUE;
    if(dwVersMinor>=70)     return TRUE;
    return FALSE;
    }
