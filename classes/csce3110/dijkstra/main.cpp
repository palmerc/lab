#include <cstdlib>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

struct AdjNode
{
   AdjNode() : node(0), distance(0) {}
   AdjNode(int _node, int _distance) : node(_node), distance(_distance) {}
   int node;
   int distance;
};

typedef map<int, list<AdjNode>*> mappy;

ostream& operator<< (ostream& LHS, AdjNode const& RHS)
{
   LHS << "Node " << RHS.node << " distance " << RHS.distance << endl;
   return LHS;
}

void dijkstra(mappy G, int start)
{
   cout << "START " << start << endl;

   list<int> S;
   list<int> Q;
   S.push_back(start);
   for (mappy::const_iterator i = G.begin(); i != G.end(); ++i)
   {
      int vertex = i->first;
      if (vertex != start)
         Q.push_back(vertex);
      cout << "VERTEX " << vertex << endl;
      copy(i->second->begin(), i->second->end(), ostream_iterator<AdjNode>(cout));
   }
   // We are going to run through the items in the discovered queue and
   // find the shortest path, then move it to the discovered queue
   // iterate throught the S and see which is the shortest undiscovered path
   for (list<int>::iterator i = S.begin(); i != S.end(); ++i)
   {
      // PETE I know this is wrong
      for (list<AdjNode>::iterator j = G[*i]->begin(); j != G[*i]->end(); ++j)
         cout << j->distance;
   }
}

int main(int argc, char* argv[])
{
   ifstream in;
   in.open(argv[1]);
   boost::regex re_start("^\\s*s\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::regex re_vertex("^\\s*v\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::cmatch matches;
   mappy AdjList;
   int start;
   string str;

   getline(in, str);
   while ( in )
   {
      if (boost::regex_match(str.c_str(), matches, re_start))
         start = boost::lexical_cast<int>(matches[1]);
      else if (boost::regex_match(str.c_str(), matches, re_vertex))
      {
         int vertex = boost::lexical_cast<int>(matches[1]);
         int node = boost::lexical_cast<int>(matches[2]);
         int distance = boost::lexical_cast<int>(matches[3]);
         // if node doesn't exist, create a linked list and attach the next item
         if (AdjList.find(vertex) == AdjList.end()) // key doesn't exist
            AdjList[vertex] = new list<AdjNode>;
         // if it does exist just add the next item and distance to the appropriate node
         AdjList[vertex]->push_back(AdjNode(node, distance));
      }
      getline(in, str);
   }
   in.close();

   dijkstra(AdjList, start);

   for (mappy::iterator i = AdjList.begin(); i != AdjList.end() ; ++i)
      delete i->second;
   AdjList.clear();

   return 0;
}
