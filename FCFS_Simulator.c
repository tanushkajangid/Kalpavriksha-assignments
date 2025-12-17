#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 1000
#define MAX_PROCESS 100
#define MAX_STRING 50
#define MAX_KILLED_PROCESS 100

typedef enum{

    READY = 1,
    RUNNING,
    WAITING,
    TERMINATED

}State;

typedef struct PcbNode{

    int processId;
    char name[MAX_STRING];

    int cpuBurstTotal;
    int cpuDone;

    int ioStartTime;
    int ioDuration;

    int ioRemaining;
    int ioStartedAt;

    State state;

    int arivalTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;

    bool killed;

    struct PcbNode *next;
    struct PcbNode *hashNext;

}PcbNode;

typedef struct queue{

    PcbNode* front;
    PcbNode* rear;
    int size;

}Queue;

typedef struct Hashmap{

    PcbNode* hashList[MAX_SIZE];

}Hashmap;

typedef struct {

    int processId;
    int killTime;

}KillProcess;

void enqueue(Queue *queue, PcbNode *process){

    process->next = NULL;

    if(!queue->front){
        queue->front = process;
        queue->rear = process;
        queue->size++;
        return;
    }
    
    queue->rear->next = process;
    queue->rear = process;
    queue->size++;
}

PcbNode* dequeue(Queue *queue){

    if(!queue->front) {
        return NULL;
    }

    PcbNode *temporaryNode = queue->front;
    queue->front = temporaryNode->next;

    if(!queue->front) {
        queue->rear = NULL;
    }

    queue->size--;
    temporaryNode->next = NULL;
    return temporaryNode;
}

