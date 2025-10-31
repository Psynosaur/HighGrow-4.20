#include <windows.h> 
#include "stdio.h"
#include "global.h"
#include "calc.h"
#include "plantmem.h"
#include "password.h"
#include "growroom.h"
#include "growedit.h"
#include "toolbar.h"
#include "nodecalc.h"

#include "SeedList.h"
                                    
/******************************************************************************\
*
*  CALC.C   - all the growth calculations functions
*
*  ** NB ** - these functions may only be called by "CAPreCalculatePlant" as
*             they must all be called in the correct order to calculate one day.
*
\******************************************************************************/

/******************************************************************************\
*  TEMPORARY VALUES OF CALCULATED INTEGERS
\******************************************************************************/

int giCurPlant          =1; // current plant for any operations
int giGrowDay           =1; // ** done **
int giLastVisited       =0; // ** done **
int giLastWatered       =0; // ** done **
int giLastPruned        =0; // ** done **
int giLastFertilized    =0; // ** done **
int giGrowthStage       =0; // ** done **
int giPotency           =0; // ** done **
int giMoisture          =0; // ** done **
int giLightOn           =0; // ** done **
int giLightOff          =0; // ** done **
int giLightHeight       =0; // ** done **
int giSoilpH            =0; // ** done **
int giHealth            =0; // ** done **
int giHeight            =0; // ** done **
int giGrowingNodes      =0; // ** done **
int giGrowingTips       =0; // ** done **
int giGrowingMass       =0; // ** done **
int giPrunedTips        =0; // ** done **
double gdPrunedMass     =0; // ** done **
int giCurPlantPotOfs    =0; // ** done **

/******************************************************************************\
*  GENERAL SOIL TYPE AND POT SIZE CALCULATIONS
\******************************************************************************/

int CACalcSoilMoistureUsage(void)
    { // calcs daily increase in moisture usage based on pot size and soil type
//    int iRoom = TBGetComboGrowroom()-1;
    int iRoom = (giCurPlant-1)/3;
    int iSizeUsage = 0;
    int iTypeUsage = 0;
    // --- calculate the moisture usage based on the size of the pot
    iSizeUsage = (120 - (GEGetCurRoomPotSize(iRoom)/10000));
    // now subtract some for previous version's average pot's volume
    iSizeUsage -= 70;
    // --- calculate the moisture usage based on the type of soil
    iTypeUsage = 100; // set the default
    switch(GR_Room[iRoom].iSoilType)
        {
        case 0:
            { // Loamy soil
            iTypeUsage = 100;
            break;
            }
        case 1:
            { // Chalky soil
            iTypeUsage = 90;
            break;
            }
        case 2:
            { // Peaty soil
            iTypeUsage = 30;
            break;
            }
        case 3:
            { // Clay/Silty soil
            iTypeUsage = 10;
            break;
            }
        case 4:
            { // Sandy/Stony soil
            iTypeUsage = 60;
            break;
            }
        }
    // now subtract some for previous version's loamy soil type drainage
    iTypeUsage -= 100;
    // return the sum of these two usage values
    return iSizeUsage + iTypeUsage;
    }



int CACalcSoilNutrientUsage(void)
    { // calcs daily increase in nutrient requirement based on pot size and soil type
//    int iRoom = TBGetComboGrowroom()-1;
    int iRoom = (giCurPlant-1)/3;
    int iSizeUsage = 0;
    int iTypeUsage = 0;
//    char sztext[100];
    // --- calculate the nutrient usage based on the size of the pot
    iSizeUsage = (GEGetCurRoomPotSize(iRoom)/10000);
    // now subtract some for previous version's average pot's volume
    iSizeUsage -= 20;
    // --- calculate the moisture usage based on the type of soil
    iTypeUsage = 0; // set the default
    switch(GR_Room[iRoom].iSoilType)
        {
        case 0:
            { // Loamy soil is perfect
            iTypeUsage = 0;
            break;
            }
        case 1:
            { // Chalky soil needs some more nutrients
            iTypeUsage = 3;
            break;
            }
        case 2:
            { // Peaty soil has way too many nutrients
            iTypeUsage = -10;
            break;
            }
        case 3:
            { // Clay/Silty soil has a little too many nutrients
            iTypeUsage = -3;
            break;
            }
        case 4:
            { // Sandy/Stony soil needs more nutrients
            iTypeUsage = 5;
            break;
            }
        }
//    wsprintf(sztext, "iRoom=%i, iSizeUsage=%i, iTypeUsage=%i", iRoom,iSizeUsage,iTypeUsage);
//    Blow(sztext);
    // return the sum of these two usage values
    return iSizeUsage + iTypeUsage;
    }



/******************************************************************************\
*  CALCULATING THE OFFSET FOR THE POT HEIGHT FOR THE CURRENT PLANT
\******************************************************************************/
// this is done because all the health etc calculations for the lights must take
// into account that the selected growroom pot may be higher or lower than the
// original HighGrow pots. ie. We don't want to burn a plant in a low pot, when 
// the lamp is just above the tips

