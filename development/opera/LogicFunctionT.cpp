/*
 * LogicFunctionT.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <cstdlib>
#include <cstring>
#include "LogicFunctionT.h"

/* CLASS: LogicFunctionT */
LogicFunctionT::LogicFunctionT(const char *name, int numinputs, const char **table):
	m_numinputs(numinputs), m_table(table)
{
	m_name = strdup(name);
}

LogicFunctionT::~LogicFunctionT()
{
	std::free(m_name);
}

int LogicFunctionT::getNumInputs() const
{
	return m_numinputs;
}

char* LogicFunctionT::getName() const
{
	return m_name;
}

const char** LogicFunctionT::getTable() const
{
	return m_table;
}

char LogicFunctionT::calculate(char *inputs) const
{
	// Move through the arrays strings representing a complete tt entry
	for (const char **t=m_table; *t ; t++) //rows
	{
		int i;
		// if item is a don't care or the input matches the truth table entry...
		for (i=0; (*t)[i] == 'x' || inputs[i] == (*t)[i];) //cols
		{
			// if the input matches a truth table entry return the tt result
			if (++i == m_numinputs)
				return (*t)[i];
		}
	}
	// Otherwise return don't care
	return 'x';
}
