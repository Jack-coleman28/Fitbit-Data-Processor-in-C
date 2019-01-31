#include "fitbit.h"

/************************************************************
* Function: void resetData(FitbitData minute[TOTALMINUTES])
* Date Created: January 12, 2019
* Date Last Modified: January 19, 2019
* Description: Resets all data in the minute struct
* Input parameters: FitbitData minute[TOTALMINUTES] -- [1446]
* Returns: -
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void resetData(FitbitData minute[TOTALMINUTES])
{
	for (int i = 0; i < TOTALMINUTES; i++)
	{
		strcpy_s(minute[i].patient, 8, "9:99:99");
		strcpy_s(minute[i].minute, 8, "9:99:99");
		minute[i].calories = 0.0;
		minute[i].distance = 0.0;
		minute[i].floors = 0;
		minute[i].heartRate = 0;
		minute[i].steps = 0;
		minute[i].sleepLevel = 0;
	}
	return;
}

/************************************************************
* Function: void consoleWindowSelect(int commandWindow)
* Date Created: January 22, 2019
* Date Last Modified: January 22, 2019
* Description: Allows the use of printing to the comand window for user. 
* Input parameters: int commandWindow
* Returns: -
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void consoleWindowSelect(int commandWindow)
{
	if (commandWindow == 1)
	{
		printf("You have selected to see console window printout.\n");
		printf("________________________________________________\n\n");
	}
	return;
}//END CONSOLE PRINTOUT ROUTINE 

/************************************************************
* Function: void fileErrorCheck(FILE *outfile, errno_t err_in, errno_t err_out, int commandWindow)
* Date Created: January 20, 2019
* Date Last Modified: January 20, 2019
* Description: Checks for errors in opening input/output files. 
* Input parameters: FILE *outfile, errno_t err_in, errno_t err_out, int commandWindow
* Returns: -
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void fileErrorCheck(FILE *outfile, errno_t err_in, errno_t err_out, int commandWindow)
{
	if (err_in != 0) //Infile was not successfully opened 
	{
		fprintf(outfile, "Input file was not opened successfully.\n");
		if (commandWindow == 1)
			printf("Input file was not opened successfully.\n");
	}
	else
	{
		fprintf(outfile, "Input file was opened successfully.\n");
		if (commandWindow == 1)
			printf("Input file was opened successfully.\n");
	}
	if (err_out != 0) //Outfile was not successfully opened 
	{
		fprintf(outfile, "Output file was not opened successfully.\n");
		if (commandWindow == 1)
			printf("Output file was not opened successfully.\n");
	}
	else
	{
		fprintf(outfile, "Output file was opened successfully.\n");
		if (commandWindow == 1)
			printf("Output file was opened successfully.\n");
	}
	return;
}

/************************************************************
* Function: void readInData(FILE *infile, FitbitData minute[TOTALMINUTES])
* Date Created: January 12, 2019
* Date Last Modified: January 22, 2019
* Description: Reading in data from input file and stores it in struct. Cleans any NULL or empty data fields. 
* Input parameters: FILE *infile, FitbitData minute[TOTALMINUTES]
* Returns: Void
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void readInData(FILE *infile, FitbitData minute[TOTALMINUTES])
{
	//Target: ,12cx7,,,,,, -- this is line 1 of input csv file
	char *token; 
	char *context = 0;
	char tempStr[LINEMAX] = "";
	//char emptyStr[LINEMAX] = "";
	char *emptyStr;
	emptyStr = (char*)malloc(MAXIDLEN);

	//char targetId[MAXIDLEN] = "";
	char *targetId; 
	targetId = (char*)malloc(MAXIDLEN);

	if (infile != EOF) //file was not empty
		fscanf_s(infile, "%s ,", tempStr, _countof(tempStr));
	else //file was empty and cannot continue
		return; 

	token = strtok_s(tempStr, ",", &context); // finds "TARGET:" first 
	token = strtok_s(NULL, ",", &context); //finds TARGET ID "*****" 

	strcpy_s(targetId, strlen(token) + 1, token); // copy the TARGET ID into ID string 

	//Patient, minute, calories, distance, floors, heart, steps, sleep_level -- this is line 2 of the input csv file 
	if (infile != EOF) //file was not empty
		fscanf_s(infile, "%s", tempStr, LINEMAX /* _countof(tempStr)*/);
	else //file was empty and cannot continue
		return;

	//while input file not at an end or the array is not full 
		//read in line as string until new line is reached
		//check patient id 
			//if correct - parse it and move on to next line
				//when parsing if any field has NULL of empty data fill it with -99s
			//if not correct - delete it and move onto next line (Pseudo built in)

	int index = -1; 

	while (index < TOTALMINUTES && &infile != EOF)
 	{
		fscanf_s(infile, "%s", tempStr, _countof(tempStr));

		token = strtok_s(tempStr, ",", &context); // get the first token -- patient id

		if (token != NULL && strcmp(token, targetId) == 0) //if strcmp = 0 the strings matched and we have the correct patient 
		{
			index++;
			strcpy_s(minute[index].patient, strlen(token) + 1, token);

			//parsing out minute
			if (context[0] == ',')
			{
				//strcpy_s(minute[index].minute, strlen(minute[index].minute) + 1, "9:99:99");
				strcpy_s(minute[index].minute, 8, "9:99:99");
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					strcpy_s(minute[index].minute, 8, "9:99:99");
				else
					strcpy_s(minute[index].minute, strlen(token) + 1, token);
			}
			//parsing out calories 
			if (context[0] == ',')
			{
				minute[index].calories = (double)BADDATA; //minute[index].calories = 25.0;
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					minute[index].calories = (double)BADDATA;
				else
					minute[index].calories = strtod(token, NULL);
			}
			//parsing out distance
			if (context[0] == ',')
			{
				minute[index].distance = (double)BADDATA;
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					minute[index].distance = (double)BADDATA;
				else
					minute[index].distance = strtod(token, NULL);
			}
			//parsing out floors
			if (context[0] == ',')
			{
				minute[index].floors = (unsigned int)BADDATA;
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					minute[index].floors = (unsigned int)BADDATA;
				else
					minute[index].floors = (unsigned int)atoi(token);
			}
			//parsing out heartRate
			if (context[0] == ',')
			{
				minute[index].heartRate = (unsigned int)BADDATA;
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					minute[index].heartRate = (unsigned int)BADDATA;
				else
					minute[index].heartRate = (unsigned int)atoi(token);
			}
			//parsing out steps 
			if (context[0] == ',')
			{
				minute[index].steps == (unsigned int)BADDATA;
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					minute[index].steps = (unsigned int)BADDATA;
				else
					minute[index].steps = (unsigned int)atoi(token);
			}
			//parsing out sleepLevel 
			/*if (context[0] == ',')
			{
				minute[index].sleepLevel == BADDATA;
				context++;
			}
			else
			{
				token = strtok_s(NULL, ",", &context);
				if (token == NULL)
					minute[index].sleepLevel == BADDATA;
				else
					minute[index].sleepLevel = atoi(token);
			}*/
			token = strtok_s(NULL, ",", &context);
			if (token == NULL)
				minute[index].sleepLevel = BADDATA;
			else
				minute[index].sleepLevel = atoi(token);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL) //if the tok is NULL or the tok is equal to an empty string
			//	minute[index].minute == BADDATA;
			//else 
			//	strcpy_s(minute[index].minute, strlen(minute[index].minute) + 1, token);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL)
			//	minute[index].calories == BADDATA;
			//else 
			//	minute[index].calories = strtod(token, NULL);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL)
			//	minute[index].distance == BADDATA;
			//else
			//	minute[index].distance = strtod(token, NULL);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL)
			//	minute[index].floors == BADDATA;
			//else
			//	minute[index].floors = (unsigned int)atoi(token);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL)
			//	minute[index].heartRate == BADDATA;
			//else
			//	minute[index].heartRate = (unsigned int)atoi(token);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL)
			//	minute[index].steps == BADDATA;
			//else
			//	minute[index].steps = (unsigned int)atoi(token);
			//token = strtok_s(NULL, ",", &context);
			//if (token == NULL)
			//	minute[index].sleepLevel == BADDATA;
			//else
			//	minute[index].sleepLevel = atoi(token);
		}
		//else 
		//index++; 
	}
	return;
}//END READ IN DATA