int CACalcPlantPotOffset(int iPlant)
    { // iPlant is from 1 to giMaxPlants
    int iOfs  = 0;
    int iRoom = 0;
    // getout if the given plant is invalid
    if((iPlant <= 0)||(iPlant > gMaxPlants)) 
        return 0;
    // now check which growroom we're in
    iRoom = 1 + (int)((iPlant-1)/3);
    // now get the offset for this pot
    iOfs = GRGetPlantOffset(iRoom);
    return iOfs;
    }
 
/******************************************************************************\
*  CALCULATING THE CURRENT GROWROOM TEMPERATURE
\******************************************************************************/

int giRoomTemp=20; // current temperature (as calculated every 15 minutes


int CACalcPlantTemperature(PPLANT plPlant)
    {
    int iQStart = plPlant->GD_Plant[giGrowDay].cLightOn;
    int iQEnd   = plPlant->GD_Plant[giGrowDay].cLightOff;
    int iQNow   = GLGetTimeNowQuarters();
    int iTemp   = 0; // current calculated temperature
    // ----------
    // check if his switch-off time is after his switch-on time
    if(iQStart <= iQEnd)
         { // check how long this plant's light has been on
         if((iQStart<iQNow)&&(iQEnd>iQNow)) // is the light currently on?
              { // temp can be raised 5 deg/plant/day (48 timeslices (15mins))
              iTemp += (((iQNow-iQStart)*5)/96);
              }
         else { // if lamp is off, calc increase for time it was on
              iTemp += (((iQEnd-iQStart)*5)/96);
              iTemp -= (((iQNow-iQEnd)*5)/96); // losing heat since switchoff
              }
         }
    else {
         if((iQStart<iQNow)||(iQEnd>iQNow)) // is the light currently on?
              { // temp can be raised 5 deg/plant/day (48 timeslices (15mins))
              iTemp += (((iQNow-iQStart)*5)/96);
              }
         else { // if lamp is off, calc increase for time it was on
              iTemp += ((((96-iQStart)+iQEnd)*5)/96);
              iTemp -= (((iQNow-iQEnd)*5)/96); // losing heat since switchoff
              }
         }
    // now return our temperature
    return iTemp; 
    }

int CACalcRoomTemperature(void)
    {
    int iQNow = GLGetTimeNowQuarters();
    int iTemp = 0; // current calculated temperature
    int iRoom = TBGetComboGrowroom()-1;
    // ------
    // reset the minimum temperature
    iTemp  = 20; 
    // set the temperature for this time of the day
    if(iQNow<60) // if it's before three o'clock...
         { // temp is raised by 10 degrees till about three pm
         iTemp += ((iQNow*10)/60); 
         }
    else { // and lowered by 10 degrees in the evening
         iTemp += 10; // for the morning's increase
         iTemp -= (((iQNow-60)*10)/36); // losing heat since five pm
         }
    // now we lower the temperature if he's using weak lights
    iTemp -= (4 - GR_Room[iRoom].iLampStrength);
    // return the current temperature
    return iTemp;
    }


int CAReCalcGrowroomTemperature(void)
    { // here we perform the temperature calculation for all plants
    int i, iTemp = 0;
    char szFile[256] ="\0";
    int iRoomOfs = (TBGetComboGrowroom()-1)*3;
    // now we add the temperature of the growroom based on the time of day
    iTemp = CACalcRoomTemperature();
    // now we add the temperature as is influenced by each plant
    for(i=0;i<3;i++)
        {
        giCurPlant = i+1; // stupid global
        sprintf(szFile, "Plant%02i.hgp", i+1+iRoomOfs);
        if(GLDoesFileExist(szFile))
            {
            if(PMPlantFileToMem(i+iRoomOfs))
                { // first lock our global memory
                P_Plant = (PPLANT)GlobalLock(hPlantMem);
                // now we can calculate the temperature
                iTemp += CACalcPlantTemperature((PPLANT)P_Plant);
                // we can unlock & free our memory again
                GlobalUnlock(hPlantMem);
                PMPlantMemoryFree();
                }
            }
        }
    iTemp = max(15,min(40,iTemp));
    return iTemp;
    }

/******************************************************************************\
*  CALCULATING THE CURRENT GROWROOM HUMIDITY
\******************************************************************************/

int giHumidity=0; // current humidity (as calculated every 15 minutes

int CACalcRoomHumidity(void)
    { // *** MUST BE CALLED AFTER TEMPERATURE CALCULATIONS ***
    int iDewPoint=0; // current dewpoint temperature
    int iFactor=0;   // subtracted from room temp to get dew point temp
    // an array of values for Saturated Vapour Pressure for our humidity calc
    int szSatVapPrs[27]={166,177,194,210,223,237,250,265,280,296,314,332,350,
                         380,410,434,457,481,508,535,562,593,625,656,691,727,0};
    // now we'll use our current temperature to generate our dew point
    // temperature (between 1 and 4 degrees lower than room temp)
    // the higher the temperature above 20 degrees, the lower the difference
    iFactor = giMoisture; // use the current moisture content to start with
    while(iFactor>8) iFactor = iFactor/2; // now reduce it until <4
    iFactor = max(2, iFactor);
    // use this factor to calculate dew point temp
    iDewPoint = giRoomTemp - iFactor;
    // now we can calculate relative humidity
    giHumidity = ((szSatVapPrs[iDewPoint -15]*10)/
                  (szSatVapPrs[giRoomTemp-15]/10));
    giHumidity = min(100, max(36, giHumidity));
    return giHumidity;
    }                                             

