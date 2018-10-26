//#######################################################################
//#			kruegerd-dominion-windows-console.cpp : Defines the entry point for the console application.
//#				
//#			Code written to be compiled in both windows and linux environments
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

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <ctime>
#endif // (_WIN32)
using namespace std;

extern "C++"
{
#include "../../dominion/dominion_helpers.h"
#include "../../dominion/dominion.h"
#include "../../dominion/rngs.h"
	int main_cardtest4();
}
// Utilities
string PASS(bool pvalue) { return (pvalue ? "PASS" : "FAIL"); }
void percent(int a, int b) { double j = (double)a / (double)b; printf("%.2d", j); }
/* Just returns the textual repesentation of the enumeration*/
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
		ss << getCardName((CARD)state->deck[player_i][i]) << " " << (i < (state->deckCount[player_i] -1) ? ",": "");
	ss << " }";
	return ss.str();
}
// a member by member (byte by byte) comparison of items in the gamestate
bool isGameStateEqual(gameState * G1, gameState * G2) {

	bool isSame = true;
	for (int offset = 0; offset < (sizeof(gameState) / sizeof(int)) && isSame; offset++)
	{
		int a = *(((int*)G1)+offset);
		int b = *(((int*)G2) + offset);

		isSame = isSame && (a == b);

		//isSame = isSame && (*(((int *)G1) + offset)) == *((int *)(G2 + offset))); // basically looking at every int size memory piece and seeing if the stuff at that location is identical
		// structures are concurrently ordered in memory so this should work.
	}
	//G2->playedCardCount++;
	return isSame;
}
/// Stubs - Overall Project Requirements///
/// Plan to move to their own files later
int unittest1_c(); // Shuffle
int unittest2_c(); // Buy Card
int unittest3_c();
int unittest4_c();
int cardtest1_c();
int cardtest2_c();
int cardtest3_c();
int cardtest4_c();
//// Write Up Requirements
//  1) Bugs
//  2) Test Coverages Section
//	3) Unit Testing Efforts
//	4) .out File  -- see bst tests.out from the make field

//TODO: 1) Write Unit Tests
//TODO: 2) Write Card Tests
//TODO: 3) Move Code to individual files and write MakeFile

	




/////////// Remember this....  To compile ".c" configureation Propertices--> C/C++ --> advanced --> compile as c++  
///////////    This is a difficult error because the files all compile fine with .c but they don't link well.. So you only get the error during the build 
///////////		 and only when the build determins a link is necessary


int playdom_c_main(int argc, const char** argv);

int main(int argc, char ** argv)
{
		const char * args[] = { "0","2" }; // used to simulate args sent to playdom_c_main
	//	cout << "Playing Simulated 2 player dominion game.\n";
	//	playdom_c_main(2, args);
		//cout << "---Running Sample cardtest4---\n";
		//int outValue = main_cardtest4();
		//cout << "---Results:  " << outValue << " ---\n";
		
		unittest1_c();
		cout << "|________________________________________________________________|" << endl;
		unittest2_c();
		cout << "|________________________________________________________________|" << endl;

		cout << "[Press Enter to Exit]";
		cin.get();
		return 0;
}


// Entire main() coppied from playdom.c
int playdom_c_main(int argc, const char** argv)
{
	struct gameState G;

	// k is the cards passed to the initialize game.  10 was hard coded
	// original 
	/*int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
	sea_hag, tribute, smithy};
	*/

	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	printf("Starting game.\n");

	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	initializeGame(2, k, atoi(argv[1]), &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)

	int money = 0;
	int smithyPos = -1;
	int adventurerPos = -1;
	int i = 0;

	int numSmithies = 0;
	int numAdventurers = 0;

	while (!isGameOver(&G)) {
		money = 0;
		smithyPos = -1;
		adventurerPos = -1;
		for (i = 0; i < numHandCards(&G); i++) {
			if (handCard(i, &G) == copper)
				money++;
			else if (handCard(i, &G) == silver)
				money += 2;
			else if (handCard(i, &G) == gold)
				money += 3;
			else if (handCard(i, &G) == smithy)
				smithyPos = i;
			else if (handCard(i, &G) == adventurer)
				adventurerPos = i;
		}

		if (whoseTurn(&G) == 0) {
			if (smithyPos != -1) {
				printf("0: smithy played from position %d\n", smithyPos);
				playCard(smithyPos, -1, -1, -1, &G);
				printf("smithy played.\n");
				money = 0;
				i = 0;
				while (i<numHandCards(&G)) {
					if (handCard(i, &G) == copper) {
						playCard(i, -1, -1, -1, &G);
						money++;
					}
					else if (handCard(i, &G) == silver) {
						playCard(i, -1, -1, -1, &G);
						money += 2;
					}
					else if (handCard(i, &G) == gold) {
						playCard(i, -1, -1, -1, &G);
						money += 3;
					}
					i++;
				}
			}

			if (money >= 8) {
				printf("0: bought province\n");
				buyCard(province, &G);
			}
			else if (money >= 6) {
				printf("0: bought gold\n");
				buyCard(gold, &G);
			}
			else if ((money >= 4) && (numSmithies < 2)) {
				printf("0: bought smithy\n");
				buyCard(smithy, &G);
				numSmithies++;
			}
			else if (money >= 3) {
				printf("0: bought silver\n");
				buyCard(silver, &G);
			}

			printf("0: end turn\n");
			endTurn(&G);
		}
		else { // other player's turn (not player 0)
			if (adventurerPos != -1) {
				printf("1: adventurer played from position %d\n", adventurerPos);
				playCard(adventurerPos, -1, -1, -1, &G);
				money = 0;
				i = 0;
				while (i<numHandCards(&G)) {
					if (handCard(i, &G) == copper) {
						playCard(i, -1, -1, -1, &G);
						money++;
					}
					else if (handCard(i, &G) == silver) {
						playCard(i, -1, -1, -1, &G);
						money += 2;
					}
					else if (handCard(i, &G) == gold) {
						playCard(i, -1, -1, -1, &G);
						money += 3;
					}
					i++;
				}
			}

			if (money >= 8) {
				printf("1: bought province\n");
				buyCard(province, &G);
			}
			else if ((money >= 6) && (numAdventurers < 2)) {
				printf("1: bought adventurer\n");
				buyCard(adventurer, &G);
				numAdventurers++;
			}
			else if (money >= 6) {
				printf("1: bought gold\n");
				buyCard(gold, &G);
			}
			else if (money >= 3) {
				printf("1: bought silver\n");
				buyCard(silver, &G);
			}
			printf("1: endTurn\n");

			endTurn(&G);
		}
	} // end of While

	printf("Finished game.\n");
	printf("Player 0: %d\nPlayer 1: %d\n", scoreFor(0, &G), scoreFor(1, &G));

	return 0;

}
// stubs

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

