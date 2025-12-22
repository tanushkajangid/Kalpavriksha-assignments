#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_SIZE 100

void sort(int array[], int size){

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

    key_t key = ftok("sharedMemoryIPCMechanism.c", 65);
    int sharedMemeoryId = shmget(key, sizeof(int) * (MAX_SIZE + 1), 0666 | IPC_CREAT);

    int *sharedMemeory = shmat(sharedMemeoryId, NULL, 0);

    printf("Enter number of elements:");
    scanf("%d", &sharedMemeory[0]);

    printf("Enter elements:\n");
    for(int i =0; i< sharedMemeory[0]; i++){
        scanf("%d", &sharedMemeory[i+1]);
    }

    printf("Before Sorting:\n");
    displayArray(&sharedMemeory[1], sharedMemeory[0]);

    if(fork() == 0){  //child process

        sort(&sharedMemeory[1], sharedMemeory[0]);
        shmdt(sharedMemeory);

    }

    else{ //parent process

        sleep(1);

        printf("After Sorting:\n");
        displayArray(&sharedMemeory[1], sharedMemeory[0]);

        shmdt(sharedMemeory);
        shmctl(sharedMemeoryId, IPC_RMID, NULL);

    }
    
    return 0;
}