#include <polynomial.h>

using namespace std;

Polynomial::Polynomial()
{
   listClass listy;
   listy.ListInsert(0, 
}

Polynomial::~Polynomial()
{
   bool success;
   listItemType DataItem;
   while (!ListIsEmpty())
      DataItem.ListDelete(1, success);
}

void Polynomial::setCoefficient(double thePower, double newCoefficient)
{
   polynomial.setCoefficient(theCoefficient);
   polynomial.setPower(thePower);
}

double Polynomial::getCoefficient() const
{
   polynomial.getCoefficient();
}

void Polynomial::Add(Polynomial addPolynomial)
{
}

void Polynomial::Display() const
{
   polynomial.Display();
}
