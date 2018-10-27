//#######################################################################
//#			Author:		Daniel Krueger
//#			Course:		362 Software Engineering II
//#			Project:	Dominion Card Game Testing - Assignment 3,4
//#
//#			File:			unitTestSupport.c - Implementation
//#								This file provides supporting functions that the 
//#								testing programs will rely on.
//#
//#			OS:				Windows and Linux support built in
//#			Dependencies:		dominion.c, rngs.c, 
//#
//#			Functions:
//#######################################################################



#include <iostream>
#include <iomanip>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <time.h>
#include <ctime>
#include <climits>

#include "unitTestSupport.h"

using namespace std;




#ifdef _WIN32  // Windows and Linux require different implementations for color coding text on the console screen
string PASS(bool pvalue) {
	HANDLE  hConsole;
	_CONSOLE_SCREEN_BUFFER_INFO  currentConsoleState;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &currentConsoleState);
	cout.flush();
	// Print sample colors
	//for (int i = 0; i < 32; i++) {
	//	SetConsoleTextAttribute(hConsole, i); // Fail is 12-red on black // Succes is 10 - Green on Black
	//	cout << "Color Number: " << i << endl;
	//}
	if (pvalue)
	{
		SetConsoleTextAttribute(hConsole, 10); // Fail is 12-red on black // Succes is 10 - Green on Black
		cout << "PASS ";
	}
	else {
		SetConsoleTextAttribute(hConsole, 12); // Fail is 12-red on black // Succes is 10 - Green on Black
		cout << "FAIL ";
	}
	// return console text to previous color
	SetConsoleTextAttribute(hConsole, currentConsoleState.wAttributes); // return display state
	cout.flush();
	return ""; // already had code written to recieve a string and it kinda still works well (pvalue ? "PASS" : "FAIL");
}
#elif __linux__
string PASS(bool pvalue) { return (pvalue ? mGREEN"PASS"mNONE : mRED"FAIL"mNONE); } // note this is a c-preprocessor concationation trick... Not exactly intuitive https://stackoverflow.com/questions/12958925/expand-macros-inside-quoted-string
#endif



void percent(int a, int b) { float j = (float)a / (float)b; printf("%.2f", j); } // just prins a 2 decimaled % value of a/b

/* Just returns the textual repesentation of the enumeration CARD*/
string getCardName(CARD c) {
	switch (c) {
		case curse:
			return "curse"; break;
		case estate:
			return "estate"; break;
		case duchy:
			return "duchy"; break;
		case province:
			return "province"; break;
		case copper:
			return "copper"; break;
		case silver:
			return "silver"; break;
		case gold:
			return "gold"; break;
			/* NOTE:  the cards from adventure to treasure_map are all action cards.  By putting them in oder here then they can be accessed by their numerical (enumerated) value. */
		case adventurer:
			return "adventurer"; break; /* Adventure is an action card, "Reveal cards from your deck until you reveal 2 treasure cards.  Put those Treasure cards into your hadn and discard the other revealed cards."*/
	/* If no/only 1 treasure found, stop when full deck seen */
		case council_room:
			return "council_room"; break;
		case feast:
			return "feast"; break; /* choice1 is supply # of card gained) */
		case gardens:
			return "gardens"; break;
		case mine:
			return "mine"; break; /* choice1 is hand# of money to trash, choice2 is supply# of money to put in hand */
		case remodel:
			return "remodel"; break; /* choice1 is hand# of card to remodel, choice2 is supply# */
		case smithy:
			return "smithy"; break; /* Smithy is an Action card, +3 Cards*/
		case village:
			return "village"; break;
		case baron:
			return "baron"; break; /* choice1: boolean for discard of estate */
	/* Discard is always of first (lowest index) estate */
		case great_hall:
			return "great_hall"; break; /*Action-Victory, +1 Card, +1 Action (and is worth 1 Victory Point" */
		case minion:
			return "minion"; break; /* choice1:  1 = +2 coin, 2 = redraw */
		case steward:
			return "steward"; break; /* choice1: 1 = +2 card, 2 = +2 coin, 3 = trash 2 (choice2,3) */
		case tribute:
			return "tribute"; break; /*Tribute, Action - The player to your left reveals then discars the top 2 cards of his deck. For each differently named card revealed, if it san Action + 2 Actions, Treasure Card + 2 Gold, Victory Card + 2 Cards."*/
		case ambassador:
			return "ambassador"; break; /* choice1 = hand#, choice2 = number to return to supply */
		case cutpurse:  /* Cutpurse, Action-Attack, + 2 Gold, Each other player discards a Copper ( or reveals a hand with no Copper)."*/
			return "catpurse"; break;
		case embargo: /* choice1 = supply# */ /*Embargo, action, +2 gold, "Trash this. Add an Embargo token to a Supply pile (For the rest of the game, when a player buys a card from that pile, they gain a Curse."*/
			return "embargo"; break;
		case outpost:
			return "outpost"; break;
		case salvager:
			return "salvager"; break; /* choice1 = hand# to trash */
		case sea_hag:
			return "sea_hag"; break;
		case treasure_map:
			return "treasure_map"; break;
		default:
			return "error";
	}
}

/*Returns a textual representation of the current deck (the draw pile)*/
string getDeckString(int player_i, gameState * state)
{
	stringstream ss;
	ss << "{ ";
	for (int i = 0; i < state->deckCount[player_i]; i++)
		ss << getCardName((CARD)state->deck[player_i][i]) << (i < (state->deckCount[player_i] - 1) ? ", " : "");
	ss << " }";
	return ss.str();
}
// a member by member (byte by byte) comparison of items in the gamestate
bool isGameStateEqual(gameState * G1, gameState * G2) {

	bool isSame = true;
	for (int offset = 0; offset < (sizeof(gameState) / sizeof(int)) && isSame; offset++)
	{
		int a = *(((int*)G1) + offset);
		int b = *(((int*)G2) + offset);

		isSame = isSame && (a == b);

		//isSame = isSame && (*(((int *)G1) + offset)) == *((int *)(G2 + offset))); // basically looking at every int size memory piece and seeing if the stuff at that location is identical
		// structures are concurrently ordered in memory so this should work.
	}
	//G2->playedCardCount++;
	return isSame;
}
