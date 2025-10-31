#include <windows.h>
#include "stdio.h"
#include "global.h"
#include "seedimp.h"
#include "seedlog.h"

// =====================================================================================
//
//  PLANTSEED.C - Initialized the growth characteristics for each seed
//
// =====================================================================================
// ORDER OF MERIT FOR OUR FIRST 12 SEEDS (NEW)
// ===========================================
// 1.  Slick's Surprise - #01 - F - S/I - Tall
// 2.  White Shark      - #05 - F -   I - Short,Bushy - (13seeds=75quid)     +- R58/seed
// 3.  AK-47            - #06 - F - S/I - Tall        - (11seeds=220dollars) +- R120/seed
// 4.  Super Crystal    - #03 - M - S/I - Short,Bushy -                       -
// 5.  Jack Flash       - #02 - F -   I - Short,Bushy - (15seeds=85quid)     +- R57/seed
// 6.  White Rhino      - #08 - F - S/I - Tall        - (13seeds=80quid)     +- R62/seed
// 7.  Blueberry        - #07 - F - S/I - Dense,Stout - (11seeds=110dollars) +- R60/seed
// 8.  Orange Bud       - #11 - F -   I - Short,Bushy - (10seeds=40quid)     +- R40/seed
// 9.  Holland's Hope   - #10 - F -   I - Tall        - (10seeds=35quid)     +- R35/seed
// 10. Thai             - #12 - M -   S - Medium      - (10seeds=35quid)     +- R35/seed
// 11. Amstel Gold      - #09 - M -   I - Tall,Bushy  - (10seeds=55dollars)  +- R33/seed
// 12. Ruderalis Skunk  - #04 - M - R/I - Short,Bushy -                       -

// ORDER OF MERIT FOR OUR SECOND 12 SEEDS
// ======================================
// 1.  Super Skunk     - Female Indica
// 2.  Silver Pearl    - Female Indica/Sativa
// 3.  Northern Lights - Female Indica
// 4.  Shiva Skunk     - Male Indica
// 5.  Silver Haze     - Female Sativa
// 6.  Malawi Gold     - Female Sativa
// 7.  Sweet Dreams    - Female Indica
// 8.  California Gold - Female Sativa
// 9.  Durban Poison   - Female Sativa
// 10. Maui Wowie      - Male Sativa
// 11. Big Bud         - Male Indica
// 12. Hindu Kush      - Male Indica

// ORDER OF MERIT FOR OUR NEXT 6 SEEDS
// ======================================
// 1.  White Widow     - Female Indica/Sativa
// 2.  Kali Mist       - Female Sativa
// 3.  Winter Koning   - Male Sativa
// 4.  Star Warz       - Female Indica
// 5.  Domina Haze     - Male Indica
// 6.  Himilayan Gold  - Female Indica

// 0 = Seed           1 = Germination   2 = Seedling   3 = Vegetative
// 4 = Preflowering   5 = Flowering     6 = Seed Set   7 = Dying


// --------------------------------------------------------------------------------
//  THE MAIN SEED INITIALIZATION PROCESS
// --------------------------------------------------------------------------------

PLANTSEED PS_Plant[gMaxPlants];    // holds info for the all the seeds

void PSCopySeed(PPLANTSEED PPS_Plant, BOOL bFemale, BOOL bIndica,
                LPCSTR szHealthWeeks, LPCSTR szPotencyWeeks, LPCSTR szWaterWeeks,
                LPCSTR szFertWeeks,   LPCSTR szSplitNodes,   int* szGrowthStageDay,
                LPCSTR szNodeGrowth,  LPCSTR szTipsGrowth)
    {
    int i;
    PPS_Plant->bSeedGender = bFemale; // True = Female
    PPS_Plant->bSeedIndica = bIndica; // True = Indica
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szHealthWeeks[i]   =szHealthWeeks[i];
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szPotencyWeeks[i]  =szPotencyWeeks[i];
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szWaterWeeks[i]    =szWaterWeeks[i];
    for(i=0;i<gMaxWeeks;i++) PPS_Plant->szFertWeeks[i]     =szFertWeeks[i];
    for(i=0;i<10;i++)        PPS_Plant->szSplitNodes[i]    =szSplitNodes[i];
    for(i=0;i<8;i++)         PPS_Plant->szGrowthStageDay[i]=szGrowthStageDay[i];
    for(i=0;i<8;i++)         PPS_Plant->szNodeGrowth[i]    =szNodeGrowth[i];
    for(i=0;i<8;i++)         PPS_Plant->szTipsGrowth[i]    =szTipsGrowth[i];
//    wsprintf(strbuf, "Health=%i, Potency=%i, Week=%i", 
//             PPS_Plant->szHealthWeeks[2], PPS_Plant->szPotencyWeeks[2], 2);
//    MessageBox(NULL, strbuf, NULL, MB_OK);
    }


