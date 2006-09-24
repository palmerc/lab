// List.h
// Linked-list template class
// To-Do
// throwing and trying errors and values
// iterator function
// find item
#ifndef LIST_H
#define LIST_H

#include<stdexcept>
#include<string>

using namespace std;

class ListIndexOutOfRangeException: public out_of_range
{
   public:
      ListIndexOutOfRangeException(const string & message = "")
         : out_of_range(message.c_str())
      {}
};

template <class T>
class List {
   public:
      List(); // Constructor
      List(List const& original); // Copy constructor
      ~List(); // Destructor
   
      void insert(int index, T item)
         throw(ListIndexOutOfRangeException);
      void erase() // Erase entire list
         throw(ListIndexOutOfRangeException);
      void erase(int index) // Erase one item at index
         throw(ListIndexOutOfRangeException);
      void erase(int start, int end) // Erase a range of items
         throw(ListIndexOutOfRangeException);
   
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
      void find(int index, Node *cur) const;
      Node *head;
      Node *tail;
      int size;
};

template <class T>
List<T>::List() // Constructor
{
   init();
}

template <class T>
List<T>::List(List const& original)
{
}

template <class T>
List<T>::~List() // Destructor
{
   erase();
   delete head;
   delete tail;
}

template <class T>
void List<T>::init()
{
   size = 0;
   head = new Node;
   tail = new Node;
   head->next = tail;
   tail->prev = head;
}

template <class T> 
void List<T>::insert(int index, T item)
   throw(ListIndexOutOfRangeException)
{
   int newSize = getSize() + 1;
   // The newSize considers that you might have an index value that is the end + 1
   if (index < 1 || index > newSize)
      throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: insert failed, index out of range");
   else
   {
      Node *n = new Node;
      size = newSize;
      n->item = item;
      cout << "head-> " << head->prev << " " << head << " " << head->next << endl;
      cout << "tail-> " << tail->prev << " " << tail << " " << tail->next << endl;
      
      Node *rhs = NULL, *lhs = NULL;
      find(index, rhs);
      cout << "rhs-> " << rhs->prev << " " << rhs << endl;
      cout << "Test" << endl;
      lhs = n->prev = rhs->prev;
      cout << "Test" << endl;
      n->next = rhs;
      cout << "Test" << endl;
      lhs->next = rhs->prev = n;
   }        
}

template <class T>
void List<T>::erase() // Erase entire list
   throw(ListIndexOutOfRangeException)
{
   while (!isEmpty())
   {
      erase(1);
   }
}

template <class T>
void List<T>::erase(int index) // Erase one item at index
   throw(ListIndexOutOfRangeException)
{
   Node *listItem;
   find(index, listItem);
   delete listItem;
   size--;
}

template <class T>
void List<T>::erase(int start, int end) // Erase a range of items
   throw(ListIndexOutOfRangeException)
{
}

// Tail operation
template <class T>
void List<T>::push(T item)
{
}

template <class T>
T List<T>::pop()
{
}

// Head operation
template <class T>
void List<T>::shift(T item)
{
}

template <class T>
T List<T>::unshift()
{
}

// Return value options
template <class T>
bool List<T>::isEmpty() const
{
   return size == 0;
}

template <class T>
int List<T>::getSize() const
{
   return size;
}

template <class T>
void List<T>::find(int index, Node *cur) const
{
   cout << "cur-> " << cur << endl;
   cur = head;
   for (int i=1; i <= index; ++i)
   {
      cur = cur->next;
      cout << "cur-> " << cur->prev << " " << cur << " " << cur->next << endl;
   }
   cout << "find-> " << cur->prev << " " << cur << " " << cur->next << endl;
}
#endif
