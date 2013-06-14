#ifndef __DATA_IO_H__
#define __DATA_IO_H__

#include <string>
#include <vector>
#include <fstream>

#include "mmatrix.h"

//	//	//

void load_ubyte_images(string filename, MMatrix& images);

void load_mnist_data(string images_filename, string labels_filename, MMatrix& images, vector<int>& labels);

void write_data_file(double delta, MMatrix& V, MMatrix& avgs);

void load_data_file(string filename, double& delta, MMatrix& V, MMatrix& avgs);

void write_results(ofstream& output_file, double delta, int k, int hits);

#endif //__DATA_IO_H__