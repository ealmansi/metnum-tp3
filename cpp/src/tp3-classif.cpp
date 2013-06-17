#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "../lib/commons.h"
#include "cmd-args.h"
#include "mmatrix.h"
#include "data-io.h"
#include "algorithms.h"

#define		FILE_NOT_CREATED(fn)	\
				("No se pudo crear el archivo: " + fn)

/*
- programa clasificador [recibe: test set image/label files, k's, archivos-binarios-de-datos | devuelve: archivo de resultados]
	- parsea los archivos tipo ubyte de mnist, cargando las fotos en una matriz y las labels en un vector
	- genero archivo de resultados
	- para cada archivo-binario-de-datos
		- cargo V y el promedio de cada clase (o dígito)
		- para cada k
			- clasifico todas las imágenes, comparando contra los promedios
			- cuento los aciertos
			- anoto resultados en el archivo de resultados
*/

int main(int argc, char** argv)
{
	CmdArgsClassif args = parse_cmd_args_classif(argc, argv);

	MMatrix images;
	vector<int> labels;
	
	BEGIN_TIMER();
	load_mnist_data(args.images_filename, args.labels_filename, images, labels);
	PRINT_ON_VERBOSE("Imágenes y etiquetas cargadas correctamente; total de imágenes: " + int2str(images.rows()) + ", tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);

	ofstream output_file;
	output_file.open(args.output_filename.c_str());
	if(!output_file.is_open())
		DISPLAY_ERROR_AND_EXIT(FILE_NOT_CREATED(args.output_filename));

	MMatrix V, avgs;
	vector<string>::const_iterator data_file;
	for (data_file = args.data_files.begin(); data_file != args.data_files.end(); ++data_file)
	{
		double delta;
		load_data_file(*data_file, delta, V, avgs);
		PRINT_ON_VERBOSE("Archivo de datos cargado, delta: " + double2str(delta) + ".", args.verbose);

		// vector<int>::const_iterator k;
		// for (k = args.k_values.begin(); k != args.k_values.end(); ++k)
		// {
		// 	PRINT_ON_VERBOSE("Comenzando la clasificación con k: " + int2str(k), args.verbose);
		// 	int hits = classify_images(images, labels, V, avgs, *k);
		// 	write_results(output_file, delta, *k, hits);
		// }
	}

	output_file.close();

	return 0;
}