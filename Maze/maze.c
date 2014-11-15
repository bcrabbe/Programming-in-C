/************************************************************************************
Recursive maze solver
---------------------
 
 

 
************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <assert.h>
//#define NDEBUG

#define WWIDTH 600
#define WHEIGHT 600
#define RECTSIZE 20
#define MILLISECONDDELAY 0
#define SDL_8BITCOLOUR 256
#define RANDOM_SIZE 50
typedef struct SDL_Simplewin
{
	SDL_bool finished;
	SDL_Window *win;
	SDL_Renderer *renderer;
} SDL_Simplewin;

void freeGrid( unsigned char **grid);
unsigned char **createMaze( int dimX, int dimY);
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
    
    
    printMaze(maze, dimensionX, dimensionY);

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
    freeGrid(maze);

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
                SDL_SetRenderDrawColor(sw->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
			}
            else if(maze[j][i]=='.')
            {
                SDL_SetRenderDrawColor(sw->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
            }
            else if(maze[j][i]=='x')
            {
                SDL_SetRenderDrawColor(sw->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            }
            else
            {
                SDL_SetRenderDrawColor(sw->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

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
   /*int dimX, dimY;
    printf("Please enter the dimensions of the maze you would like to generate: formated x y \n");
    while(scanf("%d %d",&dimX,&dimY)!=2 || dimX<5 || dimY<5)
    {
        printf("failed to read dimensions, please enter two integers greater than 5: \n");
    }*/
    int dimX=RANDOM_SIZE,
        dimY=RANDOM_SIZE;
    
    unsigned char ** maze = createMaze(dimX, dimY);
    
    /* now block the boarders */
    for(int i=1; i<=dimX; ++i)
    {
        maze[1][i]='#';
        maze[dimY][i]='#';

    }
    for(int j=1; j<=dimY; ++j)
    {
        maze[j][1]='#';
        maze[j][dimX]='#';
    }
    maze[2][1]='s';//make an entrance in top left
    maze[dimY-1][dimX]=' ';//make an exit in bottom right
    for(int j=2; j<=dimY-1; ++j)
    {
        for(int i=2; i<=dimX-1; ++i)
        {
            maze[j][i]=' ';
        }
    }
    printMaze(maze,dimX,dimY );

    if(divideChamber(maze, 3, 3, dimX-2, dimY-2)==1)
    {
        *dimensionX=dimX;
        *dimensionY=dimY;
        return maze;
    }
    else
    {
        fprintf(stderr,"random maze generator failed\n");
        exit(5);
    }
}

void testDivideChamber(unsigned char ** maze, int numberOfDivides, int verticalXcoord,
                        int horizontalYcoord, int minX, int minY, int maxX, int maxY )
{
    printf("\n>>>>>>>>>>>>>divide mase call number %d\n",numberOfDivides);
    printf(">>>>>>>>>>>>>>>verticalXcoord %d horizontalYcoord %d\n",verticalXcoord,horizontalYcoord);

    printMaze(maze, RANDOM_SIZE, RANDOM_SIZE);
    
}

