/*
 * LogicFunctionSymmetry.cpp
 *
 *  Created on: Dec 25, 2008
 *      Author: palmerc
 */

#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include "LogicFunctionADT.h"
#include "LogicFunctionSymmetry.h"

using std::ostream;
using std::string;

/* CLASS: LogicFunctionUser */
LogicFunctionSymmetry::LogicFunctionSymmetry(const char *name, int numinputs, const char **table)
{
	setName(name);
	setNumberInputs(numinputs);
	setTable(table);
}

LogicFunctionSymmetry::LogicFunctionSymmetry(string name, int numinputs, string table) : m_table(table)
{
	setName(name);
	setNumberInputs(numinputs);
}

const char** LogicFunctionSymmetry::getTable() const
{
	return (const char**) m_table.c_str();
}

vector<string> LogicFunctionSymmetry::getCols() const
{
	vector<string> cols;

	for (int i=0; i < getNumberInputs(); i++) //rows
	{
		string str = "";
		for (int j=0; j < getNumberInputs(); j++) //cols
		{
			str.append(1, m_table[i+(j*getNumberInputs())]);
		}
		cols.push_back(str);
	}
	return cols;
}

vector<string> LogicFunctionSymmetry::getRows() const
{
	vector<string> rows;

	for (int i=0; i < getNumberInputs(); i++)
	{
		rows.push_back(m_table.substr(i*getNumberInputs(), getNumberInputs()));
	}
	return rows;
}

void LogicFunctionSymmetry::setTable(const char** table)
{
	m_table = (string) *table;
}

char LogicFunctionSymmetry::calculate(char *inputs) const
{
	return 'x';
}

ostream& operator<<(ostream& os, LogicFunctionSymmetry lfs)
{
	int n = lfs.m_table.length();
	for (int i=0; i < n; i++)
	{
		os << lfs.m_table.at(i);
		if ((i+1) % lfs.getNumberInputs() == 0) os << std::endl;
	}
}

bool LogicFunctionSymmetry::hasHorizontalSymmetry() const
{
	vector<string> table = getCols();
	for (int i=0; i < table.size()/2; i++)
	{
		//std::cerr << i << ": " << table[i] << " == " << table[table.size()-(i+1)] << std::endl;
		if (not table[i].compare(table[table.size()-(i+1)]) == 0)
		{
			return false;
		}
	}
	return true;
}

bool LogicFunctionSymmetry::hasVerticalSymmetry() const
{
	vector<string> table = getRows();
	for (int i=0; i < table.size()/2; i++)
	{
		//std::cerr << i << ": " << table[i] << " == " << table[table.size()-(i+1)] << std::endl;
		if (not table[i].compare(table[table.size()-(i+1)]) == 0)
		{
			return false;
		}
	}
	return true;
}

bool LogicFunctionSymmetry::hasRotationalSymmetry() const
{
	if (hasVerticalSymmetry() && hasHorizontalSymmetry())
	{
		return true;
	} else {
		return false;
	}
}
