#include <iostream>
#include "min-q.h"

using std::cout;
using std::endl;

int main()
{
   int i;
   Heap heapy;
   
   heapy.min_heap_insert(15);
   heapy.min_heap_insert(13);
   heapy.min_heap_insert(9);
   heapy.min_heap_insert(5);
   heapy.min_heap_insert(12);
   heapy.min_heap_insert(8);
   heapy.min_heap_insert(7);
   heapy.min_heap_insert(4);
   heapy.min_heap_insert(0);
   heapy.min_heap_insert(6);
   heapy.min_heap_insert(3);
   heapy.min_heap_insert(2);
   heapy.min_heap_insert(1);

   for (i = 0; i < 12; ++i)
   {
      cout << "Min " << heapy.heap_extract_min() << endl;
   }

   return 0;
}
