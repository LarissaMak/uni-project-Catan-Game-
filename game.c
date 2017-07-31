/*
 * By Ellen, Charith, Austin and Larissa
 * Tutor: Daniel Phillips, Wed13Kora
 * This program contains the data structure for our game
 * and the functions used within the game
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "Game.h"

#define NUMBER_OF_PLAYERS 3
#define NUM_STUDENT_TYPES 6
#define MAX_COLUMNS 12
#define MAX_ROWS 6

//definitions for getCampus Function


//definitions for getArc function

//take out all these enormous functions and make some static functions!!!!!!


// this type will contain the information that needs to be stored about each university.
typedef struct _universityData {
    int KPIpoints;
    int IPpatents;
    int publications;
    int numberOfCampuses;
    int numberOfGO8s;
    int numberOfARCs;
    int students[NUM_STUDENT_TYPES]; //number of students university has of each type (0-5)
    int exchangeRates[NUM_STUDENT_TYPES]; //exchange rates of each student type (0-5)
} universityData;

typedef struct _nodeData {
    int thisVertex;
    struct _nodeData *vertexHigher;
    struct _nodeData *vertexLower;
    struct _nodeData *vertexEven;
    int arcHigher;
    int arcLower;
    int arcEven;
    char positionFrom;
    int col;
    int row;
} nodeData;

// this type will contain all the information needed about the state of the game Knowledge Island.
typedef struct _game {

    //data that will remain the same throughout the game
    int tileData[NUM_REGIONS];    //stores an array of the student type in each hexagonal tile
    int diceData[NUM_REGIONS];    //stores the dice number on each hexagonal tile

    //data that will change during the game
    universityData uniInfo[NUMBER_OF_PLAYERS];       // {campusA, campusB, campusC}
    nodeData vertexData[MAX_COLUMNS][MAX_ROWS];      // stores content of each vertex and arc (empty or playerID)
    int currentTurn;              //stores the current player turn
} game;

// ------------* Static Function Declarations *------------
static nodeData getThisNode (Game g, path p);
static void setCampus (Game g, path p, int campusType);
static void setARC (Game g, path p, int player);

// ------------* Functions *------------


Game newGame(int discipline[], int dice[]) {
    Game myGame = malloc(sizeof(game));
    assert(myGame != NULL);
    int regionCounter = 0;
    int diceRegionCounter = 0;
    //data that will remain the same throughout the game
    while (regionCounter < NUM_REGIONS) {

        myGame->tileData[regionCounter] = discipline[regionCounter];
        regionCounter++;
    }
    while (diceRegionCounter < NUM_REGIONS) {
        myGame->diceData[diceRegionCounter] = dice[diceRegionCounter];
        diceRegionCounter++;
    }

    //data that will change during the game
    int uniCounter = NO_ONE;
    while (uniCounter < UNI_C) {
        myGame->uniInfo[uniCounter].KPIpoints = 20;
        myGame->uniInfo[uniCounter].IPpatents = 0;
        myGame->uniInfo[uniCounter].publications = 0;
        myGame->uniInfo[uniCounter].numberOfCampuses = 2;
        myGame->uniInfo[uniCounter].numberOfGO8s = 0;
        myGame->uniInfo[uniCounter].numberOfARCs = 0;
        myGame->uniInfo[uniCounter].students[STUDENT_THD] = 0;
        myGame->uniInfo[uniCounter].students[STUDENT_BPS] = 3;
        myGame->uniInfo[uniCounter].students[STUDENT_BQN] = 3;
        myGame->uniInfo[uniCounter].students[STUDENT_MJ] = 1;
        myGame->uniInfo[uniCounter].students[STUDENT_MTV] = 1;
        myGame->uniInfo[uniCounter].students[STUDENT_MMONEY] = 1;
        myGame->uniInfo[uniCounter].exchangeRates[STUDENT_THD] = 3;
        myGame->uniInfo[uniCounter].exchangeRates[STUDENT_BPS] = 3;
        myGame->uniInfo[uniCounter].exchangeRates[STUDENT_BQN] = 3;
        myGame->uniInfo[uniCounter].exchangeRates[STUDENT_MJ] = 3;
        myGame->uniInfo[uniCounter].exchangeRates[STUDENT_MTV] = 3;
        myGame->uniInfo[uniCounter].exchangeRates[STUDENT_MMONEY] = 3;
        uniCounter++;
    }

    //initialise grid data
    int col = 0;
    while(col < MAX_COLUMNS){
        int row = 0;
        while(row < MAX_ROWS){
            myGame -> vertexData[col][row].thisVertex = NO_ONE;
            myGame -> vertexData[col][row].col = col;
            myGame -> vertexData[col][row].row = row;
            myGame -> vertexData[col][row].arcLower = NO_ONE;
            myGame -> vertexData[col][row].arcHigher = NO_ONE;
            myGame -> vertexData[col][row].arcEven = NO_ONE;

            //set up initial campuses
            myGame -> vertexData[5][0].thisVertex = CAMPUS_A;
            myGame -> vertexData[6][5].thisVertex = CAMPUS_A;
            myGame -> vertexData[0][0].thisVertex = CAMPUS_B;
            myGame -> vertexData[11][2].thisVertex = CAMPUS_B; // this may not be working
            myGame -> vertexData[1][3].thisVertex = CAMPUS_C;
            myGame -> vertexData[10][0].thisVertex = CAMPUS_C;

            //initialize null pointers...
            //special cases first - null pointers if it goes to somewhere off the grid!!
            //column 0 and 11, no even connection --> null pointer
            //no higher point on [1][0], [3][0], [5][0],   [6][0], [8][0], [10][0]
            //no lower point on [1][3], [3][4], [5][5],   [6][5], [8][4], [10][3]
            if (col == 0 || col == 11) {
                //NULL pointer for even connection, even ARC non-existent
                myGame -> vertexData[col][row].vertexHigher = &myGame -> vertexData[col-1][row-1];
                myGame -> vertexData[col][row].vertexLower = &myGame -> vertexData[col-1][row];
                myGame -> vertexData[col][row].vertexEven = NULL;
                myGame -> vertexData[col][row].arcEven = -1;
            } else if (row == 0 && (col == 1 || col == 3 || col == 5)){
                //NULL pointer for higher connection, higher ARC non-existent
                myGame -> vertexData[col][row].vertexHigher = NULL;
                myGame -> vertexData[col][row].vertexLower = &myGame -> vertexData[col-1][row];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col+1][row];
            } else if (row == 0 && (col == 6 || col == 8 || col == 10)){
                //NULL pointer for higher connection, higher ARC non-existent
                myGame -> vertexData[col][row].vertexHigher = NULL;
                myGame -> vertexData[col][row].vertexLower = &myGame->vertexData[col+1][row];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col-1][row];
            } else if ((col == 1 && row == 3) || (col == 3 && row == 4) || (col == 5 && row == 5)){
                //NULL pointer for lower connection, lower ARC non-existent
                myGame -> vertexData[col][row].vertexHigher = &myGame -> vertexData[col-1][row-1];
                myGame -> vertexData[col][row].vertexLower = NULL;
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col+1][row];
            } else if ((col == 6 && row == 5) || (col == 8 && row == 4) || (col == 10 && row == 3)) {
                //NULL pointer for lower connection, lower ARC non-existent
                myGame -> vertexData[col][row].vertexHigher = &myGame->vertexData[col+1][row-1];
                myGame -> vertexData[col][row].vertexLower = &myGame->vertexData[col+1][row];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col-1][row];
            }
            //initialize the rest of the pointers:
            //initialize left side of the board:
            else if (col <= 5 && col %2 == 0) {
                //even columns, left side
                myGame -> vertexData[col][row].vertexHigher = &myGame -> vertexData[col+1][row];
                myGame -> vertexData[col][row].vertexLower = &myGame -> vertexData[col+1][row+1];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col-1][row];
            } else if (col <=5) {
                //odd columns, left side
                myGame -> vertexData[col][row].vertexHigher = &myGame -> vertexData[col-1][row-1];
                myGame -> vertexData[col][row].vertexLower = &myGame -> vertexData[col-1][row];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col+1][row];
            }
            //initialise right side of board:
            else if (col >5 && col %2 == 0) {
                //even columns, right side
                myGame -> vertexData[col][row].vertexHigher = &myGame->vertexData[col+1][row-1];
                myGame -> vertexData[col][row].vertexLower = &myGame->vertexData[col+1][row];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col-1][row];
            } else {
                //odd columns, right side
                myGame -> vertexData[col][row].vertexHigher = &myGame->vertexData[col-1][row];
                myGame -> vertexData[col][row].vertexLower = &myGame->vertexData[col-1][row+1];
                myGame -> vertexData[col][row].vertexEven = &myGame -> vertexData[col+1][row];
            }
            row ++;
        }
        col ++;
    }

    myGame->currentTurn = -1;

    return myGame;
}

void disposeGame(Game g) {
    assert(g != NULL);
    //free g
    free(g);
    // null the pointer
    //g -> NULL;
}

// Needs Completing!!!
void makeAction(Game g, action a) {

    int whoseTurn = getWhoseTurn(g);
    if (a.actionCode == PASS) {
        //if (whoseTurn == UNI_A || whoseTurn == UNI_B) {
        //    g->currentTurn = getWhoseTurn(g) + 1;
        //} else {  //current player is UNI_C so next player is UNI_A
        //    g->currentTurn = UNI_A;
        //}
    } else if (a.actionCode == BUILD_CAMPUS) {
        //get paths to get to vertex to build the uni
        setCampus(g, a.destination, whoseTurn);
        g->uniInfo[whoseTurn-1].students[STUDENT_BPS] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_BQN] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_MTV] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_MJ] -= 1;
        g->uniInfo[whoseTurn-1].numberOfCampuses += 1;
        g->uniInfo[whoseTurn-1].KPIpoints += 10;
    } else if (a.actionCode == BUILD_GO8) {
        setCampus(g, a.destination, getWhoseTurn(g) + 3);
        g->uniInfo[whoseTurn-1].students[STUDENT_MJ] -= 2;
        g->uniInfo[whoseTurn-1].students[STUDENT_MMONEY] -= 3;
        g->uniInfo[whoseTurn-1].numberOfGO8s += 1;
        g->uniInfo[whoseTurn-1].KPIpoints += 10;
    } else if (a.actionCode == OBTAIN_ARC) {
        setARC(g, a.destination, getWhoseTurn(g));
        g->uniInfo[whoseTurn-1].students[STUDENT_BPS] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_BQN] -= 1;
        //update KPIs
        int currentMostARCs = getMostARCs(g);
        if (currentMostARCs != whoseTurn) {
            g->uniInfo[whoseTurn-1].numberOfARCs += 1;
            g->uniInfo[whoseTurn-1].KPIpoints += 2;
            if (getMostARCs(g) == whoseTurn) {
                g->uniInfo[whoseTurn-1].KPIpoints += 10;
                if (currentMostARCs != NO_ONE){
                    g->uniInfo[currentMostARCs].KPIpoints -= 10;
                }
            }
        } else {
            g->uniInfo[whoseTurn-1].numberOfARCs += 1;
            g->uniInfo[whoseTurn-1].KPIpoints += 2;
        }
    } else if (a.actionCode == OBTAIN_PUBLICATION) {
        g->uniInfo[whoseTurn-1].students[STUDENT_MJ] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_MTV] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_MMONEY] -= 1;
        g->uniInfo[whoseTurn-1].publications += 1;
    } else if (a.actionCode == OBTAIN_IP_PATENT) {
        g->uniInfo[whoseTurn-1].students[STUDENT_MJ] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_MTV] -= 1;
        g->uniInfo[whoseTurn-1].students[STUDENT_MMONEY] -= 1;
        g->uniInfo[whoseTurn-1].IPpatents += 1;
        g->uniInfo[whoseTurn-1].KPIpoints += 10;
    }else {        //RETRAIN_STUDENTS
        int exchangeRate = getExchangeRate(g, whoseTurn, a.disciplineFrom, a.disciplineTo);
        g -> uniInfo[whoseTurn-1].students[a.disciplineTo] +=1;
        g -> uniInfo[whoseTurn-1].students[a.disciplineFrom] -= exchangeRate;
    }
}

void throwDice(Game g, int diceScore) {
    g->currentTurn += 1;
}


int getDiscipline(Game g, int regionID) {
    int discipline = g->tileData[regionID];
    return discipline;
}

int getDiceValue(Game g, int regionID) {
    int diceValue = g->diceData[regionID];
    return diceValue;
}

int getMostARCs(Game g) {
    int uniWithMaxARCs = NO_ONE;
    int maxARCs = 0;
    int counter = 0;
    while (counter < UNI_C) {
        if (g->uniInfo[counter].numberOfARCs > maxARCs) {
            maxARCs = g->uniInfo[counter].numberOfARCs;
            uniWithMaxARCs = counter +1;
        }
        counter++;
    }
    return uniWithMaxARCs;
}

int getMostPublications(Game g) {
    int uniWithMaxPubs = NO_ONE;
    int maxPubs = 0;
    int counter = 0;
    while (counter < UNI_C) {
        if (g->uniInfo[counter].publications > maxPubs) {
            maxPubs = g->uniInfo[counter].numberOfARCs;
            uniWithMaxPubs = counter +1;
        }
        counter++;
    }
    return uniWithMaxPubs;
}

int getTurnNumber(Game g) {
    return g->currentTurn;
}

int getWhoseTurn(Game g) {
    // UNI_A (0), UNI_B (1), UNI_C (2), UNI_A (3) etc...
    int turnNumber = g->currentTurn;
    int whoseTurn = (turnNumber % 3) +1;
    return whoseTurn;
}

int getCampus(Game g, path pathToVertex) {
    //eg path "LRR"
    nodeData campus;
    campus = getThisNode (g, pathToVertex);
    return campus.thisVertex;
}

int getARC(Game g, path pathToEdge) {
    nodeData campus;
    int thisARC = NO_ONE;
    campus = getThisNode (g, pathToEdge);
    if (campus.positionFrom == 'l') {
        thisARC = campus.arcLower;
    } else if (campus.positionFrom == 'h') {
        thisARC = campus.arcHigher;
    } else {
        thisARC = campus.arcEven;
    }
    return thisARC;
}

int isLegalAction(Game g, action a) {
    //initialise to TRUE, only change if false. This reduces extra 'else' statements
    int legal = TRUE;
    int playerID = getWhoseTurn(g);

    //not valid actions
    if (a.actionCode == OBTAIN_IP_PATENT || a.actionCode == OBTAIN_PUBLICATION) {
        legal = FALSE;
    } else if (a.actionCode == START_SPINOFF) {
        //spinoff cannot be turn -1
        if (g->currentTurn <= -1) {
            legal = FALSE;
        }
        //spinoff requires 1MJ,1MTV,1M$
        if (g->uniInfo[playerID].students[STUDENT_MJ] < 1 &&
            g->uniInfo[playerID].students[STUDENT_MTV] < 1 &&
            g->uniInfo[playerID].students[STUDENT_MMONEY] < 1) {
            legal = FALSE;
        }
    } else if (a.actionCode == BUILD_GO8) {
        if (g->currentTurn <= -1) {
            legal = FALSE;
        }
        //a uni cannot have more than 8 GO8s
        if (getGO8s(g, playerID) >= 8) {
            legal = FALSE;
        }
        //uni must have 2MJ,3M$
        if (g->uniInfo[playerID].students[STUDENT_MJ] < 2 &&
            g->uniInfo[playerID].students[STUDENT_MMONEY] < 3) {
            legal = FALSE;
        }
        //path cannot have nothing in it and also cannot exceed path limit
        if (a.destination[0] == 0 || strlen(a.destination) > PATH_LIMIT) {
            legal = FALSE;
        }
        //check to see path only has letters L AND R AND B. If true than correctString turns true.
        //strlen should not count the null terminator
        int stringCounter = 0;
        int correctString = TRUE;
        while (stringCounter < strlen(a.destination - 1) && correctString == TRUE) {
            if (a.destination[stringCounter] != 'L' && a.destination[stringCounter] != 'R'
                && a.destination[stringCounter] != 'B') {
                correctString = FALSE;
            }
            stringCounter++;
        }
        if (correctString == FALSE) {
            legal = FALSE;
        }
        if (getCampus(g, a.destination) != playerID) { //has to be their own normal campus
            legal = FALSE;
        }
    } else if (a.actionCode == BUILD_CAMPUS) {
        //check adequate resources
        if (g->currentTurn <= -1) {
            legal = FALSE;
        }
        if (g->uniInfo[playerID].students[STUDENT_BPS] < 1 &&
            g->uniInfo[playerID].students[STUDENT_BQN] < 1 &&
            g->uniInfo[playerID].students[STUDENT_MJ] < 1 &&
            g->uniInfo[playerID].students[STUDENT_MTV] < 1) {
            legal = FALSE;
        }
        if (a.destination[0] == 0 || strlen(a.destination) > PATH_LIMIT) {
            legal = FALSE;
        }
        //check to see path only has letters L AND R AND B
        int stringCounter = 0;
        int correctString = TRUE;
        while (correctString == TRUE && stringCounter < strlen(a.destination - 1)) {
            if (a.destination[stringCounter] != 'L' && a.destination[stringCounter] != 'R'
                && a.destination[stringCounter] != 'B') {
                correctString = FALSE;
            }
            stringCounter++;
        }
        if (correctString == FALSE) {
            legal = FALSE;
        }
        if (getCampus(g, a.destination) != VACANT_VERTEX) {
            legal = FALSE;
        }
        //campus has to be on an arc and campus cannot be next to adjacent campus
        path vertexBack = {0};
        path vertexLeft = {0};
        path vertexRight = {0};
        int stringCount = 0;
        while (stringCount < PATH_LIMIT) {
            vertexBack[stringCount] = a.destination[stringCount];
            vertexLeft[stringCount] = a.destination[stringCount];
            vertexRight[stringCount] = a.destination[stringCount];
            if (a.destination[stringCount] == 0) {
                vertexBack[stringCount] = 'B';
                vertexLeft[stringCount] = 'L';
                vertexRight[stringCount] = 'R';
                vertexBack[stringCount + 1] = 0;
                vertexLeft[stringCount + 1] = 0;
                vertexRight[stringCount + 1] = 0;
            }
            stringCount++;
        }
        if (getCampus(g, vertexBack) != VACANT_VERTEX || getCampus(g, vertexLeft) != VACANT_VERTEX
            || getCampus(g, vertexRight) != VACANT_VERTEX) {
            legal = FALSE;
        }
        path arcBehindLeft;
        path arcBehindRight;
        int whoseTurn = getWhoseTurn(g);
        if (getARC(g, vertexLeft) != whoseTurn && getARC(g, vertexRight) != whoseTurn
                && getARC(g, arcBehindLeft) != whoseTurn && getARC(g, arcBehindRight) != whoseTurn){
            legal = FALSE;
        }

        //TESTS TO INCLUDE:arc needs to join up with another arc or campus
    } else if (a.actionCode == OBTAIN_ARC) {
        if (g->currentTurn <= -1) {
            legal = FALSE;
        }
        if (g->uniInfo[playerID].students[STUDENT_BPS] < 1 &&
            g->uniInfo[playerID].students[STUDENT_BQN] < 1) {
            legal = FALSE;
        }
        if (a.destination[0] == 0 || strlen(a.destination - 1) > PATH_LIMIT) {
            legal = FALSE;
        }
        //check to see path only has letters L AND R AND B
        int stringCounter = 0;
        int correctString = TRUE;
        while (correctString == TRUE && stringCounter < strlen(a.destination)) {
            if (a.destination[stringCounter] != 'L' && a.destination[stringCounter] != 'R'
                && a.destination[stringCounter] != 'B') {
                correctString = FALSE;
            }
            stringCounter++;
        }
        if (correctString == FALSE) {
            legal = FALSE;
        }
        if (getARC(g, a.destination) != VACANT_ARC) {
            legal = FALSE;
        }
    } else if (a.actionCode == RETRAIN_STUDENTS) {
        if (g->currentTurn <= -1) {
            legal = FALSE;
        }
        if (a.disciplineFrom > NUM_STUDENT_TYPES || a.disciplineFrom < NUM_STUDENT_TYPES) {
            legal = FALSE;
        }
        if (getStudents(g, playerID, a.disciplineFrom) <
            getExchangeRate(g, playerID, a.disciplineFrom, a.disciplineTo)) {
            legal = FALSE;
        }
    } else if (a.actionCode == PASS) {
        if (g->currentTurn <= -1) {
            legal = FALSE;
        }
    }
    return legal;
}

int getKPIpoints(Game g, int player) {
    int KPIpoints;
    KPIpoints = g->uniInfo[player-1].KPIpoints;
    return KPIpoints;
}

int getARCs(Game g, int player) {
    int ARCs;
    ARCs = g->uniInfo[player-1].numberOfARCs;
    return ARCs;
}

int getGO8s(Game g, int player) {
    int GO8s;
    GO8s = g->uniInfo[player-1].numberOfGO8s;
    return GO8s;
}

int getCampuses(Game g, int player) {
    int campuses;
    campuses = g->uniInfo[player-1].numberOfCampuses;
    return campuses;
}

int getIPs(Game g, int player) {
    int numOfIPs = g->uniInfo[player-1].IPpatents;
    return numOfIPs;
}

int getPublications(Game g, int player) {
    int numOfPublications = g->uniInfo[player-1].publications;
    return numOfPublications;
}

int getStudents(Game g, int player, int discipline) {
    int numOfStudents = g->uniInfo[player-1].students[discipline];
    return numOfStudents;
}

int getExchangeRate(Game g, int player, int disciplineFrom, int disciplineTo) {
    return g->uniInfo[player-1].exchangeRates[disciplineFrom];
}

// ------------* Static Functions *------------

static nodeData getThisNode (Game g, path p) {
    nodeData thisNode;

    size_t pathLength = strlen(p);

    //setup initial position
    int col = 5;
    int row = 0;
    nodeData lastPosition = g -> vertexData[col][row];
    nodeData newPosition = lastPosition;

    //positionFrom: 'h'= highest, 'l' = lowest, 'e' = even
    char positionFrom = 'h';

    //iterate through path string
    int stringCounter = 0;
    while (stringCounter < pathLength){
        col = newPosition.col;
        row = newPosition.row;
        char thisPath = p[stringCounter];

        //if the instruction is 'Back'
        if (thisPath == 'B') {
            newPosition = lastPosition;
            if (positionFrom == 'h') {
                positionFrom = 'l';
            } else if (positionFrom == 'l') {
                positionFrom = 'h';
            }
        }
            //if node in even column
        else if (g -> vertexData[col][row].col % 2 == 0) {
            lastPosition = newPosition;
            if (positionFrom == 'h') {
                //from highest connection, L -> lower, R -> even
                if (thisPath == 'L') {
                    newPosition = *g -> vertexData[col][row].vertexLower;
                    positionFrom = 'h';
                } else if (thisPath == 'R') {
                    newPosition = *g -> vertexData[col][row].vertexEven;
                    positionFrom = 'e';
                }
            } else if (positionFrom == 'l') {
                //from lowest connection, L -> even, R -> higher
                if (thisPath == 'L') {
                    newPosition = *g -> vertexData[col][row].vertexEven;
                    positionFrom = 'e';
                } else if (thisPath == 'R') {
                    newPosition = *g -> vertexData[col][row].vertexHigher;
                    positionFrom = 'l';
                }
            } else {
                //from even connection, L -> higher, R -> lower
                if (thisPath == 'L') {
                    newPosition = *g -> vertexData[col][row].vertexHigher;
                    positionFrom = 'l';
                } else if (thisPath == 'R') {
                    newPosition = *g -> vertexData[col][row].vertexLower;
                    positionFrom = 'h';
                }
            }
        }
            //else, if node in odd column
        else {
            lastPosition = newPosition;
            if (positionFrom == 'h') {
                //from highest connection, L -> even, R -> lower
                if (thisPath == 'L') {
                    newPosition = *g -> vertexData[col][row].vertexEven;
                    positionFrom = 'e';
                } else if (thisPath == 'R') {
                    newPosition = *g -> vertexData[col][row].vertexLower;
                    positionFrom = 'h';
                }
            } else if (positionFrom == 'l') {
                //from lowest connection, L -> higher, R -> even
                if (thisPath == 'L') {
                    newPosition = *g -> vertexData[col][row].vertexHigher;
                    positionFrom = 'l';
                } else if (thisPath == 'R') {
                    newPosition = *g -> vertexData[col][row].vertexEven;
                    positionFrom = 'e';
                }
            } else {
                //from even connection, L -> lower, R -> higher
                if (thisPath == 'L') {
                    newPosition = *g -> vertexData[col][row].vertexLower;
                    positionFrom = 'h';
                } else if (thisPath == 'R') {
                    newPosition = *g -> vertexData[col][row].vertexHigher;
                    positionFrom = 'l';
                }
            }
        }
        stringCounter ++;
    }
    thisNode = newPosition;
    thisNode.positionFrom = positionFrom;
    return thisNode;
}

static void setCampus (Game g, path p, int campusType) {
    nodeData thisCampus;
    thisCampus = getThisNode(g, p);
    g -> vertexData[thisCampus.col][thisCampus.row].thisVertex = campusType;
    if (campusType <=3) {
        g->uniInfo[campusType-1].numberOfCampuses += 1;
    } else {
        g->uniInfo[campusType-4].numberOfGO8s += 1;
        g->uniInfo[campusType-1].numberOfCampuses -= 1;
    }
}

static void setARC (Game g, path p, int player) {
    nodeData thisCampus;
    nodeData lastCampus;
    thisCampus = getThisNode(g, p);
    if (thisCampus.positionFrom == 'l') {
        g -> vertexData[thisCampus.col][thisCampus.row].arcLower = player;
        lastCampus = *thisCampus.vertexLower;
        g -> vertexData[lastCampus.col][lastCampus.row].arcHigher = player;
    } else if (thisCampus.positionFrom == 'h') {
        g -> vertexData[thisCampus.col][thisCampus.row].arcHigher = player;
        lastCampus = *thisCampus.vertexHigher;
        g -> vertexData[lastCampus.col][lastCampus.row].arcLower = player;
    } else {
        g -> vertexData[thisCampus.col][thisCampus.row].arcEven = player;
        lastCampus = *thisCampus.vertexEven;
        g -> vertexData[lastCampus.col][lastCampus.row].arcEven = player;
    }
}
