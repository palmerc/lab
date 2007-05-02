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
	std::ifstream in;
	std::map<int, int> source_list;
	int rows;
	in.open(argv[1]);
	boost::regex re_rows("^\\s*Nodes\\s+(\\d+)\\s*$", boost::regex::perl);
	boost::regex re_vertex("^\\s*E\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
	boost::cmatch matches;
	std::string str;

	getline(in, str);
	while ( in )
	{
		if (boost::regex_match(str.c_str(), matches, re_rows))
			rows = boost::lexical_cast<int>(matches[1]);
		else if (boost::regex_match(str.c_str(), matches, re_vertex))
		{
			int vertex = boost::lexical_cast<int>(matches[1]);
			int node = boost::lexical_cast<int>(matches[2]);
			int weight = boost::lexical_cast<int>(matches[3]);
			// if it does exist just add the next item and distance to the appropriate node
			if(adj.find(node) == adj.end())
				adj[node].clear();
			adj[vertex].push_back(edge(node, weight));
			source_list[vertex] = 1;
		}
		getline(in, str);
	}
	in.close();
	for(std::map<int, int>::iterator i = source_list.begin(); i != source_list.end(); ++i)
	{
		Dijkstra graph(adj, i->first);
	}
	//graph.print_min();
	return 0;
}
