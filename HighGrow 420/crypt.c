//**************************************************************************
// Program Crypt.c
// Homepage: http://chasbro.net
// Email: chasbro@nl.rogers.com
// Date:  July 22,2004
// Compile gcc crypt.c -o crypt.exe
// Exec:   crypt <password> <infile> ....
//
// Purpose:
//
//    The purpose of this program is to provide a minimal amount of
// encryption for a FILE. The program takes a password and a list of files
// to encrypt or decrypt. The program does not know if the file is crypted
// or not it simply applies the password to the file to encrypt or decrypt it.
// WARNING: IF YOU FORGET THE PASSWORD THE FILE COULD BE UNREVCOVERABLE. THE
// SAME PASSWORD MUST BE USED TO CRYPT AND DECRYPT THE FILE. IF MULTIPLE 
// ENCRYPTION IS APPLIED TO A FILE THE SAME NUMBER OF DECRYPTIONS MUST BE 
// APPLIED TO THE FILE TO OBTAIN THE ORIGINAL FILE.
//**************************************************************************

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include "crypt.h"    // for image file header struct
#include "jpgview.h"  // for loading jpeg image files
#include "global.h"   // for blowpipe

LPCSTR lpEncryptPassword ="Hey Dude";

//************************************************************************
//   The purpose of this function is to encode/ decode a chunk of a file.
// The parameters are as follows:
//    buff   --- the input ddata to be encoded/decoded
//    blen   --- the number of bytes in the buffer which contains data
//    pass   --- the password used to encode/decode the data
//    plen   --- the length of the password string
//    p      --- character position to be used to encode the next character 
//
//  Basically the algorithn works as follows:
//
// The function takes a buffer containing a number of characters and 
// encoded the characters by xoring the character in the buffer with a 
// character in the password string. The character used in the password 
// string is recycled ie. it starts a position p in the password string 
// and continues using characters until all the characters in the password
// string have been used and it then starts at the first charactwer in the
// password string again. The function remembber through the index p the
// position of the last character of the password used so the next character
// can be used the next time.
//**************************************************************************


void CREncodeBuffer(char *buff,long blen, char *pass,long plen, long *p)
    {
    long i;
    // ----
    // process all character in the buffer 
    for ( i = 0; i < blen; i++) 
        {
        // xor the buffer character with a character from the password string
        buff[i] = buff[i] ^ pass[*p]; 
        // determine the next character in the password string to be used; wrap is necessary
        *p = (*p + 1) % plen;  
        }
    }


// ***********************************************************************
// ** Function to encode/decode a given file with a given password
// ***********************************************************************
/*
const long  TMPBUFFSIZE=4096; // size 4k buffer to hold the input data and to encode/decode 
char tmpbuffer[4096];         // input and decoding buffer must be TMPbuffersize 

BOOL CREncodeFile(LPSTR lpFile, LPSTR lpPassword)
    {
    FILE *infil;  // HANDLE TO INPUT FILE 
    long ppos,ifilepos,num,plen;
    // ------
    // length of the password string 
    plen = strlen(lpPassword); 
    // process all file to be encoded/decoded
    if((infil = fopen(lpFile,"rb+")) == NULL) 
        return FALSE;
    // start at the beginning of the password string
    ppos = 0; 
    // start at the beginning of the file
    ifilepos = 0; 
    // read data from the file until there is no more 
    while((num = fread(tmpbuffer,sizeof(char),TMPBUFFSIZE,infil)) != 0) 
          {
          // encode/decode a chuck of the file
          CREncodeBuffer(tmpbuffer,num,lpPassword,plen,&ppos); 
          // seek to where the data was read
          fseek(infil,ifilepos,SEEK_SET); 
          // write the coded/decode data back to the file
          fwrite(tmpbuffer,sizeof(char),num,infil); 
          // remember where we are in the file
          ifilepos= ftell(infil); 
          // seek to where we are. You must issue a seek between a read and write operation
          fseek(infil,ifilepos,SEEK_SET); 
          }
    // close the coded/decoded file
    fclose(infil); 
    return TRUE;
    }      
*/
// ***************************************************************************************
// ** Function to encrypt a given JPG file with a given password into the HGB image format
// ***************************************************************************************

