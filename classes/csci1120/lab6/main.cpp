#include "list.h"
#include <iostream>

Item Path[MAX_ITEMS];
int num_moves = 0;

char room[5][5] = {{' ',' ',' ',' ','#'},
                   {'#','#','#',' ',' '},
                   {'E','#',' ',' ','#'},
                   {' ',' ',' ',' ','#'},
                   {'#','#','#',' ',' '}};

bool Search(int x, int y)
{
   if (room[x][y] == 'E')
      return true;
   if (x < 0 || x > 4 || y < 0 || y > 4)
      return false;
   if (room[x][y] == '#' || room[x][y] == 'V')
      return false;
   room[x][y] = 'V';

   if (Search(x, y-1))
   {   
      append(Path, num_moves, NORTH);   
      return true;
   }
   if (Search(x, y+1))
   {
      append(Path, num_moves, SOUTH);
      return true;
   }
   if (Search(x+1, y))
   {
      append(Path, num_moves, EAST);
      return true;
   }
   if (Search(x-1, y))
   {
      append(Path, num_moves, WEST);
      return true;
   }
   remove_last(Path, num_moves); 
}

int main ()
{

   Search(0, 0);
   print(Path, num_moves);

   return 0;
}
