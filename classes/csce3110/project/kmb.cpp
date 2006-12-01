#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace boost;
using namespace std;

//graph_t is the adjacency_list typedef
typedef adjacency_list <vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits <Graph>::vertex_descriptor Vertex;
typedef graph_traits <Graph>::edge_descriptor Edge;
typedef pair<int, int> E;

void output_dot(string filename, Graph g, vector<int> d, vector<Vertex> p, property_map<Graph, edge_weight_t>::type weightmap)
{
   ofstream dot_file(filename.c_str());

   dot_file << "graph G {\n"
      << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

   graph_traits <Graph>::edge_iterator ei, ei_end;
   for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
   {
      graph_traits <Graph>::edge_descriptor e = *ei;
      graph_traits <Graph>::vertex_descriptor u = source(e, g), v = target(e, g);
      dot_file << u << " -- " << v
         << " [label=\"" << get(weightmap, e) << "\"";
      cout << u << " " << v << " " << p[v] << endl;
      if (p[v] == u)
         dot_file << ", color=\"black\"";
      else
         dot_file << ", color=\"grey\"";
      dot_file << "]\n";
   }
   dot_file << "}";
}

int main(int, char *[])
{
   const int num_nodes = 5;
   E edge_array[] = {
      E(0, 1), 
      E(0, 2), 
      E(0, 3), 
      E(1, 2), 
      E(1, 4), 
      E(2, 5), 
      E(3, 5), 
      E(3, 6),
      E(4, 5), 
      E(5, 6)
   };
   int weights[] = { 6, 2, 1, 3, 1, 1, 3, 3, 3, 2 };
   int num_arcs = sizeof(edge_array) / sizeof(E);
   Graph g(edge_array, edge_array + num_arcs, weights, num_nodes);
   map<E, int> path_info;
   property_map<Graph, edge_weight_t>::type weightmap = get(edge_weight, g);
   
   set<Vertex> starts;
   starts.insert(0);
   starts.insert(4);
   starts.insert(6);
   
   vector<Vertex> p(num_vertices(g));
   vector<int> d(num_vertices(g));
   map<E, list<Vertex>*> path_map;
   stack<int> path_stack;
   // Calculate Dijkstra's
   for (set<Vertex>::iterator i = starts.begin(); i != starts.end(); ++i)
   {
      Vertex s = vertex(*i, g);
      dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
      // Creating a target tracing back to start
      
      for (set<Vertex>::iterator q = starts.begin(); q != starts.end(); ++q)
      {
         Vertex cur = 0;
         if (*q != s)
         {
            cur = *q;
            path_stack.push(cur);    
            while (cur != s)
            {
               cur = p[cur];
               path_stack.push(cur);               
            }
         }
         // Reverse the order and then store for reuse later
         // If I thought this through it would probably not smack of a bad idea.
         if (!path_stack.empty())
         {
            cout << "Path" << endl;
            cout << "Size of stack " << path_stack.size() << endl;
            Vertex u = path_stack.top();
            Vertex v = cur;

            // PETE Here is a core dump!!
            while (!path_stack.empty())
            {
               v = path_stack.top();
               cout << v << " ";
               if (path_map.find(E(u, v)) == path_map.end())
                  path_map[E(u, v)] = new list<Vertex>;
               path_map[E(u, v)]->push_back(Vertex(v));
               path_stack.pop();
            }
         }
      }  
      
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

   cout << "Print the edges in the MST:" << endl;
   for (vector <Edge>::iterator ei = spanning_tree.begin(); ei != spanning_tree.end(); ++ei)
   {
      int u = source(*ei, mst_g);
      int v = target(*ei, mst_g);
      int distance = weight[*ei];
      // Construct a graph using these to / from points and grab the paths out
      // of path_map 
      //for final_path[E(u,v)]
   }
   
   // Run MST against this graph one more time
   
   // Generate the DOT file
   
   
   
   
/*
   ofstream fout("figs/kmb4.dot");
   fout << "graph A {\n"
      << " rankdir=LR\n"
      << " size=\"3,3\"\n"
      << " ratio=\"filled\"\n"
      << " edge[style=\"bold\"]\n" << " node[shape=\"circle\"]\n";
   graph_traits<graph_t>::edge_iterator eiter, eiter_end;
   for (tie(eiter, eiter_end) = edges(mst_g); eiter != eiter_end; ++eiter) {
      graph_traits < graph_t >::vertex_descriptor 
      u = source(*eiter, mst_g), v = target(*eiter, mst_g);
    fout << name[u] << " -- " << name[v];
    if (find(spanning_tree.begin(), spanning_tree.end(), *eiter)
        != spanning_tree.end())
      fout << "[color=\"black\", label=\"" << get(edge_weight, mst_g, *eiter)
           << "\"];\n";
    else
      fout << "[color=\"gray\", label=\"" << get(edge_weight, mst_g, *eiter)
           << "\"];\n";
  }
  fout << "}\n";
*/
  return EXIT_SUCCESS;
}
