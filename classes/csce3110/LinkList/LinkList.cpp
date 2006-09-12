#include "LinkList.h"
#include <cstddef> // for NULL
#include <cassert> // for assert()

   // Constructor
   List::List(): size(0), head(NULL)
   {
   }

   // Copy Constructor
   List::List(const List& aList): size(aList.size)
   {
      // Check to see if the list is empty
      if (aList.head == NULL)
         head = NULL;
      else
      {
         head = new ListNode;
         assert(head != NULL); // Make sure memory is available
         head->item = aList.head->item; // Copy the aList head

         // Copy the rest of the list
         ListNode *newPtr = head; // New list pointer
         for (ListNode *origPtr = aList.head->next; origPtr != NULL; origPtr = origPtr->next)
         {
            newPtr->next = new ListNode; // Create a new memory location
            assert(newPtr->next != NULL); // Memory allocation successful?
            newPtr = newPtr->next;
            newPtr->item = origPtr->item;
         }
         newPtr->next = NULL; // The last item's next set to NULL
      }         
   }
   
   // Destructor
   List::~List()
   {
      while(!isEmpty())
         remove(1);
   }
   
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
      // Is the request sane? Before the beginning or after the end?
      if ( (index < 1) || (index > getLength()) )
         return NULL; // The request was not sane.
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
      // In a nutshell
      // locate place for new element
      // copy old next to new item next
      // change old next to new item
      
      int newLength = getLength() + 1;
    
      if ( (index < 1) || (index > newLength) ) 
         throw ListIndexOutOfRangeException (
         "ListIndexOutOfRangeException: insert index out of range");
      else
      {
         ListNode *newPtr = new ListNode;
         if (newPtr == NULL)
            throw ListException(
            "ListException: insert cannot allocate memory");
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
            }
         }
      }   
   }
   
   void List::remove(int index)
      throw(ListIndexOutOfRangeException)
   {
      ListNode *cur;
      
      if ( (index < 1) || (index > getLength()) )
         throw ListIndexOutOfRangeException(
         "ListIndexOutOfRangeException: remove index out of range");
      else
      {
         --size;
         if (index == 1) {
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
      if ( (index < 1) || (index > getLength()) )
         throw ListIndexOutOfRangeException(
         "ListIndexOutOfRangeException: retrieve index out of range");
      else
      {
         ListNode *cur = find(index);
         dataItem = cur->item;
      }
   }
