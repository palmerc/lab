/*
 * LogicFunction.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTION_H_
#define LOGICFUNCTION_H_

#include "LogicFunctionT.h"
#include "LogicProcessor.h"
#include "LogicFunctionList.h"

/**
 * The LogicFunction class wraps the original functionality of the LogicFunctionT class
 * If you used the original LogicFunction class and want to continue to use the old
 * functionality use LogicFunctionT. LogicFunction now allows you to create, add, and
 * remove LogicFunctionT from a simple linked list
 */
class LogicFunction {
public:
	/**
	 * The constructor sets up the linked list
	 */
	LogicFunction();
	/**
	 * The destructor deletes the linked list
	 */
	~LogicFunction();

	/**
	 * Insert a new logic function
	 * @param name is string like "and" or "not"
	 * @param numinputs is the number of inputs the function has
	 * @param table is a truth table for the logic function
	 */
	void insert(const char *name, int numinputs, const char **table);
	/**
	 * Removes a logic function from the linked list
	 * @param Takes a LogicFunctionT object pointer
	 */
	void remove(LogicFunctionT* f);

	/**
	 * Removes a logic function from the linked list by name
	 * @param Takes a string name of a logic function
	 */
	void remove(const char* name);

	/**
	 * Find a logic function by name
	 * @param Takes a string name of a logic function
	 * @return Returns a pointer to the LogicFunctionT object
	 */
	LogicFunctionT* find(const char* name) const;

private:
	LogicFunctionList* list;
};

#endif /* LOGICFUNCTION_H_ */
