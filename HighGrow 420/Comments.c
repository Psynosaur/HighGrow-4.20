/******************************************************************************\
*
*  PROGRAM:     Comments.c
*
*  PURPOSE:     Loads Robbie's comments from the Comments.dll file.
*
\******************************************************************************/

#include <windows.h>
#include "stdio.h"
#include "comres.h" // holds IDs for all our text comments
#include "global.h"

 
HINSTANCE hCommentsLib = NULL; // handle to our comments library

/******************************************************************************\
*
*  FUNCTION:   COLoadCommentLibrary - loads the comments dll
*
\******************************************************************************/

BOOL COLoadCommentLibrary(HWND hwnd)
   {
   if(!hCommentsLib)
     {
     if(!(hCommentsLib = LoadLibrary("COMMENTS.DLL")))
        {
        MessageBox(hwnd, (LPCTSTR) "Comments.dll load failed!",
                  (LPCTSTR) "Load Error", MB_OK | MB_ICONEXCLAMATION);
        return FALSE;
        }
     }
   return TRUE;
   }

/******************************************************************************\
*
*  FUNCTION:   COFreeCommentLibrary - frees the comments dll
*
\******************************************************************************/

void COFreeCommentLibrary(void)
   {
   if(hCommentsLib)
      {
      FreeLibrary(hCommentsLib);
      hCommentsLib = NULL;
      }
   }

/******************************************************************************\
*
* FUNCTION: COReadCommentResource - reads our comments from the library
*
\******************************************************************************/

BOOL COReadCommentResource(char* szComment, int iStrLen, UINT idComment)
     {
     // if we haven't loaded our library, return a false
	 if(hCommentsLib==NULL) return FALSE;
     // if we couldn't load the correct string, return a false
	 if(!LoadString(hCommentsLib, idComment, szComment, iStrLen)) 
         return FALSE;
     // everythings hunky-dory, we can return a true
	 return TRUE;
     }



/******************************************************************************\
*
* FUNCTIONS TO: Build special comments based on current growth conditions
*
\******************************************************************************/


/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildStageComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // |Oh Wow|, |Mary Jane| |has just started vegetating|

    // first replacements:
    // -------------------
    //  Yikes, Oh dear, Sadly, Beware, Watch it, It seems, Great, Well well, ..
    //  .. Wow, Cool, Fantastic, Amazing

    // second replacement:
    // -------------------
    // Plant name

    // third replacements:
    // -------------------
    // 1. seed:
    // --------
    // is just lying there, starting to show some life, has almost sprouted,
    // 2. germination:
    // ---------------
    // has just germinated, is heading for the surface, has reached the light,
    // 3. seedling:
    // ------------
    // has shown it's first two leaves, has a sturdy little stem,
    // is past the critical stage, is growing well, looks fine to me
    // 4. vegetative:
    // --------------
    // has just started vegetating, is vegetating profusely, 
    // is ending it's vegetative stage
    // 5. preflowering:
    // ----------------
    // is preparing for flowering, should start flowering soon, is about to flower, 
    // 6. flowering:
    // -------------
    // has started flowering, is a male/female, is flowering strongly,
    // 7. seed set:
    // ------------


    }

/******************************************************************************\
* builds the growday and unregistered days remaining comment
\******************************************************************************/

void COBuildGrowDayComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // At day |70|, there are now |30| unregistered growdays remaining.
    }


/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildHealthComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // With |62|% Health, this plant |is better than average|

    // second replacements:
    // -------------------
    // is already dead, is a lost cause, will probably die soon, 
    // .. cannot last much longer, could be saved, could recover, 
    // .. is looking sad, should be better, seems ok to me, isn't looking too bad,
    // .. is growing about right, is better than average, is doing great,
    // .. has never looked better, impresses even me, is almost unbelievable.
    }

/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildHeightComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // Standing |1743|mm tall, growth vigour |is good for a Sativa|

    // second replacements:
    // -------------------

    }

/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildMassComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // Leaf mass is now |1743|g, and |resin is starting to develop|
    }

/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildPotencyComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // At |2.75|% Potency, it seems |you'll have some very potent weed|
    }

/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildMoistureComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // At |475|ml moisture, you should |water within a day or two|
    }

/******************************************************************************\
* builds the general plant status comment
\******************************************************************************/

void COBuildSoilComment(PPLANT plPlant, char* szComment, int iStrLen)
    { // With a pH of |7.2|, you could add |a few grams| of |nutrients| now
    }


/******************************************************************************\
* the main comment assembly routine
\******************************************************************************/


BOOL COBuildSpecialComment(PPLANT plPlant, char* szComment, int iStrLen, UINT idComment)
    {
    switch(idComment)
        {
        case SC_STAGE:
            // |Oh Wow|, |Mary Jane| |has just started vegetating|
            COBuildStageComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_GROWDAY:
            // At day |70|, there are now |30| unregistered growdays remaining.
            COBuildGrowDayComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_HEALTH:
            // With |62|% Health, this plant |is better than average|
            COBuildHealthComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_HEIGHT:
            // Standing |1743|mm tall, growth vigour |is good for a Sativa|
            COBuildHeightComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_MASS:
            // Leaf mass is now |1743|g, and |resin is starting to develop|
            COBuildMassComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_POTENCY:
            // At |2.75|% Potency, it seems |you'll have some very potent weed|
            COBuildPotencyComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_MOISTURE:
            // At |475|ml moisture, you should |water within a day or two|
            COBuildMoistureComment(plPlant, szComment, iStrLen);
        return TRUE;

        case SC_SOIL:
            // With a pH of |7.2|, you could add |a few grams| of |nutrients| now
            COBuildSoilComment(plPlant, szComment, iStrLen);
        return TRUE;
        }

    return FALSE;
    }


