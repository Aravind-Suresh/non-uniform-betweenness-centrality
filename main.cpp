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
    betweennessWeights[u][v] = w;
  }
  double getBetweennessNorm(int v) {
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
  void shortestPath(int source, vector<vector<int> > & paths);
  int addWeights(int source, int node, vector<vector<int> > & paths);
  double computeWeightedBetweennessCentrality(int v);
};

void Graph::shortestPath(int source, vector<vector<int> > & paths) {
  priority_queue<pair<int, int> > pq;
  vector<int> dist(nodesCount + 1, INF);
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

int Graph::addWeights(int source, int node, vector<vector<int> > & paths) {
  vector<bool> visited(nodesCount + 1, false);
  int sum = 0, u = source;
  vector<pair<double, int> > contribs(nodesCount + 1, make_pair(0.0, 0));

  queue<pair<int, int> > Q;
  Q.push(make_pair(source, 0));
  visited[source] = true;

  while(!Q.empty()) {
    pair<int, int> u = Q.front();
    Q.pop();
    int s = u.second;
    if(s && u.first != node) {
      contribs[u.first].first += getBetweennessWeight(source, u.first);
      ++contribs[u.first].second;
    }
    for(int i = 0; i < paths[u.first].size(); ++i) {
      int v = paths[u.first][i];
      if(v == node) s = 1;
      else s = u.second;
      if(!visited[v]) {
        visited[v] = true;
        Q.push(make_pair(v, s));
      } else {
        ++contribs[u.first].second;
      }
    }
  }
  for(int i = 1; i <= nodesCount; ++i) {
    // cout<<i<<" "<<contribs[i].first<<" "<<contribs[i].second<<endl;
    if(contribs[i].second) sum += contribs[i].first/contribs[i].second;
  }
  // cout<<sum<<endl;
  return sum;
}

double Graph::computeWeightedBetweennessCentrality(int v) {
  vector<vector<int> > paths;
  double sum = 0;
  for(int i = 1; i <= nodesCount; ++i) {
    shortestPath(i, paths);
    sum += addWeights(i, v, paths);
  }
  sum /= getBetweennessNorm(v);
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
  cout << "Betweenness centrality ( node = " << g << " ): " << fixed << graph.computeWeightedBetweennessCentrality(g) << endl;
  return 0;
}
