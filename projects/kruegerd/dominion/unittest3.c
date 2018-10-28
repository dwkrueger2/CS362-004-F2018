//	endTurn
// Unit Test 3:  Module: Dominion.c    Function:  endTurn
//    Calling Signature: int buyCard(struct gameState *state);
//  EndTurn  means that all actions are complete and all buys are complete.  Depending on the rules the player could be required to 
//		make a play. We will not test for this since some of the online games do not force an player to play.  For example when 
//		buy are left unused then a player does not have to take a copper which costs 0.
//
//		The end turn expected actions are as follows:
//			1) Remaining cards in hand return to discard pile
//			2) Card content of player the same (all piles combined) before and after end turn)
//			3) Last card in hand discarded (or one of the cards in hand) ends up on top for everyone to see
//			4) Game State is modified as follows:
//					Player set to next player - test for multiple players
//				  coins set to 0
//					actions set to 1
//				  buys set to 1
//			5) the ending player's hand draws 5 cards / decreasing deck / shuffling etc.
//			6) phase should be set to 0 indicating the phase is action when the next player starts
//
//		looking through the code it's discovered that the game uses the updateCoins function 
//			to modify the state.coins to match what is in the Hand.  The state.hand is the set of 
//			cards that are not played but counted during the buy phase.
//  Note when compiling on the school server the default gcc options for the compiler include the following:
//		1) .c files use c copiler use  -x c++ option to force c++
//		2) -lm  links the math library
//		3) g++ links stdlibrary by default but needs explicit linking  https://stackoverflow.com/questions/28236870/undefined-reference-to-stdcout
//				use -lstdc++
//  Valid Compile Command:  gcc -x c++ -lstdc++ -o unitTest1 -g unittest1.c dominion.c rngs.c unitTestSupport.c -lm -std=c++11
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

