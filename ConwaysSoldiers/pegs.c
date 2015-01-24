//
//  pegs.c
//  conwayPegs
//
//  Created by ben on 28/11/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pegs.h"


int main(int argc, const char * argv[])
{
    if(argc!=3) {
        fprintf(stderr, "ERROR: could not read target position\n");
        fprintf(stderr, "Run program followed by the integer x y  coordinates of the");
        fprintf(stderr, "position you would like to reach\n");
        return -1;
    }
    else {
        int * targetPosition = (int*)malloc(2*sizeof(int));
        targetPosition[0] = (int)strtol(argv[1],NULL,10);
        targetPosition[1] =  (int)strtol(argv[2],NULL,10);
        if(checkTargetPositionForErrors(targetPosition)) {
            targetPosition[1] = BOARD_HEIGHT - targetPosition[1] + 1;
            getTargetPosition(targetPosition);//stores targetPosition
        }
        else {
            return -1;
        }
    }
    
    run();
   
    return 0;
}

/* Error checking on user input targetPosition
 */
int checkTargetPositionForErrors(int * targetPosition)
{
    if(targetPosition[0]==0 ||targetPosition[1]==0)
    {
        fprintf(stderr, "ERROR: could not read target position\n");
        fprintf(stderr, "Run program followed by the integer x y  coordinates of the");
        fprintf(stderr, "position you would like to reach\n");
        return 0;
    }
    if( targetPosition[0] > BOARD_WIDTH )
    {
        fprintf(stderr, "X coordinate invalid.\n");
        fprintf(stderr, "Read %d but the program is ",targetPosition[0]);
        fprintf(stderr, "currently configured for only %d rows\n",BOARD_WIDTH);
        return 0;
    }
    if( targetPosition[0] < 1 )
    {
        fprintf(stderr, "X coordinate invalid.\n");
        fprintf(stderr, "Read %d but the first row is at x=1\n",targetPosition[0]);
        return 0;
    }
    if( targetPosition[1] > BOARD_HEIGHT )
    {
        fprintf(stderr, "Y coordinate invalid.\n");
        fprintf(stderr, "Read %d but the program is ",targetPosition[1]);
        fprintf(stderr, "currently configured with only %d columns\n",BOARD_HEIGHT);
        return 0;
    }
    if( targetPosition[1] < 1 )
    {
        fprintf(stderr, "Y coordinate invalid.\n");
        fprintf(stderr, "Read %d but the first row is at Y=1\n",targetPosition[1]);
        return 0;
    }
    if( targetPosition[1] < BOARD_NUMBER_OF_FILLED_ROWS )
    {
        printf("\n\nThere is a peg there by default\n Try another target...");
        return 0;
    }
    else {
        return 1;
    }
    
}


/* The main program 
 */
void run()
{
    unsigned char ** board = createBoard();
    printf("initial board:");
    printBoard(board);
    initialiseList(board);
    int * targetPosition = getTargetPosition(NULL);

    list * boardList = getListPtr(NULL);
    storedBoard * current = boardList->head;
    while(!boardList->finished && current!=NULL)
    {
        if(boardList->numberOfFinalStates>0)//if we have found final state
        {   //check untill finish boards in the same generation
            int currentGeneration = current->history[1][0]+1;
            if (currentGeneration > boardList->finalStates[1]->history[1][0])
            {
                boardList->finished=1;
                break;
            }
        }
        decompressBoard(current->compressedBoard, board);
        findMoves( board, current->boardID);
        printf("list Length = %d, current generation = %d \n",boardList->length,  current->history[1][0]+1);
       
        current = current->next;
    }
    //printList();
    if(!boardList->finished)
    {
        printf("\n\nThere are no routes to position %d, %d\n\n",targetPosition[0],
                                                            targetPosition[1]);
        return;
    }
    else {
        printf("\n\nThere were %d routes using %d moves to target %d, %d\n",
               boardList->numberOfFinalStates,boardList->finalStates[1]->history[1][0]-1,
               targetPosition[0],targetPosition[1]);
    }
    printFinalStates();
    displaySolutionSDL();

    listDestructor();
    freeBoard(board);
    free(targetPosition);    
}

/*  Called from main on each board in the list,
 searches each slot for a peg and then calls lookForMoves.
 returns 1 if a peg has moved to the target location
 0 otherwise.
 */
