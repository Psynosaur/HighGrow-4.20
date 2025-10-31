#include <windows.h>
#include <stdio.h>
#include "global.h"
#include "calc.h"
#include "nodecalc.h"
#include "growroom.h"
#include "plantmem.h"
#include "leaves.h"

/******************************************************************************\
*  NODECALC.C - Calculating new node growth
\******************************************************************************/

/******************************************************************************\
*  CALCULATING TOTAL NUMBER OF NODES GROWN TO DATE
\******************************************************************************/


int NOCalcNumNodes(PNODE noPlant)
    { // add up how many nodes we have
    int i;     // total number of nodes
    BOOL bTrue=FALSE; 
    // first we calculate how many nodes we have so far
    for(i=0;(i<gMaxNodes && bTrue==FALSE);i++)
        { // if no length, then it's not a valid node
        if(noPlant[i].cNodeLength==0)
            bTrue=TRUE;
        }  
    return i-1;
    }

/******************************************************************************\
*  CALCULATING NODES GROWTH AND ADJUSTMENTS AND RETURNS TOTAL NUMBER OF NODES
\******************************************************************************/

// 0 = Seed           1 = Germination   2 = Seedling   3 = Vegetative
// 4 = Preflowering   5 = Flowering     6 = Seed Set   7 = Dying


BOOL NOAddNewNode(PNODE noPlant, int iPlant, int iNewNode, int iPrevNode, int iAngle)
    { // *** this is the main node adding function ***
    // ** here we add a node based on the previous node's angle **
    int  i;  // for our loops
    int  iWeek  = min(giGrowDay/7, gMaxWeeks); // current week

    // first set predeccessor node, growing tip and angle elements
    noPlant[iNewNode].iStartNode     = iPrevNode; // predeccessor node
    noPlant[iNewNode].bGrowingTip    = TRUE;  // new nodes are always tips
    noPlant[iNewNode].cNodeAngle     = iAngle;  // 0 = straight up
    
    // now check if this is one of our naturally splitting nodes

    noPlant[iNewNode].bNodeSplit = FALSE; // set the default to false
    for(i=0;(i<10 && noPlant[iNewNode].bNodeSplit==FALSE);i++) // for all elements
        { // check if we may split this week
        if(PS_Plant[iPlant].szSplitNodes[i]==iPrevNode)
            { // if we find it, set to true and stop loop
            noPlant[iNewNode].bNodeSplit = TRUE; 
            }
        }

    // now check how long this node continues growing strongly
    // we calculate this based on the current grow week, as
    // the earlier nodes grow longer before developing a new node
    noPlant[iNewNode].cNextNodeDays  = min(5, max(2, (40-iWeek)/8));
    
    // here we set some other rather obvious struct elements
    noPlant[iNewNode].cNodeLength    = 1;     // each node starts off 1mm long
    noPlant[iNewNode].cNodeLeafType  = 0;  // we set this during leaf growth calcs

    // finally we unset our previous node's growing tip status
    if(iNewNode != iPrevNode) // if this is not node zero
        {
        noPlant[iPrevNode].bGrowingTip = FALSE;  // new nodes are always tips
        noPlant[iPrevNode].bNodeSplit  = FALSE;  // old nodes don't split again
        }

    // and return the obvious true
    return TRUE;
    }



