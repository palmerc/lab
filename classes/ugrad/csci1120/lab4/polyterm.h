class PolyTerm
{
  public:
    PolyTerm();

    PolyTerm(double initialCoefficient, long initialPower);
    
    void setCoefficient(double newCoefficient);
    
    void setPower(long newPower);
    
    double getCoefficient() const;

    long getPower() const;
    
    void Display() const;
    
  private:
    double theCoefficient;
    long thePower;
};

// The class should have the following public member functions:
// A constructor that sets the coefficient to 1.0 and the power to 0.
// A constructor that sets the coefficient and power by its arguments.
// Access functions Coefficient, Power, SetCoefficient and SetPower.
// A function called Display that will output the term. 
