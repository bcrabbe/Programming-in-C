//
//  genericFunctions.c
//  
//
//  Created by ben on 15/11/2014.
//
//

#include "maze.h"


/* allocates dynamic 2d array using pointer to array of pointers to rows
 once allocated grid may be passed to function and accessed using standard grid[j][i]*/
unsigned char **createMaze( int dimX, int dimY)
{
    int	numberOfRows=dimY+2,//extra two elements to deal with the edge conditions
    numberOfColumns=dimX+2;
    unsigned char **maze=NULL;
	//here we allocate an array to hold the pointers (which will point to the start of each row)
    maze = (unsigned char **)calloc((size_t)(numberOfRows), sizeof(unsigned char *));
	if(maze==NULL)
	{
		printf("calloc failed\n");
		exit(1);
	}
	//here we allocate the proper array which will hold each cell status
    maze[0]=(unsigned char *)calloc((size_t)(numberOfRows*numberOfColumns), sizeof(unsigned char));
	if(maze[0]==NULL)
	{
		printf("calloc failed\n");
		exit(1);
	}
	// here we set each pointer in the first array to the first element of each row
    for(int i=1; i<=(dimY+2); ++i)
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
void freeGrid( unsigned char **grid)
{
    free((grid[0]));//frees the large char array malloc
    free((grid));//frees the smaller pointer array malloc
}




void printMaze( unsigned char **maze, int dimX, int dimY)
{
    if(graphicsAreOn(0))
    {
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);
        doGraphics(maze, dimX, dimY, sw);
    }
    printf("\n");
    /* for(int i=0; i<=dimX; ++i)
     {
     printf("%x",i);
     }*/
    for(int j=0; j<=dimY+1; ++j)
    {
        printf("\n");
        // printf("%x",j);
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
