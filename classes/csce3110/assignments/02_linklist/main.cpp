#include<iostream>
#include "List.h"

int main()
{
   List L;
       
   cout << "push test" << endl;
   for (int i=1; i <= 10; i++)
   {
      L.push(i);
   }
   
   L.printList();
   cout << "copy constructor test" << endl;
   List K(L);

   cout << "swap test" << endl;
   for (int i=1; i < 10; i+=2)
   {
      K.swap(i,i+1);
   }
   
   K.printList();
   
   cout << "K Start list size " << K.getSize() << endl;
   for (int i=1; i <= 10; i++)
   {
      cout << "K.pop() " << K.pop() << endl;
   }
   cout << "K End list size " << K.getSize() << endl;
   
   cout << "shift test" << endl;
   
   for (int i=1; i <= 10; i++)
   {
      L.shift(10-i);
   }
   L.printList();
   
   cout << "unshift test" << endl;
   for (int i=1; i <= 10; i++)
   {
      L.unshift();
   }
   L.printList();
   
   //cout << "Done loading list" << endl;
   cout << "Start list size " << L.getSize() << endl;
   
   cout << "End list size " << L.getSize() << endl;
      
   return 0;
}
