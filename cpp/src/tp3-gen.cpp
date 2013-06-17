#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#include "../lib/commons.h"

#include "cmd-args.h"
#include "mmatrix.h"
#include "data-io.h"
#include "algorithms.h"

/*
* - programa generador [recibe: training set image/label files, delta's | devuelve: archivos-binarios-de-datos (uno por cada delta)]
* 	- parsea el archivo tipo ubyte de mnist, cargando las fotos en una matriz
* 	- genera la matriz 'X', y computa 'Y' = Xt * X
* 	- para cada delta
* 		- computa los autovectores/autovalores de 'Y' (algoritmo QR)
* 		- ordena los autovectores segun orden decreciente de abs(autovalor) -> matriz V
* 		- transformo las imagenes usando V
* 		- computo el promedio de cada clase (o dígito)
* 		- guarda en un archivo binario a la matriz V y los promedios
*/

int main(int argc, char** argv)
{
	CmdArgsGen args = parse_cmd_args_gen(argc, argv);

	PRINT_ON_VERBOSE("Comenzando ejecución del programa.", args.verbose);

	MMatrix images;
	vector<int> labels;
	load_mnist_data(args.images_filename, args.labels_filename, images, labels);
	
	PRINT_ON_VERBOSE("Imágenes y etiquetas cargadas correctamente; total de imágenes: " + int2str(images.rows()) + ".", args.verbose);

	MMatrix cov_mat = compute_covariance_matrix(images);
	PRINT_EXPR(cov_mat);

	PRINT_ON_VERBOSE("Matriz de varianza computada.", args.verbose);

	vector<double>::const_iterator delta;
	for (delta = args.delta_values.begin(); delta != args.delta_values.end(); ++delta)
	{
		PRINT_ON_VERBOSE("Comenzando a computar los datos para delta = " + double2str(*delta), args.verbose);

	 	MMatrix V = compute_transformation_matrix(cov_mat, 30, *delta, args.verbose);

		PRINT_ON_VERBOSE("Matriz de transformación computada.", args.verbose);

	 	PRINT_EXPR(V);

	 	// MMatrix transf_images = transform_images(images, V);

	 	// PRINT_ON_VERBOSE("Imágenes transformadas.", args.verbose);
		
	 	// MMatrix avgs = compute_average_by_digit(transf_images, labels);

	 	// PRINT_ON_VERBOSE("Promedios según dígito computados.", args.verbose);

	 	// write_data_file(*delta, V, avgs);

	 	// PRINT_ON_VERBOSE("Datos escritos en el archivo de salida.", args.verbose);
	}

	PRINT_ON_VERBOSE("Datos computados para todos los valores de delta. Terminando la ejecución...", args.verbose);

	return 0;
}