/******************************************************************************\
*  CALCULATING THE GROWDAY, BASED ON THE PLANT'S START DATE
\******************************************************************************/

int CACalcGrowDay(PPLANTINFO piPlant)
    {
    char szDateNow[3];
    GLDateNow((char*)szDateNow);
    return GLDiffDate((char*)&piPlant->szStartDate, (char*)&szDateNow)+1;
    }

/******************************************************************************\
*  CALCULATING THE LAST VISTED DAY#, AS AN OFFSET FROM THE START DATE
\******************************************************************************/

int CACalcLastVisitDay(PPLANT plPlant)
    {
    int i;

    for(i=giGrowDay-1;i>=0;i=i-1)
        if(plPlant->GD_Plant[i].bVisited) return i;
    return 0;
    }

/******************************************************************************\
*  CALCULATING THE LAST WATERED DAY#, AS AN OFFSET FROM THE START DATE
\******************************************************************************/

int CACalcLastWaterDay(PPLANT plPlant)
    {
    int i;
    for(i=giGrowDay;i>=0;i=i-1)
        if(plPlant->GD_Plant[i].bWatered) return i;
    return 0;
    }

/******************************************************************************\
*  CALCULATING THE LAST FERTILIZED DAY#, AS AN OFFSET FROM THE START DATE
\******************************************************************************/

int CACalcLastFertDay(PPLANT plPlant)
    {
    int i;
    for(i=giGrowDay;i>=0;i=i-1)
        if(plPlant->GD_Plant[i].bFertilized) return i;
    return 0;
    }

/******************************************************************************\
*  CALCULATING THE LAST PRUNED DAY#, AS AN OFFSET FROM THE START DATE
\******************************************************************************/

int CACalcLastPrunedDay(PPLANT plPlant)
    {
    int i;
    for(i=giGrowDay;i>=0;i=i-1)
        if(plPlant->GD_Plant[i].cTipsPruned>0) return i;
    return 0;
    }

/******************************************************************************\
*  CALCULATING THE GROWTH STAGE AS AN INT FROM 0 TO 7
\******************************************************************************/

// 0 = Seed           1 = Germination   2 = Seedling   3 = Vegetative
// 4 = Preflowering   5 = Flowering     6 = Seed Set   7 = Dying

int CACheckGrowthForLight(PPLANT plPlant, int iGrowthStage)
    { 
    int i, j;
    int iStage = iGrowthStage;
    BOOL bIsTrue;
    int iRoom = (giCurPlant-1)/3;
    int iOfs  = iRoom*3;
    // ** if plant is vegetating and the light has been on for
    //    less than 12 hours per day for 2 weeks, we force preflowering.
    if(iStage==3)
        {
        bIsTrue=TRUE; // assume less than 12 hours for 2 weeks
        for(i=giGrowDay-1;i>=max(0, giGrowDay-14);i=i-1)
            { // calculate the photoperiod
            j= GLGrowDayPhotoperiod(plPlant, i);
            if(j>12*4) // if more than 12 hours light, set false
                bIsTrue = FALSE;
            }
        if(bIsTrue==TRUE) // we're now preflowering?
            iStage = 4;
        }
    // ** if plant is now preflowering or flowering and the light is on
    //    for more than 14 hours any day for 2 weeks, we continue vegetating
    if((iStage==4)|(iStage==5))
        {
        bIsTrue=FALSE; // assume less than 14 hours for 2 weeks
        for(i=giGrowDay-1;i>=max(0, giGrowDay-14);i=i-1)
            { // calculate the photoperiod
            j= GLGrowDayPhotoperiod(plPlant, i);
            if(j>14*4) // if more than 14 hours light, set true
                bIsTrue = TRUE;
            }
        if(bIsTrue==TRUE) // we're vegetating again?
            return 3;
        }
    // ** if plant is now preflowering and the light has been on for
    //    less than 12 hours per day for 21 days, we force flowering.
    if(iStage==4)
        {
        bIsTrue=TRUE; // assume less than 12 hours for 21 days
        for(i=giGrowDay-1;i>=max(0, giGrowDay-21);i=i-1)
            { // calculate the photoperiod
            j= GLGrowDayPhotoperiod(plPlant, i);
            if(j>12*4) // if more than 12 hours light, set false
                bIsTrue = FALSE;
            }
        if(bIsTrue==TRUE) // we're now flowering?
            iStage = 5; // set our variable and continue
        }
    // ** if plant is preflowering or flowering and the light is increased to
    //    more than 14 hours per day for 2 days, we continue vegetating
    if((iStage==4)|(iStage==5))
        {
        bIsTrue=FALSE; // assume less than 16 hours for 2 days
        for(i=giGrowDay-1;i>=max(0, giGrowDay-2);i=i-1)
            { // calculate the photoperiod
            j= GLGrowDayPhotoperiod(plPlant, i);
            if(j>14*4) // if more than 14 hours light, set true
                bIsTrue = TRUE;
            }
        if(bIsTrue==TRUE) // we're vegetating again?
            return 3;
        }
    // ** if this plant has been flowering for more than a week, or
    //    less than 14 hours per day for three weeks, we'll start to 
    //    check for our seed set conditions
    if(iStage==5)
        {
        bIsTrue=FALSE; // assume less than 14 hours for 3 weeks
        for(i=giGrowDay-1;i>=max(0, giGrowDay-21);i=i-1)
            { // calculate the photoperiod
            j= GLGrowDayPhotoperiod(plPlant, i);
            if(j>14*4) // if more than 14 hours light, set true
                bIsTrue = TRUE;
            }
        if(bIsTrue==FALSE) // we're still false?
           iStage = 6; // then we can check for seed set
        }
    // ** if this plant is a male, it may not start any seed set part 
    //    of the growth cycle. Females only do seed set if a male is around
    if(iStage==6) // are we now going into the seed set?
        { 
        bIsTrue = FALSE; // assume no males
        for(i=0;i<3;i++)
            { // check for a male, and if found we set our flag
            if(PS_Plant[i+iOfs].bSeedGender==FALSE)  
                { // now check if this male is flowering
                if(PI_PlantSeed[i+iOfs].cGrowthStage>=5)
                    bIsTrue = TRUE; // found a valid male
                }
            }
        if(PS_Plant[giCurPlant-1].bSeedGender) // if this plant is a female
            { // if no males around, stay with flowering stage, else seed set
            if(bIsTrue == FALSE) return 5;
            }
        else return 7; // males cannot go into seed set, so set dying stage
        }
    // ** no changes, simply return the current growth stage
    return iStage;
    }


