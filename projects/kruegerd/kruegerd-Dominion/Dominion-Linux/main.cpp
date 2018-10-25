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

extern "C++" //https://en.cppreference.com/w/cpp/language/language_linkage
{
#include "rngs.h"
}

using namespace std;


int LinkTestRNGS(); // testing link to rngs.c

int main()
{
    printf("hello from Dominion_Linux!\n");

		LinkTestRNGS();
		cout << "[Press Enter to Exit]";
		cin.get();
    return 0;
}

int LinkTestRNGS() {
	//double Random(void);
	//void   PlantSeeds(long x);
	//void   GetSeed(long *x);
	//void   PutSeed(long x);
	//void   SelectStream(int index);
	//void   TestRandom(void);
	cout << "Calling Random: " << Random() << endl;
	return 0;
}

