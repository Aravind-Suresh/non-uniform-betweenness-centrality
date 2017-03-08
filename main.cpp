/**
* @Author:              aravind
* @Email:               arvindsuresh2009@gmail.com
* @Github:              [Aravind-Suresh](https://github.com/Aravind-Suresh)
* @Date:                2017-03-05T16:45:13+05:30
* @Last modified by:    aravind
* @Last modified time:  2017-03-08T12:28:58+05:30
*/

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef pair<int, int> pii;
typedef vector<pii > vii;
typedef vector<vector<int> > vvi;
typedef vector<vii > vvii;

#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define rep(i,s,e) for(int i=(s);i<(e);++i)
#define repr(i,s,e) for(int i=(e);i>(s);--i)

const double INF = 1e9;

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

  Graph(int n, string type, string mode) {
    // Constructing the graph
    nodesCount = n;
    adj.resize(n+1);
    edgesCount = 0;
    totalBetweennessWeight = 0.0;

    isDirectedVar = (type == "directed");
    isWeightedVar = (mode == "weighted");

    // Initialising betweennessWeights
    betweennessWeights.resize(n+1);
    for(int i = 1; i <= n; ++i) {
      betweennessWeights[i].resize(n+1);
      for(int j = 1; j <= n; ++j) {
        betweennessWeights[i][j] = 0.0;
      }
    }
  }

  bool isDirected() {
    /**
     * Returns whether the graph is directed or not
     */
    return isDirectedVar;
  }

  bool isWeighted() {
    /**
     * Returns whether the graph is weighted or not
     */
    return isWeightedVar;
  }

  void addEdge(int a, int b, int w) {
    /**
     * Utility function to add a directed edge from a->b with a weight w.
     *
     * Undirected graphs can be constructed by using 2 edges, i.e from a->b and from b->a.
     *
     * Unweighted graphs can be constructed by using a non-zero w ( say w = 1 ) for all edges.
     *
     * :param a   Edge from 'a'
     * :param b   Edge to 'b'
     * :param w   Weight of the edge
     */
    // Directed edge from a->b with a weight w
    adj[a].push_back(make_pair(b, w));

    // Incrementing edgesCount
    ++edgesCount;
  }

  void summary() {
    /**
     * Utility function for summarising graph info
     */
    cout << "Graph summary" << endl;
    cout << "Nodes: " << nodesCount << "\tEdges: " << edgesCount << endl;
    cout << "Connectivity" << endl;

    int idx = 1;

    for(vector<vector<pair<int, int> > >::iterator it = adj.begin() + 1; it != adj.end(); ++it) {
      vector<pair<int, int> > v = (*it);
      cout<<idx<<":";
      for(vector<pair<int, int> >::iterator _it = v.begin(); _it != v.end(); ++_it) {
        cout << " " << (_it->first) << "," << (_it->second);
      }
      cout<<endl;
      ++idx;
    }

    cout << "Betweenness prior" << endl;

    for(vector<vector<double> >::iterator it = betweennessWeights.begin() + 1; it != betweennessWeights.end(); ++it) {
      vector<double> v = (*it);
      for(vector<double>::iterator _it = v.begin() + 1; _it != v.end(); ++_it) {
        cout << (*_it) << " ";
      }
      cout << endl;
    }
  }

  double getBetweennessWeight(int u, int v);

  void setBetweennessWeight(int u, int v, double w);

  double getBetweennessNorm(int v);

  void shortestPathDijkstra(int source, vector<vector<int> > & paths);

  void shortestPathBFS(int source, vector<vector<int> > & paths);

  void countShortestPathContribs(int source, int node, vector<vector<int> > & paths,
    map<pair<int, int>, pair<int, int> > & count);

  double computeWeightedBetweennessCentrality(int node);
};

double Graph::getBetweennessWeight(int u, int v) {
  /**
   * Returns the "prior" betweenness weight of the vertex pair (u, v).
   */
  return betweennessWeights[u][v];
}

