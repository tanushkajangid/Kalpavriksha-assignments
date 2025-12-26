#include "utilities.h"

void sortArray(int *array, int sizeOfArray){

    int temporary;
    for(int i = 0; i< sizeOfArray-1; i++){

        for(int j = 0; j < sizeOfArray - i - 1; j++){

            if(array[j] > array[j+1]){

                temporary = array[j];
                array[j] = array[j+1];
                array[j+1] = temporary;
                
            }
        }
    }
}

void displayArray(int *array, int sizeOfArray){

    for(int i = 0; i < sizeOfArray; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

}