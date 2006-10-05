/*
Cameron L Palmer
Iterative Fibonacci

Instructions:
To compile: g++ -g -o fib1 fibonacci1.cpp
To run automatically: sh fib1.sh
or
./fib1 16
*/

// fibonacci1.cpp
// The iterative version
#include<iostream>
#include<ctime>
#include<stdlib.h>

using namespace std;

unsigned long int loop(unsigned long int a) {
   unsigned long int total = 0, previous = 1, current = 0;
   if (a == 0)
      return 0;
   else {
      for (unsigned long int i=0; i < a; ++i) {
         total = current + previous;
         previous = current;
         current = total;
      }
      return total;
   }
}
int main(int argc, char* argv[]) {
   unsigned long int a, b;
   clock_t start, finish;
   double time;
   
   if (argc > 1)
      a = atoi(argv[1]);
   
   start = clock();
   b = loop(a);
   finish = clock();
   time = (double(finish)-double(start))/CLOCKS_PER_SEC;
   
   cout << "Fibonacci number " << a << " is " << b << " in " << time << " seconds." << endl;
}
