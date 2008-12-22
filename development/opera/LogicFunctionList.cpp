/*
 * LogicFunctionList.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <cstddef> // Needed for null
#include <cstring>
#include <iostream>
#include "LogicFunctionT.h"
#include "LogicFunctionList.h"

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
		if (0 == std::strcmp(name, elm->m_function->getName()) )
		{
			return elm->m_function;
		}
	}
	return 0;
}
