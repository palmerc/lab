#include<iostream>
#include "List.h"

int main()
{
   List L;
   while (1) {
   cout << "push" << endl;
   for (int i=1; i <= 10; i++)
   {
      L.push(i);
   }
   
   cout << "copy" << endl;
   List K(L);

   cout << "pop" << endl;
   for (int i=1; i <= 10; i++) {
      L.erase();
      cout << "Value " << K.pop() << endl;
      cout << i << endl;
   }
   
   cout << "erase" << endl;
   K.erase();
   //cout << "Done loading list" << endl;
   cout << "Start list size " << L.getSize() << endl;
   
   cout << "End list size " << L.getSize() << endl;
   }
   
   return 0;
}