void Graph::setBetweennessWeight(int u, int v, double w) {
  /**
   * Sets the betweennessWeight for the vertex pair (u, v) to the value w.
   */
  double del = w - betweennessWeights[u][v];
  totalBetweennessWeight += del;
  betweennessWeights[u][v] = w;
}

double Graph::getBetweennessNorm(int v) {
  /**
   * Utility function to compute the normalisation factor for computing betweenness centrality.
   */
  double sum = 0;
  for(int i = 1; i <= nodesCount; ++i) {
    if(i == v) continue;
    for(int j = 1; j <= nodesCount; ++j) {
      if(j == v) continue;
      sum += getBetweennessWeight(i, j);
    }
  }
  return sum;
}

void Graph::shortestPathBFS(int source, vector<vector<int> > & paths) {
  /**
   * Implementation of Breadth-first search ( Unweighted graphs ) to measure distance of every vertex v from a "source" vertex.
   * ( Single source shortest path )
   *
   * :param source  The source vertex
   * :param paths   Adjacency list representation of the shortest-path tree
   */
  queue<int> Q;
  vector<int> dist(nodesCount + 1, INF);
  vector<bool> visited(nodesCount + 1, false);
  vector<vector<int> > prev(nodesCount + 1);

  paths.clear();
  paths.resize(nodesCount + 1);

  Q.push(source);
  dist[source] = 0;
  visited[source] = true;
  prev[source].push_back(source);

  while(!Q.empty()) {
    int u = Q.front();
    Q.pop();
    for(int i = 0; i < adj[u].size(); ++i) {
      int v = adj[u][i].first;
      if(!visited[v]) {
        dist[v] = dist[u] + 1;

        prev[v].clear();
        prev[v].push_back(u);
        visited[v] = true;

        Q.push(v);
      } else {
        if(dist[v] == dist[u] + 1) prev[v].push_back(u);
      }
    }
  }
  for(int i = 1; i <= nodesCount; ++i) {
    if(prev[i].size() == 1 && prev[i][0] == i) continue;
    for(int j = 0; j < prev[i].size(); ++j) {
      paths[prev[i][j]].push_back(i);
    }
  }
  // for(int i = 1; i <= nodesCount; ++i) {
  //   cout<<i<<":";
  //   for(int j = 0; j < paths[i].size(); ++j) {
  //     cout<<paths[i][j]<<" ";
  //   }
  //   cout<<endl;
  // }
}

void Graph::shortestPathDijkstra(int source, vector<vector<int> > & paths) {
  /**
   * Implementation of Dijkstra's algorithm ( Weighted graphs ) to measure distance of every vertex v from a "source" vertex.
   * ( Single source shortest path )
   *
   * :param source  The source vertex
   * :param paths   Adjacency list representation of the shortest-path tree
   */
  priority_queue<pair<int, int> > pq;
  vector<double> dist(nodesCount + 1, INF);
  vector<vector<int> > prev(nodesCount + 1);

  paths.clear();
  paths.resize(nodesCount + 1);

  pq.push(make_pair(0, source));
  dist[source] = 0;
  prev[source].push_back(source);

  while(!pq.empty()) {
    pair<int, int> p = pq.top();
    int u = p.second;
    pq.pop();
    vector<pair<int, int> > u_neigh = adj[u];
    for(vector<pair<int, int> >::iterator it = u_neigh.begin(); it != u_neigh.end(); ++it) {
      int v = (*it).first;
      int w = (*it).second;

      if(dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        prev[v].clear();
        prev[v].push_back(u);
        pq.push(make_pair(dist[v], v));
      } else if(dist[v] == dist[u] + w) {
        prev[v].push_back(u);
      }
    }
  }
  /**
   * Constructing shortest-path tree
   */
  for(int i = 1; i <= nodesCount; ++i) {
    if(prev[i].size() == 1 && prev[i][0] == i) continue;
    for(int j = 0; j < prev[i].size(); ++j) {
      paths[prev[i][j]].push_back(i);
    }
  }
  // for(int i = 1; i <= nodesCount; ++i) {
  //   cout<<i<<":";
  //   for(int j = 0; j < paths[i].size(); ++j) {
  //     cout<<paths[i][j]<<" ";
  //   }
  //   cout<<endl;
  // }
}

