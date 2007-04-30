#include <vector>
#include <limits>

class MinHeap
{
public:
	MinHeap();
	void min_heapify(int);
	int minimum();
	int extract_min();
	unsigned int decrease_key(unsigned int, int);
	unsigned int insert(int);
	
private:
	unsigned int parent(unsigned int);
	unsigned int left(unsigned int);
	unsigned int right(unsigned int);
	void exchange(unsigned int, unsigned int);
	
	std::vector<int> A;
	unsigned int heap_size;
};

MinHeap::MinHeap(): heap_size(0) 
{
}
void MinHeap::min_heapify(int index)
{
	int smallest;
	int l = left(index);
	int r = right(index);
	if ( ( l <= heap_size ) && ( A[l] < A[index] ) )
	{
		smallest = l;
	} else {
		smallest = index;
	}
	
	if ( ( r <= heap_size ) && ( A[r] < A[smallest] ) )
	{
		smallest = r;
	}
	
	if ( smallest != index )
	{
		exchange(index, smallest);
		min_heapify(smallest);
	}	
}
int MinHeap::minimum()
{
	return A[0];
}
int MinHeap::extract_min()
{
	int min;
	
	if (heap_size < 1)
	{
		std::cerr << "Heap underflow" << std::endl;
	}
	min = A[0];
	A[0] = A[heap_size - 1];
	heap_size = heap_size - 1;
	min_heapify(0);
	return min;
}
unsigned int MinHeap::decrease_key(unsigned int index, int key)
{
	if (key < A[index])
	{
		std::cerr << "New key is smaller than current key" << std::endl;
	}
	A[index] = key;
	while ((index > 0) && (A[parent(index)] > A[index]))
	{
		exchange(index, parent(index));
		index = parent(index);
	}
	return index;
}
unsigned int MinHeap::insert(int key)
{
	heap_size = heap_size + 1;
	A.resize(heap_size);
	
	unsigned int index = heap_size - 1;
	A[index] = std::numeric_limits<int>::min();
	return decrease_key(index, key); 
}
unsigned int MinHeap::parent(unsigned int i)
{
	return (i - 1) / 2;
}
unsigned int MinHeap::left(unsigned int i)
{
	return 2 * i + 1;
}
unsigned int MinHeap::right(unsigned int i)
{
	return 2 * i + 2;
}
void MinHeap::exchange(unsigned int i, unsigned int j)
{
	int tmp = A[i];
	A[i] = A[j];
	A[j] = tmp; 
}
