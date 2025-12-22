#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DATA_FILE "data.txt"

void sortArray(int *array, int size){
    int temporary;
    for(int i = 0; i< size-1; i++){
        for(int j = 0; j < size - i - 1; j++){
            if(array[j] > array[j+1]){
                temporary = array[j];
                array[j] = array[j+1];
                array[j+1] = temporary;
            }
        }
    }
}

void displayArray(int *array, int size){

    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

}

int main(){

    int elements;
    printf("Enter number of elements: ");
    scanf("%d", &elements);

    int *numbersArray = malloc(elements * sizeof(int));

    printf("Enter Elements:\n");
    for(int i = 0; i < elements; i++){
        scanf("%d", &numbersArray[i]);
    }

    printf("Before Sorting:\n");
    displayArray(numbersArray, elements);

    FILE *filePointer;
    filePointer = fopen(DATA_FILE, "wb");

    fwrite(&elements, sizeof(int), 1, filePointer);
    fwrite(numbersArray, sizeof(int), elements, filePointer);
    fclose(filePointer);

    if(fork() == 0){ //child Process

        filePointer = fopen(DATA_FILE, "rb");
        fread(&elements, sizeof(int), 1, filePointer);

        int *childArray = malloc(elements * sizeof(int));

        fread(childArray, sizeof(int), elements, filePointer);
        fclose(filePointer);

        sortArray(childArray, elements);

        filePointer = fopen(DATA_FILE, "wb");

        fwrite(&elements, sizeof(int), 1, filePointer);
        fwrite(childArray, sizeof(int), elements, filePointer);
        fclose(filePointer);

        free(childArray);
        exit(0);
    }

    else{  //parent Process
         
        wait(NULL);

        filePointer = fopen(DATA_FILE, "rb");

        fread(&elements, sizeof(int), 1, filePointer);
        fread(numbersArray, sizeof(int), elements, filePointer);
        fclose(filePointer);

        printf("After Sorting:\n");
        displayArray(numbersArray, elements);
    }

    free(numbersArray);

    return 0;
}
