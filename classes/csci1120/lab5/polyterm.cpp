#include "polyterm.h"
#include <iostream>

using namespace std;

PolyTerm::PolyTerm()
{
   theCoefficient = 1.0;
   thePower = 0;
}

PolyTerm::PolyTerm(double initialCoefficient, long initialPower)
{
   theCoefficient = initialCoefficient;
   thePower = initialPower;
}

void PolyTerm::setCoefficient(double newCoefficient)
{
   theCoefficient = newCoefficient;
}

void PolyTerm::setPower(long newPower)
{
   thePower = newPower;
}

double PolyTerm::getCoefficient() const
{
   return theCoefficient;
}

long PolyTerm::getPower() const
{
   return thePower;
}

void PolyTerm::Display() const
{
   cout << theCoefficient << "x^" << thePower << endl; 
}
