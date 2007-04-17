#include <cmath>
#include <vector>
#include <limits>

class Heap
{
   public:
	Heap();
	void max_heapify(int);
	void build_max_heap(); // Take an array and turn it into a heap
	void heapsort();
	int heap_maximum();
	int heap_extract_max();
	void heap_increase_key(int, int);
	void max_heap_insert(int);
	
   private:
	int parent(int);
	int left(int);
	int right(int);
	void exchange(int, int);
	
    std::vector<int> A;
    int heap_size;
};

Heap::Heap(): heap_size(0) 
{
}
void Heap::max_heapify(int i)
{
	int largest;
	int l = left(i);
	int r = right(i);
	if ( ( l <= heap_size ) && ( A[l] > A[i] ) )
	{
		largest = l;
	} else {
		largest = i;
	}
	
	if ( ( r <= heap_size ) && ( A[r] > A[largest] ) )
	{
		largest = r;
	}
	
	if ( largest != i )
	{
		exchange(i,largest);
		max_heapify(largest);
	}	
}
void Heap::build_max_heap()
{
	int i;
	
	heap_size = A.size();
	for (i = A.size() / 2; i >= 0; --i) // This is dedicated to Pete.
	{
		max_heapify(i);
	}  
}
void Heap::heapsort()
{
	int i;
	
	build_max_heap();
	for (i = A.size(); i >= 0; --i)
	{
		exchange(A[0], A[i]);
		heap_size = heap_size - 1;
		max_heapify(0);
	}
}
int Heap::heap_maximum()
{
	return A[0];
}
int Heap::heap_extract_max()
{
	int max;
	
	if (heap_size < 1)
	{
		std::cerr << "Heap underflow" << std::endl;
	}
	max = A[0];
	A[0] = A[heap_size - 1];
	heap_size = heap_size - 1;
	max_heapify(0);
	return max;
}
void Heap::heap_increase_key(int i, int key)
{
	if (key < A[i])
    {
    	std::cerr << "New key is smaller than current key" << std::endl;
    }
    A[i] = key;
    while ((i > 0) && (A[parent(i)] < A[i]))
    {
		exchange(i, parent(i));
		i = parent(i);
    }
}
void Heap::max_heap_insert(int key)
{
	heap_size = heap_size + 1;
	A.resize(heap_size);
	A[heap_size - 1] = std::numeric_limits<int>::min();
	heap_increase_key(heap_size - 1, key);
}
    
int Heap::parent(int i)
{
	return (i - 1) / 2; // ceil(i/2)
}
int Heap::left(int i)
{
	return 2 * i + 1; // 2 * i
}
int Heap::right(int i)
{
	return 2 * i + 2; // 2 * i + 1
}
void Heap::exchange(int i, int j)
{
	int tmp = A[i];
	A[i] = A[j];
	A[j] = tmp; 
}