DWORD CRGetDiskDriveIDNumber(void)
    {
    DWORD dwSerial = 0;
    LPDWORD lpDW   = &dwSerial;
    char szVolumeBuf[MAX_PATH]="\0";
    // ---
    // here we get some information unique to this drive
    GetVolumeInformation(NULL, (LPTSTR)&szVolumeBuf, MAX_PATH, lpDW, 
                        (LPDWORD)NULL, (LPDWORD)NULL, (LPTSTR)NULL, 0);
    // return the value in our double
    return dwSerial;
    }

UINT CRWritePrivateHeader(int iFileHandle, DWORD dwFileSize, BOOL bSecure)
    {
    ROOMIMAGEFILEHEADERSTRUCT ri;
    // --- Prepare the file header info
    strcpy(ri.szFileDesc, "HighGrow Growroom Image File");
    strcpy(ri.szFileBluff, "BMP");
    ri.dwBitmapSize = dwFileSize;
    // if bSecure flag is set, we include the serial number in the header
    if(bSecure == TRUE)
        ri.dwRegCode = CRGetDiskDriveIDNumber();
    else // else write the public (distributable) key into the header
        ri.dwRegCode = 178648763;
    // --- Write the file header   
    return _lwrite(iFileHandle, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADERSTRUCT));
    }


BOOL CRConvertAndEncryptImageFile(LPSTR lpSourceFile, LPSTR lpRoomName, BOOL bSecure)
    {
    int hSFile, hDFile;
    HANDLE hMem = 0;
    LPVOID lpv;
    OFSTRUCT Sofs, Dofs;
    BOOL bReturn = FALSE;
    DWORD dwsize = 0;
    LONG ppos = 0;
    char* szfind = 0;
    char szDestFile[MAX_PATH];
    int iLen = strlen(lpSourceFile)-3;
    int  ch = '\\';
    // -----
    // set the cursor to an hourglass in case this takes some time
    SetCursor(LoadCursor(NULL, IDC_WAIT));
    // first we open the source file
    if((hSFile = OpenFile(lpSourceFile, &Sofs, OF_READ)) != -1)
        { // now we get the size of this file
        dwsize = (DWORD)(GetFileSize((HANDLE)hSFile, NULL));
        // create the destination file name
        strcpy(szDestFile, lpSourceFile);
        if(strlen(lpRoomName))
            {
            // find the pointer to the backslash character
            szfind = strrchr(szDestFile, ch);
            if(szfind)
                { // chop the source file name from the destination name
                szDestFile[szfind-szDestFile+1] = 0;
                }
            // now add the room name to the destination name
            strcat(szDestFile, lpRoomName);
            // now add our file extension to the destination name
            strcat(szDestFile, ".hgb");
            }
        else
            {
            strcpy(szDestFile+iLen, "hgb");
            }
//        Blow(szDestFile);
        // now delete any existing destination file
        DeleteFile(szDestFile);
        // open a new destination file for reading and writing
        if((hDFile = OpenFile(szDestFile, &Dofs, OF_CREATE | OF_READWRITE)) != -1)
            { // Allocate enough memory for reading and encrypting the file into
            hMem = GlobalAlloc(GMEM_MOVEABLE, dwsize);
            if(hMem)
                { // if we allocated the memory, lock it now
                lpv = (LPVOID)GlobalLock(hMem);
                if(lpv) 
                    { // if we locked the memory, we write our header to the new file
                    // if bSecure flag is set, we include the serial number in the header
                    CRWritePrivateHeader(hDFile, dwsize, bSecure);
                    // now we read the source file into the memory
                    if(dwsize == _lread(hSFile, (LPSTR)lpv, dwsize));
                        { 
                        // now encrypt the source file data in the buffer
                        CREncodeBuffer((LPSTR)lpv, (LONG)dwsize, 
                                       (LPSTR)lpEncryptPassword, strlen(lpEncryptPassword), &ppos); 
                        // once we encrypted our file data, we can write it to the destination file
                        if(dwsize == _lwrite(hDFile, (LPSTR)lpv, dwsize));
                            { // YIPPEE - if we're still here, then everything was successful!
                            bReturn = TRUE;
                            }
                        }
                    // now we can unlock our global memory again
                    GlobalUnlock(hMem);    
                    }
                // now we can free our global memory again
                GlobalFree(hMem);
                }
            // here we close the destination file
            _lclose(hDFile);
            }
        // and here we close the source file
        _lclose(hSFile);
        }
    // now we can reset our standard arrow cursor again
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    // if we converted everything successfully, our flag should be true
    return bReturn;
    }