int NOCalcNodeGrowth(PPLANT plPlant, int iPlant)
    {
    int  iWeek    = min(giGrowDay/7, gMaxWeeks); // current week
    int  iHealth  = PS_Plant[iPlant].szHealthWeeks[iWeek]; // ideal health
    int  iNodes   = 0;     // total number of nodes
    int  i,j,k    = 0;     // temp counter for loops
    int  iAngle   = 0;     // from -90 to +90 degrees
    int  iNeg, iPos;         // used to hold angles
    PNODE noPlant = plPlant->NO_Plant; // set pointer to our node struct
    int  iNextNode= 0;
    BOOL bPruned  = FALSE;  // half growth if he pruned in the past three days
    int iRoom = (int)(giCurPlant-1)/3; // room number for this plant (OB0)
    // ------
    // getout if we're still a seed
    if(giGrowthStage==0) return 0;
    // now calculate how many we already have
    iNodes = NOCalcNumNodes(noPlant);

    // if we've already visited today, return the calculated nodes
    if(plPlant->GD_Plant[giGrowDay].bVisited==TRUE) 
        return iNodes;

    // if plant is dead, we return without adding new nodes or growth
    if(giGrowthStage==7) 
        return iNodes;

    // if we have no nodes yet, create the first one and getout
    if(iNodes==0)
        { // add the first node at angle of 0 degrees (straight up)
        NOAddNewNode(noPlant, iPlant, 0, 0, 0);
        return 1;
        }

    // if we've reached our limit, don't create any new nodes
    if(iNodes>=gMaxNodes) 
        return iNodes;
    
    // now we check if we must add any new nodes
    iNextNode = iNodes; // we increment iNodes within our loop
    for(i=0;i<iNodes;i++)
        { // we'll only check the growing tips here
        if(noPlant[i].bGrowingTip == TRUE)
            { // check if this node must get a successor yet
            if(noPlant[i].cNextNodeDays == 0) 
                { // as iNodes is OB1, we add new node at this array offset
                if(noPlant[i].bNodeSplit == TRUE) // if this node must split
                    { // maximum angle is 45 degrees
                    // calculate an angle based on current growth week
                    iAngle = max(3, 60-iWeek); // less angle as growth progresses
                    // * now adjust the angle based on the light height
                    //   the higher the light, the smaller the angle
                    iAngle = max(3, (iAngle-plPlant->GD_Plant[giGrowDay].cLightHeight));
                    // * nodes on outside of plant must be angled more upwards,
                    //   so we must decrease their angle some more.
                    iNeg = 0-iAngle; // negative angle
                    iPos = iAngle;   // positive angle
                    // if the predecessor node is to the left,
                    if(noPlant[i].cNodeAngle<0)
                        iNeg = min(-3, iNeg+10);
                    // if the predecessor node is to the right,
                    if(noPlant[i].cNodeAngle>0)
                        iPos = max(3, iPos-10);
                    // we add one node in one direction
                    NOAddNewNode(noPlant, iPlant, iNextNode,   i, iNeg); // add a node
                    NOAddNewNode(noPlant, iPlant, iNextNode+1, i, iPos); // add another 
                    iNextNode += 2; // we added two nodes this time
                    }
                else  // we add a node based on the previous node's angle
                    { // we adjust the angle by 5 degrees until 0 (straight up)
                    iAngle = noPlant[i].cNodeAngle; // makes our calcs easier
                    if(iAngle<0) iAngle = min(iAngle+7, -3); // increase if negative
                    if(iAngle>0) iAngle = max(iAngle-7, 3);  // else decrease angle
                    // now we add that node at the calculated new angle
                    NOAddNewNode(noPlant, iPlant, iNextNode, i, iAngle); 
                    iNextNode += 1; // we only added one node now
                    }
                }
            else // no successor yet, simply adjust days count
                noPlant[i].cNextNodeDays = max(0, noPlant[i].cNextNodeDays-1);
            }
        }
    // finally, we set our variable
    iNodes = iNextNode;


    // if we're vegetating, we add some random nodes with random nextnodedays
    if(giGrowthStage==4)
        { // each day stands a 5% chance of getting a new node
        i = Random(0, 100);
        if(i<5 && iNodes>20)
            { // now we choose a random node and random angle
            j = Random(8, iNodes-1);
            k = Random(5, 15);
            // and we add a new one based on this node
            iAngle = noPlant[j].cNodeAngle; // makes our calcs easier
            if(iAngle<0) iAngle = max(iAngle-k, -60); // decrease if negative
            if(iAngle>0) iAngle = min(iAngle+k, 60);  // else increase angle
            // now we add that node at the calculated new angle
            NOAddNewNode(noPlant, iPlant, iNodes, j, iAngle); 
            iNodes += 1; // we only added one node now
            }
        }

    // if we're preflowering, we add some random nodes with random nextnodedays
    if(giGrowthStage==5)
        { // each day stands a 10% chance of getting a new node
        i = Random(0, 100);
        if(i<10 && iNodes>20)
            { // now we choose a random node
            j = Random(30, iNodes-1);
            k = Random(10, 30);
            // and we add a new one based on this node
            iAngle = noPlant[j].cNodeAngle; // makes our calcs easier
            if(iAngle<0) iAngle = max(iAngle-k, -60); // decrease if negative
            if(iAngle>0) iAngle = min(iAngle+k, 60);  // else increase angle
            // now we add that node at the calculated new angle
            NOAddNewNode(noPlant, iPlant, iNodes, j, iAngle); 
            iNodes += 1; // we only added one node now
            }
        }

    // now we add growth to the tips and normal nodes based on the current
    // growth stage and pattern of this plant

    // first check if he pruned within the last 3 days, we'll have half growth
    bPruned = FALSE; // assume he didn't prune
    for(j=giGrowDay-1;j>=max(0, giGrowDay-3);j=j-1) // check each day
        { // if he pruned, we divide today's growth by two
        if(plPlant->GD_Plant[j].cTipsPruned > 0)
             bPruned = TRUE;
        }

    for(i=0;i<iNodes;i++)
         { 
         // first check if this is a tip
         if(noPlant[i].bGrowingTip == TRUE)
             { // our tips get more growth than the rest
             k = PS_Plant[iPlant].szTipsGrowth[giGrowthStage];
             }
         else // we add the normal amount to all non-tip nodes
             {
             k = PS_Plant[iPlant].szNodeGrowth[giGrowthStage];
             }
         // now reduce growth if he has weak lights
         if((giGrowthStage>2)&&(giGrowthStage<5))
             {
             k = max(0, k-(3 - GR_Room[iRoom].iLampStrength));
             }
         // if he pruned, we divide day's growth by 2
         if(bPruned) k = k/2; 
         // now we can add the required amount of growth to this node
         noPlant[i].cNodeLength = min(100, noPlant[i].cNodeLength + k);
         }

    // now we add growth for the amount of health he's accumulated
    if((giGrowthStage>2)&&(giGrowthStage<5))
        {
        j = max(0, giHealth-iHealth); // get the % extra health for today
        for(i=0;i<iNodes;i++) // for all growing nodes
            { // if he's created enough health, and node is within limits
            if((noPlant[i].bGrowingTip==TRUE)&&(j>5)) 
                noPlant[i].cNodeLength= min(100, (j/5 + noPlant[i].cNodeLength));
            }
        }

    // **** ZERO NEXT NODE'S LENGTH NOW TO OVERCOME 16DAY BUG ****
    noPlant[iNodes].cNodeLength = 0;                
    return iNodes;
    }

