#include <iostream>
using namespace std;

int main() {
	int numberin;
	const float PI=3.14159265358979;

	
	cout << "Input the radius of the circle." << endl;
	cin >> numberin;
	cout << "The area of the circle is: ";
	cout << numberin * numberin * PI;
	cout << endl;

	return 0;
}
