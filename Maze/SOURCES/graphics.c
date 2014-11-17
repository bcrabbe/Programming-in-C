//
//  graphics.c
//  
//
//  Created by ben on 15/11/2014.
//
//

#include "maze.h"


/*
 * if graphics are initialised this is called with the value 1.
 * This is stored in graphicsAreOn and this can be retrrieved by other functions
 * later to see if SDL is initialised.
 */
int graphicsAreOn(int setOn)
{
    static int graphicsAreOn;
    if(setOn)
        graphicsAreOn=1;
    
    return graphicsAreOn;
}

/* 
 * if graphics are initialised this is called with a ptr to the SDL window.
 * This is stored in window and can be retrrieved by other functions later 
 */
SDL_Simplewin * getSdlWindowPtr(SDL_Simplewin *sw)
{
    static SDL_Simplewin * window;
    
    if(sw!=NULL)
    {
        window = sw;
    }
    
    return window;
    
}
/* outputs an image of the maze to an SDL window */
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