//
//  history.c
//  conwayPegs
//
//  Created by ben on 02/12/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pegs.h"
#include <string.h>



/*  Prints a boards history table to the screen 
 */
void printHistory(int boardID)
{
    printf("Board %d History:\n",boardID);
    storedBoard * current = getBoardFromID(boardID);
    int numberOfBranches = current->history[0][0];
    for(int branch=1; branch<=numberOfBranches; ++branch) {
        printf("Branch %d\t\t",branch);
        int numberOfParentsInBranch = current->history[branch][0];
        for(int parentIter = 1; parentIter<=numberOfParentsInBranch;
            ++parentIter) {
            printf(" %d",current->history[branch][parentIter]);
        }
        printf(".\n");
    }
}

/*  Called when adding new board to list
    this duplicates the history table of the parent board 
 */
int ** duplicateHistory(int **parentHistory)
{
    //get number of branches in parent history
    size_t numberOfRows = (size_t)parentHistory[0][0]+1;
    //we need this many rows + 1 for the [0][0]
    int ** newHistory = (int**)calloc(numberOfRows, sizeof(int*));
    if(newHistory==NULL) {
        fprintf(stderr,"ERROR: calloc 1 in duplicateHistory failed.\n");
        exit(1);
    }
    //make first 'row' this has just one column holding the number of rows in
    //the table
    newHistory[0] = (int*)calloc((size_t)1, sizeof(int));
    if(newHistory[0]==NULL) {
        fprintf(stderr,"ERROR: calloc 2 in duplicateHistory failed.\n");
        exit(1);
    }
    newHistory[0][0] = (int)numberOfRows-1;//holds the number of branches in the history
    for(int branch=1; branch<=newHistory[0][0]; ++branch) {
        size_t numberOfColumnsInBranch = (size_t)parentHistory[branch][0]+1;
        newHistory[branch] = (int*)calloc(numberOfColumnsInBranch, sizeof(int));
        int numberOfParentsInBranch = parentHistory[branch][0];
        newHistory[branch][0]=numberOfParentsInBranch;
        for(int parentIter = 1; parentIter<=numberOfParentsInBranch; ++parentIter) {
            newHistory[branch][parentIter] = parentHistory[branch][parentIter];
        }
    }
    return newHistory;
}
/* adds the int BoardID to the history table
 */
void addToHistory(int BoardID, int ** history)
{
    int numberOfBranches = history[0][0];
    for(int j=1; j<=numberOfBranches; ++j)
    {
        size_t numberOfParentsInBranch = history[j][0];
        ++numberOfParentsInBranch;//we add a new elem to each branch
        size_t numberOfRowsInBranch = numberOfParentsInBranch+1;
        //all the rents + the 0 element
        //this adds an extra element of memory...
        int * tmp=(int*)realloc(history[j], numberOfRowsInBranch*sizeof(int));
        if(tmp==NULL) {
            fprintf(stderr,"ERROR: realloc in addToHistory failed.\n");
            exit(2);
        } else {
            history[j] = tmp;
        }
        history[j][numberOfParentsInBranch] = BoardID;
        //update the number of parents...
        history[j][0]=(int)numberOfParentsInBranch;
    }
}





/*  Called when a new board is found to match one already in the list
    it adds the  history table of boardAddingHistoryFromID to boardAddingHistoryToID
*/
void mergeHistories(int boardAddingHistoryToID, int boardAddingHistoryFromID)
{

    storedBoard * boardAddingTo = getBoardFromID(boardAddingHistoryToID),
                * boardAddingFrom = getBoardFromID(boardAddingHistoryFromID);
    int oldNumberOfBranches = boardAddingTo->history[0][0];
    boardAddingTo->history[0][0] = oldNumberOfBranches + boardAddingFrom->history[0][0];
    int newNumberOfRows =  boardAddingTo->history[0][0] + 1;
    int ** tmp;
    tmp = (int**)realloc(boardAddingTo->history,
                                        (newNumberOfRows)*sizeof(int*));
    if(tmp==NULL) {
        fprintf(stderr,"ERROR: realloc 1 in mergeHistories failed.\n");
        exit(4);
    } else {
        boardAddingTo->history = tmp;
    }
    
    for(int newBranch=oldNumberOfBranches+1; newBranch<=boardAddingTo->history[0][0];
        ++newBranch) {
        int branchBeingCopiedFromParent = newBranch - oldNumberOfBranches;
        //number of rows in new branch = numberOfRowsInParentBranch + 1 for the
        //new ID that needs adding
        
        boardAddingTo->history[newBranch] = (int*)calloc((size_t)
                    (boardAddingFrom->history[branchBeingCopiedFromParent][0]+2),
                                                   sizeof(int));
        if(boardAddingTo->history[newBranch]==NULL) {
            fprintf(stderr,"ERROR: calloc 1 in mergeHistories failed.\n");
            exit(4);
        }  //the new branch has same number of parents + 1 for this board itself
        boardAddingTo->history[newBranch][0] =
        boardAddingFrom->history[branchBeingCopiedFromParent][0] + 1;
        for(int parentIter=1;
            parentIter<=boardAddingFrom->history[branchBeingCopiedFromParent][0];
            ++parentIter) {
            boardAddingTo->history[newBranch][parentIter] =
            boardAddingFrom->history[branchBeingCopiedFromParent][parentIter];
        }
        //and the final parent is the ID of this matchingBoard...
        boardAddingTo->history[newBranch][boardAddingTo->history[newBranch][0]] =
        boardAddingHistoryToID;
    }
}

/*  Call to free the alloced memory in a history table
 */
void freeHistory(int **history)
{
    for(int j=history[0][0]; j>=0; --j ) {
        free(history[j]);
    }
    free(history);
}

