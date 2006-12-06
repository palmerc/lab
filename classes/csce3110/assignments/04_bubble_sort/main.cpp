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
   
   getline(in, str);
   while (in)
   {
      stringstream ss(str);
      int temp;
      ss >> temp;
      A.push_back(temp);
      //cout << "input " << temp << endl;
      getline(in, str);
   }
      
   for (int i=0; i < A.size(); ++i)
      for (int j=A.size(); j >= i + 1; --j)
         if (A[j] < A[j - 1])
         {
            int temp;
            temp = A[j];
            A[j] = A[j - 1];
            A[j - 1] = temp;
         }
            
   for (vector<int>::iterator i = A.begin(); i != A.end(); ++i)
      cout << *i << endl;
   return 0;
}