/******************************************************************************\
*  CALCULATES THE NUMBER OF GROWING TIPS FOR THIS GROWDAY
\******************************************************************************/


int NOCalcNumTips(PNODE noPlant)
    {
    int i, iTips=0;
    for(i=0;i<giGrowingNodes;i++)
        {
        if(noPlant[i].bGrowingTip == TRUE)
            iTips += 1;
        }
    return iTips;
    }


/******************************************************************************\
*  CALCULATES AND RETURNS THE CURRENT PLANT HEIGHT
\******************************************************************************/

DRAWPLANT DP_MeasurePlant;    // for calculating height (in mm) of our plants

int NOCalcPlantHeight(PNODE noPlant)
    {
    // ** Here we calculate the Plant's height, by filling out a DrawNode 
    // structure for all the plant's node's and then checking which node
    // is the tallest one. 
    // This function returns the plant's current height in mm
    int iHeight=0;     // added to and returned later
    int i;             // used for our loops
    // ** first we calculate our DrawNode array for all the nodes    
    GRCalcDrawNodes(noPlant, &DP_MeasurePlant, 2, CDN_MM); // calc plant 1 in mm
    // ** now we check which node of our plant has the tallest tip
    iHeight = DP_MeasurePlant.DN_Plant[0].ptDNStart.y;
    for(i=0;i<giGrowingNodes;i++)
        iHeight = min(iHeight, DP_MeasurePlant.DN_Plant[i].ptDNEnd.y);
    // subtract from start Y position to calculate plant's new height
    iHeight = DP_MeasurePlant.DN_Plant[0].ptDNStart.y-iHeight;
    // ** finally, return the plant's height
    return iHeight;
    }

/******************************************************************************\
*  CALCULATES AND RETURNS THE CURRENT PLANT WEIGHT IN GRAMS
\******************************************************************************/


int NOCalcPlantMass(PNODE noPlant)
    {
    int i;
    double dMass=0;
    // add the mass of the leaves
    for(i=0;i<giGrowingNodes;i++)
        {
        if((noPlant[i].cNodeLeafType > 0)&&
           (noPlant[i].cNodeLeafType <= 10))
            {
            switch(noPlant[i].cNodeLeafType)
                {
                case  1: 
                case  2: dMass += 0.2; break; // tips weigh 0.1 grams each
                case  3: 
                case  4: dMass += 0.6; break; // small leaves weigh 0.6 grams each
                case  5: 
                case  6: dMass += 0.7; break; // medium leaves weigh 0.7 grams each
                case  7: 
                case  8: dMass += 0.8; break; // big leaves weight 0.8 grams each
                case  9: 
                case 10: dMass += 0.9; break; // extra large leaves weigh 0.9 grams 
                }
            } dMass += ((int)noPlant[i].cNodeLength*0.01); // 0.01 grams per millimeter stem
        }  
    return (int)dMass;
    }


/******************************************************************************\
*  CALCULATION THE PRUNED MASS
\******************************************************************************/