PcbNode* removeProcess(Queue *queue, int processId){

    PcbNode *currentNode = queue->front;
    PcbNode *previousNode = NULL;

    while(currentNode){
        if(currentNode->processId == processId){
            if(previousNode) {
                previousNode->next = currentNode->next;
            }
            else{
                queue->front = currentNode->next;
            }
            if(currentNode == queue->rear){
                queue->rear = previousNode;
            }
            queue->size--;
            currentNode->next = NULL;
            return currentNode;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    return NULL;
}


int hash(int processId){

    return processId % MAX_SIZE;

}

void insert(Hashmap *map, PcbNode *node){

    int index = hash(node->processId);

    node->hashNext = map->hashList[index];
    map->hashList[index]= node;

}

PcbNode* search(Hashmap *map, int processId){

    int index = hash(processId);
    PcbNode *currentNode = map->hashList[index];

    while(currentNode){
        if(currentNode->processId == processId){
            return currentNode;
        }
        currentNode = currentNode->hashNext;
    }

    return NULL;
}

void computeTimes(PcbNode *processList[], int processCount){

    for(int i = 0; i < processCount; i++){
        PcbNode *process = processList[i];

        if(process->killed){
            process->turnAroundTime = -1;
            process->waitingTime = -1;
            continue;
        }

        process->turnAroundTime = process->completionTime - process->arivalTime;
        process->waitingTime = process->turnAroundTime - process->cpuBurstTotal;

        if(process->waitingTime < 0){
            process->waitingTime = 0;
        }
    }
}

PcbNode* createBlock(char* data){
    
    PcbNode* newProcess = calloc(1, sizeof(PcbNode));

    if(newProcess == NULL){
        printf("Memory allocation failed!\n");
        return NULL;
    }

    char ioStartInput[MAX_STRING], ioDurationInput[MAX_STRING];

    sscanf(data, "%s %d %d %s %s", newProcess->name, &newProcess->processId, &newProcess->cpuBurstTotal, ioStartInput, ioDurationInput);
    
    if(strcmp(ioStartInput, "-") == 0){
        newProcess->ioStartTime = -1;
    }
    else {
        newProcess->ioStartTime = atoi(ioStartInput);
    }

    if(strcmp(ioDurationInput, "-") == 0){
        newProcess->ioDuration = 0;
    }
    else {
        newProcess->ioDuration = atoi(ioDurationInput);
    }
    
    newProcess->arivalTime = 0;
    newProcess->completionTime = 0;
    newProcess->cpuDone = 0;
    newProcess->ioRemaining = newProcess->ioDuration;
    newProcess->ioStartedAt = -1;
    newProcess->state = READY;

    newProcess->next = NULL;
    newProcess->hashNext = NULL;

    return newProcess;
}

void initializeInput(Hashmap *map, Queue *readyQ, KillProcess killedProcess[], int *killCount, PcbNode *processList[], int *processCount){
    char data[MAX_STRING];

    while(fgets(data, sizeof(data), stdin)){
        if(data[0] == '\n') {
            break;
        }

        if(strncmp(data, "KILL", 4) == 0){
            int processId, time;

            if(sscanf(data, "KILL %d %d", &processId, &time)==2){
                killedProcess[*killCount].killTime = time;
                killedProcess[*killCount].processId = processId;
                (*killCount)++;
            }

            continue; 
        }

        PcbNode *process = createBlock(data);
        processList[*processCount] = process;
        (*processCount)++;
        enqueue(readyQ, process);
        insert(map,process);
    }

    return;
}

void simulateScheduler(Hashmap *map, Queue *readyQ, Queue *waitingQ, Queue *terminatedQ, KillProcess killedProcess[], 
                       PcbNode *processList[], int processCount, int killCount){

    PcbNode *running =NULL;
    int tick = 0;
    int completed = 0;

    while(completed < processCount){

        if(!running && readyQ->size == 0 && waitingQ->size == 0){
            break;
        }

        for(int i = 0; i < killCount; i++){

            if(killedProcess[i].killTime == tick){
                PcbNode *node = search(map, killedProcess[i].processId);

                if(node && node->state != TERMINATED){
                    node->killed = true;
                    node->state = TERMINATED;
                    node->completionTime = tick;

                    if(running && running->processId == node->processId){
                        running = NULL;
                    }

                    removeProcess(readyQ, node->processId);
                    removeProcess(waitingQ, node->processId);

                    enqueue(terminatedQ, node);
                    completed++;
                }
            }
        }

        if(!running){
            running = dequeue(readyQ);

            if(running){
                running->state = RUNNING;
            }
        }

        if(running){
            running->cpuDone++;

            if(running->ioDuration > 0 && running->cpuDone == running->ioStartTime){
                running->state = WAITING;
                running->ioStartedAt = tick + 1;
                enqueue(waitingQ, running);
                running = NULL;
            }

            else if(running->cpuDone == running->cpuBurstTotal){
                running->state = TERMINATED;
                running->completionTime = tick + 1;
                enqueue(terminatedQ, running);
                running = NULL;
                completed++;
            }
        }

        PcbNode *currentNode = waitingQ->front;

        while(currentNode){
            PcbNode *next = currentNode->next;

            if(currentNode->ioRemaining > 0 && currentNode->ioStartedAt >= 0 && tick >= currentNode->ioStartedAt){
                currentNode->ioRemaining--;

                if(currentNode->ioRemaining == 0){
                    PcbNode *done = removeProcess(waitingQ, currentNode->processId);
                    done->state = READY;
                    done->ioStartedAt = -1;
                    enqueue(readyQ, done);
                }
            }
            currentNode = next;
        }
        tick++;
    }

    computeTimes(processList, processCount);
}

void displayResults(PcbNode *processList[], int processCount){
    
    int killFound = 0;

    for(int i = 0; i < processCount; i++){

        if(processList[i]->killed){
            killFound = 1;
            break;
        }
    }

    if(killFound){

        printf("\n%-5s %-12s %-14s %-11s %-18s %-10s %10s\n",
            "PID",  "Name",  "CPUBurstTime", "IODuration",  "Status",  "TurnAround",  "Waiting");

        for(int i = 0; i < processCount; i++){
            PcbNode *process = processList[i];

            if(process->killed){
                printf("%-5d %-12s %-14d %-11d KILLED at %-8d %-10s %-10s\n", 
                    process->processId, process->name, process->cpuBurstTotal, process->ioDuration, process->completionTime, "-", "-");
            }

            else{
                printf("%-5d %-12s %-14d %-11d %-18s %-12d %-10d\n", 
                    process->processId, process->name, process->cpuBurstTotal, process->ioDuration,"OK", process->turnAroundTime, process->waitingTime);
            }
        }
    }

    else{
        printf("\n%-5s %-12s %-14s %-11s %-12s %-10s\n",
            "PID", "Name", "CPUBurst", "IODuration", "TurnAround", "Waiting");

        for(int i = 0; i < processCount; i++){
            PcbNode *process = processList[i];
            printf("%-5d %-12s %-14d %-11d %-12d %-10d\n", 
                process->processId, process->name, process->cpuBurstTotal, process->ioDuration, process->turnAroundTime, process->waitingTime);
        }
    }
}

int main(){

    Hashmap map = {0};
    Queue readyQ = {0}, terminatedQ = {0}, waitingQ = {0};

    KillProcess killedProcess[MAX_KILLED_PROCESS] = {0};
    int killCount = 0;

    PcbNode *processList[MAX_PROCESS] = {0};
    int processCount = 0;

    initializeInput(&map, &readyQ, killedProcess, &killCount, processList, &processCount);

    simulateScheduler(&map, &readyQ, &waitingQ, &terminatedQ, killedProcess, processList, processCount, killCount);
    displayResults(processList, processCount);

    return 0;
}
