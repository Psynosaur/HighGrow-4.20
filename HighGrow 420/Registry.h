/******************************************************************************\
*
*  Registry.h - Saving, reading keys and other Slick Software registry functions
*
\******************************************************************************/

void RESaveRegistryKey(LPCTSTR lpKeyName, LPCTSTR lpKeyValue);
void REReadRegistryKey(LPCTSTR lpKeyName, LPCTSTR lpValueBuff, int iBuffLen);
void RESetProgramAutoLoad(BOOL bSetKey);
