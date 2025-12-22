#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_SIZE 100

typedef struct message{

    long type;
    int element;
    int data[MAX_SIZE];

}message;

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

void displayArray(int array[], int size){

    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");

}


int main(){

    key_t key = ftok("messageQueueIPCMechanism.c", 65);
    int messageId = msgget(key, 0666 | IPC_CREAT);

    message msg;

    if(fork() == 0){  //child process
        msgrcv(messageId, &msg, sizeof(msg) - sizeof(long), 1, 0);

        sort(msg.data, msg.element);

        msg.type = 2;
        msgsnd(messageId, &msg, sizeof(msg) - sizeof(long) , 0);
    }

    else{ //parent process

        msg.type = 1;
        printf("Enter number of elements:");
        scanf("%d", &msg.element);

        printf("Enter elements:\n");
        for(int i =0; i< msg.element; i++){
            scanf("%d", &msg.data[i]);
        }

        printf("Before Sorting:\n");
        displayArray(msg.data, msg.element);

        msgsnd(messageId, &msg, sizeof(msg) - sizeof(long), 0);

        msgrcv(messageId, &msg, sizeof(msg) - sizeof(long) , 2, 0);

        printf("After Sorting:\n");
        displayArray(msg.data, msg.element);

        msgctl(messageId, IPC_RMID, NULL);
    }
    
    return 0;
}