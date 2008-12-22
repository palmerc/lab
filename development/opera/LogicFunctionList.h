/*
 * LogicFunctionList.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONLIST_H_
#define LOGICFUNCTIONLIST_H_

class LogicFunctionList {
public:
	LogicFunctionList(); // Default Constructor
	~LogicFunctionList(); // Destructor

	bool isEmpty() const;
	int getLength() const;
	void insert(LogicFunctionT *f);
	void remove(LogicFunctionT *f);
	LogicFunctionT *find(const char *name) const;

private:
	struct LogicFunctionElm {
		LogicFunctionT *m_function;
		struct LogicFunctionElm *m_next;
	~LogicFunctionElm() { delete m_function; }
	};

	struct LogicFunctionElm *head;
	int size;
};

#endif /* LOGICFUNCTIONLIST_H_ */
