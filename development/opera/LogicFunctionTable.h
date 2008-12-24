/*
 * LogicFunctionTable.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONTABLE_H_
#define LOGICFUNCTIONTABLE_H_

#include "LogicFunctionADT.h"

class LogicFunctionTable : public LogicFunctionADT
{
public:
	LogicFunctionTable(const char *name, int numinputs, const char **table);
	~LogicFunctionTable();

	const char** getTable() const;

	void setTable(const char** table);

	char calculate(char *inputs) const;
private:
	const char **m_table;
};

#endif /* LOGICFUNCTIONTABLE_H_ */