int unittest3_c() {
//int main() {
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	struct gameState G;
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 4;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)

	bool allPass = true;

	int player_i = whoseTurn(&G);
	// Add 1 of each kingdom cards to player_i's deck
	for (int i = 0; i < NUM_KINGDOM_CARDS; i++)
	{
		G.deck[player_i][G.deckCount[player_i]++] = k[i]; // should post increment  -- Double Check
		G.supplyCount[k[i]]--; // reduce supply piles
	}
	// Set game state on something that we should be able to see change
	G.coins = 2;// not uncomon to have 1/2 coins left unspent
	G.numBuys = 1;// not uncommon to not have enough to make a purchase and refuse a copper (cost =0 for copper)
	/// Take a card from the smithy supply pile and put it in the had to play
	G.supplyCount[smithy]--;
	G.hand[player_i][G.handCount[player_i]] = smithy;
	G.handCount[player_i]++;
	///  Take a card from the smity supply pile and put it in the playedCards 
	G.supplyCount[smithy]--;
	G.playedCards[G.playedCardCount] = smithy;
	G.playedCardCount++;

	// Now at end of Turn there should be 3 smithy cards either in the Deck or in the discard pile

	//////////////////////END GAME STATE SETUP /////////////////
	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));
	//bool same = true; // isGameStateEqual(&G, &G_original); // test isGameState function

	bool tResult = true;
	/////////////////////// START TESTS ///////////////////////
	cout << "Unit Test 3: Module: dominion.c		Function---endTurn--\n";
	cout << "Sample Deck: " << getDeckString(player_i, &G) << endl;
	endTurn(&G);
	// 1) Remaining cards in had return to discard pile
	int expectedValue = G_original.discardCount[player_i] + G_original.handCount[player_i];
	tResult = expectedValue == G.discardCount[player_i];
	allPass = allPass && tResult;
	cout << "T1: endTurn(gameState): " << PASS(tResult) << " Previous Hand Cards (#" << G_original.handCount[player_i]
		<< ") added to discard, previous discard (#" << G_original.discardCount[player_i] << ") result discard pile (#"
		<< G.discardCount[player_i] << ")\n";

	// T2: Content of cards for player 1 remains the same
	{// for folding code
		int allCards1[treasure_map + 1] = { 0 };
		int allCards2[treasure_map + 1] = { 0 };
		// Total Card Counts
		int originalCount = G_original.handCount[player_i]
			+ G_original.deckCount[player_i]
			+ G_original.discardCount[player_i]
			+ G_original.playedCardCount;
		int endTurnCount = G.handCount[player_i]
			+ G.deckCount[player_i]
			+ G.discardCount[player_i]
			+ G.playedCardCount;


		// Count Cards in Hand
		for (int i = 0; i < G_original.handCount[player_i]; i++)
		{
			allCards1[G_original.hand[player_i][i]]++;
		}
		for (int i = 0; i < G.handCount[player_i]; i++)
		{
			allCards2[G.hand[player_i][i]]++;
		}
		// Count Cards in Deck
		for (int i = 0; i < G_original.deckCount[player_i]; i++)
		{
			allCards1[G_original.deck[player_i][i]]++;
		}
		for (int i = 0; i < G.deckCount[player_i]; i++)
		{
			allCards2[G.deck[player_i][i]]++;
		}
		// Count Cards Played
		for (int i = 0; i < G_original.playedCardCount; i++)
		{
			allCards1[G_original.playedCards[i]]++;
		}
		for (int i = 0; i < G.playedCardCount; i++)
		{
			allCards2[G.playedCards[i]]++;
		}
		// Count Cards Discarded
		for (int i = 0; i < G_original.discardCount[player_i]; i++)
		{
			allCards1[G_original.discard[player_i][i]]++;
		}
		for (int i = 0; i < G.discardCount[player_i]; i++)
		{
			allCards2[G.discard[player_i][i]]++;
		}
		// check if content is the smae
		tResult = true;
		for (int i = 0; i < treasure_map + 1; i++)
			tResult = tResult && (allCards1[i] == allCards2[i]);
		allPass = allPass && tResult;
		cout << "T2a: endTurn(gameState):" << PASS(tResult) << " Card Content is exactly the same before and after end turn for player " << player_i << endl;
		tResult = (originalCount == endTurnCount);
		allPass = allPass && tResult;
		cout << "T2b: endTurn(gameState):" << PASS(tResult) << " Total Cards before end turn (# " << originalCount
			<< ") equal count at end of turn (#" << endTurnCount << ") for player: " << player_i << endl;
	}
	// T3: Last Discard card was in hand (because only the last discarded card is shown to all players)
	{ // for folding code
		// TODO: really the cards in the hand/played/discard should be set so that this test is deterministic rather than probabilistic (i.e. make the 5 hand cards unique)
		tResult = false;
		for (int i = 0; i < G_original.handCount[player_i]; i++)
			tResult = tResult || (G_original.hand[player_i][i] == G.discard[player_i][G.discardCount[player_i] - 1]);
		allPass = allPass && tResult;
		cout << "T3: endTurn(gameState):" << PASS(tResult) << " Top Discard pile card was in hand at end of turn\n";
	}
	// T4: Game State is modified as follows:
		//					Player set to next player - test for multiple players
		//				  coins set to 0
		//					actions set to 1
		//				  buys set to 1
	{ // for code folding
		tResult = G.whoseTurn == ((player_i + 1) % numPlayers); //should be the next Player
		allPass = allPass && tResult;
		cout << "T4a: endTurn(gameState):" << PASS(tResult) << " Next Player Correct after endTurn. Current Player: " <<
			player_i << " Next Player: " << G.whoseTurn << endl;
		tResult = (0 == G.coins);
		allPass = allPass && tResult;
		cout << "T4b: endTurn(gameState):" << PASS(tResult) << " Coins are set to 0: state.coins = " << G.coins << endl;
		tResult = (1 == G.numActions);
		allPass = allPass && tResult;
		cout << "T4c: endTurn(gameState):" << PASS(tResult) << " Number of Actions is 1: state.numActions = " << G.numActions << "\n";
		tResult = (1 == G.numBuys);
		allPass = allPass && tResult;
		cout << "T4d: endTurn(gameState):" << PASS(tResult) << " Number of Buys is 1: state.numBuys = " << G.numBuys << "\n";

	}
	//			5) the ending player's hand draws 5 cards / decreasing deck / shuffling etc.
	// The last thing a player does is draw 5 cards. if there are not enough discard cards they are shuffled
	//		we already tested the quantity of cards to be the same.  drawCard takes the card at deckCount -1
	//		position and puts it in the hand 
	//		We are just going to test for 5 cards in the hand and put the Other Tests as TODO items
	//	TODO: reset gameState, make deckCount 3 , test for those 3 cards in hand + 2 more
	{
		tResult = (5 == G.handCount[player_i]);
		allPass = allPass && tResult;
		cout << "T5: endTurn(gameState):" << PASS(tResult) << " Player has 5 cards in hand after endTurn\n";
	}
	//			6) phase should be set to 0 indicating the phase is action when the next player starts
	tResult = (G.phase == 0);
	allPass = allPass && tResult;
	cout << "T6: endTurn(gameState):" << PASS(tResult) << " gameState.phase set to Action(0): state.phase = " << G.phase << endl;

	cout << "endTurn(gameState): Unit Tests 3 - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error
}