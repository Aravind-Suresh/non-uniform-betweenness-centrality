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

class Graph {
private:
  int nodesCount;
  int edgesCount;
  vector<int> nodes;
  vector<vector<pair<int, int> > > adj;
  vector<vector<int> > betweennessWeights;

public:
  Graph(int n) {
    nodesCount = n;
    nodes.reserve(n+1);
    adj.reserve(n+1);
    betweennessWeights.reserve(n+1);
  }
  void addEdge(int a, int b, int w) {
    // Directed edge from a->b with a weight w
    adj[a].push_back(make_pair(b, w));
    ++edgesCount;
  }
  void print() {
    cout<<"Graph details"<<endl;
    cout<<"Nodes: "<<nodesCount<<"\tEdges: "<<edgesCount<<endl;
    int idx = 1;
    for(vector<vector<pair<int, int> > >::iterator it = adj.begin(); it != adj.end(); ++it) {
      vector<pair<int, int> > v = (*it);
      cout<<idx<<": ";
      for(vector<pair<int, int> >::iterator _it = v.begin(); _it != v.end(); ++_it) {
        cout<<" "<<(_it->first);
      }
      cout<<endl;
    }
  }
  int getBetweennessWeight(int u, int v) {
    return betweennessWeights[u][v];
  }
  void shortestPath(int source, vector<int> & paths, vector<int> & is_leaf);
  int addWeights(int source, int node, vector<int> & paths, vector<int> & is_leaf);
  int computeWeightedBetweennessCentrality(int v);
};

void Graph::shortestPath(int source, vector<int> & paths, vector<int> & is_leaf) {
  priority_queue<pair<int, int> > pq;
  vector<int> dist(nodesCount + 1, INF);
  is_leaf.clear();
  is_leaf.reserve(nodesCount + 1);
  fill(is_leaf.begin(), is_leaf.end(), true);
  paths.clear();
  paths.reserve(nodesCount + 1);

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

int Graph::computeWeightedBetweennessCentrality(int v) {
  vector<int> paths, is_leaf;
  int sum = 0;
  for(int i = 1; i <= nodesCount; ++i) {
    shortestPath(i, paths, is_leaf);
    sum += addWeights(i, v, paths, is_leaf);
  }
  return sum;
}

int main() {
  return 0;
}
