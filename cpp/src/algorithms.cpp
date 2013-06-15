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

void QR_algorithm(MMatrix& mat, double delta, MMatrix& V, MMatrix& D, bool verbose)
{
	PRINT_ON_VERBOSE("Comenzando algoritmo QR.", verbose);

	D = mat;
	V.make_identity_matrix(mat.rows());
	MMatrix Q(mat.rows(), mat.rows());

	double error = compute_diagonalization_error(D);
	int iteration_count = 0;
	while( iteration_count < MAX_ITERATIONS && delta < error )
	{
		PRINT_ON_VERBOSE("Número de iteración: " + int2str(iteration_count) + ", error: " + double2str(error), verbose);

		QR_factorization_in_place(Q,D);

		PRINT_ON_VERBOSE("Descomposición QR computada.", verbose);

		D *= Q;
		V *= Q;

		error = compute_diagonalization_error(D);
		iteration_count++;
	}

	if( iteration_count == MAX_ITERATIONS )
		DISPLAY_ERROR_AND_EXIT(CONVERGENCE_NOT_ATTAINED(iteration_count, error, delta));
}

// void ensure_positive_diagonal(MMatrix& V, MMatrix& D)
// {
// 	for (int i = 0; i < D.cols(); ++i)
// 	{
// 		if( D(i,i) < 0 )
// 		{
// 			D(i,i) = -D(i,i);
// 			for (int k = 0; k < V.rows(); ++k)
// 			{
// 				V(k,i) = -V(k,i);
// 			}
// 		}
// 	}
// }

// bool compare_index_eigenvalue_pair(const pair<int,double>& a, const pair<int,double>& b)
// {
// 	return (a.second > b.second);
// }

// void sort_eigenvectors(MMatrix& V, MMatrix& D)
// {
// 	vector<pair<int,double> > index_eigenvalue_pairs;
// 	for (int i = 0; i < D.cols(); ++i)
// 		index_eigenvalue_pairs.push_back(pair<int,double>(i, D(i,i)));

// 	sort(index_eigenvalue_pairs.begin(), index_eigenvalue_pairs.end(), compare_index_eigenvalue_pair);

// 	MMatrix sorted_V(V.rows(), V.cols());
// 	vector<pair<int,double> >::const_iterator in_ev;
// 	for (in_ev = index_eigenvalue_pairs.begin(); in_ev != index_eigenvalue_pairs.end(); ++in_ev)
// 	{
// 		int j = in_ev->first;
// 		for (int i = 0; i < V.rows(); ++i)
// 			sorted_V(i,j) = V(i,j);
// 	}

// 	V = sorted_V;
// }

void eigen_decomposition(MMatrix& mat, double delta, MMatrix& V, MMatrix& D, bool verbose)
{
	QR_algorithm(mat, delta, V, D, verbose);
	
	/*Por alguna razón que desconozco, QR_algorithm ya los deja ordenados y positivos
	* si la matriz es simétrica y semidefinida positiva (como una matriz de covarianza).
	*/
	// ensure_positive_diagonal(V, D);
	// sort_eigenvectors(V, D);
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