int findMoves(unsigned char ** board, int boardID)
{
    
    for(int j=1; j<=BOARD_HEIGHT; ++j)
    {
        for(int i=1; i<=BOARD_WIDTH; ++i)
        {
            if(board[j][i]==PEG )
            {
                if(lookForMove(board,boardID,i,j))
                {//lookForMove returns 1 if peg in target postion

                }
            }
        }
    }
    return 0;
}
/*  Called when a move is made to the target postion
    extends the list of finals states to accomodate the new boardID
*/
void addFinalState(int finalStateBoardID)
{
    list * boardList = getListPtr(NULL);
    
    boardList->numberOfFinalStates++;

    storedBoard ** tmp = (storedBoard**)realloc(boardList->finalStates,
                                 (1+boardList->numberOfFinalStates)*sizeof(storedBoard*));
    if(tmp==NULL) {
        fprintf(stderr,"ERROR: realloc in addFinalState failed.\n");
        exit(8);
    } else {
        boardList->finalStates = tmp;
    }

    boardList->finalStates[boardList->numberOfFinalStates] = getBoardFromID(finalStateBoardID);
}
/*  Called when a move is made to the target postion
    Checks the list of final states to see if the newBoard is already on there
    returns 1 if it is 0 if not
 */
int isAlreadyFinalState(int newBoardID)
{
    list * boardList = getListPtr(NULL);
    if(boardList->numberOfFinalStates==0) return 0;
    else {
        for(int i=1; i<=boardList->numberOfFinalStates; ++i) {
            if(boardList->finalStates[i]->boardID==newBoardID) {
                return 1;
            }
        }
        return 0;
    }
}
 
/*  Called for every peg on a board
 *  looks for possible moves in each direction
 *  returns the number of moves made
 */
int lookForMove(unsigned char ** parentBoard, int parentBoardID, int i, int j)
{
    // list * boardList = getListPtr(NULL);
    int * targetPosition = getTargetPosition(NULL);
    static unsigned char ** board;
    static int calls=0;
    ++calls;
    if(calls==1)
    {
        board = createBoard();
    }
    copyBoard(parentBoard, board);
    int moved=0;
    //look for move upwards:
    if( parentBoard[j-1][i] == PEG && parentBoard[j-2][i] == NO_PEG)
    {
        int moveFrom[2] = {i, j};
        int moveTo[2] = {i, j-2};
        movePeg(moveFrom, moveTo, board);
        ++moved;
        int newBoardID = addBoardToList(board, parentBoardID);
        copyBoard(parentBoard, board);
        if((moveTo[0] == targetPosition[0] && moveTo[1] == targetPosition[1]) &&
           !isAlreadyFinalState(newBoardID)) {
            addFinalState(newBoardID);
        }
    }
    //look right for move:
    if( parentBoard[j][i+1] == PEG && parentBoard[j][i+2] == NO_PEG)
    {
        int moveFrom[2] = {i, j};
        int moveTo[2] = {i+2, j};
        movePeg(moveFrom, moveTo, board);
        ++moved;
        int newBoardID = addBoardToList(board, parentBoardID);
        copyBoard(parentBoard, board);
        if((moveTo[0] == targetPosition[0] && moveTo[1] == targetPosition[1]) &&
            !isAlreadyFinalState(newBoardID)) {
            addFinalState(newBoardID);
        }
        
    }
    //look for move downwards:
    if( parentBoard[j+1][i] == PEG && parentBoard[j+2][i] == NO_PEG)
    {
        int moveFrom[2] = {i, j};
        int moveTo[2] = {i, j+2};
        movePeg(moveFrom, moveTo, board);
        ++moved;
        int newBoardID = addBoardToList(board, parentBoardID);
        copyBoard(parentBoard, board);
        if((moveTo[0] == targetPosition[0] && moveTo[1] == targetPosition[1]) &&
           !isAlreadyFinalState(newBoardID)) {
            addFinalState(newBoardID);
        }
    }
    //look left for move:
    if( parentBoard[j][i-1] == PEG && parentBoard[j][i-2] == NO_PEG)
    {
        int moveFrom[2] = {i, j};
        int moveTo[2] = {i-2, j};
        movePeg(moveFrom, moveTo, board);
        ++moved;
        int newBoardID = addBoardToList(board, parentBoardID);
        if((moveTo[0] == targetPosition[0] && moveTo[1] == targetPosition[1]) &&
          !isAlreadyFinalState(newBoardID)) {
            addFinalState(newBoardID);
        }
    }
    //freeBoard(board);
    return moved;
}
/*  Moves a peg from{COL, ROW} to{COL, ROW} and removes one from the in between
 */
