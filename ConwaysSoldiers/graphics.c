//
//  graphics.c
//  conwayPegs
//
//  Created by ben on 02/12/2014.
//  Copyright (c) 2014 ben. All rights reserved.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
#include "pegs.h"

#define SDL_TTF_INSTALLED 0
#define MAX_STRING_SIZE 250

#if SDL_TTF_INSTALLED
#include <SDL2_ttf/SDL_ttf.h>

typedef struct display
{
    SDL_bool finished;
    SDL_bool skip;
    SDL_Window *win;
    int winWidth,
        winHeight;
    SDL_Renderer *renderer;
    TTF_Font * font;
    SDL_Color white;
    SDL_Color orange;
    SDL_Color black;
    SDL_Surface* surfaceMessage;
    SDL_Texture* textureMessage;
    SDL_Event *event;

} display;

enum sdlKey {
    UP = 1,
    DOWN = -1,
    RETURN = '\n',
    NONE = 0
};
typedef enum sdlKey sdlKey;

void checkSDLEvents(display *d);

sdlKey fetchSDLEvents(display *d);
void startSDL(display *d);
void printOpeningMessage(display *d, int numberOfMoves, int numberOfRoutes);
void printTextSDL(display *d, char * stringToPrint, int highLight, int openingMessage, int menuHeading);
void quitSDL(display * d);
void setCoordsForPrintingBranchMenu(display * d, SDL_Rect * textBox, int firstItem);
void branchMenu(display * d, int * displayState, int * displayBranch);
void play( display * d,int state, int branch );
void displayBoard(display * d, int boardID  );
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void renderPeg(display *d, SDL_Rect * slot);

