# Graph Generation for Project 3 - Girvan Newman Algo
# Created by Michael Amberg  4/1/2022

import networkx as nx

G = nx.random_partition_graph([32, 32, 32, 32, 32], .6, .2, directed=False)
i = 0
if i == 0:
    i = 2
