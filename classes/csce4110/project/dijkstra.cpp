#include "dijkstra.h"

typedef int vertex_t;
typedef int weight_t;
struct edge
{
	vertex_t target;
	weight_t w;
	edge(vertex_t arg_target, weight_t arg_weight)
    	: target(arg_target), w(arg_weight) { }
};	    
typedef std::map<vertex_t, std::list<edge> > adj_t;
int main()
{
   adj_t adj;
   vertex_t source = 0;
   adj[1].push_back(edge(2,10));
   adj[1].push_back(edge(4,5));
   adj[2].push_back(edge(3,1));
   adj[2].push_back(edge(4,2));
   adj[3].push_back(edge(5,4));
   adj[4].push_back(edge(2,3));
   adj[4].push_back(edge(3,9));
   adj[4].push_back(edge(5,2));
   adj[5].push_back(edge(1,7));
   adj[5].push_back(edge(3,6));

   Dijkstra graph(adj, source);
   return 0;
}