double NOCalcPrunedMass(PPLANT plPlant)
    {
    int i, j=0;
    
    for(i=0;i<giGrowDay;i++) 
        j += plPlant->GD_Plant[i].cTipsPruned;

    return (j*0.4);
    }


/******************************************************************************\
*  THE DAILY NODE GROWTH CALCULATION FUNCTION
\******************************************************************************/


void NOCalculateNodes(PPLANT plPlant, int iPlant)
    { // *** FILE MUST BE OPEN AND IN MEMORY ***
    // note: giGrowDay and all other giGlobals MUST be set first
    // *** IF WE HAVEN'T VISITED TODAY, CALC NEW NODES ***
    giGrowingNodes = NOCalcNodeGrowth(plPlant, iPlant);
    // *** CALCULATE AVAILABLE GROWING TIPS ***
    giGrowingTips  = NOCalcNumTips(plPlant->NO_Plant);
    // *** CALCULATE PLANT HEIGHT (mm) ***
    giHeight       = NOCalcPlantHeight(plPlant->NO_Plant);
    // *** CALCULATE LEAF GROWTH ***
    LFCalcLeafGrowth(plPlant, giGrowingNodes, giGrowthStage);
    // *** CALCULATE PLANT WEIGHT (grams) ***
    giGrowingMass  = NOCalcPlantMass(plPlant->NO_Plant);
    // *** CALCULATE THE PRUNED MASS ****
    gdPrunedMass   = NOCalcPrunedMass(plPlant);
    }


/******************************************************************************\
*  RECALCULATING NODE GROWTH FROM DAY 1
\******************************************************************************/


void NOReCalculateNodes(PPLANT plPlant, int iPlant)
    { // *** FILE MUST BE OPEN AND IN MEMORY ***
    // Recalculates all nodes since growday 1
    int i;
    int iDay = giGrowDay; // hold a copy, we're gonna change it
    
    // first reset all lengths, indicating no node
    for(i=0;i<gMaxNodes;i++)
        {
        plPlant->NO_Plant[i].cNodeLength   = 0; // set length to zero
        plPlant->NO_Plant[i].cNodeLeafType = 0; // to recalc new leaves
        }

    // now we recalculate all the nodes for each day's growth
    for(i=0;i<=iDay;i++)
        {
        giGrowDay = i; // set this global variable
        GLLoadGrowthVariables(plPlant); // load this day's growth
        NOCalculateNodes(plPlant, iPlant); // now calculate the nodes
        GLSaveGrowthVariables(plPlant); // save this day's height etc
        }
    giGrowDay = iDay; // and reset our global
    }


/******************************************************************************\
*  PRUNING ONE GROWING TIP
\******************************************************************************/

BOOL NOPruneTip(PNODE noPlant, int iCurNode, int iMaxNodes)
    { 
    // **** iCurNode MUST BE OB0 ****
    // here we prune the given node, which MUST be a tip
    PNODE pStartMem; // used to copy memory when removing a node

    // first check if this is a true tip
    if(noPlant[iCurNode].bGrowingTip != TRUE) return FALSE;
    // ok, so it's a tip, and we may continue....
    // to remove a tip, we must first set it's length to zero
    noPlant[iCurNode].cNodeLength = 0;
    // now we set it's start node to a growing tip
    noPlant[noPlant[iCurNode].iStartNode].bGrowingTip   = TRUE;
    // we must also set it's start node to a splitting type
    noPlant[noPlant[iCurNode].iStartNode].bNodeSplit    = TRUE;
    // we'll also set it's start node for 2 days growth before splitting
    noPlant[noPlant[iCurNode].iStartNode].cNextNodeDays = 2;
    // we don't have to zero or change any other node struct elements...
    // ... but now that we've pruned our tips, we must delete the nodes 
    // from our NODE array in global memory
    pStartMem = noPlant+iCurNode+1; // set startmove pointer
    // and move one more node than required to blank out previous node
    MoveMemory((pStartMem-1),pStartMem,(iMaxNodes-iCurNode+1)*sizeof(NODE));

    // ...so now we can finally return our true
    return TRUE;
    }


/******************************************************************************\
*  PRUNE A NUMBER OF GROWING TIPS
\******************************************************************************/

