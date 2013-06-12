#include <iostream>
using namespace std;

#include "gen-algorithms.h"

MMatrix normalize(MMatrix& mat)
{
	
}

MMatrix compute_covariance_matrix(MMatrix& mat)
{
	
}

void eigen_decomposition(MMatrix& cov_mat, double delta, MMatrix& V, MMatrix& D)
{
	QR_algorithm(cov_mat, delta, V, D);
	ensure_positive_diagonal(V, D);
	sort_eigenvectors(V, D);
}

void QR_algorithm(MMatrix& cov_mat, double delta, MMatrix& V, MMatrix& D)
{

}

void ensure_positive_diagonal(MMatrix& V, MMatrix& D)
{

}

void sort_eigenvectors(MMatrix& V, MMatrix& D)
{

}

MMatrix transform_images(MMatrix& images, MMatrix& V)
{

}

void compute_average_by_digit(MMatrix& transf_images, vector<int>& labels)
{

}