void displaySolutionSDL()
{
    display d;
    startSDL(&d);
    list * boardList = getListPtr(NULL);
    int numberOfRoutes = 0;
    int numberOfMoves = boardList->finalStates[1]->history[1][0];
    for(int state=1; state<=boardList->numberOfFinalStates; ++state)
    {
        numberOfRoutes += boardList->finalStates[state]->history[0][0];
    }
    printOpeningMessage(&d, numberOfMoves, numberOfRoutes);

    SDL_SetRenderDrawColor( d.renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( d.renderer );
    SDL_RenderPresent( d.renderer);
    SDL_UpdateWindowSurface(d.win);

    int displayState, displayBranch;

    //Update screen
    while (!d.finished)
    {
        printf("in main\n");

        displayState=0;
        displayBranch=0;
        SDL_SetRenderDrawColor( d.renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( d.renderer );
        SDL_RenderPresent( d.renderer);
        SDL_UpdateWindowSurface(d.win);

        branchMenu(&d, &displayState, &displayBranch );
        if(displayBranch && displayState)
        {
            SDL_SetRenderDrawColor( d.renderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( d.renderer );
            SDL_RenderPresent( d.renderer);
            SDL_UpdateWindowSurface(d.win);

            play(&d,displayState,displayBranch);
        }
        checkSDLEvents(&d);
        SDL_Delay(150);
    }
    quitSDL(&d);
}

void play(display * d, int state, int branch)
{
    SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );//orange
    
    list * boardList = getListPtr(NULL);
    for(int parentIter = 1; parentIter<=boardList->finalStates[state]->history[branch][0] ;
        ++parentIter)
    {
        printTextSDL(d, "Press ENTER to See Next Move", 0, 0, 1);
        displayBoard(d, boardList->finalStates[state]->history[branch][parentIter]);
        SDL_RenderPresent( d->renderer);
        
        while (!d->skip) {
            checkSDLEvents(d);
            SDL_Delay(30);
            if(d->finished) return;
            if(d->skip) {
                d->skip=0;
                return;
            }
        }
    }
}
void displayBoard(display * d, int boardID)
{
    printf(" Displaying board %d\n",boardID);
    storedBoard * boardToDisplay = getBoardFromID(boardID);
    unsigned char ** board = createBoard();
    decompressBoard(boardToDisplay->compressedBoard, board);
    
    SDL_SetRenderDrawColor( d->renderer, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderClear( d->renderer );


    SDL_Rect backOfBoard = {
        d->winWidth/4,//x position of top left corner
        d->winHeight/4,//y      "          "
        d->winWidth/2,//width
        d->winHeight/2//height
    };
    SDL_SetRenderDrawColor( d->renderer, 0x50, 0x50, 0x50, 0xFF );
    SDL_RenderFillRect(d->renderer, &backOfBoard);
 

    int pegSlotBoarderSize = backOfBoard.w/(BOARD_WIDTH*BOARD_WIDTH+2);
    int numberOfPegsSlots = BOARD_WIDTH*BOARD_HEIGHT;
    
    SDL_Rect * pegSlots = malloc(numberOfPegsSlots*sizeof(SDL_Rect));
    int slotNumber=0;
    for (int j=1; j<=BOARD_HEIGHT; ++j)
    {
        for(int i=1; i<=BOARD_WIDTH; ++i)
        {
            double  shiftX = (i-1)*(1/(double)BOARD_WIDTH)*backOfBoard.w;
            
            double shiftY = (j-1)*(1/(double)BOARD_HEIGHT)*backOfBoard.h;

            pegSlots[slotNumber].x = backOfBoard.x + (int)shiftX + pegSlotBoarderSize;
            pegSlots[slotNumber].y = backOfBoard.y + (int)shiftY + pegSlotBoarderSize;
            
            pegSlots[slotNumber].w = backOfBoard.w/BOARD_WIDTH - 2*pegSlotBoarderSize;
            pegSlots[slotNumber].h = backOfBoard.h/BOARD_HEIGHT - 2*pegSlotBoarderSize;
            if(board[j][i]==PEG)
            {
                renderPeg(d, &pegSlots[slotNumber]);
            }
            ++slotNumber;

        }
    }
    SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );//orange
    SDL_RenderDrawRects(d->renderer, pegSlots, numberOfPegsSlots);

    printTextSDL(d, "Press ENTER to See Next Move", 0, 0, 1);
    
    freeBoard(board);
}
void renderPeg(display *d, SDL_Rect * slot)
{
    int centreX = slot->x + slot->w/2;
    int centreY = slot->y + slot->h/2;
    int radius = slot->w < slot->h ? slot->w*0.45 : slot->h*0.45;
    SDL_SetRenderDrawColor( d->renderer, 0xA0, 0xA0, 0xA0, 0xFF );
    Neill_SDL_RenderFillCircle(d->renderer, centreX, centreY, radius);
    SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );
    Neill_SDL_RenderDrawCircle(d->renderer, centreX, centreY, radius);
}
// Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
    
    double dx, dy;
    dx = floor(sqrt((2.0 * r ) ));
    SDL_RenderDrawLine(rend, cx-(dx*0.6), cy+r, cx+(dx*0.6), cy+r);
    SDL_RenderDrawLine(rend, cx-(dx*0.6), cy-r, cx+(dx*0.6), cy-r);
    for (dy = 1; dy <= r; dy += 1.0)
    {
        dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawPoint(rend, cx+dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx+dx, cy-r+dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy-r+dy);
    }
    
}
// Filled Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
    
    for (double dy = 1; dy <= r; dy += 1.0)
    {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
    }
    
}

void startSDL(display *d)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    d->finished = 0;
    d->skip = 0;

    d->winWidth = 900;
    d->winHeight = 800;
    d->win= SDL_CreateWindow("SDL Window",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             d->winWidth, d->winHeight,
                             SDL_WINDOW_SHOWN);
    if(d->win == NULL) {
        fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    
    d->renderer = SDL_CreateRenderer(d->win, -1, 0);
    if(d->renderer == NULL) {
        fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    d->white.r = 255;
    d->white.g = 255;
    d->white.b = 255;
    d->orange.r = 205;
    d->orange.g = 133;
    d->orange.b = 63;
    d->black.r = 0;
    d->black.g = 0;
    d->black.b = 0;

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        exit(6);
    }
    d->font = TTF_OpenFont("InputMono-ExtraLight.ttf", 15);
    TTF_SetFontHinting(d->font, TTF_HINTING_MONO);
    d->event = malloc(sizeof(SDL_Event));
    
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    if(SDL_RenderClear(d->renderer)<0)
    {
        printf( "Could not renderClear in startSDL Error: %s\n", SDL_GetError() );
        exit(6);
    }
    SDL_RenderPresent(d->renderer);
    SDL_UpdateWindowSurface(d->win);


}

