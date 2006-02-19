// main.cpp
 
// This program tests the Array-based list class - ListClass
 
#include <iostream>
#include "listda.h"
 
using namespace std;
 
// -------------------------------------------- DisplayList
void DisplayList(const cListDA& theList)
{
   cListDAItemType item;
   bool success = true;
 
   cout << "List length = " << theList.getLength() << endl;
 
   for (int i=1; success; i++)
   {
      theList.retrieve(i, item, success);
      if (success)
         cout << "Item " << i << ": " << item << endl;
   }
   cout << endl;
}
 
// --------------------------------------------------- main
int main()
{
  cListDA         aList;     // a list
  int            number;  // item's position in the list
  cListDAItemType value;     // item's value
  bool         ok;        // valid operation?
  
  //cout << "GetCapacity(): " << aList.getCapacity() << endl;
  //cout << "Enter the number of items to insert" << endl;
  //cin >> number;
  number = 100000;
  for (int i=1; i <= number; i++)
  {
     aList.insert(i, i, ok); 
     //cout << "GetCapacity() " << aList.getCapacity() << endl;
  } 
  //   DisplayList(aList);
  // Build the list
  for (int i=1; i <= number; i++)
  {
     aList.remove(1, ok); 
     //cout << "GetCapacity() " << aList.getCapacity() << endl;
  } 
//     DisplayList(aList);
  
  
  return 0;
}

