#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#include "../lib/commons.h"
#include "mmatrix.h"
#include "algorithms.h"

void print_mat(MMatrix& mat);

int main(int argc, char** argv)
{
	srand(time(0));

	MMatrix mat(5,3);
	MMATRIX_MAP_IJ(mat, rand()%10);

	MMatrix normalized = normalize(mat);
	MMatrix cov_mat = compute_covariance_matrix(normalized);

	print_mat(mat);
	print_mat(normalized);
	print_mat(cov_mat);

	return 0;
}

void print_mat(MMatrix& mat)
{
	for (int i = 0; i < mat.rows(); ++i)
	{
		for (int j = 0; j < mat.cols(); ++j)
		{
			cout << mat(i,j);
			if(j + 1 < mat.rows())
				cout << " ";
		}
		cout << endl;
	}
}