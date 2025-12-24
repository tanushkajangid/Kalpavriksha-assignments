#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include "utilities.h"

#define MAX_SIZE 100

typedef struct Message{

    long type;
    int element;
    int data[MAX_SIZE];

} Message;

void parentProcess(int messageId){

    Message message;
    message.type = 1;

    printf("Enter number of elements:");
    scanf("%d", &message.element);

    printf("Enter elements:\n");
    for(int i =0; i< message.element; i++){
        scanf("%d", &message.data[i]);
    }

    printf("Before Sorting:\n");
    displayArray(message.data, message.element);

    msgsnd(messageId, &message, sizeof(message) - sizeof(long), 0);

    msgrcv(messageId, &message, sizeof(message) - sizeof(long) , 2, 0);

    printf("After Sorting:\n");
    displayArray(message.data, message.element);

    msgctl(messageId, IPC_RMID, NULL);
}

void childProcess(int messageId){

    Message message;

    msgrcv(messageId, &message, sizeof(message) - sizeof(long), 1, 0);

    sortArray(message.data, message.element);

    message.type = 2;
    msgsnd(messageId, &message, sizeof(message) - sizeof(long) , 0);

    exit(0);
}

int main(){

    key_t key = ftok("messageQueueIPCMechanism.c", 65);
    int messageId = msgget(key, 0666 | IPC_CREAT);

    if(fork() == 0){

        childProcess(messageId);
    }

    else{

        parentProcess(messageId);

    }
    
    return 0;
}
