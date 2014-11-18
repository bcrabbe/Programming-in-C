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
    if(scanf("%d %d",&dimX,&dimY)!=2 || dimX<5 || dimY<5)
    {
        printf("failed to read dimensions, please enter two integers greater than 5: \n");
		if(scanf("%d %d",&dimX,&dimY)!=2 || dimX<5 || dimY<5)
    	{	
       		 printf("failed to read dimensions, Exiting. \n");
			 exit(1);
   		}
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
    static int numberOfDivides;//for testing
    ++numberOfDivides;
    if(maxX-minX<0 || maxY-minY<0 )//if there are no availible places for chamber division 
    {
        return 1;
    }
    int topOfChamber = minY-1,//the minimum y coordinate that the dividing line must be drawn from
    	bottomOfChamber = maxY+1,//and similarly for the rest
     	leftOfChamber = minX-1,
    	rightOfChamber = maxX+1,
    	verticalXcoord = maxX-minX==0 ? minX : minX + rand()%(maxX-minX+1),//the xcoord of the vertical dividing line
    	horizontalYcoord = maxY-minY==0 ? minY : minY + rand()%(maxY-minY+1);//the y coord of the horizontal dividing line

    maze[horizontalYcoord][verticalXcoord] = '#';//block the intersection of the dividing lines
    
    int wallToLeaveWhole = rand()%4,   //random number 0-3 picks one of the 4 wall segments 
        wallCounter = 0,               //counts as we draw walls
        leaveSpace,
        lineStart, 
		lineEnd;
    /*********draw top part of vertical wall:**********/
    lineStart = topOfChamber;
    lineEnd = horizontalYcoord-1;
	leaveSpace = wallCounter!=wallToLeaveWhole ? 1 : 0;
	drawVerticalDivider( maze, lineStart,lineEnd,verticalXcoord, leaveSpace);
    ++wallCounter;
    /*********draw bottom part of vertical wall:**********/
    lineStart = horizontalYcoord+1;
    lineEnd = bottomOfChamber;
  	leaveSpace = wallCounter!=wallToLeaveWhole ? 1 : 0;
	drawVerticalDivider( maze, lineStart,lineEnd,verticalXcoord, leaveSpace);
    ++wallCounter;
    /*********draw left part of Horizontal wall:**********/
    lineStart = leftOfChamber;
    lineEnd = verticalXcoord-1;
	leaveSpace = wallCounter!=wallToLeaveWhole ? 1 : 0;
	drawHorizontalDivider( maze, lineStart,lineEnd,horizontalYcoord, leaveSpace);
    ++wallCounter;
    /*********draw right part of horizontal wall:**********/
    lineStart = verticalXcoord+1;
    lineEnd = rightOfChamber;
	leaveSpace = wallCounter!=wallToLeaveWhole ? 1 : 0;
	drawHorizontalDivider( maze, lineStart,lineEnd,horizontalYcoord, leaveSpace);
#if ENABLE_TEST_DIVIDE_CHAMBER
    testDivideChamber(maze, numberOfDivides, verticalXcoord,
		     		 horizontalYcoord, minX, minY, maxX, maxY );
#endif
    /*** Now call divide again the 4 chambers untill they all return 1***/

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
                    	   rightOfChamber-1, bottomOfChamber-1)//bottom right
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

void drawVerticalDivider(unsigned char ** maze, int lineStart, int lineEnd, int verticalXcoord, int leaveSpace)
{
	int lineLength = lineEnd-lineStart;
    int spaceToLeave=0;
    if(leaveSpace)
	{
		spaceToLeave = (lineLength)>1 ?  lineStart + rand()%(lineLength) : lineStart;
	};
    for(int j=lineStart; j<=lineEnd; ++j)
    {
        if(j==spaceToLeave && leaveSpace)
        {
            maze[j][verticalXcoord] = ' ';
            maze[j][verticalXcoord-1] = ' ';
            maze[j][verticalXcoord+1] = ' ';
        }
        else maze[j][verticalXcoord] = '#';
    }
    if( maze[lineStart-1][verticalXcoord]==' ' )
    {
        maze[lineStart][verticalXcoord] = ' ';
    }
    if(maze[lineEnd+1][verticalXcoord]==' ' )
    {
        maze[lineEnd][verticalXcoord] = ' ';
    }
    
}
void drawHorizontalDivider(unsigned char ** maze, int lineStart, int lineEnd, int horizontalYcoord, int leaveSpace)
{
	int lineLength = lineEnd-lineStart;
	int spaceToLeave=0;
    if(leaveSpace)
	{
		spaceToLeave = (lineLength)>1 ?  lineStart + rand()%(lineLength) : lineStart;
	}
    for(int i=lineStart; i<=lineEnd; ++i)
    {
        if(i==spaceToLeave && leaveSpace)
        {
            maze[horizontalYcoord][i] = ' ';
            maze[horizontalYcoord-1][i] = ' ';
            maze[horizontalYcoord+1][i] = ' ';
        }
        else maze[horizontalYcoord][i] = '#'; 
    }
    if( maze[horizontalYcoord][lineStart-1]==' ' )
    {
        maze[horizontalYcoord][lineStart] = ' ';
    }
    if(maze[horizontalYcoord][lineEnd+1]==' ' )
    {
        maze[horizontalYcoord][lineEnd] = ' ';
    }
}

/* testing function from divideChamber function 
  Enable by setting ENABLE_TEST_DIVIDE_CHAMBER to 1 in maze.h*/
void testDivideChamber(unsigned char ** maze, int numberOfDivides,
                       int verticalXcoord, int horizontalYcoord, int minX,
                       int minY, int maxX, int maxY )
{
    printf("\n>>>>>>>>>>>>>divide mase call number %d\n",numberOfDivides);
    printf(">>>>>>>>>>>>>>>verticalXcoord %d horizontalYcoord %d\n",verticalXcoord,horizontalYcoord);
    printf(">>>>>>>>>>>>>>>range x: %d to %d range y: %d to %d\n",minX,maxX,minY,maxY);
    
    printMaze(maze, RANDOM_SIZE, RANDOM_SIZE);
    
}
