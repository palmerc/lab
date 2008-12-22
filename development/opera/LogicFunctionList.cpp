/*
 * LogicFunctionList.cpp
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#include "LogicFunctionList.h"

LogicFunctionList::LogicFunctionList(): size(0), head(NULL)
{
}

LogicFunctionList::~LogicFunctionList()
{
	while(!isEmpty())
		remove(head->m_function);
}

bool LogicFunctionList::isEmpty() const
{
	return size == 0;
}

int LogicFunctionList::getLength() const
{
	return size;
}

LogicFunctionList::LogicFunctionElm *LogicFunctionList::find(const char* name) const
{
	for (LogicFunctionElm *elm=head; elm; elm=elm->m_next)
	{
		if (elm->m_name == name)
		{
			return elm;
		}
	}
	return NULL;
}

// Always inserts the new logic function at the head
void LogicFunctionList::insert(LogicFunction* f)
{
	int newLength = getLength() + 1;

	LogicFunctionElm newElm = new LogicFunctionElm;
	if (newElm == NULL) {
		throw ListException("ListException: insert cannot allocate memory");
	} else {
		size = newLength;
		newElm->m_function = f;
		newElm->m_next = head;
		head->m_prev = newElm;
		newElm->m_prev = NULL;
		head = newElm;
	}
}

void LogicFunctionList::remove(LogicFuncton* f)
{
	if (f == NULL) {
		throw ListException("ListException: null pointer");
	} else if (f == head) // The element is the head of the list
	{
		if (size > 0) {
			// Change the next items previous element to null
			f->m_next->m_prev = NULL;
		}
		// Assign the head of the list the next element
		head = f->m_next;
	} else {
		f->m_prev->m_next = f->m_next;
		// Assign the current previous element to the next elements previous element
		f->m_next->m_prev = f->m_prev;
	}
	--size;
	delete f;
}

void LogicFunctionList::remove(const char* name)
{
	LogicFunctionElm *elm = find(name);
	remove(elm);
}
