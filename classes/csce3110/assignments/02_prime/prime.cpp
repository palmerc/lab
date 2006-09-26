#include<iostream>
#include<cmath>
#include <ctime>

using namespace std;

bool isPrime(unsigned long int x)
{
   if (x < 2)
      return true;
   for (unsigned long int i=2; i <= sqrt(x); i++)
      if (x % i == 0)
         return false;
   return true;
}

int main(int argc, char* argv[])
{
   //unsigned long int x = 1020304056789;
   unsigned long int x = atoi(argv[1]);
   clock_t start, finish;
   double time;
   
   start = clock();
   if (isPrime(x))
      cout << x << " is prime." << endl;
   else
      cout << x << " is not prime." << endl;
   finish = clock();
   time = (double(finish)-double(start))/CLOCKS_PER_SEC;
   cout << "Calculated in " << time << " seconds." << endl;
   return 0;
}
