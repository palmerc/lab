#include "ListException.h"
#include "ListIndexOutOfRangeException.h"

typedef int ListItemType;

class List
{
 public:
   List(); // Default Constructor
   List(const List& aList); // Copy Constructor
   ~List(); // Destructor

   bool isEmpty() const;
   int getLength() const;
   void insert(int index, ListItemType newItem)
      throw(ListIndexOutOfRangeException, ListException);
   void remove(int index)
      throw(ListIndexOutOfRangeException);
   void retrieve(int index, ListItemType &dataItem) const
      throw(ListIndexOutOfRangeException);
      
 private:
   struct ListNode
   {
      ListItemType item;
      ListNode *next;
   };
   
   int size;
   ListNode *head;
   ListNode *find(int index) const;
};
