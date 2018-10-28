// Unit Test 1:  Module: Dominion.c    Function:  shuffle
//    Calling Signature: (int player, struct gameState *state);
//	  Function Comments:  /* Assumes all cards are now in deck array (or hand/played):  discard is empty * /
//		Overview of Game Rules and Expectations:  As the player draws cards from his facedown pile, the facedown pile will eventually become depleted. 
//				The discarded cards 
//		Outside Reference for Rules: http://www.ultradominion.com/game-rules.php
//																riograndegames.com/getFile.php?id=348  "Puerto rule us - Rio Grand Games" "Game_278_gameRules.pdf"  
//
//
//		Deck - Players have a deck from which to draw cards
//		        Shuffle occurs 1) at the begining  -- when the player starts with 10 cards, then draws 5  // Starting state is 7 copper and 3 estates
//						Cleanup - all played cards and unplayed cards go back to the discard pile and draw 5 new cards
//						Shuffle occurs only when draw pile reaches 0 - (so player is to draw a partial hand from remaining draw pile before discards are shuffled.
//
//		Expected deck states affected and not affected:
//			The total number of cards in player's hands should not be affected.
//					before shuffled deckCount[player[i] = 0
//			Therefore deckCount[player[i]]	 
//
//  Note when compiling on the school server the default gcc options for the compiler include the following:
//		1) .c files use c copiler use  -x c++ option to force c++
//		2) -lm  links the math library
//		3) g++ links stdlibrary by default but needs explicit linking  https://stackoverflow.com/questions/28236870/undefined-reference-to-stdcout
//				use -lstdc++
//  Valid Compile Command:  gcc -x c++ -lstdc++ -o unitTest1 -g unittest1.c dominion.c rngs.c unitTestSupport.c -lm -std=c++11

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "unitTestSupport.h"

#ifdef _WIN32
//extern "C++"
//{
//#include "../dominion/dominion.h"
#include "dominion_helpers.h"
#include "dominion.h"
#include "rngs.h"

//}
#endif
#ifdef __linux__
#include "dominion_helpers.h"
#include "dominion.h"
#include "rngs.h"
#endif

// testing shuffle
int unittest1_c() {
//	int main ()
	//{

	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	struct gameState G;
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 2;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)


	bool allPass = true;

	/*int numSmithies = 0;
	int numAdventurers = 0;
	int seed = 2;
	int numPlayers = 2;*/
	//int initialized(2, );
	int player_i = whoseTurn(&G);
	// Add 2 of each kingdom cards to player_i's deck
	for (int i = 0; i < NUM_KINGDOM_CARDS; i++)
	{
		G.deck[player_i][G.deckCount[player_i]++] = k[i]; // should post increment  -- Double Check
		G.deck[player_i][G.deckCount[player_i]++] = k[i];
	}
	cout << "Unit Test 1: Module: dominion.c		Function---shuffle--\n";
	cout << "Sample Deck: " << getDeckString(player_i, &G) << endl;
	// Test 1:  Shuffle when deck has cards has no effect.
	// Find array of deck cards
	int deckCopy[MAX_DECK];
	bool isSame = true;
	int isSameCounter = 0;
	bool noError = true;
	int Counter = 0;
	for (int i = 0; i < G.deckCount[player_i]; i++)
		deckCopy[i] = G.deck[player_i][i]; // coppies the current player's deck to deckCopy
	for (int j = 0; j < 50; j++) // with only 5 cards in the deck the probility of randomizing the deck returning the same deck is 50*(1:120)
	{
		Counter++;

		noError = noError && (shuffle(player_i, &G) == 0);
		if (j < 5) // print the first 5 shuffles
			cout << "Suffle No " << j << ": " << getDeckString(player_i, &G) << endl;
		isSame = true;
		for (int i = 0; i < G.deckCount[player_i]; i++)
			isSame = (deckCopy[i] == G.deck[player_i][i]) && isSame;
		isSame ? isSameCounter++ : 1;

		// copy newly shuffled deck to deckCopy
		for (int i = 0; i < G.deckCount[player_i]; i++)
			deckCopy[i] = G.deck[player_i][i];
	}

	bool acceptable = (((double)isSameCounter / (double)50) < (double) 0.05); // acceptable if not the same 95% of the time
	allPass = allPass && acceptable && noError;
	cout << "shuffle(player,gameState): " << PASS(acceptable) << " deck shuffle returned the deck the same < 5% of the time. Result: "; percent(isSameCounter, 50); cout << "%\n";
	cout << "shuffle(player,gameState): " << PASS(noError) << " deck shuffle produced no Errors during " << Counter << " runs of the previous test\n";


	// Test 2: Card Counts did not change
	int cardTypes[treasure_map + 1] = { 0 };
	int cardTypes_after[treasure_map + 1] = { 0 };
	for (int position = 0; position < G.deckCount[player_i]; position++)
	{
		cardTypes[G.deck[player_i][position]]++;
	}
	shuffle(player_i, &G);
	for (int position = 0; position < G.deckCount[player_i]; position++)
	{
		cardTypes_after[G.deck[player_i][position]]++;
	}
	acceptable = true;
	for (int c = 0; c < G.deckCount[player_i]; c++)
	{
		acceptable = acceptable && (cardTypes_after[c] == cardTypes[c]); // count card types before and after shuffle
	}
	cout << "shuffle(player,gameState): " << PASS(acceptable) << " deck has the same constituent of cards.\n";
	allPass = allPass && acceptable;
	// Test 3 = total count of cards is the same
	int numCardsBefore = G.deckCount[player_i];
	shuffle(player_i, &G);
	acceptable = (numCardsBefore == G.deckCount[player_i]);
	cout << "shuffle(player,gameState): " << PASS(acceptable) << " number of cards in deck remains unchanged: " << G.deckCount[player_i] << endl;
	allPass = allPass && acceptable;
	cout << "shuffle(player,gameState): Unit Tests 1 - all tests." << PASS(acceptable) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error
}

