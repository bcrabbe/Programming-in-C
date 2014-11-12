/************************************************************************************
Recursive maze solver
---------------------
 
 

 
************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

void freeGrid( unsigned char **grid);
unsigned char **createGrid( int dimX, int dimY);
void readFile( unsigned char **grid, int dimensionX, int dimensionY, FILE *FP);
void findStart(int * startX, int * startY, unsigned char ** maze, int dimX, int dimY );
int pathFinder(int i, int j, unsigned char **maze, int dimX, int dimY);
void printMaze( unsigned char **maze, int dimX, int dimY);
void testPathFinder(int i, int j, unsigned char **maze);
unsigned char ** initialiseFromFile(const char * fileName, int *dimensionX, int *dimensionY);

int main(int argc, const char * argv[])
{
    int dimensionX,dimensionY;
    unsigned char ** maze=NULL;
    if(argc>=2 && strcmp(argv[1],"RANDOM")!=0)//if user supplied atleast one additional cmd line arg
    {                                         //and this wasn't 'RANDOM'
        maze = initialiseFromFile(argv[1],&dimensionX,&dimensionY);            //then it should have been a file path which we init our maze from
    }
        
    int startX, startY;
    findStart(&startX, &startY, maze, dimensionX, dimensionY);
    printf("\nstart: %d %d\n",startX,startY);
    int foundPath = pathFinder(startX, startY, maze, dimensionX, dimensionY);
    if(foundPath)
    {
        printf("\nfound exit");
    }
    else
    {
        printf("\nfound no exits");
    }

    printMaze(maze, dimensionX, dimensionY);
    return 0;
}

void testPathFinder(int i, int j, unsigned char **maze)
{
    printf("moved to (%d,%d)",i, j);
    printf("  it was %c\n",maze[j][i]);

}

int pathFinder(int i, int j, unsigned char **maze, int dimX, int dimY)
{
    //testPathFinder(i,j,maze);
    if( i<1 || i>dimX || j<1 || j>dimY )//if we have exited the maze...
    {
        return 1;//...then return true
    }
    if(maze[j][i]=='#'||maze[j][i]=='.')//if element i,j is wall then we cant move here so...
    {
        return 0;//return false
    }
    if(maze[j][i]==' ')//we are in a empty space..
    {
        maze[j][i]='.';//so we mark that we have been here
    }
    if(pathFinder(i, j+1, maze, dimX, dimY)==1) return 1;//move up
    //testPathFinder(i,j,maze);
    if(pathFinder(i+1, j, maze, dimX, dimY)==1) return 1;//move to the right
    //testPathFinder(i,j,maze);
    if(pathFinder(i, j-1, maze, dimX, dimY)==1) return 1;//move downwards
    //testPathFinder(i,j,maze);
    if(pathFinder(i-1, j, maze, dimX, dimY)==1) return 1;//move left
    //testPathFinder(i,j,maze);
    
    //if we get here then i,j is not part of solution path
    //so we un mark it:
    
    maze[j][i]=' ';

    return 0;
}

unsigned char ** initialiseFromFile(const char * fileName, int *dimensionX, int *dimensionY)
{
    
    unsigned char **maze;

    FILE * FP;
    FP = fopen(fileName,"r");
    if( FP==NULL )
    {
        fprintf(stderr,"failed to open file\n");
        exit(1);
    }
    if(fscanf(FP,"%d %d", dimensionX, dimensionY)!=2)
    {
        fprintf(stderr,"failed to read dimensions from file from file\n");
        fclose(FP);
        exit(1);
    }
    printf("file dimensions: %d by %d",*dimensionX,*dimensionY);
    if(*dimensionX>20|| *dimensionX<3 || *dimensionY>20 || *dimensionY<3)
    {
        fprintf(stderr,"ERROR: maze dimensions must be in the range: 3-20 by 3-20");
        fclose(FP);
        exit(1);
    }
    maze = createGrid(*dimensionX, *dimensionY);
    if( maze==NULL )
    {
        fprintf(stderr,"failed to allocate grid\n");
        fclose(FP);
        exit(1);
    }
    readFile(maze, *dimensionX, *dimensionY, FP);//reads the pattern in from file
    fclose(FP);
	
    return maze;
}

void findStart(int * startX, int * startY, unsigned char ** maze, int dimX, int dimY )
{
    //first check top row and leftmost column simultaneously to find the space nearest the top left corner
    int i=1, j=1;
    while(i<=dimX && j<=dimY)
    {
        int foundStartInTop=0,foundStartInLeft=0;
        if( maze[1][i]==' ')
        {
            foundStartInTop=1;
        }
        if(maze[j][1]==' ')
        {
            foundStartInLeft=1;
        }
        if(foundStartInTop==1 && foundStartInLeft==0)
        {
            *startX=i;
            *startY=1;
            return;
        }
        else if(foundStartInTop==0 && foundStartInLeft==1)
        {
            *startX=1;
            *startY=j;
            return;
        }
        else if(foundStartInTop==1 && foundStartInLeft==1)
        {
            fprintf(stderr,"this maze has two possible starts equidistant from top left \n");
            exit(3);
        }
        else if(foundStartInTop==0 && foundStartInLeft==0)
        {
            if(i<dimX) ++i;
            if(i<dimY) ++j;
        }
    }
    
    //If we find no gaps in those sides then we must search the other two aswell
    i=1;
    j=1;
    while(i<=dimX && j<=dimY)
    {
        int foundStartInBottom=0,foundStartInRight=0;
        if( maze[dimY][i]==' ')
        {
            foundStartInBottom=1;
        }
        if(maze[j][dimX]==' ')
        {
            foundStartInRight=1;
        }
        if(foundStartInBottom==1 && foundStartInRight==0)
        {
            *startX=i;
            *startY=dimY;
            return;
        }
        else if(foundStartInBottom==0 && foundStartInRight==1)
        {
            *startX=dimX;
            *startY=j;
            return;
        }
        else if(foundStartInBottom==1 && foundStartInRight==1)
        {
            fprintf(stderr,"Undefined: this maze has two possible starts equidistant from top left \n");
            exit(3);
        }
        else if(foundStartInBottom==0 && foundStartInRight==0)
        {
            if(i<dimX) ++i;
            if(i<dimY) ++j;
        }
    }
    
    //if we find no start:
    fprintf(stderr,"There are no openings in the wall of the maze. \n");
    exit(3);
    
    
}

/* Frees a grid allocated with allocateGrid function*/
void freeGrid( unsigned char **grid)
{
    free((grid[0]));//frees the large char array malloc
    free((grid));//frees the smaller pointer array malloc
}

