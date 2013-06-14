#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "mmatrix.h"
#include "algorithms.h"

int main(int argc, char** argv)
{
	srand(time(0));

	MMatrix mat1(5,3);
	MMATRIX_MAP_IJ(mat1, rand()%10);
	PRINT_EXPR(mat1);

	MMatrix mat2(3,3);
	MMATRIX_MAP_IJ(mat2, rand()%10);
	PRINT_EXPR(mat2);

	mat1.multiply_in_place(mat2);
	PRINT_EXPR(mat1);

	MMatrix normalized = normalize(mat1);
	PRINT_EXPR(normalized);

	MMatrix cov_mat = compute_covariance_matrix(normalized);
	PRINT_EXPR(cov_mat);

	MMatrix mat3(7,7);
	MMATRIX_MAP_IJ(mat3, rand()%10);
	PRINT_EXPR(mat3);

	MMatrix Q;
	QR_factorization_in_place(Q, mat3);
	PRINT_EXPR(Q);
	PRINT_EXPR(mat3);

	return 0;
}