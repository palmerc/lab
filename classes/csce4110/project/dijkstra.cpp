#include <list>
#include <map>
#include <limits>

class Dijkstra
{
	public:
		Dijkstra();
		
	private:
	  	typedef int vertex_t;
		typedef int weight_t;
		struct edge
		{
			vertex_t target;
			weight_t w;
			edge(vertex_t arg_target, weight_t arg_weight)
	        	: target(arg_target), w(arg_weight) { }
		};
		
		std::map<vertex_t, vertex_t> pi;
	    std::map<vertex_t, weight_t> min_w;
	    
		typedef std::map<vertex_t, std::list<edge> > adj_t;
		
		void initialize_single_source(adj_t, int);
		void compute_shortest_path();
};

Dijkstra::Dijkstra()
{
}

void Dijkstra::initialize_single_source(adj_t adj, int source)
{
	for (adj_t::iterator i = adj.begin(); i != adj.end(); ++i)
	{
		vertex_t v = i->first;
		min_w[v] = std::numeric_limits<int>::infinity();
		pi[v] = 0;
	}

	min_w[source] = 0;
}

relax(u, v, w)
{
	if (d[v] > d[u]+ w(u,v))
    {
	   d[v] = d[u] + w(u,v);
	   pi[v] = u;
    }
}

void Dijkstra::compute_shortest_path()
{
   Q = graph.V;
   while Q != 0
   {
      u = extract_min(Q);
      S = S union {u};
      foreach (vertex v element of Adj[u])
      {
         relax(u, v, w);
      }
   }
}

int main()
{
	std::map<vertex_t, vertex_t> pi;
   return 0;
}
