/*
 * LogicFunctionADT.cpp
 *
 *  Created on: Dec 23, 2008
 *      Author: palmerc
 */

#include <string>
#include "LogicFunctionADT.h"

using std::string;

int LogicFunctionADT::getNumberInputs() const
{
	return numberInputs;
}

string LogicFunctionADT::getName() const
{
	return m_name;
}

void LogicFunctionADT::setNumberInputs(int n)
{
	numberInputs = n;
}

void LogicFunctionADT::setName(const char* name)
{
	m_name = name;
}

void LogicFunctionADT::setName(string name)
{
	m_name = name;
}
