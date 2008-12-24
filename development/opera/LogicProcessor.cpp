/*
 * LogicProcessor.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <iostream>
#include <cstddef>
#include <string>
#include "LogicFunction.h"
#include "LogicProcessor.h"

/* CLASS: LogicProcessor */
LogicProcessor::LogicProcessor(LogicFunction *function) : m_logicfunction(function)
{
	/**
	 * Changed these to use the new getter function getNumInputs() rather than directly
	 * accessing formerly public data member m_numinputs
	 */
	m_inputsources = new char* [ function->getNumberInputs() ];
	m_inputfunctions = new LogicProcessor* [ function->getNumberInputs() ];
	// Initialize m_inputsources and m_inputfunctions arrays.
	for (int i=0; i < function->getNumberInputs(); i++)
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

char LogicProcessor::process() const
{
	// Create a char array the size of the logic function's inputs
	char* inputs = new char [ m_logicfunction->getNumberInputs() ];

	for (int i=0; i < m_logicfunction->getNumberInputs(); i++)
	{
		// +5 use of ternary operator
		inputs[i] = m_inputsources[i] ? *m_inputsources[i] :
			m_inputfunctions[i] ? m_inputfunctions[i]->process() : 'x';
	}
	char output=m_logicfunction->calculate(inputs);
	delete [] inputs;
	return output;
}

/**
 * Print the results of a given truth table by iterating through
 * all the various permutations.
 */
void LogicProcessor::test(int n, char* inputs) const
{
	// Initialize inputs to false
	for (int i=0; i < n; i++)
	{
		inputs[i] = 'f';
	}

	bool done=false;
	int i;
	do {
		// Print the inputs and the result
		for (i=0; i < n; i++)
			std::cerr << inputs[i];
		std::cerr << " -> " << process() << std::endl;

		// Iterate the various permutations of true and false - 2^n permutations
		for (i=0 ; i < n; i++)
		{
			if (inputs[i] == 'f')
			{
				inputs[i] = 't';
				break;
			}
			else
			{
				inputs[i] = 'f';
			}
		}
		done = i==n;
	} while (!done);
}
