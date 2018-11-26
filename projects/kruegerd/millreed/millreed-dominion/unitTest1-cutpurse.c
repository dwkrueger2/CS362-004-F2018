// cutpurse - unitTest  
//		This is the unittest used for the cutPurse Card in Assgnment3
//
// Each Other Player discards a copper or reveals a hand without a copper
//  The other test took so much time to develop this test will just focus on 4 player hand situations instead of looking at every permutation
//
//	The 4 hands will be CGKKK,SCKKK, KKKKK, KSGKK  - (C)opper, (G)old, (S)ilver, (K)ingdom

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <ctime>
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
using namespace std;
#define USE_AS_FUNCTION 1// 0 true 1 false
#if USE_AS_FUNCTION
int cardtest1_c() {
#else
	int main (){
#endif
		time_t t1 = time(0);
		srand(t1); //rngs.c was just not very clear on why and how it is to be used.
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
	printHeader("Running Unit Test for CutPurse Card from Assignment 3");
	cardEffect(cutpurse, 0, 0, 0, &G, 0, 0); // original signuture for call to cut purse
//	cardEffectCutPurse(&G, 2);
	{// Player 1
	// Player 1 .. hand reduces by 1 / playedCards + 1
		bool hasCopper = false;
		for (int m = 0; m < G_original.handCount[0]; m++)
			if (G_original.hand[0][m] == copper)
			{
				m = G_original.handCount[0]; // a copper is found
				hasCopper = true;
			}
		if (hasCopper)
			tResult = (G.handCount[0] == (G_original.handCount[0] - 1)); // handcount decreases
		else
			tResult = (G.handCount[0] == (G_original.handCount[0])); // handcount stays the same
		tResult = (G.handCount[0] == (G_original.handCount[0] - 1));
		allPass = allPass && tResult;
		cout << "T1: CutPurse Card Test:" << PASS(tResult) << " Player 1's handCount reduces by 1\n";

		if (hasCopper)
			tResult = (G.discardCount[0] == (G_original.discardCount[0]) + 1);
		else
			tResult = (G.discardCount[0] == (G_original.discardCount[0]));
		allPass = allPass && tResult;
		cout << "T2: CutPurse Card Test:" << PASS(tResult) << " Player 1's discardCount increases by 1 if copper present\n"; // 4 players and current player is 3 (index 2)

		// Total Card Test for Player 1
		tResult = (G.handCount[0] + G.deckCount[0] + G.discardCount[0])// + G.playedCardCount)
			== (G_original.handCount[0] + G_original.deckCount[0] + G_original.discardCount[0]); //+ G_original.playedCardCount);
		allPass = allPass && tResult;
		cout << "T3: CutPurse Card Test:" << PASS(tResult) << " Player 1's total card count remains the same.\n";
	}
	// Player 2 .. hand reduces by 1 / Discard + 1
	{// Player 2
// Player 2 .. hand reduces by 1 / Discard + 1
		bool hasCopper = false;
		for (int m = 0; m < G_original.handCount[1]; m++)
			if (G_original.hand[1][m] == copper)
			{
				m = G_original.handCount[1]; // a copper is found
				hasCopper = true;
			}
		if (hasCopper)
			tResult = (G.handCount[1] == (G_original.handCount[1] - 1)); // handcount decreases
		else
			tResult = (G.handCount[1] == (G_original.handCount[1])); // handcount stays the same
		allPass = allPass && tResult;
		cout << "T4: CutPurse Card Test:" << PASS(tResult) << " Player 2's handCount reduces by 1 if copper present\n";

		// discard count test
		if (hasCopper)
			tResult = (G.discardCount[1] == (G_original.discardCount[1] + 1));
		else
			tResult = (G.discardCount[1] == (G_original.discardCount[1]));
		allPass = allPass && tResult;
		cout << "T5: CutPurse Card Test:" << PASS(tResult) << " Player 2's discardCount increases by 1 if copper present\n";

		// Total Card Test for Player 2
		tResult = (G.handCount[1] + G.deckCount[1] + G.discardCount[1]) == (G_original.handCount[1] + G_original.deckCount[1] + G_original.discardCount[1]);

		allPass = allPass && tResult;
		cout << "T6: CutPurse Card Test:" << PASS(tResult) << " Player 2's total card count remains the same.\n";
	}
	// Player 3 .. hand stays at 5 /  Discard stays the same
	{// Player 3
// Player 3 .. hand reduces by 1 / Discard + 1
		// Player 3 is the active player for this unit test (index 2)
		
		tResult = (G.handCount[2] == (G_original.handCount[2] - 1)); // handcount decreases
		allPass = allPass && tResult;
		cout << "T7: CutPurse Card Test:" << PASS(tResult) << " Player 3's (the attacker) handCount reduces by 1\n";

		tResult = (G.playedCardCount == (G_original.playedCardCount + 1));
		allPass = allPass && tResult;
		cout << "T8: CutPurse Card Test:" << PASS(tResult) << " Played Card Count increases by 1\n";

		// Total Card Test for Player 3
		tResult = (G.handCount[2] + G.deckCount[2] + G.discardCount[2] + G.playedCardCount) == (G_original.handCount[2] + G_original.deckCount[2] + G_original.discardCount[2] + G_original.playedCardCount);

		allPass = allPass && tResult;
		cout << "T9: CutPurse Card Test:" << PASS(tResult) << " Player 3 total card count remains the same.\n";
	}
	// Player 4 .. hand stays at 5 / Discard stays the same
	{// Player 4
// Player 4 .. hand reduces by 1 / Discard + 1
		bool hasCopper = false;
		for (int m = 0; m < G_original.handCount[3]; m++)
			if (G_original.hand[3][m] == copper)
			{
				m = G_original.handCount[3]; // a copper is found
				hasCopper = true;
			}
		if (hasCopper)
			tResult = (G.handCount[3] == (G_original.handCount[3] - 1)); // handcount decreases
		else
			tResult = (G.handCount[3] == (G_original.handCount[3])); // handcount stays the same
		allPass = allPass && tResult;
		cout << "T10: CutPurse Card Test:" << PASS(tResult) << " Player 4's handCount reduces by 1 if copper present\n";


		//discard count test
		if (hasCopper)
			tResult = (G.discardCount[3] == (G_original.discardCount[3] + 1));
		else
			tResult = (G.discardCount[3] == (G_original.discardCount[3]));
		allPass = allPass && tResult;
		cout << "T11: CutPurse Card Test:" << PASS(tResult) << " Player 4's discardCount remains the same\n";

		// Total Card Test for Player 4
		tResult = (G.handCount[3] + G.deckCount[3] + G.discardCount[3]) == (G_original.handCount[3] + G_original.deckCount[3] + G_original.discardCount[3]);

		allPass = allPass && tResult;
		cout << "T12: CutPurse Card Test:" << PASS(tResult) << " Player 4's total card count remains the same.\n";
	}

	cout << "Card Test 3 - CutPurse - all tests:" << PASS(allPass) << endl;
	//return (allPass ? 0 : 1);
	return 0; // make all stops on error

}
