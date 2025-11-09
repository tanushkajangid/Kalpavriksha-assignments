#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define BLOCK_SIZE 512
#define NUMBER_OF_BLOCKS 1024
#define MAX_NUMBER_OF_BLOCKS 5000
#define MAX_NAME_LENGTH 50
#define PATH_LENGTH 100
#define MAX_PATH_LENGTH 500
#define PART_LENGTH 128
#define MAX_PART_LENGTH 256
#define MAX_TOKEN 20

typedef struct FreeBlockDoublyLinkedList{

    int blockIndex;
    struct FreeBlockDoublyLinkedList *nextNode;
    struct FreeBlockDoublyLinkedList *previousNode;
    
}FreeBlock;

typedef struct FileNodeData{
    char fileName[MAX_NAME_LENGTH];
    int blockPointer[MAX_NUMBER_OF_BLOCKS];   
    int isDirectory;
    int fileContentSize;     
    struct FileNodeData *parentNode;   
    struct FileNodeData *childNode;
    struct FileNodeData *nextNode;
    struct FileNodeData *previousNode;

}FileNode;


//*********************************************************************//

char virtualDisk[NUMBER_OF_BLOCKS][BLOCK_SIZE];

FreeBlock *head = NULL;
FreeBlock *tail = NULL;

FileNode *root = NULL;
FileNode *CurrentWorkingDirectory = NULL;

//*********************************************************************//

int allocateBlock(){

    if( head == NULL){
        return -1;
    }

    int index = head->blockIndex;
    FreeBlock *temporary = head;
    head = head->nextNode;

    if(head != NULL){
        head->previousNode = NULL;
    }
    else{
        tail = NULL;
    }

    free(temporary);
    return index;

}

void freeBlock(int index){

    if (index < 0 || index >= NUMBER_OF_BLOCKS){
        return;
    }

    FreeBlock *block = malloc(sizeof(FreeBlock));
    block->blockIndex = index;
    block->nextNode = NULL;
    block->previousNode = tail;

    if(tail != NULL){
        tail->nextNode = block;
    }
    else{
        head = block;
    }

    tail = block;
}

int countFreeBlocks(){
    int count = 0;
    FreeBlock *temporary = head;
    while(temporary){
        count++;
        temporary = temporary->nextNode;
    }

    return count;
}

void initializeFreeDLL(){
    head = NULL;
    tail = NULL;

    for(int iterator = 0; iterator < NUMBER_OF_BLOCKS; iterator++){
        freeBlock(iterator);
    }
}

//*********************************************************************************** */

FileNode *createNode(const char *name, int isDirectory){

    FileNode *node = malloc(sizeof(FileNode));

    strcpy(node->fileName, name);
    node->isDirectory = isDirectory;
    node->fileContentSize = 0;
    node->childNode = NULL;
    node->parentNode = CurrentWorkingDirectory;
    node->nextNode = NULL;
    node->previousNode = NULL;

    for(int iterator = 0; iterator < MAX_NUMBER_OF_BLOCKS; iterator++){
        node->blockPointer[iterator] = -1;
    }

    return node;
}

void insertChildNode(FileNode *parent, FileNode *child){
    if (!parent || !child) {
        return;
    }

    child->parentNode = parent;

    if(parent->childNode == NULL){
        parent->childNode = child;
        child->nextNode = child;
        child->previousNode = child;
    }

    else{
        FileNode *headNode = parent->childNode; 
        FileNode *tailNode = headNode->previousNode;

        tailNode->nextNode = child;
        child->previousNode = tailNode;

        child->nextNode = headNode;
        headNode->previousNode = child;

    }
}

FileNode *findChildNode(FileNode *parent, const char *name){

    if(!parent || parent->childNode == NULL){
        return NULL;
    }

    FileNode *current = parent->childNode;
     
    do{
        if(strcmp(current->fileName, name) == 0){
            return current;
        }
        current = current->nextNode;

    } while(current != parent->childNode);

    return NULL;
}