int unittest1_c() {
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
			cout << "Suffle No " << j <<": " << getDeckString(player_i, &G)<< endl;
		isSame = true;
		for (int i = 0; i < G.deckCount[player_i]; i++)
			isSame = (deckCopy[i] == G.deck[player_i][i]) && isSame;
		isSame ? isSameCounter++ : 1;

		// copy newly shuffled deck to deckCopy
		for (int i = 0; i < G.deckCount[player_i]; i++)
			deckCopy[i] = G.deck[player_i][i];
	}
	
	bool acceptable = ( ((double)isSameCounter / (double) 50) < (double) 0.05); // acceptable if not the same 95% of the time
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
	cout << "shuffle(player,gameState): " << PASS(acceptable) << " number of cards in deck remains unchanged: " << G.deckCount[player_i]<< endl;
	allPass = allPass && acceptable;
	cout << "shuffle(player,gameState): Unit Tests 1 - all tests." << PASS(acceptable) << endl;
	return (allPass ? 0 :1); 
}

//  buyCard
// Unit Test 1:  Module: Dominion.c    Function:  buyCard
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
int unittest2_c() {
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	struct gameState G;
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 2;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)
	 
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
	bool same = true; // isGameStateEqual(&G, &G_original); // test isGameState function

	cout << "Unit Test 2: Module: dominion.c		Function---buyCard--\n";
	cout << "Sample Deck: " << getDeckString(player_i, &G) << endl;

	G.coins = 10; // give the current player 10 coins
	//Test 1:  Current Player's discard pile increases
	bool tResult = true;
	int discardBeforeCount = G.discardCount[player_i]; 
	int numCardsPurhcased = G.supplyCount[smithy];
	G.coins = 4 * G.supplyCount[smithy];
	G.numBuys = numCardsPurhcased+1;
	for (int i = 0; i < numCardsPurhcased; i++)
		buyCard(smithy, &G);

	int discardAfterCount = G.discardCount[player_i];
	tResult = (discardAfterCount == (discardBeforeCount + numCardsPurhcased));
	allPass = allPass && tResult;
	cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Purchasing all smithy cards increases player discard pile by " <<G.discardCount[player_i] << endl;


	//Test 2: Supply Count reduces by 1
	tResult = G_original.supplyCount[smithy] == G.supplyCount[smithy] + numCardsPurhcased;
	allPass = allPass && tResult;
	cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Purchasing Card Reduces smithy supply level by " << numCardsPurhcased<< endl;


	//Test 3: You should not be allowed to buy a card with none left
	// buy all the smithy (4 coins each) // Try and Buy again
	tResult = (-1 == (buyCard(smithy, &G))  );
	allPass = allPass && tResult;
	cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Smithy Cards Left: " << G.supplyCount[smithy] << " attempt to purchase should fails."  << endl;


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
	cout << "buyCard(supplyPos,gameState): " << PASS(tResult) << " Number of coins left: " << G.coins<< " attempt to purchase province should fail." << endl;
	
	
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
	

	
	return (allPass ? 0: 1); }
//	endTurn
int unittest3_c() { return 1; }
//  isGameOver
int unittest4_c() { return 1; }

// Smith Card Unit Test
int cardtest1_c() { return 1; };
// Adventurer Car Unit Test
int cardtest2_c() { return 1; };
int cardtest3_c() { return 1; };
int cardtest4_c() { return 1; };