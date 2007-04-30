// This is almost exactly from CLRS. The only addition is the concept of the 
// data value which qualifies this as some lame Tagged Q implementation.

#include <vector>
#include <limits>
#include <iostream>

struct vertex_data {
	int v; // key
	int w; // value
};

class MinHeap
{
public:
	MinHeap();
	void min_heapify(unsigned int);
	int extract_min();
	void decrease_key(unsigned int, int);
	void insert(unsigned int, int);
	bool empty();
	
private:
	unsigned int parent(unsigned int);
	unsigned int left(unsigned int);
	unsigned int right(unsigned int);
	void exchange(vertex_data*, vertex_data*);
	
	std::vector<vertex_data> A;
	std::vector<unsigned int> position;
	
	unsigned int heap_size;
};

MinHeap::MinHeap() : heap_size(0)
{
}
void MinHeap::min_heapify(unsigned int index)
{
	unsigned int smallest = index;
	unsigned int l = left(index);
	unsigned int r = right(index);
	if ( ( l <= heap_size ) && ( A[l].w < A[index].w ) )
	{
		smallest = l;
	}
	
	if ( ( r <= heap_size ) && ( A[r].w < A[smallest].w ) )
	{
		smallest = r;
	}
	
	if ( smallest != index )
	{
		exchange(&A[index], &A[smallest]);
		position[A[index].v] = index;
		position[A[smallest].v] = smallest;
		min_heapify(smallest);
	}	
}
int MinHeap::extract_min()
{
	int min;
	
	if (heap_size < 1)
	{
		std::cerr << "Heap underflow" << std::endl;
	}
	min = A[0].v;
	//std::cout << "extracting " << A.front().v << " " << A.front().w << std::endl;
	A[0] = A[heap_size];
	position[A[heap_size].v] = -1; // This value causes the seg fault
	min_heapify(0);
	return min;
}
void MinHeap::decrease_key(unsigned int vertex, int weight)
{
	unsigned int index = position[vertex]; // The position vertex points to array indice in A
	//std::cout << "Decreasing " << vertex << std::endl;
	if (weight > A[index].w)
	{
		std::cerr << "New key is smaller than current key" << std::endl;
	}
	//std::cout << "lowering weight " << weight << std::endl;
	A[index].w = weight;
	std::cout << "DecKey " << index << " parent w: "<< A[parent(index)].w << " > cur w: " << A[index].w << std::endl;
	while ((index > 0) && (A[parent(index)].w > A[index].w))
	{
		//std::cout << "lowering weight loop " << index << " " << A[parent(index)].w << " " << A[index].w << std::endl;
		exchange(&A[index], &A[parent(index)]);
		position[A[index].v] = index;
		position[A[parent(index)].v] = parent(index);
		//std::cout << "loop " << A[parent(index)].w << " " << A[index].w << std::endl;
		index = parent(index);
	}
}
void MinHeap::insert(unsigned int vertex, int weight)
{
	heap_size++;
	if (position.size() < heap_size)
		position.resize(heap_size);
	position[vertex] = heap_size;
	vertex_data tmp;
	tmp.v = vertex;
	tmp.w = std::numeric_limits<int>::max();
	//tmp.w = 10000;
	A[heap_size] = tmp;
	
	decrease_key(vertex, weight);
}
bool MinHeap::empty()
{
	if (heap_size > 0)
		return false;
	else
		return true;
}
unsigned int MinHeap::parent(unsigned int i)
{
	return i / 2;
}
unsigned int MinHeap::left(unsigned int i)
{
	return 2 * i;
}
unsigned int MinHeap::right(unsigned int i)
{
	return 2 * i + 1;
}
void MinHeap::exchange(vertex_data *i, vertex_data *j)
{
	vertex_data tmp = *i;
	*i = *j;
	*j = tmp; 
}
