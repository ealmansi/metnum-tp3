#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "algorithms.h"
#include "mmatrix.h"

#define		CONVERGENCE_NOT_ATTAINED(it, err, dlt)		\
				("El algoritmo QR no convergió después de la máxima cantidad de iteraciones (" + int2str(it) + "). Se alcanzó un error de " + double2str(err) + ", ante una tolerancia de " + double2str(dlt))
#define		CONVERGENCE_NOT_ATTAINED_POWER_MTH(it, drch, dlt)		\
				("El método de la potencia extendido no convergió después de la máxima cantidad de iteraciones (" + int2str(it) + "). Luego de la última iteración, el cambio en dirección de la estimación era de " + double2str(drch) + ", ante un valor máximo requerido de: " + double2str(dlt))

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

		BEGIN_TIMER();

		QR_factorization_in_place(Q,A);

		PRINT_ON_VERBOSE("Descomposición QR computada. Tiempo: " + double2str(MSECS_ELAPSED()) + ".", verbose);

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
				A.e(i,j) = 0;
				aux_row_v[j] = V.e(i,j);
				V.e(i,j) = 0;
			}

			for (int k = 0; k < size; ++k)
			{
				for (int j = 0; j < size; ++j)
				{
					A.e(i,j) += aux_row_a[k] * Q(k,j);
					V.e(i,j) += aux_row_v[k] * Q(k,j);
				}
			}
		}

		PRINT_ON_VERBOSE("Fin de iteración. Tiempo: " + double2str(MSECS_ELAPSED()) + ".", verbose);

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

double norm(MMatrix& m)
{
	double acc = 0;
	MMATRIX_WALK_IJ(m, {
		acc += m(i,j)*m(i,j);
	});

	return sqrt(acc);
}

void extended_power_method(MMatrix& A, int k, double delta, MMatrix& V)
{
	V.set_size(A.rows(),k);
	for (int pc = 0; pc < k; ++pc)
	{
		PRINT_ON_VERBOSE("Empezando con el autovector número: " + int2str(pc), true)
		MMatrix v;
		power_method(A, delta, v);
		double lambda = (v.t() * A * v)(0,0);

		MMATRIX_MAP_IJ(A, A(i,j) - lambda * v(i) * v(j) );

		for (int i = 0; i < A.rows(); ++i)
			V(i,pc) = v(i);
	}
}

void power_method(MMatrix& A, double delta, MMatrix& v)
{
	v.set_size(A.rows(),1);
	MMATRIX_MAP_IJ(v, ((double)rand())/RAND_MAX);
	v /= norm(v);

	int iteration_count;
	double direction_rate_of_change;
	for (iteration_count = 0; iteration_count < MAX_ITERATIONS; ++iteration_count)
	{
		MMatrix y = A * v;
		y /= norm(y);

		direction_rate_of_change = 1 - abs(MMatrix::dot_col_col(v,0,y,0));
		if(direction_rate_of_change <= delta) break;

		v = y;
	}

	if(iteration_count == MAX_ITERATIONS)
		DISPLAY_ERROR_AND_EXIT(CONVERGENCE_NOT_ATTAINED_POWER_MTH(iteration_count, direction_rate_of_change, delta));
}

//	//	classif	//	//

int classify_image(MMatrix& image_row, MMatrix& V, MMatrix& avgs, int k)
{

}

int classify_images(MMatrix& images, vector<int>& labels, MMatrix& V, MMatrix& avgs, int k)
{

}