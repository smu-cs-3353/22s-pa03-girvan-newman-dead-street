"""****************************************************
* CS 3353 - PA 03
* Description:
****************************************************"""
import os

from matrix_generator import get_similarity_matrix
from matrix_generator import get_adjacency_matrix
import networkx as netx
import sys

# Step zero: Sanity checks
if len(sys.argv) == 1:
    raise RuntimeError("Extension must have graph filename specified as an argument.\n"
                       "Format: python3 extension_main.py C:\\Reachable\\File\\Path.graphml")

if not os.path.isdir(os.getcwd() + "\..\data"):
    raise RuntimeError("Invalid working directory relative to data directory.\n"
                       "Please check your run configuration")

# Step one: Import a graph
graph = netx.read_graphml(sys.argv[1])
# Step two: Convert graph into similarity matrix form
adj_matrix = get_adjacency_matrix(graph)  # Test case, known good matrix form
sim_matrix = get_similarity_matrix(graph, "DICE")  # Control case, uses Sørensen–Dice coefficient
alt_sim_matrix = get_similarity_matrix(graph, "OVERLAP")  # Experimental case, uses Overlap coefficient
# Step three: Run deep sparse filtering (2+ layers) on each matrix

# Step four: Run clustering algorithm on sparse matrix

# Step five: Output communities/graph/etc.
