#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "data-io.h"
#include "mmatrix.h"


MMatrix& load_ubyte_images(string filename)
{

}

vector<int>& load_ubyte_labels(string filename)
{

}

void load_mnist_data(string images_filename, string labels_filename, MMatrix& images, vector<int>& labels)
{
 	images = load_ubyte_images(images_filename);
	labels = load_ubyte_labels(labels_filename);
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