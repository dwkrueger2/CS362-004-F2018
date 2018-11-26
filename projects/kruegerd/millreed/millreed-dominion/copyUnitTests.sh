#!/bin/bash

set -x
echo "Copying Unit Test Files"
cp unitTest1-cutpurse.c ./finalCode/unitTest1.c
cp unitTest2-Smithy.c ./finalCode/unitTest2.c
cp unitTest3-Adventure.c ./finalCode/unitTest3.c
cp unitTest4-garden.c ./finalCode/unitTest4.c
cp unitTest5-shuffle.c ./finalCode/unitTest5.c
cp unitTest6-buyCard.c ./finalCode/unitTest6.c
cp unitTest7-endTurn.c ./finalCode/unitTest7.c
cp unitTest8-isGameOver.c ./finalCode/unitTest8.c
cp randomtestcard1.c ./finalCode/randomtestcard1.c
cp randomtestcard2.c ./finalCode/randomtestcard2.c
cp randomtestcard3.c ./finalCode/randomtestcard3.c

echo "Verifying Unix format"
dos2unix ./finalCode/unitTest1.c
dos2unix ./finalCode/unitTest2.c
dos2unix ./finalCode/unitTest3.c
dos2unix ./finalCode/unitTest4.c
dos2unix ./finalCode/unitTest5.c
dos2unix ./finalCode/unitTest6.c
dos2unix ./finalCode/unitTest7.c
dos2unix ./finalCode/unitTest8.c
dos2unix ./finalCode/randomtestcard1.c
dos2unix ./finalCode/randomtestcard2.c
dos2unix ./finalCode/randomtestcard3.c

echo "checking random test files"
if [ ! -f ./finalCode/randomtestcard1.c]; then
	echo "ranodomtestcar1.c not present"
fi

if [ ! -f ./finalCode/randomtestcard2.c]; then
	echo "ranodomtestcar2.c not present"
fi

if [ ! -f ./finalCode/randomtestcard3.c]; then
	echo "ranodomtestcar3.c not present"
fi 

echo "copying other source code"
dos2unix rngs.h
dos2unix rngs.c
dos2unix unitTestSupport.c
dos2unix unitTestSupport.h
dos2unix dominion.h
dos2unix dominion.c
dos2unix dominion_helpers.h
dos2unix Makefile
dos2unix interface.c
dos2unix interface.h

cp rngs.h ./finalCode
cp rngs.c ./finalCode
cp unitTestSupport.c ./finalCode
cp unitTestSupport.h ./finalCode
cp dominion.h ./finalCode
cp dominion.c ./finalCode
cp dominion_helpers.h ./finalCode
cp Makefile ./finalCode
cp interface.c ./finalCode
cp interface.h ./finalCode

