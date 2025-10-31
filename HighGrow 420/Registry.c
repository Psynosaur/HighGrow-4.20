/******************************************************************************\
*
*  Registry.c - Saving, reading keys and other Slick Software registry functions
*
\******************************************************************************/

#include <windows.h>
#include <winreg.h>


/******************************************************************************\
*  Saving the given registry key value, creating it if it doesn't exist
\******************************************************************************/
        
void RESaveRegistryKey(LPCTSTR lpKeyName, LPCTSTR lpKeyValue)
     { 
     // saves the given key value for the given key name,
     // for any Slick Software program's current user.
     DWORD dwDisp;
     HKEY  hkMain;
     HKEY  hkAuthor;
     HKEY  hkProgram;
     // first open the current user's key and create/open the slick software key
     RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_ALL_ACCESS, &hkMain);
     RegCreateKeyEx(hkMain, "Slick Software", 0, "", REG_OPTION_NON_VOLATILE, 
                    KEY_ALL_ACCESS, NULL, &hkAuthor, &dwDisp);
     RegCreateKeyEx(hkAuthor, "HighGrow", 0, "", REG_OPTION_NON_VOLATILE, 
                    KEY_ALL_ACCESS, NULL, &hkProgram, &dwDisp);
     RegSetValueEx(hkProgram, lpKeyName, 0, REG_SZ, 
                    (CONST BYTE*)lpKeyValue, lstrlen(lpKeyValue)+1);
     // close all the registry keys again
     RegCloseKey(hkProgram);     
     RegCloseKey(hkAuthor);     
     RegCloseKey(hkMain);
     }

/******************************************************************************\
*  Reading key values for the given key name, creating keys if they don't exist
\******************************************************************************/

void REReadRegistryKey(LPCTSTR lpKeyName, LPTSTR lpValueBuff, int iBuffLen)
     { 
     // reads the given key name's value into the given buffer of given size
     // for any Slick Software program's current user.
     HKEY  hkMain;
     HKEY  hkAuthor;
     HKEY  hkProgram;
     long  lValue = (long)iBuffLen;
     PLONG pValueLen = &lValue;
     lpValueBuff[0]  = 0; // zero this incase nothing is read
     // first open the current user's key and create/open the slick software key
     RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_QUERY_VALUE, &hkMain);
     RegOpenKeyEx(hkMain, "Slick Software", 0, KEY_QUERY_VALUE, &hkAuthor);
     RegOpenKeyEx(hkAuthor, "HighGrow", 0, KEY_QUERY_VALUE, &hkProgram);
     RegQueryValueEx(hkProgram, lpKeyName, 0, (LPDWORD)NULL, lpValueBuff, pValueLen);
     // close all the registry keys again
     RegCloseKey(hkProgram);     
     RegCloseKey(hkAuthor);     
     RegCloseKey(hkMain);
     }


/******************************************************************************\
*  Setting or Deleting the program Auto-Load on Windows startup Registry entry
\******************************************************************************/

void RESetProgramAutoLoad(BOOL bSetKey)
     { // sets or deletes the HighGrow auto-load registry entry
     // HKEY_CURRENT_USER/Software/Microsoft/Windows/CurrentVersion/Run/HighGrow
     HKEY  hkMain;
     HKEY  hkKey1, hkKey2, hkKey3, hkKey4;
     char szCommandLine[MAX_PATH]="\0";
     // set up the command line string
     strcpy(szCommandLine, GetCommandLine());
     strcat(szCommandLine, " /A"); // adds the autoload switch
     // first open the current user's key and create/open our special key
     RegOpenKeyEx(HKEY_CURRENT_USER, "Software", 0, KEY_QUERY_VALUE, &hkMain);
     RegOpenKeyEx(hkMain, "Microsoft",      0, KEY_QUERY_VALUE, &hkKey1);
     RegOpenKeyEx(hkKey1, "Windows",        0, KEY_QUERY_VALUE, &hkKey2);
     RegOpenKeyEx(hkKey2, "CurrentVersion", 0, KEY_QUERY_VALUE, &hkKey3);
     RegOpenKeyEx(hkKey3, "Run",            0, KEY_ALL_ACCESS, &hkKey4);
     // here we set our program path and AutoLoad parameter switch
     if(bSetKey)
         {
//         Blow(szCommandLine);
         RegSetValueEx(hkKey4, "HighGrow", 0, REG_SZ, 
                      (CONST BYTE*)szCommandLine, lstrlen(szCommandLine)+1);
         }
     else // if we're unsetting it, we must delete it
         RegDeleteValue(hkKey4, "HighGrow");
     // close all the registry keys again
     RegCloseKey(hkKey4);     
     RegCloseKey(hkKey3);     
     RegCloseKey(hkKey2);     
     RegCloseKey(hkKey1);     
     RegCloseKey(hkMain);
     }

