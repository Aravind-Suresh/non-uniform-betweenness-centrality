import matplotlib.pyplot as plt
import sys
"""
Utility script to plot Execution time Vs Number of vertices based on the log-files obtained by running tests/*.sh
"""
lines = open(sys.argv[1]).readlines()[1::2]
x = map(lambda xx: eval(xx.strip().split(' ')[1].split('\t')[0]), lines[::2])
y = map(lambda xx: eval(xx.split(' ')[2]), lines[1::2])
t = {}
c = {}
for xx in x:
    c[xx] = 0
    t[xx] = 0
for i in range(len(y)):
    xx = x[i]
    t[xx] += y[i]
    c[xx] += 1
y_plot = []
for i in range(len(y)):
    xx = x[i]
    y_plot.append(t[xx]*1.0/c[xx])

plt.xlabel("Vertices")
plt.ylabel("Execution time ( ms )")

plt.plot(x,y_plot,'ro')
plt.show()