void removeChildNode(FileNode *parent, FileNode *target){

    if(!parent || parent->childNode == NULL || target == NULL){
        return;
    }

    if(target->nextNode == target){
        parent->childNode = NULL;
    }
    else{
        if(parent->childNode == target){
            parent->childNode = target->nextNode;
        }
        target->previousNode->nextNode = target->nextNode;
        target->nextNode->previousNode = target->previousNode;
    }

    free(target);
}

//*********************************************************** */

void initializeVirtualFileSystem(){

    initializeFreeDLL();

    CurrentWorkingDirectory = NULL;
    root = createNode("/",1);
    root->parentNode = NULL;
    CurrentWorkingDirectory = root;
}

void freeFileTree(FileNode *node) {
    if (!node) {
        return;
    }
    if (node->isDirectory) {
        
        while (node->childNode) {

            FileNode *child = node->childNode;

            if (child->nextNode == child) {
                node->childNode = NULL;
            } 
            else {
                node->childNode = child->nextNode;
                child->previousNode->nextNode = child->nextNode;
                child->nextNode->previousNode = child->previousNode;
            }

            freeFileTree(child);
        }
        free(node);

    } 
    else {
        
        for (int iterator = 0; iterator < NUMBER_OF_BLOCKS && node->blockPointer[iterator] != -1; iterator++) {

            freeBlock(node->blockPointer[iterator]);
            node->blockPointer[iterator] = -1;
        }
        free(node);
    }
}

void cleanUpVirtualFileSystem(){

    if(root){
        freeFileTree(root);
        root = NULL;
        CurrentWorkingDirectory = NULL;
    }
    while(head){
        FreeBlock *temporary = head;
        head = head->nextNode;
        free(temporary);
    }
    tail = NULL;

    printf("Memory released. Exiting program...\n");
}

//******************************************************************************* */

void makeDirectory(char *name){    //mkdir

    if (!name) {
        printf("Usage: mkdir <dirname>\n");
        return; 
    }

    if(findChildNode(CurrentWorkingDirectory, name)){
        printf("Directory already exits.\n");
        return ;
    }

    FileNode *directory = createNode(name, 1);
    insertChildNode(CurrentWorkingDirectory, directory);
    printf("Directory '%s' created successfully.\n", name);
    
}

void create(char *name){

    if (!name) { 
        printf("Usage: create <filename>\n"); 
        return; 
    }

    if(findChildNode(CurrentWorkingDirectory, name)){
        printf("File already exists.\n");
        return;
    }

    FileNode *file = createNode(name, 0);
    insertChildNode(CurrentWorkingDirectory, file);
    printf("File '%s' created successfully.\n", name);
    
}

void writeFile(char *name, char *data){

    if (!name || !data) { 
        printf("Usage: write <filename> <data>\n"); 
        return; 
    }

    FileNode *file = findChildNode(CurrentWorkingDirectory, name);

    if(!file || file->isDirectory){
        printf("File not found.\n");
        return;
    }

    for (int iterator = 0; iterator < MAX_NUMBER_OF_BLOCKS && file->blockPointer[iterator] != -1; iterator++) {
        freeBlock(file->blockPointer[iterator]);
        file->blockPointer[iterator] = -1;
    }

    int length = strlen(data);
    int blocksNeeded = (length + BLOCK_SIZE - 1) / BLOCK_SIZE;

    if (blocksNeeded > NUMBER_OF_BLOCKS) { 
        printf("File too large.\n"); 
        return; 
    }

    int freeCount = countFreeBlocks();
    if (blocksNeeded > freeCount) { 
        printf("Disk full. Write failed.\n"); 
        return; 
    }

    int offset = 0;

    for (int iterator = 0; iterator < blocksNeeded; iterator++) {
        int blockIndex = allocateBlock();

        if (blockIndex == -1) { 
            printf("Unexpected disk full.\n"); 
            return; 
        }

        file->blockPointer[iterator] = blockIndex;

        int bytesToCopy = (length - offset) < BLOCK_SIZE ? (length - offset) : BLOCK_SIZE;
        memcpy(virtualDisk[blockIndex], data + offset, bytesToCopy);
        offset += bytesToCopy;
    }

    file->fileContentSize = length;
    printf("Data written successfully (size=%d bytes).\n", length);
}

