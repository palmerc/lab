/*
 * LogicFunctionSymmetry.h
 *
 *  Created on: Dec 25, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONSYMMETRY_H_
#define LOGICFUNCTIONSYMMETRY_H_

#include "LogicFunctionADT.h"
#include <ostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class LogicFunctionSymmetry : public LogicFunctionADT
{
public:
	LogicFunctionSymmetry(const char *name, int numinputs, const char **table);
	LogicFunctionSymmetry(string name, int numinputs, string table);

	const char** getTable() const;
	vector<string> getCols() const;
	vector<string> getRows() const;
	void setTable(const char** table);
	char calculate(char *inputs) const;

	bool hasHorizontalSymmetry() const;
	bool hasVerticalSymmetry() const;
	bool hasRotationalSymmetry() const;

	void test();
	friend ostream& operator<<(ostream& os, LogicFunctionSymmetry lfs);

private:
	string m_table;

};

#endif /* LOGICFUNCTIONSYMMETRY_H_ */
