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
	
	BEGIN_TIMER();
	load_mnist_data(args.images_filename, args.labels_filename, images, labels);
	PRINT_ON_VERBOSE("Imágenes y etiquetas cargadas correctamente; total de imágenes: " + int2str(images.rows()) + ", tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);

	RESET_TIMER();
	MMatrix cov_mat = compute_covariance_matrix(images);
	PRINT_ON_VERBOSE("Matriz de varianza computada; tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);

	vector<double>::const_iterator delta;
	for (delta = args.delta_values.begin(); delta != args.delta_values.end(); ++delta)
	{
		PRINT_ON_VERBOSE("Comenzando a computar los datos para delta = " + double2str(*delta) + ".", args.verbose);

		RESET_TIMER();
	 	MMatrix V = compute_transformation_matrix(cov_mat, args.number_of_components, *delta, args.verbose);
		PRINT_ON_VERBOSE("Matriz de transformación computada; cantidad de autovectores: " + int2str(args.number_of_components) + ", tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);

		RESET_TIMER();
	 	MMatrix transf_images = transform_images(images, V);
	 	PRINT_ON_VERBOSE("Imágenes transformadas; tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);
		
		RESET_TIMER();
	 	MMatrix avgs = compute_average_by_digit(transf_images, labels);
	 	PRINT_ON_VERBOSE("Promedios según dígito computados; tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);

	 	RESET_TIMER();
	 	write_data_file(*delta, V, avgs);
	 	PRINT_ON_VERBOSE("Datos escritos en el archivo de salida; tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);
	}

	PRINT_ON_VERBOSE("Datos computados para todos los valores de delta. Terminando la ejecución...\n", args.verbose);

	return 0;
}
