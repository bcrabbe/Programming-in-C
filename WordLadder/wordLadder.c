//
//  wordLadder.c
//  
//
//  Created by ben on 03/11/2014.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int **findPermutations(int *arrayToBeShuffled, int*numberOfPermutations);
void freeList(int** permutations, int numberOfPermutations);
char ** getDictionary();
void foundWordLadder(char**stringLadder);
int isWord(char * word, char ** dictionary, int dictionaryLength);
void freeDict(char** dictionary, int dictionaryLength);

int main(int argc, char **argv)
{
    int a[4]={1,2,3,4};
    int numberOfPermutations;
    int **permutations = findPermutations(a,&numberOfPermutations);

    char  stringLadder[5][5];
    if(argc!=3)
    {
        printf("You must specify 2 words to ladder between\n");
        exit(0);

    }
    else
    {
        strncpy(stringLadder[0],argv[1],(size_t)5);
        strncpy(stringLadder[4],argv[2],(size_t)5);
        printf("%s--> %s\n",stringLadder[0],stringLadder[4]);
    }
    int dictionaryLength;
    char ** dictionary = getDictionary(&dictionaryLength);
    int ladderFound=0;
    for(int i=1; i<=numberOfPermutations; ++i)
    {
        strncpy(stringLadder[1],stringLadder[0],(size_t)5);
        int positionToChange=permutations[i][0];//position 1 2 3 or 4
        stringLadder[1][positionToChange-1]=stringLadder[4][positionToChange-1];
        
        if(isWord(stringLadder[1],dictionary,dictionaryLength))
        {
            strncpy(stringLadder[2],stringLadder[1],(size_t)5);
            int positionToChange=permutations[i][1];//position 1 2 3 or 4
            stringLadder[2][positionToChange-1]=stringLadder[4][positionToChange-1];
            
            if(isWord(stringLadder[2],dictionary,dictionaryLength))
            {

                strncpy(stringLadder[3],stringLadder[2],(size_t)5);
                int positionToChange=permutations[i][2];//position 1 2 3 or 4
                stringLadder[3][positionToChange-1]=stringLadder[4][positionToChange-1];
                
                if(isWord(stringLadder[3],dictionary,dictionaryLength))
                {
                    
              
                    ladderFound=1;
                    printf("found ladder: %d %d %d %d\n\n ",permutations[i][0],permutations[i][1], permutations[i][2], permutations[i][3] );
                   
                    for(int i=0; i<4; ++i)
                    {
                        printf("%s ---> ",stringLadder[i]);
                        
                    }
                    printf("%s\n",stringLadder[4]);
                    break;
                
                    
                }
            }

        }
    }
    if(ladderFound==0)
    {
        printf("No ladder found \n\n");

    }
    
    //freeList(permutations,numberOfPermutations);
    //freeDict(dictionary,dictionaryLength);

    return 0;
}
void foundWordLadder(char**stringLadder)
{
    printf("found ladder:\n\n");
    for(int i=0; i<4; ++i)
    {
        printf("%s ---> ",stringLadder[i]);

    }
    printf("%s\n",stringLadder[4]);

}
int isWord(char * word, char ** dictionary, int dictionaryLength)
{
    
    for(int dictIter=1; dictIter<=dictionaryLength; ++dictIter)
    {
        if(strcmp(dictionary[dictIter],word )==0)
        {
            return 1;
        }
        else{}//do nothing
    }
    return 0;
}

char ** getDictionary(int*dictionaryLength)
{
    FILE * FP;
    FP = fopen("./4letters.txt","r");
    if(FP==NULL)
    {
        printf("failed to open files\n");
        exit(0);
    }
    
    int numberOfWordsInDict=0;
    char scanedWord[5]={0};//all the words are 4 letters + \0
    char **dictionary=NULL;//this is an array of strings,
    int fscanfReturn;
    do
    {
        fscanfReturn=fscanf(FP,"%s",scanedWord);//if we successfully scan a word this returns 1
        if(fscanfReturn==1)
        {
            ++numberOfWordsInDict;
        }
        //printf("scanned %s\n",scanedWord);
        
        dictionary = (char**)realloc(dictionary,(size_t)numberOfWordsInDict*sizeof(char*));
        //this increases the length of our array of strings by 1 each go through(have a look at the realloc manual: http://www.cplusplus.com/reference/cstdlib/realloc/)
        
        dictionary[numberOfWordsInDict-1]=malloc((size_t)5*sizeof(char));
        //this creates a size 5 array to hold the string
        
        strncpy( dictionary[numberOfWordsInDict-1],scanedWord,(size_t)5);
        //copies the scanned word into the array

    }  while(fscanfReturn==1);
    *dictionaryLength=numberOfWordsInDict;
    return dictionary-1;//then dictionary indexed from 1->dictionaryLength rather than 0-dictionaryLength-1
}

int editDistance(char *str1, char *str2)
{
    int numCharsDiff = 0;
    for(int i=0; i<4; ++i)
    {
        if(str1[i]!=str2[i])
        {
            ++numCharsDiff;
        }
    }
    return numCharsDiff;
}
               
int **findPermutations(int *arrayToBeShuffled, int*numberOfPermutations)
{
    int** listOfPermutions =NULL;
    int listSize=0;
    for(int n1=1 ; n1<=4; ++n1)
    {
        for(int n2=1 ; n2<=4; ++n2)
        {
            for(int n3=1 ; n3<=4; ++n3)
            {
                for(int n4=1 ; n4<=4; ++n4)
                {
                    if(n1!=n2 && n1!=n3 && n1!=n4 && n2!=n3 && n2!=n4 && n3!=n4)
                    {
                        //printf("42\n");
                        ++listSize;
                        listOfPermutions = realloc(listOfPermutions,(size_t)listSize*sizeof(int*));
                        
                        listOfPermutions[listSize-1]=malloc((size_t)4*sizeof(int));
                    
                        listOfPermutions[listSize-1][0]=n1;
                        listOfPermutions[listSize-1][1]=n2;
                        listOfPermutions[listSize-1][2]=n3;
                        listOfPermutions[listSize-1][3]=n4;
                        //printf("%d %d %d %d. %d\n ",n1,n2,n3,n4,listSize);
                    }
                }
            }
        }
    }
    *numberOfPermutations=listSize;
    return listOfPermutions-1;
}

void freeList(int** permutations, int numberOfPermutations)
{
    for(int i=0; i<numberOfPermutations;++i)
    {
        free(permutations[i]);
    }
    free(permutations);
}
void freeDict(char** dictionary, int dictionaryLength)
{
    for(int i=0; i<dictionaryLength;++i)
    {
        free(dictionary[i]);
    }
    free(dictionary);
}