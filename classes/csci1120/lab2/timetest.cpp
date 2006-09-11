#include <iostream>

using namespace std;

#include "ctime.h"

int main( void )
{
	cTime now;
	cTime then(11, 30, 05);
	cTime when(14);
	
	cout << "This is a test of the cTime Class" << endl;

	cout << "now = "; 
	now.Display();
	cout << endl;
	
	cout << "then = "; 
	then.Display();
	cout << endl;
	
	cout << "when = "; 
	when.Display();
	cout << endl;
	
	return 0;
}
