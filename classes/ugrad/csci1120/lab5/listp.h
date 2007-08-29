// *********************************************
// Header file listp.h for the ADT list.
// Pointer-based implementation.
// *********************************************

#ifndef _LISTP_H_
#define _LISTP_H_

#include "polyterm.h"

typedef PolyTerm listItemType;

struct listNode;            // linked list node
typedef listNode* ptrType; // pointer to node

class listClass
{
public:
  // constructors and destructor:
  listClass();                // default constructor
  listClass(const listClass& L); // copy constructor
  ~listClass();                   // destructor

  // list operations:
  bool ListIsEmpty() const;
  int ListLength() const;
  void ListInsert(int NewPosition, 
                  listItemType NewItem,
                  bool& Success);
  void ListDelete(int Position, bool& Success);
  void ListRetrieve(int Position, 
                    listItemType& DataItem, 
                    bool& Success) const;

private:
  int Size;     // number of items in list
  ptrType Head; // pointer to linked list of items

  ptrType PtrTo(int Position) const;
      // Returns a pointer to the Position-th node 
      // in the linked list.
}; // end class listClass

#endif // _LISTP_H_

// End of header file ListP.h.

