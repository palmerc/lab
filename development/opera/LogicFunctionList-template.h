/*
 * LogicFunctionList.h
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONLIST_H_
#define LOGICFUNCTIONLIST_H_

#include "ListException.h"
#include <cstddef> // for NULL
#include <cassert> // for assert()

template <typename T>
class LogicFunctionList {
	public:
		LogicFunctionList(); // Default Constructor
		~LogicFunctionList(); // Destructor

		void insert(T f)
			throw(ListException);
		void remove(T f)
			throw(ListException);

	private:
		struct LogicFunctionElm {
			T m_function;
			LogicFunctionElm* m_next;
			LogicFunctionElm* m_prev;
			LogicFunctionElm(const T& _m_function, LogicFunctionElm* _m_next, LogicFunctionElm* _m_prev):
				m_function(_m_function), m_next(_m_next), m_prev(_m_prev) { }
		};

		int size;
		LogicFunctionElm* head;
		LogicFunctionElm* find(const char* name) const;
		bool isEmpty() const;
		int getLength() const;
};

#endif /* LOGICFUNCTIONLIST_H_ */
