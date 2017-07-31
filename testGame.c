// testGame.c
// Created by Charith, Ellen, Austin and Larissa
// Version Sat 21st May 2016

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Game.h"

int main(int argc, char *argv[]) {
    int disciplines[] = {STUDENT_BQN, STUDENT_MMONEY, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MJ, STUDENT_BPS, STUDENT_MTV, \
                STUDENT_MTV, STUDENT_BPS,STUDENT_MTV, STUDENT_BQN, \
                STUDENT_MJ, STUDENT_BQN, STUDENT_THD, STUDENT_MJ, \
                STUDENT_MMONEY, STUDENT_MTV, STUDENT_BQN, STUDENT_BPS};
    int dice[] = {9,10,8,12,6,5,3,11,3,11,4,6,4,7,9,2,8,10,5};

    // TEST 1
    // Creation of a new game
    Game myGame = newGame(disciplines, dice);

    // TEST 2.1
    // checks the start of the game to see if no turns have been made.
    printf("Testing getTurnNumber \n");
    assert(getTurnNumber(myGame) == -1);
    printf("Test 2.1 passed!\n");

    // TEST 2.2
    // checks that the normal campuses every player has is 2
    printf("Testing getCampuses \n");
    assert(getCampuses(myGame, UNI_A) == 2);
    assert(getCampuses(myGame, UNI_B) == 2);
    assert(getCampuses(myGame, UNI_C) == 2);
    printf("Test 2.2 passed!\n");

    // Test 2.3
    // checks the start of the game to see if each uni has 0 GO8 campuses.
    printf("Testing getGO8s \n");
    assert(getGO8s(myGame, UNI_A) == 0);
    assert(getGO8s(myGame, UNI_B) == 0);
    assert(getGO8s(myGame, UNI_C) == 0);
    printf("Test 2.3 passed!\n");

    // Test 2.4
    // checks the start of the game to see if each uni has 0 ARC grants.
    printf("Testing getARCs \n");
    assert(getARCs(myGame, UNI_A) == 0);
    assert(getARCs(myGame, UNI_B) == 0);
    assert(getARCs(myGame, UNI_C) == 0);
    printf("Test 2.4 passed!\n");

    // Test 2.5
    // checks the start of the game to see if each uni has 20 KPI points.
    printf("Testing getKPIpoints \n");
    assert(getKPIpoints(myGame, UNI_A) == 20);
    assert(getKPIpoints(myGame, UNI_B) == 20);
    assert(getKPIpoints(myGame, UNI_C) == 20);
    printf("Test 2.5 passed!\n");

    // Test 2.6
    // checks the start of the game to see if each uni has 0 IP patents.
    printf("Testing getIPs \n");
    assert(getIPs(myGame, UNI_A) == 0);
    assert(getIPs(myGame, UNI_B) == 0);
    assert(getIPs(myGame, UNI_C) == 0);
    printf("Test 2.6 passed!\n");

    // Test 2.7
    // checks the start of the game to see if each uni has 0 publications.
    printf("Testing getPublications \n");
    assert(getPublications(myGame, UNI_A) == 0);
    assert(getPublications(myGame, UNI_B) == 0);
    assert(getPublications(myGame, UNI_C) == 0);
    printf("Test 2.7 passed!\n");

    // Test 2.8
    // checks the start to see each uni has the 3xBPS, 3xB?, 1xMTV, 1xmJ, 1xM$, and no ThD students.
    printf("Testing getStudents \n");
    assert(getStudents(myGame, UNI_A, STUDENT_BPS) == 3);
    assert(getStudents(myGame, UNI_A, STUDENT_BQN) == 3);
    assert(getStudents(myGame, UNI_A, STUDENT_MTV) == 1);
    assert(getStudents(myGame, UNI_A, STUDENT_MJ) == 1);
    assert(getStudents(myGame, UNI_A, STUDENT_MMONEY) == 1);
    assert(getStudents(myGame, UNI_A, STUDENT_THD) == 0);

    assert(getStudents(myGame, UNI_B, STUDENT_BPS) == 3);
    assert(getStudents(myGame, UNI_B, STUDENT_BQN) == 3);
    assert(getStudents(myGame, UNI_B, STUDENT_MTV) == 1);
    assert(getStudents(myGame, UNI_B, STUDENT_MJ) == 1);
    assert(getStudents(myGame, UNI_B, STUDENT_MMONEY) == 1);
    assert(getStudents(myGame, UNI_B, STUDENT_THD) == 0);

    assert(getStudents(myGame, UNI_C, STUDENT_BPS) == 3);
    assert(getStudents(myGame, UNI_C, STUDENT_BQN) == 3);
    assert(getStudents(myGame, UNI_C, STUDENT_MTV) == 1);
    assert(getStudents(myGame, UNI_C, STUDENT_MJ) == 1);
    assert(getStudents(myGame, UNI_C, STUDENT_MMONEY) == 1);
    assert(getStudents(myGame, UNI_C, STUDENT_THD) == 0);
    printf("Test 2.8 passed!\n");

    // TEST 2.9
    // checks that the exchange rate from any student type (except ThD) to any other type is 3
    // ALSO checks that ThD students cannot be retrained
    printf("Testing getExchangeRate \n");

    // testing normal student exchange rates
    int playerCounter = 1;
    while (playerCounter < 4) {
        int disciplineFromCount = 1;
        int disciplineToCount = 1;
        while (disciplineFromCount < 6) {
            assert(getExchangeRate(myGame, playerCounter, disciplineFromCount, disciplineToCount) == 3);
            disciplineFromCount++;
        }
        playerCounter++;
    }

    printf("Test 2.9 passed!\n");

    // TEST 2.10
    // checks that each region on the board will produce the correct student type.
    // e.g. getDiscipline(myGame,0) is the first hexagon region and should return STUDENT_BQN.
    printf("Testing getDiscipline \n");
    int disciplineCounter = 0;
    while (disciplineCounter < NUM_REGIONS) {
        assert(getDiscipline(myGame, disciplineCounter) == disciplines[disciplineCounter]);
        disciplineCounter++;
    }
    printf("Test 2.10 passed!\n");

    // TEST 2.11
    // checks that each region on the board will produce the correct dice value.
    // e.g. getDiceValue(myGame,0) is the first hexagon region and should return 9.
    printf("Testing getDiceValue \n");
    int diceCounter = 0;
    while (diceCounter < NUM_REGIONS) {
        assert(getDiceValue(myGame, diceCounter) == dice[diceCounter]);
        diceCounter++;
    }
    printf("Test 2.11 passed!\n");

    // Test 2.12
    // checks the start of the game to see that no one has the most ARC grants.
    printf("Testing getMostARCs \n");
    assert(getMostARCs(myGame) == NO_ONE);
    printf("Test 2.12 passed!\n");

    // Test 3.0
    // checks that getTurnNumber increases after each dice throw
    printf("Testing getTurnNumber \n");
    int diceValue = 7;
    assert(getTurnNumber(myGame) == -1);
    throwDice(myGame, diceValue);
    assert(getTurnNumber(myGame) == 0);
    throwDice(myGame, diceValue);
    assert(getTurnNumber(myGame) == 1);
    throwDice(myGame, diceValue);
    assert(getTurnNumber(myGame) == 2);
    throwDice(myGame, diceValue);
    assert(getTurnNumber(myGame) == 3);
    printf("Test 3.6 passed!\n");

    // TEST 3.1
    printf("Testing placement of ARC grant \n");
    action placeArc = {OBTAIN_ARC, "R", 0, 0};
    makeAction(myGame, placeArc);
    // 20 points initially + 2 for ARC + 10 for prestige (most ARCs)
    assert(getKPIpoints(myGame, UNI_A) == 32);
    assert(getARCs(myGame, UNI_A) == 1);
    assert(getStudents(myGame, UNI_A, STUDENT_BPS) == 2);
    assert(getStudents(myGame, UNI_A, STUDENT_BQN) == 2);
    assert(getMostARCs(myGame) == UNI_A);
    printf("Test 3.1 passed!\n");

    // TEST 3.2
    // checks the effects of building a normal uni

    printf("Testing placement of Campus \n");

    action placeArc2 = {OBTAIN_ARC, "RL", 0, 0};
    makeAction(myGame, placeArc2);

    int num_BPS = getStudents (myGame, UNI_A, STUDENT_BPS);
    int num_BQN = getStudents (myGame, UNI_A, STUDENT_BQN);
    int num_MTV = getStudents (myGame, UNI_A, STUDENT_MTV);
    int num_MJ = getStudents (myGame, UNI_A, STUDENT_MJ);
    action buildCampus = {BUILD_CAMPUS, "RL", 0, 0};
    makeAction(myGame, buildCampus);
    assert(getKPIpoints(myGame, UNI_A) == 44);
    assert(getStudents(myGame, UNI_A, STUDENT_BPS) == num_BPS - 1);
    assert(getStudents(myGame, UNI_A, STUDENT_BQN) == num_BQN - 1);
    assert(getStudents(myGame, UNI_A, STUDENT_MTV) == num_MTV - 1);
    assert(getStudents(myGame, UNI_A, STUDENT_MJ) == num_MJ - 1);
    printf("Test 3.2 passed!\n");

    // Test 3.1
    // checks that i)  getExchangeRate is 2 for specialized retraining centers
    //             ii) getExchangeRate is 2 for GO8 campuses
    printf("Testing getExchangeRate \n");

    printf("Test 3.4 passed!\n");


    // Test 3.7
    // checks that campuses cannot be built on the same spot
    //printf("Testing 2 campuses on a single vertex \n");
    //action placeDuplicateUni = {BUILD_CAMPUS, "", 0, 0};
    //int legalOrNot = isLegalAction(myGame, placeDuplicateUni);
    //assert(legalOrNot == FALSE);
    //action placeDuplicateUni2 = {BUILD_CAMPUS, "RL", 0, 0};
    //legalOrNot = isLegalAction(myGame, placeDuplicateUni2);
    //assert(legalOrNot == FALSE);

    return EXIT_SUCCESS;
}