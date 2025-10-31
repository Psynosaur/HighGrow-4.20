/******************************************************************************\
*
*  PROGRAM:     Comments.h
*
*  PURPOSE:     Loads Robbie's comments from the Comments.dll file.
*
\******************************************************************************/

extern HINSTANCE hCommentsLib; // handle to our comments library

BOOL COLoadCommentLibrary(HWND hwnd);
void COFreeCommentLibrary(void);
BOOL COReadCommentResource(char* szComment, int iStrLen, UINT idComment);
