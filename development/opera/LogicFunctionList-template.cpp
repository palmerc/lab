/*
 * LogicFunctionList.cpp
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#include "LogicFunctionList.h"

template<typename LISTITEMTYPE>
LogicFunctionList<T>::LogicFunctionList(): size(0), head(NULL)
{
}

template<typename T>
LogicFunctionList<T>::~LogicFunctionList()
{
	while(!isEmpty())
		remove(head->m_function);
}

template<typename T>
bool LogicFunctionList<T>::isEmpty() const
{
	return size == 0;
}

template<typename T>
int LogicFunctionList<T>::getLength() const
{
	return size;
}

template<typename T>
LogicFunctionList<T>::LogicFunctionElm *LogicFunctionList::find(const char* name) const
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
template<typename T>
void LogicFunctionList<T>::insert(Object f)
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

template<typename T>
void LogicFunctionList<T>::remove(Object f)
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

template<typename T>
void LogicFunctionList<T>::remove(const char* name)
{
	LogicFunctionElm *elm = find(name);
	remove(elm);
}
