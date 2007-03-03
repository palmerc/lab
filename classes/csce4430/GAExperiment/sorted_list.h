#ifndef _SORTED_LIST_H
#define _SORTED_LIST_H

template <class Data>
class SortedList { // hooray!  inheritance!
public:
	virtual Data& operator[] (int k) = 0;
	virtual void insert(Data& value) = 0;
	virtual ~SortedList() {};
	virtual void remove() = 0;
#ifdef CAN_DUMP // because we can't print all element types...
	virtual void dump() = 0;
#endif
};

template <class Data>
class AVLList: public SortedList<Data> {
private:
	typedef struct node_t {
		Data value;
		int left_size;
		int right_size;
		int balance;
		node_t *left;
		node_t *right;
		node_t *parent;
	};

	node_t *head;
	
	Data& _seek(node_t *node, int index) {
		if (index == node->left_size)
			return node->value;
		if (index > node->left_size)
			return _seek(node->right, index - 1 - node->left_size);
		return _seek(node->left, index);
	}

	// the hack parameter is only set non-zero by _remove().
	// the return value is only used by _remove().
	// all adjustments made "if (hack)" are by a trial and error
	// session using thousands of randomly generated inputs with
	// manual debugging over a period of 8 hours.
	// i hate myself.  but _remove() works now.
	// sorted_list.h.debug still contains a lot of the code
	// i used to get this working. debug.diff is a patch showing
	// all of the differences in a condensed form.
	int _rotate(node_t *node, char *s, int hack) {
		node_t *a, *b, *c, *t1, *t2, *p;
		int t1_s, t2_s;
		int a_b, b_b, c_b;
		int ret = 1;
		// t0 and t3 never have to move, so we don't touch them.
		if (s[0] == 'l' && s[1] == 'l') {
			// before ll: 
			//         C       .
			//        / \      .
			//       B   t3    .
			//      / \        .
			//     A  t2       .
			//    / \          .
			//   t0 t1         .
			c = node;
			b = c->left;
			a = b->left;
			t1 = a->right;
			t1_s = a->right_size;
			t2 = b->right;
			t2_s = b->right_size;
			a_b = a->balance;
			b_b = c_b = 0;
		} else if (s[0] == 'r' && s[1] == 'r') {
			// before rr: 
			//         A       .
			//        / \      .
			//      t0   B     .
			//          / \    .
			//         t1  C   .
			//            / \  .
			//           t2 t3 .
			a = node;
			b = a->right;
			c = b->right;
			t1 = b->left;
			t1_s = b->left_size;
			t2 = c->left;
			t2_s = c->left_size;
			a_b = b_b = 0;
			c_b = c->balance;
			// FIXME:
			if (hack) {
				a_b = b->balance ? 0 : 1;
				b_b = b->balance ? 0 : -1;
				ret = b->balance ? 1 : 0;
			}
		} else if (s[0] == 'l' && s[1] == 'r') {
			// before lr: 
			//         C       .
			//        / \      .
			//       A   t3    .
			//      / \        .
			//     t0  B       .
			//        / \      .
			//       t1 t2     .
			c = node;
			a = c->left;
			b = a->right;
			t1 = b->left;
			t1_s = b->left_size;
			t2 = b->right;
			t2_s = b->right_size;
			a_b = (b->balance > 0) ? -1 : 0;
			b_b = 0;
			c_b = (b->balance < 0) ? 1 : 0;
		} else if (s[0] == 'r' && s[1] == 'l') {
			// before rl: 
			//         A       .
			//        / \      .
			//      t0   C     .
			//          / \    .
			//         B   t3  .
			//        / \      .
			//       t1 t2     .
			a = node;
			c = a->right;
			b = c->left;
			t1 = b->left;
			t1_s = b->left_size;
			t2 = b->right;
			t2_s = b->right_size;
			a_b = (b->balance > 0) ? -1 : 0;
			b_b = 0;
			c_b = (b->balance < 0) ? 1 : 0;
			if (hack) {
				c_b = (c->balance && (b->balance < 0)) ? 1 : 0;
				ret = c->balance ? 1 : 0;
			}
		}
		
		p = node->parent;
		if (!p)
			head = b;
		else if (p->left == node)
			p->left = b;
		else
			p->right = b;

		a->parent = b;
		a->right = t1;
		a->right_size = t1_s;
		
		c->parent = b;
		c->left = t2;
		c->left_size = t2_s;

		b->parent = p;
		b->right = c;
		b->right_size = c->left_size + c->right_size + 1;
		b->left = a;
		b->left_size = a->left_size + a->right_size + 1;
		
		if (t1) t1->parent = a;
		if (t2) t2->parent = c;
		
		a->balance = a_b;
		b->balance = b_b;
		c->balance = c_b;

		// result after rotate:
		//          B         .
		//         / \        .
		//        /   \       .
		//       A     C      .
		//      / \   / \     .
		//     t0 t1 t2 t3    .
		return ret;
	}