int divideChamber(unsigned char ** maze, int minX, int minY, int maxX, int maxY)
{
    static int numberOfDivides;
    ++numberOfDivides;
    
    if(maxX-minX<0 || maxY-minY<0 )
    {
        return 1;
    }
    int topOfChamber = minY-1,
        bottomOfChamber = maxY+1,
        leftOfChamber = minX-1,
        rightOfChamber = maxX+1;
    
    
    int verticalXcoord = maxX-minX==0 ? minX : minX + rand()%(maxX-minX+1);//the xcoord of the vertical dividing line
    int horizontalYcoord = maxY-minY==0 ? minY : minY + rand()%(maxY-minY+1);//the y coord of the horizontal dividing line
    maze[horizontalYcoord][verticalXcoord] = '#';//block the central point
    
    int wallToLeaveWhole = rand()%4 ;//random number 0-3 picks one of the 4 wall segments
    int wallCounter=0;               //iterates as we draw walls
    int spaceToLeave;
    int lineStart, lineEnd, lineLength;
    /*********draw top part of vertical wall:**********/
    //line goes from top of chamber to 1 before the y coord of the horizontalLine,
    //a random num between these bounds is:
    lineStart=topOfChamber;
    lineEnd= horizontalYcoord-1;
    lineLength= lineEnd-lineStart;
    spaceToLeave = (lineLength)>1 ?  lineStart + rand()%(lineLength) : lineStart;
    for(int j=lineStart; j<=lineEnd; ++j)
    {
        if(j==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[j][verticalXcoord]=' ';
            maze[j][verticalXcoord-1]=' ';//stops the gap being blocked
            maze[j][verticalXcoord+1]=' ';
        }
        else maze[j][verticalXcoord]='#';
    }
    //this stops the blocking of gaps
    if( maze[lineStart-1][verticalXcoord]==' ' )
    {
        maze[lineStart][verticalXcoord]=' ';
    }
    if(maze[lineEnd+1][verticalXcoord]==' ' )
    {
        maze[lineEnd][verticalXcoord]=' ';
    }
    ++wallCounter;
    /*********draw bottom part of vertical wall:**********/
    //line goes from 1 below the y coord of the horizontal Line to bottom of the chamber
    lineStart=horizontalYcoord+1;
    lineEnd= bottomOfChamber;
    lineLength= lineEnd-lineStart;
    spaceToLeave = (lineLength)>1 ?  lineStart + rand()%(lineLength) : lineStart;
    for(int j=lineStart; j<=lineEnd; ++j)
    {
        if(j==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[j][verticalXcoord]=' ';
            maze[j][verticalXcoord-1]=' ';
            maze[j][verticalXcoord+1]=' ';
        }
        else maze[j][verticalXcoord]='#';
    }
    if( maze[lineStart-1][verticalXcoord]==' ' )
    {
        maze[lineStart][verticalXcoord]=' ';
    }
    if(maze[lineEnd+1][verticalXcoord]==' ' )
    {
        maze[lineEnd][verticalXcoord]=' ';
    }

    ++wallCounter;
    /*********draw left part of Horizontal wall:**********/
    //line goes from leftOfChamber to the x coord of the vertical Line -1
    lineStart=leftOfChamber;
    lineEnd= verticalXcoord-1;
    lineLength= lineEnd-lineStart;
    spaceToLeave = (lineLength)>1 ?  lineStart + rand()%(lineLength) : lineStart;
    for(int i=lineStart; i<=lineEnd; ++i)
    {
        if(i==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[horizontalYcoord][i]=' ';
            maze[horizontalYcoord-1][i]=' ';
            maze[horizontalYcoord+1][i]=' ';


            
        }
        else maze[horizontalYcoord][i]='#';
    }
    if( maze[horizontalYcoord][lineStart-1]==' ' )
    {
        maze[horizontalYcoord][lineStart]=' ';
    }
    if(maze[horizontalYcoord][lineEnd+1]==' ' )
    {
        maze[horizontalYcoord][lineEnd]=' ';
    }
    ++wallCounter;
    /*********draw right part of horizontal wall:**********/
    //line goes from the x coord of the vertical Line+1  to chamberRight
    lineStart=verticalXcoord+1;
    lineEnd= rightOfChamber;
    lineLength= lineEnd-lineStart;
    spaceToLeave = (lineLength)>1 ?  lineStart + rand()%(lineLength) : lineStart;
    for(int i=lineStart; i<=lineEnd; ++i)
    {
        if(i==spaceToLeave && wallCounter!=wallToLeaveWhole)
        {
            maze[horizontalYcoord][i]=' ';
            maze[horizontalYcoord-1][i]=' ';
            maze[horizontalYcoord+1][i]=' ';
        }
        else maze[horizontalYcoord][i]='#';
        
    }
    if( maze[horizontalYcoord][lineStart-1]==' ' )
    {
        maze[horizontalYcoord][lineStart]=' ';
    }
    if(maze[horizontalYcoord][lineEnd+1]==' ' )
    {
        maze[horizontalYcoord][lineEnd]=' ';
    }
    testDivideChamber(maze, numberOfDivides,verticalXcoord,horizontalYcoord,minX,minY,maxX,maxY );

    // Now call divide again the 4 chambers untill they all return 1
    if(//                  minX/maxX          minY/maxY
       divideChamber(maze, minX,              minY,
                           verticalXcoord-2,  horizontalYcoord-2)//top left
       &&
       divideChamber(maze, verticalXcoord+2,  minY,
                           rightOfChamber-1,  horizontalYcoord-2)//top right
        &&
        divideChamber(maze, minX,             horizontalYcoord+2,
                            verticalXcoord-2, bottomOfChamber-1)//bottom left
        &&
        divideChamber(maze, verticalXcoord+2, horizontalYcoord+2,
                            rightOfChamber-1,bottomOfChamber-1)//bottom right
       )
    {
        return 1;
    }
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
    if(graphicsAreOn(0))
    {
        SDL_Simplewin * sw = getSdlWindowPtr(NULL);
        doGraphics(maze, dimX, dimY, sw);
    }
    //testPathFinder(i,j,maze);
 
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
    //testPathFinder(i,j,maze);
    if(pathFinder(i, j-1, maze, dimX, dimY)==1) return 1;//move downwards
    //testPathFinder(i,j,maze);
    if(pathFinder(i, j+1, maze, dimX, dimY)==1) return 1;//move up
    //testPathFinder(i,j,maze);
    if(pathFinder(i-1, j, maze, dimX, dimY)==1) return 1;//move left
    //testPathFinder(i,j,maze);
    
    //if we get here then i,j is not part of solution path
    //so we un mark it:
    
    if(maze[j][i]!='s')maze[j][i]='x';

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
    maze = createMaze(*dimensionX, *dimensionY);
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
    int i,j,newlineRead=0;
    char fileChar;
    for(j=1; j<=dimensionY; ++j)
    {
        newlineRead=0;
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
                if(fileChar == '\n') ++newlineRead;
                
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
            if(newlineRead!=1)
            {
                fprintf(stderr,"ERROR: File dimensions do not match top line\n");
                fclose(FP);
                exit(2);
            }
        }
		printf("\n");
    }
}

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
            if(maze[j][i]!='x')
            {
                printf("%c",maze[j][i]);
            }
        }
    }
    printf("\n");
}
