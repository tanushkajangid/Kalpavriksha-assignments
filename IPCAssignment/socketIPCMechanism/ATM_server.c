#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 256
#define DB_FILE "ATM_resource.txt"

pthread_mutex_t mutex;

void withdraw(int clientSocket, FILE *file, int *currentBalance){

    int transactionAmount;
    char response[BUFFER_SIZE];

    read(clientSocket, &transactionAmount, sizeof(int));

    if(transactionAmount <= *currentBalance){

        *currentBalance -= transactionAmount;
        fseek(file, 0, SEEK_SET);
        fprintf(file, "%d", *currentBalance);

        strcpy(response, "Withdrawal Successful");
    
    }

    else{

        strcpy(response, "Insufficient balance");

    }

    write(clientSocket, response, BUFFER_SIZE);
}

void deposit(int clientSocket, FILE *file, int *currentBalance){
    
    int transactionAmount;
    char response[BUFFER_SIZE];

    read(clientSocket, &transactionAmount, sizeof(int));

    *currentBalance += transactionAmount;
    fseek(file, 0, SEEK_SET);
    fprintf(file, "%d", *currentBalance);

    strcpy(response, "Deposit successful");
    write(clientSocket, response, BUFFER_SIZE);

}

void *processClientRequest(void *socket){
    
    int clientSocket = *(int *)socket;
    int operation;
    int currentBalance;

    FILE *file;

    if( read(clientSocket, &operation, sizeof(int)) <= 0){

        close(clientSocket);
        free(socket);
        return NULL;

    }

    pthread_mutex_lock(&mutex);

    file = fopen(DB_FILE, "r+");

    if (file == NULL) {

        file = fopen(DB_FILE, "w+");

        if (file == NULL) {
            
            perror("DB file error");
            pthread_mutex_unlock(&mutex);
            close(clientSocket);
            free(socket);
            return NULL;
            
        }

        fprintf(file, "0");
        rewind(file);
    }

    fscanf(file, "%d", &currentBalance);

    switch(operation){

        case 1:
            withdraw(clientSocket, file, &currentBalance);
            break;

        case 2:
            deposit(clientSocket, file, &currentBalance);
            break;

        case 3:
            write(clientSocket, &currentBalance, sizeof(int));
            break;

        default:
            char response[BUFFER_SIZE] = "Invalid Operation";
            write(clientSocket, response, BUFFER_SIZE);
    }

    fclose(file);
    pthread_mutex_unlock(&mutex);

    close(clientSocket);
    free(socket);

    return NULL;
}


int main(){

    int serverSocket;
    int *clientSocket;

    struct sockaddr_in serverAddress;
    pthread_t threadId;

    pthread_mutex_init(&mutex, NULL);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    printf("ATM Server running on port %d...\n", SERVER_PORT);

    while(1){

        clientSocket = malloc(sizeof(int));
        *clientSocket = accept(serverSocket, NULL, NULL);

        if (*clientSocket < 0) {
            perror("accept failed");
            free(clientSocket);
            continue;
        }

        pthread_create(&threadId, NULL, processClientRequest, clientSocket);
        pthread_detach(threadId);
    }

    close(serverSocket);
    pthread_mutex_destroy(&mutex);

    return 0;
}
