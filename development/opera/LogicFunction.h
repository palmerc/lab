/*
 * LogicFunction.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTION_H_
#define LOGICFUNCTION_H_

#include "LogicFunctionADT.h"

class LogicFunction : public LogicFunctionADT
{
public:
	LogicFunction(const char *name, int numinputs, const char **table);
	~LogicFunction();

	const char** getTable() const;

	void setTable(const char** table);

	char calculate(char *inputs) const;

	void test();

private:
	const char **m_table;
};

#endif /* LOGICFUNCTION_H_ */
