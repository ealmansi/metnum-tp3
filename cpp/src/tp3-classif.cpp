#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#include "cmd-args.h"
#include "mmatrix.h"
#include "data-io.h"
#include "algorithms.h"

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

int main(int argc, char const *argv[])
{
	CmdArgsClassif args = parse_cmd_args_classif(argc, argv);

	MMatrix images;
	vector<int> labels;
	load_mnist_data(args.images_filename, args.labels_filename, images, labels);

	ofstream output_file;
	output_file.open(args.output_filename.c_str());

	MMatrix V, avgs;
	vector<string>::const_iterator data_file;
	for (data_file = args.data_files.begin(); data_file != args.data_files.end(); ++data_file)
	{
		double delta;
		load_data_file(*data_file, delta, V, avgs);

		vector<int>::const_iterator k;
		for (k = args.k_values.begin(); k != args.k_values.end(); ++k)
		{
			int hits = classify_images(images, labels, V, avgs, *k);
			write_results(output_file, delta, *k, hits);
		}
	}

	output_file.close();

	return 0;
}