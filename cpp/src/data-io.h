#ifndef __DATA_IO_H__
#define __DATA_IO_H__

#include <string>
#include <vector>

#include "mmatrix.h"

//	//	//

void load_data_gen(string images_filename, string labels_filename, MMatrix& images, vector<int>& labels);

void write_data_gen(double delta, MMatrix& V, MMatrix& avgs);

#endif //__DATA_IO_H__