	// return 0 if we should quit screwing with the tree at higher
	// levels, return 1 otherwise.
	int _insert(node_t *&node, node_t *parent, Data& value, char *s) {
		if (!node) {
			node = new node_t;
			node->value = value;
			node->left = node->right = NULL;
			node->parent = parent;
			node->left_size = 0;
			node->right_size = 0;
			node->balance = 0;
			return 1;
		}
		if (value < node->value) {
			node->left_size++;
			if (!_insert(node->left, node, value, s))
				return 0;
			s[1] = s[0];
			s[0] = 'l';
			node->balance--;
		} else {
			node->right_size++;
			if (!_insert(node->right, node, value, s))
				return 0;
			s[1] = s[0];
			s[0] = 'r';
			node->balance++;
		}
		if (abs(node->balance) == 2) {
			_rotate(node, s, 0);
			return 0;
		}
		if (!node->balance) return 0;
		return 1;
	}

	int _remove(node_t *node) {
		if (!node->left) {
			node_t *p = node->parent;
			
			if (p) p->left = node->right;
			else head = node->right;
			
			if (node->right) node->right->parent = p;
			
			delete node;
			return 1;
		}

		node->left_size--;
		if (!_remove(node->left)) return 0;
		
		node->balance++;

		// we only continue to alter balance if the factors are 0.
		if (node->balance == 0) return 1;

		if (node->balance == 2) {
			// FIXME
			if (node->right->balance >= 0)
				return _rotate(node, "rr", 1);
			else
				return _rotate(node, "rl", 1);
		}

		return 0;
	}

	void _destroy(node_t *node) {
		if (node->left) _destroy(node->left);
		if (node->right) _destroy(node->right);
		delete node;
	}
	
#ifdef CAN_DUMP
	void _dump(node_t *node) {
		if (!node) return;
		_dump(node->left);
		cout << node->value << " ";
		_dump(node->right);
	}
#endif
	
public:
	
	AVLList() {
		head = NULL;
	}
	
	~AVLList() {
		if (head) _destroy(head);
		head = NULL;
	}
	
	Data& operator[] (int k) {
		return _seek(head, k);
	}

	void insert2(Data v) { insert(v); }

	void insert(Data& value) {
		char s[2]; // stack of "moves"
		s[0] = s[1] = 0;
		_insert(head, NULL, value, s);
	}

	void remove() {
		if (!head) return;
		_remove(head);
	}

#ifdef CAN_DUMP
	void dump() {
		_dump(head);
		cout << endl;
	}
#endif
};

template <class Data>
class BSTList: public SortedList<Data> {
private:
	typedef struct node_t {
		Data value;
		int left_size;
		node_t *left;
		node_t *right;
	};

	node_t *head;
	
	Data& _seek(node_t *node, int index) {
		if (index == node->left_size)
			return node->value;
		if (index > node->left_size)
			return _seek(node->right, index - 1 - node->left_size);
		return _seek(node->left, index);
	}

	void _insert(node_t *&node, Data& value) {
		if (!node) {
			node = new node_t;
			node->value = value;
			node->left = node->right = NULL;
			node->left_size = 0;
			return;
		}
		if (value < node->value) {
			_insert(node->left, value);
			node->left_size++;
			return;
		}
		_insert(node->right, value);
	}

