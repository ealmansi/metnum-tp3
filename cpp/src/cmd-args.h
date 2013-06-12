#ifndef __CMD_ARGS_H__
#define __CMD_ARGS_H__

#include <string>
#include <vector>

struct CmdArgsGen
{
	string images_filename;
	string labels_filename;
	vector<double> deltas;
};

CmdArgsGen parse_cmd_args_gen(int argc, char const *argv[]);


#endif	//__CMD_ARGS_H__