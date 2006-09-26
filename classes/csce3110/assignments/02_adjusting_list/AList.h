// AList.h
// Linked-AList template typename
// To-Do
// Make it a template class
// iterator function

#ifndef AList_H
#define AList_H

#include<stdexcept>
#include<string>

using namespace std;
typedef int T;

class AListIndexOutOfRangeException: public out_of_range
{
   public:
      AListIndexOutOfRangeException(const string & message = "")
         : out_of_range(message.c_str())
      {}
};

class AList {
   public:
      AList(); // Constructor
      AList(AList const &original); // Copy constructor
      ~AList(); // Destructor
   
      void insert(T item)
         throw(AListIndexOutOfRangeException);
      void erase() // Erase entire AList
         throw(AListIndexOutOfRangeException);
      void erase(int index) // Erase one item at index
         throw(AListIndexOutOfRangeException);
      T find(T item)
         throw(AListIndexOutOfRangeException);
      void printAList() const;
       
      // Return value options
      bool isEmpty() const;
      int getSize() const;
      
   private:   
      struct Node {
         Node *next;
         T item;
      };
      
      void init();
      Node *find(int index) const;
      Node *head;
      int size;
};

AList::AList() // Constructor
{
   init();
}

AList::~AList() // Destructor
{
   erase();
   delete head;
}

void AList::init()
{
   size = 0;
   head = new Node;
   head->next = NULL;
}

void AList::insert(T item)
   throw(AListIndexOutOfRangeException)
{
   Node *n = new Node;
   size++;
   n->item = item;
   
   n->next = head->next;
   head->next = n;
}

void AList::erase() // Erase entire AList
   throw(AListIndexOutOfRangeException)
{
   while (!isEmpty())
   {
      erase(1);
   }
}

void AList::erase(int index) // Erase one item at index
   throw(AListIndexOutOfRangeException)
{
   size--;
}

T AList::find(T item)
  throw(AListIndexOutOfRangeException)
{
   
   Node *prev, *cur = head;
   
   while (cur->item != item)
   {
      prev = cur;
      cur = cur->next;
   }
   prev->next = cur->next;
   cur->next = head->next;
   head->next = cur;
   
   return cur->item;
}

void AList::printAList() const
{
   Node *cur = head;
   
   for (int i=1; i <= getSize(); i++)
   {
      cur = cur->next;      
      cout << cur->item << endl;
   }
}

// Return value options
bool AList::isEmpty() const
{
   return size == 0;
}

int AList::getSize() const
{
   return size;
}
#endif

