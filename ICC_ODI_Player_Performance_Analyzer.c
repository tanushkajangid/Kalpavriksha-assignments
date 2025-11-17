#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

#define MAX_TEAMS 10
#define MAX_PLAYER_ID 1500
#define MAX_NAME_LENGTH 50

const char *teams[MAX_TEAMS] = {
    "Afghanistan",
    "Australia",
    "Bangladesh",
    "England",
    "India",
    "New Zealand",
    "Pakistan",
    "South Africa",
    "Sri Lanka",
    "West Indies"
};

typedef struct PlayerData{

    int playerId;
    char name[MAX_NAME_LENGTH];
    char playerTeamName[MAX_NAME_LENGTH];
    char role[MAX_NAME_LENGTH];
    int totalRuns;
    float battingAvg;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;

    struct PlayerData *next;
 
}PlayerData;

typedef struct TeamData{

    int teamId;
    char teamName[MAX_NAME_LENGTH];
    PlayerData *head;
    int totalPlayers;
    float  avgBattingStrikeRate;  

}TeamData;

float PerformanceIndexEvaluation(char *role, float battingAvg, float strikeRate, int wickets, float economyRate ){
    
    if(strcmp(role, "Batsman") == 0){

        return ((battingAvg * strikeRate) / 100.0);
    }
    else if (strcmp(role, "Bowler") == 0){

        return ((wickets * 2.0) + (100.0 - economyRate));
    }
    else{
        return (((battingAvg * strikeRate) / 100.0) + (wickets * 2));
    }
}

void codeToRole(int code, char *codeRole){
    if(code == 1) {
        strcpy(codeRole, "Batsman");
    }
    else if( code == 2) {
        strcpy(codeRole, "Bowler");
    }
    else{
        strcpy(codeRole, "All-rounder");
    }
}

