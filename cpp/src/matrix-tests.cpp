#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
using namespace std;

#include "../lib/commons.h"
#include "mmatrix.h"
#include "algorithms.h"
#include "data-io.h"

int main(int argc, char** argv) {
	srand(time(0));

	// MMatrix mat1(10, 10);
	// MMATRIX_MAP_IJ(mat1, rand()%10);
	// PRINT_EXPR(mat1);

	// MMatrix mat2(10, 10);
	// MMATRIX_MAP_IJ(mat2, rand()%10);
	// PRINT_EXPR(mat2);

	// MMatrix mat3 = mat1 * mat2;
	// PRINT_EXPR(mat3);

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

	// MMatrix imgs;

	// load_ubyte_images("../data/train-images.idx3-ubyte", imgs);

	// PRINT_EXPR(imgs);

	// MMatrix& imgs_n = normalize_in_place(imgs);

	// PRINT_EXPR(imgs_n)

	// MMatrix imgs_n_t = imgs_n;

	// double sqrt_n = sqrt((double) imgs.rows() - 1);

	// MMATRIX_MAP_IJ(imgs_n_t, imgs_n_t(i, j) / sqrt_n);

	// MMatrix X_prod_Xt = imgs_n_t.t() * imgs_n_t;

	// PRINT_EXPR(X_prod_Xt)

	MMatrix fake_data(100,700);
	MMATRIX_MAP_IJ(fake_data, rand()%255);
	// PRINT_EXPR(fake_data);

	MMatrix& norm_fake_data = fake_data;
	normalize_in_place(norm_fake_data);
	// PRINT_EXPR(norm_fake_data);

	MMatrix fake_cov_mat = fake_data.t() * fake_data;
	fake_cov_mat /= (fake_data.rows() - 1);
	PRINT_EXPR(fake_cov_mat);

	MMatrix V;
	extended_power_method(fake_cov_mat, 1, 1e-8, V);
	PRINT_EXPR(V);

	// MMatrix V;
	// V.make_identity_matrix(fake_cov_mat.rows());

	// int iteration_count = 0;

	// QR_algorithm_in_place(fake_cov_mat, V, 1e-3, iteration_count, true);
	// MMatrix& A_1 = fake_cov_mat; PRINT_EXPR(A_1);
	// MMatrix& V_1 = V; PRINT_EXPR(V_1);

	// QR_algorithm_in_place(fake_cov_mat, V, 1e-4, iteration_count, true);
	// MMatrix& A_2 = fake_cov_mat; PRINT_EXPR(A_2);
	// MMatrix& V_2 = V; PRINT_EXPR(V_2);

	// QR_algorithm_in_place(fake_cov_mat, V, 1e-5, iteration_count, true);
	// MMatrix& A_3 = fake_cov_mat; PRINT_EXPR(A_3);
	// MMatrix& V_3 = V; PRINT_EXPR(V_3);

	//     Rayleigh's method is:  (1) Start with an initial non-zero vector x0,   //
//     (2) Normalize the current vector x[k], (3) x[k+1] = A x[k],            //
//     (4) let z = x[k+1]'x[k], (5) if the relative difference of the new     //
//     value of z and the old value of z is less than a preassigned tolerance,//
//     then halt the procedure; otherwise go to step (2).                     //

	return 0;
}
