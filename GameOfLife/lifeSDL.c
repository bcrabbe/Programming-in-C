`/************************************************************************************
	The Game of Life

	Users may specify a file name as a commandline argument. The file should
	specify grid size (number of column and number of rows)on the first line and
	a board of these dimensions containing - and # representing dead and alive 
	respectively. If no file is specified then the user will be asked to specify 
	the size of the board they want and a square board of this size 
	will be populated randomly.
************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WWIDTH 800
#define WHEIGHT 800
#define RECTSIZE 20
#define MILLISECONDDELAY 50
#define SDL_8BITCOLOUR 256

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

	// All info required for windows / renderer & event loop
struct SDL_Simplewin {
	SDL_bool finished;
	SDL_Window *win;
	SDL_Renderer *renderer;
};
typedef struct SDL_Simplewin SDL_Simplewin;

void Neill_SDL_Init(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);

unsigned char **createGrid(int dimX, int dimY);
void randomiseGrid(unsigned char **grid, int dimX, int dimY);
void printGrid(unsigned char **grid, int dimX, int dimY);
void freeGrid(unsigned char **grid);
void readFile(unsigned char **grid, int dimensionX, int dimensionY, FILE *FP);
int countNeighbours(unsigned char **grid, int i, int j);
int returnCellState(unsigned char **grid, int i, int j);
void turnCellOn(unsigned char **grid, int i, int j);
void turnCellOff(unsigned char **grid, int i, int j);
void advanceGrid(unsigned char **grid, unsigned char **grid2, int dimensionX, int dimensionY);
void doGraphics(unsigned char **grid,int dimensionX, int dimensionY, SDL_Simplewin *sw, SDL_Rect *rectangle);
int main(int argc, const char * argv[])
{
	
	SDL_Simplewin sw;
	SDL_Rect rectangle;
	
	

	
	Neill_SDL_Init(&sw);
	
    unsigned char **board=NULL, **board2=NULL;
   	int dimensionX, dimensionY;//holds number of columns, number of rows
	if(argc==1)//if no file 
	{
			//printf("Please enter the size of the board: ");
			//scanf("%d",&dimensionX);//gets grid size from user
		dimensionX=750;
		dimensionY=550;//in this case we create square grid
		board = createGrid(dimensionX, dimensionY);//function to allocate 2D array
   		board2 = createGrid(dimensionX, dimensionY);
		if( board==NULL || board2==NULL )
		{
			printf("failed to allocate grids\n");
			exit(0);
		}
		randomiseGrid(board, dimensionX, dimensionY);//initialises board to a random pattern
	}
	else if(argc==2)//allows the user to specify a file to read with a command line argument
	{
	 	FILE * FP;
		FP = fopen(argv[1],"r");
		if( FP==NULL )
		{
		    printf("failed to open files\n");
			exit(0);
		}
		if(fscanf(FP,"%d %d", &dimensionX, &dimensionY)!=2)
		{
			printf("failed to read dimensions from file from file\n");
			exit(0);
		}
		board = createGrid(dimensionX, dimensionY);
   		board2 = createGrid(dimensionX, dimensionY);
		if( board==NULL || board2==NULL )
		{
			printf("failed to allocate grids\n");
			exit(0);
		}
		readFile(board, dimensionX, dimensionY, FP);//reads the pattern in from file
 		fclose(FP);
	}
	

	
		//printf("\nplease enter the number of generation you want to see:");
	int numberOfGenerations=100000;
		//scanf("%d",&numberOfGenerations);
    for(int n=1; n<=numberOfGenerations; ++n)
    {
        advanceGrid(board, board2, dimensionX, dimensionY);
			//printGrid(board, dimensionX, dimensionY);
		doGraphics(board, dimensionX, dimensionY, &sw,&rectangle);
    }
    freeGrid(board);
    freeGrid(board2);
    atexit(SDL_Quit);
    return 0;
}
void doGraphics(unsigned char **grid,int dimensionX, int dimensionY, SDL_Simplewin *sw,SDL_Rect *rectangle)
{
		//SDL_Delay(MILLISECONDDELAY);
	rectangle->w = WWIDTH/dimensionX;
	rectangle->h = WHEIGHT/dimensionY;
	int i,j;
    for(j=1; j<=dimensionY; ++j)
    {
        for(i=1; i<=dimensionX; ++i)
        {
			if(returnCellState(grid, i, j) )
			{
				Neill_SDL_SetDrawColour(sw,255,255,255);
			}
			else
			{
				
				Neill_SDL_SetDrawColour(sw,0,0,0);
			}
			rectangle->x = i*rectangle->w;
			rectangle->y = j*rectangle->h;
			SDL_RenderFillRect(sw->renderer, rectangle);
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
		exit(0);
	}
	//here we allocate the proper array which will hold each cell status
    grid[0]=(unsigned char *)cal
    loc((size_t)(numberOfRows*numberOfColumns), sizeof(unsigned char));
	if(grid[0]==NULL)
	{
		printf("calloc failed\n");
		exit(0);
	}
	// here we set each pointer in the first array to the first element of each row
    for(int i=1; i<=(dimY+2); ++i)
    {
        grid[i]=grid[i-1]+numberOfColumns;
    }
		//return pointer to array of pointers to rows
    return grid;
}

void randomiseGrid(unsigned char **grid, int dimX, int dimY)
{
	srand(time(NULL));
	int i,j;
    for(j=1; j<=dimY; ++j)
    {
        for(i=1; i<=dimX; ++i)
        {
			grid[j][i]=rand()%2;
        }
    }
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
    printf("\n");
}


/* Frees a grid allocated with allocateGrid function*/
void freeGrid( unsigned char **grid)
{
    free((grid[0]));//frees the large char array malloc
    free((grid));//frees the smaller pointer array malloc
}

/* reads from file and copies to grid. File must have dimensionX dimensionY as its first line
followed by the a board of -(=off) or #(=on) */
void readFile( unsigned char **grid, int dimensionX, int dimensionY, FILE *FP)
{
    printf("\n\n input FILE:\n\n");
    int i,j;
    char fileChar;
    for(j=1; j<=dimensionY; ++j)
    {
        for(i=1; i<=dimensionX; ++i)
        {
            if( fscanf(FP,"%c", &fileChar)!=1 )
			{
				printf("in readFile fscanf failed");
				exit(0);
			}
            if(fileChar == '\n')//deals with stray new lines left in the buffer
            {
                if(fscanf(FP,"%c", &fileChar)!=1)
				{
					printf("in readFile fscanf failed");
					exit(0);
				}
            }
			putchar(fileChar); //print the board to screen as we go
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
                exit(0);		
            }
        }
		printf("\n");	
    }
}
/* when advancing the grid this function determines the number of 'on' neighbours for a cell (i,j)*/
int countNeighbours(unsigned char **grid, int i, int j)
{
    int neighbourCount=0;  //returnCellState returns 1 if neighbour is on 0 is off
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
/* this function returns 1 if grid[j][i] is alive 0 if dead.
	we use the least significant bit to store the status of the cell
	choose this as rest of the bits may be used to store a neighbour count 
	or some other data in a more optimised method*/
int returnCellState(unsigned char **grid, int i, int j)
{
    if( grid[j][i] & 1 )//this tests the least significant bit 
    {					//and executes if it is set to 1
        return 1;					
    }								
    else
    {
        return 0;
    }
}

void turnCellOn(unsigned char **grid, int i, int j)
{
    grid[j][i] = ( grid[j][i] |  1 );//sets least sig bit to 1, leaves the rest unchanged
}

void turnCellOff(unsigned char **grid, int i, int j)
{
   //sets least sig bit to 0, leaves the rest unchanged
	 grid[j][i] = ( grid[j][i] &  254 );//254 is 1111 1110 in binary
}


void advanceGrid(unsigned char **grid, unsigned char **grid2, int dimensionX, int dimensionY)
{
    int i,j;
    /*Sets Padding cells around edges to loop to other side */
    for(j=1; j<=dimensionY; ++j)//does left and rightmost edges
    {
        grid[j][0]=grid[j][dimensionX];
        grid[j][dimensionX+1]=grid[j][1];
    }
    for(i=1; i<=dimensionX; ++i)//does top and bottom edges
    {
        grid[0][i]=grid[dimensionY][i];
        grid[dimensionY+1][i]=grid[1][i];
    }
	//and for the corners
    grid[0][0] = grid[dimensionY][dimensionX];
    grid[dimensionY+1][dimensionX+1] = grid[1][1];
    grid[dimensionY+1][0] = grid[1][dimensionX];
	grid[0][dimensionX+1] = grid[dimensionY][1];

    for(j=1; j<=dimensionY; ++j)
    {
        for(i=1; i<=dimensionX; ++i)
        {
            int neighbourCount = countNeighbours(grid, i, j);
			int cellState = returnCellState( grid, i, j);//alive cell =1 dead =0
            if( cellState==1 && ( neighbourCount==2 || neighbourCount==3 ) )
            {//if an on cell has 2 or 3 neighbours = Survival
                turnCellOn( grid2, i, j);
            }
            else if( cellState==1 && neighbourCount!=3 && neighbourCount!=2 )
            {//on cell with a number of neighbours other than 2 or 3 = Death
                turnCellOff( grid2, i, j);
            }
			else if( cellState==0 && neighbourCount==3 )
			{//off cell with 3 neighbours = birth	
                turnCellOn( grid2, i, j);
			}
			else
			{//cell stays dead
                turnCellOff( grid2, i, j);
			}
        }
    }
	//here we swap the pointers in grid2 and grid1
	//this interchanges the arrays.
    unsigned char *tmp1=NULL;
    for(i=0; i<=(dimensionY+1); ++i)
    {
        tmp1 = grid[i];
        grid[i]=grid2[i];
        grid2[i]=tmp1;
    }
    unsigned char **tmp2=NULL;
    tmp2 = grid;
    grid = grid2;
    grid2 = tmp2;
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
	
		// Set screen to black
	Neill_SDL_SetDrawColour(sw, 0, 0, 0);
	SDL_RenderClear(sw->renderer);
	SDL_RenderPresent(sw->renderer);
	
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
					//case SDL_MOUSEBUTTONDOWN:
			case SDL_KEYDOWN:
				sw->finished = 1;
		}
    }
}


	// Trivial wrapper to avoid complexities of renderer & alpha channels
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b)
{
	
	SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);
	
}



