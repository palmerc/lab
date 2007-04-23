
initialize_single_source(G, s)
{
	foreach vertex v element of V[G]
	{
	   d[v] = inf;
	   pi[b] = nil;
	}
	d[s] = 0;
}

relax(u, v, w)
{
	if d[v] > d[u]+ w(u,v)
    {
	   d[v] = d[u] + w(u,v);
	   pi[v] = u;
    }
}

dijkstra_min_prio()
{
   initialize_single_source();
   s = 0;
   Q = graph.V;
   while Q != 0
      u = extract_min(Q);
      S = S union {u}
      foreach vertex v element of Adj[u]
         do Relax(u, v, w);
}
