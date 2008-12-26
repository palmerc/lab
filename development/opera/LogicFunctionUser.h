/*
 * LogicFunctionUser.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONUSER_H_
#define LOGICFUNCTIONUSER_H_

#include "LogicFunctionADT.h"

/**
 * LogicFunctionUser demonstrates replacing a truth table with
 * C++ code as part of question 4. It is acting as a two input AND.
 */
class LogicFunctionUser : public LogicFunctionADT
{
public:
	/**
	 * The Construction just sets the name and inputs
	 */
	LogicFunctionUser();

	/**
	 * Calculate returns 'f' unless the two inputs are 't'
	 * @param A C-string of inputs to the LogicFunction
	 * @return A character either 't' or 'f' depending on the outcome.
	 */
	char calculate(char *inputs) const;

	/**
	 * The function_test from the original program. Just create an
	 * instance of this object and call test to try all 2^n permutations
	 * of inputs on this function.
	 */
	void test();

private:
};

#endif /* LOGICFUNCTIONUSER_H_ */
