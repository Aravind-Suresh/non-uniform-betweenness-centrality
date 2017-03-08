/**
* @Author:              aravind
* @Email:               arvindsuresh2009@gmail.com
* @Github:              [Aravind-Suresh](https://github.com/Aravind-Suresh)
* @Date:                2017-03-08T12:33:07+05:30
* @Last modified by:    aravind
* @Last modified time:  2017-03-08T12:33:07+05:30
*/

#include <bits/stdc++.h>

using namespace std;

class Graph {

private:
  int nodesCount, edgesCount;
  double totalBetweennessWeight;
  bool isWeightedVar, isDirectedVar;

  // Container for adjacency list
  // For every vertex 'u', adj[u] is a list of pairs (other vertex, weight)
  vector<vector<pair<int, int> > > adj;

  // Container for prior weights between pairs of vertices
  vector<vector<double> > betweennessWeights;

public:

  Graph(int n, string type, string mode);

  bool isDirected();

  bool isWeighted();

  void addEdge(int a, int b, int w);

  void summary();

  double getBetweennessWeight(int u, int v);

  void setBetweennessWeight(int u, int v, double w);

  double getBetweennessNorm(int v);

  void shortestPathDijkstra(int source, vector<vector<int> > & paths);

  void shortestPathBFS(int source, vector<vector<int> > & paths);

  void countShortestPathContribs(int source, int node, vector<vector<int> > & paths,
    map<pair<int, int>, pair<int, int> > & count);

  double computeWeightedBetweennessCentrality(int node);
};
