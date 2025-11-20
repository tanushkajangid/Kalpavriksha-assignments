#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Players_data.h"
#include "search_sort.h"
#include "function.h"
#include "structure.h"
#include "utils.h"


void addPlayerToTeam(TeamData **teamPointerArray){
    
    int teamId;
    printf("Enter Team ID to add player: ");
    
    scanf("%d", &teamId);
    if (teamId < 1 || teamId > 10) {
        return;
    }

    int position = binarySearchTeam(teamPointerArray, MAX_TEAMS, teamId);

    if(position == -1){
        printf("Team not found!\n");
        return;
    }

    TeamData *team = teamPointerArray[position];

    if(!team) {
        return;
    }

    PlayerData *newPlayer = malloc(sizeof(PlayerData));

        if (newPlayer== NULL) {
            printf("\nMemory allocation failed.\n");
            return;
        }

    printf("Enter Player Details:\n");

    printf("Player ID: ");
    scanf("%d", &newPlayer->playerId);
    if(newPlayer->playerId > MAX_PLAYER_ID){
        newPlayer->playerId = MAX_PLAYER_ID;
    }

    printf("Name: ");
    scanf(" %[^\n]", newPlayer->name);

    int roleCode;
    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder): ");

    while(1){
        if(scanf("%d", &roleCode) != 1 || roleCode > 3 || roleCode < 1){
            printf("Invalid Role!\nPlease enter valid role (1-Batsman, 2-Bowler, 3-All-rounder)\n");
            while(getchar() != '\n');
            continue;
        }
        break;
    }
    
    newPlayer->role = (RoleCode)roleCode;

    printf("Total Runs: ");
    scanf("%d", &newPlayer->totalRuns);

    printf("Batting Average: ");
    scanf("%f", &newPlayer->battingAvg);

    printf("Strike Rate: ");
    scanf("%f", &newPlayer->strikeRate);

    printf("Wickets: ");
    scanf("%d", &newPlayer->wickets);

    printf("Economy Rate: ");
    scanf("%f", &newPlayer->economyRate);

    strcpy(newPlayer->playerTeamName, team->teamName);

    newPlayer->performanceIndex = PerformanceIndexEvaluation(newPlayer->role, newPlayer->battingAvg, newPlayer->strikeRate, newPlayer->wickets, newPlayer->economyRate);
    newPlayer->next = NULL;

    insertPlayer(team, newPlayer);

    printf("Player added successfully to Team %s!\n", team->teamName);
}

void displayPlayerOfTeam(TeamData **teamPointerArray){

    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    int position = binarySearchTeam(teamPointerArray, MAX_TEAMS, teamId);

    if(position == -1){
        printf("Team not found!\n");
        return;
    }

    TeamData *team = teamPointerArray[position];

    printf("Players of Team %s:\n", team->teamName);
    
    printTableHeader();

    PlayerData *player = team->head;

    while(player) {
        printPlayerDetails(player);
        player = player->next;
    }

    printf("=========================================================================================================\n");

    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team->avgBattingStrikeRate);
}

void displayTeamByAvgBattingStrikeRate(TeamData *teamArray){

    TeamData *tempArray[MAX_TEAMS];

    for(int i = 0; i < MAX_TEAMS; i++){
        tempArray[i] = &teamArray[i];
    }

    quickSort(tempArray, 0, MAX_TEAMS - 1, sizeof(TeamData*), compareTeamAverage);

    printf("Teams Sorted by Average Batting Strike Rate\n");
        printf("=========================================================\n");
        printf("%-5s %-20s %-15s %-15s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
        printf("=========================================================\n");

        for (int i = 0; i < MAX_TEAMS; i++) {
            printf("%-5d %-20s %-15.1f %-15d\n",
                tempArray[i]->teamId, tempArray[i]->teamName, tempArray[i]->avgBattingStrikeRate, tempArray[i]->totalPlayers);
        }

        printf("=========================================================\n");
    
}

void displayTopKPlayers(TeamData **teamPointerArray){

    int teamId, roleCode, kPlayerCount;

    printf("Enter Team ID: ");
    scanf("%d", &teamId);

    int position = binarySearchTeam(teamPointerArray, MAX_TEAMS, teamId);

    if(position == -1){
        printf("Team not found!\n");
        return;
    }

    TeamData *team = teamPointerArray[position];

    if (!team){
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");

    while(1){
        if(scanf("%d", &roleCode) != 1 || roleCode > 3 || roleCode < 1){
            printf("Invalid Role!\nPlease enter valid role (1-Batsman, 2-Bowler, 3-All-rounder)\n");
            while(getchar() != '\n');
            continue;
        }
        break;
    }

    RoleCode role = (RoleCode)roleCode;

    printf("Enter number of players: ");
    scanf("%d", &kPlayerCount);

    PlayerData *player = team->head;
    PlayerData *tempArray[team->totalPlayers];

    int indexCount = 0;

    while(player){

        if (player->role == role){

            tempArray[indexCount++] = player;

        }
        player = player->next;
    }

    for(int i = indexCount/2 - 1; i >= 0; i--){
        heapify(tempArray, indexCount, i);
    }
    
    if(kPlayerCount > indexCount){
        kPlayerCount = indexCount;
    }

    printf("Top %d %s of Team %s:\n", kPlayerCount, roleToString(role), team->teamName);

    printTableHeader();

    for (int i = 0; i < kPlayerCount; i++) {

        PlayerData *topPlayer = tempArray[0];

        printPlayerDetails(topPlayer);
        tempArray[0] = tempArray[indexCount - 1 - i];

        heapify(tempArray, indexCount - 1 - i, 0);
    }
}

void displayAllPlayersByRoleOnPI(TeamData * teamArray){
    
    int roleCode;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");

    while(1){
        if(scanf("%d", &roleCode) != 1 || roleCode > 3 || roleCode < 1){
            printf("Invalid Role!\nPlease enter valid role (1-Batsman, 2-Bowler, 3-All-rounder)\n");
            while(getchar() != '\n');
            continue;
        }
        break;
    }

    RoleCode role = (RoleCode)roleCode;

    PlayerData *list[MAX_LIST_PLAYERS];     
    int indexCount = 0;

    for (int i = 0; i < MAX_TEAMS; i++) {
        PlayerData *player = teamArray[i].head;

        while(player) {
            if(player->role == role) {
                list[indexCount++] = player;
            }
            player = player->next;
        }
    }

    if (indexCount == 0) {
        printf("No players of role %s found.\n", roleToString(role));
        return;
    }

    heapSort(list, indexCount);

    printf("%s of all teams (sorted by Performance Index):\n", roleToString(role));
    printf("===========================================================================================================================\n");
    printf("%-6s %-22s %-20s %-14s %-8s %-8s %-8s %-8s %-8s %-10s\n", "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER" ,"PerfIdx");
    printf("===========================================================================================================================\n");

    for (int i = indexCount - 1; i >= 0; i--) {

        PlayerData *player = list[i];

        printf("%-6d %-22s %-20s %-14s %-8d %-8.1f %-8.1f %-8d %-8.1f %-10.2f\n",
            player->playerId, player->name, player->playerTeamName, roleToString(player->role),
            player->totalRuns, player->battingAvg, player->strikeRate,
            player->wickets, player->economyRate, player->performanceIndex);
    }
    
}
