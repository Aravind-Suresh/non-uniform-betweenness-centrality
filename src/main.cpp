/**
* @Author:              aravind
* @Email:               arvindsuresh2009@gmail.com
* @Github:              [Aravind-Suresh](https://github.com/Aravind-Suresh)
* @Date:                2017-03-05T16:45:13+05:30
* @Last modified by:    aravind
* @Last modified time:  2017-03-08T12:28:58+05:30
*/

#include <bits/stdc++.h>
#include <ctime>
#include "util.h"

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

int main() {

  int startTime = clock();

  int n, m, g;
  string type, mode;
  // type: directed or undirected
  // mode: weighted or unweighted
  cin >> type >> mode >> n >> m >> g;
  Graph graph(n, type, mode);
  int u, v;
  double w;
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
  graph.summary(true);
  cout << "Betweenness centrality ( node = " << g << " ): " << fixed << graph.computeWeightedBetweennessCentrality(g) << endl;

  int endTime = clock();
  cout << "Execution time: " << 1000*(endTime - startTime)/((double) CLOCKS_PER_SEC) << " ms" << endl;

  return 0;
}
