#include <vector>
#include <iostream>
using namespace std;

#include "algorithms.h"
#include "mmatrix.h"

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

void QR_algorithm(MMatrix& mat, double delta, MMatrix& V, MMatrix& D)
{

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