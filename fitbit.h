#ifndef FITBIT_H
#define FITBIT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TOTALMINUTES 1446
#define MAXIDLEN 20
#define LINEMAX 75
#define BADDATA 9999
#define CMD 0 //toggle when to see command window print out ----- 0 = OFF/1 = ON

typedef enum sleep
{	
	NONE, ASLEEP, AWAKE, REALLYAWAKE 
} Sleep;

typedef struct fitbit
{
	char patient[MAXIDLEN];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;

void readInData(FILE *infile, FitbitData minute[]);
void calcTotals(FitbitData minute[], unsigned int *floorPtr, unsigned int *stepPtr, double *caloriesPtr, double *distancePtr);
void calcAverage(FitbitData minute[], unsigned int *averagePtr);
void calcMaxStep(FitbitData minute[], unsigned int *maxStep); 
void findPoorSleep(FitbitData minute[], int *startMin, int *endMin, int *totalPoorSleep);
void printToFile(FitbitData minute[], FILE *outfile, double *caloriesPtr, double *distancePtr, unsigned int *floorPtr, unsigned int *stepPtr, unsigned int *averagePtr, int *startMin, int *endMin); 
void consoleWindowSelect(int commandWindow);
void fileErrorCheck(FILE *outfile, errno_t err_in, errno_t err_out, int commandWindow);

#endif
