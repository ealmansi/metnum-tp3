#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
using namespace std;

#include "../lib/tclap/CmdLine.h"
using namespace TCLAP;

#include "cmd-args.h"

vector<double> parseDeltaValues(string deltaValuesStr, int deltaExp);
vector<int> parseKValues(string kValuesStr);

CmdArgsGen parse_cmd_args_gen(int argc, char** argv)
{
	// Define the command line object.
	CmdLine cmd("Programa generador de los datos requeridos para la clasificación.", ' ', "0.1");

	// Define a switch and add it to the command line.
	SwitchArg verboseSwitch("v","verbose","Imprimir mensajes indicando el estado del programa durante la ejecución.", false);
	cmd.add( verboseSwitch );

	// Define a value argument and add it to the command line.
	ValueArg<int> deltaExpArg("e","deltaexp","Orden de magnitud de los valores del parámetro de la condición de parada para el algoritmo QR (ver argumento \"deltas\").",true,-1,"int");
	cmd.add( deltaExpArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> deltaValuesArg("d","deltas","Expresión indicando los valores del parámetro de la condición de parada para el algoritmo QR. Debe ser de la forma \"d1:d2:d3...:dn\", sin comillas, con valores positivos; se utilizará como valor de delta:\n\td_i * 10^(-deltaexp) ",true,"error","lista_double");
	cmd.add( deltaValuesArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> labelsFilenameArg("l","etiquetas","Archivo de etiquetas de entrenamiento de la base de datos MNIST (formato ubyte); debe ser el correspondiente al archivo de imágenes.",true,"error","archivo");
	cmd.add( labelsFilenameArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> imagesFilenameArg("i","imagenes","Archivo de imágenes de entrenamiento de la base de datos MNIST (formato ubyte).",true,"error","archivo");
	cmd.add( imagesFilenameArg );

	// Parse the args.
	cmd.parse( argc, argv );

	// Get the value parsed by each arg.
	CmdArgsGen args; 
	args.images_filename = imagesFilenameArg.getValue();
	args.labels_filename = labelsFilenameArg.getValue();
	args.delta_values = parseDeltaValues(deltaValuesArg.getValue(), deltaExpArg.getValue());
	if( args.delta_values.empty() )
	{
		CmdLineParseException e("Formato incorrecto o resultado vacío.", "deltas");
		cmd.getOutput()->failure(cmd,e); exit(1);
	}
	args.verbose = verboseSwitch.getValue();
	
	return args;
}

CmdArgsClassif parse_cmd_args_classif(int argc, char** argv)
{
	// Define the command line object.
	CmdLine cmd("Programa clasificador.", ' ', "0.1");

	// Define a switch and add it to the command line.
	SwitchArg verboseSwitch("v","verbose","Imprimir mensajes indicando el estado del programa durante la ejecución.", false);
	cmd.add( verboseSwitch );

	// Define a value argument and add it to the command line.
	UnlabeledMultiArg<string> dataFilesArg("datos", "Archivos de datos generados con el programa tp3-gen, conteniendo la información necesaria para realizar la clasificación utilizando distintas precisiones en el cálculo de autovectores.", true, "archivo");
	cmd.add( dataFilesArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> kValuesArg("k","valores_k","Expresión indicando distintos valores para la cantidad de componentes principales que se utilizan para realizar la clasificación. Debe ser de la forma \"k1:k2:k3...:kn\", sin comillas, con números naturales.",true,"error","lista_nat");
	cmd.add( kValuesArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> outputFilenameArg("o","resultados","Archivo donde se guardarán los resultados.",true,"error","archivo");
	cmd.add( outputFilenameArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> labelsFilenameArg("l","etiquetas","Archivo de etiquetas de test de la base de datos MNIST (formato ubyte); debe ser el correspondiente al archivo de imágenes.",true,"error","archivo");
	cmd.add( labelsFilenameArg );

	// Define a value argument and add it to the command line.
	ValueArg<string> imagesFilenameArg("i","imagenes","Archivo de imágenes de test de la base de datos MNIST (formato ubyte).",true,"error","archivo");
	cmd.add( imagesFilenameArg );

	// Parse the args.
	cmd.parse( argc, argv );

	// Get the value parsed by each arg.
	CmdArgsClassif args; 
	args.images_filename = imagesFilenameArg.getValue();
	args.labels_filename = labelsFilenameArg.getValue();
	args.output_filename = outputFilenameArg.getValue();
	args.k_values = parseKValues(kValuesArg.getValue());
	if( args.k_values.empty() )
	{
		CmdLineParseException e("Formato incorrecto o resultado vacío.", "valores_k");
		cmd.getOutput()->failure(cmd,e); exit(1);
	}
	args.data_files = dataFilesArg.getValue();
	args.verbose = verboseSwitch.getValue();
	
	return args;
}

double str2double(string& str)
{
	stringstream ss(str);
	double value;
	ss >> value;
	if(!ss || value < 0)
		value = -1;
	return value;
}

vector<double> parseDeltaValues(string deltaValuesStr, int deltaExp)
{
	vector<double> deltaValues;

	stringstream ss(deltaValuesStr);
    string item;
    while (getline(ss, item, ':'))
    {	
    	double value = str2double(item);
    	if(value == -1)
    	{
    		deltaValues.clear();
    		break;
    	}
        deltaValues.push_back(value*pow(10,-deltaExp));
    }

    return deltaValues;
}

int str2int(string& str)
{
	stringstream ss(str);
	int value;
	ss >> value;
	if(!ss)
		value = -1;
	return value;
}

vector<int> parseKValues(string kValuesStr)
{
	vector<int> kValues;

	stringstream ss(kValuesStr);
    string item;
    while (getline(ss, item, ':'))
    {	
    	int value = str2int(item);
    	if(value <= 0)
    	{
    		kValues.clear();
    		break;
    	}
        kValues.push_back(value);
    }

    return kValues;
}