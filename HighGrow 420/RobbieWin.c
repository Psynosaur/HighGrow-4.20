/******************************************************************************\
*
*  PROGRAM:     RobbieWin.C
*
*  PURPOSE:     A simple demonstration of LoadLibrary()-ing the Robbie95 DLL at
*               runtime, linking and calling all available functions within it.
*
\******************************************************************************/

#include <windows.h>
#include "stdio.h"
#include "resource.h"
#include "comres.h"
#include "robbie.h"
#include "robbiewin.h"
#include "global.h"
#include "comments.h"
#include "dibitmap.h"
#include "toolbar.h"

PFNDLL gpfnDLLtoonstep      = NULL;
PFNDLL gpfnDLLtoonresource  = NULL;
     
PFNDLL gpfnDLLeztoonresource= NULL;
PFNDLL gpfnDLLeztoonframe   = NULL;

PFNDLL gpfnDLLisplaying     = NULL;
PFNDLL gpfnDLLstop          = NULL;
PFNDLL gpfnDLLpause         = NULL;
PFNDLL gpfnDLLcontinue      = NULL;

PFNDLL gpfnDLLrobbiesize     = NULL;
PFNDLL gpfnDLLrobbiepaint    = NULL;
PFNDLL gpfnDLLrobbietimer    = NULL;
PFNDLL gpfnDLLrobbieendframe = NULL;
PFNDLL gpfnDLLrobbieendsound = NULL;

/******************************************************************************\
*  FUNCTION:   LoadRobbieLibrary - loads the animation dll
\******************************************************************************/

HANDLE hRobbieLib = NULL;

