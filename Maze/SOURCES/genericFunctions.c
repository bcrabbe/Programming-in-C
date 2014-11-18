//
//  genericFunctions.c
//  
//
//  Created by ben on 15/11/2014.
//
//

#include "maze.h"


/* allocates dynamic 2d array using pointer to array of pointers to rows*/
unsigned char **createMaze( int dimX, int dimY)
{
    int	numberOfRows=dimY+2,//extra elements to store one cell thick outside edge
    numberOfColumns=dimX+2;
    unsigned char **maze=NULL;
	//here we allocate an array to hold the pointers (which will point to the start of each row)
    maze = (unsigned char **)calloc((size_t)(numberOfRows), sizeof(unsigned char *));
	if(maze==NULL)
	{
		printf("calloc failed\n");
		exit(1);
	}
	//here we allocate the proper array which will hold each cell state
    maze[0]=(unsigned char *)calloc((size_t)(numberOfRows*numberOfColumns), sizeof(unsigned char));

	if(maze[0]==NULL)
	{
		printf("calloc failed\n");
		exit(1);
	}
	// here we set each pointer in the first array to the first element of each row
    for(int i=1; i<numberOfRows; ++i)
    {
        maze[i]=maze[i-1]+numberOfColumns;
    }
    /* set the outside edge, this is not part of the actual maze*/
    for(int i=0; i<=dimX+1; ++i)
    {
        maze[0][i]=' ';
        maze[dimY+1][i]=' ';   
    }
    for(int j=0; j<=dimY+1; ++j)
    {
        maze[j][0]=' ';
        maze[j][dimX+1]=' ';
    }
    //return pointer to array of pointers to rows
    return maze;
}


/* Frees a grid allocated with allocateGrid function*/
void freeMaze( unsigned char **maze)
{
    free((maze[0]));//frees the large char array calloc
    free((maze));//frees the smaller pointer array calloc
}




void printMaze( unsigned char **maze, int dimX, int dimY)
{
    if(graphicsAreOn(0))
    {
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);
        doGraphics(maze, dimX, dimY, sw);
    }
    printf("\n");
#if ENABLE_TEST_DIVIDE_CHAMBER
     for(int i=0; i<=dimX; ++i)
     {
    	 printf("%x",i);
     }
#endif
    for(int j=0; j<=dimY+1; ++j)
    {
        printf("\n");
#if ENABLE_TEST_DIVIDE_CHAMBER
		printf("%x",j);
#endif
        for(int i=0; i<=dimX+1; ++i)
        {
            if(maze[j][i]=='x')
            {
                printf(" ");
            }
            else printf("%c",maze[j][i]);
        }
    }
    printf("\n");
}

void usageError()
{
	fprintf(stderr,"\nERROR.\n");
	fprintf(stderr,"Run program with a cmdline arg stating either a txt file which contains a maze,\n");
    fprintf(stderr,"or RANDOM.\n");
    fprintf(stderr,"To run with graphics type SDL as a argument following these.\n\n");
	exit(1);
}
