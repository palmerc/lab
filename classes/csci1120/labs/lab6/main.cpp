#include <iostream>
#include "list.h"
 
using namespace std;
 
Item Path[MAX_ITEMS];
int num_moves = 0;
 
char room[5][5] = {{' ',' ',' ',' ','#'},
                   {'#','#','#',' ',' '},
                   {'E','#',' ',' ','#'},
                   {' ',' ',' ',' ','#'},
                   {'#','#','#',' ',' '}};
 
void Display()
{
   cout << endl << "Robot Moves: " << endl;
   cout << "Moves - " << num_moves << endl;
   for (int y = 0; y < 5; y++)
   {
      for (int x = 0; x < 5; x++)
         cout << room[y][x];
      cout << endl;
   }
   cout << endl;
}
 
bool Search(int x, int y)
{
   Display();
   if (x < 0 || x > 4 ||
       y < 0 || y > 4 ||
       room[y][x] == '#' ||
       room[y][x] == 'V')
      return false;
 
   if (room[y][x] == 'E')
   {
      cout << "Robot found exit!" << endl;
      return true;
   }
 
   room[y][x] = 'V';
 
   append(Path, num_moves, NORTH);
   if (Search(x, y-1))
      return true;
   else
      remove_last(Path, num_moves);
 
   append(Path, num_moves, SOUTH);
   if (Search(x, y+1))
      return true;
   else
      remove_last(Path, num_moves);
 
   append(Path, num_moves, EAST);
   if (Search(x+1, y))
      return true;
   else
      remove_last(Path, num_moves);
 
   append(Path, num_moves, WEST);
   if (Search(x-1, y))
      return true;
   else
      remove_last(Path, num_moves);
 
   room[y][x] = ' ';
   return false;
}
 
int main ()
{
 
   Search(0, 0);
   print(Path, num_moves);
 
   return 0;
}

