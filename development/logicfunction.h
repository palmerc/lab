/* Logical function */
#ifndef LOGICFUNCTION_H
#define LOGICFUNCTION_H


class LogicFunction {
public:

	// inputs/output are char: 't','f','x'
	// tableentries are strings of numinputs inputs + resulting output
	LogicFunction(const char *name, int numinputs, const char **table);
	~LogicFunction();

	static LogicFunction *findFunction(const char *name);

	char calculate(char *inputs);

	int m_numinputs;
	char *m_name;
	const char **m_table;
};


class LogicProcessor {
public:
	LogicProcessor( LogicFunction *function );
	~LogicProcessor();

	void setInput(int input, LogicProcessor *lf);
	void setInput(int input, char * source);

	char process();

	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	LogicFunction *m_logicfunction;
};

#endif // LOGICFUNCTION_H
