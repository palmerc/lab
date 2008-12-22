/*
 * LogicFunction.cpp
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#include "LogicFunction.h"

LogicFunction::LogicFunction(const char *name, int numinputs, const char **table):
	m_numinputs(numinputs), m_table(table)
{
	m_name = strdup(name);
	LogicFunctionList::insert(this);
}

LogicFunction::~LogicFunction()
{
	LogicFunctionList::remove(this);
}

LogicFunction *LogicFunction::findFunction(const char *name)
{
	return LogicFunctionList::find(name);
}

int LogicFunction::getNumInputs() const
{
	return m_numinputs;
}

char* LogicFunction::getName() const
{
	return m_name;
}

char LogicFunction::calculate(char *inputs)
{
	for (const char **t=m_table; *t ; t++)
	{
		int i;
		for (i=0; (*t)[i] == 'x' || inputs[i] == (*t)[i] ; )
		{
			if (++i == getNumInputs() )
				return (*t)[i];
		}
	}
	return 'x';
}
