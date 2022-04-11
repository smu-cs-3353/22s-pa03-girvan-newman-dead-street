"""****************************************************
* CS 3353 - PA 03
* Description: Attempt to learn the distribution of a
* set of data provided by an unweighted, undirected
* graph.
****************************************************"""
import numpy as np
import scipy
from numpy.linalg import norm
from scipy.optimize import fmin_l_bfgs_b


def fit_sparse_filter(matrix, features, runs, verb):
    # Step 0: Transpose matrix to flip samples and features from rows/columns to columns/rows, initialize weight matrix
    dataset = np.transpose(matrix)
    rand_weights = np.random.rand(features, dataset.shape[0])

    def _objective_function(weights):
        # Step 1: non-linear processing of dataset with randomized weight matrix weights
        weights = weights.reshape(features, dataset.shape[0])
        pre_processed = np.dot(weights, dataset)
        processed = np.sqrt(pre_processed ** 2 + 1e-8)  # F

        # Step 2: L2 normalization (LSE) along features (rows)
        row_normalized_matrix = processed / norm(processed, axis=0, keepdims=True)  # F tilde

        # Step 3: L2 normalization (LSE) along the samples (columns)
        column_normalized_matrix = row_normalized_matrix / norm(row_normalized_matrix, axis=1, keepdims=True)  # F hat

        # Step 4: Minimize (L-BFGS) the L1 normalization
        objective_value = norm(column_normalized_matrix, ord=1, keepdims=True)
        return objective_value

    final_weights, f, d = fmin_l_bfgs_b(_objective_function, rand_weights.flatten(), maxfun=runs, iprint=verb,
                                        approx_grad=True)
    return final_weights.reshape(features, dataset.shape[0])