BOOL NOPruneRandomTips(PPLANT plPlant, int iPot, int iPlant, int iNumTips)
    {
    // here we prune a number of growing tips from this plant
    // *** WE MAY NEVER PRUNE MORE THAN 100 TIPS AT ANY TIME ***
    PRUNETIP  PT_Plant[gMaxTipNodes];    // allow space for 100 tips
    int i, j, k;
    int iTips, iNodes, iTallest, iTHeight, iFound;
    // -------------------------------------------

    // first we'll calculate our own local number of tips and nodes in this plant
    iTips  = min(gMaxTipNodes, NOCalcNumTips(plPlant->NO_Plant));
    iNodes = NOCalcNumNodes(plPlant->NO_Plant);

    // now we'll just check if something went wrong with things
    if(iNumTips>iTips) return FALSE; // getout if he prunes more than we have

    j = 0; // offset into our PRUNETIP struct array
    for(i=0;(i<iNodes && j<gMaxTipNodes);i++)
        { // first we must fill our PRUNETIP structure for all tips
        if(plPlant->NO_Plant[i].bGrowingTip == TRUE)
            { // if the node is a tip, we'll add it to our array
            PT_Plant[j].iCurNode = i;    // "i" is our current node
            // check into DRAWPLANT array for our node's end Y position
            PT_Plant[j].iHeight  = gPlantStartY-DP_Plant[iPot].DN_Plant[i].ptDNEnd.y;
            // now increment our PRUNETIP struct array pointer
            j += 1;
            }
        }

    // Now we should have "j" tips in our PRUNETIP struct array
    // We must now select the tallest tips to prune.
    for(i=0;i<iNumTips;i++)
        { // first we must find our tallest tip
        iTallest = 0;   // zero this now so that we can search again
        iTHeight = 0;   // zero this tallest tip variable
        for(k=0;k<j;k++)
            { // check all tips in the PRUNETIP array and find the tallest
            if((PT_Plant[k].iHeight)>iTHeight) // tallest so far?
                { // this is the tallest so far, so set our vars
                iTallest = PT_Plant[k].iCurNode; // set tallest tip's node
                iTHeight = PT_Plant[k].iHeight;  // and it's height
                iFound   = k;
                }
            }
        // now that we're sure of the tallest tip, we can prune it
        NOPruneTip(plPlant->NO_Plant, iTallest, iNodes);
        // and we must remove it from our PRUNETIP array, so that we
        // don't find it again when looking for the next tallest tip
        PT_Plant[iFound].iHeight = 0;
        // because we removed 1 node, any tip with a higher node number
        // found further in our PRUNETIP array should be reduced by one
        for(k=iFound+1;k<iNumTips;k++)
            { // check from our this pruned node onwards
            if(PT_Plant[k].iCurNode > iTallest)
                PT_Plant[k].iCurNode -= 1;
            }
        }

    // OK, now that we've pruned all those tips, we must recalculate our 
    // drawplant structure again for this plant.
    GRInitDrawPlant(plPlant, iPot, TRUE);

    // everything went smoothly, so return our true
    return TRUE;
    }

/******************************************************************************\
*  PRUNE ONE TIP BY CLICKING WITH THE MOUSE
\******************************************************************************/


BOOL NOMousePruneTip(PPLANT plPlant, int iPot, int iTipNode)
    {
    int iTips;
    // check exit condition
    if(iTipNode==0) return FALSE;
    // first we must calculate how many tips we have
    iTips  = min(gMaxTipNodes, NOCalcNumTips(plPlant->NO_Plant));
    // getout if we somehow found none or less than the pruned one
    if((iTips==0)||(iTips < 0)) return FALSE;
    // now we may prune this tip
    if(NOPruneTip(plPlant->NO_Plant, iTipNode, giGrowingNodes))
        { // set our global growday states
        giLastPruned  = giGrowDay; // we pruned today
        giPrunedTips += 1;         // we pruned one more tip
        // re-initialize our drawplant info again
        GRInitDrawPlant(plPlant, iPot, TRUE);
        return TRUE;
        }
    return FALSE;
    }


BOOL NOMousePrune(int iPlant, int iPot, int iTipNode)
    { // prune the OB0 plant's OB0 node if it's a tip
    char szFile[128]="\0";
    BOOL bSuccess=FALSE;

    giCurPlant = iPlant+1;
    sprintf(szFile, "Plant%02i.hgp", iPlant+1);
    if(GLDoesFileExist(szFile))
        {
        if(PMPlantFileToMem(iPlant))
            { // first lock our global memory
            P_Plant = (PPLANT)GlobalLock(hPlantMem);
            // now calculate to fill our current growday variables
            CACalculatePlant(P_Plant, iPlant);
            // let's prune that tip then
            bSuccess  = NOMousePruneTip(P_Plant, iPot, iTipNode);
            // save our new current growday structs
            GLSaveGrowthVariables(P_Plant);
            // and then free our memory again
            PMPlantMemToFile(iPlant);
            }
        }
    return bSuccess;
    }
 