void readFile(char *name){

    if (!name) { 
        printf("Usage: read <filename>\n"); 
        return; 
    }

    FileNode *file = findChildNode(CurrentWorkingDirectory, name);

    if(!file || file ->isDirectory){
        printf("File Not Found.\n");
        return ;
    }

    printf("File Content: ");
    int remainingSize = file->fileContentSize;

    for(int iterator = 0; iterator < MAX_NUMBER_OF_BLOCKS && file->blockPointer[iterator] != -1 && remainingSize > 0; iterator++){
        int blockIndex = file->blockPointer[iterator];
        int bytesToRead ;
        if(remainingSize >BLOCK_SIZE ){
            bytesToRead = BLOCK_SIZE;
        }
        else{
            bytesToRead = remainingSize;
        }

        for(int bytes = 0; bytes < bytesToRead; bytes++){
            putchar(virtualDisk[blockIndex][bytes]);
        }
        remainingSize -= bytesToRead;

    }

    printf("\n");
} 

void deleteFile(char *name){

    if (!name) { 
        printf("Usage: delete <filename>\n"); 
        return; 
    }

    FileNode *file = findChildNode(CurrentWorkingDirectory, name);

    if(!file){
        printf("File Not found.\n");
        return;
    }

    if(file->isDirectory){
        printf("cannot delete a directory, Use 'rmdir' instead.\n");
        return;
    }

    for(int iterator = 0; iterator < MAX_NUMBER_OF_BLOCKS && file ->blockPointer[iterator] != -1; iterator++){
        freeBlock(file->blockPointer[iterator]);
        file->blockPointer[iterator] = -1;
    }

    removeChildNode(CurrentWorkingDirectory, file);

    printf("File '%s' delete successfully.\n", name);
}      

void removeDirectory(char *name){

    if (!name) { 
        printf("Usage: rmdir <dirname>\n"); 
        return; 
    }

    FileNode *directory = findChildNode(CurrentWorkingDirectory, name);

    if (!directory) {
        printf("Directory not found.\n");
        return;
    }

    if (!directory->isDirectory) {
        printf("'%s' is not a directory.\n", name);
        return;
    }

    if (directory->childNode != NULL) {
        printf("Directory '%s' is not empty.\n", name);
        return;
    }

    removeChildNode(CurrentWorkingDirectory, directory);

    printf("Directory '%s' removed successfully.\n", name);

} 

void list(){

    if(!CurrentWorkingDirectory-> childNode){
        printf("empty\n");
        return ;
    }

    FileNode *current = CurrentWorkingDirectory->childNode;
    do{
        printf("%s%s  ", current->fileName, current->isDirectory ? "/" : "");
        current = current->nextNode;

    } while(current != CurrentWorkingDirectory->childNode);

    printf("\n");
}  

void changeDirectory(char *name){

    if (!name) { 
        printf("Usage: cd <dirname>\n"); 
        return; 
    }

    if (strcmp(name, "..") == 0) {
        if (CurrentWorkingDirectory->parentNode) {
            CurrentWorkingDirectory = CurrentWorkingDirectory->parentNode;
        }
        return;
    }

    FileNode *directory = findChildNode(CurrentWorkingDirectory, name);

    if (!directory || !directory->isDirectory) {
        printf("Directory not found.\n");
        return;
    }
    CurrentWorkingDirectory = directory;
} 

void pathOfWorkingDirectory(){

    if (CurrentWorkingDirectory == root) { 
        printf("/\n"); 
        return; 
    }
    FileNode *temporary = CurrentWorkingDirectory;
    char path[MAX_PATH_LENGTH] = "";

    while (temporary && temporary != root) {
        char tempPath[PATH_LENGTH];
        snprintf(tempPath, sizeof(tempPath), "/%s%s", temporary->fileName, path);
        strncpy(path, tempPath, sizeof(tempPath)-1);
        path[sizeof(path)-1] = '\0';
        temporary = temporary->parentNode;
    }
    printf("%s\n", path[0] ? path : "/");

} 