void printOpeningMessage(display *d, int numberOfMoves, int numberOfRoutes)
{
    int * targetPosition = getTargetPosition(NULL);
    
    char outputString[MAX_STRING_SIZE];
    if(numberOfRoutes>1) {
        sprintf(outputString, " There are %d routes using %d moves to position %d, %d (Press ENTER to skip) ",
                numberOfRoutes, numberOfMoves,targetPosition[0],
                targetPosition[1]);
    }
    else {
        sprintf(outputString, " There is only 1 route using %d moves to position %d, %d (Press ENTER to skip) ",
                numberOfMoves, targetPosition[0],
                targetPosition[1]);
    }
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(d->renderer)
    printTextSDL(d, outputString, 0, 1, 0);
    SDL_RenderPresent(d->renderer);
    for(int ticks = 1; ticks<=100; ++ticks)
    {
        checkSDLEvents(d);
        SDL_Delay(30);
        if(d->finished) return;
        if(d->skip) {
            d->skip=0;
            return;
        }
    }
}

/*
 *
 */
char * getBranchString(storedBoard * board, int branch)
{
    char * branchHistory = (char *)malloc(MAX_STRING_SIZE*sizeof(char));
    int numberOfParentsInBranch = board->history[branch][0],
        lengthOfString=0;
    lengthOfString += sprintf(branchHistory,"Branch %d:    ", branch);
    for(int parentIter = 1; parentIter<=numberOfParentsInBranch;
        ++parentIter) {
        lengthOfString += sprintf(branchHistory+lengthOfString," %d",board->history[branch][parentIter]);
    }
    //strcat(branchHistory, ".\n");
    return branchHistory;
}


char ** buildBranchMenuString( storedBoard * finalState)
{
    char ** branchStrings = malloc(finalState->history[0][0]*sizeof(char*));
    branchStrings = branchStrings - 1; //now branchNum = its strings position in array
    for(int branch=1; branch<=finalState->history[0][0]; ++branch)
    {
        branchStrings[branch] = getBranchString(finalState, branch);
    }

    return branchStrings;
}


void freeMenuStrings(char *** menuStrings)
{
    list * boardList = getListPtr(NULL);
    for(int state=1; state<=boardList->numberOfFinalStates; ++state)
    {
        for(int branch=1; branch<=boardList->finalStates[state]->history[0][0]; ++branch)
        {
            free(menuStrings[state][branch]);//free each string
        }
    }
    for(int state=1; state<=boardList->numberOfFinalStates; ++state)
    {
        free(menuStrings[state]+1);//free the array of branches for each state
    }
    free(menuStrings+1);//free the array of final states
}


