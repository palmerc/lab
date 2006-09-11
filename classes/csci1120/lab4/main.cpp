#include <iostream>
#include "polyterm.h"

using namespace std;

int main()
{
   PolyTerm polyguy;
   PolyTerm polyguy2(123.4, 5);

   cout << "Default constructor initial value:" << endl;
   polyguy.Display();
   polyguy.setCoefficient(299.1);
   polyguy.setPower(4);
   cout << "Changed values using setCoefficient and set Power:" << endl;
   polyguy.Display();
   cout << "Default constructor with specified value:" << endl;
   polyguy2.Display();
   cout << "Test of getCoefficient: " << polyguy2.getCoefficient() << endl;
   cout << "Test of getPower: " << polyguy2.getPower() << endl;

   return 0;
}
