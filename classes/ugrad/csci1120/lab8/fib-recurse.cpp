#include <iostream>
#include <fstream>
 
using namespace std;
 
int fib(int n)
{
   if ( n <= 1 )
   {
      return n;
   } else {
      return fib(n-1) + fib(n-2);
   }
}
 
int main()
{
   int x;
 
   x = fib(45);

   cout << x << endl;
 
   return 0;
}

