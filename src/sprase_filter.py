"""****************************************************
* CS 3353 - PA 03
* Description: Attempt to learn the distribution of a
* set of data provided by an unweighted, undirected
* graph.
****************************************************"""
import math

import numpy as np
from numpy.linalg import norm
from scipy.optimize import fmin_l_bfgs_b


def _back_prop(raw_data, norm_data, norm_sum, delta):
    return delta / norm_sum - norm_data * (delta * raw_data).sum(axis=1) / (norm_sum ** 2)


def _get_attributes(weights, dataset):
    pre_processed = np.dot(weights, dataset)  # F
    processed = np.sqrt(pre_processed ** 2 + 1e-8)  # Fs

    # Step 2: L2 normalization (LSE) along features (rows)
    row_norm_sum = np.linalg.norm(processed, axis=0)
    row_normalized_matrix = processed / row_norm_sum  # F tilde

    # Step 3: L2 normalization (LSE) along the samples (columns)
    col_norm_sum = np.linalg.norm(row_normalized_matrix, axis=1)
    column_normalized_matrix = np.transpose(row_normalized_matrix) / col_norm_sum  # F hat

    return pre_processed, processed, row_norm_sum, row_normalized_matrix, col_norm_sum, column_normalized_matrix


def fit_sparse_filter(matrix, features, runs, verb):
    # Step 0: Transpose matrix to flip samples and features from rows/columns to columns/rows, initialize weight matrix
    dataset = np.transpose(matrix)
    rand_weights = np.random.rand(features, dataset.shape[0])

    def _objective_function(weights):
        # Step 1: non-linear processing of dataset with randomized weight matrix weights
        weights = weights.reshape(features, dataset.shape[0])

        # Steps 2 & 3 inside...
        pre_processed, processed, row_norm_sum, row_normalized_matrix, col_norm_sum, column_normalized_matrix = \
            _get_attributes(weights, dataset)

        # Step 4: Minimize (L-BFGS) the L1 normalization
        objective_value = column_normalized_matrix.sum()
        new_weights = _back_prop(row_normalized_matrix,
                                 column_normalized_matrix,
                                 col_norm_sum,
                                 np.ones_like(column_normalized_matrix)
                                 )

        new_weights = _back_prop(processed,
                                 np.transpose(row_normalized_matrix),
                                 row_norm_sum,
                                 np.transpose(new_weights)
                                 )

        new_weights = (new_weights * (pre_processed / processed)).dot(np.transpose(dataset))

        return objective_value, new_weights.flatten()

    final_weights, f, d = fmin_l_bfgs_b(_objective_function, rand_weights.flatten(), maxfun=runs, iprint=verb)
    final_weights = final_weights.reshape(features, dataset.shape[0])

    return _get_attributes(final_weights, dataset)[5]  # Return F hat
