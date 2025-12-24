#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utilities.h"

#define DATA_FILE "data.txt"

void parentProcess(int *numbersArray, int numberOfElements ){

    FILE* filePointer;

    wait(NULL);

    filePointer = fopen(DATA_FILE, "rb");

    fread(&numberOfElements, sizeof(int), 1, filePointer);
    fread(numbersArray, sizeof(int), numberOfElements, filePointer);
    fclose(filePointer);

    printf("After Sorting:\n");
    displayArray(numbersArray, numberOfElements);
}

void childProcess(){

    int numberOfElements;
    FILE *filePointer;

    filePointer = fopen(DATA_FILE, "rb");
    fread(&numberOfElements, sizeof(int), 1, filePointer);

    int *childArray = malloc(numberOfElements * sizeof(int));

    fread(childArray, sizeof(int), numberOfElements, filePointer);
    fclose(filePointer);

    sortArray(childArray, numberOfElements);

    filePointer = fopen(DATA_FILE, "wb");

    fwrite(&numberOfElements, sizeof(int), 1, filePointer);
    fwrite(childArray, sizeof(int), numberOfElements, filePointer);
    fclose(filePointer);

    free(childArray);
    exit(0);

}

int main(){

    int numberOfElements;
    printf("Enter number of elements: ");
    scanf("%d", &numberOfElements);

    int *numbersArray = malloc(numberOfElements * sizeof(int));

    printf("Enter Elements:\n");
    for(int i = 0; i < numberOfElements; i++){
        scanf("%d", &numbersArray[i]);
    }

    printf("Before Sorting:\n");
    displayArray(numbersArray, numberOfElements);

    FILE *filePointer;
    filePointer = fopen(DATA_FILE, "wb");

    fwrite(&numberOfElements, sizeof(int), 1, filePointer);
    fwrite(numbersArray, sizeof(int), numberOfElements, filePointer);
    fclose(filePointer);

    if(fork() == 0){

        childProcess();

    }

    else{
         
        parentProcess(numbersArray, numberOfElements);
        
    }

    free(numbersArray);

    return 0;
}
