/*
 * LogicProcessor.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICPROCESSOR_H_
#define LOGICPROCESSOR_H_

/**
 *
 */
class LogicProcessor {
public:
	/**
	 * Constructor
	 * @param Takes a LogicFunctionT pointer
	 * @return Returns a LogicProcessor object pointer
	 */
	LogicProcessor(LogicFunctionT *function);
	/**
	 * Destructor
	 */
	~LogicProcessor();

	/**
	 * setInput
	 * @param input is the value to change
	 * @param takes a LogicProcessor pointer
	 */
	void setInput(int input, LogicProcessor *lf);
	/**
	 * setInput
	 * @param input is the value to change
	 * @param the character pointer to set it to
	 */
	void setInput(int input, char* source);

	char process();

private:
	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	LogicFunctionT *m_logicfunction;
};

#endif /* LOGICPROCESSOR_H_ */
