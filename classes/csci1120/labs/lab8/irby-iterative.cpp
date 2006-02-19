#include "cQueueP.h"
#include <iostream>
#include <fstream>
 
using namespace std;
 
int I(int n, int k)
{
   cQueueP q;
   bool success;
   int rvalue1, rvalue2, total;
 
   if ( k <= 2 )
      return 0;
 
   if ( n < k )
      return n+k;
 
   for ( int i = 1; i <= k; i++ )
      q.QueueInsert(i+k, success);
 
   for ( int i = k+1; i <= n; i++ )
   {
      q.QueueDelete( rvalue1, success );
      q.GetQueueFront( rvalue2, success );
      total = rvalue1 + rvalue2;
      q.QueueInsert( total, success );
   }
   return total;
}
 
int main()
{
   int n, k;
 
   cout << "Irby numbers generator" << endl;
   cout << "Please enter n value: ";
   cin >> n;
   cout << "Please enter k value: ";
   cin >> k;
 
   cout << "The total is " << I(n, k) << endl;
 
   return 0;
}

