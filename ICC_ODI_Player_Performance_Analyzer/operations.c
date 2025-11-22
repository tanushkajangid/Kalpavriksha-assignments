#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Players_data.h"
#include "search_sort.h"
#include "function.h"
#include "operations.h"
#include "structure.h"
#include "utils.h"

void showMenu(TeamData *teamArray, TeamData **teamPointerArray){

    int choice = 0;

    while(choice != 6){
        printf("\n==============================================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. exit\n");
        printf("==============================================================================\n");

        printf("Enter your choice: ");
        if(scanf("%d", &choice) != 1 || choice > 6 || choice < 1){
            while(getchar()!='\n'); 
            printf("Invalid choice\n"); 
            continue; 
        }

        while(getchar()!='\n');

        switch(choice){

            case 1 : 
                addPlayerToTeam(teamPointerArray);
                break;
            case 2:
                displayPlayerOfTeam(teamPointerArray);
                break;
            case 3:
                displayTeamByAvgBattingStrikeRate(teamArray);
                break;
            case 4:
                displayTopKPlayers(teamPointerArray);
                break;
            case 5:
                displayAllPlayersByRoleOnPI(teamArray);
                break;
            case 6:
                return;
            default:
                break;
        }
    }
    
}

void initializeTeams(TeamData *teamArray, TeamData **teamPointerArray){
    
    for(int i =0; i< MAX_TEAMS; i++){

        teamArray[i].teamId = i+1;
        strncpy(teamArray[i].teamName, teams[i], MAX_NAME_LENGTH-1);
        teamArray[i].teamName[MAX_NAME_LENGTH - 1] = '\0';
        teamArray[i].head = NULL;
        teamArray[i].totalPlayers = 0;
        teamArray[i].avgBattingStrikeRate = 0.0f;
    }

    for(int i = 0; i < MAX_TEAMS; i++){
        teamPointerArray[i] = &teamArray[i];
    }

    quickSort(teamPointerArray, 0, MAX_TEAMS - 1, sizeof(TeamData*), compareTeamId);
}

void loadInitialPlayers(TeamData *teamArray){

    for(int i= 0; i<playerCount; i++){
        PlayerData *player = malloc(sizeof(PlayerData));

        if (player == NULL) {
            printf("\nMemory allocation failed.\n");
            return ;
        }

        player->playerId = players[i].id;
        strcpy(player->name, players[i].name);
        strcpy(player->playerTeamName, players[i].team);

        player->role = stringToRoleCode(players[i].role);

        player->totalRuns = players[i].totalRuns;
        player->battingAvg = players[i].battingAverage;
        player->strikeRate = players[i].strikeRate;
        player->wickets = players[i].wickets;
        player->economyRate = players[i].economyRate;

        player->performanceIndex = PerformanceIndexEvaluation(player->role, player->battingAvg, player->strikeRate, player->wickets, player->economyRate);
        player->next = NULL;

        int teamOrder = findTeamOrder(player->playerTeamName);

        if(teamOrder == -1){
            printf("Invalid Team Name. Player Not Loaded!\n");
            free(player);
            continue;
        }

        int teamIndex = teamOrder-1;
        insertPlayer(&teamArray[teamIndex], player);
    }
}

void freeAllPlayers(TeamData *teamArray){

    for (int i = 0; i < MAX_TEAMS; i++) {

        PlayerData *currentPlayer = teamArray[i].head;

        while(currentPlayer){

            PlayerData *tempPlayer = currentPlayer;
            currentPlayer = currentPlayer->next;
            free(tempPlayer);
        }

        teamArray[i].head = NULL;
        teamArray[i].totalPlayers = 0;
        teamArray[i].avgBattingStrikeRate = 0.0f;

    }
}
