#include <iostream>
#include "polynomial.h"

using namespace std;

int main()
{
   Polynomial polyguy;
   Polynomial polyguy2(123.4, 5);

   cout << "Default constructor initial value:" << endl;
   polyguy.Display();

   polyguy.setCoefficient(4, 299.1);
   cout << "Changed values using setCoefficient and set Power:" << endl;
   polyguy.Display();

   cout << "Default constructor with specified value:" << endl;
   polyguy2.Display();
   cout << "Test of getCoefficient: " << polyguy2.getCoefficient() << endl;

   polyguy.addPolynomial(polyguy2);
   cout << "Addition of the default and specified polynomials:" << endl;
   polyguy.Display();

   return 0;
}
