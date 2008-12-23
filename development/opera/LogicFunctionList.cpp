/*
 * LogicFunctionList.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <cstddef> // Needed for null
#include <cstring> // Needed for strcmp
#include <iostream> // Needed for cerr
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
	//std::cerr << "DELETE: " << f->getName() << std::endl;
	LogicFunctionElm* prev = NULL;
	LogicFunctionElm* next = NULL;
	// This is sooo beautiful.
	for (LogicFunctionElm *elm=head; elm;)
	{
		next=elm->m_next;
		if (elm->m_function == f)
		{
			//std::cerr << "DELETING: " << elm->m_function->getName() << " " << f->getName() << " SIZE: " << getLength() << std::endl;
			--size;
			if (head == elm)
			{
				head = next;
			} else {
				prev->m_next = next;
			}
			delete elm;
		}
		prev = elm;
		elm = next;
	}
}

LogicFunctionT* LogicFunctionList::find(const char* name) const
{
	//std::cerr << "Looking for " << name << std::endl;
	for (LogicFunctionElm* elm=head; elm;)
	{
		if (0 == std::strcmp(name, elm->m_function->getName()) )
		{
			//std::cerr << name << " " << elm->m_function->getName() << std::endl;
			return elm->m_function;
		} else {
			 elm=elm->m_next;
		}
	}
	return NULL;
}
