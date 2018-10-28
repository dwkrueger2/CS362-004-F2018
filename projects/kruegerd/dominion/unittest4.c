//  isGameOver
// Unit Test 4:  Module: Dominion.c    Function:  isGameOver
//    Calling Signature: int buyCard(struct gameState *state);
//		*** Note: the isGameOver returns 0 when false and 1 if true... This is contrary to the use
//			troughout other aspects of the program were 0 = success and nonzero indicates failure.
//			We are going to take this as intential
//  
//		Game ends when the follow criteria are met:
//			1) A player's turn os over
//			and
//				2) All the province cards are purchased
//				or
//				3) any 3 supply piles are empty (coins/estate/duchy/curse cards are all part of the supply)
//
//		Testing Plan
//			1) T1 - Initial state should be false because all supply piles have cards
//			2) T2 - Set Province supply to =0 expect true to game over
//			3) T3 - Reset Province supply to !=0 and Set Estate, Duchy and Smithy supply to 0 expect true to game over
//			4) T4 - Reset smithy != 0 and Curse = 0 expect true to game over
//			5) T5 - Reset Estate, Dutchy != 0 and set 3 kingdom cards to 0 expect true to game over
//		  6) T6 - Set all 5 Kigdom cards = 0 expect true to game over
//			**** Note all permutations are not checked but theoretically could be 5 kingdom cards this leaves 
//				a total of 12 cards, which with the province !=0 leaves only 11 pick 3 combinations or 165 permutations.  but 2048 
//				(2^11) when more than 3 cards can be empty. For the scope if the unit test however, it seems
//				practical to just test the sub groups of cards to make sure they are not overlooked.

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
int unittest4_c() {
//int main() {
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	struct gameState G;
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 4;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)

	bool allPass = true;
	bool tResult = true;

	//int player_i = whoseTurn(&G);
	// set phase to 0 indicating end of turn

	// Note isGameOver returns 0 for false and 1 for true
	// T1 test check if game over with initial setup
	tResult = (0 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T1: isGameOver(gameState):" << PASS(tResult) << " Returns False (0) after initial setup\n";

	// T2 test check if game over if province supply is 0
	G.supplyCount[province] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T2: isGameOver(gameState):" << PASS(tResult) << " Returns True when Province Supply = 0\n";

	// T3 - Reset Province supply to !=0 and Set Estate, Duchy and Smithy supply to 0 expect true to game over
	G.supplyCount[province] = 6;
	G.supplyCount[duchy] = 0;
	G.supplyCount[estate] = 0;
	G.supplyCount[smithy] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T3: isGameOver(gameState):" << PASS(tResult) << " Returns True when Estate,Duchy, & Smithy Supply = 0\n";


	//	4) T4 - Reset smithy != 0 and Curse = 0 expect true to game over
	G.supplyCount[smithy] = 6; // assertion at this point is no empty piles exist becuase of initial setup.
	G.supplyCount[duchy] = 0;
	G.supplyCount[estate] = 0;
	G.supplyCount[curse] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T4: isGameOver(gameState):" << PASS(tResult) << " Returns True when Estate,Duchy, & Curse Supply = 0\n";

	//			5) T5 - Reset Estate, Dutchy != 0 and set 3 kingdom cards to 0 expect true to game over
	G.supplyCount[duchy] = 6;
	G.supplyCount[estate] = 6;
	G.supplyCount[smithy] = 0;
	G.supplyCount[adventurer] = 0;
	G.supplyCount[cutpurse] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T5: isGameOver(gameState):" << PASS(tResult) << " Returns True when smithy, adventurer, cutpurse Supply = 0\n";

	// T6 - Set all 5 Kigdom cards = 0 expect true to game over
	G.supplyCount[gardens] = 0;
	G.supplyCount[village] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T6: isGameOver(gameState):" << PASS(tResult) << " Returns True when smithy, adventurer, cutpurse, gardens, village Supply = 0\n";

	cout << "endTurn(gameState): Unit Tests 4 - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error
}

