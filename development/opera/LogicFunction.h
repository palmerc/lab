/*
 * LogicFunction.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTION_H_
#define LOGICFUNCTION_H_

class LogicFunction {
public:
	LogicFunction();
	~LogicFunction();

	void insert(const char *name, int numinputs, const char **table);
	void remove(LogicFunctionT* f);
	LogicFunctionT* find() const;

private:
	LogicFunctionList* list;
};

#endif /* LOGICFUNCTION_H_ */
