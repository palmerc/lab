//list.cc, containing functions for operations on an ordered list
//CSCI 1120
//Will Briggs
 
#include <iostream>
#include "list.h"
 
using namespace std;
 
void insert (Item List[], int& howmany, Item thing)
{
  if (howmany == MAX_ITEMS)
    cerr << "Error in insert: can't add " << thing
         << ", list is full!" << endl;
  else
  {
    int where = 0;
 
    while (thing > List[where] && where < howmany) where++;
                                                 //go till at or past position
                                                 //of thing OR at end of list
 
    for (int i = howmany; i > where; i--)        //shift all subsequent items
      List [i] = List[i-1];                      //down list by 1
 
    List[where] = thing;                         //finally, add thing in place
    howmany++;
  }
}
 
int  search (Item List[], int  howmany, Item thing)
{
  int i = 0;
 
  while (thing > List[i] && i < howmany-1) i++; //go till at or past position
                                               //of thing OR at end of list
 
  if (List[i] == thing) return i; else return NOT_FOUND;
}
 
void print  (Item List[], int  howmany)
{
  int i;
 
  for (i = 0; i < howmany; i++)
    switch (List[i])
    {
      case NORTH: cout << "NORTH "; break;
      case SOUTH: cout << "SOUTH "; break;
      case  EAST: cout << "EAST ";  break;
      case  WEST: cout << "WEST ";  break;
         default: cout << "Error in print (Item[], int): unknown direction ("
                       << List[i] << ").\n";
    }
 
  cout << endl;
}
 
void append (Item List[], int& howmany, Item thing)
{
  if (howmany == MAX_ITEMS)
    cerr << "Error in append: can't add " << thing
         << ", list is full!" << endl;
  else List[howmany++] = thing;
}
 
void remove_last (Item List[], int& howmany)
{
  if (howmany == 0)
    cerr << "Error in remove: list is full!" << endl;
  else howmany--;
}

