#include "MinHeap.h"
#include <list>
#include <map>
#include <vector>
#include <limits>
#include <iostream>

typedef int vertex_t;
typedef int weight_t;
typedef std::map<vertex_t, vertex_t> pi_t;
typedef std::map<vertex_t, weight_t> min_w_t;
struct edge
{
	vertex_t target;
	weight_t w;
	edge(vertex_t arg_target, weight_t arg_weight)
    	: target(arg_target), w(arg_weight) { }
};	    
typedef std::map<vertex_t, std::list<edge> > adj_t;
class Dijkstra
{	
	private:
		vertex_t s;
		std::vector<vertex_t> S;
		min_w_t min_w;
		pi_t pi;
		MinHeap Q;
		unsigned int rounds;
	
		void initialize_single_source();
		void compute_shortest_path();
		void relax(int, int, int);
	public:
		adj_t adj;
		Dijkstra(adj_t, vertex_t);
		void set_source(vertex_t);
		void print_min();
};

Dijkstra::Dijkstra(adj_t new_adj, vertex_t new_s)
{
	set_source(new_s);
	adj = new_adj;
	
	initialize_single_source();
	compute_shortest_path();
}

void Dijkstra::set_source(vertex_t v)
{
	s = v;
}

void Dijkstra::print_min()
{
	std::cout << "Completed with " << rounds << " rounds" << std::endl;
	for (min_w_t::iterator i = min_w.begin(); i != min_w.end(); ++i)
	{
		std::cout << "Vertex " << i->first << " distance " << i->second << std::endl;
	}
}

void Dijkstra::initialize_single_source()
{
	for (adj_t::iterator i = adj.begin(); i != adj.end(); ++i)
	{
		vertex_t v = i->first;
		min_w[v] = std::numeric_limits<int>::max();
		//min_w[v] = 10000;
		pi[v] = 0;
	}
	min_w[s] = 0;
}
void Dijkstra::relax(int u, int v, int w)
{
	if (min_w[v] > w)
	{
		min_w[v] = w;
		Q.decrease_key(v, w);
		pi[v] = u;
	} 
}
void Dijkstra::compute_shortest_path()
{
	// Load the Q (not really) with the vertices
	for (min_w_t::iterator i = min_w.begin(); i != min_w.end(); ++i)
	{
		std::cout << "Q insertion " << i->first << " " << i->second << std::endl;
		Q.insert(i->first, i->second);
	}
	// Run until your Q (not really) is out of vertices.
	rounds = 1;
	while (!Q.empty())
	{
		//std::cout << "Round " << rounds << std::endl;
		vertex_t u = Q.extract_min();
		S.push_back(u);
		std::list<edge> targets = adj[u];
		// Go through each target for the current vertex
		for(std::list<edge>::iterator i = targets.begin(); i != targets.end(); ++i)
		{
			vertex_t v = (*i).target;
			weight_t w = min_w[u] + (*i).w;
			relax(u, v, w);		
			rounds++;
		}
		rounds++;
	}		
}