/* reads from file and copies to grid. File must have dimensionX dimensionY as its first line
 followed by the a board of ' '(=possible path) or #(=obstical) */
void readFile( unsigned char **grid, int dimensionX, int dimensionY, FILE *FP)
{
    printf("\n input FILE:\n");
    int i,j;
    char fileChar;
    for(j=1; j<=dimensionY; ++j)
    {
        for(i=1; i<=dimensionX; ++i)
        {
            if( fscanf(FP,"%c", &fileChar)!=1 )
			{
				fprintf(stderr,"in readFile fscanf failed");
                fclose(FP);
				exit(2);
			}
            if(fileChar == '\n')//deals with stray new lines left in the buffer
            {
                if(fscanf(FP,"%c", &fileChar)!=1)
				{
					fprintf(stderr,"in readFile fscanf failed");
                    fclose(FP);
					exit(2);
				}
            }
			putchar(fileChar); //print the board to screen as we go
            if( fileChar == ' ' )
            {
                grid[j][i]=' ';
            }
            else if( fileChar == '#' || fileChar == '#')
            {
                grid[j][i]='#';
            }
            else
            {
                fprintf(stderr,"read %c for %d,%d element\n",fileChar,i,j);
                fprintf(stderr,"This is incorrect file format\n");
                fclose(FP);
                exit(2);
            }
        }
		printf("\n");
    }
}

/* allocates dynamic 2d array using pointer to array of pointers to rows
 once allocated grid may be passed to function and accessed using standard grid[j][i]*/
unsigned char **createGrid( int dimX, int dimY)
{
    int	numberOfRows=dimY+2,//extra two elements to deal with the edge conditions
    numberOfColumns=dimX+2;
    unsigned char **grid=NULL;
	//here we allocate an array to hold the pointers (which will point to the start of each row)
    grid = (unsigned char **)calloc((size_t)(numberOfRows), sizeof(unsigned char *));
	if(grid==NULL)
	{
		printf("calloc failed\n");
		exit(1);
	}
	//here we allocate the proper array which will hold each cell status
    grid[0]=(unsigned char *)calloc((size_t)(numberOfRows*numberOfColumns), sizeof(unsigned char));
	if(grid[0]==NULL)
	{
		printf("calloc failed\n");
		exit(1);
	}
	// here we set each pointer in the first array to the first element of each row
    for(int i=1; i<=(dimY+2); ++i)
    {
        grid[i]=grid[i-1]+numberOfColumns;
    }
    //return pointer to array of pointers to rows
    return grid;
}

void printMaze( unsigned char **maze, int dimX, int dimY)
{
    printf("\n");
    int i,j;
    for(j=1; j<=dimY; ++j)
    {
        printf("\n");
        for(i=1; i<=dimX; ++i)
        {
            printf("%c",maze[j][i]);
        }
    }
    printf("\n");
}
