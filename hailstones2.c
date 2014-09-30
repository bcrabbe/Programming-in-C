//
//  File.c
//
//
//  Created by ben on 15/09/2014.
//
//

#include <stdio.h>
#include <math.h>
int stepper (int n);

int main()
{
	
	int n,					// n is value of sequence
		N,						// N is starting value of the sequence
		lengthOfSequence,
		largestNumberOfSequence,
		longestSequence = 1,		//number (N) which produduces it
		longestSequenceN = 0,		//the longest sequence and the n initial
		largestNumber = 1,		// and same for the largest number
		largestNumberN = 1;
	
	
	for(N=2; N<=50000; ++N)
	{
		n = N;
		lengthOfSequence = 0;
		largestNumberOfSequence = N;
		while(n>1)
		{
			//printf("%d\n",n);
			n = stepper(n);
			++lengthOfSequence;
			if(n>largestNumberOfSequence)
				largestNumberOfSequence = n;

		}
		//printf("%d\n",n);
		++lengthOfSequence;
		if(lengthOfSequence>longestSequence)
		{
			longestSequence = lengthOfSequence;
			longestSequenceN = N;
		}
		if(largestNumberOfSequence>largestNumber)
		{
			largestNumber = largestNumberOfSequence;
			largestNumberN = N;
			//printf("%d  from %d\n",largestNumber,largestNumberN);
		}
		
		//printf("\n");
 	}
	
	printf("\nThe longest sequence was %d numbers long, produced by an initial value of %d\n",longestSequence,longestSequenceN);
	printf("\nThe largest number was %d, produced by an initial value of %d\n",largestNumber,largestNumberN);

	return 0;
}
		   
		   
int stepper (int n)
{
	n = ( (n%2==0) ? (n/2) : ((3*n)+1) ); // if n is even n->n/2 if its odd n->3n+1
	
	/*if( fabs((double)(n%2))<0.00001 )
		n = n/2;
	else
		n = (3*n+1);*/
		
	return n;
}