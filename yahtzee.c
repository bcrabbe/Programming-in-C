//
//  yahtzee.c
//  
//
//  Created by ben on 25/09/2014.
//
//

#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

void throwDice(int *dice, unsigned int diceToRoll);
void printDice(int *dice);

int main(void)
{
	srand(time(NULL));
	printf("\nYahtzee\n\n");
	int dice[5];
	char r;
	do
	{
		unsigned int diceToRoll=31;

		throwDice(dice, diceToRoll);
		printDice(dice);
		int i;
		for(i=0; i<2; ++i)
		{
			do
			{
				printf("Which dice would you like to reroll?\n");
				scanf("%ud",&diceToRoll);
				if(diceToRoll<0 || diceToRoll>31)
				{
					printf("Invalid Input.\n\n");
				}
			} while (diceToRoll<0 || diceToRoll>31);
			throwDice(dice, diceToRoll);
			printDice(dice);
		}
		printf("\n\nPlay again? y/n");
		r = getchar();
		if (r == '\n')
		{
			r = getchar();
		}
		while( (r != 'n') && (r != 'N') && (r != 'y') && (r != 'Y') )
		{
			printf("\ninvalid input, enter the choice(y/Y/n/N) again : ");
			r = getchar();
			if (r == '\n')
			{
				r = getchar();
			}
		}
		if( r=='n' ) break;
	} while( (r != 'y') || (r!='Y')) ;
	
	return 0;
}

void throwDice(int *dice, unsigned int diceToRoll)
{
	if ( (diceToRoll & 16) > 0 )
	{
		*dice = (rand()%6 + 1);
	}
	if ( (diceToRoll & 8) > 0 )
	{
		*(dice+1) = (rand()%6 + 1);
	}
	if ( (diceToRoll & 4) > 0 )
	{
		*(dice+2) = (rand()%6 + 1);
	}
	if ( (diceToRoll & 2) > 0 )
	{
		*(dice+3) = (rand()%6 + 1);
	}
	if ( (diceToRoll & 1) > 0 )
	{
		*(dice+4) = (rand()%6 + 1);
	}
}

void printDice(int *dice)
{
	printf("You got:\n");
	int i;
	for(i=0; i<=4; ++i)
	{
		printf("%d ",*(dice+i));
	}
	printf("\n ");
}