int CACalcGrowthStage(PPLANT plPlant)
    { // define the day ending values for the (0 to 7) growth stages 
    int iStage=0;
    // * first check if the plant was still alive yesterday.
    //   if it isn't then we the DEAD growthstage
    if(giGrowDay>7)
        {
        if(plPlant->GD_Plant[giGrowDay-1].cHealth==0)
            return 7; // plant is dead
        }
    // we check the day value for all the 8 stages
    for(iStage=0;iStage<8;iStage++)
        { 
        if(PS_Plant[giCurPlant-1].szGrowthStageDay[iStage]>=giGrowDay)
            {
            return CACheckGrowthForLight(plPlant, iStage);
            }
        }
 
    return 7; // if we didn't succeed, the plant must be dead
    }

/******************************************************************************\
*  CALCULATING THE PLANT POTENCY BASED ON IT'S CURRENT WEEK NUMBER
\******************************************************************************/

int CACalcPotency(PPLANT plPlant)
    { // *** NOTE: giHealth Must be calculated before calling this ***
    // As potency increases and decreases during the different growth
    // stages, we must base the general potency on the growth stage.
    // This function multiplies the number by ten, so that we can still
    // slightly alter the potency value based on health and water conditions
    int  iPotency=0;  // working copy of current potency
    int  iWeek  = min(giGrowDay/7, gMaxWeeks); // current week
    int  iDOW   = giGrowDay-(((int)giGrowDay/7)*7); // day of this grow week
    int  iHealth= PS_Plant[giCurPlant-1].szHealthWeeks[iWeek]; // ideal health

    // * first check if the plant was still alive yesterday.
    //   if it isn't then we return yesterday's potency less 13
    if(plPlant->GD_Plant[giGrowDay-1].cHealth==0)
        return max(0, (plPlant->GD_Plant[giGrowDay-1].iPotency - 13));

    // * now calculate the ideal potency for today
    //   x10 allows us to /100 later & get 3.24 etc
    iPotency  = (PS_Plant[giCurPlant-1].szPotencyWeeks[iWeek] * 10);
    iPotency += ((((PS_Plant[giCurPlant-1].szPotencyWeeks[iWeek+1]
                  - PS_Plant[giCurPlant-1].szPotencyWeeks[iWeek])*10)*iDOW)/7);

    // now add some potency based on health he's added himself
    iPotency += max(0, (giHealth - iHealth));
    // and finally return our calculated daily potency
    return iPotency;
    }

/******************************************************************************\
*  CALCULATING THE SOIL'S MOISTURE BASED ON GROWTHSTAGE, LIGHT ETC.
\******************************************************************************/

// 0 = Seed           1 = Germination   2 = Seedling   3 = Vegetative
// 4 = Preflowering   5 = Flowering     6 = Seed Set   7 = Dying

