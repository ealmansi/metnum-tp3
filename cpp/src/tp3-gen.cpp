#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#include "../lib/commons.h"

#include "cmd-args.h"
#include "mmatrix.h"
#include "data-io.h"
#include "algorithms.h"

void generate_data(MMatrix& images, vector<int>& labels,
	MMatrix& A, vector<double>& delta_values, bool verbose);
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

	PRINT_ON_VERBOSE("Matriz de varianza computada.", args.verbose);

	generate_data(images, labels, cov_mat, args.delta_values, args.verbose);

	PRINT_ON_VERBOSE("Datos computados para todos los valores de delta. Terminando la ejecución...", args.verbose);

	return 0;
}

/*
* Ordena los valores de delta en orden decreciente, e itera el algoritmo QR
* hasta lograr un error menor al menor delta, guardando en archivos los
* resultados parciales cada vez que se satisfaga un valor de delta menos restrictivo.
*/
void generate_data(MMatrix& images, vector<int>& labels,
	MMatrix& A, vector<double>& delta_values, bool verbose)
{
	sort(delta_values.begin(), delta_values.end(), greater<double>());

	MMatrix V;
	V.make_identity_matrix(A.rows());

	int iteration_count = 0;

	PRINT_ON_VERBOSE("Comenzando algoritmo QR.", verbose);

	vector<double>::const_iterator delta;
	for (delta = delta_values.begin(); delta != delta_values.end(); ++delta)
	{
		QR_algorithm_in_place(A, V, *delta, iteration_count, verbose);

	 	MMatrix transf_images = transform_images(images, V);
		
	 	MMatrix avgs = compute_average_by_digit(transf_images, labels);

	 	write_data_file(*delta, V, avgs);

	 	PRINT_ON_VERBOSE("Datos computados para delta = " + double2str(*delta) + ".", verbose);
	}
}