"""****************************************************
* CS 3353 - PA 03
* Description:
****************************************************"""
import os

from matrix_generator import get_similarity_matrix
from matrix_generator import get_adjacency_matrix
import networkx as netx
import sys

# Step one: Import a graph
graph = netx.read_graphml(sys.argv[1])
# Step two: Convert graph into similarity matrix form

# Step three: Run deep sparse filtering (2+ layers) on each matrix

# Step four: Run clustering algorithm on sparse matrix

# Step five: Output communities/graph/etc.
