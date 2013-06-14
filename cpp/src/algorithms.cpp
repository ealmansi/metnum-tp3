#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "algorithms.h"
#include "mmatrix.h"

#define		CONVERGENCE_NOT_ATTAINED(it, dlt)	("El algoritmo QR no convergió después de la máxima cantidad de iteraciones (" + int2str(it) + "), dada una tolerancia delta = " + double2str(dlt))

#define		MAX_ITERATIONS		10

//	//	gen	//	//

MMatrix compute_mean_row(MMatrix& mat);

MMatrix normalize(MMatrix& mat)
{
	MMatrix mean_row = compute_mean_row(mat);
	
	MMatrix normalized(mat.rows(), mat.cols());
	MMATRIX_MAP_IJ(normalized, mat(i,j) - mean_row(j));

	return normalized;
}

MMatrix compute_covariance_matrix(MMatrix& mat)
{
	MMatrix cov_mat(mat.cols(), mat.cols());
	MMATRIX_WALK_IJ(cov_mat,{
		if( i < j ) break;
		cov_mat(i,j) = 0;
		for (int k = 0; k < mat.rows(); ++k)
			cov_mat(i,j) += mat(k,i) * mat(k,j);
		cov_mat(j,i) = cov_mat(i,j);
	});

	cov_mat /= (mat.rows() - 1);

	return cov_mat;
}

MMatrix identity_matrix(int n)
{
	MMatrix res(n,n, 0.0);
	for (int i = 0; i < n; ++i)
		res(i,i) = 1;

	return res;
}

double compute_sum_ignoring_diagonal(MMatrix& mat)
{
	double res = 0;
	MMATRIX_WALK_IJ(mat,{
		if(i == j) continue;
		res += mat(i,j);
	});

	return res;
}

void QR_factorization_in_place(MMatrix& Q, MMatrix& A)
{
	Q = identity_matrix(A.rows());
	for (int i = 0; i < A.cols(); ++i)
		for (int j = i+1; j < A.rows(); ++j)
		{
			double x1 = A(i,i), x2 = A(j,i);
			double r = sqrt(x1 * x1 + x2 * x2);
			double c = x1 / r, s = x2 / r;

			A(i,i) = r;
			A(j,i) = 0;
			for (int k = 0; k < A.cols(); ++k)
			{
				double q1 = Q(i,k), q2 = Q(j,k);
				Q(i,k) = c * q1 + s * q2;
				Q(j,k) = -s * q1 + c * q2;
				if( i < k )
				{
					double x1 = A(i,k), x2 = A(j,k);
					A(i,k) = c * x1 + s * x2;
					A(j,k) = -s * x1 + c * x2;
				}
			}
		}

	Q = Q.t();
}

void QR_algorithm(MMatrix& mat, double delta, MMatrix& V, MMatrix& D)
{
	D = mat;
	V = identity_matrix(mat.rows());
	MMatrix Q(mat.rows(), mat.rows());

	int iteration_count = 0;
	while( iteration_count++ < MAX_ITERATIONS && delta < compute_sum_ignoring_diagonal(D) )
	{
		QR_factorization_in_place(Q,D);
		D.multiply_in_place(Q);
		V.multiply_in_place(Q);
	}

	if( iteration_count == MAX_ITERATIONS )
		DISPLAY_ERROR_AND_EXIT(CONVERGENCE_NOT_ATTAINED(iteration_count, delta));
}

void ensure_positive_diagonal(MMatrix& V, MMatrix& D)
{

}

void sort_eigenvectors(MMatrix& V, MMatrix& D)
{

}

void eigen_decomposition(MMatrix& mat, double delta, MMatrix& V, MMatrix& D)
{
	QR_algorithm(mat, delta, V, D);
	ensure_positive_diagonal(V, D);
	sort_eigenvectors(V, D);
}

MMatrix transform_images(MMatrix& images, MMatrix& V)
{

}

MMatrix compute_average_by_digit(MMatrix& transf_images, vector<int>& labels)
{

}

MMatrix compute_mean_row(MMatrix& mat)
{
	MMatrix mean_row(1, mat.cols(), 0.0);
	MMATRIX_WALK_IJ(mat, {
		mean_row(j) += mat(i,j);
	});

	mean_row /= mat.rows();

	return mean_row;
}

//	//	classif	//	//

int classify_image(MMatrix& image_row, MMatrix& V, MMatrix& avgs, int k)
{

}

int classify_images(MMatrix& images, vector<int>& labels, MMatrix& V, MMatrix& avgs, int k)
{

}