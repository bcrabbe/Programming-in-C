//
//  mazeSolver.c
//  
//
//  Created by ben on 15/11/2014.
//
//

#include "maze.h"

/* Recursive maze solver*/
int pathFinder(int i, int j, unsigned char **maze, int dimX, int dimY)
{
    if(graphicsAreOn(0))
    {
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);
        doGraphics(maze, dimX, dimY, sw);
    }
#if ENABLE_TEST_PATH_FINDER
    testPathFinder(i,j,maze);
#endif 
    if(maze[j][i]=='#'||maze[j][i]=='.'||maze[j][i]=='x')//if element i,j is wall then we cant move here so...
    {
        return 0;//return false
    }
    if(maze[j][i]==' ')//we are in a empty space..
    {
        maze[j][i]='.';//so we mark that we have been here
    }
    if( (i<=1 || i>=dimX || j<=1 || j>=dimY) && maze[j][i]!='s' )
    {        //if we have exited the maze...and this exit is not adjacent to the start
        if( maze[j][i+1]=='s' || maze[j][i-1]=='s' ||
           maze[j+1][i]=='s' || maze[j-1][i]=='s' )
        {
            return 0;
        }
        else
        {
            return 1;//...then return true
        }
    }
    if(pathFinder(i+1, j, maze, dimX, dimY)==1) return 1;//move to the right
#if ENABLE_TEST_PATH_FINDER
	testPathFinder(i,j,maze);
#endif
    if(pathFinder(i, j-1, maze, dimX, dimY)==1) return 1;//move downwards
#if ENABLE_TEST_PATH_FINDER
	testPathFinder(i,j,maze);
#endif
    if(pathFinder(i, j+1, maze, dimX, dimY)==1) return 1;//move up
#if ENABLE_TEST_PATH_FINDER
	testPathFinder(i,j,maze);
#endif
    if(pathFinder(i-1, j, maze, dimX, dimY)==1) return 1;//move left
#if ENABLE_TEST_PATH_FINDER
	testPathFinder(i,j,maze);
#endif
    
    //if we get here then i,j is not part of solution path
    //so we un mark it:
    if(maze[j][i]!='s')
	{
		maze[j][i]='x';
	}    
    return 0;
}

/*testing function for pathFinder
  Enable by setting ENABLE_TEST_PATH_FINDER to 1 in maze.h*/
void testPathFinder(int i, int j, unsigned char **maze)
{
    printf(">>>>>moved to (%d,%d)",i, j);
    printf(">>>>>it was %c\n",maze[j][i]);
    
}
