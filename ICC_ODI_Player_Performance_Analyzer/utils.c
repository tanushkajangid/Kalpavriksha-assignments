# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include "utils.h"
# include "Players_data.h"
# include "structure.h"

extern TeamData teamArray[10];

int stringCaseCompare( const char* string1, const char* string2){

    while(*string1 && *string2){
        int difference = tolower((unsigned char)*string1) - tolower((unsigned char)*string2);
        if(difference != 0){
            return difference;
        }
        string1++;
        string2++;
    }

    return tolower((unsigned char)*string1) - tolower((unsigned char)*string2);
}

const char* roleToString(RoleCode role){
    switch(role){
        case Batsman: return "Batsman";
        case Bowler: return "Bowler";
        case All_Rounder: return "All-rounder";
        default: return "Unknown";
    }
}

RoleCode stringToRoleCode( const char* roleStr){
    
    if(stringCaseCompare(roleStr, "Batsman") == 0){
        return Batsman;
    }
    else if( stringCaseCompare(roleStr, "Bowler") == 0) {
        return Bowler;
    }
    else if(stringCaseCompare(roleStr, "All-rounder") == 0 || stringCaseCompare(roleStr, "All_Rounder") == 0){
        return All_Rounder;
    }
    return Unknown;
}

int compareTeamId( const void *termA, const void *termB){

    const TeamData *term1 = *(const TeamData * const *)termA;
    const TeamData *term2 = *(const TeamData * const*)termB;

    return (term1->teamId - term2->teamId);

}

int compareTeamAverage(const void *termA, const void *termB) {

    const TeamData *term1 = *( TeamData *const*)termA;
    const TeamData *term2 = *( TeamData * const*)termB;

    if (term2->avgBattingStrikeRate > term1->avgBattingStrikeRate){
        return 1;
    }

    if (term2->avgBattingStrikeRate < term1->avgBattingStrikeRate){
        return -1;
    }

    return 0;
}

void printPlayerDetails( PlayerData *player){

    printf("%-6d %-22s %-14s %-8d %-8.1f %-8.1f %-8d %-10.2f %-10.2f\n",
                player->playerId, player->name, roleToString(player->role), player->totalRuns,
                player->battingAvg, player->strikeRate, player->wickets,
                player->economyRate, player->performanceIndex);
}

void printTableHeader(){

    printf("=========================================================================================================\n");
    printf("%-6s %-22s %-14s %-8s %-8s %-8s %-8s %-10s %-10s\n","ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER" , "PerfIdx");
    printf("=========================================================================================================\n");
}

int findTeamOrder(const char *name){

    for(int i = 0; i< MAX_TEAMS; i++){
        if(stringCaseCompare(name, teams[i]) == 0){   
            return i+1;
        }
    }

    return -1;
}

float PerformanceIndexEvaluation(RoleCode role, float battingAvg, float strikeRate, int wickets, float economyRate ){
    
    if(role == Batsman){

        return ((battingAvg * strikeRate) / 100.0);
    }
    else if (role == Bowler){

        return ((wickets * 2.0) + (100.0 - economyRate));
    }
    else{
        return (((battingAvg * strikeRate) / 100.0) + (wickets * 2));
    }
}

void updateAvgStrikeRate(TeamData *team){

    float sum = 0;
    int count = 0;
    PlayerData *player = team->head;

    while(player){
        if(player->role == Batsman || player->role == All_Rounder){
            sum += player->strikeRate;
            count++;
        }
        player = player->next;
    }

    if(count != 0){
        team->avgBattingStrikeRate = sum/count;
    }
    else{
        team->avgBattingStrikeRate = 0;
    }
     
}

void insertPlayer(TeamData *teamArray, PlayerData *node){

    node->next = teamArray->head;
    teamArray->head = node;

    teamArray->totalPlayers++;
    updateAvgStrikeRate(teamArray);

}
