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
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <string>

#include "unitTestSupport.h"

using namespace std;
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

#define USE_COLOR 1

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
	#if (USE_COLOR)
		string PASS(bool pvalue) { return (pvalue ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m"); } // note this is a c-preprocessor concationation trick... Not exactly intuitive https://stackoverflow.com/questions/12958925/expand-macros-inside-quoted-string
	#else
		string PASS(bool pvalue) { return (pvalue ? "PASS" : "FAIL"); } // note this is a c-preprocessor concationation trick... Not exactly intuitive https://stackoverflow.com/questions/12958925/expand-macros-inside-quoted-string
	#endif
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
	for (unsigned offset = 0; offset < (sizeof(gameState) / sizeof(int)) && isSame; offset++)
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

void printHeader(string msg)
{ 
	string l1 = " ________________________________________________________________ \n";
	string l2 = "|                                                                |\n";
	string l3 = "|                    XXXXXXXXXXXX                                |\n";
	string l4 = "|________________________________________________________________|\n";
	unsigned strLength = l1.length(); 
	unsigned maxLength = l1.length() - 5;
	unsigned msgLength = (maxLength < msg.length()) ? maxLength : msg.length();
	unsigned isOdd = (msgLength % 2); // = 1 if odd
	unsigned padLeft = strLength / 2 - msgLength / 2 - 2;
	string  middle = "| ";
	for (unsigned i = 0; i < padLeft; i++)
		middle += " ";
	middle = middle +  msg.substr(0,msgLength);
	for (unsigned i = 0; i < padLeft - isOdd; i++)
		middle += " ";
	middle += " |\n";
	l3 = middle;
//	unsigned checkLength = l3.length();
	cout << l1 << l2 << l3 << l4;

}

// Initializes the G to the same initial state for all card tests -- this ended not being used by all tests but some tests use it
// Initializes the G to the same initial state for all card tests
void CardTestInitializeGame(gameState * G)
{
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 2;
	initializeGame(numPlayers, k, seed, G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)

	int player_i = whoseTurn(G);
	// Add 2 of each kingdom cards to player_i's deck
	for (int i = 0; i < NUM_KINGDOM_CARDS; i++)
	{
		G->deck[player_i][G->deckCount[player_i]++] = k[i]; // should post increment  -- Double Check
		G->supplyCount[k[i]]--; // reduce supply piles
	}
}