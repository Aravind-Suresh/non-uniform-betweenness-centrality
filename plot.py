import matplotlib.pyplot as plt
import sys
"""
Utility script to plot Execution time Vs Number of vertices based on the log-files obtained by running tests/*.sh
"""
lines = open(sys.argv[1]).readlines()[1::2]
x = map(lambda xx: 5*eval(xx.strip().split('\t')[1].split(' ')[1]), lines[::2])
# x = map(lambda xx: 400*eval(xx.strip().split(' ')[1].split('\t')[0]), lines[::2])
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
    y_plot.append(t[xx]*0.0001*25/c[xx])
    # y_plot.append(t[xx]*14.711*0.6*0.5*10*16/c[xx])
print x
print y_plot
# print t[700]*10.0/c[700]
# print y_plot

plt.xlabel("Edges | Number of vertices = 5000")
# plt.xlabel("Vertices")
plt.ylabel("Execution time ( s )")

plt.plot(x,y_plot,'ro')
plt.show()
