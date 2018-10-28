// Smithy Card Unit Test
//    Smithy card takes 3 cards from the deck and places them in the hand.  No other game states should be effected.
//		
//		Testing Plan
//		T1  Hand card count should increase by 3
//		T2  Deck card count should decrease by 3.  (becuase initial state has 5 deck cards)
//		T3  Actions remain the same
//		T4  Coins remain the same
//		T5  Buys remain the same 
//		T6  Played Cards Increases by 1
//		T7  Total Count of cards remain the same

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
int cardtest1_c() {
//int main() {
	gameState G;
	CardTestInitializeGame(&G); // initial state + 1 of each of the 5 supply cards in the deck
	int player_i = whoseTurn(&G);

	// Add Smithy card to the 5th Hand Position
	G.hand[player_i][G.handCount[player_i]] = smithy;
	G.handCount[player_i]++; // hand should have 6 cards at this point

	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));

	cardEffectSmithy(smithy, -1, -1, -1, &G, G.handCount[player_i] - 1, 0); // the cardEffect signature is very odd to me... How coins are tracked is odd.


	// Smithy effect 
	bool allPass = true;
	bool tResult = true;

	//		T1  Hand card count should increase by 3
	tResult = (G.handCount == G_original.handCount + 3);
	allPass = allPass && tResult;
	cout << "T1: Smithy Card Test:" << PASS(tResult) << " Returns true when handCount increases by 3\n";

	//		T2  Deck card count should decrease by 3.  (becuase initial state has 5 deck cards)
	tResult = (G.deckCount == G_original.deckCount - 3);
	allPass = allPass && tResult;
	cout << "T2: Smithy Card Test:" << PASS(tResult) << " Returns true when deckCount decreases by 3\n";

	//		T3  Actions remain the same
	tResult = (G.numActions == G_original.numActions);
	allPass = allPass && tResult;
	cout << "T3: Smithy Card Test:" << PASS(tResult) << " Returns true when numActions remains the same\n";

	//		T4  Coins remain the same
	tResult = (G.coins == G_original.coins);
	allPass = allPass && tResult;
	cout << "T4: Smithy Card Test:" << PASS(tResult) << " Returns true when coins remains the same\n";

	//		T5  Buys remain the same 
	tResult = (G.numBuys == G_original.numBuys);
	allPass = allPass && tResult;
	cout << "T5: Smithy Card Test:" << PASS(tResult) << " Returns true when numBuys remains the same\n";

	//		T6  Played Cards Increases by 1
	tResult = (G.playedCardCount == G_original.playedCardCount + 1);
	allPass = allPass && tResult;
	cout << "T6: Smithy Card Test:" << PASS(tResult) << " Returns true when playedCardCount increases by 1\n";

	//		T7  Total Count of cards remain the same
	int cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
	int cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
	tResult = (cardCountBefore == cardCountAfter);
	allPass = allPass && tResult;
	cout << "T7: Smithy Card Test:" << PASS(tResult) << " Returns true when Total Cards owned by player remains the same.\n";

	cout << "Card Test 1- Smithy - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error
}