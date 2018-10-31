// This folder contains the modified source code used for user: kruegerd 
//	kruegerd-random-quiz
//
//

Execute: make test
Execute: tm
// wait for tm to finish
Execute: make testCoverage
output: testme-coverage.out

// Note the Make file could not be written to execute the program then run gcov because make stops running on a non zero result of the previous command.  
//   The program returns 200 when the error string is realized.
//
//
Files:
testme.c
Makefile
kruegerd-randomstring.pdf
README.md  (this file)



Repository Branch: kruegerd-random-quiz
