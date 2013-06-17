#ifndef __DATA_IO_H__
#define __DATA_IO_H__

#include <string>
#include <vector>
#include <fstream>

#include "mmatrix.h"

//	//	//

void load_ubyte_images(string filename, MMatrix& images);

void load_mnist_data(string images_filename, string labels_filename, MMatrix& images, vector<int>& labels);

string write_covariance_matrix_to_file(string images_filename, MMatrix& cov_mat);

void load_covariance_matrix(string filename, MMatrix& cov_mat);

string write_data_file(double delta, MMatrix& V, MMatrix& avgs);

void load_data_file(string filename, double& delta, MMatrix& V, MMatrix& avgs);

void open_output_file(string filename, ofstream& file);

void write_results(ofstream& file, double delta, int k, string images_filename, int total_images, int hits);

void close_output_file(ofstream& file);

#endif //__DATA_IO_H__