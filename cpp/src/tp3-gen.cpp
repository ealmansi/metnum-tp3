#include <iostream>
#include <vector>
using namespace std;

#include "../lib/commons.h"

#include "cmd-args.h"
#include "mmatrix.h"
#include "data-io.h"
#include "algorithms.h"

/*
- programa generador [recibe: training set image/label files, delta's | devuelve: archivos-binarios-de-datos (uno por cada delta)]
	- parsea el archivo tipo ubyte de mnist, cargando las fotos en una matriz
	- genera la matriz 'X', y computa 'Y' = Xt * X
	- para cada delta
		- computa los autovectores/autovalores de 'Y' (algoritmo QR)
		- ordena los autovectores segun orden decreciente de abs(autovalor) -> matriz V
		- transformo las imagenes usando V
		- computo el promedio de cada clase (o dígito)
		- guarda en un archivo binario a la matriz V y los promedios
*/

int main(int argc, char** argv)
{
	CmdArgsGen args = parse_cmd_args_gen(argc, argv);

	MMatrix images;
	vector<int> labels;
	load_mnist_data(args.images_filename, args.labels_filename, images, labels);
	
	MMatrix cov_mat = compute_covariance_matrix(images);

	vector<double>::const_iterator delta;
	for (delta = args.delta_values.begin(); delta != args.delta_values.end(); ++delta)
	{
	 	MMatrix V, D;
		eigen_decomposition(cov_mat, *delta, V, D);

	 	PRINT_EXPR(V);
	 	PRINT_EXPR(D);

	 	MMatrix transf_images = transform_images(images, V);
		
	 	MMatrix avgs = compute_average_by_digit(transf_images, labels);

	 	write_data_file(*delta, V, avgs);
	}

	return 0;
}
