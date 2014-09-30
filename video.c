//
//  video.c
//  
//
//  Created by ben on 25/09/2014.
//
//

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#define INVALID -1
#define VALID 0

int videoPlusDay( double time);
int convertToVideoPlusNumber( float recordingTime, int recordingDaysInFuture);

int main(void)
{
	printf("There have been %ld seconds since 1/1/1970\n", time(NULL));
	printf("The videoPlus day is %d.\n", videoPlusDay(time(NULL)) );
	
	
	
	float programTime;
	int programDaysInFuture, inputFormat = INVALID;
	
	while(inputFormat==INVALID)
	{
		printf("Please enter the time (hh.mm) and the number of days into the future of the program you want to record, eg 23.15 6 then ENTER.\n");
		scanf("%f %d",&programTime, &programDaysInFuture);
		if(programTime>24.00 || programTime<00.00 ||
							programTime-(int)programTime >0.59)
		{
			printf("ERROR: invalid time.\n Try again.\n ");
		}
		else if(programDaysInFuture<0 || programDaysInFuture>49)
		{
			printf("ERROR: invalid day, you may record a max of 49 day into the future.\n Try again.\n ");
		}
		else inputFormat=VALID;
	}
	printf("you entered %2.2f %d\n",programTime, programDaysInFuture);
	int videoPlusNumber = convertToVideoPlusNumber(programTime,programDaysInFuture);
	printf("The videoPlus code is: %d.\n",videoPlusNumber);
	
	return 0;
}

int videoPlusDay(double time)
{
	//86400 seconds in a day
	float numberOfDays=time/86400;
	numberOfDays=floor(numberOfDays);//rounds down
	
	int videoPlusDay;
	
	videoPlusDay=(int)numberOfDays%50;
	
	return videoPlusDay;
}

int convertToVideoPlusNumber( float recordingTime, int recordingDaysInFuture)
{
	int videoPlusDayToday=videoPlusDay(time(NULL));
	int recordingVideoPlusDay = videoPlusDayToday + recordingDaysInFuture;
	if(recordingVideoPlusDay>49)
	{
		recordingVideoPlusDay = recordingVideoPlusDay-49;
	}
	
	int videoPlusNumber,timeDateCombined;
	if(recordingVideoPlusDay<10)
	{
		recordingTime = recordingTime*1000;
		timeDateCombined = (int)recordingTime + recordingVideoPlusDay;
	}
	else// if(recordingDaysInFuture>=10)
	{
		recordingTime = recordingTime*10000;
		timeDateCombined = (int)recordingTime + recordingVideoPlusDay;
	}
	//printf("%d\n", timeDateCombined);
	int i=9,divisor=-1;
	while(divisor==-1 && i>0)
	{
		if(timeDateCombined%i == 0)
			divisor =i;
		else
			--i;
	}
	videoPlusNumber= (timeDateCombined/divisor)*10 + divisor;
	//printf("%d\n", videoPlusNumber);

	return videoPlusNumber;
	
}

