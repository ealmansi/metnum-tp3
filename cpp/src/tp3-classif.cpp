#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "../lib/commons.h"
#include "cmd-args.h"
#include "mmatrix.h"
#include "data-io.h"
#include "algorithms.h"

#define		NOT_ENOUGH_PRINC_COMPONENTS(amnt,k)		\
				("El archivo de datos solo contiene información sobre las primeras " + int2str(amnt) + " componentes principales, por lo que no se puede realizar la clasificación con un valor de k: " + int2str(k))

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
	open_output_file(args.output_filename,output_file);

	MMatrix V, avgs;
	vector<string>::const_iterator data_file;
	for (data_file = args.data_files.begin(); data_file != args.data_files.end(); ++data_file)
	{
		double delta;

		RESET_TIMER();
		load_data_file(*data_file, delta, V, avgs);
		PRINT_ON_VERBOSE("Archivo de datos cargado, delta: " + double2str(delta) + ", tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);

		vector<int>::const_iterator k;
		for (k = args.k_values.begin(); k != args.k_values.end(); ++k)
		{
			if(V.cols() < *k)
				DISPLAY_ERROR_AND_EXIT(NOT_ENOUGH_PRINC_COMPONENTS(V.cols(), *k));

			PRINT_ON_VERBOSE("Realizando clasificación con k: " + int2str(*k), args.verbose);

			RESET_TIMER();
			int hits = classify_images(images, labels, V, avgs, *k);
			PRINT_ON_VERBOSE("Clasificaciones acertadas: " + int2str(hits) + ", tiempo (ms):" + int2str(MSECS_ELAPSED()) + ".", args.verbose);
			PRINT_ON_VERBOSE("Porcentaje de aciertos: " + double2str(hits*100.0/images.rows()) + "%.", args.verbose);

			write_results(output_file, delta, *k, args.images_filename, images.rows(), hits);
		}
	}

	close_output_file(output_file);

	PRINT_ON_VERBOSE("Clasificación realizada para todos los archivos de datos y valores de k. Terminando la ejecución...\n", args.verbose);

	return 0;
}