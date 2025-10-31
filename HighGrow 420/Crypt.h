// ***********************************************************************
// ** CRYPT.H - functions to encrypt and decrypt highgrow image files
// ***********************************************************************

typedef struct
   { // Private growroom bitmap file header
    char  szFileDesc[30]; // HighGrow Growroom Image File
    char  szFileBluff[3]; // some text to fool hackers into thinking this may be a .bmp file
    DWORD dwBitmapSize;   // bytes of bitmap in this file 
    DWORD dwRegCode;      // unique code generated on file creation
   } ROOMIMAGEFILEHEADERSTRUCT;


//BOOL CREncodeFile(LPSTR lpFile, LPSTR lpPassword);
BOOL CRConvertAndEncryptImageFile(LPSTR lpSourceFile, LPSTR lpRoomName, BOOL bSecure);
BOOL CRConvertAndDecryptImageFile(LPSTR lpSourceFile, LPSTR lpDestFile);