BOOL RRLoadRobbieLibrary(HWND hwnd)
   {
   if(!hRobbieLib)
     {
     if(!(hRobbieLib = LoadLibrary("ROBBIE.DLL")))
        {
        MessageBox(hwnd, (LPCTSTR) "Robbie.dll load failed!",
                  (LPCTSTR) "Load Error", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
        }
     else
        {
        gpfnDLLrobbiesize     = (PFNDLL) GetProcAddress(hRobbieLib, "Robbie_Size");
        gpfnDLLrobbiepaint    = (PFNDLL) GetProcAddress(hRobbieLib, "Robbie_Paint");
        gpfnDLLrobbietimer    = (PFNDLL) GetProcAddress(hRobbieLib, "Robbie_Timer");
        gpfnDLLrobbieendframe = (PFNDLL) GetProcAddress(hRobbieLib, "Robbie_EndFrame");
        gpfnDLLrobbieendsound = (PFNDLL) GetProcAddress(hRobbieLib, "Robbie_EndSound");
        gpfnDLLtoonstep      = (PFNDLL) GetProcAddress(hRobbieLib, "RobbiePlayToonStep");
        gpfnDLLtoonresource  = (PFNDLL) GetProcAddress(hRobbieLib, "RobbiePlayToonResource");
        gpfnDLLeztoonresource= (PFNDLL) GetProcAddress(hRobbieLib, "RobbiePlayToonResourceEz");
        gpfnDLLeztoonframe   = (PFNDLL) GetProcAddress(hRobbieLib, "RobbiePlayToonFrameEz");
        gpfnDLLisplaying     = (PFNDLL) GetProcAddress(hRobbieLib, "IsRobbiePlaying");
        gpfnDLLstop          = (PFNDLL) GetProcAddress(hRobbieLib, "StopRobbiePlayback");
        gpfnDLLpause         = (PFNDLL) GetProcAddress(hRobbieLib, "PauseRobbiePlayback");
        gpfnDLLcontinue      = (PFNDLL) GetProcAddress(hRobbieLib, "ContinueRobbiePlayback");
        }
     }
   return TRUE;
   }

/******************************************************************************\
*  FUNCTION:   FreeRobbieLibrary - frees the animation dll
\******************************************************************************/

void RRFreeRobbieLibrary(HWND hwnd)
   {
   if(hRobbieLib)
      {
      FreeLibrary(hRobbieLib);
      gpfnDLLrobbiesize     = NULL;
      gpfnDLLrobbiepaint    = NULL;
      gpfnDLLrobbietimer    = NULL;
      gpfnDLLrobbieendframe = NULL;
      gpfnDLLrobbieendsound = NULL;
      gpfnDLLtoonstep      = NULL;
      gpfnDLLtoonresource  = NULL;
      gpfnDLLeztoonresource= NULL;
      gpfnDLLeztoonframe   = NULL;
      gpfnDLLisplaying     = NULL;
      gpfnDLLpause         = NULL;
      gpfnDLLstop          = NULL;
      gpfnDLLcontinue      = NULL;
      hRobbieLib = NULL;
      }
   }


/******************************************************************************\
*  CREATING AND DESTROYING ANIMATION WINDOW
\******************************************************************************/

/*
HWND hAnimationWindow = NULL;

void DestroyAnimationWindow(void)
    {
    if(hAnimationWindow)
	    DestroyWindow(hAnimationWindow);
    }
	
BOOL CreateAnimationWindow(HWND hwnd)
    {
    RECT rcWin;
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

	if(hAnimationWindow) return FALSE;

    GetClientRect(hwnd, &rcWin);
    hAnimationWindow = CreateWindow("RobbieAnimate", NULL,
                                   WS_CHILD|WS_VISIBLE,
                                   rcWin.left, rcWin.top,
                                   rcWin.right-rcWin.left,
                                   rcWin.bottom-rcWin.top,
                                   hwnd, NULL, hInstance, NULL);

	if(!hAnimationWindow) return FALSE;

	return TRUE;
    }
*/

/******************************************************************************\
*  FUNCTION:   InterruptPlayback - pauses or continues the playback. 
*                                  changes IDM_INTERRUPT menu item's text to suit.
\******************************************************************************/

/*
void InterruptPlayback(HWND hWnd)
    { 
    HMENU hMenu = GetMenu(hWnd);
    
    // check and exit if any functions not linked
    if(!gpfnDLLisplaying) return;
    if(!gpfnDLLpause)     return;
    if(!gpfnDLLcontinue)  return;

    // now interrupt or continue playback
    if((gpfnDLLisplaying)())
         (gpfnDLLpause)(hWnd);
    else (gpfnDLLcontinue)(hWnd);
    }
*/

/******************************************************************************\
*  FUNCTION:   WalkToMousePosition
*                 - walks from previous position to mouse click position.
*                   when he gets there, he winks and displays current position.
*                   everytime he stops, he poses in the next available pose.
*                   if the last pose is reached, he walks offscreen again.
\******************************************************************************/

/*
int       iCurPose=0;
TOONSTEP  tsMouseWalk;

BOOL WalkToMousePosition(HWND hwnd, LPARAM lParam)
   {
   POINT ptEnd;
   RECT  rcWin;
   // set the end point to be the mouse position
   ptEnd.x = (signed short)LOWORD(lParam);
   ptEnd.y = (signed short)HIWORD(lParam);
   // set some of the other toonstep variables
   tsMouseWalk.itPose = iCurPose;
   tsMouseWalk.uiWait = 5;
   tsMouseWalk.btWink = TRUE;
   // and report mouse position and pose in text
   sprintf(tsMouseWalk.sztText, 
           "Walked to x=%i, y=%i, and now standing in Pose %i", 
           ptEnd.x, ptEnd.y, iCurPose+1);

   // now we'll check if we have to set the start or end point
   GetClientRect(hwnd, &rcWin);
   if(iCurPose==0)
      { // is we haven't started yet, position Robbie offscreen
      tsMouseWalk.itYpos = ptEnd.y;
      if(ptEnd.x<((rcWin.right-rcWin.left)/2))
         tsMouseWalk.itXpos = -150; 
      else
         tsMouseWalk.itXpos = (rcWin.right-rcWin.left)+150; 
      }
   else
      {
      if(iCurPose==16)
         { // if we're at the last pose, make Robbie walk offscreen
         ptEnd.y = tsMouseWalk.itYpos;
         if(tsMouseWalk.itXpos<((rcWin.right-rcWin.left)/2))
            ptEnd.x = -150;
         else
            ptEnd.x = (rcWin.right-rcWin.left)+150;
         
         iCurPose = -1; // to start at zero next time
         tsMouseWalk.sztText[0]=0; // set to speak no text
         tsMouseWalk.uiWait = 1;   // reduce wait period to 1 second
         }
      }
   iCurPose += 1; // increase to show next pose next time
   // Finally call the DLL function to play the toonstep
   return (gpfnDLLtoonstep)(hwnd, (PTOONSTEP)&tsMouseWalk, ptEnd);
   }
*/


/******************************************************************************\
* FUNCTION: This functions read our text resource strings
\******************************************************************************/


BOOL ReadTextResource(HWND hwnd, char* szText, int iStrLen, UINT idString)
     {
     HINSTANCE hInst=(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
	 if(!LoadString(hInst, idString, szText, iStrLen)) return FALSE;
	 return TRUE;
     }


/******************************************************************************\
* FUNCTION: Loads the Animation Dll and prepares for animation
\******************************************************************************/

/*
void RRWakeRobbie(HWND hMainWin)
    { 
    HWND hGrowRoomWin = GetDlgItem(hMainWin, 0); // get handle of growroom

    if(hGrowRoomWin) CreateAnimationWindow(hGrowRoomWin);
    }
*/

/******************************************************************************\
* FUNCTIONS: Preparing for Animation
\******************************************************************************/

/*
#define gMaxFrames    20
#define gMaxToonSteps (gMaxFrames*3)
#define gWalkYpos     337

int      giAnimFrames;             // set during init and used during playback
int      giCurAnimFrame;           // the current toonstep we're animating
TOONSTEP tsAnimate[gMaxToonSteps]; // Robbie's animation control for 3 plants

typedef struct
   { // temp struct for holding comments
   int iPlantAnimStartX;
   int szPlantComments[gMaxFrames]; 
   } TEMPCOMMENTS;

TEMPCOMMENTS TC_PlantComments[3]; // holds comments for our three plants

void RRTempInitCommentAnimation(void)
    {
    // setup comments and walk stop X positions for our two test plants
    TC_PlantComments[0].iPlantAnimStartX   = 60;
    TC_PlantComments[0].szPlantComments[0] = SC_1NAME;
    TC_PlantComments[0].szPlantComments[1] = SC_1GREET;
    TC_PlantComments[0].szPlantComments[2] = 0;

    TC_PlantComments[1].iPlantAnimStartX   = 260;
    TC_PlantComments[1].szPlantComments[0] = SC_2NAME;
    TC_PlantComments[1].szPlantComments[1] = SC_2GREET;
    TC_PlantComments[1].szPlantComments[2] = SC_2LATER;
    TC_PlantComments[1].szPlantComments[3] = 0;
    }    

void RRPrepareCommentAnimation(HWND hwnd)
    {  
    // prepares all the toonsteps we need for today's animation
    // first declare lots of usable ints
    int i, j, iX, iComment;
    RECT rcWin;
    // get the size of our playback window
    GetClientRect(hwnd, &rcWin);
    // ***TEMP*** setup some test data
    RRTempInitCommentAnimation();
    // initialize this now for the toonstep after the offscreen one
    giAnimFrames = 0;
    // now setup a toonstep structures for all our plant's comments
    for(i=0;i<3;i++)
        { // if we have at least one comment for this plant
        if(TC_PlantComments[i].szPlantComments[0] != 0)
            {
            // set the point where robbie stops to describe the plant
            iX = TC_PlantComments[i].iPlantAnimStartX;
            // now set up a toonstep struct element for each comment
            for(j=0;j<gMaxFrames;j++)
                { // set our local for ease
                iComment=TC_PlantComments[i].szPlantComments[j];
                // don nothing if we have no next comment for this plant
                if(iComment != 0) 
                    { // ok, so we have another, we'll add another toonstep
                    // first lookup and set the comment from our comment library
                    if(COReadCommentResource((char*)&tsAnimate[giAnimFrames].sztText,
                                              70,iComment)==FALSE)
                        { // if we didn't find the comment, use the error one
                        ReadTextResource(hwnd, (char*)&tsAnimate[giAnimFrames].sztText, 
                                             128, IDSERR_COMMENT_LOAD);
                        }
                    // now set the other toonstep elements
                    tsAnimate[giAnimFrames].itPose = Random(0, 23);
                    tsAnimate[giAnimFrames].btWink = TRUE;
                    tsAnimate[giAnimFrames].uiWait = 5;
                    tsAnimate[giAnimFrames].itXpos = iX;
                    tsAnimate[giAnimFrames].itYpos = gWalkYpos;
                    // and increment our next toonstep global
                    giAnimFrames += 1;
                    }
                }
            }
        }
    // finally setup the last offscreen toonstep
    tsAnimate[giAnimFrames].itXpos     = (rcWin.right-rcWin.left)+150;
    tsAnimate[giAnimFrames].itYpos     = gWalkYpos;
    tsAnimate[giAnimFrames].uiWait     = 1;
    tsAnimate[giAnimFrames].btWink     = FALSE;
    tsAnimate[giAnimFrames].sztText[0] = 0;
    giAnimFrames += 1;
    }

*/

/******************************************************************************\
* FUNCTIONS: Performing the Animation
\******************************************************************************/

/*     
BOOL RRStartCommentAnimation(HWND hwnd)
    {
    int   i;
    POINT ptEnd;
    // getout if we somehow haven't calculated any yet
    if(giAnimFrames==0) return FALSE;
    // if we're currently animating something, we getout
    if((gpfnDLLisplaying)()==TRUE)  return FALSE;
    // show that we're animating in our toolbar's progress window
    // and update our toolbar's animation progress indicator
    TBInitProgress(giAnimFrames, "Animating..(click to cancel)");
    TBShowNextProgress();
    // because Robbie always walks from itXpos to ptEnd, we must move all
    // the itXpos's on by one toonstep 
    for(i=giAnimFrames;i>=0;i=i-1)
        {
        tsAnimate[i+1].itXpos = tsAnimate[i].itXpos;
        tsAnimate[i+1].itYpos = tsAnimate[i].itYpos;
        }
    // and override the start position of the first toonstep
    tsAnimate[0].itXpos     = -150;
    tsAnimate[0].itYpos     = gWalkYpos;
    // set the end point for our first toonstep as first plant from the left
    ptEnd.x = tsAnimate[1].itXpos;
    ptEnd.y = tsAnimate[1].itYpos;
    // set our current frame indicator for our next toonstep
    giCurAnimFrame = 1;
    // now animate the first toonstep frame
    return(gpfnDLLtoonstep)(hwnd, (PTOONSTEP)&tsAnimate[0], ptEnd);
    }

BOOL RRNextCommentAnimation(HWND hwnd)
    {
    POINT ptEnd;
    BOOL  bSuccess;
    // if we've reached the end of our animation, we getout now
    if(giCurAnimFrame==giAnimFrames)  return FALSE;
    // update our toolbar's animation progress indicator
    TBShowNextProgress();
    // set the end point for our toonstep 
    ptEnd.x = tsAnimate[giCurAnimFrame+1].itXpos;
    ptEnd.y = tsAnimate[giCurAnimFrame+1].itYpos;
    // now animate the next toonstep frame if we can
    bSuccess = (gpfnDLLtoonstep)(hwnd, (PTOONSTEP)&tsAnimate[giCurAnimFrame], ptEnd);
    // increment our current frame indicator for our next toonstep
    giCurAnimFrame += 1;
    // and return our animation started indicator
    return bSuccess;
    }
*/

/******************************************************************************\
*  FUNCTION: Telling him that that there was an error loading one of our dlls
\******************************************************************************/

void RRDllLoadError(HWND hwnd, LPCSTR szDLLName)
    {
    char szErrTextBuff[128]="\0";
    char szCommentBuff[128]="\0";

    ReadTextResource(hwnd, (char*)&szErrTextBuff, 128, IDSERR_DLL_LOAD);
    sprintf(szCommentBuff, szErrTextBuff, szDLLName);
    MessageBox(GetParent(hwnd), (LPCSTR)szCommentBuff,
               "Animation Problem", MB_OK|MB_ICONEXCLAMATION);
    }


/******************************************************************************\
*  FUNCTION: RobbieWndProc (standard window procedure INPUTS/RETURNS)
\******************************************************************************/

/*
LRESULT CALLBACK RobbieWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
  HINSTANCE ghInst = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

  switch (message)
    {
    case WM_CREATE:
        {
         // if not yet loaded, load the comments resource library
         if(COLoadCommentLibrary(hwnd)==FALSE)
             { // if we didn't load any comments, we close our window
             PostMessage(hwnd, WM_CLOSE, 0, 0); 
             RRDllLoadError(hwnd, "comments.dll");
             break;
             }
         // if not yet loaded, load the animation library
         // the library is only loaded once and free'ed on program exit
         if(RRLoadRobbieLibrary(hwnd)==TRUE)
             { // we loaded our dll AND linked the play function...
             // first initialize our toonstep array
             RRPrepareCommentAnimation(hwnd);
             // ok, now we show the window
             ShoRRindow(hwnd, SW_SHOW);
             // and immediatelly start the animation
             RRStartCommentAnimation(hwnd);
             }
         else // we didn't load our dll AND link the play function...
             { // post ourselves a close message
             PostMessage(hwnd, WM_CLOSE, 0, 0); 
             RRDllLoadError(hwnd, "Robbie97.dll");
             }
        }
    break;

    case WM_ERASEBKGND:
         // this is done to prevent the flash before the paint message is
         // received after any of the EzRobbie functions have completed.
    return TRUE;

    case WM_TIMER:
         if(gpfnDLLrobbietimer)
           (gpfnDLLrobbietimer)(hwnd, wParam);
    break;

    case WM_LBUTTONDOWN:
         if(gpfnDLLtoonstep)
            WalkToMousePosition(hwnd, lParam);
    break;

    case WM_PAINT:
        if(gpfnDLLrobbiepaint)
          {
          PAINTSTRUCT ps;
          HDC hdc = BeginPaint(hwnd, &ps);
          (gpfnDLLrobbiepaint)(hdc);
          EndPaint(hwnd, &ps);
          }
    break;

    case WM_SIZE:
        if(gpfnDLLrobbiesize)
          (gpfnDLLrobbiesize)(hwnd, lParam);
    break;

    case WM_ENDFRAME:
        if(gpfnDLLrobbieendframe)
          (gpfnDLLrobbieendframe)(hwnd);
    break;

    case WM_ENDSOUND:
        if(gpfnDLLrobbieendsound)
          (gpfnDLLrobbieendsound)(hwnd);
    break;

    case WM_ENDPLAYBACK:
        // play next toonstep & if we can't, we close animation window
        if(RRNextCommentAnimation(hwnd)==FALSE)
            { // we may not send the message, we must post it
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
    break;

    case WM_DESTROY:
        hAnimationWindow=0;
        TBEndProgress(); // hide the progress window
    break;

    default:
        return (DefWindowProc(hwnd, message, wParam, lParam));
    }
  return (0);
}

*/

/******************************************************************************\
*                                                                              *
*  SHOWING AND MANAGING THE HIGHGROW OBJECTIVES DIALOG                         *
*                                                                              *
\******************************************************************************/

/******************************************************************************\
* Playing Toon Resources
\******************************************************************************/


BOOL RRPlayObjectivesToon(HWND hwnd, LPCSTR szToonRes)
    {
    // if not yet loaded, we load the comments resource library
    if(COLoadCommentLibrary(hwnd)==FALSE)
        { // if we didn't load any comments...
        RRDllLoadError(hwnd, "Comments.dll"); // tell him and getout
        return FALSE;
        }
    // if not yet loaded, we load the animation resource library
    // If we failed to load OR if we didn't correctly link our playback function
    if(RRLoadRobbieLibrary(hwnd)==FALSE||gpfnDLLeztoonresource==NULL)
        { // if we didn't load Robbie or his functions...
        RRDllLoadError(hwnd, "Robbie.dll"); // tell him and getout
        return FALSE;
        }
    // if we're currently animating something, we stop this first
    (gpfnDLLstop)(hwnd);
    // everythink checked out, so let's animate
    return (gpfnDLLeztoonresource)(hwnd, hCommentsLib, szToonRes);
    }

/******************************************************************************\
* Playing Toon Frames
\******************************************************************************/

BOOL RRPlayWindowFrame(HWND hWnd, POINT ptEnd, int iPose, BOOL bWink, LPSTR szText)
    {
    // if not yet loaded, we load the comments resource library
    if(COLoadCommentLibrary(hWnd)==FALSE)
        { // if we didn't load any comments...
        RRDllLoadError(hWnd, "Comments.dll"); // tell him and getout
        return FALSE;
        }
    // if not yet loaded, we load the animation resource library
    // If we failed to load OR if we didn't correctly link our playback function
    if(RRLoadRobbieLibrary(hWnd)==FALSE||gpfnDLLeztoonframe==NULL)
        { // if we didn't load Robbie or his functions...
        RRDllLoadError(hWnd, "Robbie.dll"); // tell him and getout
        return FALSE;
        }
    // if we're currently animating something, we stop this first
    (gpfnDLLstop)(hWnd);
    // everythink checked out, so let's animate
    return (gpfnDLLeztoonframe)(hWnd, ptEnd, iPose, bWink, szText);
    }



/******************************************************************************\
* Playing Toon Resources
\******************************************************************************/

BOOL RRPlayWindowToon(HWND hwnd, LPCSTR szToonFile)
    {
    // if not yet loaded, we load the comments resource library
    if(COLoadCommentLibrary(hwnd)==FALSE)
        { // if we didn't load any comments...
        RRDllLoadError(hwnd, "Comments.dll"); // tell him and getout
        return FALSE;
        }
    // if not yet loaded, we load the animation resource library
    // If we failed to load OR if we didn't correctly link our playback function
    if(RRLoadRobbieLibrary(hwnd)==FALSE||gpfnDLLeztoonresource==NULL)
        { // if we didn't load Robbie or his functions...
        RRDllLoadError(hwnd, "Robbie.dll"); // tell him and getout
        return FALSE;
        }
    // if we're currently animating something, we stop this first
    (gpfnDLLstop)(hwnd);
    // everythink checked out, so let's animate
    return (gpfnDLLeztoonresource)(hwnd, hCommentsLib, szToonFile);
    }


void RRStopWindowToon(HWND hwnd)
    {
    if(gpfnDLLstop)
       (gpfnDLLstop)(hwnd);
    }

/******************************************************************************\
* Resizes all the Objectives dialog controls 
\******************************************************************************/

void RRFitObjectivesDialog(HWND hdlg)
    {
    RECT rcDlg;
    int  i,iX;
    int  iBWidth;       // button width
    int  iBHeight = 22; // button height
    int  iEdge    = 8;  // edge width

    // Resizes the dialog inside the HighGrow Program window. 
    GLSizeDialogToFit(hdlg);
    // let's read the size and pos of the dialog
    GetClientRect(hdlg, &rcDlg);
    // let's move the static window first
    MoveWindow(GetDlgItem(hdlg, IDC_ST01), iEdge, iEdge, 
                      rcDlg.right-rcDlg.left-(iEdge*2),
                      rcDlg.bottom-rcDlg.top-iBHeight-(iEdge*3), FALSE);
    // now we move all our button windows
    iBWidth = ((rcDlg.right-rcDlg.left-(iEdge*2))/7)-iEdge;
    iX      = iEdge; // current button's X position
    for(i=0;i<6;i++)
        {
        MoveWindow(GetDlgItem(hdlg,IDC_RB01+i), 
                    iX, rcDlg.bottom-iEdge-iBHeight, iBWidth, iBHeight, FALSE);
        iX += iBWidth+iEdge;
        }
    MoveWindow(GetDlgItem(hdlg,IDCANCEL), 
               iX+(iEdge*2), rcDlg.bottom-iEdge-iBHeight, 
               iBWidth-iEdge, iBHeight, FALSE);

    }


/******************************************************************************\
* The Objectives WinProc
\******************************************************************************/


BOOL CALLBACK ObjectivesDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
   {
   static HBITMAP  hBackground;
   static HPALETTE hPalette;
   const HINSTANCE dhInst = (HINSTANCE)GetWindowLong(GetParent(hDlg), GWL_HINSTANCE);
    switch (message)
      {
      case WM_INITDIALOG:
           hBackground = DILoadResourceBitmap(dhInst, "LeafTile", &hPalette);
           if(gbRegistered)
               SendDlgItemMessage(hDlg, IDC_RB06, WM_SETTEXT, 0, (LPARAM)" &Thanks");
           RRFitObjectivesDialog(hDlg);
      return (TRUE);

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
          if(i==IDC_ST01)
              {
              RECT rc;
              GetClientRect((HWND)lParam, &rc);
              DITileBitmap((HDC)wParam, rc, hBackground, hPalette);
             }
          else
              {
              SetBkMode((HDC)wParam, TRANSPARENT);
              return (int)GetStockObject(NULL_BRUSH);
              }
          }
      return (FALSE);

      case WM_COMMAND:
          switch (LOWORD(wParam))
            {
            case IDC_RB01:
                RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBIntro");
            return (FALSE);
            
            case IDC_RB02:
                RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBPlant");
            return (FALSE);
            
            case IDC_RB03:
                RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBGrow");
            return (FALSE);
            
            case IDC_RB04:
                RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBCycle");
            return (FALSE);
            
            case IDC_RB05:
                RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBHarves");
            return (FALSE);

            case IDC_RB06:
                if(gbRegistered)
                     RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBThanks");
                else RRPlayObjectivesToon(GetDlgItem(hDlg, IDC_ST01), "OBRegist");
            return (FALSE);

            case IDOK:
            case IDCANCEL:
                {
                DIFreeBitmap(hBackground, hPalette);
                EndDialog(hDlg, TRUE); 
                return TRUE;
                }  
            return (FALSE);
            }
        }
       return (FALSE);
   }                          


