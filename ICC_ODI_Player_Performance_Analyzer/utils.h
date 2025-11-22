#ifndef UTILS_H
#define UTILS_H

#include "structure.h"

int stringCaseCompare( const char* string1, const char* string2);

const char* roleToString(RoleCode role);

RoleCode stringToRoleCode( const char* roleStr);

int compareTeamId( const void *termA, const void *termB);

int compareTeamAverage(const void *termA, const void *termB);

void printPlayerDetails( PlayerData *player);

void printTableHeader();

int findTeamOrder(const char *name);

float PerformanceIndexEvaluation(RoleCode role, float battingAvg, float strikeRate, int wickets, float economyRate );

void updateAvgStrikeRate(TeamData *team);

void insertPlayer(TeamData *teamArray, PlayerData *node);

#endif