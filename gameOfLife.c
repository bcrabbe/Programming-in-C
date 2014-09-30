//
//  main.c
//  Life
//
//  Created by ben on 16/09/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

unsigned char **createGrid(int dimX, int dimY);
//unsigned char **grid(int rowLeft, int rowRight, int columnTop, int columnBottom);
void printGrid(unsigned char **grid, int dimX, int dimY);
void freeGrid(unsigned char **grid);
void readFile(unsigned char **grid, int dimensionX, int dimensionY, FILE *FP);
int countNeighbours(unsigned char **grid, int i, int j);
int returnCellState(unsigned char **grid, int i, int j);
void turnCellOn(unsigned char **grid, int i, int j);
void turnCellOff(unsigned char **grid, int i, int j);
void advanceGrid(unsigned char **grid, unsigned char **grid2, int dimensionX, int dimensionY);

int main(int argc, const char * argv[])
{
	FILE * FP;
	FP = fopen(argv[1],"r");
	if(FP == NULL)
	{
		printf("failed to open files\n");
		return 1;
	}
	int dimensionX, dimensionY;
	fscanf(FP,"%d %d", &dimensionX, &dimensionY);
	printf(" %d x %d\n", dimensionX, dimensionY);
	
	unsigned char **board, **board2;
	board = createGrid(dimensionX, dimensionY);
	board2 = createGrid(dimensionX, dimensionY);

		//printGrid(board, dimensionX, dimensionY);
	readFile(board, dimensionX, dimensionY, FP);
	int i;
	for(i=0; i<=200000; ++i)
	{
		advanceGrid(board, board2, dimensionX, dimensionY);
		printGrid(board, dimensionX, dimensionY);

	}
	
		//printGrid(board, dimensionX, dimensionY);

	
	
	freeGrid(board);
	fclose(FP);
    return 0;
}

/* allocates dynamic 2d array*/
unsigned char **createGrid( int dimX, int dimY)
{
	int i, 
		numberOfRows=dimY+2,
		numberOfColumns=dimX+2;
	unsigned char **grid;
	//allocates pointers to rows
	grid = (unsigned char **)malloc((size_t)((numberOfRows+1)*sizeof(unsigned char *)));
	grid +=1;
	// allocates row+columns	
	grid[0]=(unsigned char *)malloc((size_t)((numberOfRows*numberOfColumns+1)*sizeof(unsigned char)));
	grid[0] +=1;
	
	// and sets pointers to first block of each row
	for(i=1; i<=(dimY+2); ++i)
	{
		grid[i]=grid[i-1]+numberOfColumns;
	}
	//return pointer to array of pointers to rows

	return grid;

}

void printGrid( unsigned char **grid, int dimX, int dimY)
{	
	printf("\n");		
	int i,j;
	for(j=1; j<=dimY; ++j)
	{
		printf("\n");		
		for(i=1; i<=dimX; ++i)
		{	
			if( returnCellState(grid, i, j) )
			{
				printf("#");
			}
			else
			{
				printf("-");
			}
		}
	}
	printf("\n\n");		

}


/*
//allocates the grid 
unsigned char **grid(int rowLeft, int rowRight, int columnTop, int columnBottom)
{
	int i, 
		numberOfRows=rowRight-rowLeft+1,
		numberOfColumns=columnBottom-columnTop+1;
		
	unsigned char **grid;
	
	//allocates pointers to rows
	grid = (unsigned char **)malloc((size_t)((numberOfRows+1)*sizeof(unsigned char *)));
	grid +=1;
	grid -=rowLeft;

	// allocates rows
	grid[rowLeft]=(unsigned char *)malloc((size_t)((numberOfRows*numberOfColumns+1)*sizeof(unsigned char)));
	grid[rowLeft] +=1;
	grid[rowLeft] -= columnTop;

	// and sets pointers to them
	for(i=rowLeft+1; i<=rowRight; ++i)
	{
		grid[i]=grid[i-1]+numberOfColumns;
	}
	//return pointer to array of pointers to rows
	return grid;
}*/