/************************************************************
* Function: void calcTotals(FitbitData minute[TOTALMINUTES], unsigned int *floorPtr, unsigned int *stepPtr, double *caloriesPtr, double *distancePtr)
* Date Created: January 18, 2019
* Date Last Modified: January 20, 2019
* Description: Calculates the totals from struct fields for presentation and calculation. 
* Input parameters: FitbitData minute[TOTALMINUTES], unsigned int *floorPtr, unsigned int *stepPtr, double *caloriesPtr, double *distancePtr
* Returns: Void 
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void calcTotals(FitbitData minute[TOTALMINUTES], unsigned int *floorPtr, unsigned int *stepPtr, double *caloriesPtr, double *distancePtr)
{
	//double totalCal = 0.0, totalDist = 0.0; 
	//unsigned int totalFls = 0, totalStp = 0; 

	for (int i = 0; i < TOTALMINUTES; i++)
	{
		if(minute[i].calories != BADDATA) //if the data has not been cleaned, sum it
			*caloriesPtr += minute[i].calories;
		if (minute[i].calories != BADDATA)
			*distancePtr += minute[i].distance; 
		if (minute[i].calories != BADDATA)
			*floorPtr += minute[i].floors; 
		if (minute[i].calories != BADDATA)
			*stepPtr += minute[i].steps;
	}
	return; 
}//END CALC TOTALS

/************************************************************
* Function: void calcAverage(FitbitData minute[TOTALMINUTES], unsigned int *averagePtr)
* Date Created: January 18, 2019
* Date Last Modified: January 20, 2019
* Description: Calculates the average heartrate of the patient over the course of the gathered data. 
* Input parameters: FitbitData minute[TOTALMINUTES], unsigned int *averagePtr
* Returns: Void 
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void calcAverage(FitbitData minute[TOTALMINUTES], unsigned int *averagePtr)
{
	//sum of minute[index value].average 
		//exclude cleaned data
	//sum divided by the 24 hour period (1440 minutes)

	for (int i = 0; i < TOTALMINUTES; i++)
	{
		if (minute[i].heartRate != BADDATA)
			*averagePtr += minute[i].heartRate;
	}

	*averagePtr = *averagePtr / TOTALMINUTES;

	return; 
}//END CALC AVERAGE HEARTRATE

/************************************************************
* Function: void calcMaxStep(FitbitData minute[TOTALMINUTES], unsigned int *maxStep)
* Date Created: January 18, 2019
* Date Last Modified: January 20, 2019
* Description: Finds the maximum steps taken in any one minute, from the total data gathered from the file. 
* Input parameters: FitbitData minute[TOTALMINUTES], unsigned int *maxStep
* Returns: Void 
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void calcMaxStep(FitbitData minute[TOTALMINUTES], unsigned int *maxStep)
{
	//set max steps to zero (done in main)
	//compare (0) maxstep against any others in minute[index value].steps
	//if the value in .step is not BADDATA(9999) 
		//if minute[index value].steps is larger then set it as max step 
			//repeat for the length of the struct 

	for (int i = 0; i < TOTALMINUTES; i++)
	{
		if (minute[i].steps != BADDATA)
		{
			if (minute[i].steps > *maxStep)
				*maxStep = minute[i].steps;
		}
	}
	return; 
}//END FIND MAX STEPS

/************************************************************
* Function: void findPoorSleep(FitbitData minute[TOTALMINUTES], int *startMin, int *endMin, int *totalPoorSleep)
* Date Created: January 18, 2019
* Date Last Modified: January 20, 2019
* Description: Finds the poor sleep period of the longest duration, from the total data gathered from the file. 
* Input parameters: FitbitData minute[TOTALMINUTES], int *startMin, int *endMin, int *totalPoorSleep
* Returns: Void 
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void findPoorSleep(FitbitData minute[TOTALMINUTES], int *startMin, int *endMin, int *totalPoorSleep)
{
	//Poor sleep is a .sleepLevel of 2 or 3 (AWAKE or REALLYAWAKE)

	int currentStartMin = -1, currentEndMin = -1, currentTotalTime = -1;
	int masterStartMin = -1, masterEndMin = -1, masterTotalTime = -1;

	//for the TOTALMINUTES
		//if minute[index value].minute != BADDATA 
			//if the sleeplevel is > ASLEEP			
				//if startMin = 0
					//startMin = current index 
				//totalPoorSleep += minute[index value].sleepLevel
			//else 
				//endMin = current index
			//if current set(local set) > master set(pointer set)
				//master set (pointer set) = current set (local set)
				//local set set to zero(reinitialized)

	for (int i = 0; i < TOTALMINUTES; i++)
	{
		if (minute[i].sleepLevel != BADDATA) //if (minute[i].minute != BADDATA || minute[i].sleepLevel != BADDATA)
		{
			if (minute[i].sleepLevel > ASLEEP)
			{
				if (currentStartMin == -1)
					currentStartMin = i;
				currentTotalTime += 1; // minute[i].sleepLevel; 
			}
			else if (currentStartMin > 0) //(minute[i].sleepLevel <= ASLEEP)
			{
				currentEndMin = (i - 1);
				if (currentTotalTime > masterTotalTime)
				{
					masterTotalTime = currentTotalTime;
					masterStartMin = currentStartMin;
					masterEndMin = currentEndMin;
				}
				currentEndMin = -1;
				currentStartMin = -1;
				currentTotalTime = -1;
			}
		}
	}
	*totalPoorSleep = masterTotalTime;
	*startMin = masterStartMin;
	*endMin = masterEndMin;

	return;
}//END POOR SLEEP

/************************************************************
* Function: void printToFile(FitbitData minute[TOTALMINUTES], FILE *outfile, 
*			double *caloriesPtr, double *distancePtr, unsigned int *floorPtr, unsigned int *stepPtr, unsigned int *averagePtr, int *startMin, int *endMin)
* Date Created: January 12, 2019
* Date Last Modified: January 19, 2019
* Description: Resets all data in the minute struct
* Input parameters: FitbitData minute[TOTALMINUTES] -- [1446]
* Returns: -
* Preconditions: -
* Postconditions: -
* Additional: -
*************************************************************/
void printToFile(FitbitData minute[TOTALMINUTES], FILE *outfile, double *caloriesPtr, double *distancePtr, unsigned int *floorPtr, unsigned int *stepPtr, unsigned int *averagePtr, int *startMin, int *endMin)
{
	//Line 1: Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep
	//Line 2 : valueCalories, valueDistance, valueFloors, valueSteps, valueHeartrate, valueMax, valueSleepStart : valueSleepEnd
	//Lines 3 – N : the filtered, deduped, and cleansed data in the original FitbitData.csv format

	fprintf(outfile, "____________________________________________________________________________________________");
	fprintf(outfile, "\nProcessed Data: \n");
	fprintf(outfile, "Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Sleep \n");
	fprintf(outfile, "%.2lf,%.2lf,%d,%d,%d,%s,%s\n", *caloriesPtr, *distancePtr, *floorPtr, *stepPtr, *averagePtr, minute[*startMin].minute, minute[*endMin].minute); //convert start time and end time into the 0:00:00 format
	
	fprintf(outfile, "____________________________________________________________________________________________");
	fprintf(outfile, "\nCleaned Data: \n");
	fprintf(outfile, "Target:, %s, , , , , ,\n", minute[1].patient); //Target:, 12cx7, , , , , ,
	fprintf(outfile, "Patient, minute, calories, distance, floors, heart, steps, sleep_level\n"); //Patient, minute, calories, distance, floors, heart, steps, sleep_level
	for (int i = 0; i < TOTALMINUTES - 4; i++)
	{
		fprintf(outfile, "%s,%s,%lf,%lf,%d,%d,%d,%d\n", minute[i].patient, minute[i].minute, minute[i].calories, minute[i].distance, minute[i].floors, minute[i].heartRate, minute[i].steps, minute[i].sleepLevel);
	}
	return; 
}//END PRINT TO FILE
