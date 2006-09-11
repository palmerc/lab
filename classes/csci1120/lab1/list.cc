//list.cc, containing functions for operations on an ordered list
//CSCI 1120
//Will Briggs

#include <iostream.h>
#include "list.h"

void insert (Item List[], int& howmany, Item thing)
{
  if (howmany == MAX_ITEMS)
    cerr << "Error in insert: can't add " << thing 
	 << ", list is full!" << endl;
  else
  {
    int where = INITIAL_VALUE;

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
  for (int i = 0; i < howmany; i++) cout << List[i] << endl;
  cout << endl;
}


