/************************************************************************************
Recursive maze solver
---------------------
 
 

 
************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <assert.h>
#define NDEBUG

#define WWIDTH 600
#define WHEIGHT 600
#define RECTSIZE 20
#define MILLISECONDDELAY 500
#define SDL_8BITCOLOUR 256

typedef struct SDL_Simplewin
{
	SDL_bool finished;
	SDL_Window *win;
	SDL_Renderer *renderer;
} SDL_Simplewin;

void freeGrid( unsigned char **grid);
unsigned char **createGrid( int dimX, int dimY);
void readFile( unsigned char **grid, int dimensionX, int dimensionY, FILE *FP);
void findStart(int * startX, int * startY, unsigned char ** maze, int dimX, int dimY );
int pathFinder(int i, int j, unsigned char **maze, int dimX, int dimY);
void printMaze( unsigned char **maze, int dimX, int dimY);
void testPathFinder(int i, int j, unsigned char **maze);
unsigned char ** initialiseFromFile(const char * fileName, int *dimensionX, int *dimensionY);
unsigned char ** generateMaze( int *dimensionX, int *dimensionY);
unsigned char ** generateRandomMaze( int *dimensionX, int *dimensionY);
int divideChamber(unsigned char ** maze, int minX, int minY, int chamberWidth, int chamberHeight);
int graphicsAreOn(int sdl);
SDL_Simplewin * getSdlWindowPtr(SDL_Simplewin *sw);
void doGraphics(unsigned char **maze,int dimensionX, int dimensionY, SDL_Simplewin *sw);



void Neill_SDL_Init(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);

int main(int argc, const char * argv[])
{
    srand(time(NULL));
    int dimensionX,dimensionY;//number of columns and rows respectively
    unsigned char ** maze=NULL;//2d charrecter array for the maze
    if( argc==3 && strcmp(argv[2],"SDL")==0)
    {
        graphicsAreOn(1);//set graphics on
        SDL_Simplewin sw;
        Neill_SDL_Init(&sw);
        getSdlWindowPtr(&sw);//store pointer to sw
    }
    if(argc>=2 && strcmp(argv[1],"RANDOM")!=0)
    {
        maze = initialiseFromFile(argv[1],&dimensionX,&dimensionY);
        printMaze(maze, dimensionX, dimensionY);
    }
    else if(argc>=2 && strcmp(argv[1],"RANDOM")==0)
    {
        maze = generateRandomMaze(&dimensionX,&dimensionY);
        printMaze(maze, dimensionX, dimensionY);
        
        if(graphicsAreOn(0))    SDL_Delay(100000);
    }
    
    int startX, startY;
    findStart(&startX, &startY, maze, dimensionX, dimensionY);
    maze[startY][startX]='s';
    
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
    
    if(graphicsAreOn(0))
    {
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);//retreive simplewin ptr
        while(!sw->finished)
        {
            Neill_SDL_Events(sw);
        }
    }

    return 0;

}

int graphicsAreOn(int setOn)
{
    static int graphicsAreOn;
    if(setOn)
        graphicsAreOn=1;
        
    return graphicsAreOn;
}

SDL_Simplewin * getSdlWindowPtr(SDL_Simplewin *sw)
{
    static SDL_Simplewin * window;
    
    if(sw!=NULL)
    {
        window = sw;
    }
    
    return window;
    
}
void Neill_SDL_Init(SDL_Simplewin *sw)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	
	sw->finished = 0;
	
	sw->win= SDL_CreateWindow("SDL Window",
							  SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED,
							  WWIDTH, WHEIGHT,
							  SDL_WINDOW_SHOWN);
	if(sw->win == NULL){
		fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	
	sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
	if(sw->renderer == NULL){
		fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	
    SDL_SetRenderDrawColor(sw->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(sw->renderer);
	SDL_RenderPresent(sw->renderer);
	
}

void doGraphics(unsigned char **maze,int dimensionX, int dimensionY, SDL_Simplewin *sw)
{
    SDL_Rect rectangle;
    
    SDL_Delay(MILLISECONDDELAY);
	rectangle.w = WWIDTH/dimensionX;
	rectangle.h = WHEIGHT/dimensionY;
	int i,j;
    for(j=0; j<=dimensionY+1; ++j)
    {
        
        for(i=0; i<=dimensionX+1; ++i)
        {
            Neill_SDL_Events(sw);
			if(maze[j][i]=='#' )
			{
                SDL_SetRenderDrawColor(sw->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			}
			else if(maze[j][i]==' ' )
			{
                SDL_SetRenderDrawColor(sw->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			}
            else if(maze[j][i]=='s' )
			{
                SDL_SetRenderDrawColor(sw->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
			}
            else if(maze[j][i]=='.')
            {
                SDL_SetRenderDrawColor(sw->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
            }
			rectangle.x = (i-1)*rectangle.w;
			rectangle.y = (j-1)*rectangle.h;
			SDL_RenderFillRect(sw->renderer, &rectangle);
        }
    }
	SDL_RenderPresent(sw->renderer);
	SDL_UpdateWindowSurface(sw->win);
	Neill_SDL_Events(sw);
	if(sw->finished)
	{
		exit(0);
	}
}
// Gobble all events & ignore most
void Neill_SDL_Events(SDL_Simplewin *sw)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
            case SDL_KEYDOWN:
				sw->finished = 1;
		}
    }
}

unsigned char ** generateRandomMaze( int *dimensionX, int *dimensionY)
{
    printf("Please enter the dimensions of the maze you would like to generate: formated x y \n");
    if(scanf("%d %d",dimensionX,dimensionY)!=2 || *dimensionX<=5 || *dimensionY<=5)
    {
        printf("failed to read dimensions, please enter two integers greater than 5 \n");
    }
    unsigned char ** maze = createGrid(*dimensionX, *dimensionY);
    
    if(divideChamber(maze, 1, 1, *dimensionX, *dimensionY)==1)
    {
        return maze;
    }
    else
    {
        fprintf(stderr,"random maze generator failed\n");
        exit(5);
    }
}

void testDivideChamber(unsigned char ** maze, int numberOfDivides, int verticalXcoord,
                        int horizontalYcoord )
{
    printf("\n>>>>>>>>>>>>>divide mase call number %d\n",numberOfDivides);
    printf(">>>>>>>>>>>>>>>verticalXcoord %d horizontalYcoord %d\n",verticalXcoord,horizontalYcoord);

    printMaze(maze, 15, 15);
    
}

int divideChamber(unsigned char ** maze, int minX, int minY, int chamberWidth, int chamberHeight)
{
    static int numberOfDivides;
    ++numberOfDivides;

    if(chamberWidth<6 || chamberHeight<6)
    {
        return 1;
    }
    
    int verticalXcoord = minX + rand()%(chamberWidth);//the xcoord of the vertical dividing line
    int horizontalYcoord = minY + rand()%(chamberHeight);//the y coord of the horizontal dividing line
    
    int wallToLeaveWhole = rand()%4 ;//random number 0-3 picks one of the 4 wall segments
    int wallCounter=0;               //iterates as we draw walls
    int spaceToLeave;
    
    /*********draw top part of vertical wall:**********/
    //line goes from top of chamber to the y coord of the horizontalLine,
    //a random num between these bounds is:
    spaceToLeave = minY + rand()%(horizontalYcoord-minY+1);
    assert(spaceToLeave<= horizontalYcoord);
    for(int j=minY; j<=horizontalYcoord; ++j)
    {
        if(j==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[j][verticalXcoord]=' ';

        }
        else maze[j][verticalXcoord]='#';
    }
    ++wallCounter;
    /*********draw bottom part of vertical wall:**********/
    //line goes from the y coord of the horizontal Line to ymin+chamberHeight,
    //a random num between these bounds is:
    spaceToLeave = horizontalYcoord + rand()%(minY+chamberHeight-horizontalYcoord+1);
    assert(spaceToLeave <= minY + chamberHeight);
    for(int j=horizontalYcoord; j<=minY+chamberHeight; ++j)
    {
        if(j==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[j][verticalXcoord]=' ';
            
        }
        else maze[j][verticalXcoord]='#';
    }
    ++wallCounter;
    /*********draw left part of Horizontal wall:**********/
    //line goes from minX to the x coord of the vertical Line
    //a random num between these bounds is:
    spaceToLeave = minX + rand()%(verticalXcoord-minX+1);
    assert(spaceToLeave <= verticalXcoord);
    for(int i=minX; i<=verticalXcoord; ++i)
    {
        if(i==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[horizontalYcoord][i]=' ';
            
        }
        else maze[horizontalYcoord][i]='#';
    }
    ++wallCounter;
    /*********draw right part of horizontal wall:**********/
    //line goes from the x coord of the vertical Line to minX+chamberWidth,
    //a random number between these bounds is:
    spaceToLeave = verticalXcoord + rand()%(minX+chamberWidth-verticalXcoord+1);
    assert(spaceToLeave <= minX + chamberWidth);
    for(int i=verticalXcoord; i<= minX + chamberWidth; ++i)
    {
        if(i==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[horizontalYcoord][i]=' ';
            
        }
        else maze[horizontalYcoord][i]='#';
    }
    //testDivideChamber(maze, numberOfDivides,verticalXcoord,horizontalYcoord );

    // Now call divide again the 4 chambers
    if(
       divideChamber(maze, minX, minY, verticalXcoord-minX, horizontalYcoord-minY)
       &&
       divideChamber(maze, verticalXcoord, minY, minX+chamberWidth-verticalXcoord,
                     horizontalYcoord-minY)
        &&
        divideChamber(maze,verticalXcoord,horizontalYcoord,
                      minX+chamberWidth-verticalXcoord,
                      minY+chamberHeight-horizontalYcoord)
        &&
        divideChamber(maze, minX, horizontalYcoord,verticalXcoord-minX,
                      minY+chamberHeight-horizontalYcoord)
       )
        return 1;


    else
    {
        fprintf(stderr,"divide chamber call %d failed\n",numberOfDivides);
        exit(5);
    }
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
    int i,j,newlineRead;
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
            while(fileChar == '\n'||fileChar == '\r')//deals with the new lines
            {
                newlineRead=1;
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
                fprintf(stderr,"read %d/%c for %d,%d element\n",fileChar,fileChar,i,j);
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
    if(graphicsAreOn(0))
    {
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);
        doGraphics(maze, dimX, dimY, sw);
    }
    printf("\n");
    int i,j;
   /* for(i=0; i<=dimX; ++i)
    {
        printf("%x",i);
    }*/
    for(j=1; j<=dimY; ++j)
    {
        printf("\n");
        // printf("%x",j);
        for(i=1; i<=dimX; ++i)
        {
            printf("%c",maze[j][i]);
        }
    }
    printf("\n");
}
