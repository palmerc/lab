/*
 * LogicFunctionList.h
 *
 *  Created on: Dec 20, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONLIST_H_
#define LOGICFUNCTIONLIST_H_

#include "LogicFunction.h"
#include "ListException.h"
#include <cstddef> // for NULL

// This class inserts and remove LogicFunction objects into a linked-list
class LogicFunctionList {
	public:
		LogicFunctionList(); // Default Constructor
		~LogicFunctionList(); // Destructor

		bool isEmpty() const;
		int getLength() const;
		void insert(LogicFunction *f);
		void remove(LogicFunction *f);

	private:
		struct LogicFunctionElm {
			LogicFunction* m_function;
			LogicFunctionElm* m_next;
			LogicFunctionElm* m_prev;
		};

		int size;
		LogicFunctionElm *head;
		LogicFunction* find(const char *name) const;
};

#endif /* LOGICFUNCTIONLIST_H_ */
