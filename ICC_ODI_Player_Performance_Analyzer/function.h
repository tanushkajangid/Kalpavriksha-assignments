#ifndef FUNCTION_H
#define FUNCTION_H

#include "structure.h"


void addPlayerToTeam(TeamData **teamPointerArray);

void displayPlayerOfTeam(TeamData **teamPointerArray);

void displayTeamByAvgBattingStrikeRate(TeamData *teamArray);

void displayTopKPlayers(TeamData **teamPointerArray);

void displayAllPlayersByRoleOnPI(TeamData * teamArray);

#endif