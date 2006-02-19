// listda.h
 
// *********************************************************
// Header file ListAD.h for the ADT list
// Dynamic Array-based implementation
// *********************************************************
#ifndef _LISTDA_H_
#define _LISTDA_H_
 
typedef int cListDAItemType;
 
class cListDA
{
 public:
   cListDA(); // default constructor
              // destructor is supplied by compiler
 
   cListDA(int initialAllocation, int allocationIncrement); // constructor
 
   ~cListDA(); // destructor
 
// cListDA operations:
   bool isEmpty() const;
   // Determines whether a cListDA is empty.
   // Precondition: None.
   // Postcondition: Returns true if the cListDA is empty;
   // otherwise returns false.
 
   int getCapacity() const;
   // Determines the capacity of the currently allocated array.
 
   int getLength() const;
   // Determines the length of a cListDA.
   // Precondition: None.
   // Postcondition: Returns the number of items
   // that are currently in the cListDA.
 
   void insert(int index, cListDAItemType newItem,
               bool& success);
   // Inserts an item into the cListDA at position index.
   // Precondition: index indicates the position at which
   // the item should be inserted in the cListDA.
   // Postcondition: If insertion is successful, newItem is
   // at position index in the cListDA, and other items are
   // renumbered accordingly, and success is true;
   // otherwise success is false.
   // Note: Insertion will not be successful if
   // index < 1 or index > getLength()+1.
 
   void remove(int index, bool& success);
   // Deletes an item from the cListDA at a given position.
   // Precondition: index indicates where the deletion
   // should occur.
   // Postcondition: If 1 <= index <= getLength(),
   // the item at position index in the cListDA is
   // deleted, other items are renumbered accordingly,
   // and success is true; otherwise success is false.
 
   void retrieve(int index, cListDAItemType& dataItem,
                 bool& success) const;
   // Retrieves a cListDA item by position.
   // Precondition: index is the number of the item to
   // be retrieved.
   // Postcondition: If 1 <= index <= getLength(),
   // dataItem is the value of the desired item and
   // success is true; otherwise success is false.
 
 private:
   cListDAItemType	*ptrToList;
   int          size;            // number of items in cListDA
   int          curAllocation;
   int          allocationIncrement;
 
   int translate(int index) const;
   // Converts the position of an item in a cListDA to the
   // correct index within its array representation.
}; // end cListDA class
 
#endif // _LISTDA_H_
 
// End of header file.

