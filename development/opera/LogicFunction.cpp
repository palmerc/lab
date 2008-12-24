/*
 * LogicFunctionTable.cpp
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#include <iostream>
#include "LogicFunctionADT.h"
#include "LogicProcessor.h"
#include "LogicFunction.h"

/* CLASS: LogicFunction */
LogicFunction::LogicFunction(const char *name, int numinputs, const char **table)
{
	setName(name);
	setNumberInputs(numinputs);
	setTable(table);
}

LogicFunction::~LogicFunction()
{
}

const char** LogicFunction::getTable() const
{
	return m_table;
}

void LogicFunction::setTable(const char** table)
{
	m_table = table;
}

char LogicFunction::calculate(char *inputs) const
{
	// Move through the arrays strings representing a complete tt entry
	for (const char **t=getTable(); *t ; t++) //rows
	{
		int i;
		// if item is a don't care or the input matches the truth table entry...
		for (i=0; (*t)[i] == 'x' || inputs[i] == (*t)[i];) //cols
		{
			// if the input matches a truth table entry return the tt result
			if (++i == getNumberInputs())
				return (*t)[i];
		}
	}
	// Otherwise return don't care
	return 'x';
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
