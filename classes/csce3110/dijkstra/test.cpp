#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
   // So yes argv is there
   for (int i = 0; i < argc; i++)
      cout << argv[i] << endl;
   
   ifstream in;
   
   in.open(argv[1]);
   while (in.good())
      cout << (char) in.get();
   
   in.close();
   return 0;
}
