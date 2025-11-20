#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H
#include <stdlib.h>
#include "structure.h"

int binarySearchTeam(TeamData **teamPointerArray, int indexCount, int teamId);

void swapBytes(char *term1, char *term2, size_t size);

int partitionArray(void *array, int low, int high, size_t elementSize, int (*compareFunction)(const void *, const void *));

void quickSort(void *array, int low, int high, size_t elementSize, int (*compareFunction)(const void *, const void *));

void heapify(PlayerData **array, int n, int i);

void heapSort(PlayerData **array, int n);

#endif