/* Logical function */
#ifndef LOGICFUNCTION_H
#define LOGICFUNCTION_H

class LogicFunctionT;
class LogicFunctionList;

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

class LogicFunctionT {
public:
	// inputs/output are char: 't','f','x'
	// tableentries are strings of numinputs inputs + resulting output
	LogicFunctionT(const char *name, int numinputs, const char **table);
	~LogicFunctionT();

	// Getters for private data members
	int getNumInputs() const;
	char* getName() const;
	const char** getTable() const;

	char calculate(char *inputs) const;

private:
	int m_numinputs;
	char *m_name;
	const char **m_table;
};

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
	};

	struct LogicFunctionElm *head;
	int size;
};

class LogicProcessor {
public:
	LogicProcessor( LogicFunctionT *function );
	~LogicProcessor();

	void setInput(int input, LogicProcessor *lf);
	void setInput(int input, char * source);

	char process();

	char **m_inputsources;
	LogicProcessor **m_inputfunctions;
	LogicFunctionT *m_logicfunction;
};

#endif // LOGICFUNCTION_H