void checkSDLEvents(display *d)
{
    if(SDL_PollEvent(d->event))
    {
        if( d->event->type == SDL_QUIT ) {
            d->finished = 1;
        }
        else if( d->event->type == SDL_KEYDOWN ) {
            int sym = d->event->key.keysym.sym;
            if (sym == SDLK_RETURN) {
                d->skip = 1;
            }
        }
    }
}
sdlKey fetchSDLEvents(display *d)
{
    while(1)
    {
        SDL_PollEvent(d->event);
        if( d->event->type == SDL_QUIT ) {
            d->finished = 1;
            return NONE;
        }
        if( d->event->type == SDL_KEYDOWN ) {
            int sym = d->event->key.keysym.sym;
            
            if (sym == SDLK_UP)         return UP;
            else if (sym == SDLK_DOWN)  return DOWN;
            else if (sym == SDLK_RETURN)  return RETURN;
            
            else                        return NONE;
        }

    }
}
#define STATE 0 
#define BRANCH 1
void branchMenu(display * d, int * displayState, int * displayBranch)
{
    printf("in branch menu\n");
    list * boardList = getListPtr(NULL);
    char *** menuStrings = malloc(boardList->numberOfFinalStates*sizeof(char**));
    menuStrings = menuStrings-1;//unit offset
    for(int state=1; state<=boardList->numberOfFinalStates; ++state)
    {
        menuStrings[state]=buildBranchMenuString(boardList->finalStates[state]);
    }
    int highlightMethod[2] = {1,1};//the state and branch highlighted
    while(!d->finished)
    {

        SDL_SetRenderDrawColor( d->renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( d->renderer );
  
        printTextSDL(d, "Please Select A Branch To View...", 0, 0, 1);
        for(int state=1; state<=boardList->numberOfFinalStates; ++state)
        {
            char boardName[MAX_STRING_SIZE];
            sprintf(boardName,"Board %d:", state);
            printTextSDL(d, boardName, 0, 0, 0);

            for(int branch=1; branch<=boardList->finalStates[state]->history[0][0]; ++branch)
            {
                if(state == highlightMethod[0] && branch==highlightMethod[1]) {
                    printTextSDL(d, menuStrings[state][branch], 1, 0, 0);
                }
                else {
                    printTextSDL(d, menuStrings[state][branch], 0, 0, 0);
                }
            }
        }
        SDL_RenderPresent(d->renderer);
        SDL_Delay(200);
        sdlKey keyPress = fetchSDLEvents(d); //wait for key press
        if ( keyPress == UP)
        {       //if we are currently on a first branch
            if(highlightMethod[BRANCH]==1)
            {   //then moving up takes us to new state
                //if we are on first state
                highlightMethod[STATE] = highlightMethod[STATE] == 1 ?
                boardList->numberOfFinalStates ://then move to last state
                highlightMethod[STATE]-1;//else move up one

                //and set branch to the last branch in that state
                highlightMethod[BRANCH] = boardList->finalStates[highlightMethod[STATE]]->history[0][0];

                
            }
            else {//not new state just increment branch
                --highlightMethod[BRANCH];
            }
        }
        if ( keyPress == DOWN)
        {   //if we are currently on the final branch in this state
            if(highlightMethod[BRANCH]==boardList->finalStates[highlightMethod[STATE]]->history[0][0])
            {   //then move to next state
                //then if the state is the last state in the list:
                highlightMethod[STATE] = highlightMethod[STATE] == boardList->numberOfFinalStates ?
                1   : //then go back to the start
                highlightMethod[STATE] + 1;//else increment one
                
                highlightMethod[BRANCH] = 1;//new state so branch resets to 1
            }
            else {//not new state just increment branch
                ++highlightMethod[BRANCH];
            }
        }
        if ( keyPress == RETURN)
        {
            *displayState = highlightMethod[STATE];
            *displayBranch = highlightMethod[BRANCH];
            break;
        }
   
        SDL_Delay(300);//stops single key presses being sampled many times.
        SDL_SetRenderDrawColor( d->renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( d->renderer );
        SDL_RenderPresent( d->renderer);
        SDL_UpdateWindowSurface(d->win);

    }
    freeMenuStrings(menuStrings);
}


void printTextSDL(display *d, char * stringToPrint, int highLight, int openingMessage, int menuHeading)
{
    SDL_Color textColour;
    if(highLight) {
        textColour = d->orange;
    }
    else {
        textColour = d->orange;
    }
    
    d->surfaceMessage = TTF_RenderText_Solid(d->font, stringToPrint, textColour);
    if(d->surfaceMessage==NULL) {
        fprintf(stderr, "ERROR: display Soln surfaceMessage TTF_RenderText_Solid failed \n");
        exit(6);
    }
    d->textureMessage = SDL_CreateTextureFromSurface(d->renderer, d->surfaceMessage);
    if(d->textureMessage==NULL) {
        fprintf(stderr, "ERROR: display Soln: textureMessage SDL_CreateTextureFromSurface failed \n");
        exit(6);
    }
    SDL_Rect textBox;
    textBox.w = d->surfaceMessage->w;
    textBox.h = d->surfaceMessage->h;
    if (openingMessage) {//opening message goes in middle of screen:
        textBox.x = 0;
        textBox.y = d->winHeight/2 - textBox.h;
    }
    else if (menuHeading)
    {
        setCoordsForPrintingBranchMenu(d, &textBox,1);
    }
    else {
        setCoordsForPrintingBranchMenu(d, &textBox,0);
    }

    if(highLight) {
        SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );
        //SDL_RenderFillRect(d->renderer, &textBox);
        SDL_RenderDrawRect ( d->renderer,&textBox);
    }
    SDL_RenderCopy(d->renderer, d->textureMessage, NULL, &textBox);

}

void setCoordsForPrintingBranchMenu(display * d, SDL_Rect * textBox, int firstItem)
{
    static int coordOfNextItem[2];//x,y
    textBox->x =  d->winWidth/15 ;
    if(firstItem)
    {
        textBox->y = d->winHeight/10;
    }
    else
    {
        textBox->y = yCoordOfNextItem;
    }
    if( textBox->y + textBox->h > d->winHeight)
    {
        textBox->x = textBox->x + textBox->w + d->winWidth/15;
        textBox->y = d->winHeight/10 + textBox->h;

    }
    coordOfNextItem[1] = textBox->y + 1.3*textBox->h;
    coordOfNextItem[0] = textBox->x;
    
}

void quitSDL(display * d)
{
    free(d->event);
    SDL_DestroyTexture(d->textureMessage);
    SDL_FreeSurface(d->surfaceMessage);
    TTF_CloseFont( d->font );
    SDL_DestroyRenderer( d->renderer);
    SDL_DestroyWindow( d->win );
    d->renderer = NULL;
    d->win = NULL;
    d->font = NULL;
    TTF_Quit();
    SDL_Quit();
}



























/*******************************************************************************
    if no SDL_ttf
 ******************************************************************************/



#else

typedef struct display
{
    SDL_bool finished;
    SDL_bool skip;
    SDL_Window *win;
    int winWidth,
        winHeight;
    SDL_Renderer *renderer;
    SDL_Event *event;
    
} display;

enum sdlKey {
    UP = 1,
    DOWN = -1,
    LEFT = -2,
    RIGHT = +2,
    RETURN = '\n',
    NONE = 0
};
typedef enum sdlKey sdlKey;

int stateMenu(display * d);
int branchMenu(display * d, int stateToDisplayID);
void play(display * d, int state, int branch);
void displayBoard(display * d, int boardID, SDL_Rect * displayArea);
void quitSDL(display * d);
sdlKey getSdlMenuKeyPresses(display *d);
void checkSDLwinClosed(display *d);
void startSDL(display *d);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r);
void renderPeg(display *d, SDL_Rect * slot);
/*  Called at end of main program to open the SDL window and display the 
    solutions.
 */
