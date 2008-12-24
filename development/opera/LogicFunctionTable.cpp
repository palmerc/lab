/*
 * LogicFunctionTable.cpp
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#include "LogicFunctionADT.h"
#include "LogicFunctionTable.h"

/* CLASS: LogicFunctionTable */
LogicFunctionTable::LogicFunctionTable(const char *name, int numinputs, const char **table)
{
	setName(name);
	setNumberInputs(numinputs);
	setTable(table);
}

LogicFunctionTable::~LogicFunctionTable()
{
}

const char** LogicFunctionTable::getTable() const
{
	return m_table;
}

void LogicFunctionTable::setTable(const char** table)
{
	m_table = table;
}

char LogicFunctionTable::calculate(char *inputs) const
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