int CACalcMoisture(PPLANT plPlant)
    { 
    // * here we calculate and return today's moisture level for this plant.
    //   this moisture value is in millileters, and will not exceed 1000
    //   the day's usage depends on the plant's growth stage, the height of
    //   the light above the plant, the light strength and the photoperiod
    int i;       // for general use
    int iWeek = min(giGrowDay/7, gMaxWeeks);
    int iMoisture=0; // moisture usage count in ml
    int iRoom  = (int)(giCurPlant-1)/3; // room number for this plant (OB0)

    // * first check if the plant was still alive yesterday.
    //   if it isn't then we return yesterday's moisture less 17
    if(plPlant->GD_Plant[giGrowDay-1].cHealth==0)
        return max(0, (plPlant->GD_Plant[giGrowDay-1].iMoisture - 17));

    // * then check day's moisture usage rating based on the current
    //   growth stage
    iMoisture = (PS_Plant[giCurPlant-1].szWaterWeeks[iWeek] * 4);

    // * now add some (ml) for distance the plant is away from the light.
    //   we will use yesterday's plant and light height to calculate this
    //   value for today.
    // first establish the distance between plant and light
    i = (((plPlant->GD_Plant[giGrowDay-1].cLightHeight)*125)
         - plPlant->GD_Plant[giGrowDay-1].iHeight+giCurPlantPotOfs);
    // if the light is closer than 1m away, we'll use more water
    if(i<=1000) // we use an extra 10 ml for every 10cm closer than 1m
        iMoisture += ((10-(i/100))*10);

    // * now add some (ml) usage based on yesterday's photoperiod.
    // first calculate the photoperiod in units of 15 minutes (max=96)
    i = GLGrowDayPhotoperiod(plPlant, giGrowDay-1);
    // if the photoperiod is less than 12 hours (48 units), don't worry
    if(i>=48) // we'll use an extra 2 ml for every unit above 48
        iMoisture += ((i-48)*2);

    // * now, we add some (ml) if he watered yesterday, as this would
    //   cause some extra surface evaporation or bottom leakage.
    if(plPlant->GD_Plant[giGrowDay-1].bWatered)
        iMoisture += 10;

    // * now, we add some (ml) for the strength of the lights he 
    //   is using which would cause more (or less) transpiration.
    iMoisture += (4 - GR_Room[iRoom].iLampStrength)*5;

    // * now, we add some (ml) based on the amount of soil in the pot
    // NOW add some for this pot size
    iMoisture += CACalcSoilMoistureUsage();

    // * now we calculate the new moisture based on yesterday's level
    //   and the current water usage.
     iMoisture = max(0,((plPlant->GD_Plant[giGrowDay-1].iMoisture)-iMoisture));

    // and finally we return today's moisture level
    return iMoisture;
    }


/******************************************************************************\
*  CALCULATING THE SOIL'S PH BALANCE BASED ON THE FERTILIZING FREQUENCY
\******************************************************************************/


int CACalcSoilPH(PPLANT plPlant)
    { 
    // * Here we calculate the soil's PH balance based on the ideal fertilizer
    //   requirements of the plant in it's various growth stages. We start of
    //   with an ideal pH of 7.0 and we lower that based on how much NPK he
    //   added more than the ideal amount. We also raise it again for the amount
    //   of Calcium he added to adjust it. The Maximum pH we will return is
    //   12 (120/10) and the minimum is 3 (30/10).
    int  i,j;           // for general use
    int  iNPK=0, iCa=0; // to total nutrients
    int  iPH=70;        // ideal ph balance we will adjust and return
    int  iWeek = min(giGrowDay/7, gMaxWeeks);      // current grow week
    int  iDOW  = giGrowDay-(((int)giGrowDay/7)*7); // day of this grow week
    int  iRequd= 0;

    // * first check if the plant was still alive yesterday.
    //   if it isn't then we return yesterday's pH
    if(plPlant->GD_Plant[giGrowDay-1].cHealth==0)
        return (plPlant->GD_Plant[giGrowDay-1].cSoilPH);

    // add the extra amount required for this day of the week
    iRequd =  PS_Plant[giCurPlant-1].szFertWeeks[iWeek]; 
    iRequd += (((PS_Plant[giCurPlant-1].szFertWeeks[iWeek+1]
               - PS_Plant[giCurPlant-1].szFertWeeks[iWeek])*iDOW)/7);

    // * Now we adjust our NPK requirement for the pot size and soil type
    iRequd += CACalcSoilNutrientUsage();
    // check that we haven't gone negative
    iRequd = max(0, iRequd);

    // * First we add up all the NPK added in the last 7 days
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        {
        for(j=0;j<3;j++) // add NP and K added
            iNPK += plPlant->GD_Plant[i].szNutrients[j];
        // also add up the Calcium added
        iCa += plPlant->GD_Plant[i].szNutrients[3];
        }  

    // * Now we LOWER the pH if he's over the ideal amount
    iPH -= max(0, ((iNPK - iRequd)/6));
    // * We will RAISE it slightly if he hasn't fertilized enough
    if(iNPK<=(iRequd/2))
       iPH +=(iRequd/6);
    // * We also RAISE it for the Calcium he added
    iPH += min(30, (iCa/2));
 
    // * finally we ensure it's within our limits     
    iPH  = max(30, min(120, iPH));

    return iPH;
    }


/******************************************************************************\
*  CALCULATING THE PLANT'S HEALTH BASED ON ALL THE KNOWN GROWTH FACTORS
\******************************************************************************/


