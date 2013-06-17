#ifndef __CMD_ARGS_H__
#define __CMD_ARGS_H__

#include <string>
#include <vector>

//	//	//

struct CmdArgsGen
{
	bool compute_cov_mat;
	string images_filename;
	string cov_mat_filename;
	string labels_filename;
	int number_of_components;
	vector<double> delta_values;
	bool verbose;
};

CmdArgsGen parse_cmd_args_gen(int argc, char** argv);

struct CmdArgsClassif
{
	string images_filename;
	string labels_filename;
	string output_filename;
	vector<string> data_files;
	vector<int> k_values;
	bool verbose;
};

CmdArgsClassif parse_cmd_args_classif(int argc, char** argv);

#endif	//__CMD_ARGS_H__