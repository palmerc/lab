/*
 * LogicProcessor.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICPROCESSOR_H_
#define LOGICPROCESSOR_H_

class LogicProcessor {
public:
	LogicProcessor(LogicFunctionT *function);
	~LogicProcessor();

	void setInput(int input, LogicProcessor *lf);
	void setInput(int input, char * source);

	char process();

	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	LogicFunctionT *m_logicfunction;
};

#endif /* LOGICPROCESSOR_H_ */
