CFLAGS= -Wall -fpic -coverage -lm -std=c++11 -lstdc++

test: testme.c
	gcc -x c++ -o tm testme.c -g  $(CFLAGS)
	tm

testCoverage: 
	gcov testme.c >> testme-coverage.out
	cat testme.c.gcov >> testme-coverage.out


clean:
	rm -f *.o tm *.gcda *.gcno *.out *.gcov