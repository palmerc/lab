/*
Cameron L Palmer
Problem 3

Instructions:
To compile: g++ -g -o num2 number2.cpp
To run: ./num2
*/

// number2.cpp

#include<iostream>
using namespace std;
int main()
{
   int n=10, sum=0;
   for (int i=0; i < n; i++)
      for (int j=0; j < n; j++)
         sum++;
   cout << "Sum is " << sum << endl;
   return 0;
}
