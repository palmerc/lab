/*
 * LogicFunctionT.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONT_H_
#define LOGICFUNCTIONT_H_

class LogicFunctionT {
public:
	// inputs/output are char: 't','f','x'
	// tableentries are strings of numinputs inputs + resulting output
	LogicFunctionT(const char *name, int numinputs, const char **table);
	~LogicFunctionT();

	// Getters for private data members
	int getNumInputs() const;
	char* getName() const;
	const char** getTable() const;

	char calculate(char *inputs) const;

private:
	int m_numinputs;
	char *m_name;
	const char **m_table;
};

#endif /* LOGICFUNCTIONT_H_ */
