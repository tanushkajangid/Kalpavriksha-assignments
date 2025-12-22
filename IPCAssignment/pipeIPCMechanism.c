#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DATA_FILE "data.txt"
#define SIZE 2

void sortArray(int array[], int size){
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

void displayArray(int array[], int size){

    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

}

int main(){


    int parentToChild[SIZE];
    int childToParent[SIZE];

    pipe(parentToChild);
    pipe(childToParent);

    if(fork() == 0){ //child Process

        close(parentToChild[1]); //parent to child
        close(childToParent[0]); //child to parent 

        int elements;

        read(parentToChild[0], &elements, sizeof(int));

        int* array = malloc(elements * sizeof(int));
        read(parentToChild[0], array, elements * sizeof(int));

        sortArray(array, elements);

        write(childToParent[1], array, elements * sizeof(int));

        free(array);
    }

    else{  //parent Process
         
        close(parentToChild[0]);
        close(childToParent[1]);

        int elements;
        printf("Enter number of elements: ");
        scanf("%d", &elements);

        int* array = malloc(elements * sizeof(int));
        printf("Enter elements:\n");
        for(int i = 0; i < elements; i++){
            scanf("%d", &array[i]);
        }

        printf("Before Sorting:\n");
        displayArray(array, elements);
        
        write(parentToChild[1], &elements, sizeof(int));
        write(parentToChild[1], array, elements * sizeof(int));

        wait(NULL);
        read(childToParent[0], array, elements * sizeof(int));

        printf("After Sorting:\n");
        displayArray(array, elements);

        free(array);

    }

    return 0;
}