/*
 * LogicFunction.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTION_H_
#define LOGICFUNCTION_H_

#include "LogicFunctionTable.h"

class LogicFunction : public LogicFunctionTable
{
public:
	LogicFunction(const char *name, int numinputs, const char **table);

	void test();
private:

};

#endif /* LOGICFUNCTION_H_ */
