#include "Dijkstra.h"
#include <string>
#include <ostream>
#include <fstream>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char *argv[])
{
	adj_t adj;
	vertex_t source = 0;
	std::ifstream in;
	in.open(argv[1]);
	boost::regex re_start("^\\s*s\\s+(\\d+)\\s*$", boost::regex::perl);
	boost::regex re_vertex("^\\s*v\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
	boost::cmatch matches;
	std::string str;

	getline(in, str);
	while ( in )
	{
		if (boost::regex_match(str.c_str(), matches, re_start))
			source= boost::lexical_cast<int>(matches[1]);
		else if (boost::regex_match(str.c_str(), matches, re_vertex))
		{
			int vertex = boost::lexical_cast<int>(matches[1]);
			int node = boost::lexical_cast<int>(matches[2]);
			int weight = boost::lexical_cast<int>(matches[3]);
			// if it does exist just add the next item and distance to the appropriate node
			if(adj.find(node) == adj.end())
				adj[node].clear();
			adj[vertex].push_back(edge(node, weight));
		}
		getline(in, str);
	}
	in.close();
	Dijkstra graph(adj, source);
	graph.print_min();
	return 0;
}
