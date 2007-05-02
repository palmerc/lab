// This is almost exactly from CLRS. The only addition is the concept of the 
// data value which qualifies this as some lame Tagged Q implementation.

#include <vector>
#include <limits>
#include <iostream>
#include <string>

using std::swap;

struct data {
	int key;
	int value;
};

class MinHeap
{
public:
	MinHeap();
	void min_heapify(int);
	int extract_min();
	void decrease_key(int, int);
	void insert(int, int);
	bool empty();
	
private:
	int parent(int);
	int left(int);
	int right(int);
	void exchange(data*, data*);
	
	data A[100000];
	int position[100000];
	
	int heap_size;
};

MinHeap::MinHeap() : heap_size(0)
{
}
void MinHeap::min_heapify(int index)
{
	int smallest = index;
	int l = left(index);
	int r = right(index);
	if ( ( l <= heap_size ) && ( A[l].value < A[index].value ) )
	{
		smallest = l;
	}
	
	if ( ( r <= heap_size ) && ( A[r].value < A[smallest].value ) )
	{
		smallest = r;
	}
	
	if ( smallest != index )
	{
		swap(A[index], A[smallest]);
		position[A[index].key] = index;
		position[A[smallest].key] = smallest;
		min_heapify(smallest);
	}	
}

int MinHeap::extract_min()
{
	int min;
	
	if (heap_size < 1)
	{
//		std::cerr << "Heap underflow" << std::endl;
	}
	min = A[0].key;
	//std::cout << "extracting " << A.front().v << " " << A.front().w << std::endl;
	A[0] = A[heap_size - 1];
	position[A[heap_size - 1].key] = 0; // This value causes the seg fault
	heap_size--;
	min_heapify(0);
	return min;
}
void MinHeap::decrease_key(int key, int value)
{
	int index = position[key]; // The position vertex points to array indice in A
	//std::cout << "Decreasing " << vertex << std::endl;
	if (value > A[index].value)
	{
//		std::cerr << "New key is smaller than current key" << std::endl;
	}
	//std::cout << "lowering weight " << weight << std::endl;
	A[index].value = value;
	//std::cout << "Decrease Key: " << index << " parent value: "<< A[parent(index)].value << " > cur value: " << A[index].value << std::endl;
	while ((index > 0) && (A[parent(index)].value > A[index].value))
	{
		//std::cout << "loop " << index << " swapping " << A[parent(index)].value << " and " << A[index].value << std::endl;
		exchange(&A[index], &A[parent(index)]);
		position[A[index].key] = index;
		position[A[parent(index)].key] = parent(index);
		//std::cout << "heap index " << A[parent(index)].w << " " << A[index].w << std::endl;
		index = parent(index);
	}
}
void MinHeap::insert(int key, int value)
{
	position[key] = heap_size;
	A[heap_size].key = key;
	A[heap_size].value = std::numeric_limits<int>::max();
	heap_size++;
	decrease_key(key, value);
}
bool MinHeap::empty()
{
	if (heap_size > 0)
		return false;
	else
		return true;
}
int MinHeap::parent(int i)
{
	return (i - 1) / 2;
}
int MinHeap::left(int i)
{
	return 2 * i + 1;
}
int MinHeap::right(int i)
{
	return 2 * i + 2;
}
void MinHeap::exchange(data *i, data *j)
{
	data tmp = *i;
	*i = *j;
	*j = tmp; 
}
