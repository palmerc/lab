/*
 * LogicProcessor.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include "LogicFunctionT.h"
#include "LogicProcessor.h"

/* CLASS: LogicProcessor */
LogicProcessor::LogicProcessor( LogicFunctionT *function )
	: m_logicfunction ( function )
{
	m_inputsources = new char * [ function->getNumInputs() ];
	m_inputfunctions = new LogicProcessor * [ function->getNumInputs() ];
	for (int i=0; i<function->getNumInputs(); i++)
	{
		m_inputsources[i] = 0;
		m_inputfunctions[i] = 0;
	}
}

LogicProcessor::~LogicProcessor()
{
	delete [] m_inputsources;
	delete [] m_inputfunctions;
}

void LogicProcessor::setInput(int input, LogicProcessor *lf)
{
	m_inputfunctions[input] = lf;
}

void LogicProcessor::setInput(int input, char * source)
{
	m_inputsources[input] = source;
}

char LogicProcessor::process()
{
	char *inputs = new char [ m_logicfunction->getNumInputs() ];

	for (int i=0;i<m_logicfunction->getNumInputs();i++)
	{
		inputs[i] =  m_inputsources[i] ? *m_inputsources[i] :
			m_inputfunctions[i] ? m_inputfunctions[i]->process() : 'x';
	}
	char output=m_logicfunction->calculate(inputs);
	delete [] inputs;
	return output;
}
