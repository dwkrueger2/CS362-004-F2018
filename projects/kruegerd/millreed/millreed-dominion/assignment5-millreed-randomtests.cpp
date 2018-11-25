//#######################################################################
//#			Author:		Daniel Krueger
//#			Course:		362 Software Engineering II
//#			Project:	Dominion Card Game Testing - Assignment 5
//#
//#			Purpose:	This file provides a combined file for executing all the
//#								tests against the card functions modified with buggs by 
//#								Reed Miller.
//#
//#			File:			assignment5-millreed-randomtests.cpp - Implementation
//#								This file provides supporting functions that the 
//#								testing programs will rely on.  
//#
//#			OS:				Windows and Linux supported 
//#			Dependencies:		dominion.c, rngs.c, 
//#
//#			Functions:			Inspection of Reed Miller's dommion.c revealed the
//#											following functions were modified to respond to the original cardeffect swith statement.
//#
//# 										adventurer_card(state, currentPlayer, &drawntreasure, &z, &nextPlayer);
//#											smithy_card(currentPlayer, state, handPos);
//#											council_room_card(currentPlayer, state, handPos);
//#											mine_card(state, currentPlayer, choice1, choice2, handPos);
//#											minion_card(state, handPos, currentPlayer, choice1, choice2);
//#
//#######################################################################

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <ctime>
#include "unitTestSupport.h"



#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
//extern "C++"
//{

//}
#endif
#ifdef __linux__
#include "dominion_helpers.h"
#include "dominion.h"
#include "rngs.h"
#endif
#include <iostream>
using namespace std;




// Smithy is Random Test Card 1
int * SmithyCardTest(gameState * beforeTest, gameState * afterTest, bool verbose); // returns array [passcount, failcount]
// gardens is Random Test Card 2
int * CutPurseCardTest(gameState * beforeTest, gameState * afterTest, bool verbose);// returns array [passcount, failcount]
int * AdventurerCardTest(gameState * beforeTest, gameState * afterTest, bool verbose);// returns array [passcount, failcount]
int gameStateRandomizer(gameState * G, int challengCard);

#define NUM_TESTS_TO_RUN 5000
#define VERBOSE true
#define EXAMPLE_TEST_COUNT 5
//#define RUN_SMITHY
#define RUN_CUTPURSE
//#define RUN_ADVENTURE

