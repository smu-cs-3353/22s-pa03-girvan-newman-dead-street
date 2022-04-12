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


def __dice_similarity(graph):
    node_dict = defaultdict(list)
    node_similarities = []

    for node in graph:
        for neighbor in graph.neighbors(node):
            node_dict[node].append(neighbor)

    # Is there a library to compute dictionary permutations? Yes. Would I be able to look anybody in the class in the
    # eyes after using it? No.
    for x_neighbors in node_dict.values():
        for y_neighbors in node_dict.values():
            node_similarities.append(
                # 2|X ∩ Y| / (|X| + |Y|); Sørensen–Dice coefficient
                (2 * len(list(set(x_neighbors) & set(y_neighbors)))) / (len(x_neighbors) + len(y_neighbors))
            )
    assert len(node_similarities) % graph.number_of_nodes() == 0, "Dice matrix generation failure"
    similarity_matrix = np.array(node_similarities).reshape(-1, graph.number_of_nodes())

    return similarity_matrix


def __overlap_similarity(graph):
    node_dict = defaultdict(list)
    node_similarities = []

    for node in graph:
        for neighbor in graph.neighbors(node):
            node_dict[node].append(neighbor)

    # Is there a library to compute dictionary permutations? Yes. Would I be able to look anybody in the class in the
    # eyes after using it? No.
    for x_neighbors in node_dict.values():
        for y_neighbors in node_dict.values():
            node_similarities.append(
                # |X ∩ Y| / min(|X|, |Y|); Overlap coefficient
                len(list(set(x_neighbors) & set(y_neighbors))) / min(len(x_neighbors), len(y_neighbors))
            )
    assert len(node_similarities) % graph.number_of_nodes() == 0, "Overlap matrix generation failure"
    similarity_matrix = np.array(node_similarities).reshape(-1, graph.number_of_nodes())

    return similarity_matrix
