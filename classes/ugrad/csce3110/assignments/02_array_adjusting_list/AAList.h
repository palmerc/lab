// AList.h
// Linked-AList template typename
// To-Do
// Make it a template class
// iterator function

#ifndef AAList_H
#define AAList_H

#include<stdexcept>
#include<string>

using namespace std;
typedef int T;
const int MAXSIZE = 100;

class AAList {
   public:
      AAList(); // Constructor
      ~AAList(); // Destructor
   
      void insert(T item);
      T find(T item);
      void printAAList();      
      int getSize();
     
   private:   
      T Node[MAXSIZE];
      int size;

};

AAList::AAList() // Constructor
{
   for (int i=0; i < MAXSIZE; i++)
      Node[i] = 0;
   size = 0;
}

AAList::~AAList() // Destructor
{
}

void AAList::insert(T item)
{
   if (getSize() > 0 && getSize()+1 < MAXSIZE)
      for (int i=getSize()+1; i > 0; i--) {
         Node[i] = Node[i-1];
      }
   Node[0] = item;
   size++;
}

T AAList::find(T item)
{
   int i=0;
   for (; Node[i] != item; ++i) {}
   
   for (; i < getSize(); i++)
      Node[i] = Node[i+1];
   size--;
   insert(item);
   return item;
}

int AAList::getSize()
{
   return size;
}

void AAList::printAAList()
{
   for (int i=0; i < getSize(); i++)
      cout << "Index->" << i << " Value->" << this->Node[i] << endl;
}
#endif

