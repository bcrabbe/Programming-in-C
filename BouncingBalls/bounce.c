#include <stdio.h>
#include <stdlib.h>
#include "neillsdl2.h"

#define RECTSIZE 20
#define MILLISECONDDELAY 20
#define GRAVITY 0//(9.81* MILLISECONDDELAY * 1E-5)




int main(void)
{

    SDL_Simplewin sw;
    int numberOfBalls=2;//balls iter for the ballArray, numberOfballs is total number of balls in system.
    Ball **ballArray=allocateBalls(numberOfBalls);//array of pointers to struct balls
    Neill_SDL_Init(&sw);
    do
    {
        drawBalls(ballArray, &sw, numberOfBalls);

        for(int balls=1; balls<=numberOfBalls; ++balls)
        {

            moveBalls(ballArray, numberOfBalls);
        }
    }while(!sw.finished);


   atexit( SDL_Quit );

   return 0;

}

Ball **allocateBalls( int numberOfBalls)
{
    Ball **ballArray=NULL;//array of pointers to struct balls
    ballArray=(Ball **)calloc((size_t)(numberOfBalls+1),sizeof(Ball*));//allocates space for enough pointers to all balls and one off the end just in case
    if(ballArray==NULL)
	{
		printf("calloc failed\n");
		exit(0);
	}
   
    ballArray = ballArray-1;//gives unit offset access

    for(int balls=1; balls<=numberOfBalls; ++balls)
    {
        ballArray[balls]=(Ball *)calloc((size_t)(numberOfBalls+1), sizeof(Ball));//allocates space for enough balls and one off the end just in case
        if(ballArray[balls]==NULL)
        {
            printf("calloc failed\n");
            exit(0);
        }
        
        *ballArray[balls]= (Ball)
        {
            0.3*balls,
            0.9,
            0,
            0.04-(0.020*balls),
            0.001*balls,
            0,
            0.02*balls,
            0.09,
            0,
        };
        // ballArray[balls]->r = ballArray[balls]->weight;

    }
    
    return ballArray;
    
}

