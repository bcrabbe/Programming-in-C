//
//  mazeGenerater.c
//  
//
//  Created by ben on 15/11/2014.
//
//

#include "maze.h"

/* Creates an array and intialises it ready for random division */
unsigned char ** generateRandomMaze( int *dimensionX, int *dimensionY)
{
#if ENABLE_RANDOM_SELECT_GRID_SIZE
    int dimX, dimY;
    printf("Please enter the dimensions of the maze you would like to generate: formated x y \n");
    while(scanf("%d %d",&dimX,&dimY)!=2 || dimX<5 || dimY<5)
    {
        printf("failed to read dimensions, please enter two integers greater than 5: \n");
    }
#else
    int dimX=RANDOM_SIZE,
    dimY=RANDOM_SIZE;
#endif
    
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
/* recursive method in random division generation.
 *
 * minX is the minimum x coordinate that a vertical dividing line can be drawn
 * from and similarly for the rest.
 */
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
#if TEST_DIVIDE_CHAMBER
    testDivideChamber(maze, numberOfDivides,verticalXcoord,horizontalYcoord,minX,minY,maxX,maxY );
#endif
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


/* testing function from divideChamber function */
void testDivideChamber(unsigned char ** maze, int numberOfDivides,
                       int verticalXcoord, int horizontalYcoord, int minX,
                       int minY, int maxX, int maxY )
{
    printf("\n>>>>>>>>>>>>>divide mase call number %d\n",numberOfDivides);
    printf(">>>>>>>>>>>>>>>verticalXcoord %d horizontalYcoord %d\n",verticalXcoord,horizontalYcoord);
    
    printMaze(maze, RANDOM_SIZE, RANDOM_SIZE);
    
}
