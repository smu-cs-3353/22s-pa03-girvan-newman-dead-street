import networkx as nx
import matplotlib.pyplot as plt

graph = nx.read_graphml("data/randomGraph.graphml")
G = nx.read_graphml("data/output.graphml")
plot1 = plt.figure(1)
nx.draw(G)
plot2 = plt.figure(2)
nx.draw(graph)
plt.show()