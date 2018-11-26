// Adventurer Card Unit Test from Assignment 3
//    Adventurer card Draws cards from the deck until you reveal 2 Treasure Cards
//		Testing Plan -- This card has a bit of integrated testing component to it becuase if there are insufficient
//			treasure cards in the deck then the discard pile will have to be shuffled. There is guarented to be 
//			a solution becuase the initial game state has 7 coppers in it.  (and I couldn't find a scenerio 
//			where there wouldn't be enough Treasure cards in the discard and deck combined)
//		
//		Given this assertion that it is mathmatically impossible (no formal proof here) for there not to be 2
//			Treasure cards in either the Deck or Deck and Discard combined.  We will focus on the state where
//			there are 0,1,2,3 Treasure cards in the deck and the state where the First 2 draws are Treasure and 
//			First 2 draws are not Treasure
//		T1  Total Count of cards remain the same
//		T2  Deck State permutations of k,k,T,T,T   = k means kindom card, T means Treasure Card  will be executed against the test battery
//					for practicality, gold, silver and copper cards will be included but not every permuation of 
//					the 3 different Treasure cards shall be considered.
//				T2a		Total Player Owned Cards Remain the same
//				T2b		handCount increases by 2
//				T2c		Buys and Actions Remain the Same
//				T2d		Coin Count before < Coin Count After
//		

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

#define USE_AS_FUNCTION 0// 0 true 1 false
#if USE_AS_FUNCTION
int cardtest3_c() {
#else
int main() {
#endif
	gameState G;
	CardTestInitializeGame(&G); // initial state + 1 of each of the 5 supply cards in the deck
	int player_i = whoseTurn(&G);

	// Add Adventurer card to the 5th Hand Position
	G.hand[player_i][G.handCount[player_i]] = adventurer;
	G.handCount[player_i]++; // hand should have 6 cards at this point

	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));
	printHeader("Running Unit Test for Adventurer Card from Assignment 3");
	cardEffect(adventurer, 0, 0, 0, &G, 5, 0); // the cardEffect signature is very odd to me... How coins are tracked is odd.

	bool allPass = true;
	bool tResult = true;

	//		T1  Total Count of cards remain the same
	int cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
	int cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
	tResult = (cardCountBefore == cardCountAfter);
	cout << "T1: Adventurer Card Test:" << PASS(tResult) << " Returns true when Total Cards owned by player remains the same.\n";

	allPass = allPass && tResult;

	// T2 with permutations
	// In order to run the permutations we need to make sure there are 2 Treasure cards in the discard pile
	memcpy(&G, &G_original, sizeof(gameState)); // return state to the original state
	G.deck[player_i][G.deckCount[player_i]] = silver;
	G.deckCount[player_i]++;
	G.deck[player_i][G.deckCount[player_i]] = gold;
	G.deckCount[player_i]++;
	memcpy(&G_original, &G, sizeof(gameState)); // reset the original state to have the 2 cards
	//int failedPermutations = 0;
	int totalPermutations = 0;
	cout << "Perm No: " << "\tCoin Before" << "\tCoin After" << "\tValue Increase"
		<< "\t  Card Count Same" << "\t\tHandCount+1" << "\tBuys/Actions Same" << endl; // 7 fields
	for (int c1 = 0; c1 < 2; c1++) { // 1st Card
		CARD card1 = c1 ? smithy : copper;
		for (int c2 = 0; c2 < 2; c2++) { // 1st Card
			CARD card2 = c2 ? smithy : gold;
			for (int c3 = 0; c3 < 2; c3++) { // 1st Card
				CARD card3 = c3 ? smithy : silver;
				for (int c4 = 0; c4 < 2; c4++) { // 1st Card
					CARD card4 = c4 ? smithy : copper;
					for (int c5 = 0; c5 < 2; c5++) { // 1st Card
						CARD card5 = c5 ? smithy : copper;
						// all 32 Permutations of 5 cards covered here
						totalPermutations++;
						G.deck[player_i][0] = card1;
						G.deck[player_i][1] = card2;
						G.deck[player_i][2] = card3;
						G.deck[player_i][3] = card4;
						G.deck[player_i][4] = card5;
						memcpy(&G_original, &G, sizeof(gameState));
						stringstream ss_Permutation;
						for (int j = 0; j < G.deckCount[player_i]; j++)
						{
							switch (G.deck[player_i][j]) {
								case copper:
									ss_Permutation << "T";
									break;
								case silver:
									ss_Permutation << "T";
									break;
								case gold:
									ss_Permutation << "T";
									break;
								default:
									ss_Permutation << "K";
									break;
							}

						}
						cardEffect(adventurer, 0, 0, 0, &G, 0, 0);
						//cardEffectAdventurer(&G); // the cardEffect signature is very odd to me... How coins are tracked is odd.
						// Check Total Card Count
						//		T1-Repeat  Total Count of cards remain the same
						int cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
						int cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
						bool tResult_TotalCardCount = (cardCountBefore == cardCountAfter);
						bool tResult_HandCount_plus2 = (G.handCount[player_i] == G_original.handCount[player_i] +2);
						bool tResult_Buys_Actions_same = (G.numBuys == G_original.numBuys) && (G.numActions == G_original.numActions);
						// Check coint count
						int coinCountBefore = 0;
						int coinCountAfter = 0;
						for (int j = 0; j < G.handCount[player_i]; j++)
						{
							switch (G.hand[player_i][j]) {
								case copper:
									coinCountAfter++;
									break;
								case silver:
									coinCountAfter += 3;
									break;
								case gold:
									coinCountAfter += 6;
									break;
								default:
									break;
							}

						}
						for (int j = 0; j < G_original.handCount[player_i]; j++)
						{
							switch (G_original.hand[player_i][j]) {
								case copper:
									coinCountBefore++;
									break;
								case silver:
									coinCountBefore += 3;
									break;
								case gold:
									coinCountBefore += 6;
									break;
								default:
									break;
							}
						}

						bool tResult_valueIncrease = (coinCountBefore < coinCountAfter);
						allPass = allPass && tResult_valueIncrease &&tResult_TotalCardCount&&tResult_HandCount_plus2
							&&tResult_Buys_Actions_same;
						{ // print results: brack for code folding only
							cout << "" << totalPermutations << "-";
							cout << ss_Permutation.str();
							cout << "\t\t" << coinCountBefore << "\t" << coinCountAfter << "\t\t";
							cout << PASS(tResult_valueIncrease);
							cout << "\t\t\t";
							cout << PASS(tResult_TotalCardCount);
							cout << "\t\t\t";
							cout << PASS(tResult_HandCount_plus2);
							cout << "\t\t";
							cout << PASS(tResult_Buys_Actions_same);
						}
						cout << endl;
						// reset the deck otherwise discards keep piling up
						memcpy(&G, &G_original, sizeof(gameState));
					}
				}
			}
		}
	}
	// TODO: Build same test with required shuffle of Discarded Cards

	cout << "Card Test 2- Adventurer - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error
}