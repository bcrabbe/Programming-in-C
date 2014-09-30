#include <stdio.h>
#include <stdlib.h>
#define INVALID -1
#define VALID 0

void placeBets( float *oddBet, float *evenBet, float *numberBet, float *totalBet);
void showBets(float oddBet, float evenBet, float *numberBet);
void spinWheel(float oddBet, float evenBet, float *numberBet);

int main(void)
{
	float oddBet=0, evenBet=0, numberBet[36]={0}, totalBet=0;
	
	char playAgain=0;
	while(playAgain!='n')
	{
		placeBets( &oddBet, &evenBet, numberBet, &totalBet);
		showBets(oddBet, evenBet, numberBet);
		spinWheel(oddBet, evenBet, numberBet);
		printf("\nYou won $%.2f.\n\n",winnings);

		while(playAgain)
		printf("Play again? y/n \n");
		scanf("%c",&playAgain);
		if (playAgain=='n')
			break;
		if(playAgain=='y');
			
		else
			
	}

	float balence = winnings-losses;
	
	if(balence<0)
	{
		printf("\nYou have lost $%.2f.\n",balence);
	}
	else if(balence>0)
	{
		printf("\nYou have made $%.2f.\n",balence);
	}
	else
		printf("\nYou have broke even");

	*/
	
	return 0;
}



void placeBets( float * oddBet, float * evenBet, float * numberBet, float * totalBet)
{
	//float oddBet, evenBet, numberBet[36]={0}, totalBet=0;

	printf("You can place a bet on odd, even or any number 0 to 35.\n");
	
	printf("Enter the ammount you would like to place on odd: ");
	scanf("%f",oddBet);
	
	printf("\nEnter the ammount you would like to place on even: ");
	scanf("%f",evenBet);
	
	int numberToBetOn;
	
	while(numberToBetOn >= 0)
	{
		printf("\nEnter a number (0-35) you would like to place a bet on.\n");
		printf("To enter no more bets enter -1.\n");
		printf("You can change a bet by entering that number again.\n\n");
		scanf("%d", &numberToBetOn);
		if(numberToBetOn >35)
			printf("....non valid bet entered. Try again.");
		else if (numberToBetOn ==-1) break;
		else
		{
			printf("\nHow much would you like to place on %d? ",numberToBetOn);
			scanf("%f",(numberBet+numberToBetOn) );
			* totalBet += * (numberBet+numberToBetOn);
			printf("\nYou have placed $%.2f on %d\n",numberBet[numberToBetOn],numberToBetOn);
		}
	}
	* totalBet += *oddBet +  *evenBet;

}

void showBets(float oddBet, float evenBet, float *numberBet)
{
	printf("Bets:\n");
	printf("odd: $%.2f\n",oddBet);
	printf("even: $%.2f\n",evenBet);
	int i;
	for ( i=0; i<=35; ++i)
	{
		printf("%d: $%.2f\n",i,*(numberBet+i));
	}

	
}

void spinWheel(float oddBet, float evenBet, float *numberBet, float *winnings, float *losses)
{
	printf("\nBets have been placed. Spinning.....");
	int outcome = rand()%36;
	printf("The number is %d.\n",outcome);
	float winnings, losses;
	if (outcome%2==0 && outcome!=0)
	{
		*winnings += evenBet*2;
		*losses -= oddBet;
	}
	else if (outcome%2==1 && outcome!=0)
	{
		*winnings += oddBet*2;
		*losses -= evenBet;
	}
	if( numberBet[outcome]!=0)
	{
		*winnings += numberBet[outcome]*35;
		*losses += numberBet[outcome];
	}
	for(int i=0 ; i<=35 ; ++i)
	{
		*losses -= numberBet[i];
	}
	
}
