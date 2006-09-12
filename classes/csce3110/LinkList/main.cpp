#include "LinkList.h"
#include <iostream>

using namespace std;

int main()
{
   List L;
   int value;
 
   for (int index=1; index <= 50; index++)
      L.insert(1, index);
   
   for (int index=1; index <= L.getLength(); index++)
   {
      L.retrieve(index, value); 
      cout << "Value retrieved: " << value << endl;
   }

   List M(L);
 
   for (int index=1; index <= M.getLength(); index++)
   {
      M.retrieve(index, value); 
      cout << "Value copied: " << value << endl;
   }
   
   for (int index=1; index <= 25; index++)
   {
      M.remove(1); 
   }
   
   for (int index=1; index <= M.getLength(); index++)
   {
      M.retrieve(index, value); 
      cout << "Values remaining: " << value << endl;
   }

   return 0;
}
