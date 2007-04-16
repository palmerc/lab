class MinPriorityQueue {
public:
	MinPriorityQueue();
	~MinPriorityQueue();
	
	void insert();
	void minimum();
	void extract_min();
	void decrease_key();
private:
	void length();
	void heap_size();
	
	int A[];
	
	struct element {
	};
};
