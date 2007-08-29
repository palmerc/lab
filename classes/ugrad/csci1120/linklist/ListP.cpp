#include "ListP.h"
#include <cstddef> // for NULL
#include <cassert> // for assert() 

   List::List(): size(0), head(NULL)
   {
   }

   List::List(const List& aList): size(aList.size) // Copy constructor
   {
      // Check to see if list is empty
      if (aList.head == NULL)
         head = NULL;
      else
      {
         head = new ListNode;
         assert(head != NULL); // Make sure memory is available
         head->item = aList.head->item; // Copy the aList head
         // Copy rest of list
         ListNode *newPtr = head; // New list pointer
         for (ListNode *origPtr = aList.head->next;
              origPtr != NULL;
              origPtr = origPtr->next)
         {
            newPtr->next = new ListNode;
            assert(newPtr->next != NULL);
            newPtr = newPtr->next;
            newPtr->item = origPtr->item;
         } // end for
         newPtr->next = NULL;
      } // end if
   } // end copy constructor

   List::~List() // Destructor
   {
      while(!isEmpty())
         remove(1);
   } // end destructor

   bool List::isEmpty() const
   {
      return size == 0;
   }

   int List::getLength() const
   {
      return size;
   }

   List::ListNode *List::find(int index) const
   {
      if ( (index < 1) || (index > getLength()) )
         return NULL;
      else
      {
         ListNode *cur = head;
         for (int skip = 1; skip < index; ++skip)
            cur = cur->next;
         return cur;
      }
   }

   void List::insert(int index, ListItemType newItem)
         throw(ListIndexOutOfRangeException, ListException)
   {
      int newLength = getLength() + 1;
      
      if ((index < 1) || (index > newLength))
         throw ListIndexOutOfRangeException(
         "ListOutOfRangeException: insert index out of range");
      else
      {
         ListNode *newPtr = new ListNode;
         if (newPtr == NULL)
            throw ListException(
            "ListException: insert cannot allocated memory"); 
         else
         {
            size = newLength;
            newPtr->item = newItem;

            if (index == 1)
            {
               newPtr->next = head;
               head = newPtr;
            }
            else
            {
               ListNode *prev = find(index-1);
               newPtr->next = prev->next;
               prev->next = newPtr;
            }
         }
      }
   }

   void List::remove(int index)
         throw(ListIndexOutOfRangeException)
   {
      ListNode *cur;
      
      if ((index < 1) || (index > getLength()))
         throw ListIndexOutOfRangeException(
         "ListOutOfRangeException: remove index out of range");
      else
      {
         --size;
         if (index == 1)
         {
            cur = head;
            head = head->next;
         }
         else
         {
            ListNode *prev = find(index-1);
            cur = prev->next;
            prev->next = cur->next;
         }
  
         cur->next = NULL;
         delete cur;
         cur = NULL;
      }
   }

   void List::retrieve(int index, ListItemType &dataItem) const
         throw(ListIndexOutOfRangeException)
   {
      if ((index < 1) || (index > getLength()))
         throw ListIndexOutOfRangeException(
         "ListOutOfRangeException: retrieve index out of range");
      else
      {
         ListNode *cur = find(index);
         dataItem = cur->item;
      }
   }
