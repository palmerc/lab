/*
 * LogicFunction.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTION_H_
#define LOGICFUNCTION_H_

#include "LogicFunctionADT.h"

/**
 * LogicFunction provides the ability to create objects that have a truth
 * table that defines a logic function like and, or, xor, and so on. This
 * class is now derived from LogicFunctionADT
 */
class LogicFunction : public LogicFunctionADT
{
public:
	/**
	 * The constructor
	 * @param Take a C-string name of the function
	 * @param Take an integer number that represents the number of inputs
	 * @param Take a C-string table of characters 't', 'f', and 'x'.
	 */
	LogicFunction(const char *name, int numinputs, const char **table);

	/**
	 * Getter for the table
	 * @return C-string const char** for the truth table
	 */
	const char** getTable() const;
	/**
	 * Setter for the table
	 * @param Takes a C-string const char** table of inputs
	 */
	void setTable(const char** table);
	/**
	 * calculate does all the work for any piece of logic. It determines
	 * if a set of inputs match an entry in the truth table and returns
	 * the result. If no match it returns an 'x'.
	 * @param Takes a C-string of inputs
	 * @return Returns the single character outcome of the evaluation 't', 'f', or 'x'
	 */
	char calculate(char *inputs) const;

	/**
	 * Test was formerly function_test. It calls LogicProcessor::test to try all
	 * 2^n permutations of logic input.
	 */
	void test();

private:
	const char **m_table;
};

#endif /* LOGICFUNCTION_H_ */
