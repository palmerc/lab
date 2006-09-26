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

class ListIndexOutOfRangeException: public out_of_range
{
   public:
      ListIndexOutOfRangeException(const string & message = "")
         : out_of_range(message.c_str())
      {}
};

template<class T>
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

template<class T>
List<T>::List() // Constructor
{
   init();
}

template<class T>
List<T>::List(List const &original)
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

template<class T>
List<T>::~List() // Destructor
{
   erase();
   delete head;
   delete tail;
}

template<class T>
void List<T>::init()
{
   size = 0;
   head = new Node;
   tail = new Node;
   head->next = tail;
   tail->prev = head;
}

template<class T>
void List<T>::insert(int index, T item)
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

template<class T>
void List<T>::erase() // Erase entire list
   throw(ListIndexOutOfRangeException)
{
   while (!isEmpty())
   {
      erase(1);
   }
}

template<class T>
void List<T>::erase(int index) // Erase one item at index
   throw(ListIndexOutOfRangeException)
{
   Node *listItem;
   listItem = find(index);
   listItem->prev->next = listItem->next;
   listItem->next->prev = listItem->prev;
   delete listItem;
   size--;
}

template<class T>
void List<T>::erase(int start, int end) // Erase a range of items
   throw(ListIndexOutOfRangeException)
{
}

template<class T>
T List<T>::retrieve(int index) const
  throw(ListIndexOutOfRangeException)
{
   if (index < 1 || index > getSize())
      throw ListIndexOutOfRangeException("ListIndexOutOfRangeException: retrieve failed, index out of range");
   else
      return find(index)->item;
}

template<class T>
// Tail operation
void List<T>::push(T item)
{
   insert(getSize()+1, item);
}

template<class T>
T List<T>::pop()
{
   T cur = retrieve(getSize());
   erase(getSize());
   return cur;
}

template<class T>
void List<T>::swap(int index1, int index2)
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

template<class T>
// Head operation
void List<T>::shift(T item)
{
   insert(1, item);
}

template<class T>
T List<T>::unshift()
{
   T cur = retrieve(1);
   erase(1);
   return cur;
}

template<class T>
// Return value options
bool List<T>::isEmpty() const
{
   return size == 0;
}

template<class T>
int List<T>::getSize() const
{
   return size;
}

template<class T>
void List<T>::printList() const
{
   for (int i=1; i <= getSize(); i++)
      cout << find(i)->item << endl;
}

template<class T>
typename List<T>::Node *List<T>::find(int index) const
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
