#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>

using namespace std;

int main(int argc, char* argv[])
{
   boost::regex re("^\\s*v\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::cmatch matches;
   std::string str;
   // So yes argv is there
   for (int i = 0; i < argc; i++)
      cout << argv[i] << endl;
   
   ifstream in;
   
   in.open(argv[1]);
   
   std::string vertex, next, weight;
   int i = 1;
   getline(in, str);
   while ( in )
   {
      if (boost::regex_match(str.c_str(), matches, re))
      {
         vertex = matches[1];
         next = matches[2];
         weight = matches[3];
         cout << "Line: " << i << " Vertex: " << vertex << " Next: " << next << " Weight: " << weight << endl;
      }
      getline(in, str);
      i++;
   }
   in.close();
   return 0;
}