int CACalcHealth(PPLANT plPlant)
    { 
    // * Here we calculate the plant's health for the day, based on all the
    //   factors we have calculated so far, and a few others that will influence
    //   the plant's growth (registration etc).
    int  i, j, k=0;    // for general use
    BOOL bIsTrue;      // used to check conditions
    int  iWeek  = min(giGrowDay/7, gMaxWeeks); // current week
    int  iDOW   = giGrowDay-(((int)giGrowDay/7)*7); // day of this grow week
    int  iHealth= PS_Plant[giCurPlant-1].szHealthWeeks[iWeek]; // ideal health
    int  iRoom  = (int)(giCurPlant-1)/3; // room number for this plant (OB0)

    // * we add the health accumulated for this day of the current growweek
    iHealth += (((PS_Plant[giCurPlant-1].szHealthWeeks[iWeek+1] 
                - PS_Plant[giCurPlant-1].szHealthWeeks[iWeek])*iDOW)/7);

    // * The iHealth can be maximum of 75% health, so we can use the remaining
    //   25% to increase the health based on all our factors.

    // ** FIRST WE CHECK OUR DEATH CONDITIONS **
    // -----------------------------------------
    // * If he's reached the cutoff for an unregistered version
    if(gbRegistered)
         i = 300; // 300 days for the registered version
    else i = 60;  // but only 60 days for shareware users
    if((plPlant->PI_Plant.bRegistered==FALSE)&&(giGrowDay>=i))
        return 0;

    // * if the plant has no moisture for 14 days, forget it
    bIsTrue = TRUE; // assume he hasn't watered enough
    for(i=giGrowDay-1;i>=max(0, giGrowDay-14);i=i-1)
        { // if he has watered, unset our true flag
        if(plPlant->GD_Plant[i].iMoisture>0)
            bIsTrue = FALSE;
        } // plant is dead if our flag is still true
    if(bIsTrue) 
        return 0;

    // * if it's received too much fertilizer in one week, it dies
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        {
        for(j=0;j<4;j++) // add all nutrient components
            k += plPlant->GD_Plant[i].szNutrients[j];
        } // if he gave 30 mg each day for 7 days
    if(k>=210)
        return 0;

    // ** THEN WE INCREASE HEALTH **
    // -----------------------------
    // * add 5% if this plant is registered
    if(plPlant->PI_Plant.bRegistered) 
        iHealth += 3;

    // * add 3% if he visited every day this week
    bIsTrue = TRUE; // assume he's visited every day
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        { // if he hasn't visited, unset our true flag
        if(plPlant->GD_Plant[i].bVisited==FALSE)
            bIsTrue = FALSE;
        } // now increase health if still true
    if(bIsTrue) 
        iHealth += 3;

    // * add 1% for every 2 hours photoperiod is above 12 hours
    i = GLGrowDayPhotoperiod(plPlant, giGrowDay-1);
    // if the photoperiod is less than 12 hours (48 units), don't worry
    if(i>=48) // we'll add 1% for every 4 units
        iHealth += ((i-48)/8);

    // * add 1% for every 20cm light is closer than 1m away from plant
    // first establish the distance between plant and light
    i = (((plPlant->GD_Plant[giGrowDay-1].cLightHeight)*125)
         - plPlant->GD_Plant[giGrowDay-1].iHeight+giCurPlantPotOfs);
    // only if the light is closer than 1m away, we add health
    if(i<=1000) // add an extra 1% for every 20cm closer than 1m
        iHealth += (10-(i/200));

    // * add 1% if this plant is a female (!!!SEXIST!!!)
    if(PS_Plant[giCurPlant-1].bSeedGender == TRUE) 
       iHealth += 1;

    // * add 2% if yesterday's pH was between 6 and 8
    i = plPlant->GD_Plant[giGrowDay-1].cSoilPH/10;
    if(i>=6 && i<=8)
        iHealth += 2;

    // * add 2 if he managed to get THC above 4.0% (ie 400)
    if(plPlant->GD_Plant[giGrowDay-1].iPotency>=400);
        iHealth += 2;

    // * add 3 if he fertilized this week
    bIsTrue = FALSE; // assume he hasn't fertilized once
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        { // if he fertilized, set our flag
        if(plPlant->GD_Plant[i].bFertilized==TRUE)
            bIsTrue = TRUE;
        } // now increase health if still true
    if(bIsTrue) 
        iHealth += 3;

    // * add 1 if the growroom music is enabled
    if(PACheckSoundEnabled()==TRUE)
        iHealth += 1;

    // * add 1 if he's using the correct kind of light at the right time
    // if we're before pre-flowering, add health if he's using MH lights
    if(giGrowthStage<4)
        {
        if(GR_Room[iRoom].iLampType == 0)
            iHealth += 1;
        }
    // if pre-flowering, flowering, or seed set add health if using HPS lights
    if((giGrowthStage>=4)&&(giGrowthStage<=6))
        {
        if(GR_Room[iRoom].iLampType == 1)
            iHealth += 1;
        }

    // ** NOW WE DECREASE HEALTH **
    // ----------------------------
    // * he loses 15% health if the light is touching the plant
    if((plPlant->GD_Plant[giGrowDay-1].cLightHeight*125)
        <= plPlant->GD_Plant[giGrowDay-1].iHeight+giCurPlantPotOfs)
        iHealth -= 15;

    // * he loses 1% for every day he didn't visit this week
    j = 0; // reset our counter
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        { // if he hasn't visited, increase our counter
        if(plPlant->GD_Plant[i].bVisited==FALSE)
            j += 1;
        } // now decrease health by this value
        iHealth -= j;

    // * he loses 1% for every day without moisture this week
    j = 0; // reset our counter
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        { // if he hasn't visited, increase our counter
        if(plPlant->GD_Plant[i].iMoisture==0)
            j += 1;
        } // now decrease health by this value
        iHealth -= j;

    // * he loses 3% if pH is below 6 or above 8
    i = plPlant->GD_Plant[giGrowDay-1].cSoilPH/10;
    if(i<6 || i>8)
        iHealth -= 3;

    // * he loses 5% if he didn't water in past 4 days
    bIsTrue = TRUE; // assume he didn't water once
    for(i=giGrowDay-1;i>=max(0, giGrowDay-4);i=i-1)
        { // if he hasn't visited, unset our true flag
        if(plPlant->GD_Plant[i].bWatered==TRUE)
            bIsTrue = FALSE;
        } // now decrease health if flag is still true
    if(bIsTrue) 
        iHealth -= 5;

    // * he loses 7% if he fertilized without water in past 7 days
    bIsTrue = FALSE; // assume he didn't cock-up
    for(i=giGrowDay-1;i>=max(0, giGrowDay-7);i=i-1)
        { // if he fertilized without watering, set our true flag
        if((plPlant->GD_Plant[i].bFertilized)
            &&(!plPlant->GD_Plant[i].bWatered))
            bIsTrue = TRUE;
        } // now decrease health if flag is true
    if(bIsTrue) 
        iHealth -= 7;

    // *** NOW RETURN OUR CALCULATED VALUE ***
    return max(0, min(100, iHealth));
    }

