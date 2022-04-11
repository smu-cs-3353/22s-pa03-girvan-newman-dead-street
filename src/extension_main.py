"""****************************************************
* CS 3353 - PA 03
* Description: Compute potential communities by
* leveraging deep sparse filtering on matrix
* representations of a provided graph.
****************************************************"""

import argparse
import math
import os
import networkx as netx
from matrix_generator import get_similarity_matrix
from matrix_generator import get_adjacency_matrix
from sprase_filter import fit_sparse_filter
from sklearn.cluster import KMeans, DBSCAN
from sklearn.metrics.cluster import normalized_mutual_info_score
from collections import defaultdict
import matplotlib.pyplot as plt
import networkx.algorithms.community as nx_comm


# Step zero: Argument parsing & sanity checks
parser = argparse.ArgumentParser(description="Experiment")
parser.add_argument("-i", "--input", type=str, required=True, help="A relative path to a graphml file.")
parser.add_argument("-m", "--method", type=str, required=True, choices=["ADJ", "DICE", "OVERLAP"], help="Methodology "
                                                                                                        "for for graph "
                                                                                                        "conversion.")
parser.add_argument("-r", "--runs", type=int, required=False, default=200, help="A value between 1 and INT_MAX for "
                                                                                "LM-BFGS optimization")
parser.add_argument("-v", "--verbosity", type=int, required=False, default=20, help="A value between -1 and 101 for "
                                                                                    "debugging and statistical output.")
args = parser.parse_args()

if not any(vars(args).values()):
    raise RuntimeError("Extension must have graph filename, matrix method, and optionally verbosity specified as an "
                       "argument.\n"
                       "Format: python3 extension_main.py -i C:\\Reachable\\File\\Path.graphml -m <METHOD> "
                       "-v <VERBOSITY>")

if not os.path.isdir(os.getcwd() + "/../data"):
    raise RuntimeError("Invalid working directory relative to data directory.\n"
                       "Please check your run configuration")

# Step one: Import a graph
#graph = netx.read_graphml(args.input)
graph = netx.karate_club_graph()

# Step two: Convert graph into similarity matrix form
# ADJ = test case, known good matrix form
# DICE = control case, uses Sørensen–Dice coefficient
# OVERLAP = Experimental case, uses Overlap coefficient
matrix = get_adjacency_matrix(graph) if args.method == "ADJ" else get_similarity_matrix(graph, args.method)

modularity_dict = {}
for run_num in range(1, 11):
    print("=== Run number " + str(run_num) + " ===")

    # Step three: Run deep sparse filtering (1-5 layers) on each matrix
    num_features = graph.number_of_nodes()
    last_features = matrix
    itr = 0
    while True:
        print("Learning " + str(num_features) + " features...")
        features = fit_sparse_filter(last_features, num_features, args.runs, args.verbosity)
        last_features = features
        num_features = int(pow(2, int(math.log(num_features - 1, 2))))  # next closest power of 2 less than num_features
        itr += 1
        if num_features < 16 or itr >= 5:
            break

    # Step four: Run clustering algorithm on sparse matrix
    """
    # DEBUG INFORMATION - UNCOMMENT FOR EXTRA FIGURES
    plt.hist(last_features.flat, bins=50)
    plt.xlabel("Activation")
    plt.ylabel("Count")
    plt.title("Feature activation histogram")
    plt.show()
    """


    cluster_list = []
    num_community_est = 3  # Should be calculated through WCSS or something
    for i in range(0, 30):
        #kmeans = KMeans(num_community_est, n_init=5)  # Change init to be closer to default of 10?
        #kmeans.fit(last_features)
        #cluster_list.append(kmeans.fit_predict(last_features))
        clustering = DBSCAN(eps=1.15, min_samples=3)
        clustering.fit(last_features)
        cluster_list.append(clustering.fit_predict(last_features))

    node_dict = defaultdict(set)
    for cluster in cluster_list:
        for node_num in range(0, graph.number_of_nodes()):
            node_dict[cluster[node_num]].add(str(node_num))
        modularity_dict[nx_comm.partition_quality(graph, node_dict.values())[1]] = cluster
        node_dict.clear()

# Step five: Output communities/graph/etc.
best_cluster = modularity_dict[max(modularity_dict)]
print("\nHighest performance found: " + str(max(modularity_dict)))
print(best_cluster)
color_map = []

# Needs to be extensible for n communities, if desired
for color in best_cluster:
    if color == -1:
        color_map.append('blue')
    if color == 0:
        color_map.append('red')
    if color == 1:
        color_map.append('green')
    if color == 2:
        color_map.append('yellow')

mapping = dict(zip(graph, range(1, graph.number_of_nodes() + 1)))
graph = netx.relabel_nodes(graph, mapping)
netx.draw(graph, node_color=color_map, with_labels=True)
plt.show()

# Need to be set per-graph, only useful when we know the ground truth of a graph's communities
# true_cluster = [0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
true_cluster = [0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2]
print("NMI: " + str(normalized_mutual_info_score(best_cluster, true_cluster)))
