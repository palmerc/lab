#include <limits>
#include <map>
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

int adj[1000][1000];
int rows;
int min(int x, int y)
{
	return ((x) < (y) ? (x) : (y));
}

void FloydWarshall()
{
	for (int k = 0; k < rows; ++k)
    	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < rows; ++j)
			{
				adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
			}
		}
    	}
}

int main(int argc, char *argv[])
{
	std::ifstream in;
	in.open(argv[1]);
	boost::regex re_rows("^\\s*r\\s+(\\d+)\\s*$", boost::regex::perl);
	boost::regex re_vertex("^\\s*v\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
	boost::cmatch matches;
	std::string str;

	getline(in, str);
	
	for (int i = 0; i < 1000; ++i)
	{
		for (int j = 0; j < 1000; ++j)
		{
			adj[i][j] = 100000;
		}
	} 
	while ( in )
	{	
		if (boost::regex_match(str.c_str(), matches, re_rows))
		{
			rows = boost::lexical_cast<int>(matches[1]);
		}
		else if (boost::regex_match(str.c_str(), matches, re_vertex))
		{
			int vertex = boost::lexical_cast<int>(matches[1]);
			int node = boost::lexical_cast<int>(matches[2]);
			int weight = boost::lexical_cast<int>(matches[3]);
			// if it does exist just add the next item and distance to the appropriate node
			adj[node][vertex] = weight;
		}
		getline(in, str);
	}
	in.close();
	FloydWarshall();
	
	for (int i=0; i < rows; ++i)
    {
	   for (int j=0; j < rows; ++j)
	   {
		if (i == j)
			std::cout << "0 ";
		else
			std::cout << adj[i][j] << " ";
       }
       std::cout << std::endl;
    }
	return 0;
}
