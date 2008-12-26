/*
 * LogicFunctionSymmetry.h
 *
 *  Created on: Dec 25, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONSYMMETRY_H_
#define LOGICFUNCTIONSYMMETRY_H_

#include "LogicFunctionADT.h"
#include <ostream>
#include <string>
#include <vector>

using std::string;

/**
 * LogicFunctionSymmetry provides symmetry checking functionality by extending
 * the LogicFunctionADT.
 */
class LogicFunctionSymmetry : public LogicFunctionADT
{
public:
	/**
	 * The constructor. This is derived from LogicFunctionADT but the
	 * symmetry version LogicFunction doesn't check a truth table for
	 * valid inputs.
	 * @param Take a C-string name of the function
	 * @param Take an integer number of elements in the rows and columns and should be even.
	 * @param Take a C-string table of characters.
	 */
	LogicFunctionSymmetry(const char* name, int numinputs, const char **table);

	/**
	 * Getter for the table
	 * @return C-string const char** for the table
	 */
	const char** getTable() const;
	/**
	 * Setter for the table
	 * @param Takes a C-string const char** table of inputs
	 */
	void setTable(const char** table);
	/**
	 * This function is here to please the compiler it isn't used in this class
	 * @param Accepts a C-string char* input
	 * @return Will always return 'x'
	 */
	char calculate(char *inputs) const;

	/**
	 * Determines if a table has Horizontal symmetry
	 * @return A boolean true or false indicating symmetry or not
	 */
	bool hasHorizontalSymmetry() const;
	/**
	 * Determines if a table has Vertical symmetry
	 * @return A boolean true or false indicating symmetry or not
	 */
	bool hasVerticalSymmetry() const;
	/**
	 * Determines if a table has Rotational symmetry
	 * @return A boolean true or false indicating symmetry or not
	 */
	bool hasRotationalSymmetry() const;

	/**
	 * Overloaded the ostream << operator to print out the table.
	 */
	friend std::ostream& operator<<(std::ostream& os, LogicFunctionSymmetry lfs);

private:
	/**
	 * getCols returns the table in the form of a vector of column slices.
	 * This makes the symmetry calculations easier.
	 * @return A vector of strings, each element is one column
	 */
	std::vector<string> getCols() const;
	/**
	 * getRows returns the table in the form of a vector of row slices.
	 * This makes the symmetry calculations easier.
	 * @return A vector of strings, each element is one row
	 */
	std::vector<string> getRows() const;

	string m_table;

};

#endif /* LOGICFUNCTIONSYMMETRY_H_ */
