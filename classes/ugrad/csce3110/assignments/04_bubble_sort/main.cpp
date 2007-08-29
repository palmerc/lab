/*
Cameron L Palmer
Bubblesort

Instructions:
To compile: g++ -g -o main main.cpp
To run: ./main <input file>
*/
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>

using namespace std;


int main(int argc, char*argv[])
{
   ifstream in;
   in.open(argv[1]);
   
   vector<int> A;
   string str;
   
   cout << "The input data:" << endl;
   getline(in, str);
   while (in)
   {
      stringstream ss(str);
      int temp;
      ss >> temp;
      A.push_back(temp);
      cout << temp << " ";
      getline(in, str);
   }
   cout << endl << endl;
      
   for (int i=0; i < A.size(); ++i)
      for (int j=A.size(); j >= i + 1; --j)
         if (A[j] < A[j - 1])
         {
            int temp;
            temp = A[j];
            A[j] = A[j - 1];
            A[j - 1] = temp;
         }
            
   cout << "The output data:" << endl;
   for (vector<int>::iterator i = A.begin(); i != A.end(); ++i)
      cout << *i << " ";
   cout << endl << endl;
   return 0;
}
