#include <list>
#include <map>
#include <vector>
#include <limits>
#include <iostream>

class Dijkstra
{
	typedef int vertex_t;
	typedef int weight_t;
	private:
		struct edge
		{
			vertex_t target;
			weight_t w;
			edge(vertex_t arg_target, weight_t arg_weight)
	        	: target(arg_target), w(arg_weight) { }
		};
		vertex_t s;
		typedef std::map<vertex_t, vertex_t> pi_t;
		typedef std::map<vertex_t, weight_t> min_w_t;
	    
		typedef std::map<vertex_t, std::list<edge> > adj_t;
		min_w_t min_w;
		pi_t pi;
		adj_t adj;
		
		
		void initialize_single_source();
		void compute_shortest_path();
	public:
		Dijkstra();
		void set_source(vertex_t);
		void print_min();
};

Dijkstra::Dijkstra()
{
}

void Dijkstra::set_source(vertex_t v)
{
	s = v;
}

void Dijkstra::print_min()
{
	for (min_w_t::iterator i = min_w.begin(); i != min_w.end(); ++i)
	{
		std::cout << i->first << " distance " << i->second << std::endl;
	}
}

void Dijkstra::initialize_single_source()
{
	for (adj_t::iterator i = adj.begin(); i != adj.end(); ++i)
	{
		vertex_t v = i->first;
		min_w[v] = std::numeric_limits<int>::infinity();
		pi[v] = 0;
	}

	min_w[s] = 0;
}

void Dijkstra::compute_shortest_path()
{
	vertex_t lowest;
	// Load the Q (not really) with the vertices
	std::vector<vertex_t> Q;
	for (adj_t::iterator i = adj.begin(); i != adj.end(); ++i)
	{
		Q.push_back(i->first);
	}
	// Start by setting the source node cost to zero.
	min_w[s] = 0;
	// set the initial vertex to source.
	vertex_t v = s;
	// Run until your Q (not really) is out of vertices.
	while (!Q.empty())
	{
		std::list<edge> targets = adj[v];
		// Go through each target for the current vertex
		for(std::list<edge>::iterator t = targets.begin(); t != targets.end(); ++t)
		{
			int cost = min_w[v] + t.w;
			if (cost < min_w[*t.target])
			{
				min_w[*t.target] = cost;
				pi[*t.target] = v;
				if (cost < min_w[lowest])
				{
					lowest = v;
				}
			}
		}
		Q.erase(v);
		v = lowest;
	}		
}

int main()
{
	std::map<vertex_t, vertex_t> pi;
   return 0;
}
