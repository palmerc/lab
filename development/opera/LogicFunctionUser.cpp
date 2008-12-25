/*
 * LogicFunction.cpp
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#include <iostream>
#include "LogicFunctionADT.h"
#include "LogicFunction.h"
#include "LogicProcessor.h"
#include "LogicFunctionUser.h"

/* CLASS: LogicFunctionUser */
LogicFunctionUser::LogicFunctionUser()
{
	setNumberInputs(10);
	setName("and10");
}

char LogicFunctionUser::calculate(char *inputs) const
{
	int i;
	for (i=0; i < 10; i++)
	{
		if (inputs[i] != 't')
			return 'f';

	}
	return 't';
}

void LogicFunctionUser::test()
{
	char *inp;
	int n = getNumberInputs();

	// Setup the logic processor
	LogicProcessor proc((LogicFunction*) this);

	std::cerr << "Testing function: " << getName() << std::endl;
	inp = new char[n];
	for (int i=0; i<n; i++)
	{
		proc.setInput(i, inp+i);
	}
	proc.test(n, inp);
	delete [] inp;
}
