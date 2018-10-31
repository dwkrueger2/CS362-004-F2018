
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>

#ifdef _WIN32
	#include <Windows.h>
#define _CRT_SECURE_NO_WARNINGS
#endif
using namespace std;
string PASS(bool pvalue); // A utility function to print PASS or FAIL based on pvalue;
char inputChar() // Range ' ' to '~' utilized.
{
	// TODO: rewrite this function
	int r = rand() % ((int)'~' - ' '+1);
	char s = (char)(r + ' ');
	return s;
}
char inputChar2() // Range 'a'-'z' utilized.
{
	
	int r = rand() % ((int)'a' - 'z' + 1);
	char s = (char)(r + 'a');
	return s;
}
char *inputString()
{
	// TODO: rewrite this function
	 //char s1[6] = "reset";
	 //char * s1 = new char[6];
	char * s1 = (char *)malloc(sizeof(char) * 6);
	for (int i = 0; i < 6; i++)
		s1[i] = '\0';
	int size = strlen("reset");
	for (int i = 0; i < size; i++)
	{
		s1[i] = inputChar2();
		if (s1[i] < 'a' || s1[i] > 'z')
			cout << "Error from inputChar2: out of range character: " << s1[i] << ": value: " << (int)s1[i] << endl;
	}

	return s1;
}
 void testme()
 {
	 int tcCount = 0;
	 char s[8] = { '\0' };
	 char c;
	 int state = 0;
	 while (1)
	 {
		 tcCount++;
		 c = inputChar();
	/* #ifdef __linux__
		 
		 strcpy(s, inputString());
	 #elif _WIN32*/
		 char * a = inputString();
		 for (int i = 0; i < 6; i++)
			 s[i] = a[i]; 
	 //#endif
		 //s = inputString();
		 printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s,
			 state);
		 if (c == '[' && state == 0) state = 1;
		 if (c == '(' && state == 1) state = 2;
		 if (c == '{' && state == 2) state = 3;
		 if (c == ' '&& state == 3) state = 4;
		 if (c == 'a' && state == 4) state = 5;
		 if (c == 'x' && state == 5) state = 6;
		 if (c == '}' && state == 6) state = 7;
		 if (c == ')' && state == 7) state = 8;
		 if (c == ']' && state == 8) state = 9;
		 if (state == 9)
	//		 cout << "State 9 hit on iteration: " << tcCount << endl;
		 if (s[0] == 'r' && s[1] == 'e'
			 && s[2] == 's' && s[3] == 'e'
			 && s[4] == 't' && s[5] == '\0'
			 && state == 9)
		 {
			 printf("error ");
			 exit(200);
		 }
		// delete(a);
		 free(a);
	 }
 }

 bool testInputCharacter()
 {

	 int s[95] = { 0 };
	// cout << "Range :" << ((int)'~' - (int)' ') << endl;
	 int maxNumIterations = 10000;
	 bool generatedAllCharacters = true;
	 for (int i = 0; i < maxNumIterations; i++)
	 {
		 generatedAllCharacters = true;
		 char s1 = inputChar();
		 //s1 = '~';
		// cout << s1 << ": " << ((int)s1) << endl;
		 int index = (int)s1 - (int)' ';
		 if (index < 0 || index >= 95)
			 cout << "Error 1: Index Out of Range\n";
		 if (s1 < ' ' && s1 > '~')
			 cout << "Error 2: Character out of Range\n";
		 if (s[index] != 1)
			 s[index] = 1;
		 for (int j = 0; j < 95; j++)
			 generatedAllCharacters = generatedAllCharacters && (s[j] == 1) && (s1 >= ' ' && s1 <= '~');
		 if (generatedAllCharacters)
		 {
			// cout << ": All 100 characters hit on the " << i << "th iteration\n";
			 //i = maxNumIterations;
		 }
		/* if (i % 100 == 0)
			 cout << "100::::\n";*/
	 }
	 return generatedAllCharacters;
 }

int main(int argc, char *argv[])
{
	srand(time(NULL));
	cout << "Testing inputChar() generates all 94 characters from ' ' to '~'.\n";
#ifdef _WIN32
	PASS(testInputCharacter());
#endif
#ifdef __linux__
	cout << PASS(testInputCharacter()) << endl;
#endif


	testme();
	return 0;
}

#ifdef _WIN32  // Windows and Linux require different implementations for color coding text on the console screen
string PASS(bool pvalue) {
	HANDLE  hConsole;
	_CONSOLE_SCREEN_BUFFER_INFO  currentConsoleState;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &currentConsoleState);
	cout.flush();
	// Print sample colors
	//for (int i = 0; i < 32; i++) {
	//	SetConsoleTextAttribute(hConsole, i); // Fail is 12-red on black // Succes is 10 - Green on Black
	//	cout << "Color Number: " << i << endl;
	//}
	if (pvalue)
	{
		SetConsoleTextAttribute(hConsole, 10); // Fail is 12-red on black // Succes is 10 - Green on Black
		cout << "PASS ";
	}
	else {
		SetConsoleTextAttribute(hConsole, 12); // Fail is 12-red on black // Succes is 10 - Green on Black
		cout << "FAIL ";
	}
	// return console text to previous color
	SetConsoleTextAttribute(hConsole, currentConsoleState.wAttributes); // return display state
	cout.flush();
	return ""; // already had code written to recieve a string and it kinda still works well (pvalue ? "PASS" : "FAIL");
}

#elif __linux__
#if (USE_COLOR)
string PASS(bool pvalue) { return (pvalue ? "\033[32mPASS\033[0m" : "\033[31mFAIL\033[0m"); } // note this is a c-preprocessor concationation trick... Not exactly intuitive https://stackoverflow.com/questions/12958925/expand-macros-inside-quoted-string
#else
string PASS(bool pvalue) { return (pvalue ? "PASS" : "FAIL"); } // note this is a c-preprocessor concationation trick... Not exactly intuitive https://stackoverflow.com/questions/12958925/expand-macros-inside-quoted-string
#endif
#endif
