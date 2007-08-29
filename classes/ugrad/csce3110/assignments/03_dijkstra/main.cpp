/*
Cameron L Palmer
Dijkstra's algorithm

Instructions:
To compile: g++ -g -o main main.cpp
To run: ./main <orig.graph>
*/

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
   Vertex() : spe(INT_MAX), pi(0), known(0) {}
   Vertex(int _spe, int _pi, int _known) :
      spe(_spe), pi(_pi), known(_known) {}
   int spe;
   int pi;
   int known;
};
int loop = 0;
typedef map<int, list<AdjNode>*> AdjList;
typedef map<int, Vertex> VertexList;

ostream& operator<< (ostream& LHS, Vertex const& RHS)
{
   LHS << "spe " << RHS.spe << " pi " << RHS.pi << endl;
   return LHS;
}

void print_out (VertexList V)
{
   // print out
   for (VertexList::iterator i = V.begin(); i != V.end(); ++i)
   {
      cout << i->first << " spe:" << i->second.spe << " pi:" << i->second.pi << endl;
   }
}

int extract_min(AdjList &G, VertexList &V, list<int> const &S)
{
//   cout << "Extract Min Status of S and Q" << endl;
//   cout << "The S" << endl;
//   print_out(S);
//   cout << "The Q" << endl;
//   print_out(Q);
//   cout << endl;
   int minmin = INT_MAX;
   // Run through the list of known vertices in S

   //cout << "The processing loop" << endl;
   int u, spe;
   for (list<int>::const_iterator i = S.begin(); i != S.end(); ++i)
   {
      //cout << "minmin " << minmin << endl;
      //cout << "i " << i->first << " SPE " << i->second.spe << " PI " << i->second.pi << endl;
      // i represents the u of an edge
      //cout << "Vertex " << *i <<endl;
      spe = V[*i].spe;
      // Run through the adjacency list of the vertices in S
      for (list<AdjNode>::iterator j = G[*i]->begin(); j != G[*i]->end(); ++j)
      {
         ++loop;
         int node = j->node;
         //cout << "Size of S " << S.size() << endl;
         int total = j->weight + spe; // total is the edge weight + u's spe
         //cout << "u" << *i << " v" << node << " weight " << total << endl;

         //cout << V[node].known << " From " << *i << " to " << node << " total "<< total << " < " << V[node].spe << endl;
         // if edge weight + u's spe < v's current spe
         if (total < V[node].spe || (V[node].known == 0))
         {
            // update v's spe and pi with new values
            V[node].spe = total;
            V[node].pi = *i;
            //cout << "updated " << *i << " edge to " << node << " spe " << total << " pi " << *i << endl;
            // We want to return the lowest edge who's v isn't in S
            //if ((total < minmin) && (V[node].known != 1))
            if (total < minmin && (V[node].known == 0))
            {
               minmin = total;
               u = node;
            }               
         }
      }
   }
   return u;
}

void dijkstra(AdjList &G, int start)
{
   VertexList V;
   list<int> S; // Discovered shortest paths
   list<int> Q; // Pool of unknown vertices
   
   V[start] = Vertex(0, 0, 1);
   S.push_back(start);
   for(AdjList::iterator i = G.begin(); i != G.end(); ++i)
      if (i->first != start)
      {
         V[i->first] = Vertex(INT_MAX, 0, 0);
         Q.push_back(i->first);
      }
   cout << "Shortest path discovered " << start << endl;
   while (!Q.empty())
   {
      int u = extract_min(G, V, S);
      cout << "Shortest path discovered " << u << endl;
      V[u].known = 1;
      S.push_back(u);
      Q.remove(u);
   }
   print_out(V);
}

int main(int argc, char* argv[])
{
   ifstream in;
   in.open(argv[1]);
   boost::regex re_start("^\\s*s\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::regex re_vertex("^\\s*v\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::cmatch matches;
   AdjList G;
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
         if (G.find(vertex) == G.end()) // key doesn't exist
            G[vertex] = new list<AdjNode>;
         // if it does exist just add the next item and distance to the appropriate node
         G[vertex]->push_back(AdjNode(node, weight));
      }
      getline(in, str);
   }
   in.close();

   dijkstra(G, start);
   cout << "Loops " << loop << endl;
   for (AdjList::iterator i = G.begin(); i != G.end() ; ++i)
      delete i->second;
   G.clear();

   return 0;
}
