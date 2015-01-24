//
//  list.c
//  conwayPegs
//
//  Created by ben on 28/11/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pegs.h"

/* Call at the end to free the allocated memory in the list structure
 */
void listDestructor()
{
    list * boardList = getListPtr(NULL);
    storedBoard * current = boardList->head;
    while (current!=NULL)
    {
        freeHistory(current->history);
        free(current->compressedBoard);
        storedBoard * toBeFreed = current;
        current=current->next;
        free(toBeFreed);
    }
}


/* returns a point to the storeed board for the given ID
 */
storedBoard * getBoardFromID(int boardID)
{
    list * boardList = getListPtr(NULL);
    storedBoard * current = boardList->head;
    if(boardID > boardList->length)
    {
        fprintf(stderr, "ERROR: getBoardFromID call on boardID %d\n",boardID);
        fprintf(stderr, "there are only %d boards in the list\n",boardList->length);
        exit(3);
    }
    while(current->boardID != boardID)
    {
        current = current->next;
        if(current==NULL)
        {
            fprintf(stderr, "ERROR: getBoardFromID call on boardID %d\n",boardID);
            fprintf(stderr, "could not find board\n");
            exit(3);
        }
    }
    return current;
}


/* Sets up list adding the initial board as the first entry
 */
void initialiseList(unsigned char ** initialBoard)
{
    list * boardList = (list *)malloc(sizeof(list));
    if(boardList==NULL)
    {
        fprintf(stderr,"ERROR: malloc 1 in initialiseList failed.\n");
        exit(1);
    }
    storedBoard * newNode = (storedBoard *)malloc(sizeof(storedBoard));
    if(newNode==NULL)
    {
        fprintf(stderr,"ERROR: malloc 2 in initialiseList failed.\n");
        exit(1);
    }
    newNode->boardID = 1;
    newNode->compressedBoard = compressBoard(initialBoard);
    
    /**** Now to make table to store boards ancestors ****/
    //make array of pointers
    newNode->history = (int**)calloc((size_t)2, sizeof(int*));
    if(newNode->history==NULL)
    {
        fprintf(stderr,"ERROR: calloc 1 in initialiseList failed.\n");
        exit(1);
    }
    //make first 'row' this has just one column holding the number of rows in
    //the table
    newNode->history[0] = (int*)calloc((size_t)1, sizeof(int));
    if(newNode->history==NULL)
    {
        fprintf(stderr,"ERROR: calloc 2 in initialiseList failed.\n");
        exit(1);
    }
    newNode->history[0][0] = 1;//holds the number of branches in the history
                               //this is equal to the number of rows - 1
    //set first proper row
    newNode->history[1] = (int*)calloc((size_t)2, sizeof(int));
    newNode->history[1][0] = 1;//this holds the number of parents in this branch
                               //this equals the number of columns in this row - 1
    // this is this the first element of actual history:
    // for the first board this is just itself
    newNode->history[1][1] = newNode->boardID;

    boardList->length = 1;
    boardList->head = newNode;
    
    newNode->boardID = 1;
    newNode->next = NULL;
    boardList->tail = newNode;

    boardList->finished = 0;
    boardList->numberOfFinalStates=0;
    boardList->finalStates = NULL;
    
    getListPtr(boardList);//store ptr to boardList

}
/*  Call when a new board is made
    Checks if it already exists in the list,    if it does then the history of 
    the new board is merged with that that exists.
    if no matches are found then the new board is added to the end of the list
    the ID of whereever the board was added is returned
 */
int addBoardToList(unsigned char ** newBoard, unsigned int parentBoardID  )
{
    list * boardList = getListPtr(NULL);
    unsigned char * newBoardCompressed = compressBoard(newBoard);
    int matchingBoard = matchesBoard(newBoardCompressed);
    if( matchingBoard )
    {
        mergeHistories(matchingBoard, parentBoardID);
        free(newBoardCompressed);
        return matchingBoard;
    }
    else
    {
        boardList->length++;
        storedBoard * newNode = (storedBoard*)malloc(sizeof(storedBoard));
        newNode->boardID = boardList->length;//boardID=postition in the list
                                             //new boards will always be appended to list
        newNode->compressedBoard = compressBoard(newBoard);
        newNode->history = duplicateHistory(getBoardFromID(parentBoardID)->history);
        newNode->next = NULL;
        addToHistory(newNode->boardID, newNode->history);
        boardList->tail->next = newNode;
        boardList->tail = newNode;
        return  newNode->boardID;
    }
}

/*
 * given a new board (compressed) it returns the ID of a board already in the list
 * if one matches
 * and 0 if none matches
 */
int matchesBoard(unsigned char * newBoardCompressed)
{
    list * boardList = getListPtr(NULL);
    storedBoard * current = boardList->head;
    while (current != NULL)
    {
        if( compareBoards(current->compressedBoard, newBoardCompressed) )
        {
            return current->boardID;
        }
        current = current->next;
    }
    return 0;//does not match any
}
/*  Compares two compressed boards,
    returns 0 if not matching
    1 if matching
 */
int compareBoards(unsigned char * compressedBoard1, unsigned char * compressedBoard2)
{
    for(int row = 0; row<BOARD_HEIGHT; ++row)
    {
        if( compressedBoard1[row] != compressedBoard2[row] )
        {
            return 0;
        }
    }
    return 1;
}
/*  Called with listAddress =  list ptr in initialiseList
 this ptr is then stored for the duration of the program
 when we need to access the list in following functions call this with a NULL
 argument and it will return the stored list ptr
 
 WARNING: do not call with a none zero argument apart frpm at initialisation
 */
list * getListPtr(list * listAddress)
{
    static list * listPtr;
    if(listAddress!=NULL && sizeof(listAddress)==sizeof(list *))
    {
        listPtr = listAddress;
    }
    return listPtr;
}
/*  Given a node will print that board
 */
void printStoredBoard(storedBoard * node)
{
    printf("\n\nBoardID: %d\n",node->boardID);
    //printHistoryArray(node->boardID);
    printHistory(node->boardID);
    //printCompressedBoard(node->compressedBoard);
    unsigned char ** board = createBoard();
    decompressBoard(node->compressedBoard, board);
    printBoard(board);
}
/*  Prints the entire list
 */

void printList()
{
    printf("**** Printing List ****\n");
    list * boardList = getListPtr(NULL);
    storedBoard * current = boardList->head;
    while(current!=NULL)
    {
        printStoredBoard(current);
        current = current->next;
    }
    printf("**** End Printing List ****\n");
}

/* prints all the boards listed as final states
 */
void printFinalStates()
{
    list * boardList = getListPtr(NULL);
    for(int state = 1 ;  state<=boardList->numberOfFinalStates; ++state)
    {
        printStoredBoard(boardList->finalStates[state]);
    }
}






