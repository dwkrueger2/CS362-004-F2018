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

#include <string.h>
#include "dominion.h"

using namespace std;


#ifdef _WIN32
#include <Windows.h>
#elif __linux__
// #include <ctime> // Not used
#endif // (_WIN32)


// Utilities
#ifdef __linux__  // add tty color codes
#define mRED "\033[31m"
#define mGREEN "\033[32m"
#define mYELLOW "\033[33m"
#define mBLUE "\033[34m"
#define mPURPLE "\033[35m"
#define mCYAN "\033[36m"
#define mWHITE "\033[37m"
#define mNONE "\033[0m"
#endif

string PASS(bool pvalue); // returns color coded PASS or FAIL

void percent(int a, int b); // just prints a 2 decimal point number % to stdout
string getCardName(CARD c);
string getDeckString(int player_i, gameState * state); // returns a string of the current deck example: "{ copper, province, smithy, adventure }"
bool isGameStateEqual(gameState * G1, gameState * G2); // a member by member (byte by byte) comparison of items in the gamestate items.  if they are identical in value then true is returned
#endif // UNITTESTSUPPORT_C