void updateAvgStrikeRate(TeamData *team){

    float sum = 0;
    int count = 0;
    PlayerData *player = team->head;

    while(player){
        if(strcmp (player->role, "Batsman") == 0 || strcmp(player->role, "All-rounder") == 0){
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

void insertPlayer(TeamData *teamArray, PlayerData * node){

    node->next = teamArray->head;
    teamArray->head = node;
    teamArray->totalPlayers++;
    updateAvgStrikeRate(teamArray);

}

int compareTeamId( const void *termA, const void *termB){

    TeamData *term1 = *(TeamData **)termA;
    TeamData *term2 = *(TeamData **)termB;

    return (term1->teamId - term2->teamId);

}

int compareTeamAverage(const void *termA, const void *termB) {

    TeamData *term1 = *(TeamData **)termA;
    TeamData *term2 = *(TeamData **)termB;

    if (term2->avgBattingStrikeRate > term1->avgBattingStrikeRate){
        return 1;
    }

    if (term2->avgBattingStrikeRate < term1->avgBattingStrikeRate){
        return -1;
    }

    return 0;
}

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
    scanf("%d", &roleCode);
    codeToRole(roleCode, newPlayer->role);

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
    printf("=========================================================================================================\n");
    printf("%-6s %-22s %-14s %-8s %-8s %-8s %-8s %-10s %-10s\n","ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER" , "PerfIdx");
    printf("=========================================================================================================\n");

    PlayerData *player = team->head;
    while(player) {
        printf("%-6d %-22s %-14s %-8d %-8.1f %-8.1f %-8d %-10.2f %-10.2f\n",
               player->playerId, player->name, player->role, player->totalRuns,
               player->battingAvg, player->strikeRate, player->wickets,
               player->economyRate, player->performanceIndex);
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

    qsort(tempArray, 10, sizeof(TeamData*), compareTeamAverage);

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
    scanf("%d", &roleCode);

    char role[MAX_NAME_LENGTH];
    codeToRole(roleCode, role);

    printf("Enter number of players: ");
    scanf("%d", &kPlayerCount);

    PlayerData *player = team->head;
    PlayerData *tempArray[team->totalPlayers];

    int indexCount = 0;

    while(player){

        if (strcmp(player->role, role) == 0){

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

    printf("Top %d %s of Team %s:\n", kPlayerCount, role, team->teamName);
    printf("=========================================================================================================\n");
    printf("%-6s %-22s %-14s %-8s %-8s %-8s %-8s %-10s %-10s\n","ID", "Name", "Role", "Runs", "Avg", "SR", "Wkts", "ER" , "PerfIdx");
    printf("=========================================================================================================\n");

    for (int i = 0; i < kPlayerCount; i++) {

        PlayerData *topPlayer = tempArray[0];

        printf("%-6d %-22s %-14s %-8d %-8.1f %-8.1f %-8d %-10.2f %-10.2f\n",
               topPlayer->playerId, topPlayer->name, topPlayer->role, topPlayer->totalRuns,
               topPlayer->battingAvg, topPlayer->strikeRate, topPlayer->wickets,
               topPlayer->economyRate, topPlayer->performanceIndex);
        
        tempArray[0] = tempArray[indexCount - 1 - i];

        heapify(tempArray, indexCount - 1 - i, 0);
    }
}


void displayAllPlayersByRoleOnPI(TeamData * teamArray){
    
    int roleCode;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &roleCode);

    char role[20];
    codeToRole(roleCode, role);

    PlayerData *list[500];
    int indexCount = 0;

    for (int i = 0; i < MAX_TEAMS; i++) {
        PlayerData *player = teamArray[i].head;

        while(player) {
            if(strcmp(player->role, role) == 0) {
                list[indexCount++] = player;
            }
            player = player->next;
        }
    }

    if (indexCount == 0) {
        printf("No players of role %s found.\n", role);
        return;
    }

    heapSort(list, indexCount);

    printf("%s of all teams (sorted by Performance Index):\n", role);
    printf("=========================================================================================================\n");
    printf("%-6s %-22s %-20s %-14s %-8s %-8s %-8s %-8s %-8s %-10s\n", "ID", "Name", "Team", "Role", "Runs", "Avg", "SR", "Wkts", "ER" ,"PerfIdx");
    printf("=========================================================================================================\n");

    for (int i = indexCount - 1; i >= 0; i--) {

        PlayerData *player = list[i];

        printf("%-6d %-22s %-20s %-14s %-8d %-8.1f %-8.1f %-8d %-8.1f %-10.2f\n",
               player->playerId, player->name, player->playerTeamName, player->role,
               player->totalRuns, player->battingAvg, player->strikeRate,
               player->wickets, player->economyRate, player->performanceIndex);
    }
}

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

int findTeamOrder(const char *name){

    for(int i = 0; i< MAX_TEAMS; i++){
        if(strcmp(name, teams[i]) == 0){
            return i+1;
        }
    }

    return -1;
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

    qsort(teamPointerArray, MAX_TEAMS, sizeof(TeamData*), compareTeamId);
}

void loadInitialPlayers(TeamData *teamArray){

    for(int i= 0; i<playerCount; i++){
        PlayerData *player = malloc(sizeof(PlayerData));

        player->playerId = players[i].id;
        strcpy(player->name, players[i].name);
        strcpy(player->playerTeamName, players[i].team);
        strcpy(player->role, players[i].role);
        player->totalRuns = players[i].totalRuns;
        player->battingAvg = players[i].battingAverage;
        player->strikeRate = players[i].strikeRate;
        player->wickets = players[i].wickets;
        player->economyRate = players[i].economyRate;

        player->performanceIndex = PerformanceIndexEvaluation(player->role, player->battingAvg, player->strikeRate, player->wickets, player->economyRate);
        player->next = NULL;

        int teamOrder = findTeamOrder(player->playerTeamName);
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

int main(){

    TeamData teamArray[10];

    TeamData *teamPointerArray[MAX_TEAMS];

    initializeTeams(teamArray, teamPointerArray);
    loadInitialPlayers(teamArray);

    showMenu(teamArray, teamPointerArray);

    freeAllPlayers(teamArray);
    return 0;
}
