#include <stdio.h>
#include <stdlib.h>
#include "structure.h"
#include "function.h"
#include "operations.h"
#include "utils.h"


int main(){

    TeamData teamArray[10];

    TeamData *teamPointerArray[MAX_TEAMS];  

    initializeTeams(teamArray, teamPointerArray);
    loadInitialPlayers(teamArray);

    showMenu(teamArray, teamPointerArray);

    freeAllPlayers(teamArray);
    return 0;
}
