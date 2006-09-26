#include<iostream>
#include<cmath>
using namespace std;
bool isPrime(int x)
{
   if (x < 2)
      return true;
   for (int i=2; i <= sqrt(x); i++)
      if (x % i == 0)
         return false;
   return true;
}

int main(int argc, char* argv[])
{
   int x = atoi(argv[1]);

   if (isPrime(x))
      cout << x << " is prime." << endl;
   else
      cout << x << " is not prime." << endl;
      
   return 0;
}
