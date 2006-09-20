#include<iostream>
#include "List.h"

int main()
{
   List<int> L;
   
   for (int i=0; i < 100; i++)
   {
      L.insert(i, i);
   }
   return 0;
}
