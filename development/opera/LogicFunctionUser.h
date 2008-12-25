/*
 * LogicFunctionUser.h
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONUSER_H_
#define LOGICFUNCTIONUSER_H_

#include "LogicFunctionADT.h"

class LogicFunctionUser : public LogicFunctionADT
{
public:
	LogicFunctionUser();

	char calculate(char *inputs) const;

	void test();

private:
};

#endif /* LOGICFUNCTIONUSER_H_ */
