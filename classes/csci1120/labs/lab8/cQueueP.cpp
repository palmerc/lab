// *********************************************************
// Implementation file QueueP.cpp for the ADT queue.
// Pointer-based implementation.
// *********************************************************
#include "cQueueP.h" // header file
#include <cstddef> // for NULL
 
// The queue is implemented as a circular linked list
// with one external pointer to the back of the queue.
 
struct queueNode
{
  queueItemType Item;
  ptrType Next;
}; // end struct
 
cQueueP::cQueueP() : BackPtr(NULL)
{
} // end default constructor
 
cQueueP::cQueueP(const cQueueP& Q)
{ // Implementation left as an exercise (Exercise 4).
} // end copy constructor
 
cQueueP::~cQueueP()
{
  bool Success;
 
  while (!QueueIsEmpty())
    QueueDelete(Success);
  // Assertion: BackPtr == NULL
} // end destructor
 
bool cQueueP::QueueIsEmpty() const
{
  return bool(BackPtr == NULL);
} // end QueueIsEmpty
 
void cQueueP::QueueInsert(queueItemType NewItem,
                             bool& Success)
{
  // create a new node
  ptrType NewPtr = new queueNode;
 
  Success = bool(NewPtr != NULL); // check allocation
  if (Success)
  { // allocation successful; set data portion of new node
    NewPtr->Item = NewItem;
 
    // insert the new node
    if (QueueIsEmpty())
     // insertion into empty queue
     NewPtr->Next = NewPtr;
 
    else
    { // insertion into nonempty queue
      NewPtr->Next = BackPtr->Next;
      BackPtr->Next = NewPtr;
    } // end if
 
    BackPtr = NewPtr; // new node is at back
  } // end if
} // end QueueInsert
 
void cQueueP::QueueDelete(bool& Success)
{
 
  Success = bool(!QueueIsEmpty());
 
  if (Success)
  { // queue is not empty; remove front
    ptrType FrontPtr = BackPtr->Next;
    if (FrontPtr == BackPtr) // special case?
      BackPtr = NULL;        // yes, one node in queue
    else
      BackPtr->Next = FrontPtr->Next;
 
    FrontPtr->Next = NULL; // defensive strategy
    delete FrontPtr;
  } // end if
} // end QueueDelete
 
void cQueueP::QueueDelete(queueItemType& QueueFront, 
                             bool& Success)
{
  Success = bool(!QueueIsEmpty());
 
  if (Success)
  { // queue is not empty; retrieve front
    ptrType FrontPtr = BackPtr->Next;
    QueueFront = FrontPtr->Item;
 
    QueueDelete(Success); // delete front
  } // end if
} // end QueueDelete
 
void cQueueP::GetQueueFront(queueItemType& QueueFront, 
                               bool& Success) const
{
  Success = bool(!QueueIsEmpty());
 
  if (Success)
  { // queue is not empty; retrieve front
    ptrType FrontPtr = BackPtr->Next;
    QueueFront = FrontPtr->Item;
  } // end if
} // end GetQueueFront
// End of implementation file.

