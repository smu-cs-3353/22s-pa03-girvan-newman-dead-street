# Graph Generation for Project 3 - Girvan Newman Algo
# Created by Michael Amberg  4/1/2022

import networkx as nx

path = "data/"

G = nx.relaxed_caveman_graph(3, 4, 0.35)
nx.write_graphml(G, path+"randomGraph.graphml")

# think this will work but I have no way to confirm or not since networkx decided to stop working
