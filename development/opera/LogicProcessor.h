/*
 * LogicProcessor.h
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#ifndef LOGICPROCESSOR_H_
#define LOGICPROCESSOR_H_

#include "LogicFunction.h"

class LogicProcessor {
	public:
		LogicProcessor( LogicFunction *function );
		~LogicProcessor();

		void setInput(int input, LogicProcessor *lf);
		void setInput(int input, char * source);

		char process();

		char **m_inputsources;
		LogicProcessor **m_inputfunctions;
		LogicFunction *m_logicfunction;
};

#endif /* LOGICPROCESSOR_H_ */