void displaySolutionSDL()
{
    display d;
    startSDL(&d);
    
    while (!d.finished)
    {
        
        SDL_SetRenderDrawColor( d.renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( d.renderer );
        
        int displayState = stateMenu(&d);
        if(d.finished) break;
        int displayBranch;
        list *boardList = getListPtr(NULL);
        //if more than one branch in the selected state enter the branchMenu:
        if(boardList->finalStates[displayState]->history[0][0]>1) {
            displayBranch = branchMenu(&d, displayState);
            
        }
        else {
            displayBranch = 1;
        }
        if(d.finished) break;
        if(displayBranch && displayState)
        {
            SDL_SetRenderDrawColor( d.renderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( d.renderer );
            play(&d, displayState, displayBranch);
        }
        if(d.finished) break;
        SDL_Delay(150);
    }
    quitSDL(&d);
}

/*  This function displays every final state we found satisfying the target
    the user can then move through the boards with the arrow keys and RETURN to 
    select one to view.
    it returns the position of the selected state in the state array.
 */
int stateMenu(display * d)
{
    printf("in state menu\n");
    list * boardList = getListPtr(NULL);
    printf("Please Select A State To View...(use arrow keys and enter)\n");
    //Now we split the window up into roughly square section depending on
    //the number of states we have found
    float toBeRounded = sqrtf((float)boardList->numberOfFinalStates);
    int numberOfBoardsPerRow = (int)ceilf(toBeRounded);//rounds up
    int numberOfRows = (int)ceilf(boardList->numberOfFinalStates/(float)numberOfBoardsPerRow);
    
    SDL_Rect stateDisplayAreas, highlightedBoarder;
    //set the gaps between boards
    int spaceBetweenStates = d->winWidth/(numberOfBoardsPerRow*numberOfBoardsPerRow*numberOfBoardsPerRow);
    stateDisplayAreas.w = d->winWidth/(float)numberOfBoardsPerRow - 2*spaceBetweenStates;
    stateDisplayAreas.h = d->winHeight/(float)numberOfRows - 2*spaceBetweenStates;
    
    int highlightPosition[2] = {0, 0};//the row([0]) and col([1]) being highlighted
    int statesPostionsOnScreen[numberOfRows][numberOfBoardsPerRow];
    highlightedBoarder.w = d->winWidth/(float)numberOfBoardsPerRow;
    highlightedBoarder.h = d->winHeight/(float)numberOfRows;
    
    while(!d->finished)
    {
        SDL_SetRenderDrawColor( d->renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( d->renderer );
        //printf("w=%d h=%d gap=%d \n",stateDisplayAreas.w, stateDisplayAreas.h,spaceBetweenStates);
        for(int state=1; state<=boardList->numberOfFinalStates; ++state)
        {
            int row = (state-1)/numberOfBoardsPerRow;
            int col = ((state-1)%(numberOfBoardsPerRow));
            statesPostionsOnScreen[ row ][ col ] = state;
            stateDisplayAreas.x = col * d->winWidth/(float)numberOfBoardsPerRow + spaceBetweenStates;
            stateDisplayAreas.y =  row * d->winHeight/(float)numberOfRows + spaceBetweenStates;
            //printf("state=%d x=%d y=%d\n",state,stateDisplayAreas.x,stateDisplayAreas.y);
            if(row == highlightPosition[1] && col == highlightPosition[0]) {
                highlightedBoarder.x = col * highlightedBoarder.w ;
                highlightedBoarder.y =  row * d->winHeight/(float)numberOfRows;
                SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );//orange
                SDL_RenderFillRect(d->renderer, &highlightedBoarder);
            }
            displayBoard(d, boardList->finalStates[state]->boardID,&stateDisplayAreas);
        }
        SDL_RenderPresent( d->renderer);
        SDL_Delay(200);//stops single key presses being sampled many times.
        sdlKey keyPress = getSdlMenuKeyPresses(d); //wait for key press
        if ( keyPress == UP ) {
            if(highlightPosition[1]==0) {
                highlightPosition[1] = numberOfRows-1;
            }
            else {//not new state just increment branch
                --highlightPosition[1];
            }
        }
        else if ( keyPress == DOWN ) {
            if(highlightPosition[1]==numberOfRows-1) {
                highlightPosition[1] = 0;
            }
            else {//not new state just increment branch
                ++highlightPosition[1];
            }
        }
        else if ( keyPress == RIGHT ) {
            if(highlightPosition[0]==numberOfBoardsPerRow-1) {
                highlightPosition[0] = 0;
            }
            else {//not new state just increment branch
                ++highlightPosition[0];
            }
        }
        else if ( keyPress == LEFT ) {
            if(highlightPosition[0]==0) {
                highlightPosition[0] = numberOfBoardsPerRow-1;
            }
            else {//not new state just increment branch
                --highlightPosition[0];
            }
        }
        else if ( keyPress == RETURN ) {
            return statesPostionsOnScreen[ highlightPosition[1] ] [ highlightPosition[0] ];
        }
        else if ( keyPress == NONE) {
            return 0;
        }
    }
    return 0;
}

int branchMenu(display * d, int stateToDisplayID)
{
    printf("in branch menu\n");
    storedBoard * stateToDisplay = getBoardFromID(stateToDisplayID);
    printf("Please Select A Branch To View...(use arrow keys and enter)\n");
    printf("Sorry, I know theres no way to know what your picking...need SDL_ttf on lab machines!\n");

    //Now we split the window up into roughly square section depending on
    //the number of branches we have
    float toBeRounded = sqrtf((float)stateToDisplay->history[0][0]);//sqrt the num of branches
    int numberOfBranchesPerRow = (int)ceilf(toBeRounded);//rounds up
    int numberOfRows = (int)ceilf(stateToDisplay->history[0][0]/(float)numberOfBranchesPerRow);
    
    SDL_Rect branchDisplayAreas, highlightedBoarder;
    //set the gaps between boards winwidth/cube of the numberOfBranchesPerRow seems to work well
    int spaceBetweenBranches = d->winWidth/(numberOfBranchesPerRow*numberOfBranchesPerRow*numberOfBranchesPerRow);
    branchDisplayAreas.w = d->winWidth/(float)numberOfBranchesPerRow - 2*spaceBetweenBranches;
    branchDisplayAreas.h = d->winHeight/(float)numberOfRows - 2*spaceBetweenBranches;
    
    int highlightPosition[2] = {0, 0};//the row([0]) and col([1]) being highlighted
    int branchesPostionsOnScreen[numberOfRows][numberOfBranchesPerRow];
    highlightedBoarder.w = d->winWidth/(float)numberOfBranchesPerRow;
    highlightedBoarder.h = d->winHeight/(float)numberOfRows;
    
    while(!d->finished)
    {
        SDL_SetRenderDrawColor( d->renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_RenderClear( d->renderer );
        for(int branch=1; branch<=stateToDisplay->history[0][0]; ++branch)
        {
            int row = (branch-1)/numberOfBranchesPerRow;
            int col = ((branch-1)%(numberOfBranchesPerRow));
            branchesPostionsOnScreen[ row ][ col ] = branch;
            branchDisplayAreas.x = col * d->winWidth/(float)numberOfBranchesPerRow + spaceBetweenBranches;
            branchDisplayAreas.y =  row * d->winHeight/(float)numberOfRows + spaceBetweenBranches;
            if(row == highlightPosition[1] && col == highlightPosition[0]) {
                highlightedBoarder.x = col * highlightedBoarder.w ;
                highlightedBoarder.y =  row * d->winHeight/(float)numberOfRows;
                SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );//orange
                SDL_RenderFillRect(d->renderer, &highlightedBoarder);
            }
            SDL_SetRenderDrawColor( d->renderer, 100, 100, 100, 0xFF );//grey
            SDL_RenderFillRect(d->renderer, &branchDisplayAreas);
        }
        SDL_RenderPresent( d->renderer);
        SDL_Delay(200);
        sdlKey keyPress = getSdlMenuKeyPresses(d); //wait for key press
        if ( keyPress == UP ) {
            if(highlightPosition[1]==0) {
                highlightPosition[1] = numberOfRows-1;
            }
            else {//not new state just increment branch
                --highlightPosition[1];
            }
        }
        if ( keyPress == DOWN ) {
            if(highlightPosition[1]==numberOfRows-1) {
                highlightPosition[1] = 0;
            }
            else {//not new state just increment branch
                ++highlightPosition[1];
            }
        }
        if ( keyPress == RIGHT ) {
            if(highlightPosition[0]==numberOfBranchesPerRow-1) {
                highlightPosition[0] = 0;
            }
            else {//not new state just increment branch
                ++highlightPosition[0];
            }
        }
        if ( keyPress == LEFT ) {
            if(highlightPosition[0]==0) {
                highlightPosition[0] = numberOfBranchesPerRow-1;
            }
            else {//not new state just increment branch
                --highlightPosition[0];
            }
        }
        if ( keyPress == RETURN ) {
            return branchesPostionsOnScreen[ highlightPosition[1] ] [ highlightPosition[0] ];
        }
        else if ( keyPress == NONE) {
            return 0;
        }
    }
    return 0;
}



/*  displays each of the boards in the chosen branch leading to the final state
 */
void play(display * d, int state, int branch)
{
    list * boardList = getListPtr(NULL);

    SDL_Rect displayBoardIn = {
        d->winWidth/4,//x position of top left corner
        d->winHeight/4,//y      "          "
        d->winWidth/2,//width
        d->winHeight/2//height
     };
    for(int parentIter = 1; parentIter<=boardList->finalStates[state]->history[branch][0] ;
        ++parentIter)
    {
        SDL_SetRenderDrawColor( d->renderer, 0x0, 0x0, 0x0, 0xFF );

        SDL_RenderClear(d->renderer);
        displayBoard(d, boardList->finalStates[state]->history[branch][parentIter], &displayBoardIn);
        SDL_RenderPresent(d->renderer);
        SDL_Delay(600);
        checkSDLwinClosed(d);
        if(d->finished)
        {
            return;
        }
    }
}


/*  Displays the boardID given in the rectangular area displayArea
 */
void displayBoard(display * d, int boardID, SDL_Rect * displayArea)
{
    storedBoard * boardToDisplay = getBoardFromID(boardID);
    unsigned char ** board = createBoard();
    decompressBoard(boardToDisplay->compressedBoard, board);


    SDL_SetRenderDrawColor( d->renderer, 0x50, 0x50, 0x50, 0xFF );
    SDL_RenderFillRect(d->renderer, displayArea);
    
    
    int pegSlotBoarderSize = displayArea->w/(float)(BOARD_WIDTH*BOARD_WIDTH+2);//sets the gaps between slots
    int numberOfPegsSlots = BOARD_WIDTH*BOARD_HEIGHT;
    
    SDL_Rect * pegSlots = malloc(numberOfPegsSlots*sizeof(SDL_Rect));
    int slotNumber=0;
    for (int j=1; j<=BOARD_HEIGHT; ++j)
    {
        for(int i=1; i<=BOARD_WIDTH; ++i)
        {
            float  shiftX = (i-1)*(1/(float)BOARD_WIDTH)*displayArea->w;
            
            float shiftY = (j-1)*(1/(float)BOARD_HEIGHT)*displayArea->h;
            
            pegSlots[slotNumber].x = displayArea->x + (int)shiftX + pegSlotBoarderSize;
            pegSlots[slotNumber].y = displayArea->y + (int)shiftY + pegSlotBoarderSize;
            
            pegSlots[slotNumber].w = displayArea->w/(float)BOARD_WIDTH - 2*pegSlotBoarderSize;
            pegSlots[slotNumber].h = displayArea->h/(float)BOARD_HEIGHT - 2*pegSlotBoarderSize;
            if(board[j][i]==PEG)
            {
                renderPeg(d, &pegSlots[slotNumber]);
            }
            ++slotNumber;
        }
    }
    SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );//orange
    SDL_RenderDrawRects(d->renderer, pegSlots, numberOfPegsSlots);

    free(pegSlots);
    freeBoard(board);
}





void renderPeg(display *d, SDL_Rect * slot)
{
    int centreX = slot->x + slot->w/2;
    int centreY = slot->y + slot->h/2;
    int radius = slot->w < slot->h ? slot->w*0.45 : slot->h*0.45;
    SDL_SetRenderDrawColor( d->renderer, 0xA0, 0xA0, 0xA0, 0xFF );
    Neill_SDL_RenderFillCircle(d->renderer, centreX, centreY, radius);
    SDL_SetRenderDrawColor( d->renderer, 205, 133, 63, 0xFF );
    Neill_SDL_RenderDrawCircle(d->renderer, centreX, centreY, radius);
}





// Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderDrawCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
    
    double dx, dy;
    dx = floor(sqrt((2.0 * r ) ));
    SDL_RenderDrawLine(rend, cx-(dx*0.6), cy+r, cx+(dx*0.6), cy+r);
    SDL_RenderDrawLine(rend, cx-(dx*0.6), cy-r, cx+(dx*0.6), cy-r);
    for (dy = 1; dy <= r; dy += 1.0)
    {
        dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawPoint(rend, cx+dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx+dx, cy-r+dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy+r-dy);
        SDL_RenderDrawPoint(rend, cx-dx, cy-r+dy);
    }
    
}



