#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

struct AdjNode
{
   AdjNode() : node(0), weight(0) {}
   AdjNode(int _node, int _weight) : node(_node), weight(_weight) {}
   int node;
   int weight;
};
ostream& operator<< (ostream& LHS, AdjNode const& RHS) 
{ 
   LHS << "Node " << RHS.node << " Weight " << RHS.weight << endl;
   return LHS;
}

int main(int argc, char* argv[])
{
   ifstream in; 
   in.open(argv[1]);
    
   boost::regex re("^\\s*v\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::cmatch matches;
   map<int, list<AdjNode>*> AdjList;
   string str;
   
   getline(in, str);
   while ( in )
   {
      if (boost::regex_match(str.c_str(), matches, re))
      {
         int vertex = boost::lexical_cast<int>(matches[1]);
         int node = boost::lexical_cast<int>(matches[2]);
         int weight = boost::lexical_cast<int>(matches[3]);
         // if node doesn't exist, create a linked list and attach the next item
         if (AdjList.find(vertex) == AdjList.end()) // key doesn't exist
            AdjList[vertex] = new list<AdjNode>;
         // if it does exist just add the next item and weight to the appropriate node
         AdjList[vertex]->push_back(AdjNode(node, weight));
      }
      getline(in, str);
   }
   in.close();
   
   for (map<int, list<AdjNode>*>::const_iterator i = AdjList.begin(); i != AdjList.end(); ++i) 
   {
      cout << "VERTEX " << i->first << endl;
      copy(i->second->begin(), i->second->end(), ostream_iterator<AdjNode>(cout));
   }
    
   for (map<int, list<AdjNode>*>::iterator i = AdjList.begin(); i != AdjList.end(); ++i) 
      delete i->second;
   AdjList.clear();
   
   return 0;
}
