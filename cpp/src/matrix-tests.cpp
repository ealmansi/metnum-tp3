#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "mmatrix.h"
#include "algorithms.h"
#include "data-io.h"

int main(int argc, char** argv)
{
	srand(time(0));

	// MMatrix mat1(10,7);
	// MMATRIX_MAP_IJ(mat1, rand()%10);
	// PRINT_EXPR(mat1);

	// MMatrix mat2(3,3);
	// MMATRIX_MAP_IJ(mat2, rand()%10);
	// PRINT_EXPR(mat2);

	// mat1.multiply_in_place(mat2);
	// PRINT_EXPR(mat1);

	// MMatrix normalized = normalize(mat1);
	// PRINT_EXPR(normalized);


	// MMatrix mat3(7,7);
	// MMATRIX_MAP_IJ(mat3, rand()%10);
	// PRINT_EXPR(mat3);

	// MMatrix Q;
	// QR_factorization_in_place(Q, mat3);
	// PRINT_EXPR(Q);
	// PRINT_EXPR(mat3);

	MMatrix imgs;
	load_ubyte_images("../data/train-images.idx3-ubyte", imgs);

	MMatrix& imgs_n = normalize_in_place(imgs);

	MMatrix imgs_n_t = imgs_n.t();

	MMatrix prod = imgs_n_t * imgs_n;

	/*	en matlab:
		>> salida
		>> norm(res - lhs*rhs)
	*/

	// MMatrix V, D;
	// eigen_decomposition(cov_mat, 1e-5, V, D);
	// PRINT_EXPR(V);
	// PRINT_EXPR(D);

	// MMatrix V_t = V.t();
	// PRINT_EXPR(V*D*V_t - cov_mat);

	return 0;
}