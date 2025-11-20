#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "structure.h"

void showMenu(TeamData *teamArray, TeamData **teamPointerArray);

void initializeTeams(TeamData *teamArray, TeamData **teamPointerArray);

void loadInitialPlayers(TeamData *teamArray);

void freeAllPlayers(TeamData *teamArray);

#endif