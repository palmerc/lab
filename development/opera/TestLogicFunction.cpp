/*
 * Test software
 */

#include <cstdio>
#include "LogicFunction.h"

const char *or2_table [] =
{
"txt",
"xtt",
"fff",
0
};

const char *or3_table [] =
{
"txxt",
"xtxt",
"xxtt",
"ffff",
0
};

const char *and2_table [] =
{
"ttt",
"fxf",
"xff",
0
};

const char *and3_table [] =
{
"tttt",
"fxxf",
"xfxf",
"xxff",
0
};

const char *xor2_table  [] =
{
"tft",
"ftt",
"fff",
"ttf",
0
};

const char *xor3_table  [] =
{
"fftt",
"ftft",
"tfft",
"tttt",
"ffff",
"fttf",
"ttff",
"tftf",
0
};

const char *impl_table [] =
{
"xtt",
"fxt",
"tff",
0
};

const char *not_table [] =
{
"tf",
"ft",
0
};

const char *incl_table [] =
{
"txxt",
0
};

/**
 * Print the results of a given truth table by iterating through
 * all the various permutations.
 */
void processor_test(LogicProcessor *proc, int n, char *inp)
{
	int i;

	// Initialize a all false set of inputs
	for (i=0;i<n;i++)
	{
		inp[i] = 'f';
	}

	bool done=false;
	do {
		// Print the inputs and the result
		for (i=0 ; i<n; i++)
			printf("%c ", inp[i]);
		printf(" -> %c\n", proc->process());

		// Iterate the various permutations of true and false - 2^n permutations
		for (i=0 ; i<n; i++)
		{
			if (inp[i] == 'f')
			{
				inp[i] = 't';
				break;
			}
			else
			{
				inp[i] = 'f';
			}
		}
		done = i==n;
	} while (!done);

}

/**
 * This function takes a LogicFunctionT and calls LogicProcessor
 * This sets up the char array for processor_test and deletes it
 */
void function_test ( LogicFunctionT *func )
{
	char *inp;
	char n=func->getNumInputs();

	// Setup the logic processor
	LogicProcessor proc(func);

	printf("Testing function: %s\n", func->getName());
	inp = new char[n];
	for (int i=0; i<n; i++)
	{
		proc.setInput(i, inp+i);
	}

	processor_test(&proc, n, inp);

	delete [] inp;
}



int main()
{
	// Uses the constructor to generate a bunch of LogicFunctionT objects
	LogicFunctionT
		f_not("not", 1, not_table),
		f_and2("and2", 2, and2_table),
		f_and3("and3", 3, and3_table),
		f_or2("or2", 2, or2_table),
		f_or3("or3", 3, or3_table),
		f_xor2("xor2", 2, xor2_table),
		f_xor3("xor3", 3, xor3_table),
		f_implies("implies", 2, impl_table);

	LogicFunctionT
		f_incomplete("incomplete",3, incl_table);

	// This tests my new wrapper version of LogicFunction.
	LogicFunction lf;
	lf.insert("not", 1, not_table);
	lf.insert("and2", 2, and2_table);
	lf.insert("and3", 3, and3_table);
	lf.remove("not");
	lf.remove("and6");

	// Run through each truth table of a given size, and try all permutations
	function_test(&f_not);
	function_test(&f_and2);
	function_test(&f_and3);
	function_test(&f_or2);
	function_test(&f_or3);
	function_test(&f_xor2);
	function_test(&f_xor3);
	function_test(&f_implies);

	function_test(&f_incomplete);


	// Combinatorial tests
	{
		printf("Testing combinatorial not (P and Q)\n");
		char inputs[2];
		// Create the LogicProcessor objects for each LogicFunctionT
		LogicProcessor p_not(&f_not),  p_and(&f_and2);
		// Let's combine two sets of logic
		p_and.setInput(0,inputs);
		p_and.setInput(1,inputs + 1);
		p_not.setInput(0,&p_and);

		// Evaluate the combined logic
		processor_test(&p_not, 2, inputs);
	}

	{
		// Basically the same as the previous combinatorial test but with more logic
		printf("Testing combinatorial P and not (Q or not R)\n");
		//  A && !(B || !C)
		char inputs[3];
		LogicProcessor p_not0(&f_not), p_not1(&f_not), p_or(&f_or2), p_and(&f_and2);
		p_not0.setInput(0,inputs+2);
		p_or.setInput(0,inputs+1);
		p_or.setInput(1,&p_not0);
		p_not1.setInput(0,&p_or);
		p_and.setInput(0,inputs);
		p_and.setInput(1, &p_not1);

		processor_test(&p_and, 3, inputs);
	}
}
