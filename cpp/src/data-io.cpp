#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "../lib/commons.h"

#include "data-io.h"
#include "mmatrix.h"

#define		INVALID_FILE_FORMAT(fn)					("El archivo \"" + fn + "\" no cumple con el formato IDX. Ver http://yann.lecun.com/exdb/mnist/" )
#define		IMAGES_LABELS_INCONSISTENCY(igs,lbs)	("Los archivos \"" + igs + "\" y \"" + lbs + "\" no contienen la misma cantidad de elementos" )
#define		FILE_NOT_FOUND(filename)				("El archivo \"" + filename + "\" no existe o se encuentra inutilizable")

#define		IMAGE_HEIGHT_PXS	28
#define		IMAGE_WIDTH_PXS		28

#define		BYTE_2_INT(buff)		 	((int)(0xFF & ((unsigned char)*(buff))))
#define 	BYTE_ARRAY_2_INT(buff) 		((BYTE_2_INT(buff) << 24) + (BYTE_2_INT(buff+1) << 16) + (BYTE_2_INT(buff+2) << 8)  + (BYTE_2_INT(buff+3) << 0))

#define  	LIMIT	400

void load_ubyte_images(string filename, MMatrix& images)
{

	ifstream file (filename.c_str(), ios::in | ios::binary);
	if(!file.is_open())
		DISPLAY_ERROR_AND_EXIT(FILE_NOT_FOUND(filename));

	char buffer[4];

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int magic_number = BYTE_ARRAY_2_INT(buffer);
	
	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_images = LIMIT; //BYTE_ARRAY_2_INT(buffer);
	
	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_rows = BYTE_ARRAY_2_INT(buffer);
	
	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_cols = BYTE_ARRAY_2_INT(buffer);
	
	if( number_of_rows != IMAGE_HEIGHT_PXS || number_of_cols != IMAGE_WIDTH_PXS )
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));

	int number_of_non_empty_cols = 0;
	bool col_is_empty[number_of_cols];
	MMatrix images_full(number_of_images, number_of_rows * number_of_cols);
	for (int im = 0; im < number_of_images; ++im)
		for (int j = 0; j < number_of_cols; ++j)
		{
			bool empty_col = true;
			for (int i = 0; i < number_of_rows; ++i)
			{
				file.read(buffer, 1);
				if(!file)
					DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));

				int n = i * number_of_cols + j;
				int pixel_value = BYTE_2_INT(buffer);
				if(pixel_value != 0) empty_col = false;

				images_full(im, n) = ((double)pixel_value);
			}
			col_is_empty[j] = empty_col;
			if(!empty_col) ++number_of_non_empty_cols;
		}

	file.close();

	images.set_size(number_of_images, number_of_rows * number_of_non_empty_cols);
	int images_index = 0;
	for (int j = 0; j < number_of_cols; ++j)
	{
		if(col_is_empty[j]) continue;

		for (int i = 0; i < number_of_rows; ++i)
			images(i,images_index) = images_full(i,j);
		++images_index;
	}

}

void load_ubyte_labels(string filename, vector<int>& labels)
{
	ifstream file (filename.c_str(), ios::in | ios::binary);
	if(!file.is_open())
		DISPLAY_ERROR_AND_EXIT(FILE_NOT_FOUND(filename));
	
	char buffer[4];

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int magic_number = BYTE_ARRAY_2_INT(buffer);

	file.read(buffer, 4);
	if(!file)
		DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));
	int number_of_items = LIMIT;//BYTE_ARRAY_2_INT(buffer);

	for (int i = 0; i < number_of_items; ++i)
	{
		file.read(buffer, 1);
		if(!file)
			DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));

		labels.push_back(BYTE_2_INT(buffer));
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
