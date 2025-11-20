#ifndef STRUCTURE_H
#define STRUCTURE_H

#define MAX_TEAMS 10
#define MAX_PLAYER_ID 1500
#define MAX_NAME_LENGTH 50
#define MAX_LIST_PLAYERS 500

typedef enum {

    Unknown = 0,
    Batsman = 1,
    Bowler = 2,
    All_Rounder = 3

}RoleCode;

typedef struct PlayerData{

    int playerId;
    char name[MAX_NAME_LENGTH];
    char playerTeamName[MAX_NAME_LENGTH];
    RoleCode role;
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
    float avgBattingStrikeRate;

}TeamData;

#endif