/*
 * LogicFunctionT.h
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#ifndef LOGICFUNCTIONT_H_
#define LOGICFUNCTIONT_H_

/**
 * LogicFunctionT was formerly LogicFunction, this class has been simplified
 * to an abstract data type representing a logic function
 */
class LogicFunctionT {
public:
	/**
	 * The LogicFunctionT constructor
	 * @param name is a string to represent the logic function like "and" or "not"
	 * @param numinputs is the number of inputs contained in the truth table
	 * @param table is a string of characters, 't', 'f', and 'x', representing a truth table
	 *  Example: const char *impl_table [] = { "xtt", "fxt", "tff", 0 };
	 */
	LogicFunctionT(const char *name, int numinputs, const char **table);
	/**
	 * The LogicFunctionT deconstructor deletes the logic function
	 */
	~LogicFunctionT();

	/**
	 * Returns the number of inputs a logic function contains
	 * @return The number of inputs in the logic function
	 */
	int getNumInputs() const;
	/**
	 * Returns the string name that represents the logic function
	 * @return Name of the logic function as a C-string
	 */
	char* getName() const;
	/**
	 * Returns the truth table of the logic function
	 * @return A pointer to the truth table
	 */
	const char** getTable() const;

	/**
	 * Determines if a set of inputs match an entry in the truth table
	 * @param inputs is a string to check against the truth table
	 * @return returns the entry in the truth table or 'x' if no match
	 */
	char calculate(char *inputs) const;

private:
	/**
	 * The original components of a LogicFunctionT were public, and changed to private
	 * to enforce data protection.
	 */
	int m_numinputs;
	char *m_name;
	const char **m_table;
};

#endif /* LOGICFUNCTIONT_H_ */
