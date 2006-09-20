// List.h
// Linked-list template class
// To-Do
// throwing and trying errors and values
// iterator function
// find item
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
      bool isEmpty();
      int getSize();
      
   private:   
      struct Node {
         Node* next;
         Node* prev;
         T item;
      };
      
      void init();
      Node find(int index);
      Node *head;
      Node *tail;
      int size;
};

template <class T> List<T>::List()
{
   init();
}

template <class T> List<T>::List(List const& original)
{
}

template <class T> List<T>::~List()
{
   erase();
   delete head;
   delete tail;
}

template <class T> void List<T>::init()
{
   size = 0;
   head = new Node;
   tail = new Node;
   head->next = tail;
   tail->prev = head;
}


template <class T> void List<T>::insert(int index, T item)
   throw(ListIndexOutOfRangeException)
{
   if (isEmpty())
   {
      if (index !=1)
         throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: insert failed, index out of range");
      head->next = tail = new Node;
      head->next = head->prev = NULL;
      head->item = item;
      ++size;
   }
   else
   {
      int newSize = size++;
      // The newSize considers that you might have an index value that is the end + 1
      if (index > 0 && index < newSize)
      {
         Node* n, lhs, rhs;
         rhs = find(index);
         lhs = rhs->prev;
         n = lhs->next = rhs->prev = new Node;
         n->prev = lhs;
         n->next = rhs;
         n->item = item;
         size = newSize;
      }
      else
         throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: insert failed, index out of range");
   }
}

template <class T> void List<T>::erase() // Erase entire list
   throw(ListIndexOutOfRangeException)
{
   while (!isEmpty)
   {
      erase(1);
   }
}

template <class T> void List<T>::erase(int index) // Erase one item at index
   throw(ListIndexOutOfRangeException)
{
   Node *listItem;
   listItem = find(index);
   delete listItem;
   size--;
}
template <class T> void List<T>::erase(int start, int end) // Erase a range of items
   throw(ListIndexOutOfRangeException)
{
}

// Tail operation
template <class T> void List<T>::push(T item)
{
}

template <class T> T List<T>::pop()
{
}

// Head operation
template <class T> void List<T>::shift(T item)
{
}

template <class T> T List<T>::unshift()
{
}

// Return value options
template <class T> bool List<T>::isEmpty()
{
   return size == 0;
}

template <class T> int List<T>::getSize()
{
   return size;
}

template <class T> Node List<T>::find(int index)
{
   
}