int movePeg(int from[2], int to[2], unsigned char ** board)
{
    int pegBeingJumped[2] = { (from[0] + to[0])/2, (from[1] + to[1])/2 };
    
    board   [ from[1] ]            [ from[0] ]             = NO_PEG;
    board   [ pegBeingJumped[1] ]  [ pegBeingJumped[0] ]   = NO_PEG;
    board   [ to[1] ]              [ to[0] ]               = PEG;
    
    return 1;
}
/*  Copies the peg positions of board to dupBoard.
 Note this is ok because *board was allocated in one single malloc.
 */
int copyBoard(unsigned char ** board, unsigned char ** dupBoard)
{
    size_t	numberOfRows=BOARD_HEIGHT+2,//extra elements to store one cell thick outside edge
            numberOfColumns=BOARD_WIDTH+2;
    memcpy(dupBoard[0], board[0], numberOfColumns*numberOfRows);
    return 1;
}


/*  Allocates dynamic 2d array using pointer to array of pointers to rows.
    Fills element with 1111 1111 if peg and 0000 0000 if not peg
    Board is filled from bottom up with BOARD_NUMBER_OF_FILLED_ROWS filled rows.
 */
unsigned char **createBoard()
{
    int	numberOfRows=BOARD_HEIGHT+2,//extra elements to store one cell thick outside edge
        numberOfColumns=BOARD_WIDTH+2;
    unsigned char **board=NULL;
	//here we allocate an array to hold the pointers
    //these will point to the start of each row
    board = (unsigned char **)calloc((size_t)(numberOfRows), sizeof(unsigned char *));
	if(board==NULL)
	{
		printf("createBoard calloc 1 failed\n");
		exit(1);
	}
	//here we allocate the proper array which will hold each cell state
    board[0]=(unsigned char *)calloc((size_t)(numberOfRows*numberOfColumns),
                                     sizeof(unsigned char));
	if(board[0]==NULL)
	{
		printf("createBoard calloc 2 failed\n");
		exit(1);
	}
	// here we set each pointer in the first array to the first element of each row
    for(int i=1; i<numberOfRows; ++i)
    {
        board[i]=board[i-1]+numberOfColumns;
    }
    //now set the outside edges with x's to stop moves here
    //this is not part of the actual board
    for(int i=0; i<numberOfColumns; ++i)
    {
        board[0][i]='x';
        board[numberOfRows-1][i]='x';
    }
    for(int j=0; j<numberOfRows; ++j)
    {
        board[j][0]='x';
        board[j][numberOfColumns-1]='x';
    }
    //now set the filled bottom half of the board
    for(int i=1; i<=BOARD_WIDTH; ++i)
    {
        for(int j=BOARD_HEIGHT; j>BOARD_HEIGHT-BOARD_NUMBER_OF_FILLED_ROWS; --j)
        {
            board[j][i]=PEG;
        }
    }
    //return pointer to array of pointers to rows
    return board;
}

/*  Prints a board to screen
 set ENABLE_MARK_COLUMNS in pegs.h to 1 to have the columns and rows
 numbered. (note: they are marked with a hex number to stop the label for
 row/cols above 9 taking up 2 colums/rows)
 */
void printBoard( unsigned char **board)
{
    printf("\n");

#if ENABLE_MARK_COLUMNS
    for(int i=0; i<=BOARD_WIDTH; ++i)
    {
        printf("%x",i);
    }
#endif
    for(int j=1; j<=BOARD_HEIGHT; ++j)
    {
        printf("\n");
#if ENABLE_MARK_COLUMNS
		printf("%x",j);
#endif
        for(int i=1; i<=BOARD_WIDTH; ++i)
        {
            if(board[j][i]==NO_PEG)
            {
                printf(" ");
            }
            else if(board[j][i]==PEG)
            {
                printf("o");
            }
        }
    }
    printf("\n\n");
}



/*  Call at start of program with the targetPosition array to store its address
 can be called later with NULL argument to return the address for access
 
 WARNING: do not call with non  NULL pointer other than at initialisation
 */

int * getTargetPosition(int * targetPosition)
{
    static int * storedTargetPosition;
    if(targetPosition!=NULL)
    {
        storedTargetPosition = targetPosition;
    }
    return storedTargetPosition;
}


/* Frees a grid allocated with allocateGrid function*/
void freeBoard( unsigned char **board)
{
    free((board[0]));//frees the large char array calloc
    free((board));//frees the smaller pointer array calloc
}









