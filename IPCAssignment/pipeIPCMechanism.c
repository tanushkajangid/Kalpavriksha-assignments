#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utilities.h"

#define DATA_FILE "data.txt"
#define SIZE 2

void childProcess(int parentToChild[], int childToParent[]){

    close(parentToChild[1]); //parent to child
    close(childToParent[0]); //child to parent 

    int numberOfElements;

    read(parentToChild[0], &numberOfElements, sizeof(int));

    int* array = malloc(numberOfElements * sizeof(int));
    read(parentToChild[0], array, numberOfElements * sizeof(int));

    sortArray(array, numberOfElements);

    write(childToParent[1], array, numberOfElements * sizeof(int));

    free(array);

    close(parentToChild[0]);
    close(childToParent[1]);

    exit(0);
}

void parentProcess(int parentToChild[], int childToParent[]){
    
    close(parentToChild[0]);
    close(childToParent[1]);

    int numberOfElements;
    printf("Enter number of elements: ");
    scanf("%d", &numberOfElements);

    int* array = malloc(numberOfElements * sizeof(int));
    printf("Enter elements:\n");

    for(int i = 0; i < numberOfElements; i++){
        scanf("%d", &array[i]);
    }

    printf("Before Sorting:\n");
    displayArray(array, numberOfElements);
    
    write(parentToChild[1], &numberOfElements, sizeof(int));
    write(parentToChild[1], array, numberOfElements * sizeof(int));

    wait(NULL);
    read(childToParent[0], array, numberOfElements * sizeof(int));

    printf("After Sorting:\n");
    displayArray(array, numberOfElements);

    free(array);

    close(parentToChild[1]);
    close(childToParent[0]);
}

int main(){

    int parentToChild[SIZE];
    int childToParent[SIZE];

    pipe(parentToChild);
    pipe(childToParent);

    if(fork() == 0){

        childProcess(parentToChild, childToParent);

    }

    else{
         
        parentProcess(parentToChild, childToParent);

    }

    return 0;
}
