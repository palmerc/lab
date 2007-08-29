/*
Cameron L Palmer
Problem 3

Instructions:
To compile: g++ -g -o num4 number4.cpp
To run: ./num4
*/

// number4.cpp

#include<iostream>
using namespace std;
int main()
{
   int n=10, sum=0;
   for (int i=0; i < n; i++)
      for (int j=0; j < i; j++)
         sum++;
   cout << "Sum is " << sum << endl;
   return 0;
}