void availableFreeDisk(){

    int freeBlocks = countFreeBlocks();
    int usedBlocks = NUMBER_OF_BLOCKS - freeBlocks;
    double usagePercent = ((double)usedBlocks / NUMBER_OF_BLOCKS) * 100.0;

    printf("Total Blocks: %d\n", NUMBER_OF_BLOCKS);
    printf("Used Blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usagePercent);
}

int inputParser(char *userInput, char *tokens[]){
    int count = 0;
    char *token = strtok(userInput, " \t\n");
    while(token != NULL && count < MAX_TOKEN-1 ){
        tokens[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[count] = NULL;
    return count;
}

void executeCommand(int tokenCount, char *tokens[]){

    if(tokenCount == 0){
        return;
    }

    char *command = tokens[0];

    if(!strcmp(command, "mkdir")){
        if (tokenCount < 2) {
            printf("Usage: mkdir <dirname>\n");
        }
        else{
            makeDirectory(tokens[1]);
        }
    }

    else if (!strcmp(command, "create")) {
        if (tokenCount < 2) {
            printf("Usage: create <filename>\n");
        }
        else {
            create(tokens[1]);
        }
    }

    else if (!strcmp(command, "write")) {
        if (tokenCount < 3) {
            printf("Usage: write <filename> <data>\n");
        }
        else {

           char data[BLOCK_SIZE * 10] = "";
            for (int i = 2; i < tokenCount; i++) {
                strcat(data, tokens[i]);
                if (i < tokenCount - 1)
                    strcat(data, " ");
            }
            writeFile(tokens[1], data);
        }
    }

    else if (!strcmp(command, "read")) {
        if (tokenCount < 2)
            printf("Usage: read <filename>\n");
        else{
            readFile(tokens[1]);
        }
    }

    else if (!strcmp(command, "delete")) {
        if (tokenCount < 2)
            printf("Usage: delete <filename>\n");
        else {
            deleteFile(tokens[1]);
        }
    }

    else if (!strcmp(command, "rmdir")) {
        if (tokenCount < 2)
            printf("Usage: rmdir <dirname>\n");
        else{
            removeDirectory(tokens[1]);
        }
    }

    else if (!strcmp(command, "ls")) {
        list();
    }

    else if (!strcmp(command, "cd")) {
        if (tokenCount < 2)
            printf("Usage: cd <dirname>\n");
        else{
            changeDirectory(tokens[1]);
        }
    }

    else if (!strcmp(command, "pwd")) {
        pathOfWorkingDirectory();
    }

    else if (!strcmp(command, "df")) {
        availableFreeDisk();
    }

    else if (!strcmp(command, "exit")) { 
        cleanUpVirtualFileSystem();
        exit(0);
    }

    else {
        printf("Unknown command: %s\n", command);
    }
}

int main(){

    char userInput[BLOCK_SIZE];

    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    char *tokens[MAX_TOKEN];

    initializeVirtualFileSystem();

    while(1){

        if(CurrentWorkingDirectory == root){
            printf("/ > ");
        }
        else{
            char path[MAX_PART_LENGTH] = "";
            FileNode *temporary = CurrentWorkingDirectory;
            while( temporary && temporary != root){
                char part[PART_LENGTH];
                snprintf(part, sizeof(part), "/%s%s", temporary->fileName, path);
                strncpy(path, part, sizeof(path)-1);
                path[sizeof(path)-1] = '\0';
                temporary = temporary->parentNode;
            }
            printf("%s > ", path[0] ? path : "/");
        }

        if(fgets(userInput, sizeof(userInput),stdin) == NULL){
            break;
        }

        int tokenCount = inputParser(userInput, tokens);
        executeCommand(tokenCount, tokens);
    }
    
    cleanUpVirtualFileSystem();
    return 0;
}
