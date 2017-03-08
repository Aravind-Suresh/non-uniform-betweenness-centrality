import numpy as np
import sys

type = sys.argv[1]
is_directed = ( type == "directed" )

mode = sys.argv[2]
is_weighted = ( mode == "weighted" )

n = eval(sys.argv[3])
m = eval(sys.argv[4])
k = eval(sys.argv[5])

edges = []

while True:
    a = np.random.randint(1, n+1)
    b = np.random.randint(1, n+1)
    t = None

    if is_weighted:
        w = np.random.random()
        t = (a, b, w)
    else:
        t = (a, b)

    if a == b or ( not is_directed and t in edges ):
        continue
    edges.append(t)

    if len(edges) == m:
        break

betweenness_prior = np.random.random((n, n))

def print_graph_test_input():
    print type, mode
    print n, m, k
    for e in edges:
        if is_weighted:
            print e[0], e[1], e[2]
        else:
            print e[0], e[1]
    for i in range(n):
        for j in range(n):
            print betweenness_prior[i][j],
        print ""

print_graph_test_input()