/******************************************************************************\
*  VACATION MODE CALCULATIONS START HERE
\******************************************************************************/

void CACalcVacationChanges(PPLANT plPlant)
    {
    // NOTE: We must ALWAYS load the growroom struct BEFORE calling this function.
    // Before calculating the growth variables, we calculate the vacation mode
    // first check if the vacation mode has been enabled
    int i, j, iRoom = 0;
    int iHeight, iHOfs = 0;
    int iWDays, iWml = 0;
    int iPWaters = 0;
    BOOL bWatered = FALSE;
//    char sztext[100];
    // do nothing if this plant is now dead
    if(giGrowthStage>=7) return;
    // do nothing if the vacation mode is not enabled
    if(PACheckVacModeEnabled())
        {
        // if vacation mode is enabled, we'll do the calculations
        // check which growroom this plant is growing in
        iRoom = (int)((giCurPlant-1)/3);
        // now check if this growroom requires lighting adjustment
        if(GR_Room[iRoom].bVacLights)
            { // 
            iHeight = plPlant->GD_Plant[giGrowDay-1].iHeight+giCurPlantPotOfs;
            iHOfs   = GR_Room[iRoom].iVacLightMM;
            // now set light height to the required adjustment
            giLightHeight = (iHeight+iHOfs)/125;
            } 
        // now check if this growroom requires watering
        if(GR_Room[iRoom].bVacWater)
            {
            iWDays = giGrowDay - giLastWatered;
            iWml   = GR_Room[iRoom].iVacWaterML;
            // if we're on (or beyond) the day to auto-water
            if(iWDays >= GR_Room[iRoom].iVacWaterDays)
                { // add the required amount of water
                plPlant->GD_Plant[giGrowDay].bWatered = TRUE;
                giLastWatered = giGrowDay;
                giMoisture += iWml;
                bWatered = TRUE;
                }
            }
        // now check if this growroom requires fertilizing
        if(GR_Room[iRoom].bVacFert)
            { // since fertilizing is based on when he last watered...
            // ...there's no point in checking unless we watered today
            if(bWatered)
                { // first calculate the number of watering since last fertilized
                j = 1; // init this for safety
                for(i=giGrowDay;i>0;i-=1)
                    { // if plant has more water today than yesterday
                    if(plPlant->GD_Plant[i].iMoisture > plPlant->GD_Plant[i-1].iMoisture)
                        { // first we increment our day count
                        // now we check if this exceeds the required number
                        if(j >= GR_Room[iRoom].iVacFertWaters)
                            { // if it does, we apply the nutrients
                            giLastFertilized = giGrowDay;
                            plPlant->GD_Plant[giGrowDay].bFertilized     = TRUE;
                            plPlant->GD_Plant[giGrowDay].szNutrients[0] += GR_Room[iRoom].iVacFertN;
                            plPlant->GD_Plant[giGrowDay].szNutrients[1] += GR_Room[iRoom].iVacFertP;
                            plPlant->GD_Plant[giGrowDay].szNutrients[2] += GR_Room[iRoom].iVacFertK;
                            plPlant->GD_Plant[giGrowDay].szNutrients[3] += GR_Room[iRoom].iVacFertCa;
                            // set the loop counter to zero to kick out now
//                        wsprintf(sztext, "Plant=%i, Room=%i, Growday=%i",  giCurPlant, iRoom, i);
//                        Blow(sztext);
                            i = 0;
                            }
                        j += 1;
                        }
                    // kick-out of the loop if we fertilized anywhere along the way
                    if(plPlant->GD_Plant[i].bFertilized == TRUE)  i = 0;
                    }        
                } 
            }
        }
    }