#if defined (RUN_SMITHY)
#define PRINTHEADER printHeader("Running Random Card Test 1 - Smithy");
#define RANDOMIZER gameStateRandomizer(&G,smithy);
#define CARD_EFFECT cardEffectSmithy(&G, cardPosition);
#define RUNNINGTEST SmithyCardTest(&G_original, &G, verbose);
#elif defined(RUN_CUTPURSE)
#define PRINTHEADER printHeader("Running Random Card Test 2 - Cutpurse");	
#define RANDOMIZER gameStateRandomizer(&G,cutpurse);
//#define CARD_EFFECT cardEffectCutPurse(&G, cardPosition);
#define CARD_EFFECT   cardEffect(cutpurse, 0, 0, 0, &G, 0, 0); // original signuture for call to cut purse
#define RUNNINGTEST CutPurseCardTest(&G_original, &G, verbose);
#elif defined(RUN_ADVENTURE)
#define PRINTHEADER printHeader("Running Random Card Test 3 - Adventurer");  if (VERBOSE) cout <<  "\tCoin Before" << "\tCoin After" << "\tValue Increase" << "\t  Card Count Same" << "\t\tHandCount+2" << "\tBuys/Actions Same" << endl;
#define RANDOMIZER gameStateRandomizer(&G,adventurer);
#define CARD_EFFECT cardEffectAdventurer(&G);
#define RUNNINGTEST AdventurerCardTest(&G_original, &G, verbose);
#endif
#define PRINT_TAIL_HEADER printHeader("CodeCoverage Information to Follow");
int main()
{
	srand(time(0)); //rngs.c was just not very clear on why and how it is to be used.
	/*for (int i = 0; i < 50; i++)
		cout << "\ti: " << floor(rand()%100);
	return 1;
	*/
	int masterCounter[2] = { 0 };
	int * results;
	int pass = 0, fail = 1;
	bool verbose = VERBOSE;
	gameState G;
	struct gameState G_original;
	int cardPosition = 0; // the card poisiton the challeng card will be played from 
	PRINTHEADER
		cout << "Running " << NUM_TESTS_TO_RUN << " tests.  Showing only " << EXAMPLE_TEST_COUNT << " of " << NUM_TESTS_TO_RUN << endl;
	for (int i = 0; i < NUM_TESTS_TO_RUN; i++)
	{

		if (i > EXAMPLE_TEST_COUNT)
			verbose = false;

		CardTestInitializeGame(&G); // initial state + 1 of each of the 5 supply cards in the deck
		cardPosition = RANDOMIZER; // puts challenge card in cardPosition and makes random deck
		// testing the rand generator		//cout <<"smithy pos: " << cardPosition<<":" <<  getHandString(0, &G)<< endl;
		memcpy(&G_original, &G, sizeof(gameState));
		CARD_EFFECT

			//results = SmithyCardTest(&G_original, &G, true);
			results = RUNNINGTEST;
		masterCounter[pass] += results[pass];
		masterCounter[fail] += results[fail];
		delete(results);

	}



	// Add Smithy card to the 5th Hand Position
	//G.hand[player_i][G.handCount[player_i]] = smithy;
	//G.handCount[player_i]++; // hand should have 6 cards at this point
	cout << "Tests Run: " << (masterCounter[0] + masterCounter[1]) << endl;
	cout << "pass: " << masterCounter[0] << " fail: " << masterCounter[1] << endl;

	//
	cout << "--End of Tests--" << endl;
	PRINT_TAIL_HEADER
}



