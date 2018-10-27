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
	//int main_cardtest4();
}
// Utilities
#define mRED "\033[31m"
#define mGREEN "\033[32m"
#define mYELLOW "\033[33m"
#define mBLUE "\033[34m"
#define mPURPLE "\033[35m"
#define mCYAN "\033[36m"
#define mWHITE "\033[37m"
#define mNONE "\033[0m"

#ifdef _WIN32
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
void percent(int a, int b) { float j = (float)a / (float)b; printf("%.2f", j); }
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
/// Stubs - Overall Project Requirements///
/// Plan to move to their own files later
int unittest1_c(); // Shuffle
int unittest2_c(); // Buy Card
int unittest3_c(); // end turn
int unittest4_c(); // is game over
int cardtest1_c(); // smithy
int cardtest2_c(); // adventurer
int cardtest3_c(); // cutpurse
int cardtest4_c(); // garden

// Initializes the G to the same initial state for all card tests
void CardTestInitializeGame(gameState * G);
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
//

int playdom_c_main(int argc, const char** argv);

int main(int argc, char ** argv)
{
	const char * args[] = { "0","2" }; // used to simulate args sent to playdom_c_main
//	cout << "Playing Simulated 2 player dominion game.\n";
//	playdom_c_main(2, args);
	//cout << "---Running Sample cardtest4---\n";
	//int outValue = main_cardtest4();
	//cout << "---Results:  " << outValue << " ---\n";
/*	cout << "This is testing " << PASS(true) << endl;
	cout << "This is testing " << PASS(false) << endl;
	cout << "End Test" << endl;
//	return 0;
	*/

	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Unit Test 1                                |" << endl;
	cout << "|________________________________________________________________|" << endl;
	unittest1_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Unit Test 2                                |" << endl;
	cout << "|________________________________________________________________|" << endl;
	unittest2_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Unit Test 3                                |" << endl;
	cout << "|________________________________________________________________|" << endl;
	unittest3_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Unit Test 4                                |" << endl;
	cout << "|________________________________________________________________|" << endl;
	unittest4_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Card Test 1 - Smithy Test                  |" << endl;
	cout << "|________________________________________________________________|" << endl;
	cardtest1_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Card Test 2 - Adventurer                   |" << endl;
	cout << "|________________________________________________________________|" << endl;
	cardtest2_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Card Test 3 - CutPurse                     |" << endl;
	cout << "|________________________________________________________________|" << endl;
	cardtest3_c();
	cout << " ________________________________________________________________ " << endl;
	cout << "|                                                                |" << endl;
	cout << "|                     Card Test 4 - Gardens                      |" << endl;
	cout << "|________________________________________________________________|" << endl;
	cardtest4_c();
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
				while (i < numHandCards(&G)) {
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
				while (i < numHandCards(&G)) {
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
	return (allPass ? 0 : 1);
}

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



	return (allPass ? 0 : 1);
}


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

//

int unittest3_c() {
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
	bool same = true; // isGameStateEqual(&G, &G_original); // test isGameState function

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
	return 0;
}
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
int unittest4_c() {
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	struct gameState G;
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 4;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)

	bool allPass = true;
	bool tResult = true;

	int player_i = whoseTurn(&G);
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
	cout << "T1: isGameOver(gameState):" << PASS(tResult) << " Returns True when Estate,Duchy, & Smithy Supply = 0\n";


	//	4) T4 - Reset smithy != 0 and Curse = 0 expect true to game over
	G.supplyCount[smithy] = 6; // assertion at this point is no empty piles exist becuase of initial setup.
	G.supplyCount[duchy] = 0;
	G.supplyCount[estate] = 0;
	G.supplyCount[curse] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T1: isGameOver(gameState):" << PASS(tResult) << " Returns True when Estate,Duchy, & Curse Supply = 0\n";

	//			5) T5 - Reset Estate, Dutchy != 0 and set 3 kingdom cards to 0 expect true to game over
	G.supplyCount[duchy] = 6;
	G.supplyCount[estate] = 6;
	G.supplyCount[smithy] = 0;
	G.supplyCount[adventurer] = 0;
	G.supplyCount[cutpurse] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T1: isGameOver(gameState):" << PASS(tResult) << " Returns True when smithy, adventurer, cutpurse Supply = 0\n";

	// T6 - Set all 5 Kigdom cards = 0 expect true to game over
	G.supplyCount[gardens] = 0;
	G.supplyCount[village] = 0;
	tResult = (1 == isGameOver(&G));
	allPass = allPass && tResult;
	cout << "T1: isGameOver(gameState):" << PASS(tResult) << " Returns True when smithy, adventurer, cutpurse, gardens, village Supply = 0\n";

	cout << "endTurn(gameState): Unit Tests 4 - all tests:" << PASS(allPass) << endl;
	return allPass ? 0 : 1;
}

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
int cardtest1_c() {
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
	return allPass;
};

