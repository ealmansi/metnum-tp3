#ifndef __COMMON_MACROS_H__
#define __COMMON_MACROS_H__

#include <iostream>
#include <cstdlib>

#define		PRINT_MSG(msg)								\
	{ std::cout << (msg) << std::endl; }				\

#define		PRINT_EXPR(expr)												\
	{ std::cout << #expr << " = " << (expr) << ";" << std::endl; }			\

#define		DISPLAY_ERROR_AND_EXIT(msg)																		\
	{																										\
		std::cerr << "Error en la operación \"" << __FUNCTION__ << "\" | " << (msg) << "." << std::endl;	\
		std::exit(-1);																						\
	}

#endif	// __COMMON_MACROS_H__