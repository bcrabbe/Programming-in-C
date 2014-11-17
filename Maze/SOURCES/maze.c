/************************************************************************************
Recursive maze solver
---------------------
 
 

 
************************************************************************************/

#include "maze.h"


int main(int argc, const char * argv[])
{
    srand(time(NULL));
    int dimensionX,dimensionY;//number of columns and rows respectively
    int startX, startY;
    unsigned char ** maze=NULL;//2d charrecter array for the maze
    /*** SDL ***/
    if( argc==3 && strcmp(argv[2],"SDL")==0)
    {
        graphicsAreOn(1);//set graphics on
        SDL_Simplewin sw;
        Neill_SDL_Init(&sw);
        getSdlWindowPtr(&sw);//store pointer to sw
    }
    /*** Read Maze from File ***/
    if(argc>=2 && strcmp(argv[1],"RANDOM")!=0)
    {
        maze = initialiseFromFile(argv[1],&dimensionX,&dimensionY);
        findStart(&startX, &startY, maze, dimensionX, dimensionY);
        maze[startY][startX]='s';
    }
    /*** RANDOM Gen Maze ***/
    else if(argc>=2 && strcmp(argv[1],"RANDOM")==0)
    {
        maze = generateRandomMaze(&dimensionX,&dimensionY);
        startX=1;
        startY=2;
    }
    else
    {
        fprintf(stderr,"USAGE MESSAGE\n");
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
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);//retreive simplewin ptr
        while(!sw->finished)
        {
            Neill_SDL_Events(sw);
        }
    }
    freeGrid(maze);

    return 0;

}
