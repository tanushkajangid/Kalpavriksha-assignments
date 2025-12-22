#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 256

int main(){
    
    int clientSocket;
    int choice;
    int amount;
    int balanceAmount;
    char response[BUFFER_SIZE];

    struct sockaddr_in serverAddress;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    printf("\n ATM MENU \n");
    printf("1. Withdraw\n");
    printf("2. Deposit\n");
    printf("3. Display Balance\n");
    printf("4. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 4) {
        close(clientSocket);
        return 0;
    }

    write(clientSocket, &choice, sizeof(int));

    if(choice == 1 || choice == 2){

        printf("Enter Amount: ");
        scanf("%d", &amount);
        
        write(clientSocket, &amount, sizeof(int));
        read(clientSocket, response, BUFFER_SIZE);
        printf("Server Response: %s\n", response);
        
    }

    else if (choice == 3){

        read(clientSocket, &balanceAmount, sizeof(int));
        printf("Current Balance: %d\n", balanceAmount);

    } 
    
    else {

        read(clientSocket, response, BUFFER_SIZE);
        printf("Server Response: %s\n", response);
    }

    close(clientSocket);

    return 0;
}