#include "ListException.h"
#include "ListIndexOutOfRangeException.h"

template <typename LISTITEMTYPE>
class List
{
 public:
   List(); // Default Constructor
   List(const List& aList); // Copy Constructor
   ~List(); // Destructor

   bool isEmpty() const;
   int getLength() const;
   void insert(int index, LISTITEMTYPE newItem)
      throw(ListIndexOutOfRangeException, ListException);
   void remove(int index)
      throw(ListIndexOutOfRangeException);
   void retrieve(int index, LISTITEMTYPE &dataItem) const
      throw(ListIndexOutOfRangeException);
      
 private:
   template <LISTITEMTYPE> struct ListNode
   {
      LISTITEMTYPE item;
      ListNode *next;
   };
   
   int size;
   ListNode *head;
   ListNode *find(int index) const;
};
