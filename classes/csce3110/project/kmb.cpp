#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

//graph_t is the adjacency_list typedef
typedef adjacency_list <vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits <Graph>::edge_descriptor Edge;
typedef pair<int, int> E;

int main(int argc, char* argv[])
{
   
   ifstream in;
   ofstream dot_file;
   
   if (argc < 2)
   {
      cout << "Usage: " << argv[0] << " <input graph> <dot_output file>" << endl << endl;
      return EXIT_FAILURE;
   }
   
   string infile = argv[1];
   string outfile = argv[2];
   in.open(infile.c_str());
   dot_file.open(outfile.c_str());
   
   boost::regex re_nodes("^\\s*Nodes\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::regex re_dest("^\\s*T\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::regex re_vertex("^\\s*E\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::regex re_pos("^\\s*DD\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s*$", boost::regex::perl);
   boost::cmatch matches;
   
   int nodes = 0;
   vector<int> weights;
   set<int> starts;
   vector<E> edge_array;
   map<int, E> pos_info;
   
   string str;
   getline(in, str);
   while ( in )
   {
      if (boost::regex_match(str.c_str(), matches, re_nodes))
         nodes = boost::lexical_cast<int>(matches[1]);
      else if (boost::regex_match(str.c_str(), matches, re_dest))
         starts.insert(boost::lexical_cast<int>(matches[1]));
      else if (boost::regex_match(str.c_str(), matches, re_pos))
      {
         int node = boost::lexical_cast<int>(matches[1]);
         int x = boost::lexical_cast<int>(matches[2]);
         int y = boost::lexical_cast<int>(matches[3]);
         pos_info[node] = E(x, y);
      }  
      else if (boost::regex_match(str.c_str(), matches, re_vertex))
      {
         int u = boost::lexical_cast<int>(matches[1]);
         int v = boost::lexical_cast<int>(matches[2]);
         int distance = boost::lexical_cast<int>(matches[3]);
         // if node doesn't exist, create a linked list and attach the next item
         edge_array.push_back(E(u, v));
         weights.push_back(distance);
         // if it does exist just add the next item and distance to the appropriate node
      }
      getline(in, str);
   }
   in.close();
      
   const int num_nodes = nodes;
   //E edge_array[] = {
   //   E(0, 1),
   //   E(0, 2),
   //   E(0, 3), 
   //   E(1, 2), 
   //   E(1, 4), 
   //   E(2, 5), 
   //   E(3, 5), 
   //   E(3, 6),
   //   E(4, 5), 
   //   E(5, 6)
   //};
   //int weights[] = { 6, 2, 1, 3, 1, 1, 3, 3, 3, 2 };
   //int num_arcs = sizeof(edge_array) / sizeof(E);
   size_t num_arcs = edge_array.size();
   Graph g(&edge_array[0], &edge_array[0] + num_arcs, &weights[0], num_nodes);
   map<E, int> path_info;
   property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
   
   //starts.insert(0);
   //starts.insert(4);
   //starts.insert(6);
   
   vector<Vertex> p(num_vertices(g));
   vector<int> d(num_vertices(g));
   map<E, list<int>*> path_map;
   stack<int> path_stack;
   // Calculate Dijkstra's
   // For each starting vertex run Dijkstra's once
   for (set<int>::iterator i = starts.begin(); i != starts.end(); ++i)
   {
      int s = vertex(*i, g); // This chnages the starting node
      dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
      
      // Creating a target tracing back to start
      // Iterate through the set of possible starts and label them q
      int cur, end = 0; // current will be reassinged from the end vertex back to the start
      for (set<int>::iterator q = starts.begin(); q != starts.end(); ++q)
      {
         if (*q != s) // Avoid the start. No point in going to myself
         {
            cur = *q;
            end = cur;
            path_stack.push(cur); // push the ending point on the stack
            while (cur != s) // keep pushing previous nodes until you reach the start
            {
               cur = p[cur];
               path_stack.push(cur);
            }
         }
         
         // Now that the order has been pushed onto the stack we can and then store for reuse later
         // If I thought this through it would probably not smack of a bad idea.
         if (!path_stack.empty())
         {
            int u = path_stack.top();
            int v = end;
            while (!path_stack.empty())
            {
               cur = path_stack.top();
               
               if (path_map.find(E(u, v)) == path_map.end())
               {
                  //cout << "new list for E(" << u << ", " << v << ")" << endl;
                  path_map[E(u, v)] = new list<int>;
               }
               path_map[E(u, v)]->push_back(Vertex(cur));
               path_stack.pop();
            }
            //cout << "Steps ";
            //for (list<int>::iterator i = path_map[E(u, v)]->begin(); i != path_map[E(u, v)]->end(); ++i)
            //   cout << *i << " ";
            //cout << endl << endl;
         }
      }  
      
      // Create path_info which is a map of edges and their distance.
      graph_traits <Graph>::vertex_iterator vi, vend;
      for (tie(vi, vend) = vertices(g); vi != vend; ++vi)
      {
         int u = s;
         int v = *vi;
         int distance = d[*vi];
         if ( starts.find(v) != starts.end() )
            path_info[E(u, v)] = distance;
      }
   }
   
   // Construct the new graph
   int const mst_num_nodes = starts.size();
   
   vector<E> mst_edge_array;
   vector<int> mst_weights;
   for (map<E, int>::const_iterator i = path_info.begin(); i != path_info.end(); ++i)
   {
      int u = i->first.first;
      int v = i->first.second;
      int distance = i->second;
      mst_edge_array.push_back(E(u, v));
      mst_weights.push_back(distance);
   }
   size_t mst_num_edges = mst_edge_array.size();
   Graph mst_g(&mst_edge_array[0], &mst_edge_array[0] + mst_num_edges, &mst_weights[0], mst_num_nodes);
   property_map <Graph, edge_weight_t >::type weight = get(edge_weight, mst_g);
   vector <Edge> spanning_tree;

   // Calculate the MST
   kruskal_minimum_spanning_tree(mst_g, back_inserter(spanning_tree));

   // MST Printing begins here

   map<E, int> kmb_edges_map;
   for (vector<Edge>::iterator ei = spanning_tree.begin(); ei != spanning_tree.end(); ++ei)
   {
      int u = source(*ei, mst_g);
      int v = target(*ei, mst_g);
      int distance = weight[*ei];
      //cout << "Edge(" << u << ", " << v << ")" << endl;
      //cout << "Steps ";
      int start = u, finish;
      
      for (list<int>::iterator i = path_map[E(u, v)]->begin(); i != path_map[E(u, v)]->end(); ++i)
      {
         finish = *i;
         if (finish != start) 
            kmb_edges_map[E(start, finish)] = distance;
         start = finish;
      }
         
      //cout << endl << endl;
   }
   
   // Run MST against this graph one more time
   
   // Generate the DOT file
   dot_file << "graph G {\n"
      << "  node[shape=\"circle\"]\n";

   graph_traits <Graph>::edge_iterator ei, ei_end;
   for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
   {
      graph_traits <Graph>::edge_descriptor e = *ei;
      graph_traits <Graph>::vertex_descriptor u = source(e, g), v = target(e, g);
      dot_file << u << " -- " << v
         << " [label=\"" << get(weightmap, e) << "\"";
      if (pos_info.find(u) != pos_info.end())
         dot_file << ", pos=\"" << pos_info[u].first << "," << pos_info[u].second << "\"";
      //cout << u << " " << v << " " << p[v] << endl;
      if (kmb_edges_map.find(E(u, v)) != kmb_edges_map.end() || kmb_edges_map.find(E(v, u)) != kmb_edges_map.end())
         dot_file << ", color=\"black\", style=bold, weight=10";
      else
         dot_file << ", color=\"grey\"";
      dot_file << "]\n";
   }
   dot_file << "}";
  
   return EXIT_SUCCESS;
}