// Adventurer Car Unit Test
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
int cardtest2_c() {
	gameState G;
	CardTestInitializeGame(&G); // initial state + 1 of each of the 5 supply cards in the deck
	int player_i = whoseTurn(&G);

	// Add Smithy card to the 5th Hand Position
	G.hand[player_i][G.handCount[player_i]] = smithy;
	G.handCount[player_i]++; // hand should have 6 cards at this point

	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));

	cardEffectAdventurer( &G); // the cardEffect signature is very odd to me... How coins are tracked is odd.
 
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
	G.deck[player_i][G.deckCount[player_i] ] = silver;
	G.deckCount[player_i]++;
	G.deck[player_i][G.deckCount[player_i] ] = gold;
	G.deckCount[player_i]++;
	memcpy(&G_original, &G, sizeof(gameState)); // reset the original state to have the 2 cards
	int failedPermutations = 0;
	int totalPermutations = 0;
	cout << "Perm No: " << "\tCoin Before" << "\tCoin After" <<  "\tValue Increase"
		<< "\tCard Count Same"<<"\tHandCount+2"<<"\tBuys/Actions Same"<< endl; // 7 fields
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
						cardEffectAdventurer(&G); // the cardEffect signature is very odd to me... How coins are tracked is odd.
						// Check Total Card Count
						//		T1-Repeat  Total Count of cards remain the same
						int cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
						int cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
						bool tResult_TotalCardCount = (cardCountBefore == cardCountAfter);
						bool tResult_HandCount_plus2 = (G.handCount[player_i] == G_original.handCount[player_i]);
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
							cout << "" << totalPermutations <<"-";
							cout << ss_Permutation.str();
							cout << "\t\t" << coinCountBefore << "\t\t" << coinCountAfter << "\t";
							PASS(tResult_valueIncrease);
							cout << "\t\t\t";
							PASS(tResult_TotalCardCount);
							cout << "\t\t";
							PASS(tResult_HandCount_plus2);
							cout << "\t\t";
							PASS(tResult_Buys_Actions_same);
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
	return allPass;
};
// cutpurse
// Each Other Player discards a copper or reveals a hand without a copper
//  The other test took so much time to develop this test will just focus on 4 player hand situations instead of looking at every permutation
//
//	The 4 hands will be CGKKK,SCKKK, KKKKK, KSGKK  - (C)opper, (G)old, (S)ilver, (K)ingdom
int cardtest3_c() {
	gameState G;
	// initial state + 1 of each of the 5 supply cards in the deck
	int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };
	// TODO:  initializeGame can return -1 if it doesn't initialize.  No provision for this is accounted for - danKrueger
	int seed = 2;
	int numPlayers = 4;
	initializeGame(numPlayers, k, seed, &G); // call signature -> (numplayers, cardDeck, random seed, gameState Object)
	bool tResult = true;
	bool allPass = true;
	// Setup the 4 player hands
	//	The 4 hands will be CGKKK,SCKKK, KKKKK, KSGKK  - (C)opper, (G)old, (S)ilver, (K)ingdom
	// player 1
	G.hand[0][0] = copper;
	G.hand[0][1] = gold;
	G.hand[0][2] = smithy;
	G.hand[0][3] = smithy;
	G.hand[0][4] = adventurer;
	
	// player 2
	G.hand[1][0] = silver;
	G.hand[1][1] = copper;
	G.hand[1][2] = smithy;
	G.hand[1][3] = adventurer;
	G.hand[1][4] = cutpurse;
	// player 3
	G.hand[2][0] = cutpurse;
	G.hand[2][1] = cutpurse;
	G.hand[2][2] = cutpurse;
	G.hand[2][3] = cutpurse;
	G.hand[2][4] = cutpurse;
	// player 4
	G.hand[3][0] = cutpurse;
	G.hand[3][1] = silver;
	G.hand[3][2] = gold;
	G.hand[3][3] = adventurer;
	G.hand[3][4] = smithy;
	for (int i = 0; i < numPlayers; i++)
		G.handCount[i] = 5;
	G.whoseTurn = 2;
	/// Finish Setup
	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));
	
	cardEffectCutPurse(&G,2);
	{// Player 1
	// Player 1 .. hand reduces by 1 / Discard + 1
		tResult = (G.handCount[0] == (G_original.handCount[0] - 1));
		allPass = allPass && tResult;
		cout << "T1: CutPurse Card Test:" << PASS(tResult) << " Player 1's handCount reduces by 1\n";

		tResult = (G.discardCount[0] == (G_original.discardCount[0] - 1));
		allPass = allPass && tResult;
		cout << "T2: CutPurse Card Test:" << PASS(tResult) << " Player 1's discardCount reduces by 1\n";

		// Total Card Test for Player 1
		tResult = (G.handCount[0] + G.deckCount[0] + G.discardCount[0]) == (G_original.handCount[0] + G_original.deckCount[0] + G_original.discardCount[0]);

		allPass = allPass && tResult;
		cout << "T3: CutPurse Card Test:" << PASS(tResult) << " Player 1's total card count remains the same.\n";
	}
	// Player 2 .. hand reduces by 1 / Discard + 1
	{// Player 2
// Player 2 .. hand reduces by 1 / Discard + 1
		tResult = (G.handCount[1] == (G_original.handCount[1] - 1));
		allPass = allPass && tResult;
		cout << "T4: CutPurse Card Test:" << PASS(tResult) << " Player 2's handCount reduces by 1\n";

		tResult = (G.discardCount[1] == (G_original.discardCount[1] - 1));
		allPass = allPass && tResult;
		cout << "T5: CutPurse Card Test:" << PASS(tResult) << " Player 2's discardCount reduces by 1\n";

		// Total Card Test for Player 2
		tResult = (G.handCount[1] + G.deckCount[1] + G.discardCount[1]) == (G_original.handCount[1] + G_original.deckCount[1] + G_original.discardCount[1]);

		allPass = allPass && tResult;
		cout << "T6: CutPurse Card Test:" << PASS(tResult) << " Player 2's total card count remains the same.\n";
	}
	// Player 3 .. hand stays at 5 /  Discard stays the same
	{// Player 3
// Player 1 .. hand reduces by 1 / Discard + 1
		tResult = (G.handCount[2] == (G_original.handCount[2] -1 ));
		allPass = allPass && tResult;
		cout << "T7: CutPurse Card Test:" << PASS(tResult) << " Player 3's (the attacker) handCount reduces by 1\n";

		tResult = (G.playedCardCount == (G_original.playedCardCount + 1 ));
		allPass = allPass && tResult;
		cout << "T8: CutPurse Card Test:" << PASS(tResult) << " Played Card Count increases by 1\n";

		// Total Card Test for Player 3
		tResult = (G.handCount[2] + G.deckCount[2] + G.discardCount[2] + G.playedCardCount) == (G_original.handCount[2] + G_original.deckCount[2] + G_original.discardCount[2] + G_original.playedCardCount);

		allPass = allPass && tResult;
		cout << "T9: CutPurse Card Test:" << PASS(tResult) << " Player 3 total card count remains the same.\n";
	}
	// Player 4 .. hand stays at 5 / Discard stays the same
	{// Player 2
// Player 1 .. hand reduces by 1 / Discard + 1
		tResult = (G.handCount[3] == (G_original.handCount[3] ));
		allPass = allPass && tResult;
		cout << "T10: CutPurse Card Test:" << PASS(tResult) << " Player 4's handCount remains the same\n";

		tResult = (G.discardCount[3] == (G_original.discardCount[3]));
		allPass = allPass && tResult;
		cout << "T11: CutPurse Card Test:" << PASS(tResult) << " Player 4's discardCount remains the same\n";

		// Total Card Test for Player 4
		tResult = (G.handCount[3] + G.deckCount[3] + G.discardCount[3]) == (G_original.handCount[3] + G_original.deckCount[3] + G_original.discardCount[3]);

		allPass = allPass && tResult;
		cout << "T12: CutPurse Card Test:" << PASS(tResult) << " Player 4's total card count remains the same.\n";
	}

	cout << "Card Test 3 - CutPurse - all tests:" << PASS(allPass) << endl;
	return allPass ? 0 : 1;
	
}
// garden
// Worth 1 VP for every 10 cards you have
//  Note we are testing cardEffectGardens, eventhough cardEffect call does not specifically count the victory points it seemed prudent to check the effect of
//		this card on the score.  This is a slight variation of unit testing principle that feels appropriate in this situation.
//	
//	Testing Strategy  -- playing a card should not have any affect on the game state, card is not playable and thus is not discardable
//	T1 Gamestate unchanged
//	T2-T5 player score calcuated corectly 
int cardtest4_c() { 
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
	for (int p = 0 ; p < numPlayers; p++)
		for (int i = 0; i < 25; i++)
		{
			G.deck[p][G.deckCount[p]++] = smithy; // we are exceeding the smithy card count but its good for this test.
		}
	int totalCards[4] = { 0 };
	for (int p = 0; p < numPlayers; p++) {
		totalCards[p] = G.handCount[p] + G.deckCount[p] + G.discardCount[p]; // no turn has taken place yet
	}
	int player_i = G.whoseTurn;
	bool tResult = true;
	bool allPass = true;
	struct gameState G_original;
	memcpy(&G_original, &G, sizeof(gameState));
	cardEffectGardens(&G);
	
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
	tResult = scores[0] == scoreFor(0,&G);
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
	return allPass ? 0 : 1;
};