// Filled Circle centred at (cx,cy) of radius r, see :
// http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{
    
    for (double dy = 1; dy <= r; dy += 1.0)
    {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
    }
    
}



/* Call to initialise SDL
 */
void startSDL(display *d)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    d->finished = 0;
    d->skip = 0;
    
    d->winWidth = 900;
    d->winHeight = 660;
    d->win= SDL_CreateWindow("SDL Window",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             d->winWidth, d->winHeight,
                             SDL_WINDOW_SHOWN);
    if(d->win == NULL) {
        fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    
    d->renderer = SDL_CreateRenderer(d->win, -1, 0);
    if(d->renderer == NULL) {
        fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    d->event = malloc(sizeof(SDL_Event));
    
    SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    if(SDL_RenderClear(d->renderer)<0)
    {
        printf( "Could not renderClear in startSDL Error: %s\n", SDL_GetError() );
        exit(6);
    }
    SDL_RenderPresent(d->renderer);
}


/*  Call frequently to see if the wind has been closed
    then check d->finished to see if this is the case
 */
void checkSDLwinClosed(display *d)
{
    if(SDL_PollEvent(d->event))
    {
        if( d->event->type == SDL_QUIT ) {
            d->finished = 1;
        }
    }
}



/*  waits for arrow, a return or for the window to be closed
 */
sdlKey getSdlMenuKeyPresses(display *d)
{
    while(1)
    {
        SDL_PollEvent(d->event);
        if( d->event->type == SDL_QUIT ) {
            d->finished = 1;
            return NONE;
        }
        if( d->event->type == SDL_KEYDOWN ) {
            int sym = d->event->key.keysym.sym;
            
            if (sym == SDLK_UP)         return UP;
            else if (sym == SDLK_DOWN)  return DOWN;
            else if (sym == SDLK_LEFT)  return LEFT;
            else if (sym == SDLK_RIGHT)  return RIGHT;

            else if (sym == SDLK_RETURN)  return RETURN;
            
 
        }
        
    }
}


/* call if window is closed 
 */
void quitSDL(display * d)
{
    free(d->event);
    SDL_DestroyRenderer( d->renderer);
    SDL_DestroyWindow( d->win );
    d->renderer = NULL;
    d->win = NULL;
    SDL_Quit();
}
#endif


















