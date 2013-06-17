#ifndef __COMMON_MACROS_H__
#define __COMMON_MACROS_H__

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#define		PRINT_MSG(msg)								\
	{ std::cout << (msg) << std::endl; }				\

#define		PRINT_EXPR(expr)												\
	{ std::cout << std::setprecision(19) << #expr << " = " << (expr) << ";" << std::endl; }			\

#define		PRINT_NAMED_EXPR(name, expr)												\
	{ std::cout << std::setprecision(19) << (name) << " = " << (expr) << ";" << std::endl; }			\

#define		DISPLAY_ERROR_AND_EXIT(msg)																		\
	{																										\
		std::cerr << "Error en la operación \"" << __FUNCTION__ << "\" | " << (msg) << "." << std::endl;	\
		std::exit(-1);																						\
	}

#define		PRINT_ON_VERBOSE(msg, verbose)		\
	{ if( (verbose) ) PRINT_MSG(string("% ").append((msg))); }

#define		DBL_TOLERANCE_2_ZERO		1e-8

#define		BEGIN_TIMER()		clock_t _start_t_macro_var_ = clock();
#define		MSECS_ELAPSED()		((int)((clock() - _start_t_macro_var_)* 1000 / CLOCKS_PER_SEC))

#define		MIN(a,b)		(((a) < (b))?(a):(b))
#define		MAX(a,b)		(((a) > (b))?(a):(b))

inline std::string int2str(int n)
{
	std::stringstream ss;
	ss << n;
	return ss.str();
}

inline std::string double2str(double f)
{
	std::stringstream ss;
	ss << f;
	return ss.str();
}

inline double swap(double& a, double& b)
{
	double temp = a;
	a = b;
	b = temp;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
	os << "[";
	typename std::vector<T>::const_iterator i;
	for (i = vec.begin(); i != vec.end(); ++i)
		os << std::setw(20) << *i << " ";
	os << "]";

	return os;
}

#endif	// __COMMON_MACROS_H__