void freeGrid( unsigned char **grid)
{
	free((char*)(grid[0]-1));
	free((char*)(grid-1));
}

/* reads from file and copies to grid*/
void readFile( unsigned char **grid, int dimensionX, int dimensionY, FILE *FP)
{
	printf("\n\n input FILE:\n\n");

	int i,j;
	char fileChar;
	for(j=1; j<=dimensionY; ++j)
	{
		for(i=1; i<=dimensionX; ++i)
		{
			fscanf(FP,"%c", &fileChar);
			if(fileChar == '\n')
			{
				fscanf(FP,"%c", &fileChar);
			}
				putchar(fileChar);
			
			if( fileChar == '-' )
			{
				turnCellOff(grid, i, j);
			}
			else if( fileChar == '#' )
			{
				turnCellOn(grid, i, j);
			}
			else
			{
				printf("\n read %c for %d,%d element\n",fileChar,i,j);
				printf("This is incorrect file format\n");
				exit(1);

			}
		}
			printf("\n");

	}
}

int countNeighbours(unsigned char **grid, int i, int j)
{
	int neighbourCount=0;
	
	if( returnCellState(grid, i, j+1) ) ++neighbourCount;
	if( returnCellState(grid, i+1, j+1) ) ++neighbourCount;
	if( returnCellState(grid, i+1, j) ) ++neighbourCount;
	if( returnCellState(grid, i+1, j-1) ) ++neighbourCount;
	if( returnCellState(grid, i, j-1) ) ++neighbourCount;
	if( returnCellState(grid, i-1, j-1) ) ++neighbourCount;
	if( returnCellState(grid, i-1, j) ) ++neighbourCount;
	if( returnCellState(grid, i-1, j+1) ) ++neighbourCount;
	
	return neighbourCount;
}

int returnCellState(unsigned char **grid, int i, int j)
{
	if( (grid[j][i] & 0x01) == 0x01 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void turnCellOn(unsigned char **grid, int i, int j)
{
	grid[j][i] = ( grid[j][i] |  0x01 );
}

void turnCellOff(unsigned char **grid, int i, int j)
{
	grid[j][i] = ( grid[j][i] &  0xFE );
}


void advanceGrid(unsigned char **grid, unsigned char **grid2, int dimensionX, int dimensionY)
{
	int i,j;
	
	/*Sets Padding cells around edges to loop to other side */
	for(j=1; j<=dimensionY; ++j)
	{
		grid[j][0]=grid[j][dimensionX];
		grid[j][dimensionX+1]=grid[j][1];
		
	}
	
	for(i=1; i<=dimensionX; ++i)
	{
		grid[0][i]=grid[dimensionY][i];
		grid[dimensionY+1][i]=grid[1][i];
	}
	//corners
	grid[0][0] = grid[dimensionY][dimensionX];
	grid[dimensionY+1][dimensionX+1] = grid[1][1];
	grid[dimensionY+1][0] = grid[1][dimensionX];
 	grid[0][dimensionX+1] = grid[dimensionY][1];

	int neighbourCount;
	for(j=1; j<=dimensionY; ++j)
	{
		for(i=1; i<=dimensionX; ++i)
		{
			neighbourCount = countNeighbours(grid, i, j);
			if( neighbourCount== 2 || neighbourCount==3 )
			{
				turnCellOn( grid2, i, j);
			}
			else
			{
				turnCellOff( grid2, i, j);
			}

		}
	}
		//printGrid(grid, dimensionX, dimensionY);

		//printGrid(grid2, dimensionX, dimensionY);

	unsigned char *tmp1;
	
	for(i=0; i<=(dimensionY+1); ++i)
	{
		tmp1 = grid[i];
		grid[i]=grid2[i];
		grid2[i]=tmp1;
	}
	unsigned char **tmp2;

	tmp2 = grid;
	grid = grid2;
	grid2 = tmp2;
	
		//printGrid(grid, dimensionX, dimensionY);
	
		//printGrid(grid2, dimensionX, dimensionY);
	
}






