//lab1.cc, a driver for list.cc
//CSCI 1120
//Will Briggs

#include <iostream.h>
#include "list.h"


main ()
{
  Item Letters [MAX_ITEMS]; int num_letters = 0;

  insert (Letters, num_letters, 'A'); 
  insert (Letters, num_letters, 'D'); 
  insert (Letters, num_letters, 'R'); 
  insert (Letters, num_letters, 'E'); 
  insert (Letters, num_letters, 'T'); 
  insert (Letters, num_letters, 'M'); 
  insert (Letters, num_letters, 'V'); 
  insert (Letters, num_letters, 'P'); 
  insert (Letters, num_letters, 'Q'); 
  insert (Letters, num_letters, 'X'); 

  print  (Letters, num_letters);
}

