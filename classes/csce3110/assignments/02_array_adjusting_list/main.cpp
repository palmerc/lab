#include<iostream>
#include "AAList.h"
using namespace std;
int main()
{
   AAList L;
       
   cout << "insert test" << endl;
   for (int i=1; i <= 10; i++)
   {
      L.insert(i);
   }
   
   L.printAAList();
   
   cout << "finding a value" << endl;
   L.find(5);
   L.printAAList();
      
   return 0;
}
