#include <cstdlib>
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
   AdjNode() : node(0), weight(0), pi(0), spe(0), used(0) {}
   AdjNode(int _node, int _weight, int _pi, int _spe, int _used) : 
      node(_node), weight(_weight), pi(_pi), spe(_spe), used(_used) {}
   int node;
   int weight;
   int pi;
   int spe; // shortest-path estimate
   int used;
};

typedef map<int, list<AdjNode>*> mappy;

ostream& operator<< (ostream& LHS, AdjNode const& RHS)
{
   LHS << "Node " << RHS.node << " weight " << RHS.weight << endl;
   return LHS;
}

void print_out (mappy G, list<int> S)
{
   // print out
   cout << endl << "Running through S list" << endl; 
   for (list<int>::iterator i = S.begin(); i != S.end(); ++i)
   {
      for (list<AdjNode>::iterator j = G[*i]->begin(); j != G[*i]->end(); ++j)
         cout << *i << " to " << j->node << " - weight: " << j->weight 
            << ", pi: " << j->pi << ", spe: " << j->spe << endl;
   }
}

int extract_min(mappy G, list<int> S)
{
   // G is the adjacency list
   // S is the list of vertices we know the shortest path for
   // Q is the remaining vertices we don't know the shortest path for
   int min = INT_MAX;
   int min_node;
   // Scan the from the S set for an edge to an item in Q with the smallest weight
   for (list<int>::iterator i = S.begin(); i != S.end(); ++i)
      for (list<AdjNode>::iterator j = G[*i]->begin(); j != G[*i]->end(); ++j)
         if ((j->spe < min) && (j->used != 1))
         {
            j->used = 1; // I am replacing V-S = Q with this variable
            min_node = j->node;
         }
   // Return the vertex that has the lowest SPE
   return min_node;
}

void relax(mappy G, list<int> S, int u)
{
   int pi_spe = 0;
   for (list<AdjNode>::iterator i = G[S.back()]->begin(); i != G[S.back()]->end(); ++i)
   {
      if (i->node == u)
      {
         pi_spe = i->spe;
         cout << pi_spe << endl;
         break;
      }
   }
   for (list<AdjNode>::iterator j = G[u]->begin(); j != G[u]->end(); ++j)
      j->spe = j->weight + pi_spe;
}

void dijkstra(mappy G, int start)
{
   list<int> S; // The list of vertices we have got shortest paths for
   list<int> Q; // The list of all verticies

   // Initialize Q with all the vertices in G except the starting node
   for (mappy::const_iterator i = G.begin(); i != G.end(); ++i)
   {
      int vertex = i->first;
      if (vertex != start)
         Q.push_back(vertex);
   }
   // Initialize S with the starting node
   S.push_back(start);
   int u = 0;
  
   while (!Q.empty())
   {
      // u is a vertex
      u = extract_min(G, S);
      cout << "Moving into S " << u << endl;
      S.push_back(u);
      Q.remove(u);
      relax(G, S, u);
   }
   
   print_out(G, S);
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
         AdjList[vertex]->push_back(AdjNode(node, weight, 0, 0, 0));
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
