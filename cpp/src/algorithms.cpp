#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "algorithms.h"
#include "mmatrix.h"

#define		CONVERGENCE_NOT_ATTAINED(it, err, dlt)	("El algoritmo QR no convergió después de la máxima cantidad de iteraciones (" + int2str(it) + "). Se alcanzó un error de " + double2str(err) + ", ante una tolerancia de " + double2str(dlt))

#define		MAX_ITERATIONS		10000
#define		NUM_DIGITS			10

//	//	gen	//	//

MMatrix compute_mean_row(MMatrix& mat);

MMatrix& normalize_in_place(MMatrix& mat)
{
	MMatrix mean_row = compute_mean_row(mat);

	MMATRIX_MAP_IJ(mat, mat(i,j) - mean_row(j));

	return mat;
}

MMatrix compute_covariance_matrix(MMatrix& mat)
{
	normalize_in_place(mat);

	MMatrix cov_mat(mat.cols(),mat.cols());

	MMATRIX_WALK_IJ(cov_mat,{
		if(i < j) break;
		cov_mat(i,j) = cov_mat(j,i) = MMatrix::dot_col_col(mat, i, mat, j);
	});

	cov_mat /= (mat.rows() - 1);

	return cov_mat;
}

double compute_diagonalization_error(MMatrix& mat)
{
	double res = 0;
	MMATRIX_WALK_IJ(mat,{
		if(i == j) continue;
		res += abs(mat(i,j));
	});

	return res;
}

void QR_factorization_in_place(MMatrix& Q, MMatrix& A)
{
	Q.make_identity_matrix(A.rows());
	for (int i = 0; i < A.cols(); ++i)
		for (int j = i+1; j < A.rows(); ++j)
		{
			if(abs(A(j,i)) < DBL_TOLERANCE_2_ZERO) continue;

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

	Q.t_in_place();
}

void QR_algorithm_in_place(MMatrix& A, MMatrix& V, double delta, int& iteration_count, bool verbose)
{
	MMatrix Q;

	double error = compute_diagonalization_error(A);
	while( iteration_count < MAX_ITERATIONS && delta < error )
	{
		PRINT_ON_VERBOSE("Número de iteración: " + int2str(iteration_count) + ", error: " + double2str(error), verbose);

		QR_factorization_in_place(Q,A);

		PRINT_ON_VERBOSE("Descomposición QR computada.", verbose);

		// A *= Q;
		// V *= Q;
		/* Multiplicación in situ y simultánea para A y V, a ver si acelera un poco... */
		int size = A.rows();
		double aux_row_a[size], aux_row_v[size], aux_row_q[size];
		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				aux_row_a[j] = A.e(i,j);
				aux_row_v[j] = V.e(i,j);
			}

			for (int j = 0; j < size; ++j)
			{
				for (int k = 0; k < size; ++k)
					aux_row_q[k] = Q.e(k,j);

				double a_ij_a = 0, a_ij_v = 0;
				for (int k = 0; k < size; ++k)
				{
					a_ij_a += aux_row_a[k] * aux_row_q[k];
					a_ij_v += aux_row_v[k] * aux_row_q[k];
				}
				A.e(i,j) = a_ij_a;
				V.e(i,j) = a_ij_v;
			}
		}

		error = compute_diagonalization_error(A);
		iteration_count++;
	}

	if( iteration_count == MAX_ITERATIONS )
		DISPLAY_ERROR_AND_EXIT(CONVERGENCE_NOT_ATTAINED(iteration_count, error, delta));
}

MMatrix transform_images(MMatrix& images, MMatrix& V)
{
	return images * V;
}

MMatrix compute_average_by_digit(MMatrix& transf_images, vector<int>& labels)
{
	int digits_count[NUM_DIGITS] = {0};
	MMatrix avgs(NUM_DIGITS, transf_images.cols(), 0.0);

	MMATRIX_WALK_IJ(transf_images,{
		avgs(labels.at(i), j) += transf_images(i,j);
		digits_count[labels.at(i)]++;
	});

	MMATRIX_WALK_IJ(avgs, avgs(i,j) / digits_count[i]);

	return avgs;
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