void PSInitSeed(PPLANTSEED PPS_Plant, int iSeed)
    { // initializes the PlantSeed struct for any of our 3 seeds
    // if our seed number is invalid, we exit
    if(iSeed < 0) return;
    // if our seed is a seedlog seed, we'll look it up there
    if(iSeed > 29)
        {
        SGInitLogSeed(PPS_Plant, iSeed-30);
        return;
        }
    // the seed wasn't from our log, so continue
    if(iSeed==0)
        { // Slick's Surprise
        char szHealthWeeks[gMaxWeeks+1]
                 ={10,13,16,19,25,34,42,49,52,55,56,58,60,61,62,64,66,68,69,71,
                   72,71,73,74,75,75,76,76,75,75,75,73,70,68,59,54,48,38,32,0 };
        char szPotencyWeeks[gMaxWeeks+1]
                 ={0, 1, 1, 2, 2, 4, 6, 8, 9, 11,12,16,19,21,26,28,31,29,28,24,
                   24,25,29,33,36,39,41,42,41,39,38,38,37,37,36,36,35,34,33,32};
        char szWaterWeeks[gMaxWeeks+1]
                 ={0, 1, 3,  6, 8,11,16,23,29,33,37,42,51,58,62,63,60,54,42,32,
                   42,53,60,62,65,62,58,56,54,52,51,50,48,46,43,41,38,36,32,30};
        char szFertWeeks[gMaxWeeks+1]
                 ={17,19,10,8, 7, 6, 9, 14,21,29,34,37,39,40,35,29,28,28,29,29,
                   30,32,34,35,35,35,34,33,28,20,10,6, 4, 3, 2, 1, 1, 1, 0, 0};
        char szSplitNodes[10]    ={2,8,17,28,39,45,75,86,95,112};
        int  szGrowthStageDay[8] ={2,8,31,106,142,216,248,280};
        // mm growth per day for a Strong Female Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,3,6,2,0,0,0};
        char szTipsGrowth[8]     ={0,1,15,11,7,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==1)
        { // Jack Flash
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 7, 9,10,13,17,25,37,43,47,51,52,54,56,58,59,59,61,62,63,66,
                   68,66,69,71,69,71,72,70,70,71,69,67,62,61,52,49,37,31,23, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 2, 3, 5, 8, 9,10,13,15,17,21,25,28,29,28,24,18,
                   20,22,28,33,35,38,40,41,38,39,38,37,36,35,34,33,33,33,30,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 6, 8,11,13,15,22,27,33,37,39,48,47,53,63,62,52,41,29,
                   33,49,50,61,62,57,51,53,46,47,44,42,43,39,36,34,32,33,28,27};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,12, 7, 2, 2, 1, 5,11,17,25,30,31,36,35,31,28,25,23,26,24,
                   26,27,30,30,31,29,31,30,22,17, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={4,11,17,28,35,43,51,62,77,103};
        int  szGrowthStageDay[8] ={2,9,31,124,138,218,245,280};
        // mm growth per day for a Strong female Indica Plant
        char szNodeGrowth[8]     ={0,0,4,7,4,2,1,0};
        char szTipsGrowth[8]     ={0,2,16,10,6,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==2)
        { // Super Crystal
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 6, 9,12,15,19,28,36,45,48,52,51,55,56,58,59,60,61,65,66,65,
                   66,70,72,70,69,71,72,71,73,72,71,67,64,60,57,50,40,31,25, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 1, 2, 3, 4, 6, 8, 7, 8, 10,16,20,23,26,25,27,20,18,22,29,
                   31,33,34,35,33,34,32,33,31,32,31,30,28,29,28,28,27,27,27,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 7,13,18,24,28,38,45,49,51,56,58,62,49,46,40,49,51,
                   60,60,66,54,53,46,46,45,39,35,31,29,27,28,25,27,25,26,22,23};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,14, 5, 3, 4, 2, 6,10,18,24,31,32,36,35,32,25,25,26,24,27,
                   27,28,31,33,30,33,32,31,25,18, 7, 4, 1, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={7,10,13,27,38,51,67,76,89,100};
        int  szGrowthStageDay[8] ={2,7,30,103,123,151,261,275};
        // mm growth per day for a Strong Male Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,4,5,3,2,0,0};
        char szTipsGrowth[8]     ={0,3,14,12,5,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==3)
        { // Ruderalis Skunk
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 2, 4, 7,10,19,30,37,39,41,44,46,49,47,51,53,55,56,58,60,
                   58,61,62,62,63,64,64,65,65,64,64,60,53,53,46,41,33,21,17, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 2, 4, 6, 6, 6, 7, 9,14,20,23,24,25,24,21,19,20,27,
                   30,32,33,34,35,34,33,33,32,31,30,29,28,27,26,27,27,25,26,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 3,12,14,20,32,36,41,44,46,52,53,45,37,31,40,46,
                   48,54,56,47,43,42,36,36,31,27,22,19,20,20,21,17,17,17,17,17};
        char szFertWeeks[gMaxWeeks+1]
                 ={10,10, 5, 4, 0, 0, 5, 9,14,22,29,31,32,34,29,22,22,22,24,23,
                   26,27,28,29,31,28,28,28,22,14, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,6,12,20,29,34,43,63,82,99};
        int  szGrowthStageDay[8] ={2,7,29,104,128, 150,276,279};
        // mm growth per day for a weak male Indica Plant
        char szNodeGrowth[8]     ={0,0,4,5,2,1,1,0};
        char szTipsGrowth[8]     ={0,7,14,12,6,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==4)
        { // White Shark
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 8,12,16,17,21,29,37,44,48,52,56,57,59,60,62,64,65,67,66,68,
                   69,70,73,74,75,74,75,72,73,74,72,69,66,64,59,50,40,31,26, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 1, 2, 4, 6, 8, 9, 12,13,14,16,19,22,25,28,32,29,27,19,
                   20,25,28,33,37,39,40,42,39,38,36,35,36,35,34,35,33,31,32,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 8,10,14,16,22,21,31,37,43,42,49,53,60,62,67,53,44,31,
                   42,49,57,65,63,63,55,51,53,50,47,49,45,41,43,39,33,35,31,27};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,13,11, 6, 4, 3, 7,13,19,27,31,36,38,37,35,29,28,28,29,29,
                   29,31,32,33,34,35,34,32,25,18, 9, 3, 3, 0, 2, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,5,18,25,33,48,56,68,83,102};
        int  szGrowthStageDay[8] ={2,9,32,115,145,203,239,280};
        // mm growth per day for a Strong female Indica Plant
        char szNodeGrowth[8]     ={0,0,3,6,2,0,0,0};
        char szTipsGrowth[8]     ={0,3,15,12,5,1,0,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==5)
        { // AK-47
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 8,11,15,18,22,26,36,43,49,54,53,55,56,58,58,62,63,65,68,67,
                   70,71,69,72,70,70,71,71,72,72,71,69,65,61,57,51,41,32,25, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 1, 2, 4, 6, 7, 8,10,13,17,19,24,28,29,30,27,19,
                   21,24,29,32,35,37,39,42,43,37,36,35,36,35,36,35,32,31,31,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 8,11,13,14,19,22,32,37,42,41,50,51,57,64,67,53,43,27,
                   42,50,55,64,62,63,54,52,48,51,49,46,42,43,40,37,34,33,32,26};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,17, 6, 5, 4, 5, 6,12,18,28,30,34,35,37,34,28,24,26,27,27,
                   27,28,30,34,32,34,30,31,23,17, 6, 5, 2, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,10,19,40,44,56,74,82,88,97};
        int  szGrowthStageDay[8] ={2,9,37,129,136,212,243,280};
        // mm growth per day for a Strong Female Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,2,4,2,1,0,0};
        char szTipsGrowth[8]     ={0,2,16,12,6,1,0,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==6)
        { // Blueberry
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 3, 8, 9,12,15,21,34,42,45,47,49,50,54,54,57,56,59,61,63,62,
                   65,65,67,67,68,68,69,69,70,70,69,67,64,56,51,45,35,26,20, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 1, 4, 6, 8, 9,10,11,15,17,21,24,26,28,28,24,18,
                   19,24,28,31,36,39,38,41,39,38,37,37,35,35,34,35,32,32,30,30};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 6, 9,11,15,18,28,31,36,37,44,49,54,60,63,51,37,24,
                   37,45,48,58,59,58,54,48,47,46,45,40,39,39,38,32,29,29,26,25};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,11, 6, 1, 2, 0, 5, 7,16,23,29,33,33,36,30,24,21,24,24,23,
                   23,28,28,29,31,30,30,26,24,12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,8,23,28,35,39,47,61,73,85};
        int  szGrowthStageDay[8] ={2,9,35,118,136,212,248,280};
        // mm growth per day for a Female Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,2,3,2,1,0,0};
        char szTipsGrowth[8]     ={0,2,10,8,5,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==7)
        { // White Rhino
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 5, 7,11,13,17,25,36,41,47,48,48,52,53,56,57,58,61,62,64,63,
                   65,66,66,67,68,70,71,71,70,70,68,67,64,57,53,48,39,27,22, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 1, 1, 2, 2, 6, 7, 8, 9,10,14,16,22,24,29,29,26,25,20,
                   22,23,27,32,36,37,39,40,41,38,37,35,36,34,34,34,33,33,30,28};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 6, 9, 8,10,17,21,26,32,38,38,45,49,56,59,65,51,39,24,
                   40,46,53,61,60,56,52,48,49,47,43,44,40,40,36,33,32,30,25,25};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,13, 5, 1, 1, 0, 2, 7,18,25,27,31,34,36,31,25,21,22,24,25,
                   25,26,27,31,30,29,27,29,23,14, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,7,13,19,32,39,45,57,68,75};
        int  szGrowthStageDay[8] ={2,9,33,117,134,212,249,280};
        // mm growth per day for a Strong female Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,5,8,4,2,1,0};
        char szTipsGrowth[8]     ={0,3,17,12,7,3,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==8)
        { // Amstel Gold
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 3, 4, 8,13,19,29,37,43,43,45,46,47,48,50,55,56,57,58,59,
                   61,62,63,64,64,65,65,64,64,63,62,61,56,52,47,41,31,22,16, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 1, 2, 4, 7, 7, 8, 8, 8, 9,15,20,23,24,26,25,21,18,20,27,
                   30,32,33,35,35,34,33,32,33,33,32,31,29,27,27,27,27,25,27,24};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 6,11,19,22,32,35,40,44,46,47,55,43,35,31,41,46,
                   52,55,58,47,42,39,37,35,33,26,24,22,23,22,19,18,18,17,17,14};
        char szFertWeeks[gMaxWeeks+2]
                 ={13,12, 4, 4, 0, 0, 4, 9,16,24,27,31,33,34,29,22,21,24,25,25,
                   26,27,28,29,29,30,30,26,23,15, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={1,8,15,30,37,46,53,67,76,91};
        int  szGrowthStageDay[8] ={2,7,34,106,119,154,266,280};
        // mm growth per day for a weak male Indica Plant
        char szNodeGrowth[8]     ={0,0,4,7,4,2,1,0};
        char szTipsGrowth[8]     ={0,4,16,9,5,4,3,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==9)
        { // Holland's Hope
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 2, 4, 5,12,15,21,34,39,42,48,46,49,50,53,55,56,57,58,59,62,
                   62,63,64,65,67,64,67,65,67,66,65,63,59,55,50,43,33,24,18, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 1, 1, 2, 3, 6, 6, 7, 8,12,13,18,19,25,29,28,27,25,19,
                   19,24,28,31,35,39,39,40,38,37,36,35,36,35,35,34,32,33,30,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 6, 6, 9,14,14,24,30,36,34,43,44,53,58,58,48,36,21,
                   35,44,51,56,56,56,50,44,45,45,40,41,36,38,35,33,27,26,22,24};
        char szFertWeeks[gMaxWeeks+1]
                 ={10,12, 6, 2, 1, 1, 2, 9,16,24,30,30,35,35,28,22,24,24,25,24,
                   24,26,27,29,31,31,29,27,22,14, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,6,25,30,37,46,54,73,89,118,};
        int  szGrowthStageDay[8] ={2,8,32,122,137,212,243,280};
        // mm growth per day for a good female Indica Plant
        char szNodeGrowth[8]     ={0,0,5,8,4,2,1,0};
        char szTipsGrowth[8]     ={0,3,19,12,6,3,2,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==10)
        { // Orange Bud
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 3, 6, 7,10,15,24,34,38,43,46,48,49,51,54,55,58,59,60,61,61,
                   62,62,64,65,66,65,68,67,67,66,68,64,59,58,52,44,37,27,20, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 0, 2, 5, 7, 8,11,13,15,16,19,24,27,30,28,26,19,
                   21,23,27,30,34,38,38,40,39,36,37,35,36,34,36,33,32,31,32,28};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 4, 5, 7,11,12,19,25,31,36,38,42,48,53,59,59,51,35,24,
                   35,47,47,61,57,54,52,48,44,42,43,42,39,35,36,34,29,28,25,22};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,12, 5, 2, 1, 2, 2, 9,16,23,30,31,34,36,29,24,24,21,24,25,
                   23,28,28,31,29,31,28,28,21,15, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={1,7,17,22,30,40,62,69,78,87};
        int  szGrowthStageDay[8] ={2,8,35,127,142,218,246,280};
        // mm growth per day for a good female Indica Plant
        char szNodeGrowth[8]     ={0,0,4,7,6,3,1,0};
        char szTipsGrowth[8]     ={0,3,16,13,8,5,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==11)
        { // Thai
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 3, 5,10,13,21,30,38,43,47,48,49,51,52,52,54,57,58,59,60,
                   60,64,64,63,65,66,65,67,64,65,65,63,57,55,51,44,34,24,17, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 2, 3, 5, 7, 6, 7, 8,10,15,19,24,25,25,24,21,22,24,28,
                   31,34,35,36,35,35,34,33,32,32,31,30,29,28,28,26,27,25,26,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 1, 2, 6,12,15,23,35,39,41,46,49,53,54,45,40,32,44,45,
                   53,54,59,51,43,42,41,36,34,29,27,21,23,20,22,20,21,21,20,18};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,14, 5, 2, 1, 0, 2,10,16,22,29,30,35,33,31,22,24,21,25,23,
                   24,27,30,28,29,30,29,29,21,14, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,7,12,32,57,63,75,87,97,121};
        int  szGrowthStageDay[8] ={2,8,35,112,125,157,202,270};
        // mm growth per day for a male Sativa Plant
        char szNodeGrowth[8]     ={0,4,8,9,3,2,2,0};
        char szTipsGrowth[8]     ={0,6,20,14,8,6,3,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==12)
        { // Super Skunk
        char szHealthWeeks[gMaxWeeks+1]
                 ={10,12,15,18,22,30,40,47,51,54,56,57,59,60,62,64,65,67,69,70,
                   71,72,73,74,74,74,75,75,75,75,74,72,68,64,59,53,43,34,28,0 };
        char szPotencyWeeks[gMaxWeeks+1]
                 ={0, 0, 1, 1, 2, 4, 6, 8, 8, 10,12,15,18,21,26,29,30,29,27,20,
                   21,24,29,32,36,39,40,41,40,38,38,37,37,36,36,35,34,33,32,30};
        char szWaterWeeks[gMaxWeeks+1]
                 ={0, 0, 3,  5, 8,10,16,23,28,33,37,41,50,57,61,63,60,55,43,32,
                   42,52,59,61,64,62,58,56,53,52,51,50,47,44,42,40,38,36,32,30};
        char szFertWeeks[gMaxWeeks+1]
                 ={17,19,10,8, 7, 6, 9, 14,21,29,34,37,39,40,35,29,28,28,29,29,
                   30,32,34,35,35,35,34,33,28,20,10,6, 4, 3, 2, 1, 1, 1, 0, 0};
        char szSplitNodes[10]    ={2,8,18,26,37,46,72,88,95,112};
        int  szGrowthStageDay[8] ={2,8,30,109,143,218,248,280};
        // mm growth per day for a Strong Female Indica Plant
        char szNodeGrowth[8]     ={0,0,2,5,1,0,0,0};
        char szTipsGrowth[8]     ={0,1,14,10,6,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==13)
        { // Durban Poison
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 2, 3, 5,11,14,21,33,39,42,47,46,49,50,53,54,54,57,58,59,62,
                   62,63,66,65,67,64,67,65,67,67,66,63,59,55,50,44,33,24,18, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 1, 1, 2, 3, 5, 6, 6, 8,12,13,17,19,25,29,28,29,25,19,
                   19,24,28,32,34,39,38,40,38,37,36,35,37,36,35,34,32,33,30,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 6, 6, 9,15,14,24,30,36,35,43,44,53,58,59,48,36,21,
                   35,44,50,56,56,56,50,44,44,45,40,41,36,38,34,33,27,27,22,24};
        char szFertWeeks[gMaxWeeks+1]
                 ={10,13, 6, 2, 2, 2, 2, 9,16,23,30,30,35,35,29,22,24,24,25,24,
                   24,26,27,28,31,31,29,28,22,14, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,6,23,29,33,42,56,73,89,118,};
        int  szGrowthStageDay[8] ={2,8,33,121,137,212,243,280};
        // mm growth per day for a good female Sativa Plant
        char szNodeGrowth[8]     ={0,0,4,8,3,2,1,0};
        char szTipsGrowth[8]     ={0,3,19,11,6,3,2,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==14)
        { // Silver Haze
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 4, 9,10,12,17,25,35,43,47,50,52,54,56,57,59,59,60,62,64,66,
                   68,66,68,71,69,70,72,69,70,71,69,68,62,61,54,49,37,31,23, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 2, 2, 5, 8, 8,10,12,15,17,20,25,28,29,27,25,18,
                   19,22,28,31,35,38,40,41,38,38,36,36,36,36,34,33,33,33,30,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 6, 7,11,13,15,22,28,33,39,39,48,47,55,63,62,52,40,25,
                   40,49,50,61,62,57,51,51,46,47,47,42,43,39,39,34,32,33,27,27};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,15, 7, 2, 2, 1, 5,11,15,24,30,31,36,35,31,23,25,23,26,24,
                   26,26,30,30,31,29,31,30,22,17, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,9,16,28,35,41,49,62,77,103};
        int  szGrowthStageDay[8] ={2,8,32,120,139,212,245,280};
        // mm growth per day for a Strong female Sativa Plant
        char szNodeGrowth[8]     ={0,0,5,8,4,2,1,0};
        char szTipsGrowth[8]     ={0,2,18,11,6,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==15)
        { // Hindu Kush
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 2, 2, 6,10,19,29,37,38,41,44,46,49,47,50,53,55,55,58,60,
                   58,61,61,62,63,63,64,64,65,65,64,62,56,53,46,41,33,21,17, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 2, 4, 6, 6, 6, 7, 9,15,20,23,24,25,24,21,19,20,29,
                   30,33,34,35,35,34,32,33,32,31,30,30,28,29,26,27,27,25,26,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 3,12,14,20,32,36,41,44,46,52,53,45,37,31,40,46,
                   48,54,56,47,43,42,36,36,31,27,22,19,20,20,21,17,17,17,17,17};
        char szFertWeeks[gMaxWeeks+1]
                 ={10,13, 5, 4, 0, 0, 5, 9,15,22,30,31,32,36,29,22,23,22,24,23,
                   26,27,28,28,31,28,29,28,22,16, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,6,12,19,29,35,43,73,82,99};
        int  szGrowthStageDay[8] ={2,7,30,106,120,150,276,279};
        // mm growth per day for a weak male Indica Plant
        char szNodeGrowth[8]     ={0,0,4,5,2,1,1,0};
        char szTipsGrowth[8]     ={0,7,13,10,6,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==16)
        { // Shiva Skunk
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 6, 9,11,14,18,28,36,45,47,52,51,55,54,58,59,60,60,65,66,66,
                   66,70,70,70,69,71,72,71,73,70,72,69,64,60,57,50,40,31,25, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 2, 3, 4, 5, 7, 6, 7, 8,16,20,22,26,25,26,20,18,20,28,
                   31,33,34,35,33,34,32,33,31,32,30,31,28,29,27,28,27,27,27,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 7,13,18,24,28,38,45,49,51,56,58,62,49,46,40,49,51,
                   60,60,66,54,53,46,46,45,39,35,31,29,27,28,25,27,25,26,22,23};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,14, 5, 3, 5, 2, 7,10,18,24,31,32,36,35,32,25,24,26,24,27,
                   27,28,30,33,30,33,32,31,26,18, 7, 3, 1, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,9,13,25,38,51,63,71,89,100};
        int  szGrowthStageDay[8] ={2,7,29,103,121,151,261,275};
        // mm growth per day for a Strong Male Indica Plant
        char szNodeGrowth[8]     ={0,0,4,5,3,2,0,0};
        char szTipsGrowth[8]     ={0,2,15,12,6,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==17)
        { // California Gold
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 3, 6, 6,10,15,24,34,38,42,46,48,49,52,53,55,57,59,60,61,62,
                   62,66,66,66,66,65,69,68,67,66,68,64,59,58,52,44,37,27,20, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 0, 2, 5, 7, 8,10,10,15,16,19,25,27,30,28,26,18,
                   21,23,27,31,34,38,38,40,40,36,37,35,36,34,36,33,32,31,32,28};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 4, 4, 7,11,12,19,24,31,36,38,42,48,52,59,59,51,35,24,
                   35,46,47,61,57,54,51,48,44,42,44,42,39,35,37,34,29,28,25,22};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,12, 5, 2, 1, 2, 2, 9,16,23,30,31,34,36,29,24,24,21,24,25,
                   23,28,28,31,29,31,28,28,21,15, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={1,7,19,23,32,43,62,69,78,87};
        int  szGrowthStageDay[8] ={2,8,35,127,142,218,246,280};
        // mm growth per day for a good female Sativa Plant
        char szNodeGrowth[8]     ={0,0,6,10,6,3,1,0};
        char szTipsGrowth[8]     ={0,4,18,13,8,5,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==18)
        { // Maui Wowie
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 2, 5, 9,13,21,31,38,43,46,48,49,51,51,52,53,57,58,59,60,
                   60,64,64,63,66,65,65,67,64,66,64,64,57,55,51,44,34,24,17, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 2, 2, 5, 7, 6, 6, 6,10,15,19,24,25,25,24,20,18,21,28,
                   31,34,35,36,35,34,34,33,33,32,31,30,29,28,28,26,27,25,26,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 6,14,15,23,33,39,41,46,49,53,55,45,40,32,44,45,
                   53,54,59,50,43,42,41,36,35,26,27,21,23,20,22,20,21,21,20,18};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,14, 5, 2, 1, 0, 2,10,16,22,29,30,35,33,31,22,24,21,25,23,
                   24,27,30,28,29,30,29,29,21,14, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,5,12,32,58,63,75,88,97,121};
        int  szGrowthStageDay[8] ={2,7,34,112,123,157,203,270};
        // mm growth per day for a male Sativa Plant
        char szNodeGrowth[8]     ={0,3,8,9,3,2,2,0};
        char szTipsGrowth[8]     ={0,6,22,13,8,6,3,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==19)
        { // Sweet Dreams
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 3, 7, 9,11,15,22,35,42,45,47,49,49,54,53,57,56,59,59,63,62,
                   65,64,67,67,67,68,69,69,69,69,69,67,63,56,51,45,35,26,20, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 1, 4, 6, 8, 8,10,11,15,16,21,24,28,28,28,27,18,
                   19,23,28,31,36,39,38,40,38,38,37,37,35,35,34,35,32,32,30,30};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 6, 9,10,15,18,27,31,36,38,46,49,54,60,63,50,37,24,
                   37,45,48,58,58,57,51,46,46,46,45,40,39,39,38,32,29,29,26,25};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,12, 5, 1, 2, 0, 5, 7,16,22,29,33,33,36,30,23,21,24,24,23,
                   23,28,29,29,31,29,30,26,24,13, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,9,23,28,36,39,48,61,73,85};
        int  szGrowthStageDay[8] ={2,8,34,118,136,212,248,280};
        // mm growth per day for a Female Indica Plant
        char szNodeGrowth[8]     ={0,0,2,4,2,1,0,0};
        char szTipsGrowth[8]     ={0,2,11,9,5,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==20)
        { // Big Bud
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 3, 3, 7,13,18,29,37,42,43,45,46,48,49,50,55,56,57,58,58,
                   62,62,63,63,64,64,65,64,64,64,63,61,56,52,47,41,31,22,16, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 1, 2, 4, 6, 7, 7, 7, 7, 9,15,20,23,25,26,25,20,18,20,27,
                   30,32,33,35,34,33,33,32,33,32,32,31,29,27,27,27,27,25,27,24};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 7,11,18,22,32,34,40,44,46,49,55,43,36,31,41,45,
                   51,55,58,47,42,39,38,35,33,26,26,22,23,22,18,18,18,17,17,14};
        char szFertWeeks[gMaxWeeks+2]
                 ={13,12, 4, 4, 0, 0, 4, 8,16,25,27,32,33,34,28,22,21,24,25,25,
                   25,27,28,28,29,30,30,26,22,15, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={1,8,17,31,38,48,53,67,76,91};
        int  szGrowthStageDay[8] ={2,7,33,107,119,153,266,280};
        // mm growth per day for a weak male Indica Plant
        char szNodeGrowth[8]     ={0,0,4,6,3,2,1,0};
        char szTipsGrowth[8]     ={0,3,14,8,5,4,3,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==21)
        { // Silver Pearl
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 8,11,15,15,20,29,40,44,48,51,56,57,59,60,61,63,65,67,66,67,
                   69,70,72,74,74,73,75,72,73,74,74,69,66,63,59,50,40,31,26, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 0, 3, 4, 7, 6, 9,10,15,17,21,24,28,30,27,25,18,
                   20,23,28,32,36,39,40,41,38,38,37,35,37,35,34,35,33,31,32,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 8,10,14,16,22,21,31,37,43,42,49,53,60,62,67,53,44,31,
                   42,49,57,65,63,63,55,51,53,50,47,49,45,41,43,39,33,35,31,27};
        char szFertWeeks[gMaxWeeks+1]
                 ={15,16,10, 6, 4, 3, 6,11,19,27,32,36,38,39,35,29,28,28,29,29,
                   29,31,32,32,32,35,34,32,26,18, 9, 3, 3, 0, 2, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,4,19,27,34,48,56,68,83,102};
        int  szGrowthStageDay[8] ={2,8,34,117,143,203,239,280};
        // mm growth per day for a Strong female Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,3,7,2,0,0,0};
        char szTipsGrowth[8]     ={0,2,16,11,6,1,0,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==22)
        { // Malawi Gold
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 5, 5,10,12,17,24,36,41,47,48,49,52,53,56,57,57,61,61,64,63,
                   65,66,66,67,67,70,71,71,70,70,68,67,64,57,53,48,39,27,22, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 1, 0, 2, 2, 6, 7, 7, 9,10,14,16,21,24,29,29,28,25,20,
                   21,23,27,32,36,37,39,40,40,38,36,35,36,34,34,34,33,33,30,28};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 6, 9, 8,10,17,21,26,32,38,38,45,49,56,59,65,51,39,24,
                   40,46,53,61,60,56,52,48,49,47,43,44,40,40,36,33,32,30,25,25};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,14, 5, 2, 1, 0, 2, 7,17,25,27,31,34,36,31,22,21,22,24,25,
                   25,26,27,31,30,29,27,29,23,14, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,6,13,19,32,38,45,57,68,75};
        int  szGrowthStageDay[8] ={2,9,33,118,134,218,249,280};
        // mm growth per day for a Strong female Sativa Plant
        char szNodeGrowth[8]     ={0,0,6,9,3,2,1,0};
        char szTipsGrowth[8]     ={0,3,19,12,7,3,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==23)
        { // Northern Lights
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 8,11,14,17,18,26,36,44,48,53,52,56,55,58,58,62,62,64,68,67,
                   70,70,69,73,70,70,71,71,71,72,71,69,65,61,57,51,41,32,25, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 0, 2, 4, 6, 6, 8,10,13,16,19,24,28,29,29,25,19,
                   20,24,29,30,34,38,39,41,40,36,36,35,36,35,36,35,32,31,31,29};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 8,10,13,14,19,22,32,36,42,41,50,50,57,64,67,52,43,28,
                   42,50,55,64,61,63,54,51,48,51,48,46,42,44,40,37,35,33,32,26};
        char szFertWeeks[gMaxWeeks+1]
                 ={14,17, 6, 6, 4, 5, 6,12,19,28,30,34,36,38,34,28,24,26,27,27,
                   27,29,30,34,31,34,30,31,25,17, 6, 5, 2, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={2,9,18,39,43,56,73,82,88,97};
        int  szGrowthStageDay[8] ={2,9,36,129,139,212,243,280};
        // mm growth per day for a Strong Female Indica Plant
        char szNodeGrowth[8]     ={0,0,2,4,1,1,0,0};
        char szTipsGrowth[8]     ={0,1,15,12,5,1,0,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==24)
        { // White Widow - Female Indica/Sativa
        char szHealthWeeks[gMaxWeeks+1]
                 ={11,14,17,19,24,33,42,48,53,55,57,58,59,61,63,64,66,68,70,71,
                   71,72,73,74,75,75,76,76,77,76,74,72,68,65,59,54,43,36,27,0 };
        char szPotencyWeeks[gMaxWeeks+1]
                 ={0, 1, 1, 2, 3, 4, 6, 8, 9, 11,13,15,18,22,27,29,32,29,27,25,
                   23,24,29,32,37,39,41,42,41,40,39,38,37,36,35,35,34,33,32,30};
        char szWaterWeeks[gMaxWeeks+1]
                 ={0, 0, 3, 4, 8, 11,16,23,28,34,38,41,50,56,61,63,60,55,43,32,
                   42,52,59,61,63,62,58,56,52,52,51,50,47,44,42,40,38,36,32,30};
        char szFertWeeks[gMaxWeeks+1]
                 ={17,18,11,8, 7, 6, 11,14,21,29,35,37,39,41,35,30,28,28,29,30,
                   31,32,32,34,35,35,34,32,29,22,10,6, 4, 3, 2, 1, 1, 1, 0, 0};
        char szSplitNodes[10]    ={2,8,18,26,38,46,71,89,93,112};
        int  szGrowthStageDay[8] ={2,9,31,108,143,219,248,280};
        // mm growth per day for a Strong Female Indica Plant
        char szNodeGrowth[8]     ={0,0,2,5,1,0,0,0};
        char szTipsGrowth[8]     ={0,1,13,10,6,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==25)
        { // Star Warz - Female Indica
        char szHealthWeeks[gMaxWeeks+1]
                 ={10,13,16,19,23,32,41,46,50,54,56,58,59,60,62,64,65,67,69,70,
                   71,72,73,74,75,74,75,75,76,75,74,73,68,65,59,53,46,35,28,0 };
        char szPotencyWeeks[gMaxWeeks+1]
                 ={0, 0, 1, 2, 3, 4, 6, 8, 9, 10,12,16,19,21,26,29,31,29,27,23,
                   21,25,29,32,37,39,40,42,41,39,38,37,36,36,36,35,34,33,32,30};
        char szWaterWeeks[gMaxWeeks+1]
                 ={0, 0, 3,  6, 8,10,16,23,29,34,37,41,51,57,61,63,60,55,43,32,
                   42,52,59,61,64,62,58,56,53,52,51,50,47,44,42,40,38,36,32,30};
        char szFertWeeks[gMaxWeeks+1]
                 ={18,17,11,8, 7, 6, 9, 14,22,29,34,38,39,41,35,29,28,28,29,29,
                   31,33,34,35,35,35,34,33,28,20,10,6, 4, 3, 2, 1, 1, 1, 0, 0};
        char szSplitNodes[10]    ={2,8,17,27,36,46,72,88,95,112};
        int  szGrowthStageDay[8] ={2,8,30,108,144,217,248,280};
        // mm growth per day for a Strong Female Indica Plant
        char szNodeGrowth[8]     ={0,0,2,5,1,0,0,0};
        char szTipsGrowth[8]     ={0,2,13,9,6,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }
                    
    if(iSeed==26)
        { // Himilayan Gold - Female Indica
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 3, 7, 9,11,15,22,35,42,45,47,49,49,54,53,57,56,59,59,63,62,
                   65,64,67,67,67,68,69,69,69,69,69,67,63,56,51,45,35,26,20, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 0, 1, 4, 6, 8, 8,10,11,15,16,21,24,28,28,28,27,18,
                   19,23,28,31,36,39,38,40,38,38,37,37,35,35,34,35,32,32,30,30};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 6, 9,10,15,18,27,31,36,38,46,49,54,60,63,50,37,24,
                   37,45,48,58,58,57,51,46,46,46,45,40,39,39,38,32,29,29,26,25};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,12, 5, 1, 2, 0, 5, 7,16,22,29,33,33,36,30,23,21,24,24,23,
                   23,28,29,29,31,29,30,26,24,13, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={3,9,23,28,36,39,48,61,73,85};
        int  szGrowthStageDay[8] ={2,8,34,118,136,212,248,280};
        // mm growth per day for a Female Indica Plant
        char szNodeGrowth[8]     ={0,0,2,4,2,1,0,0};
        char szTipsGrowth[8]     ={0,2,11,9,5,1,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==27)
        { // Winter Koning - Male Sativa
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 6, 9,12,15,19,28,36,45,48,52,51,55,56,58,59,60,61,65,66,65,
                   66,70,72,70,69,71,72,71,73,72,71,67,64,60,57,50,40,31,25, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 1, 2, 3, 4, 6, 8, 7, 8, 10,16,20,23,26,25,27,20,18,22,29,
                   31,33,34,35,33,34,32,33,31,32,31,30,28,29,28,28,27,27,27,26};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 3, 7,13,18,24,28,38,45,49,51,56,58,62,49,46,40,49,51,
                   60,60,66,54,53,46,46,45,39,35,31,29,27,28,25,27,25,26,22,23};
        char szFertWeeks[gMaxWeeks+1]
                 ={12,14, 5, 3, 4, 2, 6,10,18,24,31,32,36,35,32,25,25,26,24,27,
                   27,28,31,33,30,33,32,31,25,18, 7, 4, 1, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={7,10,13,27,38,51,67,76,89,100};
        int  szGrowthStageDay[8] ={2,7,30,103,123,151,261,275};
        // mm growth per day for a Strong Male Sativa/Indica Plant
        char szNodeGrowth[8]     ={0,0,4,5,3,2,0,0};
        char szTipsGrowth[8]     ={0,3,14,12,5,2,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==28)
        { // Kali Mist- Female Sativa
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 3, 6, 6,10,15,24,34,38,42,46,48,49,52,53,55,57,59,60,61,62,
                   62,66,66,66,66,65,69,68,67,66,68,64,59,58,52,44,37,27,20, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 0, 2, 5, 7, 8,10,10,15,16,19,25,27,30,28,26,18,
                   21,23,27,31,34,38,38,40,40,36,37,35,36,34,36,33,32,31,32,28};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 4, 4, 7,11,12,19,24,31,36,38,42,48,52,59,59,51,35,24,
                   35,46,47,61,57,54,51,48,44,42,44,42,39,35,37,34,29,28,25,22};
        char szFertWeeks[gMaxWeeks+1]
                 ={13,12, 5, 2, 1, 2, 2, 9,16,23,30,31,34,36,29,24,24,21,24,25,
                   23,28,28,31,29,31,28,28,21,15, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={1,7,19,23,32,43,62,69,78,87};
        int  szGrowthStageDay[8] ={2,8,35,127,142,218,246,280};
        // mm growth per day for a good female Sativa Plant
        char szNodeGrowth[8]     ={0,0,6,10,6,3,1,0};
        char szTipsGrowth[8]     ={0,4,18,13,8,5,1,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, TRUE, FALSE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    if(iSeed==29)
        { // Domina Haze - Male Indica
        char szHealthWeeks[gMaxWeeks+1]
                 ={ 0, 3, 4, 8,13,19,29,37,43,43,45,46,47,48,50,55,56,57,58,59,
                   61,62,63,64,64,65,65,64,64,63,62,61,56,52,47,41,31,22,16, 0};
        char szPotencyWeeks[gMaxWeeks+1]
                 ={ 0, 1, 2, 4, 7, 7, 8, 8, 8, 9,15,20,23,24,26,25,21,18,20,27,
                   30,32,33,35,35,34,33,32,33,33,32,31,29,27,27,27,27,25,27,24};
        char szWaterWeeks[gMaxWeeks+1]
                 ={ 0, 0, 0, 1, 6,11,19,22,32,35,40,44,46,47,55,43,35,31,41,46,
                   52,55,58,47,42,39,37,35,33,26,24,22,23,22,19,18,18,17,17,14};
        char szFertWeeks[gMaxWeeks+2]
                 ={13,12, 4, 4, 0, 0, 4, 9,16,24,27,31,33,34,29,22,21,24,25,25,
                   26,27,28,29,29,30,30,26,23,15, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0};
        char szSplitNodes[10]    ={1,8,15,30,37,46,53,67,76,91};
        int  szGrowthStageDay[8] ={2,7,34,106,119,154,266,280};
        // mm growth per day for a weak male Indica Plant
        char szNodeGrowth[8]     ={0,0,4,7,4,2,1,0};
        char szTipsGrowth[8]     ={0,4,16,9,5,4,3,0};
        // now we call the function that fills our PLANTSEED array
        PSCopySeed(PPS_Plant, FALSE, TRUE,
                   szHealthWeeks, szPotencyWeeks,   szWaterWeeks, szFertWeeks,
                   szSplitNodes,  szGrowthStageDay, szNodeGrowth, szTipsGrowth);
        return;
        }

    }

// --------------------------------------------------------------------------------
//  THE SEED INITIALIZATION API FUNCTION
// --------------------------------------------------------------------------------


void PSInitPlantSeeds(int iPlant, int iSeed)
    {
    PSInitSeed(&PS_Plant[iPlant], iSeed);
    }

// --------------------------------------------------------------------------------
//  COMBINING (BREEDING) TWO SEEDS TOGETHER
// --------------------------------------------------------------------------------


BOOL PSCombinePlantSeeds(int iSeedMale, int iSeedFemale, PPLANTSEED PPS_Plant)
    {
    int i;
    BOOL bFemale;
    PLANTSEED PS_BreedSeed[2];   // holds info for combining two plants

    // first initialize our male and female seeds
    PSInitSeed(&PS_BreedSeed[0], iSeedMale);
    PSInitSeed(&PS_BreedSeed[1], iSeedFemale);

    // here we combine the genetic characteristics of the two seeds
    // we give the seed a 60% chance of becoming female
    i = Random(0,90);
    if(i<30) bFemale = FALSE;
    else     bFemale = TRUE;
    PPS_Plant->bSeedGender = bFemale; 
    PPS_Plant->bSeedIndica = PS_BreedSeed[1].bSeedIndica; // True = Indica

    for(i=0;i<gMaxWeeks;i++)
        PPS_Plant->szHealthWeeks[i]   = (PS_BreedSeed[0].szHealthWeeks[i]+
                                         PS_BreedSeed[1].szHealthWeeks[i])/2;
    for(i=0;i<gMaxWeeks;i++)
        PPS_Plant->szPotencyWeeks[i]  = (PS_BreedSeed[0].szPotencyWeeks[i]+
                                         PS_BreedSeed[1].szPotencyWeeks[i])/2;
    for(i=0;i<gMaxWeeks;i++)
        PPS_Plant->szWaterWeeks[i]    = (PS_BreedSeed[0].szWaterWeeks[i]+
                                         PS_BreedSeed[1].szWaterWeeks[i])/2;
    for(i=0;i<gMaxWeeks;i++)
        PPS_Plant->szFertWeeks[i]     = (PS_BreedSeed[0].szFertWeeks[i]+
                                         PS_BreedSeed[1].szFertWeeks[i])/2;
    for(i=0;i<10;i++)
        PPS_Plant->szSplitNodes[i]    = (PS_BreedSeed[0].szSplitNodes[i]+
                                         PS_BreedSeed[1].szSplitNodes[i])/2;
    for(i=0;i<8;i++)
        PPS_Plant->szGrowthStageDay[i]= (PS_BreedSeed[0].szGrowthStageDay[i]+
                                         PS_BreedSeed[1].szGrowthStageDay[i])/2;
    for(i=0;i<8;i++)
        PPS_Plant->szNodeGrowth[i]    = (PS_BreedSeed[0].szNodeGrowth[i]+
                                         PS_BreedSeed[1].szNodeGrowth[i])/2;
    for(i=0;i<8;i++)
        PPS_Plant->szTipsGrowth[i]    = (PS_BreedSeed[0].szTipsGrowth[i]+
                                         PS_BreedSeed[1].szTipsGrowth[i])/2;
    return TRUE ;
    }

