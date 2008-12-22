/*
 * LogicFunction.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include "LogicFunction.h"

/* CLASS: LogicFunction */
LogicFunction::LogicFunction()
{
	list = new LogicFunctionList();
}

LogicFunction::~LogicFunction()
{
	delete list;
}

void LogicFunction::insert(const char *name, int numinputs, const char **table)
{
	list->insert(new LogicFunctionT::LogicFunctionT(name, numinputs, table));
}

void LogicFunction::remove(LogicFunctionT* f)
{
	list->remove(f);
}

LogicFunctionT* LogicFunction::find() const
{

}
