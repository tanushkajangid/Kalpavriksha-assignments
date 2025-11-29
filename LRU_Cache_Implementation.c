#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_MAP_SIZE 1000
#define MAX_CAPACITY 1000
#define MIN_CAPACITY 1
#define MAX_STRING_LENGTH 50

typedef struct Node{

    int key;
    char value[MAX_STRING_LENGTH];
    struct Node* next;
    struct Node* prev;
    struct Node* hashNext;

} Node;

typedef struct Queue{

    Node* front; 
    Node* rear;

} Queue;

typedef struct HashMap {

    Node* buckets[HASH_MAP_SIZE];

} HashMap;

int hash(int key){

    return key % HASH_MAP_SIZE;

}

int stringCaseCompare( const char* string1, const char* string2){

    while(*string1 && *string2){
        int difference = tolower((unsigned char)*string1) - tolower((unsigned char)*string2);
        if(difference != 0){
            return difference;
        }
        string1++;
        string2++;
    }

    return tolower((unsigned char)*string1) - tolower((unsigned char)*string2);
}

void stringCopy(char* destination, const char* source){

    int i = 0;
    while(source[i] != '\0' && i < MAX_STRING_LENGTH-1){
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';

}

void hashInsert(HashMap *map, Node* node){

    int index = hash(node->key);
    node->hashNext = map->buckets[index];
    map->buckets[index] = node;

}

Node* hashSearch(HashMap *map, int key){

    int index = hash(key);
    Node* curr = map->buckets[index];

    while(curr){
        if(curr->key == key){
            return curr;
        }
        curr = curr->hashNext;
    }

    return NULL;
}

void hashDelete(HashMap *map, int key){

    int index = hash(key);
    Node* curr = map->buckets[index];
    Node* prev = NULL;

    while(curr){
        if(curr->key == key){
            if(prev){
                prev->hashNext = curr->hashNext;
            }
            else{
                map->buckets[index] = curr->hashNext;
            }
            return;
        }
        prev = curr;
        curr= curr->hashNext;
    }

}

void moveNodeToFront(Queue *queue, Node* node){

    if(queue->front == node){
        return;
    }

    if(node->prev) {
        node->prev->next = node->next;
    }

    if(node->next){ 
        node->next->prev = node->prev;
    }

    if(queue->rear == node){
        queue->rear = node->prev;
    }

    node->next = queue->front;
    node->prev = NULL;

    if(queue->front){
        queue->front->prev = node;
    }
    queue->front = node;

    if(!queue->rear){
        queue->rear = node;
    }
}

void removeLRUNode(Queue *queue, HashMap *map, int* currentSize){

    if(!queue->rear){
        return;
    }

    Node* leastRecentNode = queue->rear;

    if(leastRecentNode->prev ){
        leastRecentNode->prev->next = NULL;
    }

    else{
        queue->front = NULL;
    }

    queue->rear = leastRecentNode->prev;
    hashDelete(map, leastRecentNode->key);

    free(leastRecentNode);
    (*currentSize)--;
    
}

Node* createCacheBlock(int key, char* value){

    Node* node  = malloc(sizeof(Node));

    if (!node) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    node->key = key;
    stringCopy(node->value, value);
    node->next = NULL;
    node->prev = NULL;
    node->hashNext = NULL;
    return node;
     
}

void initializeCache(Queue *queue, HashMap *map){

    queue->front = NULL;
    queue->rear = NULL;

    for(int i = 0; i < HASH_MAP_SIZE; i++){
        map->buckets[i] = NULL;
        
    }

}

void putKeyValue(int key, char* value, Queue *queue, HashMap *map, int capacity, int *currentSize){

    Node* node = hashSearch(map, key);

    if(node){
        stringCopy(node->value, value); 
        moveNodeToFront(queue, node);
        return ;
    }

    if(*currentSize == capacity){
        removeLRUNode(queue, map, currentSize);
    }

    Node* newNode = createCacheBlock(key, value);
    newNode->next = queue->front;

    if( queue->front){
        queue->front->prev = newNode;
    }

    queue->front = newNode;
    if(!queue->rear) {
        queue->rear = newNode;
    }

    hashInsert(map, newNode);
    (*currentSize)++;

}

char* getValue(HashMap *map, Queue *queue, int key){

    Node* node = hashSearch(map,key);

    if(!node){
        return NULL;
    }

    moveNodeToFront(queue, node);
    return node->value;

}

void freeCache(Queue *queue){

    Node* curr = queue->front;

    while(curr){
        Node* temp = curr;
        curr = curr->next;
        free(temp);
    }
} 

void takeCommands(){

    HashMap map;
    Queue queue;

    int capacity = 0;
    int currentSize = 0;

    char outputList[2000][MAX_STRING_LENGTH];
    int outputCount = 0;

    initializeCache(&queue, &map);

    char command[MAX_STRING_LENGTH];
    char value[MAX_STRING_LENGTH];
    int key;

    printf("Input:\n");

    while(1){

        if(scanf("%s", command) != 1){
            continue;
        }
        
        if(stringCaseCompare(command, "createCache") == 0){

            while(1){
                if(scanf("%d", &capacity) != 1 || capacity > MAX_CAPACITY || capacity < MIN_CAPACITY){
                    printf("Invalid size!\n");
                    while ( getchar() != '\n');
                }
                break;
            }
            currentSize = 0;
            initializeCache(&queue, &map);

        }

        else if( stringCaseCompare(command, "put") == 0){

            while(1){
                if(scanf("%d %s", &key, value) != 2 || key < 0 ){
                    printf("Invalid key or value not inserted\n");
                    while(getchar() != '\n');
                }
                break;
            }
            putKeyValue(key, value, &queue, &map, capacity, &currentSize);

        }

        else if (stringCaseCompare (command, "get") == 0){

            while(1){
                if(scanf("%d", &key) != 1 || key < 0 ){
                    printf("Invalid key \n");
                    while(getchar() != '\n');
                }
                break;
            }
            char* result = getValue(&map, &queue, key);

            if(result){
                stringCopy( outputList[ outputCount++], result);
            }
            else {
                stringCopy( outputList[ outputCount++ ], "NULL");
            }

        }

        else if (stringCaseCompare(command, "exit") == 0){

            printf("\nOutput: \n");

            for(int i = 0; i < outputCount; i++){
                printf("%s\n", outputList[i] );
            }

            freeCache(&queue);
            break;
        }

    }

}

int main(){

    takeCommands();
    return 0;

}