// ***************************************************************************************
// ** Function to decrypt a given HGB file with a given password into the JPG image format
// ***************************************************************************************


BOOL CRUpgradePrivateFileHeader(int hFile, DWORD dwsize)
    { // this checks to see if the room image file header is valid
    // *** UPGRADE THE FILE FOR THIS PROCESSOR ONLY ***
    ROOMIMAGEFILEHEADERSTRUCT ri;
    // seek to the beginning of the file
    _llseek(hFile, 0, SEEK_SET);
    // --- Read the file header into the struct
    _lread(hFile, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADERSTRUCT));
    // --- Prepare the file header info
    if(strcmp(ri.szFileDesc, "HighGrow Growroom Image File") != 0)
        return FALSE;
    if(strcmp(ri.szFileBluff, "BMP")  != 0)
        return FALSE;
    if(ri.dwBitmapSize != dwsize)
        return FALSE;
    if(ri.dwRegCode != 178648763)
        return FALSE;
    // now set the serial number key into our struct
    ri.dwRegCode = CRGetDiskDriveIDNumber();
    // seek to the beginning of the file
    _llseek(hFile, 0, SEEK_SET);
    // --- Write the file header   
    _lwrite(hFile, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADERSTRUCT));
    // --- we passed everything, so close the file and return true
    return TRUE;
    }



BOOL CRCheckPrivateHeader(int iFileHandle, DWORD dwsize, BOOL bSecure)
    { // this checks to see if the room image file header is valid
    // *** IF bSecure IS TRUE, THEN CHECK FOR THIS PROCESSOR ONLY ***
    ROOMIMAGEFILEHEADERSTRUCT ri;
    // seek to the beginning of the file
    _llseek(iFileHandle, 0, SEEK_SET);
    // --- Read the file header into the struct
    _lread(iFileHandle, (LPSTR)&ri, sizeof(ROOMIMAGEFILEHEADERSTRUCT));
    // --- Prepare the file header info
    if(strcmp(ri.szFileDesc, "HighGrow Growroom Image File") != 0)
        return FALSE;
    if(strcmp(ri.szFileBluff, "BMP")  != 0)
        return FALSE;
    if(ri.dwBitmapSize != dwsize)
        return FALSE;
    // --- now check if the serial number matches
    if(bSecure == TRUE)
        { // if secure flag was passed, we must match his drive serial number
        if(ri.dwRegCode != CRGetDiskDriveIDNumber())
            return FALSE;
        }
    else // if secure flag is false, we try to match the public (distributable) key
        {
        if(ri.dwRegCode != 178648763)
            return FALSE;
        }
    // --- return our flag
    return TRUE;
    }



