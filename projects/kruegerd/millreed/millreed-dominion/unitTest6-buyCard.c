//  buyCard
// Unit Test 2:  Module: Dominion.c    Function:  buyCard
//    Calling Signature: int buyCard(int supplyPos, struct gameState *state);
//  When you buy a card the following state is present:
//		1) The phase is not fully defined in the program but presumably phases are Action, Buy, Cleanup as per the instructions.
//				buy phase should be phase 1  (since there is not an enum for the buy phase)
//		2) the state->numBuys should be set from the Action Phase
//		3) the state->coins should be set from the Action Phase
//	The act of buying should do the following
//		1) Increase the number of cards in the players discard pile 
//		2) supplyCount of the card chosen is reduced by 1
//		3) you should not be permitted to buy a card that has none left
//		4) buyCard should return failure (-1) if no buys are left
//		5) buyCard should return failure if buy is attempted with insufficient coin
//		6) buyCard should return failure if buyCard is called and phase is 0 or 2 (assuming 1 is phase == buy)
//		7) failed attempts should not affect gameState
//	Not to be Test as part of Buy -- (i.e. managed elsewhere or use integrated testing) 
//		8) buying Provincial cards should increase the player score by 1 (but is this testing buyCard or Score??)
//		9) -- buying and running out of cards affects the end of game trigger.  (this is managed in isGameOver() call)
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

#define USE_AS_FUNCTION 0 // 0 true 1 false
#if USE_AS_FUNCTION
int functionTest6_c() {
#else
int main() {
#endif
		//int main() {
		int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
		struct gameState G;
		// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
		int seed = 2;
		int numPlayers = 2;
		initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)
		printHeader("Running Unit Test for buyCard Function from Assignment 3");
		bool allPass = true;

		int player_i = whoseTurn(&G);
		// Add 2 of each kingdom cards to player_i's deck
		for (int i = 0; i < NUM_KINGDOM_CARDS; i++)
		{
			G.deck[player_i][G.deckCount[player_i]++] = k[i]; // should post increment  -- Double Check
			G.deck[player_i][G.deckCount[player_i]++] = k[i];
			G.supplyCount[k[i]]--; // reduce supply piles
			G.supplyCount[k[i]]--; // reduce supply piles
		}

		struct gameState G_original;
		memcpy(&G_original, &G, sizeof(gameState));
		//	bool same = true; // isGameStateEqual(&G, &G_original); // test isGameState function

		cout << "Unit Test 2: Module: dominion.c		Function---buyCard--\n";
		cout << "Sample Deck: " << getDeckString(player_i, &G) << endl;

		G.coins = 10; // give the current player 10 coins
		//Test 1:  Current Player's discard pile increases
		bool tResult = true;
		int discardBeforeCount = G.discardCount[player_i];
		int numCardsPurhcased = G.supplyCount[smithy];
		G.coins = 4 * G.supplyCount[smithy];
		G.numBuys = numCardsPurhcased + 1;
		for (int i = 0; i < numCardsPurhcased; i++)
			buyCard(smithy, &G);

		int discardAfterCount = G.discardCount[player_i];
		tResult = (discardAfterCount == (discardBeforeCount + numCardsPurhcased));
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Purchasing all smithy cards increases player discard pile by " << G.discardCount[player_i] << endl;


		//Test 2: Supply Count reduces by 1
		tResult = G_original.supplyCount[smithy] == G.supplyCount[smithy] + numCardsPurhcased;
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Purchasing Card Reduces smithy supply level by " << numCardsPurhcased << endl;


		//Test 3: You should not be allowed to buy a card with none left
		// buy all the smithy (4 coins each) // Try and Buy again
		tResult = (-1 == (buyCard(smithy, &G)));
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Smithy Cards Left: " << G.supplyCount[smithy] << " attempt to purchase should fails." << endl;


		//Test 4 - Buy should fail if no buys are present
		G.coins = 10;
		buyCard(cutpurse, &G); // should use up the last buy
		memcpy(&G_original, &G, sizeof(gameState)); // <------- For gameState on failure
		tResult = (-1 == ((buyCard(cutpurse, &G))));
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Number of Buys Left: " << G.numBuys << " attempt to purchase should fail." << endl;


		//Test 5) buyCard should return failure if buy is attempted with insufficient coin
		G.coins = 2;
		tResult = (-1 == ((buyCard(province, &G)))); // province card costs 6
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Number of coins left: " << G.coins << " attempt to purchase province should fail." << endl;


		//		6) buyCard should return failure if buyCard is called and phase is 0 or 2 (assuming 1 is phase == buy)
		G.phase = 0;
		G.coins = 7; // need coin to purchase providence card 
		tResult = (-1 == ((buyCard(province, &G))));
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Game Phase:  Action " << " attempt to purchase province should fail for purchase not in Buy Phase of the game." << endl;

		//		7) failed attempts should not affect gameState
		// We modfied G.coins and G.pahse duiring a tests
		G.coins = G_original.coins;
		G.phase = G_original.phase;
		tResult = isGameStateEqual(&G, &G_original);
		allPass = allPass && tResult;
		cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " gameStates before and after attempted buys that fail should equal eachother" << endl;

		cout << "buyCard(supplyPos,gameState): Unit Tests 2 - all tests." << PASS(allPass) << endl;



		//return (allPass ? 0 : 1);
		return 0; // make all stops on error
	}