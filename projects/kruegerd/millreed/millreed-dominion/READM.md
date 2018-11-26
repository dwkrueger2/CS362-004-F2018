// This folder contains the modified source code used for user: kruegerd 
//
//	Current Version - Assignment 5 -- see source control for other versions 

11 Tests are created
3 random card tests
8 unitTests  from Assignment 3



Note Code is gcc/g++ c11 standard compliant. See Makefile for command details

Makefile provided:

To Build and run type 'make testAll' 
To Build only type 'make testSuite'

To Run the test suite and generate coverage and capture output data run 'make testALL'. If this option is chosen then the file 'finalTestOutput.out' 
will show coverage data of the dominion.c file.  For convience the test results are listed first then listed at the head of each gcov section.
Because of the manor in wich the project requirements were structured I was unable to get all tests to combine into one gcov file.

to clean up object files run 'make clean'

*** Note randomtestcard1,randomtestcard2 and adventurertestcards all have the same code with a 
preprocessor directive changed.  Changing the folllowing definition will change the running file.
//#define RUN_SMITHY
#define RUN_CUTPURSE
//#define RUN_ADVENTURE
*** 

*** if you want to see the test on the uncorrected file change the following to preprocessor directive in dominion.c to a 0
#define USE_CORRECTED_VERSION 1  //

Contents: 
	dominion.c 
	dominion.h 
	dominion_helpers.h 
	Makefile 
	README.md 
	rngs.c 
	rngs.h 
	interface.c
	interface.h
	unitTestSupport.c 
	unitTestSupport.h
	randomtestcard1.c
	randomtestcard2.c
	randomtestcard3.c
	unitTest1.c
	unitTest2.c
	unitTest3.c
	unitTest4.c
	unitTest5.c
	unitTest6.c
	unitTest7.c
	unitTest8.c