/******************************************************************************\
*  MAIN CALCULATION FUNCTION, RETURNS TRUE IF PLANT IS GROWING AND CALCULATED
\******************************************************************************/


void CACalcGrowthVariables(PPLANT plPlant)
    { 
    // *** Calculates growth factors based on day before "giGrowDay" ***
    // * Note: the "giGrowDay" and "giLastVisited" variables must 
    //          be set before calling this function.
    // * Note: the "CACalcVacationChanges()" function must be called before this
    // *** CALCULATE THE CURRENT GROWTH STAGE ***
    giGrowthStage    = CACalcGrowthStage(plPlant);
    // *** SET TODAY'S LIGHT HEIGHT & PHOTOPERIOD ***
    giLightOn        = plPlant->GD_Plant[giGrowDay-1].cLightOn;
    giLightOff       = plPlant->GD_Plant[giGrowDay-1].cLightOff;
    giLightHeight    = plPlant->GD_Plant[giGrowDay-1].cLightHeight;
    // *** CALCULATE THE CURRENT MOISTURE ***
    giMoisture       = CACalcMoisture(plPlant);
    // *** CALCULATE THE SOIL PH BALANCE ***
    giSoilpH         = CACalcSoilPH(plPlant);
    // *** IF PLANT STILL ALIVE, CALC THE REST
    if(giGrowthStage<7)
        {
        // *** CALCULATE THE PLANT'S HEALTH ***
        giHealth     = CACalcHealth(plPlant);
        // *** AFTER HEALTH, CALCULATE CURRENT POTENCY ***
        giPotency    = CACalcPotency(plPlant);
        }
    else
        {
        giHealth      = 0;
        giPotency     = 0;
        giLightHeight = 20;
        }
    }


void CAPreCalculateStatistics(PPLANT plPlant)
    {
    // *** CALCULATE THE LAST VISITED DAY ***
    giLastVisited    = CACalcLastVisitDay(plPlant);
    // *** CALCULATE THE LAST WATERED DAY ***
    giLastWatered    = CACalcLastWaterDay(plPlant);
    // *** CALCULATE THE LAST FERTILIZED DAY ***
    giLastFertilized = CACalcLastFertDay(plPlant);
    // *** CALCULATE THE LAST PRUNED DAY ***
    giLastPruned     = CACalcLastPrunedDay(plPlant);
    }


BOOL CACalculatePlant(PPLANT plPlant, int iPlant)
    { // before we do anything with this plant, we must calculate
    // and initialize the growth for all the days since his last
    // visit. The main idea is to arrive at a %health for each day
    // which will determine how much to grow each tip by.
    int i, iTemp; // for our loop
    giCurPlant = iPlant + 1;
    // *** IF THE PLANT ISN'T GROWING YET, GETOUT ***
    if(plPlant->PI_Plant.szStartDate[0]==0) return FALSE;
    // *** MUST CALCULATE THE GROWDAY FIRST ***
    giGrowDay   = CACalcGrowDay(&plPlant->PI_Plant);
    // *** EXIT IF SOMETHING IS WRONG ***
    if(!giGrowDay) return FALSE;
    // *** HOLD THE CURRENT GROWDAY IN A TEMP VARIABLE ***
    iTemp = giGrowDay;
    // calculate the last visit day etc
    CAPreCalculateStatistics(plPlant);
    // CALC THE OFFSET BETWEEN THE STANDARD AND SELECTED POTS
    giCurPlantPotOfs = CACalcPlantPotOffset(giCurPlant);
    // if we've already calc'ed today, only recalc health factors
//    if(plPlant->GD_Plant[giGrowDay].bVisited==FALSE) 
    if(plPlant->GD_Plant[giGrowDay].bVisited) 
        { // ** we must recalculate giHealth each time **
        GLLoadGrowthVariables(plPlant);
        NOCalculateNodes(plPlant, iPlant);
        GLSaveGrowthVariables(plPlant);
        // ******** TEMPORARY CODE *********
//        CACalcVacationChanges(plPlant);
        // *********************************
        return TRUE;
        }  
    // *** NOW WE CALCULATE ALL THE GROWTHDAYS ***
    for(i=giLastVisited+1;i<=iTemp;i++)
        {                
        giGrowDay=i;
        // now we calculate the plant's health, water, lights, etc
        CACalcGrowthVariables(plPlant);
        // here we save everything into our current growth variables
        GLSaveGrowthVariables(plPlant);
        // now we must do the vacation mode checks
        CACalcVacationChanges(plPlant);
        // here we calculate the new nodes for this plant
        NOCalculateNodes(plPlant, iPlant);
        // after calculating nodes, and if vacmode set, set the visited flaf
        if(PACheckVacModeEnabled())
            plPlant->GD_Plant[i].bVisited = TRUE;
        // finally we save averything
        GLSaveGrowthVariables(plPlant);
        }
    // our giGrowday should be correct, but reset it just in case
    giGrowDay     = iTemp;
    // set that we visited here today
    plPlant->GD_Plant[giGrowDay].bVisited = TRUE;
    // now return true
    return TRUE;
    }     



