#ifndef __CMD_ARGS_H__
#define __CMD_ARGS_H__

#include <string>
#include <vector>

//	//	//

struct CmdArgsGen
{
	string images_filename;
	string labels_filename;
	vector<double> deltas;
};

CmdArgsGen parse_cmd_args_gen(int argc, char const *argv[]);

struct CmdArgsClassif
{
	string images_filename;
	string labels_filename;
	string output_filename;
	vector<string> data_files;
	vector<int> k_values;
};

CmdArgsClassif parse_cmd_args_classif(int argc, char const *argv[]);

#endif	//__CMD_ARGS_H__