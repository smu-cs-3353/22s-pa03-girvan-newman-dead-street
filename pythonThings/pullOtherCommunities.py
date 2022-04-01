# Getting other graphs for Project 3 - Girvan Newman Algo
# Created by Michael Amberg  4/1/2022

import networkx as nx

G1 = nx.karate_club_graph()

G2 = nx.davis_southern_women_graph()

G3 = nx.florentine_families_graph()

G4 = nx.les_miserables_graph()

path = "data/"

nx.write_graphml(G1, path+"karateClub.graphml")
nx.write_graphml(G2, path+"southernWomen.graphml")
nx.write_graphml(G3, path+"florentineFams.graphml")
nx.write_graphml(G4, path+"lesMis.graphml")

