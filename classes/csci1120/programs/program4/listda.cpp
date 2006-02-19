// listda.cpp
 
// **********************************************
// Implementation file for the ADT list - cListDA.
// Array-based implementation.
// **********************************************
 
 
#include "listda.h" //header file
#include <iostream>
using namespace std;
 
// -------------------------------------------------- cListDA
cListDA::cListDA() : size(0), curAllocation(5), allocationIncrement(5)
{
   ptrToList = new cListDAItemType[curAllocation];
} // end default constructor
 
cListDA::cListDA(int initialAllocation, int initialIncrement)
{
   curAllocation = initialAllocation; 
   allocationIncrement = initialIncrement; 
   size = 0;
 
   ptrToList = new cListDAItemType[curAllocation];
} // end constructor
 
cListDA::~cListDA()
{
   bool success;
   for (int i=0; i < size; i++)
      remove(1, success);
   delete[] ptrToList;
   ptrToList = NULL;
} // end destructor
 
// ----------------------------------------- cListDA::isEmpty
bool cListDA::isEmpty() const
{
   return bool(size == 0);
} // end isEmpty
 
// --------------------------------------- cListDA::getCapacity
int cListDA::getCapacity() const
{
   return curAllocation;
} // end getLength
 
// --------------------------------------- cListDA::getLength
int cListDA::getLength() const
{
   return size;
} // end getLength
 
// ------------------------------------------ cListDA::insert
void cListDA::insert(int index, cListDAItemType newItem,
                  bool& success)
{
   if (size == curAllocation) 
   {
      //cout << "INSERT: " << curAllocation << endl; 
      cListDAItemType* oldArray = ptrToList;
      ptrToList = new cListDAItemType[curAllocation + allocationIncrement];
 
      for (int i = 0; i < curAllocation; i++)
         ptrToList[i] = oldArray[i]; 
      delete[] oldArray;
      oldArray = NULL;
      curAllocation = curAllocation + allocationIncrement;
   }
 
   success = bool( (index >= 1) &&
                   (index <= size+1) &&
                   (size < curAllocation) );
 
   if (success)
   {  // make room for new item by shifting all items at
      // positions >= index toward the end of the
      // list (no shift if index == size+1)
      for (int pos = size; pos >= index; --pos)
         ptrToList[translate(pos+1)] = ptrToList[translate(pos)];
 
      // insert new item
      ptrToList[translate(index)] = newItem;
      ++size; // increase the size of the list by one
   } // end if
} // end insert
 
// ------------------------------------------ cListDA::remove
void cListDA::remove(int index, bool& success)
{
   success = bool( (index >= 1) && (index <= size) );
 
   if (success)
   {  // delete item by shifting all items at positions >
      // index toward the beginning of the list
      // (no shift if index == size)
      for (int fromPosition = index+1;
               fromPosition <= size; ++fromPosition)
         ptrToList[translate(fromPosition-1)] =
                             ptrToList[translate(fromPosition)];
      --size; // decrease the size of the list by one
   } // end if
 
   if ((size * 2) < curAllocation) 
   {
      //cout << "Remove : " << endl;
      //cout << size << " " << curAllocation << endl;
      cListDAItemType* oldArray = ptrToList;
      ptrToList = new cListDAItemType[(curAllocation / 2) + (curAllocation % 2)];
 
      for (int i = 0; i < size; i++)
         ptrToList[i] = oldArray[i]; 
      delete[] oldArray;
      oldArray = NULL;
      curAllocation = (curAllocation / 2) + (curAllocation % 2);
   }
} // end remove
 
// ------------------------------------------ cListDA::retrieve
void cListDA::retrieve(int index, 
                      cListDAItemType& dataItem,
                      bool& success) const
{
   success = bool( (index >= 1) &&
                   (index <= size) );
 
   if (success)
      dataItem = ptrToList[translate(index)];
} // end retrieve
 
// ----------------------------------------- cListDA::translate
int cListDA::translate(int index) const
{
   return index-1;
} // end translate
 
// End of implementation file.

