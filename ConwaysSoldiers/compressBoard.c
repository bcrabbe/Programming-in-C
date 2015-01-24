//
//  compressBoard.c
//  conwayPegs
//
//  Created by ben on 28/11/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "pegs.h"
/* Given a compressed board and a pre allocated regular board, this function 
    copies the compressed board state to the regular board
 */
void decompressBoard(unsigned char * compressedBoard, unsigned char ** board)
{
    for(int j=1; j<=BOARD_HEIGHT; ++j)
    {
        for(int i=1; i<=BOARD_WIDTH; ++i)
        {
            if( (compressedBoard[j-1] & (1 << (BOARD_WIDTH-i)) ) ==
                                                        (1 << (BOARD_WIDTH-i)))
            {
                board[j][i]=0xFF;
            }
            else
            {
                board[j][i]=0;
            }
        }
    }
}
/*  This function compresses the 2D array board into a malloced 1D array
    which is returned
    WARNING: if board width is set higher than 8 this function will need to 
    be adjusted to a larger type (char only holds 8 bits)
 */
unsigned char * compressBoard(unsigned char **board)
{
    //int numberOfPegs=0;
    unsigned char * compressedBoard = (unsigned char *)calloc((size_t)BOARD_HEIGHT,
                                                              sizeof(unsigned char));
    for(int j=1; j<=BOARD_HEIGHT; ++j)
    {
        for(int i=1; i<=BOARD_WIDTH; ++i)
        {
            if(board[j][i]==PEG)
            {
                compressedBoard[j-1] = compressedBoard[j-1] |
                (1 << (BOARD_WIDTH-i));
            }
        }
    }
    return compressedBoard;
}
//Macro taken from http://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
#define BYTETOBINARY(byte)  \
(byte & 0x80 ? 1 : 0), \
(byte & 0x40 ? 1 : 0), \
(byte & 0x20 ? 1 : 0), \
(byte & 0x10 ? 1 : 0), \
(byte & 0x08 ? 1 : 0), \
(byte & 0x04 ? 1 : 0), \
(byte & 0x02 ? 1 : 0), \
(byte & 0x01 ? 1 : 0)
/* prints the compress board bit pattern to screen
 */
void printCompressedBoard(unsigned char * compressedBoard)
{
    printf("\n");
    
    for(int j=1; j<=BOARD_WIDTH; ++j)
    {
        printf("%d%d%d%d%d%d%d%d",BYTETOBINARY(compressedBoard[j-1]));
        printf("\n");
    }
}

