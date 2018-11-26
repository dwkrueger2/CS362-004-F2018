CFLAGS= -Wall -fpic -coverage -lm -std=c++11 -lstdc++
# Version 202
# for setting bash (i.e. cut an paste)  $CFLAGS=" -Wall -fpic -coverage -lm -std=c++11 -lstdc++"
# you have to remove the '(' ')' from command line when running one at a time

rngs.o: rngs.h rngs.c
	gcc -x c++ -c rngs.c -g  $(CFLAGS) 

unitHelper: unitTestSupport.c dominion.o
	gcc -x c++ -c unitTestSupport.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -x c++ -c dominion.c -g  $(CFLAGS)

#unitTest1 is the cutpurse test  It is not included in the full test suite
unitTest1: dominion.c unitTest1-cutpurse.c unitHelper
	gcc -x c++ -o unitTest1 -g unitTest1-cutpurse.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

#randomtestcard1 is the cutpurse code
cardTest1: dominion.c randomtestcard1.c unitHelper
	#gcc -x c++ -lstdc++ -o cardTest1 -c -g randomtestcard1.c dominion.c rngs.c unitTestSupport.c $(CFLAGS)
	#gcc -x c++ -o cardTest1 -g randomtestcard1.c dominion.c rngs.c unitTestSupport.c  -lstdc++ $(CFLAGS)
	gcc -x c++ -o cardTest1 -g randomtestcard1.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

#randomtestcard2 is the smithy cardtest code
cardTest2: dominion.c randomtestcard2.c unitHelper
	gcc -x c++ -o cardTest2 -g randomtestcard2.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

#randomtestcard3 is the Adventurer cardtest code
cardTest3: dominion.c randomtestcard3.c unitHelper
	gcc -x c++ -o cardTest3 -g randomtestcard3.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

adventureTest: dominion.c randomtestadventurer.c unitHelper
	#gcc -x c++ -lstdc++ -o adventureTest -c -g randomtestadventurer.c dominion.c rngs.c unitTestSupport.c $(CFLAGS)
	gcc -x c++ -o adventureTest -g randomtestadventurer.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

testSuite: cardTest1 cardTest2 adventureTest
	./cardTest1
	./cardTest2
	./adventureTest

testAll: cardTest1 cardTest2 adventureTest
	./cardTest1 &> unittestresult.out
	./cardTest2 &>> unittestresult.out
	./adventureTest &>> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	


#runtests: testDrawCard 
#	./testDrawCard &> unittestresult.out
#	gcov dominion.c >> unittestresult.out
#	cat dominion.c.gcov >> unittestresult.out


#player: player.c interface.o
#	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

#all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe adventureTest *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe unitTest[1-4] cardTest[1-4] testSuite