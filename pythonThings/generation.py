# Graph Generation for Project 3 - Girvan Newman Algo
# Created by Michael Amberg  4/1/2022

import networkx as nx
import matplotlib.pyplot as plt
path = "data/"

# \/ the parameters, respectively, are # of groups to produce, the # of vertices in each group, and the chance that a node will form a different edge
G = nx.relaxed_caveman_graph(6, 10, 0.15)
nx.write_graphml(G, path+"randomGraph.graphml")
# Load the graph or w/e
nx.draw(G)
plt.show()

# think this will work but I have no way to confirm or not since networkx decided to stop working
