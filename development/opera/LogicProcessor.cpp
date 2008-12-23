/*
 * LogicProcessor.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <iostream>
#include <cstddef>
#include <string>
#include "LogicFunctionT.h"
#include "LogicProcessor.h"

/* CLASS: LogicProcessor */
LogicProcessor::LogicProcessor( LogicFunctionT *function )
	: m_logicfunction ( function )
{
	/**
	 * Changed these to use the new getter function getNumInputs() rather than directly
	 * accessing formerly public data member m_numinputs
	 */
	m_inputsources = new char* [ function->getNumInputs() ];
	m_inputfunctions = new LogicProcessor* [ function->getNumInputs() ];
	// Initialize m_inputsources and m_inputfunctions arrays.
	for (int i=0; i<function->getNumInputs(); i++)
	{
		m_inputsources[i] = NULL;
		m_inputfunctions[i] = NULL;
	}
}

LogicProcessor::~LogicProcessor()
{
	delete [] m_inputsources;
	delete [] m_inputfunctions;
}

void LogicProcessor::setInput(int input, LogicProcessor* lf)
{
	m_inputfunctions[input] = lf;
}

void LogicProcessor::setInput(int input, char* source)
{
	m_inputsources[input] = source;
}

char LogicProcessor::process()
{
	//std::cerr << "PROCESS " << std::endl;
	// Create a char array the size of the logic functions inputs
	char* inputs = new char [ m_logicfunction->getNumInputs() ];

	for (int i=0;i<m_logicfunction->getNumInputs();i++)
	{
		// +5 use of ternary operator
		//std::cerr << "i: " << i << ", IF " << m_inputsources[i] << std::endl;
		/**
		 * if (m_inputsources[i])
		 * 		inputs[i] = *m_inputsources[i]
		 * else
		 * 		if (m_inputfunctions[i])
		 * 			inputs[i] = m_inputfunctions[i]->process()
		 * 		else
		 * 			inputs[i] = 'x'
		 */
		// if (m_inputsources[i]) { inputs[i] = *m_inputsources[i] }

		inputs[i] =  m_inputsources[i] ? *m_inputsources[i] :
			m_inputfunctions[i] ? m_inputfunctions[i]->process() : 'x';
	}
	char output=m_logicfunction->calculate(inputs);
	delete [] inputs;
	return output;
}
