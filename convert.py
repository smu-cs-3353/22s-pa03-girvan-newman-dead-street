# Graph Conversion for Project 3 - Girvan Newman Algo
# Created by Michael Amberg  3/30/2022

import networkx as nx
import lxml.etree as lxmletree

path = "/Users/Michael/Desktop/Random SMU docs/football/"
graph = nx.read_gml(path+"football.gml")
# graphml = nx.generate_graphml(gml)
nx.write_graphml(graph, path+"converted.graphml")
