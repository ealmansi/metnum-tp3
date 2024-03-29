#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "algorithms.h"
#include "mmatrix.h"

#define		CONVERGENCE_NOT_ATTAINED_POWER_MTH(it, drch, dlt)		\
				("El método de la potencia no convergió después de la máxima cantidad de iteraciones (" + int2str(it) + "). Luego de la última iteración, el cambio en dirección de la estimación era de " + double2str(drch) + ", ante un valor máximo aceptado de: " + double2str(dlt))

#define		MAX_ITERATIONS		10000

#define		NUM_DIGITS			10

//	//	//	//

MMatrix compute_mean_row(MMatrix& mat);
void extended_power_method(MMatrix& A, int k, double delta, MMatrix& V);
void power_method(MMatrix& A, double delta, MMatrix& v);
void sort_eigenvectors(MMatrix& V, vector<double>& eigenvalues);
double compute_raleygh_quotient(MMatrix& v, MMatrix& A);
double norm(MMatrix& m);

//	//	//	//

MMatrix normalize_variables(MMatrix& mat)
{
	MMatrix mean_row = compute_mean_row(mat);

	MMatrix norm_mat(mat.rows(), mat.cols());
	foreach_a_ij(norm_mat, a_ij = mat(i,j) - mean_row(j));

	return norm_mat;
}

void compute_covariance_matrix(MMatrix& mat, MMatrix& cov_mat)
{
	MMatrix norm_mat = normalize_variables(mat);

	cov_mat.set_size(norm_mat.cols(),norm_mat.cols());
	double denominator = 1.0/(norm_mat.rows() - 1);

	foreach_a_ij_lower_triangular(cov_mat,{
		cov_mat(i,j) = cov_mat(j,i) = (MMatrix::dot_col_col(norm_mat, i, norm_mat, j) * denominator);
	});
}

MMatrix compute_transformation_matrix(MMatrix A, int num_eigenvectors, double delta, bool verbose)
{
	MMatrix V(A.rows(), num_eigenvectors);
	vector<double> eigenvalues;
	for (int k = 0; k < num_eigenvectors; ++k)
	{
		BEGIN_TIMER();

		MMatrix v = power_method(A, delta);
		double lambda = compute_raleygh_quotient(v, A);
		eigenvalues.push_back(lambda);

		/* deflation */
		foreach_a_ij(A, a_ij = a_ij - lambda * v(i) * v(j) );

		foreach_v_i(v,{
			V(i,k) = v_i;
		});

		PRINT_ON_VERBOSE("Autovector número " + int2str(k) + " calculado (" + int2str(MSECS_ELAPSED()) + " ms).", verbose)
	}

	sort_eigenvectors(V, eigenvalues);

	return V;
}

MMatrix power_method(MMatrix& A, double delta)
{
	MMatrix v(A.rows(),1);
	foreach_v_i(v, v_i = ((double)rand())/RAND_MAX);
	v /= norm(v);

	int iteration_count;
	double direction_rate_of_change;
	for (iteration_count = 0; iteration_count < MAX_ITERATIONS; ++iteration_count)
	{
		MMatrix y = A * v;
		y /= norm(y);

		direction_rate_of_change = 1 - abs(MMatrix::dot(v,y));
		if(direction_rate_of_change <= delta) break;

		v = y;
	}

	if(iteration_count == MAX_ITERATIONS)
		DISPLAY_ERROR_AND_EXIT(CONVERGENCE_NOT_ATTAINED_POWER_MTH(iteration_count, direction_rate_of_change, delta));

	return v;
}

bool compare_index_eigenvalue_pair(pair<int,double> a, pair<int,double> b)
{
	return a.second > b.second;
}

void sort_eigenvectors(MMatrix& V, vector<double>& eigenvalues)
{
	vector<pair<int,double> > index_eigenvalue_pairs;
	for (int i = 0; i < eigenvalues.size(); ++i)
		index_eigenvalue_pairs.push_back(pair<int,double>(i, eigenvalues.at(i)));

	sort(index_eigenvalue_pairs.begin(), index_eigenvalue_pairs.end(), compare_index_eigenvalue_pair);

	MMatrix sorted_V(V.rows(), V.cols());
	for(int j = 0; j < V.cols(); ++j)
	{
		int prev_row = index_eigenvalue_pairs.at(j).first;
		for (int i = 0; i < V.rows(); ++i)
			sorted_V(i,j) = V(i, prev_row);
	}

	V = sorted_V;
}

/* no dimension check, only unary vectors*/
double compute_raleygh_quotient(MMatrix& v, MMatrix& A)
{
	double res = 1;
	foreach_a_ij(A,{
		res += a_ij * v(i) * v(j);
	});

	return res;
}

/* no dimension check, only vectors*/
double norm(MMatrix& v)
{
	double res = 0;
	foreach_v_i(v, {
		res += v_i * v_i;
	});

	return sqrt(res);
}

MMatrix transform_images(MMatrix& images, MMatrix& V)
{
	return images * V;
}

MMatrix compute_average_by_digit(MMatrix& transf_images, vector<int>& labels)
{
	int images_per_digit[NUM_DIGITS] = {0};
	MMatrix avgs(NUM_DIGITS, transf_images.cols(), 0.0);

	for (int i = 0; i < transf_images.rows(); ++i)
	{
		int digit_index = labels.at(i);
		++images_per_digit[digit_index];

		for (int j = 0; j < transf_images.cols(); ++j)
			avgs(digit_index, j) += transf_images(i,j);
	}

	foreach_a_ij(avgs, a_ij = (a_ij / images_per_digit[i]));

	return avgs;
}

MMatrix compute_mean_row(MMatrix& mat)
{
	MMatrix mean_row(1, mat.cols(), 0.0);
	foreach_a_ij(mat, {
		mean_row(j) += a_ij;
	});

	mean_row /= mat.rows();

	return mean_row;
}

int classify_image(MMatrix& transf_image, MMatrix& V, MMatrix& avgs, int k)
{
	double min_dist = +INFINITY, dist;
	int digit = -1;

	for (int d = 0; d < NUM_DIGITS; ++d)
	{
		MMatrix diff(1, k);
		foreach_v_i(diff, v_i = transf_image(i) - avgs(d,i));

		dist = norm(diff);
		if( dist < min_dist )
		{
			min_dist = dist;
			digit = d;
		}
	}

	return digit;
}

int classify_images(MMatrix& images, vector<int>& labels, MMatrix& V, MMatrix& avgs, int k)
{
	int hits = 0;

	MMatrix transf_images = transform_images(images, V);

	for (int i = 0; i < transf_images.rows(); ++i)
	{
		MMatrix transf_image;
		transf_images.copy_row(i, transf_image);

		int digit = classify_image(transf_image, V, avgs, k);
		if(digit == labels.at(i)) ++hits;
	}

	return hits;
}