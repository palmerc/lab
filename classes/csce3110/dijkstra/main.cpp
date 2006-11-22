#include <cstdlib>
#include <climits>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <queue>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

struct AdjNode
{
   AdjNode() : node(0), weight(0) {}
   AdjNode(int _node, int _weight) : 
      node(_node), weight(_weight) {}
   int node;
   int weight;
};

struct Vertex
{
   Vertex() : spe(INT_MAX), pi(0) {}
   Vertex(int _spe, int _pi) :
      spe(_spe), pi(_pi) {}
   int spe;
   int pi;
};

typedef map<int, list<AdjNode>*> mappy;

ostream& operator<< (ostream& LHS, Vertex const& RHS)
{
   LHS << "spe " << RHS.spe << " pi " << RHS.pi << endl;
   return LHS;
}

void print_out (map<int, Vertex> S)
{
   // print out
   cout << endl << "Running through S list" << endl;
   for (map<int, Vertex>::iterator i = S.begin(); i != S.end(); ++i)
   {
      cout << endl;
   }
}

void dijkstra(mappy G, int start)
{
   map<int, Vertex> S; // Discovered shortest paths
   map<int, Vertex> Q; // Pool of unknown vertices
   
   for(mappy::iterator i = G.begin(); i != G.end(); ++i)
      if (i->first != start)
         Q[i->first] = Vertex(INT_MAX, 0);
   S[start] = Vertex(0, 0);
   
   int i=2;
   while (!Q.empty())
   {
      S[i] = Q[i];
      cout << Q[i].spe << endl;
      Q.erase(i);
      ++i;
   }
   print_out(S);
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
         int weight = boost::lexical_cast<int>(matches[3]);
         // if node doesn't exist, create a linked list and attach the next item
         if (AdjList.find(vertex) == AdjList.end()) // key doesn't exist
            AdjList[vertex] = new list<AdjNode>;
         // if it does exist just add the next item and distance to the appropriate node
         AdjList[vertex]->push_back(AdjNode(node, weight));
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
