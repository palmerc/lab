#include <iostream>
using namespace std;

// This program reads in 10 values from standard input
// and finds the average.

int ZeroArray(int v[], int num_values) {
   for ( int i = 0 ; i < num_values ; i++ )
       v[i] = 0;
}

int ReadArray(int v[], int num_values) {
   for ( int i = 0 ; i < num_values ; i++ ) {
      cout << i << ">";
      cin >> v[i];
   }
}

int SumArray(int v[], int num_values) {
   int sum;
   for ( int i = 0 ; i < num_values ; i++ )
      sum = sum + v[i];
   return sum;
}

int main() {

   int num_grades;
   int values[10];
   int sum;

  num_grades = 10;

  ZeroArray(values,10);

  ReadArray(values,10);

  sum = SumArray(values,10);

  cout << "Hello Cameron Palmer" << endl;
  cout << endl << "Average: " << sum / num_grades << endl;

  return 0;
}
