
/******************************************************************************\
*
*  CALC.H - all the growth calculations functions
*
\******************************************************************************/

extern char PlantHealthWeeks[gMaxWeeks+1]; // Ideal health in % per week ending 

BOOL CACalculatePlant(PPLANT plPlant, int iPlant);

// temperature calculations
extern int giRoomTemp; // current temperature (as calculated every 15 minutes
extern int giHumidity; // current humidity (as calculated every 15 minutes

int CACalcPlantTemperature(PPLANT plPlant);
int CACalcRoomTemperature(void);
int CAReCalcGrowroomTemperature(void);
int CACalcRoomHumidity(void);
