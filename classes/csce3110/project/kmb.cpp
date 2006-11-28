#include <boost/config.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace boost;
using namespace std;

//graph_t is the adjacency_list typedef
typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > > graph_t;
typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
typedef pair<int, int> Edge;
  
void output_dot(string filename, graph_t g, vector<int> d, vector<vertex_descriptor> p, property_map<graph_t, edge_weight_t>::type weightmap, char name[])
{

  cout << "distances and parents:" << endl;
  graph_traits < graph_t >::vertex_iterator vi, vend;
  for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
    cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << endl;
  }
  cout << endl;

  ofstream dot_file(filename.c_str());

  dot_file << "digraph D {\n"
    << "  rankdir=LR\n"
    << "  size=\"4,3\"\n"
    << "  ratio=\"fill\"\n"
    << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

  graph_traits < graph_t >::edge_iterator ei, ei_end;
  for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei) {
    graph_traits < graph_t >::edge_descriptor e = *ei;
    graph_traits < graph_t >::vertex_descriptor
      u = source(e, g), v = target(e, g);
    dot_file << name[u] << " -> " << name[v]
      << "[label=\"" << get(weightmap, e) << "\"";
    if (p[v] == u)
      dot_file << ", color=\"black\"";
    else
      dot_file << ", color=\"grey\"";
    dot_file << "]";
  }
  dot_file << "}";
}

int main(int, char *[])
{
  const int num_nodes = 5;
  enum nodes { A, B, C, D, E, F, G };
  char name[] = "ABCDEFG";
  Edge edge_array[] = { Edge(A, B), Edge(B, A), Edge(A, C), Edge(C, A), 
     Edge(A, D), Edge(D, A), Edge(B, C), Edge(C, B), Edge(B, E), Edge(E, B),
     Edge(C, F), Edge(F, C), Edge(D, F), Edge(F, D), Edge(D, G), Edge(G, D),
     Edge(E, F), Edge(F, E), Edge(F, G), Edge(G, F)
  };
  int weights[] = { 6, 6, 2, 2, 1, 1, 3, 3, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 2, 2 };
  
  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  
  graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
  
  property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
  
  vector<vertex_descriptor> p(num_vertices(g));
  vector<int> d(num_vertices(g));

  // Calculate Dijkstra's
  vertex_descriptor s = vertex(A, g);
  dijkstra_shortest_paths(g, s, predecessor_map(&p[0]).distance_map(&d[0]));
  cout << "Starting point " << name[s] << endl;
  cout << " to E " << d[4] << endl;
  cout << " to G " << d[6] << endl;
  output_dot("figs/kmb1.dot", g, d, p, weightmap, name);
  
  vertex_descriptor t = vertex(E, g);
  dijkstra_shortest_paths(g, t, predecessor_map(&p[0]).distance_map(&d[0]));
  cout << "Starting point " << name[t] << endl;
  cout << " to A " << d[0] << endl;
  cout << " to G " << d[6] << endl;
  output_dot("figs/kmb2.dot", g, d, p, weightmap, name);
     
  vertex_descriptor u = vertex(G, g);
  dijkstra_shortest_paths(g, u, predecessor_map(&p[0]).distance_map(&d[0]));
  cout << "Starting point " << name[u] << endl;
  cout << " to A " << d[0] << endl;
  cout << " to E " << d[4] << endl;
  output_dot("figs/kmb3.dot", g, d, p, weightmap, name);
  
  
  // Construct the new graph
  const int mst_num_nodes = 3;
  Edge mst_edge_array[] = { Edge(A, E), Edge(A, G), Edge(E, G)
  };
  int mst_weights[] = { 6, 4, 5 };
  size_t mst_num_edges = sizeof(mst_edge_array) / sizeof(Edge);
  graph_t mst_g(mst_edge_array, mst_edge_array + mst_num_edges, mst_weights, mst_num_nodes);
  property_map < graph_t, edge_weight_t >::type weight = get(edge_weight, mst_g);
  vector < edge_descriptor > spanning_tree;

  // Calculate the MST
  kruskal_minimum_spanning_tree(mst_g, back_inserter(spanning_tree));
  
  // MST Printing begins here
  cout << "Print the edges in the MST:" << endl;
  for (vector < edge_descriptor >::iterator ei = spanning_tree.begin();
       ei != spanning_tree.end(); ++ei) {
    cout << source(*ei, mst_g) << " <--> " << target(*ei, mst_g)
      << " with weight of " << weight[*ei]
      << endl;
  }

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
  
  return EXIT_SUCCESS;
}
