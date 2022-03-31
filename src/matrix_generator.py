"""****************************************************
* CS 3353 - PA 03
* Description: Generate a variety of matrix
* representations of a provided unweighted, undirected
* graph.
****************************************************"""

from collections import defaultdict
import networkx as netx
import numpy as np


def get_similarity_matrix(graph, method):
    match str.upper(method):
        case "DICE":
            return __dice_similarity(graph)
        case "OVERLAP":
            return __overlap_similarity(graph)
        case default:
            raise RuntimeError('Invalid matrix conversion method specified.')


def get_adjacency_matrix(graph):
    return netx.to_numpy_array(graph)



