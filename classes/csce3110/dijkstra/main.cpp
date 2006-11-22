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

typedef map<int, list<AdjNode>*> AdjList;
typedef map<int, Vertex> VertexList;

ostream& operator<< (ostream& LHS, Vertex const& RHS)
{
   LHS << "spe " << RHS.spe << " pi " << RHS.pi << endl;
   return LHS;
}

void print_out (VertexList S)
{
   // print out
   for (VertexList::iterator i = S.begin(); i != S.end(); ++i)
   {
      cout << i->first << " spe:" << i->second.spe << " pi:" << i->second.pi << endl;
   }
}

int extract_min(AdjList G, VertexList S, VertexList Q)
{
   cout << "Extract Min Status of S and Q" << endl;
   cout << "The S" << endl;
   print_out(S);
   cout << "The Q" << endl;
   print_out(Q);
   cout << endl;
   int minmin = INT_MAX;
   // Run through the list of known vertices in S

   cout << "The processing loop" << endl;
   int u, index, spe;
   for (VertexList::iterator i = S.begin(); i != S.end(); ++i)
   {
      //cout << "minmin " << minmin << endl;
      //cout << "i " << i->first << " SPE " << i->second.spe << " PI " << i->second.pi << endl;
      index = i->first; // index represents the u of an edge
      cout << "Vertex " << index <<endl;
      spe = i->second.spe;
      // Run through the adjacency list of the vertices in S
      for (list<AdjNode>::iterator j = G[index]->begin(); j != G[index]->end(); ++j)
      {
         //cout << "u" << index << " v" << j->node << " weight " << j->weight << endl;
         int total = j->weight + spe; // total is the edge weight + u's spe
         cout << "total "<< total << " < " << S[j->node].spe << endl;
         // if edge weight + u's spe < v's current spe
         if (total < S[j->node].spe)
         {
            // We want to return the lowest edge who's v isn't in S
            if ((total < minmin) && (S.find(j->node) == S.end()))
            //if (total < minmin)
            {
               cout << "BEFORE MINMIN " << minmin << endl;
               minmin = total;
               u = j->node;
            }               
            // update v's spe and pi with new values
            S[j->node].spe = total;
            S[j->node].pi = index;
            cout << "updating edge " << j->node << " spe " << total << " pi " << index << endl;
         }
      }
   }
   
   cout << "Extract min is returning " << u << endl;
   return u;
}

void dijkstra(AdjList G, int start)
{
   VertexList S; // Discovered shortest paths
   VertexList Q; // Pool of unknown vertices
   
   S[start] = Vertex(0, 0);
   for(AdjList::iterator i = G.begin(); i != G.end(); ++i)
      if (i->first != start)
         Q[i->first] = Vertex(INT_MAX, 0);
   while (!Q.empty())
   {
      cout << "******** TOP OF LOOP" << endl;
      int u = extract_min(G, S, Q);
      S[u] = Q[u];
      Q.erase(u);
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
   
   for (AdjList::iterator i = G.begin(); i != G.end() ; ++i)
      delete i->second;
   G.clear();

   return 0;
}
