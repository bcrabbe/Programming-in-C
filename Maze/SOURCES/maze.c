/************************************************************************************
Recursive maze solver
---------------------

FULL ASSIGNMENT COMPLETED

Run program with a cmdline arg stating either a path to a  txt file which contains a maze
with the dimensions specifed on the top line and open spaces represented by ' ' and walls
represented by '#' 
or "RANDOM" which will  then ask you for the dimensions of the maze you would like to 
randomly generate

these can be followed by "SDL" for a graphics window to open and display the maze


 

 
************************************************************************************/

#include "maze.h"


int main(int argc, const char * argv[])
{
    srand(time(NULL));
    int dimensionX,dimensionY;//number of columns and rows respectively
    int startX, startY;
    unsigned char ** maze=NULL;//2d character array for holding the state of the maze
    /*** SDL ***/
    if( argc==3 && strcmp(argv[2],"SDL")==0)
    {
        graphicsAreOn(1);//set graphics on
        SDL_Simplewin sw;
        Neill_SDL_Init(&sw);
        getSdlWindowPtr(&sw);//store pointer to sw
    }
	if( argc==3 && strcmp(argv[2],"SDL")!=0)
    {
		usageError();
    }
    /*** Read Maze from File ***/
    if( (argc==2 || argc==3) && strcmp(argv[1],"RANDOM")!=0)
    {
        maze = initialiseFromFile(argv[1],&dimensionX,&dimensionY);
        findStart(&startX, &startY, maze, dimensionX, dimensionY);
        maze[startY][startX]='s';
    }
    /*** RANDOM Gen Maze ***/
    else if( (argc==2 || argc==3) && strcmp(argv[1],"RANDOM")==0)
    {
        maze = generateRandomMaze(&dimensionX,&dimensionY);
        startX=1;
        startY=2;
    }
	else
	{
		usageError();
	}
    
	

    printMaze(maze, dimensionX, dimensionY);

    int foundPath = pathFinder(startX, startY, maze, dimensionX, dimensionY);
    if(foundPath)
    {
        printf("\n>>> Found Exit <<<");
    }
    else
    {
        printf("\n>>> Found No Exit s<<<");
    }

    printMaze(maze, dimensionX, dimensionY);
    
    if(graphicsAreOn(0))
    {
		printf("\nClose SDL window to exit.\n");
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);//retreive simplewin ptr
        while(!sw->finished)
        {
            Neill_SDL_Events(sw);
        }
    }
    freeMaze(maze);

    if(foundPath)
    {
        return 0;
    }
    else
    {
		return 1;
    }

}

