#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search_sort.h"
#include "structure.h"

int binarySearchTeam(TeamData **teamPointerArray, int indexCount, int teamId) {

    int leftIndex = 0;
    int rightIndex = indexCount - 1;

    while(leftIndex <= rightIndex){

        int midIndex = leftIndex + (rightIndex - leftIndex) / 2;

        if(teamPointerArray[midIndex]->teamId == teamId){
            return midIndex;
        }
        else if(teamPointerArray[midIndex]->teamId < teamId){
            leftIndex = midIndex + 1;
        }
        else{
            rightIndex = midIndex - 1;
        }
    }

    return -1;
}

void swapBytes(char *term1, char *term2, size_t size) {

    while(size--){
        char temp = *term1;
        *term1 = *term2;
        *term2 = temp;
        term1++;
        term2++;
    }
    
}

int partitionArray(void *array, int low, int high, size_t elementSize, int (*compareFunction)(const void *, const void *)){

    char* base = (char*)array;
    char* pivot = base + high * elementSize;
    int i = low - 1;

    for(int j = low; j <= high-1; j++){
        char* element = base + j * elementSize;

        if(compareFunction( element, pivot) < 0){
            i++;
            swapBytes(base + i * elementSize, element, elementSize);
        }
    }
    swapBytes(base + (i+1) * elementSize, pivot, elementSize);
    return i+1;
}

void quickSort(void *array, int low, int high, size_t elementSize, int (*compareFunction)(const void *, const void *)){
    if(low < high){     
        int partitionIndex = partitionArray((char*) array, low, high, elementSize, compareFunction);
        quickSort(array, low, partitionIndex -1, elementSize, compareFunction);
        quickSort(array, partitionIndex +1, high, elementSize, compareFunction);
    }
}

void heapify(PlayerData **array, int n, int i) {

    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if (left < n && array[left]->performanceIndex > array[largest]->performanceIndex){
        largest = left;
    }

    if (right < n && array[right]->performanceIndex > array[largest]->performanceIndex){
        largest = right;
    }

    if (largest != i) {
        PlayerData *temp = array[i];
        array[i] = array[largest];
        array[largest] = temp;
        heapify(array, n, largest);
    }
}

void heapSort(PlayerData **array, int n) {

    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(array, n, i);
    }

    for (int i = n - 1; i >= 0; i--) {
        PlayerData *temp = array[0];
        array[0] = array[i];
        array[i] = temp;
        heapify(array, i, 0);
    }
}
