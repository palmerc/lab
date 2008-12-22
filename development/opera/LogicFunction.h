/*
 * LogicFunction.h
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTION_H_
#define LOGICFUNCTION_H_

#include "LogicFunctionList.h"
#include "LogicProcessor.h"
#include <string.h>

class LogicFunction {
	public:
		// inputs/output are char: 't','f','x'
		// tableentries are strings of numinputs inputs + resulting output
		LogicFunction(const char *name, int numinputs, const char **table);
		~LogicFunction();

		int getNumInputs() const;
		char* getName() const;
		char calculate(char *inputs);

	private:
		static LogicFunction *findFunction(const char *name);

		const char *m_name;
		int m_numinputs;
		const char **m_table;
};

#endif /* LOGICFUNCTION_H_ */
