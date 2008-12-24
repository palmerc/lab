/*
 * LogicFunction.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <iostream>
#include "LogicFunction.h"
#include "LogicFunctionTable.h"
#include "LogicProcessor.h"

/* CLASS: LogicFunction */
LogicFunction::LogicFunction(const char* name, int numinputs, const char** table) : LogicFunctionTable(name, numinputs, table)
{
}

void LogicFunction::test()
{
	char *inp;
	int n = getNumberInputs();

	// Setup the logic processor
	LogicProcessor proc(this);

	std::cerr << "Testing function: " << getName() << std::endl;
	inp = new char[n];
	for (int i=0; i<n; i++)
	{
		proc.setInput(i, inp+i);
	}
	proc.test(n, inp);
	delete [] inp;
}