void moveBalls( Ball **ballArray, int numberOfBalls )
{
    for(int balls=1; balls<=numberOfBalls; ++balls)
    {
        Ball * b = ballArray[balls];//the ball we are looking at now just called b
        b->dy = b->dy - GRAVITY;//v = u + at
        
        b->x = b->x + b->dx;
        b->y = b->y + b->dy;
        
        if( b->x > 1 - b->r )//right edge
        {
            b->dx = - b->dx;
            b->x = (1 - b->r) - (b->x - (1 - b->r));
        }
        if( b->x < b->r )//left edge
        {
            b->dx = - b->dx;
            b->x = b->r + (b->r - b->x);
        }
        if( b->y > 1 - b->r )//top edge
        {
            b->dy = - b->dy;
            b->y = (1 - b->r) - (b->y - (1 - b->r));
        }
        if( b->y < b->r)//bottom edge
        {
            b->dy = - (1-b->weight)*b->dy;
            b->y = b->r + (b->r - b->y);
        }
    }
    
    //now sort out colliding balls:
    for(int balls=1; balls<=numberOfBalls; ++balls)
    {
        for(int balls2=1; balls<=numberOfBalls; ++balls)
        {
            Ball * b = ballArray[balls];//the ball we are looking at now just called b
            Ball * b2 = ballArray[balls2];//the 2nd ball we are looking at now just called b2
            if( balls!=balls2 && !b->impactedCorrected && !b2->impactedCorrected)
            {
                //distnce between balls:
                double ballDistance = fabs(sqrt(pow(fabs(b->x - b2->x),2) + pow(fabs(b->y - b2->y),2)));
                //the minimum distance before collision:
                double ballRadiiSumed = b->r + b2->r;
                if( ballDistance<=ballRadiiSumed && !b->impactedCorrected && !b2->impactedCorrected )
                {
                    //  here we move the balls back along the paths they just followed untill they no longer overlap:
                    int n=1, maxPossibleSteps=5;
                    while(ballDistance<ballRadiiSumed && n<=maxPossibleSteps)
                    {
                        b->x = b->x - b->dx/maxPossibleSteps;
                        b->y = b->y - b->dy/maxPossibleSteps;
                        b2->x = b2->x - b2->dx/maxPossibleSteps;
                        b2->y = b2->y - b2->dy/maxPossibleSteps;
                        ballDistance = fabs(sqrt(pow(fabs(b->x - b2->x),2) + pow(fabs(b->y - b2->y),2)));
                        ++n;
                    }//if the balls still are not separated then we must move them manually:
                    //line going from b1x,b1y -> b2x,b2y ie conecting the centres has gradient:
                    double grad = (b2->y-b->y)/(b2->x-b->x);
                
                    if(ballDistance<ballRadiiSumed && n>maxPossibleSteps)
                    {//the distance they still overlap by:
                        double distanceToMove = ballRadiiSumed - ballDistance;
                        // then move them back along the line conecting the centres
                        //the angle this line makes with x axis is:
                        double thetaConectingLine = tan(1/grad);
                        b->x = b->x - cos(thetaConectingLine)*distanceToMove;
                        b->y = b->y - sin(thetaConectingLine)*distanceToMove;
                        b2->x = b2->x + cos(thetaConectingLine)*distanceToMove;
                        b2->y = b2->y + sin(thetaConectingLine)*distanceToMove;
                    }
                   
                
                    // the collision line (which is normal to this line) has gradient: m = -1/grad
                    
                    //the angle the collision surface makes with the x axis = theta = tan(1/m) = tan(-grad)
                    double theta = tan(-grad);
                    
                    //perpVels get reflected, tangeVels stay same.
                    double  b1PerpVel = -(-b->dx*sin(theta) + b->dy*cos(theta)),
                            b1TangeVel = (b->dx*cos(theta) + b->dy*sin(theta)),
                            b2PerpVel = -(-b2->dx*sin(theta) + b2->dy*cos(theta)),
                            b2TangeVel = (b2->dx*cos(theta) + b2->dy*sin(theta));
                    //then convert back to x,y velocities:
                    b->dx = (-b1TangeVel*cos(theta) + b1PerpVel*sin(theta));
                    b->dy = (b1TangeVel*sin(theta) + b1PerpVel*cos(theta));
                    b2->dx = (-b2TangeVel*cos(theta) + b2PerpVel*sin(theta));;
                    b2->dy = (b2TangeVel*sin(theta) + b2PerpVel*cos(theta));
                    
                  
                  
                    b->impactedCorrected=1;
                    b2->impactedCorrected=1;
                }
            }
        }
    }
    for(int balls=1; balls<=numberOfBalls; ++balls)
    {
        Ball * b = ballArray[balls];
        b->impactedCorrected=0;
    }
}



void drawBalls( Ball ** ballArray, SDL_Simplewin *sw, int numberOfBalls )
{
    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_SetDrawColour(sw, 0, 0, 0);
    SDL_RenderClear(sw->renderer);
 
    for(int balls=numberOfBalls; balls>=1; --balls)
    {
        Uint8 randColR =10+rand()%50;
        Uint8 randColG =120+rand()%130;
        Uint8 randColB =120+rand()%130;

        //Neill_SDL_SetDrawColour(sw,randColR,randColG,randColB);
         Neill_SDL_SetDrawColour(sw,240,100,20);

        Neill_SDL_RenderFillCircle(sw->renderer, ballArray[balls]->x*WWIDTH, (1-ballArray[balls]->y)*WHEIGHT, ballArray[balls]->r*WWIDTH);
        printf("ball %d coord %f, %f\n",balls,ballArray[balls]->x,ballArray[balls]->y);
        printf("dx dy %f, %f\n",ballArray[balls]->dx,ballArray[balls]->dy);

    }

    SDL_RenderPresent(sw->renderer);
    SDL_UpdateWindowSurface(sw->win);
    Neill_SDL_Events(sw);
}
