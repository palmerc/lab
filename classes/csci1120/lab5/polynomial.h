#include "listp.h"
#include "polyterm.h"

class Polynomial
{
  public:
    Polynomial();
    ~Polynomial();

    void setCoefficient(long thePower, double newCoefficient);
    void Add(Polynomial addPolynomial);
    void getCoefficient() const;
    void Display() const;
    
  private:
    listClass listPolynomial;
};
