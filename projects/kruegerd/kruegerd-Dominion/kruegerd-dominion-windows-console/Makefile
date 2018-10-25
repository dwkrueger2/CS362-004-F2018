#CFLAGS= -Wall -fpic -coverage -lm -std=c99
#CFLAGS= -Wall -fpic -coverage -lm -std=c++11
CFLAGS= -Wall -std=c++11
rngs.o: ../../dominion/rngs.h ../../dominion/rngs.c
	gcc -c -x c++ -o ../../dominion/rngs.o ../../dominion/rngs.c -g  $(CFLAGS)
#	 gcc -c -x c++ ../../dominion/rngs.c -g -Wall -fpic -coverage -lm -std=c++11

dominion.o: ../../dominion/dominion.h ../../dominion/dominion.c ../../dominion/rngs.o
	gcc -c -x c++ ../../dominion/dominion.c -g  $(CFLAGS)
#gcc -c -x c++ ../../dominion/dominion.c -g -Wall -fpic -coverage -lm -std=c++11

playdom: ../../dominion/dominion.o ../../dominion/playdom.c
	gcc -x c++ -o dominion/playdom ../../dominion/playdom.c -g ../../dominion/dominion.o ../../dominion/rngs.o $(CFLAGS)
#gcc -x c++ -o dominion/playdom ../../dominion/playdom.c -g dominion.o rngs.o -g -Wall -fpic -coverage -lm -std=c++11


winConsole: ../../dominion/dominion.o ../../dominion/playdom.c kruegerd-dominion-windows-console.cpp
	gcc -o winC kruegerd-dominion-windows-console.cpp  -g ../../dominion/dominion.o ../../dominion/rngs.o $(CFLAGS)

#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
#testDrawCard: testDrawCard.c dominion.o rngs.o
#	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)
#
#	#badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
#	#	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)
#
#	#testBuyCard: testDrawCard.c dominion.o rngs.o
#	#	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)
#
#	#testAll: dominion.o testSuite.c
#	#	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)
#
#	#interface.o: interface.h interface.c
#	#	gcc -c interface.c -g  $(CFLAGS)
#
#	#runtests: testDrawCard 
#	#	./testDrawCard &> unittestresult.out
#	#	gcov dominion.c >> unittestresult.out
#	#	cat dominion.c.gcov >> unittestresult.out
#
#
#	player: ../../dominion/player.c ../../dominion/interface.o
#		gcc -o ../../dominion/player ../../dominion/player.c -g  ../../dominion/dominion.o ../../dominion/rngs.o ../../dominion/interface.o $(CFLAGS)
#
#all: playdom player 
all: playdom winConsole 
#
clean:
	rm -f *.o ../../dominion/playdom.exe ../../dominion/playdom ../../dominion/player ../../dominion/player.exe  ../../dominion/*.gcov ../../dominion/*.gcda ../../dominion/*.gcno ../../dominion/*.so ../../dominion/*.out ../../dominion/testDrawCard ../../dominion/testDrawCard.exe
#