	int _remove(node_t *node) {
		if (!node->left)
			return 1;
		node->left_size--;
		if (_remove(node->left)) {
			node_t *a = node->left;
			node->left = node->left->right;
			delete a;
		}
		return 0;
	}

#ifdef CAN_DUMP
	void _dump(node_t *node) {
		if (!node) return;
		_dump(node->left);
		cout << node->value << " ";
		_dump(node->right);
	}
#endif
	
	void _destroy(node_t *node) {
		if (node->left) _destroy(node->left);
		if (node->right) _destroy(node->right);
		delete node;
	}
	
public:
	BSTList() {
		head = NULL;
	}
	
	~BSTList() {
		if (head) _destroy(head);
		head = NULL;
	}
	
	Data& operator[] (int k) {
		return _seek(head, k);
	}

	void insert(Data& value) {
		_insert(head, value);
	}

	void remove() {
		if (!head) return;
		if (_remove(head)) {
			node_t *a = head;
			head = head->right;
			delete a;
		}
	}

#ifdef CAN_DUMP
	void dump() {
		_dump(head);
		cout << endl;
	}
#endif
};

template <class Data>
class ArrayList : public SortedList<Data> {
private:
	int length; // length of the piece of ram alloc'd for array
	int used; // how much of this array is used
	Data *ram; // the beginning of my alloc'd ram
	Data *start; // the beginning of my used array

	// keep getting crashes inside of glibc's realloc...
	// probably my fault, but i can't see what i'm doing wrong.
	// perhaps something to do with {con,de}structors in the Data classes...
	void grow_list() {
		int new_length;
		Data *new_list;
		
		if (length == 0)
			new_length = 32; // default
		else
			new_length = length * 2; // doubles in size
		
		new_list = new Data[new_length];
		for(int i = 0; i < length; i++)
			new_list[i] = ram[i];

		ram = new_list;
		length = new_length;
	}
	
public:
	ArrayList() {
		length = used = 0;
		ram = start = NULL;
	}

	~ArrayList() {
		// delete array
		if (ram) delete ram;
	}

	Data& operator[](int num) {
		return start[num];
	}

	// This is an example of overloading the new operator.
        void *operator new ( size_t num_bytes )
	{
		return malloc( num_bytes );
	}

	void insert(Data& x) {
		// if the array is full,
		if (used == length) {
			grow_list();
			start = ram;
			// move the start of the array to the new piece of ram.
			// this works every time, because if the old start were
			// not the same as the old piece of ram, the list would
			// not be full.  (it could expand backwards instead.)
		}
		
		int a;
		for (a = 0; (a < used) && (start[a] <= x); a++);

		if (start != ram) { // expand backwards
			// say, if i removed an element already
			for (int i=-1; i < a; i++)
				start[i] = start[i+1];
			start--;
		} else { // expand forwards
			for (int i=used; i > a; i--)
				start[i] = start[i-1];
		}
		start[a] = x;
		used++;
	}

	void remove()
	{
		if (!used) return;
		start++;
		used--;
	}

#ifdef CAN_DUMP
	void dump()
	{
		for (int i = 0; i < used; i++)
			cout << start[i] << " ";
		cout << endl;
	}
#endif
};

template <class Data>
class LinkedList : public SortedList<Data> {
private:
	// node structure for LL
	typedef struct node_t {
		Data data;
		node_t *next;
	};
	
	node_t *head; // the head of my LL

public:
	LinkedList() {
		head = NULL;
	}

	~LinkedList() {
		for (node_t *a=head,*b; a; b=a->next,delete a,a=b);
	}

	Data& operator[] (int num) {
		node_t *a = head;
		for (int i=0; i < num; a=a->next, i++);
		return a->data;
	}

	void insert(Data& x) {
		node_t *y = new node_t;
		node_t *a = NULL;
		// search for the right place for this new node
		for (node_t *b = head; b && (b->data <= x); a=b, b=b->next);

		if (!a) {
			// insert at the head
			y->next = head; head = y;
		} else {
			// insert elsewhere
			y->next = a->next; a->next = y;
		}
		y->data = x;
	}

	// removes the smallest element
	void remove()
	{
		if (!head) return;
		node_t *y = head;
		head = head->next;
		delete y;
	}
	
#ifdef CAN_DUMP
	void dump()
	{
		for (node_t *a = head; a; a = a->next)
			cout << a->data << " ";
		cout << endl;
	}
#endif
};

#endif
