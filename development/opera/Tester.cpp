/*
 * Tester.cpp
 *
 *  Created on: Dec 21, 2008
 *      Author: palmerc
 */

#include <iostream>
#include <list>
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

int main()
{
	// Uses the constructor to generate a bunch of LogicFunction objects
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

	// Combinatorial tests
	{
		std::cout << "Testing combinatorial not (P and Q)" << std::endl;
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
		std::cout << "Testing combinatorial P and not (Q or not R)" << std::endl;
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
		LogicFunctionUser lfu_and2;
		// Basically the same as the previous combinatorial test but with more logic
		std::cout << "Testing combinatorial P and not (Q or not R) with User Defined AND2" << std::endl;
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

	cout << "Testing the symmetry of the series of tables" << endl;
	list <LogicFunctionSymmetry> llist;
	for (int i=0; i < sizeof(testcases)/sizeof(*testcases); i++) // 32 Grids
	{
		//std::string tmp = testcases[i];
		//llist.push_back(LogicFunctionSymmetry(tmp));
		string f_name = "Case " + i;
		llist.push_back(LogicFunctionSymmetry(f_name.c_str(), 8, &testcases[i]));
	}

	list<LogicFunctionSymmetry>::iterator i;
	int count = 1;
	for (i = llist.begin(); i != llist.end(); i++)
	{
		cout << "Case " << count << endl;
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
