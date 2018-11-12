#include 	<iostream>
#include	<iomanip>
#include	<unistd.h>

//Color codes for terminal text
#define KNRM		"\x1B[0m"
#define KRED		"\x1B[5;31m"
#define KGRN		"\x1B[4;32m"
#define KYEL		"\x1B[7;33m"
#define KBLU		"\x1B[1;34m"
#define KMAG		"\x1B[35m"
#define KCYN		"\x1B[36m"
#define KWHT  		"\x1B[37m"

//Moves the cursor to the specified line and column

#define KCUR(X,Y) 	"\x1B[X;YH"

//Moves the cursor up N lines

#define KCUP(X)		"\x1B[XA"
//Clear screen
#define KCLR		"\x1B[2J"
using namespace std;
int main(){
	printf(KCLR);
	printf("\x1B[0;0H");
	for(int i = 0; i < 60; i++){
		switch(i % 5){
			case 0: printf(KNRM); break;
			case 1: printf(KRED); break;
			case 2: printf(KGRN); break;
			case 3: printf(KYEL); break;
			case 4: printf(KBLU); break;
		}
	
		printf("\x1B[0;0H");
		printf("%s%d%s", "Value of i: ", i, "                 " KNRM);
		fflush(stdout);

		usleep(1000 * 1000);
	}
	cout << endl;
}
