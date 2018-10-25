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
#include <time.h>
#include <ctime>
#include <climits>

#ifdef _WIN32
#include <Windows.h>
#elif __linux__
#include <ctime>
#endif // (_WIN32)
//extern "C++"
{
#include "../../dominion/dominion_helpers.h"
#include "../../dominion/dominion.h"
#include "../../dominion/rngs.h"
}



/////////// Remember this....  To compile ".c" configureation Propertices--> C/C++ --> advanced --> compile as c++  
///////////    This is a difficult error because the files all compile fine with .c but they don't link well.. So you only get the error during the build 
///////////		 and only when the build determins a link is necessary

using namespace std;
int playdom_c_main(int argc, const char** argv);

int main(int argc, char ** argv)
{
		const char * args[] = { "0","2" }; // used to simulate args sent to playdom_c_main
		playdom_c_main(2, args);
		
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