//
int * SmithyCardTest(gameState * beforeTest, gameState * afterTest, bool verbose)
{

	// Smithy effect 
	//int counter[2]; // counter[0] = pass count, counter[1] = fail count;
	int * counter = new int[2]; // counter[0] = pass count, counter[1] = fail count;
	counter[0] = 0;
	counter[1] = 0;
	int pass = 0, fail = 1;
	bool allPass = true;
	bool tResult = true;
	gameState G = *afterTest;
	gameState G_original = *beforeTest;
	int player_i = whoseTurn(&G);

	//		T1  Hand card count should increase by 3
	tResult = (G.handCount == G_original.handCount + 3);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T1: Smithy Card Test:" << PASS(tResult) << " Returns true when handCount increases by 3\n";

	//		T2  Deck card count should decrease by 3.  (becuase initial state has 5 deck cards)
	tResult = (G.deckCount == G_original.deckCount - 3);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T2: Smithy Card Test:" << PASS(tResult) << " Returns true when deckCount decreases by 3\n";

	//		T3  Actions remain the same
	tResult = (G.numActions == G_original.numActions);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T3: Smithy Card Test:" << PASS(tResult) << " Returns true when numActions remains the same\n";

	//		T4  Coins remain the same
	tResult = (G.coins == G_original.coins);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T4: Smithy Card Test:" << PASS(tResult) << " Returns true when coins remains the same\n";

	//		T5  Buys remain the same 
	tResult = (G.numBuys == G_original.numBuys);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T5: Smithy Card Test:" << PASS(tResult) << " Returns true when numBuys remains the same\n";

	//		T6  Played Cards Increases by 1
	tResult = (G.playedCardCount == G_original.playedCardCount + 1);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T6: Smithy Card Test:" << PASS(tResult) << " Returns true when playedCardCount increases by 1\n";

	//		T7  Total Count of cards remain the same
	int cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
	int cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
	tResult = (cardCountBefore == cardCountAfter);
	if (tResult) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult;
	if (verbose) cout << "T7: Smithy Card Test:" << PASS(tResult) << " Returns true when Total Cards owned by player remains the same.\n";

	if (verbose) cout << "Card Test 1- Smithy - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return counter; // make all stops on error

}
int * CutPurseCardTest(gameState * beforeTest, gameState * afterTest, bool verbose) {
	gameState G = *afterTest;
	struct gameState G_original = *beforeTest;
	// initial state + 1 of each of the 5 supply cards in the deck
	//int k[NUM_KINGDOM_CARDS] = { smithy, adventurer, gardens, village, cutpurse };

	int * counter = new int[2]; // counter[0] = pass count, counter[1] = fail count;
	counter[0] = 0;
	counter[1] = 0;
	int pass = 0, fail = 1;

	bool tResult = true;
	bool allPass = true;
	{// Player 1
	// Player 1 .. hand reduces by 1 / Discard + 1
		tResult = (G.handCount[0] == (G_original.handCount[0] - 1));
		if (tResult) counter[pass]++; else counter[fail]++;
		allPass = allPass && tResult;
		if (verbose) cout << "T1: CutPurse Card Test:" << PASS(tResult) << " Player 1's handCount reduces by 1\n";

		tResult = (G.discardCount[0] == (G_original.discardCount[0]));
		if (tResult) counter[pass]++; else counter[fail]++;
		allPass = allPass && tResult;
		if (verbose) cout << "T2: CutPurse Card Test:" << PASS(tResult) << " Player 1's discardCount stays the same\n";

		// Total Card Test for Player 1
		tResult = (G.handCount[0] + G.deckCount[0] + G.discardCount[0] + G.playedCardCount) 
			== (G_original.handCount[0] + G_original.deckCount[0] + G_original.discardCount[0] + G_original.playedCardCount);
		if (tResult) counter[pass]++; else counter[fail]++;
		allPass = allPass && tResult;
		if (verbose) cout << "T3: CutPurse Card Test:" << PASS(tResult) << " Player 1's total card count remains the same.\n";
	}
	// Player 2 .. hand reduces by 1 / Discard + 1
	{// Player 2
// Player 2 .. hand reduces by 1 / Discard + 1
		bool hasCopper = false;
		for (int m = 0; m < G_original.handCount[1] ; m++)
			if (G_original.hand[1][m] == copper)
			{
				m = G_original.handCount[1]; // a copper is found
				hasCopper = true;
			}
		if (hasCopper)
			tResult = (G.handCount[1] == (G_original.handCount[1] - 1)); // handcount decreases
		else
			tResult = (G.handCount[1] == (G_original.handCount[1])); // handcount stays the same
		if (tResult) counter[pass]++; else counter[fail]++;
		allPass = allPass && tResult;
		if (verbose) cout << "T4: CutPurse Card Test:" << PASS(tResult) << " Player 2's handCount reduces by 1 if copper present\n";

		if (hasCopper)
			tResult = (G.discardCount[1] == (G_original.discardCount[1] + 1));
		else
			tResult = (G.discardCount[1] == (G_original.discardCount[1]));
		if (tResult) counter[pass]++; else counter[fail]++;
		allPass = allPass && tResult;
		if (verbose) cout << "T5: CutPurse Card Test:" << PASS(tResult) << " Player 2's discardCount increases by 1 if copper was present\n";

		// Total Card Test for Player 2
		tResult = (G.handCount[1] + G.deckCount[1] + G.discardCount[1]) == (G_original.handCount[1] + G_original.deckCount[1] + G_original.discardCount[1]);
		if (tResult) counter[pass]++; else counter[fail]++;
		allPass = allPass && tResult;
		if (verbose) cout << "T6: CutPurse Card Test:" << PASS(tResult) << " Player 2's total card count remains the same.\n";
	}

	if (verbose) cout << "Card Test -- CutPurse - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return counter; // make all stops on error

}
int *  AdventurerCardTest(gameState * beforeTest, gameState * afterTest, bool verbose) {
	// AdventureCard Effect
	//int counter[2] = { 0 }; // counter[0] = pass count, counter[1] = fail count;
	int * counter = new int[2]; // counter[0] = pass count, counter[1] = fail count;
	counter[0] = 0;
	counter[1] = 0;
	int pass = 0, fail = 1;
	bool allPass = true;
	bool tResult = true;
	gameState G = *afterTest;
	gameState G_original = *beforeTest;
	int player_i = whoseTurn(&G);

	//		T1  Total Count of cards remain the same
	int cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
	int cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
	tResult = (cardCountBefore == cardCountAfter);
	if (tResult) counter[pass]++; else counter[fail]++;
	//if (verbose) cout << "T1: Adventurer Card Test:" << PASS(tResult) << " Returns true when Total Cards owned by player remains the same.\n";

	allPass = allPass && tResult;



	//					}
					//	cardEffectAdventurer(&G); // the cardEffect signature is very odd to me... How coins are tracked is odd.
						// Check Total Card Count
						//		T1-Repeat  Total Count of cards remain the same
	cardCountBefore = G.deckCount[player_i] + G.discardCount[player_i] + G.playedCardCount + G.handCount[player_i];
	cardCountAfter = G_original.deckCount[player_i] + G_original.discardCount[player_i] + G_original.playedCardCount + G_original.handCount[player_i];
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
	if (tResult_Buys_Actions_same) counter[pass]++; else counter[fail]++;
	if (tResult_HandCount_plus2) counter[pass]++; else counter[fail]++;
	if (tResult_TotalCardCount) counter[pass]++; else counter[fail]++;
	if (tResult_valueIncrease) counter[pass]++; else counter[fail]++;
	allPass = allPass && tResult_valueIncrease &&tResult_TotalCardCount&&tResult_HandCount_plus2
		&&tResult_Buys_Actions_same;
	{ // print results: brack for code folding only
		if (verbose) cout << "\t\t" << coinCountBefore << "\t" << coinCountAfter << "\t\t";
		if (verbose) cout << PASS(tResult_valueIncrease);
		if (verbose) cout << "\t\t\t";
		if (verbose) cout << PASS(tResult_TotalCardCount);
		if (verbose) cout << "\t\t\t";
		if (verbose) cout << PASS(tResult_HandCount_plus2);
		if (verbose) cout << "\t\t";
		if (verbose) cout << PASS(tResult_Buys_Actions_same);
	}
	if (verbose) cout << endl;


	// TODO: Build same test with required shuffle of Discarded Cards

	//if (verbose) cout << "Card Test - Adventurer - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return counter; // make all stops on error}
}
// the function used to generate a random game state // after initializations
// returns the hand position of the challenge card
int gameStateRandomizer(gameState * G, int challengCard)
{
	// generate random metrics
//	bool implmented = false;

	// variables that are randomized
	int handCount, deckCount, discardCount, playedCount, challengeCardPosition, numberActions, coins, numBuys;
	handCount = rand() % 25 + 1; //  between 0 and 25 cards
	deckCount = rand() % 25 + 1;
	discardCount = rand() % 25 + 1;
	playedCount = rand() % 25 + 1;
	challengeCardPosition = rand() % handCount; // 
	numberActions = rand() % 6; // up to 5 actions
	coins = rand() % 11; // up to 10 coins
	numBuys = rand() % 6; // up to 5 buys

	for (int player_i = 0; player_i < G->numPlayers; player_i++)
	{
		for (int j = 0; j < 25; j++) // were going to just fill all 25 spots,  the dominion program uses a counter to access the card in question
		{
			//find a random card
			//supplyCount[treasure_map + 1];
			int card = 0;
			card = rand() % (treasure_map + 1);
			G->hand[player_i][j] = card;
			card = rand() % (treasure_map + 1);
			G->deck[player_i][j] = card;
			card = rand() % (treasure_map + 1);
			G->discard[player_i][j] = card;
			card = rand() % (treasure_map + 1);
			G->playedCards[j] = card;

		}
		G->handCount[player_i] = handCount;
		G->deckCount[player_i] = deckCount;
		G->discardCount[player_i] = discardCount;
		G->playedCardCount = playedCount;
		G->hand[player_i][challengeCardPosition] = challengCard;
	}
	G->numActions = numberActions;
	G->coins = coins;
	G->numBuys = numBuys;
	return challengeCardPosition;
}