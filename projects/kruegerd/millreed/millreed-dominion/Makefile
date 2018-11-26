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
#unitTest1: dominion.c unitTest1-cutpurse.c unitHelper
#	gcc -x c++ -o unitTest1 -g unitTest1-cutpurse.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

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


#randomtestcard3 is the Adventurer cardtest code
unitTest1: dominion.c unitTest1.c unitHelper
	gcc -x c++ -o unitTest1 -g unitTest1.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
#randomtestcard3 is the Adventurer cardtest code
unitTest2: dominion.c unitTest2.c unitHelper
	gcc -x c++ -o unitTest2 -g unitTest2.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
#randomtestcard3 is the Adventurer cardtest code
unitTest3: dominion.c unitTest3.c unitHelper
	gcc -x c++ -o unitTest3 -g unitTest3.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
#randomtestcard3 is the Adventurer cardtest code
unitTest4: dominion.c unitTest4.c unitHelper
	gcc -x c++ -o unitTest4 -g unitTest4.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
unitTest5: dominion.c unitTest5.c unitHelper
	gcc -x c++ -o unitTest5 -g unitTest1.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
#randomtestcard3 is the Adventurer cardtest code
unitTest6: dominion.c unitTest6.c unitHelper
	gcc -x c++ -o unitTest6 -g unitTest2.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
#randomtestcard3 is the Adventurer cardtest code
unitTest7: dominion.c unitTest7.c unitHelper
	gcc -x c++ -o unitTest7 -g unitTest3.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++
#randomtestcard3 is the Adventurer cardtest code
unitTest8: dominion.c unitTest8.c unitHelper
	gcc -x c++ -o unitTest8 -g unitTest4.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

adventureTest: dominion.c randomtestadventurer.c unitHelper
	#gcc -x c++ -lstdc++ -o adventureTest -c -g randomtestadventurer.c dominion.c rngs.c unitTestSupport.c $(CFLAGS)
	gcc -x c++ -o adventureTest -g randomtestadventurer.c dominion.c rngs.c unitTestSupport.c -Wall -fpic -coverage -lm -std=c++11 -lstdc++

testSuite: cardTest1 cardTest2 cardTest3 unitTest1 unitTest2 unitTest3 unitTest4 unitTest5 unitTest6 unitTest7 unitTest8
	./cardTest1
	./cardTest2
	./cardTest3
	./unitTest1
	./unitTest2
	./unitTest3
	./unitTest4
	./unitTest5
	./unitTest6
	./unitTest7
	./unitTest8


testAll: cardTest1 cardTest2 cardTest3 unitTest1 unitTest2 unitTest3 unitTest4 unitTest5 unitTest6 unitTest7 unitTest8
	./cardTest1 &> unittestresult.out
	./cardTest2 &>> unittestresult.out
	./cardTest3 &>> unittestresult.out
	./unitTest1 &>> unittestresult.out
	./unitTest2 &>> unittestresult.out
	./unitTest3 &>> unittestresult.out
	./unitTest4 &>> unittestresult.out
	./unitTest5 &>> unittestresult.out
	./unitTest6 &>> unittestresult.out
	./unitTest7 &>> unittestresult.out
	./unitTest8 &>> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out
	make clean




clean:
	rm -f *.o playdom.exe playdom player player.exe adventureTest *.gcov *.gcda *.gcno *.so unitTest[1-8] cardTest[1-4] testSuite