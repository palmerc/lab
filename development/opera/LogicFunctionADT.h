/*
 * LogicFunctionADT.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONADT_H_
#define LOGICFUNCTIONADT_H_

#include <string>
using std::string;

/**
 * The LogicFunctionADT with a virtual function.
 * This is how I provided user-defined logic. LogicFunctions have a name and
 * number of inputs but the calculate function really does the work.
 */
class LogicFunctionADT
{
public:
	/**
	 * The calculate function needs to be implemented in the derived class
	 * @param Takes a set of inputs in the form of a string
	 * @return Returns the result as a char
	 */
    virtual char calculate(char* inputs) const = 0;

    /**
     * Getter for the number of input
     * @return Returns the integer indicating number of inputs
     */
    int getNumberInputs() const;
    /**
     * Getter for the name given the function
     * @return Returns a C string name for the function
     */
    string getName() const;

    /**
     * Set the number of inputs the function has
     * @param Integer number of inputs
     */
    void setNumberInputs(int n);
    /**
     * Set the name of the function using a C-string
     * @param Takes a C-string const char* name for the function
     */
    void setName(const char* name);
    /**
     * Set the name of the function using a C++ string
     * @param Takes a C++ string name for the function
     */
    void setName(string name);


private:
	int numberInputs;
	string m_name;
};

#endif /* LOGICFUNCTIONADT_H_ */
