// List.h
// Linked-list template typename
// To-Do
// Make it a template class
// iterator function

#ifndef LIST_H
#define LIST_H

#include<stdexcept>
#include<string>

using namespace std;
typedef int T;

class ListIndexOutOfRangeException: public out_of_range
{
   public:
      ListIndexOutOfRangeException(const string & message = "")
         : out_of_range(message.c_str())
      {}
};

class List {
   public:
      List(); // Constructor
      List(List const &original); // Copy constructor
      ~List(); // Destructor
   
      void insert(int index, T item)
         throw(ListIndexOutOfRangeException);
      void erase() // Erase entire list
         throw(ListIndexOutOfRangeException);
      void erase(int index) // Erase one item at index
         throw(ListIndexOutOfRangeException);
      void erase(int start, int end) // Erase a range of items
         throw(ListIndexOutOfRangeException);
      T retrieve(int index) const
         throw(ListIndexOutOfRangeException);
      void swap(int index1, int index2)
         throw(ListIndexOutOfRangeException);
      void printList() const;
      // Tail operation
      void push(T item);
      T pop();
      
      // Head operation
      void shift(T item);
      T unshift();
   
      // Return value options
      bool isEmpty() const;
      int getSize() const;
      
   private:   
      struct Node {
         Node *next;
         Node *prev;
         T item;
      };
      
      void init();
      Node *find(int index) const;
      Node *head;
      Node *tail;
      int size;
};

List::List() // Constructor
{
   init();
}

List::List(List const &original)
{
   init();
   Node *cur = original.head;
   //cout << "Point A" << endl;
   for (int i=1; i <= original.getSize(); ++i) 
   {
      cur = cur->next;
      this->insert(i, cur->item);
   }
}

List::~List() // Destructor
{
   erase();
   delete head;
   delete tail;
}

void List::init()
{
   size = 0;
   head = new Node;
   tail = new Node;
   head->next = tail;
   tail->prev = head;
}

void List::insert(int index, T item)
   throw(ListIndexOutOfRangeException)
{
   int newSize = getSize() + 1;
   // The newSize considers that you might have an index value that is the end + 1
   if (index < 1 || index > newSize)
      throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: insert failed, index out of range");
   else
   {
      //cout << endl << "A new node" << endl;
      Node *n = new Node;
      size = newSize;
      n->item = item;
      //cout << "head-> " << head->prev << " " << head << " " << head->next << endl;
      //cout << "tail-> " << tail->prev << " " << tail << " " << tail->next << endl;
      
      Node *rhs = NULL, *lhs = NULL;
      rhs = find(index);
      //cout << "rhs-> " << rhs->prev << " " << rhs << endl;
      lhs = n->prev = rhs->prev;
      n->next = rhs;
      lhs->next = rhs->prev = n;
   }        
}

void List::erase() // Erase entire list
   throw(ListIndexOutOfRangeException)
{
   while (!isEmpty())
   {
      erase(1);
   }
}

void List::erase(int index) // Erase one item at index
   throw(ListIndexOutOfRangeException)
{
   Node *listItem;
   listItem = find(index);
   listItem->prev->next = listItem->next;
   listItem->next->prev = listItem->prev;
   delete listItem;
   size--;
}

void List::erase(int start, int end) // Erase a range of items
   throw(ListIndexOutOfRangeException)
{
}

T List::retrieve(int index) const
  throw(ListIndexOutOfRangeException)
{
   if (index < 1 || index > getSize())
      throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: retrieve failed, index out of range");
   else
      return find(index)->item;
}

// Tail operation
void List::push(T item)
{
   insert(getSize()+1, item);
}

T List::pop()
{
   T cur = retrieve(getSize());
   erase(getSize());
   return cur;
}

void List::swap(int index1, int index2)
   throw(ListIndexOutOfRangeException)
{
   if (index1 < 1 || 
      index2 < 1 || 
      index1 > getSize() ||
      index2 > getSize())
      throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: swap failed, index out of range");
   else
   {
      Node *leftNode = find(index1);
      Node *rightNode = find(index2);
      T leftItem = leftNode->item;
      leftNode->item = rightNode->item;
      rightNode->item = leftItem;
   }
}

// Head operation
void List::shift(T item)
{
   insert(1, item);
}

T List::unshift()
{
   T cur = retrieve(1);
   erase(1);
   return cur;
}

// Return value options
bool List::isEmpty() const
{
   return size == 0;
}

int List::getSize() const
{
   return size;
}

void List::printList() const
{
   for (int i=1; i <= getSize(); i++)
      cout << find(i)->item << endl;
}

List::Node *List::find(int index) const
{
   Node *cur;
   //cout << "cur-> " << cur << endl;
   cur = head;
   for (int i=1; i <= index; ++i)
   {
      cur = cur->next;
      //cout << "cur-> " << cur->prev << " " << cur << " " << cur->next << endl;
   }
   //cout << "find-> " << cur->prev << " " << cur << " " << cur->next << endl;
   return cur;
}
#endif
