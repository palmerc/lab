/*
Cameron L Palmer
Quicksort

Instructions:
To compile: g++ -g -o main main.cpp
To run: ./main <input file>
*/
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>

using namespace std;

int partition(vector<int> &A, int p, int r)
{
   int x = A[r];
   int i = p - 1;
   for (int j = p; j <= r - 1; ++j)
      if (A[j] <= x)
      {
         ++i;
         int temp = A[i];
         A[i] = A[j];
         A[j] = temp;
      }
   int temp = A[i + 1];
   A[i + 1] = A[r];
   A[r] = temp;
   return ++i;
}

void quicksort(vector<int> &A, int p, int r)
{
   if (p < r)
   {
      int q = partition(A, p, r);
      quicksort(A, p, q - 1);
      quicksort(A, q + 1, r);
   }
}

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
      
   quicksort(A, 0, A.size());
            
   cout << "The output data:" << endl;
   for (vector<int>::iterator i = A.begin(); i != A.end(); ++i)
      cout << *i << " ";
   cout << endl << endl;
   return 0;
}

