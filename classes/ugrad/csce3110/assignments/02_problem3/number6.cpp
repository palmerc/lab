/*
Cameron L Palmer
Problem 3

Instructions:
To compile: g++ -g -o num6 number6.cpp
To run: ./num6
*/

// number6.cpp

#include<iostream>
using namespace std;
int main()
{
   int n=10, sum=0;
   for (int i=1; i < n; i++)
      for (int j=1; j < i*i; j++)
         if (j % 1 == 0)
            for (int k=0; k < j; k++)
               sum++;
   cout << "Sum is " << sum << endl;
   return 0;
}
