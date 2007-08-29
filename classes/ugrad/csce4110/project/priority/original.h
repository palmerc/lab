#include <cmath>
#include <vector>
#include <limits>
#include <map>

typedef int vertex_t;
typedef int weight_t;
typedef struct vertex_data {
	vertex_t v;
	weight_t w;
} vertex_data;
	
class Heap
{
   public:
	Heap();
	void min_heapify(int);
	void build_min_heap(); // Take an array and turn it into a heap
	void heapsort();
	int heap_minimum();
	int heap_extract_min();
	void heap_decrease_key(int, vertex_data);
	void min_heap_insert(vertex_data);
	
   private:
	int parent(int);
	int left(int);
	int right(int);
	void exchange(int, int);
	
    std::vector<vertex_data> A;
    int heap_size;
};

Heap::Heap(): heap_size(0) 
{
}
void Heap::min_heapify(int i)
{
	int smallest;
	int l = left(i);
	int r = right(i);
	if ( ( l <= heap_size ) && ( A[l].w < A[i].w ) )
	{
		smallest = l;
	} else {
		smallest = i;
	}
	
	if ( ( r <= heap_size ) && ( A[r].w < A[smallest].w ) )
	{
		smallest = r;
	}
	
	if ( smallest != i )
	{
		exchange(i,smallest);
		min_heapify(smallest);
	}	
}
void Heap::build_min_heap()
{
	int i;
	
	heap_size = A.size();
	for (i = A.size() / 2; i >= 0; --i) // This is dedicated to Pete.
	{
		min_heapify(i);
	}  
}
void Heap::heapsort()
{
	int i;
	
	build_min_heap();
	for (i = A.size(); i >= 0; --i)
	{
		exchange(0, i);
		heap_size = heap_size - 1;
		min_heapify(0);
	}
}
vertex_t Heap::heap_minimum()
{
	return A[0].v;
}
vertex_t Heap::heap_extract_min()
{
	vertex_t min;
	
	if (heap_size < 1)
	{
		std::cerr << "Heap underflow" << std::endl;
	}
	min = A[0].v;
	A[0] = A[heap_size - 1];
	heap_size = heap_size - 1;
	min_heapify(0);
	return min;
}
void Heap::heap_decrease_key(int i, vertex_data key)
{
	if (key.w < A[i].w)
    {
    	std::cerr << "New key is smaller than current key" << std::endl;
    }
    A[i] = key;
    while ((i > 0) && (A[parent(i)].w > A[i].w))
    {
		exchange(i, parent(i));
		i = parent(i);
    }
}
void Heap::min_heap_insert(vertex_data key)
{
	heap_size = heap_size + 1;
	A.resize(heap_size);
	A[heap_size - 1].w = std::numeric_limits<int>::min();
	heap_decrease_key(heap_size - 1, key);
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
	vertex_data tmp = A[i];
	A[i] = A[j];
	A[j] = tmp; 
}
