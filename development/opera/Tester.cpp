/*
 * Tester.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

/**
 * This is the suite of tests provided by Opera, extended to support the changes
 * made to the rest of the software.
 *
 * Note: The testcases used in this program come from Tester.h and testcases.h
 */

#include <iostream>
#include <list>
#include <boost/lexical_cast.hpp>
#include "Tester.h"
#include "testcases.h"
#include "LogicFunction.h"
#include "LogicFunctionUser.h"
#include "LogicFunctionSymmetry.h"
#include "LogicProcessor.h"

using std::cout;
using std::endl;
using std::list;
using std::string;
using boost::lexical_cast;

int main()
{
	/**
	 * The original way to create a LogicFunction object. This hasn't changed.
	 */
	LogicFunction
		f_not("not", 1, not_table),
		f_and2("and2", 2, and2_table),
		f_and3("and3", 3, and3_table),
		f_or2("or2", 2, or2_table),
		f_or3("or3", 3, or3_table),
		f_xor2("xor2", 2, xor2_table),
		f_xor3("xor3", 3, xor3_table),
		f_implies("implies", 2, impl_table);

	LogicFunction
		f_incomplete("incomplete",3, incl_table);

	/**
	 * The tests were originally function_test(). It seemed easier just to stuff
	 * those in the class. Some would argue they don't belong there. I would
	 * argue in this case it doesn't matter too much. And it looks nicer here.
	 */
	// Run through each truth table of a given size, and try all permutations
	f_not.test();
	f_and2.test();
	f_and3.test();
	f_or2.test();
	f_or3.test();
	f_xor2.test();
	f_xor3.test();
	f_implies.test();
	f_incomplete.test();

	/**
	 * This is the test for combined logic using LogicProcessors.
	 * Not much changed except the processor_test code was moved into the
	 * processor for looks here.
	 */
	// Combinatorial tests
	{
		cout << "Testing combinatorial not (P and Q)" << endl;
		char inputs[2];
		// Create the LogicProcessor objects for each LogicFunction
		LogicProcessor p_not(&f_not),  p_and(&f_and2);
		// Let's combine two sets of logic
		p_and.setInput(0,inputs);
		p_and.setInput(1,inputs + 1);

		p_not.setInput(0,&p_and);

		// Evaluate the combined logic
		p_not.test(2, inputs);
	}

	{
		// Basically the same as the previous combinatorial test but with more logic
		cout << "Testing combinatorial P and not (Q or not R)" << endl;
		//  A && !(B || !C)
		char inputs[3];
		LogicProcessor p_not0(&f_not), p_not1(&f_not), p_or(&f_or2), p_and(&f_and2);
		p_not0.setInput(0,inputs+2);
		p_or.setInput(0,inputs+1);
		p_or.setInput(1,&p_not0);
		p_not1.setInput(0,&p_or);
		p_and.setInput(0,inputs);
		p_and.setInput(1, &p_not1);

		p_and.test(3, inputs);
	}

	{
		/**
		 * This is a demo of a simple 2 input user defined AND using the previous example
		 * as verification that user logic has no issues.
		 */
		LogicFunctionUser lfu_and2;
		// Basically the same as the previous combinatorial test but with more logic
		cout << "Testing combinatorial P and not (Q or not R) with User Defined AND2" << endl;
		//  A && !(B || !C)
		char inputs[3];
		LogicProcessor p_not0(&f_not), p_not1(&f_not), p_or(&f_or2), p_and((LogicFunction *)&lfu_and2);
		p_not0.setInput(0,inputs+2);
		p_or.setInput(0,inputs+1);
		p_or.setInput(1,&p_not0);
		p_not1.setInput(0,&p_or);
		p_and.setInput(0,inputs);
		p_and.setInput(1, &p_not1);

		p_and.test(3, inputs);
	}

	/**
	 * This is the symmetry testing code. The assignment seemed to assume the code would go
	 * into LogicProcessors, but it fit naturally into a user-defined LogicFunction.
	 * I used the STL list instead of the LogicFunctionList.
	 */
	cout << "Testing the symmetry of the series of tables" << endl;
	list <LogicFunctionSymmetry> llist;
	/**
	 * This for loop just reads the data from the provided data structure into the
	 * LogicFunctionSymmetry object and makes a list out of it.
	 */
	for (int i=0; i < sizeof(testcases)/sizeof(*testcases); i++) // 32 Grids
	{
		string f_name = "Case ";
		f_name.append(lexical_cast<string>(i));
		llist.push_back(LogicFunctionSymmetry(f_name.c_str(), 8, &testcases[i]));
	}

	list<LogicFunctionSymmetry>::iterator i;
	int count = 1;
	// The list is iterated through and each test for symmetry performed.
	for (i = llist.begin(); i != llist.end(); i++)
	{
		cout << i->getName() << endl;
		if (i->hasHorizontalSymmetry())
			cout << "Has horizontal symmetry." << endl;
		else
			cout << "Does not have horizontal symmetry." << endl;
		if (i->hasVerticalSymmetry())
			cout << "Has vertical symmetry." << endl;
		else
			cout << "Does not have vertical symmetry." << endl;
		if (i->hasRotationalSymmetry())
			cout << "Has rotational symmetry." << endl;
		else
			cout << "Does not have rotational symmetry." << endl;
		cout << *i;
		cout << endl;
		++count;
	}

	return 0;
}
