#include<iostream>
#include "List.h"

int main()
{
   List<int> L;
       
   cout << "push" << endl;
   for (int i=1; i <= 10; i++)
   {
      L.push(i);
   }
   
   cout << "copy" << endl;
   List<int> K(L);

   cout << "swap" << endl;
   for (int i=1; i < 10; i++)
   {
      K.swap(i,10-i);
   }
   
   K.printList();
   cout << "erase" << endl;
   K.erase();
   //cout << "Done loading list" << endl;
   cout << "Start list size " << L.getSize() << endl;
   
   cout << "End list size " << L.getSize() << endl;
      
   return 0;
}
