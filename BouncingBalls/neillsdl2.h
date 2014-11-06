#include <SDL2/SDL.h>
#include <math.h>

#define WWIDTH 750
#define WHEIGHT 750

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

typedef struct ball Ball;


struct ball
{
	double  x,
            y,
            z,
            dx,
            dy,
            dz,
            weight, // (kg)
            r;//radius (m)
    int impactedCorrected;
};
void moveBalls(Ball **b, int numberOfBalls);
void drawBalls( Ball ** ballArray, SDL_Simplewin *sw, int numberOfBalls );
Ball **allocateBalls( int numberOfBalls);
