#include "dijkstra.h"

int main(int argc, int argv[argc])
{
   adj_t adj;
   vertex_t source = 0;
   adj[0].push_back(edge(1,10));
   adj[0].push_back(edge(3,5));
   adj[1].push_back(edge(2,1));
   adj[1].push_back(edge(3,2));
   adj[2].push_back(edge(4,4));
   adj[3].push_back(edge(1,3));
   adj[3].push_back(edge(2,9));
   adj[3].push_back(edge(4,2));
   adj[4].push_back(edge(0,7));
   adj[4].push_back(edge(2,6));

   Dijkstra graph(adj, source);
   graph.print_min();
   return 0;
}
