/**
* @Author: aravind
* @Date:   2017-03-05T16:45:13+05:30
* @Last modified by:   aravind
* @Last modified time: 2017-03-05T16:45:13+05:30
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

const int INF = (int)(1e9);

// TODO: Assert in range ( 1, n )
class Graph {
private:
  int nodesCount;
  int edgesCount;
  double totalBetweennessWeight;

  // Container for adjacency list
  // For every vertex 'u', adj[u] is a list of pairs (other vertex, weight)
  vector<vector<pair<int, int> > > adj;

  // Container for prior weights between pairs of vertices
  vector<vector<double> > betweennessWeights;

public:
  Graph(int n) {
    // Constructing the graph
    nodesCount = n;
    adj.resize(n+1);
    edgesCount = 0;

    betweennessWeights.resize(n+1);
    for(int i = 1; i <= n; ++i) {
      betweennessWeights[i].resize(n+1);
      for(int j = 1; j <= n; ++j) {
        betweennessWeights[i][j] = 0.0;
      }
    }
    totalBetweennessWeight = 0.0;
  }
  void addEdge(int a, int b, int w) {
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
  double getBetweennessWeight(int u, int v) {
    return betweennessWeights[u][v];
  }
  void setBetweennessWeight(int u, int v, double w) {
    double del = w - betweennessWeights[u][v];
    totalBetweennessWeight += del;
    betweennessWeights[u][v] = w;
  }
  void shortestPath(int source, vector<int> & paths, vector<int> & is_leaf);
  int addWeights(int source, int node, vector<int> & paths, vector<int> & is_leaf);
  double computeWeightedBetweennessCentrality(int v);
};

void Graph::shortestPath(int source, vector<int> & paths, vector<int> & is_leaf) {
  priority_queue<pair<int, int> > pq;
  vector<int> dist(nodesCount + 1, INF);
  is_leaf.clear();
  is_leaf.resize(nodesCount + 1);
  fill(is_leaf.begin(), is_leaf.end(), true);
  paths.clear();
  paths.resize(nodesCount + 1);

  pq.push(make_pair(0, source));
  dist[source] = 0;
  paths[source] = source;

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
        paths[v] = u;
        is_leaf[u] = false;
        pq.push(make_pair(dist[v], v));
      }
    }
  }
}

int Graph::addWeights(int source, int node, vector<int> & paths, vector<int> & is_leaf) {
  vector<bool> visited(nodesCount + 1, false);
  int sum = 0;
  for(int i = 1; i <= is_leaf.size(); ++i) {
    if(is_leaf[i]) {
      int u = i;
      vector<int> dest;
      while(paths[u] != u) {
        if(u == node) {
          for(vector<int>::iterator it = dest.begin(); it != dest.end(); ++it) {
            sum += getBetweennessWeight(source, (*it));
          }
          break;
        } else {
          dest.push_back(u);
        }
        u = paths[u];
      }
    }
  }
  return sum;
}

double Graph::computeWeightedBetweennessCentrality(int v) {
  vector<int> paths, is_leaf;
  double sum = 0;
  for(int i = 1; i <= nodesCount; ++i) {
    shortestPath(i, paths, is_leaf);
    sum += addWeights(i, v, paths, is_leaf);
  }
  sum /= totalBetweennessWeight;
  return sum;
}

int main() {
  int n, m, g;
  cin >> n >> m >> g;
  Graph graph(n);
  int u, v, w;
  for(int i = 0; i < m; ++i) {
    cin >> u >> v >> w;
    graph.addEdge(u, v, w);
  }
  int t = n*(n-1)/2;
  for(int i = 1; i <= n; ++i) {
    graph.setBetweennessWeight(i, i, 0);
  }
  double x;
  for(int i = 0; i < t; ++i) {
    cin >> u >> v >> x;
    graph.setBetweennessWeight(u, v, x);
    graph.setBetweennessWeight(v, u, x);
  }
  graph.summary();
  cout << "Betweenness centrality: " << fixed << graph.computeWeightedBetweennessCentrality(g) << endl;
  return 0;
}
