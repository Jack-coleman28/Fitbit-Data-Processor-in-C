#include "fitbit.h"

/*********************************************************************************
** Company: WSU - CPTS 122
** Programmer: Jack Coleman
**
** Create Date: 01//12/2019
** Date Last Modified: 01/23/2019
** Design Name: Programming Assignment 1 - Analyzing Fitbit Data
** Project Name: PA01_JSC_SPR2019
** Description:
** Revision:
** Additional Comments: Command window print out available in header file. See: CMD. 
**
**********************************************************************************/
int main(int argc, char *argv[])
{
	FILE *infile = NULL, *outfile = NULL;

	errno_t err_in, err_out;
	FitbitData minute[TOTALMINUTES + 10]; //Array with added surplus 
		
	unsigned int totalSteps = 0;
	unsigned int totalFloors = 0;
	unsigned int averageHeartrate = 0; 
	unsigned int maximumStep = 0;

	int startPoorsleep = 0; 
	int endPoorsleep = 0; 
	int totalSleepCycle = 0; 
	int commandWindow = CMD;
	double totalCalories = 0.0;
	double totalDistance = 0.0;

	unsigned int *floorPtr = NULL; 
	unsigned int *stepPtr = NULL;
	double *caloriesPtr = NULL;
	double *distancePtr = NULL;

	floorPtr = &totalFloors;
	stepPtr = &totalSteps;
	(double *)caloriesPtr = &totalCalories;
	(double *)distancePtr = &totalDistance;

	unsigned int *averagePtr = NULL;
	averagePtr = &averageHeartrate;

	unsigned int *maxStep = NULL;
	maxStep = &maximumStep;
	int *startMin = NULL; 
	startMin = &startPoorsleep;
	int *endMin = NULL; 
	endMin = &endPoorsleep; 
	int *totalPoorSleep = NULL;
	totalPoorSleep = &totalSleepCycle;

	err_in = fopen_s(&infile, "InitialData.csv", "r+");
	err_out = fopen_s(&outfile, "Results.csv", "w+");

	consoleWindowSelect(commandWindow); //FOR PRINTED CONSOLE LINES 

	fileErrorCheck(outfile, err_in, err_out, commandWindow); //FILE OPEN ERROR CHECKER

	resetData(minute); //RESET STRUCT FIELDS

	readInData(infile, minute); //STORES DATA FROM FIEL IN STRUCT

	calcTotals(minute, floorPtr, stepPtr, caloriesPtr, distancePtr); //CALCULATES TOTALS FROM STRUCT FIELDS

	calcAverage(minute, averagePtr); //CALCULATE THE AVERAGE HEARTRATE 

	calcMaxStep(minute, maxStep, startMin, endMin, totalPoorSleep); //CALCULATE MAXIMUM STEPS TAKEN 

	findPoorSleep(minute, startMin, endMin, totalPoorSleep); //FIND THE LONGEST POOR SLEEP PERIOD 

	printToFile(minute, outfile, caloriesPtr, distancePtr, floorPtr, stepPtr, averagePtr, startMin, endMin); //PRINT END RESULTS AND CLEANED FILE 

	err_in = fclose(infile); 
	err_out = fclose(outfile); 

	return 0;
}
