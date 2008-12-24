/*
 * LogicFunctionADT.cpp
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#include "LogicFunctionADT.h"

int LogicFunctionADT::getNumberInputs() const
{
	return numberInputs;
}
const char* LogicFunctionADT::getName() const
{
	return name;
}

void LogicFunctionADT::setNumberInputs(int n)
{
	numberInputs = n;
}
void LogicFunctionADT::setName(const char* _name)
{
	name = _name;
}
