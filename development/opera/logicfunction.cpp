/* Logical function */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cstddef> // for NULL

#include "logicfunction.h"

/* CLASS: LogicFunction */
LogicFunction::LogicFunction()
{
	list = new LogicFunctionList();
}

LogicFunction::~LogicFunction()
{
	delete list;
}

void LogicFunction::insert(const char *name, int numinputs, const char **table)
{
	list->insert(new LogicFunctionT::LogicFunctionT(name, numinputs, table));
}

void LogicFunction::remove(LogicFunctionT* f)
{
	list->remove(f);
}

LogicFunctionT* LogicFunction::find() const
{

}

/* CLASS: LogicFunctionT */
LogicFunctionT::LogicFunctionT(const char *name, int numinputs, const char **table):
	m_numinputs(numinputs), m_table(table)
{
	m_name = strdup(name);
}

LogicFunctionT::~LogicFunctionT()
{
	free(m_name);
}

int LogicFunctionT::getNumInputs() const
{
	return m_numinputs;
}

char* LogicFunctionT::getName() const
{
	return m_name;
}

const char** LogicFunctionT::getTable() const
{
	return m_table;
}

char LogicFunctionT::calculate(char *inputs) const
{
	for (const char **t=m_table; *t ; t++) //rows
	{
		int i;
		for (i=0; (*t)[i] == 'x' || inputs[i] == (*t)[i];)
		{
			if (++i == m_numinputs)
				return (*t)[i];
		}
	}
	return 'x';
}

/* CLASS LogicFunctionList */
LogicFunctionList::LogicFunctionList(): size(0), head(NULL)
{
}

LogicFunctionList::~LogicFunctionList()
{
	while(!isEmpty())
	{
		//printf("isEmpty(): %d, %d\n", size, head->m_function);
		remove(head->m_function);
	}

	//printf("Is empty: %d\n", size);
}

bool LogicFunctionList::isEmpty() const
{
	return size == 0;
}

int LogicFunctionList::getLength() const
{
	return size;
}

void LogicFunctionList::insert(LogicFunctionT* f)
{
	struct LogicFunctionElm *oldhead=head;
	head = new struct LogicFunctionElm;
	if (head != NULL)
	{
		++size;
		head->m_function = f;
		head->m_next = oldhead;
	} else {
		std::cerr << "LogicFunctionList::insert unable to allocate memory." << std::endl;
	}
}

void LogicFunctionList::remove(LogicFunctionT* f)
{
	for (LogicFunctionElm *elm=head; elm;)
	{
		if ( elm->m_function == f)
		{
			//std::cerr << "DELETE: " << elm->m_function->getName() << " SIZE: " << getLength() << std::endl;
			--size;
			LogicFunctionElm *next = elm->m_next;
			delete elm;
			if (head == elm) head = next;
			elm = next;
		} else {
			 elm=elm->m_next;
		}
	}
}

LogicFunctionT *LogicFunctionList::find(const char *name) const
{
	for (LogicFunctionElm *elm=head; elm; elm=elm->m_next)
	{
		if (0 == strcmp(name, elm->m_function->getName()) )
		{
			return elm->m_function;
		}
	}
	return 0;
}

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

