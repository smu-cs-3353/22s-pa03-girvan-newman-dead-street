# Graph Generation for Project 3 - Girvan Newman Algo
# Created by Michael Amberg  4/1/2022

import networkx as nx

path = "data/"

G = nx.random_partition_graph([32, 32, 32, 32, 32], .6, .2, directed=False)
nx.write_graphml(G, path+"randomGraph.graphml")

# think this will work but I have no way to confirm or not since networkx decided to stop working
