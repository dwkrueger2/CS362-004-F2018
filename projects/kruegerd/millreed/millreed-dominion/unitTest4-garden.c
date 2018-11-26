// garden
// Worth 1 VP for every 10 cards you have
//  Note we are testing cardEffectGardens, eventhough cardEffect call does not specifically count the victory points it seemed prudent to check the effect of
//		this card on the score.  This is a slight variation of unit testing principle that feels appropriate in this situation.
//	
//	Testing Strategy  -- playing a card should not have any affect on the game state, card is not playable and thus is not discardable
//	T1 Gamestate unchanged
//	T2-T5 player score calcuated corectly 

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

#define USE_AS_FUNCTION 1// 0 true 1 false
#if USE_AS_FUNCTION
int cardtest4_c() {
#else
int main() {
#endif
	printHeader("Running Unit Test for Gardens Card from Assignment 3");
	gameState G;
	// initial state + 1 of each of the 5 supply cards in the deck
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 4;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)
	G.deckCount[0] = 10;
	// Add cards to the players --- well just add them to the deck
	G.deck[0][G.deckCount[0]++] = gardens;
	G.deck[1][G.deckCount[1]++] = gardens;
	G.deck[1][G.deckCount[1]++] = gardens;
	G.deck[2][G.deckCount[2]++] = gardens;
	G.deck[2][G.deckCount[2]++] = gardens;
	G.deck[2][G.deckCount[2]++] = gardens;
	G.deck[3][G.deckCount[3]++] = gardens;
	G.deck[3][G.deckCount[3]++] = gardens;
	G.deck[3][G.deckCount[3]++] = gardens;
	G.deck[3][G.deckCount[3]++] = gardens;
	G.deck[1][G.deckCount[1]++] = estate;
	G.deck[2][G.deckCount[2]++] = duchy;
	G.deck[3][G.deckCount[3]++] = province;
	int numGardens[4] = { 0,3,3,4 };
	for (int p = 0; p < numPlayers; p++)
		for (int i = 0; i < 25; i++)
		{
			G.deck[p][G.deckCount[p]++] = smithy; // we are exceeding the smithy card count but its good for this test.
		}
	//int totalCards[4] = { 0 };
	//for (int p = 0; p < numPlayers; p++) {
	//	totalCards[p] = G.handCount[p] + G.deckCount[p] + G.discardCount[p]; // no turn has taken place yet
	//}
	//int player_i = G.whoseTurn;
	bool tResult = true;
	bool allPass = true;
	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));
	//cardEffectGardens(&G);
	cardEffect(gardens, 0, 0, 0, &G, 0, 0);
	// T1 Check Gamestate
	tResult = isGameStateEqual(&G, &G_original);
	allPass = allPass && tResult;
	cout << "T1: Gardens Card Test:" << PASS(tResult) << " Card unplayable - Gamestate should be unchanged.\n";

	//T2-T5 Check Player's score
	int scores[4] = { 0 };
	int fullDeck[4] = { 0 };
	int scores_from_dominion_c[4] = { 0 };
	for (int p = 0; p < numPlayers; p++)
	{
		fullDeck[p] = G.handCount[p] + G.deckCount[p] + G.discardCount[p];
		scores[p] = fullDeck[p] / 10 * numGardens[p];
		scores_from_dominion_c[p] = scoreFor(p, &G);
	}
	scores[1] += 1;  // for the Estate
	scores[2] += 3; // for the Duchy
	scores[3] += 6; // For the Province
	tResult = scores[0] == scoreFor(0, &G);
	allPass = allPass && tResult;
	cout << "T2: Gardens Card Test:" << PASS(tResult) << " Score for player 1 is " << scores_from_dominion_c[0] << " and should be: 0\n";
	tResult = scores[1] == scoreFor(1, &G);;
	allPass = allPass && tResult;
	cout << "T3: Gardens Card Test:" << PASS(tResult) << " Score for player 2 is " << scores_from_dominion_c[1] << " and should be: 10\n";
	tResult = scores[2] == scoreFor(2, &G);;
	cout << "T4: Gardens Card Test:" << PASS(tResult) << " Score for player 3 is " << scores_from_dominion_c[2] << " and should be: 12\n";
	tResult = scores[3] == scoreFor(3, &G);;
	allPass = allPass && tResult;
	cout << "T5: Gardens Card Test:" << PASS(tResult) << " Score for player 4 is " << scores[3] << " and should be: 18\n";

	cout << "Card Test 4- Gardens - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error
}