void Graph::countShortestPathContribs(int source, int node, vector<vector<int> > & paths,
                                      map<pair<int, int>, pair<int, int> > & count) {
  /**
   * Counts total number of shortest paths between pairs of nodes ( source, v ) and how many of them
   * pass through the given "node".
   *
   * :param source  The source vertex
   * :param node    The "node" whose betweenness centrality is to be found
   * :param paths   The shortest-path tree
   * :param count   Stores the counts. count[(source, v)] = ( number of shortest paths between (source, v),
   *                                                          number of such shortest paths which pass through node )
   */
  int sum = 0, u = source;

  stack<pair<int, int> > S;
  S.push(make_pair(source, 0));

  while(!S.empty()) {
    pair<int, int> p = S.top();
    S.pop();
    int u = p.first, s = p.second;
    // cout<<u<<" "<<s<<endl;
    if(u == node && u != source) {
      s = 1;
    }
    if(s && u != node && u != source) {
      ++count[make_pair(source, u)].first;
      ++count[make_pair(source, u)].second;
    } else if(u != source) {
      ++count[make_pair(source, u)].second;
    }
    for(int i = 0; i < paths[u].size(); ++i) {
      int v = paths[u][i];
      S.push(make_pair(v, s));
    }
  }
}

double Graph::computeWeightedBetweennessCentrality(int node) {
  /**
   * Computes weighted betweenness centrality for a given node v.
   * :param node  Node whose betweenness centrality is to be found
   * :return      Betweenness centrality(node)
   */
  vector<vector<int> > paths;
  map<pair<int, int>, pair<int, int> > count;

  for(int i = 1; i <= nodesCount; ++i) {
    // Use Dijkstra for shortest paths if weighted
    if(isWeighted()) shortestPathDijkstra(i, paths);
    // Else use BFS - Optimised
    else shortestPathBFS(i, paths);
    countShortestPathContribs(i, node, paths, count);
  }

  double sum = 0.0, norm = 0.0;
  for(map<pair<int, int>, pair<int, int> >::iterator it = count.begin(); it != count.end(); ++it) {
    pair<int, int> p = it->first;
    pair<int, int> q = it->second;
    // cout<<"=="<<p.first<<" "<<p.second<<" "<<q.first<<" "<<q.second<<endl;
    double x;
    if(q.second) {
      x = ((double)(q.first))/q.second;
      x *= getBetweennessWeight(p.first, p.second);
      sum += x;
    }
  }
  // Normalizing betweenness centrality
  sum /= getBetweennessNorm(node);
  return sum;
}

int main() {
  int n, m, g;
  string type, mode;
  // type: directed or undirected
  // mode: weighted or unweighted
  cin >> type >> mode >> n >> m >> g;
  Graph graph(n, type, mode);
  int u, v, w;
  for(int i = 0; i < m; ++i) {
    cin >> u >> v;

    if(graph.isWeighted()) cin >> w;
    else w = 1.0;

    graph.addEdge(u, v, w);
    if(!graph.isDirected()) graph.addEdge(v, u, w);
  }

  double x;
  for(int i = 1; i <= n; ++i) {
    for(int j = 1; j <=n; ++j) {
      cin >> x;
      graph.setBetweennessWeight(i, j, x);
    }
  }
  // Printing graph summary
  graph.summary();
  cout << "Betweenness centrality ( node = " << g << " ): " << fixed << graph.computeWeightedBetweennessCentrality(g) << endl;
  return 0;
}
