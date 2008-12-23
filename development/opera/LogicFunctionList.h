/*
 * LogicFunctionList.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONLIST_H_
#define LOGICFUNCTIONLIST_H_

#include "LogicFunctionT.h"

/**
 * LogicFunctionList provides a linked list object that can contain pointers to logic functions
 * This LogicFunctionList class should be replaced with the STL List
 */
class LogicFunctionList {
public:
	/**
	 * Constructor that sets up an empty linked list
	 */
	LogicFunctionList(); // Default Constructor
	/**
	 * Destructor that tears down the linked list
	 */
	~LogicFunctionList(); // Destructor

	/**
	 * isEmpty() is a helper function that checks if the linked list is empty
	 * @return Returns a boolean indicating if the list is empty
	 */
	bool isEmpty() const;
	/**
	 * getLength() is a helper function that returns the length of the linked list
	 * @return Returns the length of the linked list
	 */
	int getLength() const;
	/**
	 * Insert takes a LogicFunctionT object pointer and adds it to the list
	 * @param Takes a LogicFunctionT object and adds it to the linked list
	 */
	void insert(LogicFunctionT *f);
	/**
	 * Remove takes a LogicFunctionT object pointer finds the item in the list and deletes it
	 * @param Takes a LogicFunctionT object
	 */
	void remove(LogicFunctionT *f);
	/**
	 * Locate a function by name in the list
	 * @param Takes a string name of a logic function
	 * @return Returns a pointer to the logic function or NULL if not found
	 */
	LogicFunctionT* find(const char* name) const;

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
