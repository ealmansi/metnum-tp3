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
#define		FILE_NOT_CREATED(fn)					("No se pudo crear el archivo: " + fn)

#define		IMAGE_HEIGHT_PXS	28
#define		IMAGE_WIDTH_PXS		28

#define		BYTE_2_INT(buff)		 	((int)(0xFF & ((unsigned char)*(buff))))
#define 	BYTE_ARRAY_2_INT(buff) 		((BYTE_2_INT(buff) << 24) + (BYTE_2_INT(buff+1) << 16) + (BYTE_2_INT(buff+2) << 8)  + (BYTE_2_INT(buff+3) << 0))

#define  	LIMIT	50

#ifdef _WIN64
	#define		DIRECTORY_SEPARATOR		('\\')
#elif _WIN32
	#define		DIRECTORY_SEPARATOR		('\\')
#elif __linux
	#define		DIRECTORY_SEPARATOR		('/')
#endif

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

	images.set_size(number_of_images, number_of_rows * number_of_cols);
	for (int im = 0; im < number_of_images; ++im)
		for (int j = 0; j < number_of_cols; ++j)
			for (int i = 0; i < number_of_rows; ++i)
			{
				file.read(buffer, 1);
				if(!file)
					DISPLAY_ERROR_AND_EXIT(INVALID_FILE_FORMAT(filename));

				int n = i * number_of_cols + j;
				images(im, n) = ((double)BYTE_2_INT(buffer));
			}

	file.close();
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

#define		BYTE_ARRAY_DOUBLE_LEN		(sizeof(double))
#define		BYTE_ARRAY_INT_LEN		(sizeof(int))

typedef union {
    char bytes[BYTE_ARRAY_INT_LEN];
    int value;
} ByteArrayIntConverter;

typedef union {
    char bytes[BYTE_ARRAY_DOUBLE_LEN];
    double value;
} ByteArrayDoubleConverter;

void write_data_file(double delta, MMatrix& V, MMatrix& avgs)
{
	string filename = "tp3_data_delta_" + double2str(delta) + ".mdat";
	ofstream file (filename.c_str(), ios::out | ios::binary);

	ByteArrayIntConverter int_converter;
	ByteArrayDoubleConverter double_converter;

    double_converter.value = delta;
    file.write(double_converter.bytes, BYTE_ARRAY_DOUBLE_LEN);

    int_converter.value = V.rows();
    file.write(int_converter.bytes, BYTE_ARRAY_INT_LEN);

    int_converter.value = V.cols();
    file.write(int_converter.bytes, BYTE_ARRAY_INT_LEN);

    for (int i = 0; i < V.rows(); ++i)
    {
    	for (int j = 0; j < V.cols(); ++j)
    	{
    		double_converter.value = V(i,j);
    		file.write(double_converter.bytes, BYTE_ARRAY_DOUBLE_LEN);
    	}
    }

    int_converter.value = avgs.rows();
    file.write(int_converter.bytes, BYTE_ARRAY_INT_LEN);

    int_converter.value = avgs.cols();
    file.write(int_converter.bytes, BYTE_ARRAY_INT_LEN);

    for (int i = 0; i < avgs.rows(); ++i)
    {
    	for (int j = 0; j < avgs.cols(); ++j)
    	{
    		double_converter.value = avgs(i,j);
    		file.write(double_converter.bytes, BYTE_ARRAY_DOUBLE_LEN);
    	}
    }

	file.close();
}

void load_data_file(string filename, double& delta, MMatrix& V, MMatrix& avgs)
{
	ifstream file (filename.c_str(), ios::in | ios::binary);

	ByteArrayIntConverter int_converter;
	ByteArrayDoubleConverter double_converter;

    file.read(double_converter.bytes, BYTE_ARRAY_DOUBLE_LEN);
    delta = double_converter.value;

    file.read(int_converter.bytes, BYTE_ARRAY_INT_LEN);
    int rows = int_converter.value;

    file.read(int_converter.bytes, BYTE_ARRAY_INT_LEN);
    int cols = int_converter.value;

    V.set_size(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
    	for (int j = 0; j < cols; ++j)
    	{
    		file.read(double_converter.bytes, BYTE_ARRAY_DOUBLE_LEN);
    		V(i,j) = double_converter.value;
    	}
    }

    file.read(int_converter.bytes, BYTE_ARRAY_INT_LEN);
    rows = int_converter.value;

    file.read(int_converter.bytes, BYTE_ARRAY_INT_LEN);
    cols = int_converter.value;

    avgs.set_size(rows, cols);
    for (int i = 0; i < rows; ++i)
    {
    	for (int j = 0; j < cols; ++j)
    	{
    		file.read(double_converter.bytes, BYTE_ARRAY_DOUBLE_LEN);
    		avgs(i,j) = double_converter.value;
    	}
    }

	file.close();
}

string get_file_basename( string const& path )
{
    string filename = path.substr( path.find_last_of( DIRECTORY_SEPARATOR ) + 1 );
    string basename = filename.substr( 0, filename.find_last_of( '.' ) );

    return basename;
}

void open_output_file(string filename, ofstream& file)
{
	file.open(filename.c_str());
	if(!file.is_open())
		DISPLAY_ERROR_AND_EXIT(FILE_NOT_CREATED(filename));

	file << "% delta, k, archivo de imágenes clasificadas, cantidad de imágenes clasificadas, cantidad de aciertos" << endl;
	file << get_file_basename(filename) << " = { " << endl;
}

void write_results(ofstream& file, double delta, int k, string images_filename, int total_images, int hits)
{
	file << "\t{";
	file << delta << ", ";
	file << k << ", ";
	file << "'" << images_filename << "', ";
	file << total_images << ", ";
	file << hits;
	file << "}," << endl;
}

void close_output_file(ofstream& file)
{
	file << "};";
	file.close();
}