// Joe Student

#include <iostream>
#include <string>
#define CAN_DUMP // since we can cout integers
#include "sorted_list.h"

void test1(SortedList<int>& a)
{
	int i;
	cout << "TEST 1" << endl;

	i = 4; a.insert(i);
	i = 2; a.insert(i);
	i = 9; a.insert(i);
	i = 3; a.insert(i);
	a.dump();
	// 2 3 4 9

	a.remove();
	a.dump();
	//   3 4 9
	
	i = 5; a.insert(i);
	a.dump();
	// 3 4 5 9

	cout << endl;
}

void test3(SortedList<int>& a)
{
	cout << "TEST 3" << endl;

	for (int i=0; i < 128; i++) // to make the list grow many times
	{
		int j = i ^ 101;
		a.insert(j); // xor'd to make the list more interesting
	}
	
	a.dump();

	for (int i=0; i < 96; i++) // remove 3/4 of the list
		a.remove();
	
	a.dump();
	
	cout << endl;
}

// run all available tests
int main(int argc, char **argv) {
	SortedList<int> *a = NULL;
	
	for(int choice = 1; choice <= 3; choice++) {
		cout << "METHOD " << choice << endl << endl;
		if (choice == 1) a = new ArrayList<int>;
		else if (choice == 2) a = new LinkedList<int>;
		else if (choice == 3) a = new BSTList<int>;
	
		test1(*a);
		test3(*a);
		delete a;
	}
	
	return 0;
}

