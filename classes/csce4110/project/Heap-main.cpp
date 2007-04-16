#include <iostream>
#include "Heap.h"

using namespace std;

int main()
{
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

   return 0;
}
