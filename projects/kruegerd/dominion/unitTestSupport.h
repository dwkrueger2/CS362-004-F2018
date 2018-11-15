//#######################################################################
//#			Author:		Daniel Krueger
//#			Course:		362 Software Engineering II
//#			Project:	Dominion Card Game Testing - Assignment 3,4
//#
//#			File:			unitTestSupport.h 
//#								This file provides supporting functions that the 
//#								testing programs will rely on.
//#
//#			OS:				Windows and Linux support built in
//#			Dependencies:		dominion.c, rngs.c, 
//#
//#			Functions:
//#######################################################################
#ifndef UNITTESTSUPPORT_C
#define UNITTESTSUPPORT_C


using namespace std;


#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "../../../dominion/dominion.h"
#elif __linux__

#include "dominion.h" 
#endif // (_WIN32)
#include <string.h>




string PASS(bool pvalue); // returns color coded PASS or FAIL

void percent(int a, int b); // just prints a 2 decimal point number % to stdout
string getCardName(CARD c);
string getDeckString(int player_i, gameState * state); // returns a string of the current deck example: "{ copper, province, smithy, adventure }"
string getHandString(int player_i, gameState * state); // same thing just the hand
bool isGameStateEqual(gameState * G1, gameState * G2); // a member by member (byte by byte) comparison of items in the gamestate items.  if they are identical in value then true is returned
void printHeader(string msg); // prints a box with a header 
// Initializes the G to the same initial state for all card tests -- this ended not being used by all tests but some tests use it
void CardTestInitializeGame(gameState * G);
#endif // UNITTESTSUPPORT_C