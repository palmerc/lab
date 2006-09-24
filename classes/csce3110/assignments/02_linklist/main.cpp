#include<iostream>
#include "List.h"

int main()
{
   List<int> L;
   
   for (int i=1; i <= 100; i++)
   {
      L.insert(1, i);
   }
   return 0;
}
