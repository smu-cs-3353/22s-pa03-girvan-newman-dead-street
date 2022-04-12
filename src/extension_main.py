"""****************************************************
* CS 3353 - PA 03
* Description: Compute potential communities by
* leveraging deep sparse filtering on matrix
* representations of a provided graph.
****************************************************"""

import argparse
import os
import random

import networkx
import networkx as netx
import numpy as np

from matrix_generator import get_similarity_matrix
from matrix_generator import get_adjacency_matrix
from sprase_filter import fit_sparse_filter
from sklearn.cluster import KMeans
from collections import defaultdict
import matplotlib.pyplot as plt
import networkx.algorithms.community as nx_comm

# Step zero: Argument parsing & sanity checks
parser = argparse.ArgumentParser(description="Experiment")
parser.add_argument("-i", "--input", type=str, required=True, help="A relative path to a graphml file.")
parser.add_argument("-m", "--method", type=str, required=True, choices=["ADJ", "DICE", "OVERLAP"], help="Methodology "
                                                                                                        "for for graph "
                                                                                                        "conversion.")
parser.add_argument("-r", "--runs", type=int, required=False, default=100, help="A value between 1 and INT_MAX for "
                                                                                "LM-BFGS optimization. Default: 100")
parser.add_argument("-v", "--verbosity", type=int, required=False, default=-1, help="A value between -1 and 101 for "
                                                                                    "debugging and statistical "
                                                                                    "output. Default: -1")
parser.add_argument("-d", "--debug", type=bool, required=False, default=False, help="Generates additional data about "
                                                                                    "the sparse filter if enabled. "
                                                                                    "Default: False")
args = parser.parse_args()

if not any(vars(args).values()):
    raise RuntimeError("Extension must have graph filename, matrix method, and optionally verbosity specified as an "
                       "argument.\n"
                       "Format: python3 extension_main.py -i C:\\Reachable\\File\\Path.graphml -m <METHOD> "
                       "-r <RUNS> (optional) -v <VERBOSITY> (optional) -d <TRUE/FALSE> (optional)")

if not os.path.isdir(os.getcwd() + "/../data"):
    raise RuntimeError("Invalid working directory relative to data directory.\n"
                       "Please check your run configuration.")

# Step one: Import a graph
graph = netx.read_graphml(args.input)

# Step two: Convert graph into similarity matrix form
# ADJ = test case, known good matrix form
# DICE = control case, uses Sørensen–Dice coefficient
# OVERLAP = experimental case, uses Overlap coefficient
matrix = get_adjacency_matrix(graph) if args.method == "ADJ" else get_similarity_matrix(graph, args.method)

performance_dict = {}
if args.debug:
    feature_dict = {}
for run_num in range(1, 11):
    print("====== Run number " + str(run_num) + " ======")

    # Step three: Run deep sparse filtering (1-5 layers) on each matrix
    num_features = graph.number_of_nodes()
    last_features = matrix
    for layers in range(1, 6):
        print("Layer " + str(layers) + " learning " + str(num_features) + " features...")
        features = fit_sparse_filter(last_features, num_features, args.runs, args.verbosity)
        last_features = features

    wcss = []
    for i in range(1, graph.number_of_nodes()):
        kmeans = KMeans(i, n_init=1)
        kmeans.fit(last_features)
        wcss.append(kmeans.inertia_)
    deltas = np.diff(wcss)
    delta_prime = np.diff(deltas)
    num_community_est = (np.where(delta_prime == max(delta_prime))[0][0]) + 2
    print("Estimated community count: " + str(num_community_est))

    # Step four: Run clustering algorithm on sparse matrix
    print("Clustering...")
    cluster_list = []
    for i in range(0, 30):
        kmeans = KMeans(num_community_est, n_init=5)  # Change init to be closer to default of 10?
        kmeans.fit(last_features)
        cluster_list.append(kmeans.fit_predict(last_features))

    node_dict = defaultdict(set)
    for cluster in cluster_list:
        node_num = 0
        for node_id in networkx.nodes(graph):
            node_dict[cluster[node_num]].add(node_id)
            node_num += 1
        quality = nx_comm.partition_quality(graph, node_dict.values())[1]
        performance_dict[quality] = cluster
        if args.debug:
            # noinspection PyUnboundLocalVariable
            feature_dict[quality] = last_features
        node_dict.clear()

# Step five: Output communities/graph/etc.
print("====== Generating output ======")
best_performance = max(performance_dict)
best_cluster = performance_dict[best_performance]
if args.debug:
    best_features = feature_dict[best_performance]
print("Highest performance found: " + str(max(performance_dict)))

if args.debug:
    # noinspection PyUnboundLocalVariable
    plt.hist(best_features.flat, bins=50)
    plt.xlabel("Activation")
    plt.ylabel("Count")
    plt.title("Feature activation histogram")
    plt.show()

color_map = []
seen_communities = {}
for community in best_cluster:
    if community not in seen_communities:
        seen_communities[community] = "#" + "".join([random.choice("ABCDEF0123456789") for nibble in range(6)])
    color_map.append(seen_communities[community])

mapping = dict(zip(graph, range(1, graph.number_of_nodes() + 1)))
graph = netx.relabel_nodes(graph, mapping)
netx.draw(graph, node_color=color_map, with_labels=True)
plt.show()

"""
# Need to be set per-graph, only useful when we know the ground truth of a graph's communities
from sklearn.metrics.cluster import normalized_mutual_info_score
true_cluster = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                6,
                6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10,
                10,
                10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11]
# true_cluster = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
# true_cluster = [0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2]
print("NMI: " + str(normalized_mutual_info_score(best_cluster, true_cluster)))
"""
