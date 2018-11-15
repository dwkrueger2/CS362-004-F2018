// This folder contains the modified source code used for user: kruegerd 
//
//	Current Version - Assignment 4 -- see source control for other versions 

3 Tests are created 
randomtestcard1.c
randomtestcard2.c
randomtestadventurer.c


Note Code is gcc/g++ c11 standard compliant. See Makefile for command details

Makefile provided:

To Build and run type 'make testSuite' 

To Run the test suite and generate coverage and capture output data run 'make testALL'. If this option is chosen then the file 'unittestresult.out' will show coverage data of the dominion.c file.

to clean up object files run 'make clean'

*** Note randomtestcard1,randomtestcard2 and adventurertestcards all have the same code with a 
preprocessor directive changed.  Changing the folllowing definition will change the running file.
//#define RUN_SMITHY
#define RUN_CUTPURSE
//#define RUN_ADVENTURE
*** 


Contents: 
	dominion.c 
	dominion.h 
	dominion_helpers.h 
	Makefile 
	README.md 
	rngs.c 
	rngs.h 
	unittestresult.out 
	unitTestSupport.c 
	unitTestSupport.h
	randomtestcard1.c
	randomtestcard2.c
