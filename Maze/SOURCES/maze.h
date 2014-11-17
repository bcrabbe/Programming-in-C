//
//  maze.h
//  
//
//  Created by ben on 15/11/2014.
//
//

#ifndef _maze_h
#define _maze_h


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>


#define WWIDTH 600
#define WHEIGHT 600
#define RECTSIZE 20
#define MILLISECONDDELAY 0
#define SDL_8BITCOLOUR 256

#define TEST_DIVIDE_CHAMBER 0
#define ENABLE_RANDOM_SELECT_GRID_SIZE 1

#if ENABLE_RANDOM_SELECT_GRID_SIZE
#define RANDOM_SIZE 25
#endif


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


#endif
