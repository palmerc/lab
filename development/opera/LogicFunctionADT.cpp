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

const char* LogicFunctionADT::getName() const
{
	return m_name.c_str();
}

void LogicFunctionADT::setNumberInputs(int n)
{
	numberInputs = n;
}

void LogicFunctionADT::setName(const char* name)
{
	m_name = (string) name;
}

void LogicFunctionADT::setName(string name)
{
	m_name = name;
}
