#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "../lib/common_macros.h"

#include "data-io.h"
#include "mmatrix.h"

#define		INVALID_FILE_FORMAT(fn)					( "El archivo \"" + fn + "\" no cumple con el formato IDX. Ver http://yann.lecun.com/exdb/mnist/" )
#define		IMAGES_LABELS_INCONSISTENCY(igs,lbs)	( "Los archivos \"" + igs + "\" y \"" + lbs + "\" no contienen la misma cantidad de elementos" )

#define		IMAGE_HEIGHT_PXS	28
#define		IMAGE_WIDTH_PXS		28

void load_ubyte_images(string filename, MMatrix& images)
{
	ifstream file (filename.c_str(), ios::in | ios::binary);

	char buffer[4];

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int magic_number = (((int)buffer[0] << 24) + ((int)buffer[1] << 16) + ((int)buffer[2] << 8) + ((int)buffer[3]));

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_images = (((int)buffer[0] << 24) + ((int)buffer[1] << 16) + ((int)buffer[2] << 8) + ((int)buffer[3]));

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_rows = (((int)buffer[0] << 24) + ((int)buffer[1] << 16) + ((int)buffer[2] << 8) + ((int)buffer[3]));

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_cols = (((int)buffer[0] << 24) + ((int)buffer[1] << 16) + ((int)buffer[2] << 8) + ((int)buffer[3]));

	if( number_of_rows != IMAGE_HEIGHT_PXS || number_of_cols != IMAGE_WIDTH_PXS )
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));

	for (int i = 0; i < number_of_rows; ++i)
		for (int j = 0; j < number_of_cols; ++j)
		{
			file.read(buffer, 1);
			if(!file)
				DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
			images(i,j) = ((double)buffer[0]);
		}

	file.close();
}

void load_ubyte_labels(string filename, vector<int>& labels)
{
	ifstream file (filename.c_str(), ios::in | ios::binary);

	char buffer[4];

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int magic_number = (((int)buffer[0] << 24) + ((int)buffer[1] << 16) + ((int)buffer[2] << 8) + ((int)buffer[3]));

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_items = (((int)buffer[0] << 24) + ((int)buffer[1] << 16) + ((int)buffer[2] << 8) + ((int)buffer[3]));

	for (int i = 0; i < number_of_items; ++i)
	{
		file.read(buffer, 1);
		if(!file)
			DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
		labels.push_back(((int)buffer[0]));
	}

	file.close();
}

void load_mnist_data(string images_filename, string labels_filename, MMatrix& images, vector<int>& labels)
{
 	load_ubyte_images(images_filename, images);
	load_ubyte_labels(labels_filename, labels);
	if(images.rows() != labels.size())
		DISPLAY_ERROR_AND_EXIT(IMAGES_LABELS_INCONSISTENCY(images_filename, labels_filename));
}

void write_data_file(double delta, MMatrix& V, MMatrix& avgs)
{

}

void load_data_file(string filename, double& delta, MMatrix& V, MMatrix& avgs)
{

}

void write_results(ofstream& output_file, double delta, int k, int hits)
{

}