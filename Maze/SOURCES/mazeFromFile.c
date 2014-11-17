//
//  mazeFromFile.c
//  
//
//  Created by ben on 15/11/2014.
//
//

#include "maze.h"


unsigned char ** initialiseFromFile(const char * fileName, int *dimensionX, int *dimensionY)
{
    
    unsigned char **maze;
    
    FILE * FP;
    FP = fopen(fileName,"r");
    if( FP==NULL )
    {
        fprintf(stderr,"failed to open file\n");
        exit(1);
    }
    if(fscanf(FP,"%d %d", dimensionX, dimensionY)!=2)
    {
        fprintf(stderr,"failed to read dimensions from file from file\n");
        fclose(FP);
        exit(1);
    }
    printf("file dimensions: %d by %d",*dimensionX,*dimensionY);
    if(*dimensionX>20|| *dimensionX<3 || *dimensionY>20 || *dimensionY<3)
    {
        fprintf(stderr,"ERROR: maze dimensions must be in the range: 3-20 by 3-20");
        fclose(FP);
        exit(1);
    }
    maze = createMaze(*dimensionX, *dimensionY);
    if( maze==NULL )
    {
        fprintf(stderr,"failed to allocate grid\n");
        fclose(FP);
        exit(1);
    }
    readFile(maze, *dimensionX, *dimensionY, FP);//reads the pattern in from file
    fclose(FP);
	
    return maze;
}

/* reads from file and copies to grid. File must have dimensionX dimensionY as its first line
 followed by the a board of ' '(=possible path) or #(=obstical) */
void readFile( unsigned char **grid, int dimensionX, int dimensionY, FILE *FP)
{
    printf("\n input FILE:\n");
    int i,j,newlineRead=0;
    char fileChar;
    for(j=1; j<=dimensionY; ++j)
    {
        newlineRead=0;
        for(i=1; i<=dimensionX; ++i)
        {
            if( fscanf(FP,"%c", &fileChar)!=1 )
			{
				fprintf(stderr,"in readFile fscanf failed");
                fclose(FP);
				exit(2);
			}
            while(fileChar == '\n'||fileChar == '\r')//deals with the new lines
            {
                if(fileChar == '\n') ++newlineRead;
                
                if(fscanf(FP,"%c", &fileChar)!=1)
				{
					fprintf(stderr,"in readFile fscanf failed");
                    fclose(FP);
					exit(2);
				}
            }
			putchar(fileChar); //print the board to screen as we go
            if( fileChar == ' ' )
            {
                grid[j][i]=' ';
            }
            else if( fileChar == '#' || fileChar == '#')
            {
                grid[j][i]='#';
            }
            else
            {
                fprintf(stderr,"read %d/%c for %d,%d element\n",fileChar,fileChar,i,j);
                fprintf(stderr,"This is incorrect file format\n");
                fclose(FP);
                exit(2);
            }
            if(newlineRead!=1)
            {
                fprintf(stderr,"ERROR: File dimensions do not match top line\n");
                fclose(FP);
                exit(2);
            }
        }
		printf("\n");
    }
}


void findStart(int * startX, int * startY, unsigned char ** maze, int dimX, int dimY )
{
    //first check top row and leftmost column simultaneously to find the space nearest the top left corner
    int i=1, j=1;
    while(i<=dimX && j<=dimY)
    {
        int foundStartInTop=0,foundStartInLeft=0;
        if( maze[1][i]==' ')
        {
            foundStartInTop=1;
        }
        if(maze[j][1]==' ')
        {
            foundStartInLeft=1;
        }
        if(foundStartInTop==1 && foundStartInLeft==0)
        {
            *startX=i;
            *startY=1;
            return;
        }
        else if(foundStartInTop==0 && foundStartInLeft==1)
        {
            *startX=1;
            *startY=j;
            return;
        }
        else if(foundStartInTop==1 && foundStartInLeft==1)
        {
            fprintf(stderr,"this maze has two possible starts equidistant from top left \n");
            exit(3);
        }
        else if(foundStartInTop==0 && foundStartInLeft==0)
        {
            if(i<dimX) ++i;
            if(i<dimY) ++j;
        }
    }
    
    //If we find no gaps in those sides then we must search the other two aswell
    i=1;
    j=1;
    while(i<=dimX && j<=dimY)
    {
        int foundStartInBottom=0,foundStartInRight=0;
        if( maze[dimY][i]==' ')
        {
            foundStartInBottom=1;
        }
        if(maze[j][dimX]==' ')
        {
            foundStartInRight=1;
        }
        if(foundStartInBottom==1 && foundStartInRight==0)
        {
            *startX=i;
            *startY=dimY;
            return;
        }
        else if(foundStartInBottom==0 && foundStartInRight==1)
        {
            *startX=dimX;
            *startY=j;
            return;
        }
        else if(foundStartInBottom==1 && foundStartInRight==1)
        {
            fprintf(stderr,"Undefined: this maze has two possible starts equidistant from top left \n");
            exit(3);
        }
        else if(foundStartInBottom==0 && foundStartInRight==0)
        {
            if(i<dimX) ++i;
            if(i<dimY) ++j;
        }
    }
    
    //if we find no start:
    fprintf(stderr,"There are no openings in the wall of the maze. \n");
    exit(3);
}
