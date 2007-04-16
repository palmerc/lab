#include <iostream>
#include "Heap.h"

using std::cout;
using std::endl;

int main()
{
   int i;
   Heap heapy;
   
   heapy.max_heap_insert(15);
   heapy.max_heap_insert(13);
   heapy.max_heap_insert(9);
   heapy.max_heap_insert(5);
   heapy.max_heap_insert(12);
   heapy.max_heap_insert(8);
   heapy.max_heap_insert(7);
   heapy.max_heap_insert(4);
   heapy.max_heap_insert(0);
   heapy.max_heap_insert(6);
   heapy.max_heap_insert(2);
   heapy.max_heap_insert(1);

   for (i = 0; i < 12; ++i)
   {
      cout << "Max " << heapy.heap_extract_max() << endl;
   }

   return 0;
}
