#ifndef _TAGGEDPQ_H
#define _TAGGEDPQ_H

#include <vector>

struct TPQ_t { 
	int key;
	int data;
	unsigned int loc;
	TPQ_t(int _key, int _data, unsigned int _loc)
		: key(_key), data(_data), loc(_loc) { }
} TPQ_t;
typedef vector<TPQ_t> tpq_vector; // Index values of TPQ will be what is contained in the heap.

template <class DataType>
class TaggedPQ {

private:
	tpq_vector TPQ;
	vector<unsigned int> Heap; // The heap will contain index values to TPQ vector.
	
	void heap_insert(unsigned int tpq_index)
	{
		// Each time a value is moved up and down the heap it must update the value
		// of the location in the TPQ and the value contained in the heap should be
		// the index value of the TPQ.
		unsigned int heap_index;
		
		Heap.push_back(tpq_index);
		heap_index = Heap.size() - 1;
		Heap[heap_index] = tpq_index;
		percolate_down(heap_index, tpq_index);
	}
	void percolate_down(unsigned int heap_index, unsigned int tpq_index)
	{
		while ((heap_index > 0) && (TPQ[Heap[parent(heap_index)]].data > TPQ[Heap[heap_index]].data))
		{
			exchange(heap_index, parent(heap_index));
			heap_index = parent(heap_index);
		}
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
	void exchange(unsigned int i, unsigned int j)
	{
		int tmp = Heap[i];
		TPQ[i].loc = j;
		TPQ[j].loc = i;
		Heap[i] = Heap[j];
		Heap[j] = tmp;
	}
	unsigned int extract_min()
	{
		unsigned int min;
	
		if (Heap.size() < 1)
		{
			std::cerr << "Heap underflow" << std::endl;
		}
		min = Heap[0];
		Heap[0] = Heap.back(); 
		Heap.pop_back();
		min_heapify(0);
		return min;
	}
	void min_heapify(unsigned int heap_index)
	{
		unsigned int smallest;
		unsigned int l = left(heap_index);
		unsigned int r = right(heap_index);
		if ( ( l <= Heap.size() ) && ( TPQ[Heap[l]].data < TPQ[Heap[heap_index]].data ) )
		{
			smallest = l;
		} else {
			smallest = heap_index;
		}
		
		if ( ( r <= Heap.size() ) && ( TPQ[Heap[r]].data < TPQ[Heap[smallest]].data ) )
		{
			smallest = r;
		}
		
		if ( smallest != heap_index )
		{
			exchange(heap_index, smallest);
			min_heapify(smallest);
		}	
	}
public:
	TaggedPQ(int sizeBound) {
		TPQ.reserve(sizeBound);
		Heap.reserve(sizeBound);
	};

	int insert(int key, DataType const &obj) {
		// This should return the tag for the inserted (key,data) pair
		unsigned int tpq_index;
				
		TPQ_t tpq_record(key, *obj, std::numeric_limits<unsigned int>::max());
		TPQ.push_back(tpq_record);
		tpq_index = TPQ.size() - 1;
		heap_insert(tpq_index);
		return tpq_index;
	};

	void changeKey(int tag, int newkey) {
		int newtag;
		TPQ[tag].key = newkey;
		percolate_down(TPQ[tag].loc, tag);
	};

	int getMinKey() {
   	return TPQ[Heap[0]].key; 
	};

	DataType &getMinData() {
		return TPQ[Heap[0]].data;
	};

	void deleteMin() {
		Heap.extract_min();
	};

    bool isEmpty() {
    	if (Heap.empty())
    		return true;
    	else
    		return false;
    };

    ~TaggedPQ() {
    };
};

#endif
