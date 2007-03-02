#include <iostream>
#include <string>
#define CAN_DUMP // since we can cout integers
#define DEBUG
#include "sorted_list.h"

void test_ll()
{
	int top = 12;
	AVLList<int> a;
	cout << "ll" << endl;
	for (int i = top; i >= 0; i--)
		a.insert2(i);
	a.dump();
	for (int i = 0; i <= top; i++) a.remove();
	a.dump();
	cout << "done" << endl << endl;
}

void test_rr()
{
	int top = 12;
	AVLList<int> a;
	cout << "rr" << endl;
	for (int i = 0; i <= top; i++)
		a.insert2(i);
	a.dump();
	for (int i = 0; i <= top; i++) a.remove();
	a.dump();
	cout << "done" << endl << endl;
}

void test_lr()
{
	AVLList<int> a;
	cout << "lr" << endl;
	a.insert2(12);
	a.insert2(9);
	a.insert2(11);
	a.insert2(4);
	a.insert2(8);
	a.insert2(10);
	a.insert2(5);
	a.insert2(2);
	a.insert2(3);
	a.insert2(6);
	a.insert2(7);
	a.insert2(13);
	a.insert2(0);
	a.insert2(1);
	a.dump();
	for (int i = 0; i < 14; i++) a.remove();
	a.dump();
	cout << "done" << endl << endl;
}

void test_rl()
{
	AVLList<int> a;
	cout << "rl" << endl;
	a.insert2(0);
	a.insert2(3);
	a.insert2(1);
	a.insert2(8);
	a.insert2(4);
	a.insert2(2);
	a.insert2(7);
	a.insert2(10);
	a.insert2(9);
	a.insert2(6);
	a.insert2(5);
	a.insert2(-1);
	a.insert2(12);
	a.insert2(11);
	a.dump();
	for (int i = 0; i < 14; i++) a.remove();
	a.dump();
	cout << "done" << endl << endl;
}

void test_random()
{
	int top = 5000;
	AVLList<int> a;
	cout << "random" << endl;
	for (int i = 0; i < top; i++) a.insert2(rand());
	a.dump();
	for (int i = 0; i < top; i++) a.remove();
	a.dump();
	cout << "done" << endl << endl;
}

// run all available tests
int main() {

	test_ll();
	test_rr();
	test_lr();
	test_rl();
	
	srand(time(NULL));
	test_random();

	return 0;
}