BOOL CRDecryptImageFile(LPSTR lpSourceFile, LPSTR lpDestFile)
    {
    int hSFile, hDFile;
    HANDLE hMem = 0;
    LPVOID lpv;
    OFSTRUCT Sofs, Dofs;
    BOOL bReturn = FALSE;
    DWORD dwsize = 0;
    LONG ppos = 0;
    int iHdrSize = sizeof(ROOMIMAGEFILEHEADERSTRUCT);
    // -----
    // set the cursor to an hourglass in case this takes some time
    SetCursor(LoadCursor(NULL, IDC_WAIT));
    // first we open the source file
    if((hSFile = OpenFile(lpSourceFile, &Sofs, OF_READ)) != -1)
        { // now we get the size of this file
        dwsize = (DWORD)(GetFileSize((HANDLE)hSFile, NULL) - iHdrSize);
        // check if the file header is acceptable (key matches his drive serial number)
        if(CRCheckPrivateHeader(hSFile, dwsize, TRUE))
            { // First delete any existing destination file
            DeleteFile(lpDestFile);
            // open a new destination file for reading and writing
            if((hDFile = OpenFile(lpDestFile, &Dofs, OF_CREATE | OF_READWRITE)) != -1)
                { // Allocate enough memory for reading and encrypting the file into
                hMem = GlobalAlloc(GMEM_MOVEABLE, dwsize);
                if(hMem)
                    { // if we allocated the memory, lock it now
                    lpv = (LPVOID)GlobalLock(hMem);
                    if(lpv) 
                        { 
                        // here we read the source file into the memory
                        if(dwsize == _lread(hSFile, (LPSTR)lpv, dwsize));
                            { 
                            // now encrypt the source file data in the buffer
                            CREncodeBuffer((LPSTR)lpv, (LONG)dwsize, 
                                           (LPSTR)lpEncryptPassword, strlen(lpEncryptPassword), &ppos); 
                            // once we encrypted our file data, we can write it to the destination file
                            if(dwsize == _lwrite(hDFile, (LPSTR)lpv, dwsize));
                                { // YIPPEE - if we're still here, then everything was successful!
                                bReturn = TRUE;
                                }
                            }
                        // now we can unlock our global memory again
                        GlobalUnlock(hMem);    
                        }
                    // now we can free our global memory again
                    GlobalFree(hMem);
                    }
                // here we close the destination file
                _lclose(hDFile);
                }
            }
        // here we close the source file
        _lclose(hSFile);
        }
    // now we can reset our standard arrow cursor again
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    // if we converted everything successfully, our flag should be true
    return bReturn;
    }



BOOL CRConvertAndDecryptImageFile(LPSTR lpSourceFile, LPSTR lpDestFile)
    { // *** HGB IMAGE FILE READER ***
    // first try to read the source file as a disk serial# key encrypted growroom image
    // if we failed to read it as a valid image for this hard drive, try to read it as
    // a public (distributable) key encrypted image file. If it is a public key file, we
    // upgrade it to the private key file and then try to convert it to the destination file
    int hFile;
    OFSTRUCT ofs;
    BOOL bReturn = FALSE;
    DWORD dwsize = 0;
    int iHdrSize = sizeof(ROOMIMAGEFILEHEADERSTRUCT);
    // see if we manage to read the private key encrypted image file
    bReturn = CRDecryptImageFile(lpSourceFile, lpDestFile);
    // if we failed, we'll try to upgrade it
    if(bReturn == FALSE)
        { // the file in read/write mode (so that we can upgrade it)
        if((hFile = OpenFile(lpSourceFile, &ofs, OF_READWRITE)) == -1)
            return FALSE;
        dwsize = (DWORD)(GetFileSize((HANDLE)hFile, NULL) - iHdrSize);
        // check if the file header includes the distributable serial key
        if(CRCheckPrivateHeader(hFile, dwsize, FALSE))
            { // here we try to upgrade the private header with his unique serial key
            if(CRUpgradePrivateFileHeader(hFile, dwsize))
                { // we managed the upgrade, so close the file and try reading it again
                _lclose(hFile);
                return CRConvertAndDecryptImageFile(lpSourceFile, lpDestFile);
                }
            }
        // failed to read the public header, so close file 
        _lclose(hFile);
        }
    return bReturn;
    }
