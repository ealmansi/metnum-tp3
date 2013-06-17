#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <vector>

#include "mmatrix.h"

MMatrix& normalize_in_place(MMatrix& mat);

MMatrix compute_covariance_matrix(MMatrix& mat);

MMatrix compute_transformation_matrix(MMatrix A, int num_eigenvectors, double delta, bool verbose);

MMatrix power_method(MMatrix& A, double delta);

MMatrix transform_images(MMatrix& images, MMatrix& V);

MMatrix compute_average_by_digit(MMatrix& transf_images, vector<int>& labels);

int classify_images(MMatrix& images, vector<int>& labels, MMatrix& V, MMatrix& avgs, int k);

#endif // __ALGORITHMS_H__