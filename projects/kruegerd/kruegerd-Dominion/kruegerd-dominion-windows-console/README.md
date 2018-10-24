/// Dominion Windows Visual Studio Project Files
/// This is a windows console program that should run in both a windows and linux environment


/// From the ./dominion/MAKEFILE  the instructions for playdom.c to run included dependences for the following:
playdom.c
	| Links to--
	|--> dominion.c
			| Links to--
			|-->rngs.c

// Testsuites had there own main and depended on only the dominion and rngs code
./testSuite.c
	| Links to--
	|--> dominion.c
			| Links to--
			|-->rngs.c




/////////// Remember this....  To compile ".c" configuration Propertices--> C/C++ --> advanced --> compile as c++  
///////////    This is a difficult error because the files all compile fine with .c but they don't link well.. So you only get the error during the build 
///////////		 and only when the build determins a link is necessary  https://msdn.microsoft.com/en-us/library/032xwy55.aspx
// reference to command line windows compile:   https://msdn.microsoft.com/en-us/library/ms235639.aspx

