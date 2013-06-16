#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <vector>

#include "mmatrix.h"

//	//	gen	//	//

MMatrix& normalize_in_place(MMatrix& mat);

MMatrix compute_covariance_matrix(MMatrix& mat);

void QR_algorithm_in_place(MMatrix& A, MMatrix& V, double delta, int& iteration_count, bool verbose);

void QR_factorization_in_place(MMatrix& Q, MMatrix& A);

void ensure_positive_diagonal(MMatrix& V, MMatrix& D);

void sort_eigenvectors(MMatrix& V, MMatrix& D);

MMatrix transform_images(MMatrix& images, MMatrix& V);

MMatrix compute_average_by_digit(MMatrix& transf_images, vector<int>& labels);

void extended_power_method(MMatrix& A, int k, double delta, MMatrix& V);

void power_method(MMatrix& A, double delta, MMatrix& v);

//	//	classif	//	//

int classify_images(MMatrix& images, vector<int>& labels, MMatrix& V, MMatrix& avgs, int k);

#endif // __ALGORITHMS_H__