// This is almost exactly from CLRS. The only addition is the concept of the 
// data value which qualifies this as some lame Tagged Q implementation.

#include <vector>
#include <limits>

struct vertex_data {
	int v; // key
	int w; // value
};

class MinHeap
{
public:
	MinHeap();
	void min_heapify(int);
	int extract_min();
	void decrease_key(unsigned int, int);
	void insert(unsigned int, int);
	
private:
	unsigned int parent(unsigned int);
	unsigned int left(unsigned int);
	unsigned int right(unsigned int);
	void exchange(vertex_data*, vertex_data*);
	
	std::vector<vertex_data> A;
	std::vector<unsigned int> position;
};

MinHeap::MinHeap()
{
}
void MinHeap::min_heapify(int index)
{
	int smallest = index;
	int l = left(index);
	int r = right(index);
	if ( ( l <= A.size() ) && ( A[l].w < A[index].w ) )
	{
		smallest = l;
	}
	
	if ( ( r <= A.size() ) && ( A[r].w < A[smallest].w ) )
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
	
	if (A.size() < 1)
	{
		std::cerr << "Heap underflow" << std::endl;
	}
	min = A.front().v;
	//std::cout << "extracting " << A.front().v << " " << A.front().w << std::endl;
	A.front() = A.back();
	position[A.back().v] = -1;
	A.pop_back();
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
	//std::cout << "comparison " << A[parent(index)].w << " > " << A[index].w << std::endl;
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
	position.resize(A.size() + 1);
	position[vertex] = A.size();
	vertex_data tmp;
	tmp.v = vertex;
	tmp.w = std::numeric_limits<int>::max();
	A.push_back(tmp);
	
	decrease_key(vertex, weight);
}
unsigned int MinHeap::parent(unsigned int i)
{
	return (i) / 2;
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
