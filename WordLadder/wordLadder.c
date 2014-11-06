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

int main(int argc, char **argv)
{
    int a[4]={1,2,3,4};
    int numberOfPermutations;
    int **permutations = findPermutations(a,&numberOfPermutations);

    char  stringStart[5], stringTarget[5], stringLadder[5][5];
    if(argc!=3)
    {
        printf("You must specify 2 words to ladder between\n");
        exit(0);

    }
    else
    {
        strncpy(stringStart,argv[1],(size_t)5);
        strncpy(stringTarget,argv[2],(size_t)5);
        strncpy(stringLadder[0],stringStart,(size_t)5);
        strncpy(stringLadder[4],stringTarget,(size_t)5);
        printf("%s--> %s\n",stringStart,stringTarget);
    }
    int dictionaryLength;
    char ** dictionary = getDictionary(&dictionaryLength);
 
    int foundLadder=0;
    int ladderSteps[5]={1,0,0,0,0};
    for(int i=1; i<=numberOfPermutations; ++i)
    {
        for(int j=0; j<4; ++j)
        {
            strncpy(stringLadder[j+1],stringTarget,(size_t)5);
            int positionToChange=permutations[i][j];
            stringLadder[j+1][positionToChange-1]=stringTarget[positionToChange-1];
            
            for(int dictIter=1; dictIter<=dictionaryLength; ++dictIter)
            {

                if(strcmp(dictionary[dictIter],stringLadder[j+1] )==0)
                {
                    ladderSteps[j+1]=1;
                    break;
                }
                else
                {
                    ladderSteps[j+1]=0;

                }
            }
            
        }
        
    }
   
    
    freeList(permutations,numberOfPermutations);
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