//
//  pegs.h
//  conwayPegs
//
//  Created by ben on 28/11/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//

#ifndef conwayPegs_pegs_h
#define conwayPegs_pegs_h




#define BOARD_WIDTH 7
#define BOARD_HEIGHT 8
#define BOARD_NUMBER_OF_FILLED_ROWS 4
#define ENABLE_MARK_COLUMNS 1


#define PEG 0xFF
#define NO_PEG 0x00


//pegs.c
int checkTargetPositionForErrors(int * targetPosition);
unsigned char **createBoard();
void printBoard( unsigned char **board);
void freeBoard( unsigned char **board);
void run();
int findMoves(unsigned char ** board, int boardID);
int lookForMove(unsigned char ** parentBoard, int parentBoardID, int i, int j);
int movePeg(int from[2], int to[2], unsigned char ** board);
int copyBoard(unsigned char ** board, unsigned char ** dupBoard);
int * getTargetPosition(int * targetPosition);
int isAlreadyFinalState(int newBoardID);


//list.c

typedef struct list list;
typedef struct storedBoard storedBoard;

struct storedBoard {
    int boardID;//equals the position of the board in the list.
    
    int ** history;
    //We store the list positions of each of the boards parents in this array
    //additional rows hold seperate branchs that lead to the same board.
    //element [0][0] = nRow (contains the number of rows/branches in the table)
    //element [n][0] = nColr (contains the number of elements/parents in that row/branch )
    //elements [n][n] contain the ID's of the parent boards.
    
    unsigned char * compressedBoard;
    storedBoard * next;
};

struct list {
    unsigned int finished;
    unsigned int length;
    storedBoard * head;
    storedBoard * tail;
    storedBoard ** finalStates;
    unsigned int numberOfFinalStates;
};

int addBoardToList(unsigned char ** newBoard, unsigned int parentBoardID);
list * getListPtr(list * listAddress);
void initialiseList();
void listDestructor();
void freeHistory(int **history);
void printHistory(int boardID);
storedBoard * getBoardFromID(int boardID);
int ** duplicateHistory(int **parentHistory);
void addToHistory(int BoardID, int ** history);
void printStoredBoard(storedBoard * node);
void printList();
int compareBoards(unsigned char * compressedBoard1, unsigned char * compressedBoard2);
int matchesBoard(unsigned char * newBoardCompressed);
void printHistoryArray(int boardID);
void mergeHistories(int boardAddingHistoryToID, int boardAddingHistoryFromID);
char * getBranchString(storedBoard * board, int branch);
void addFinalState(int finalStateBoardID);
void printFinalStates();

//compressBoard.c:
void decompressBoard(unsigned char * compressedBoard, unsigned char ** board);
unsigned char * compressBoard(unsigned char **board);
void printCompressedBoard(unsigned char * compressedBoard);


//graphics
void displaySolutionSDL();



#endif
