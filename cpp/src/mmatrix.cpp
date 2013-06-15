#include <cmath>
#include <iomanip>
#include <iostream>
using namespace std;

#include "../lib/commons.h"

#include "mmatrix.h"

#define 	INVALID_MATRIX_SIZE(rows,cols)			\
				("Tamaño de matriz inválido; filas: " + int2str(rows) + ", columnas: " + int2str(cols))
#define 	OUT_OF_BOUNDS(i,j)						\
				("Índices fuera de rango; i: " + int2str(i) + ", j: " + int2str(j))
#define 	OUT_OF_BOUNDS_LINEAR(n)					\
				("Índice lineal fuera de rango; i: " + int2str(n))
#define		DIMENSIONS_MISMATCH(r1,r2,c1,c2)		\
				("Las dimensiones no concuerdan; el lado izquierdo es de (" + int2str(r1) + ", " + int2str(c1) + "), y el lado derecho de (" + int2str(r2) + ", " + int2str(c2) + ")")
#define		DIMENSIONS_MISMATCH_MULT_INPLACE(r1,r2,c1,c2)	\
				("Las dimensiones no concuerdan para la multiplicación in situ (el producto debe estar definido y la matriz derecha debe ser cuadrada); el lado izquierdo es de (" + int2str(r1) + ", " + int2str(c1) + "), y el lado derecho de (" + int2str(r2) + ", " + int2str(c2) + ")")
#define		NEAR_ZERO_DIVISION(denom)				\
				("Divisón por un valor cercano a cero: " + double2str(denom))
#define		DIMENSIONS_MISMATCH_TRANSP_INPLACE(rows,cols)	\
				("No se puede trasponer in situ porque la matriz no es cuadrada; filas: " + int2str(rows) + ", columnas: " + int2str(cols))

#define		IN_RANGE(a,x,b)				(((a) <= (x)) && ((x) < (b)))
#define		NOT_IN_RANGE(a,x,b)			(!IN_RANGE((a),(x),(b)))

//	//	//	//

MMatrix::MMatrix()
{
	initialize();
}

MMatrix::MMatrix(int rows, int cols)
{
	initialize();
	set_size(rows, cols);
}

MMatrix::MMatrix(int rows, int cols, double value)
{
	initialize();
	set_size(rows, cols);
	MMATRIX_MAP_IJ((*this), value);
}

MMatrix::MMatrix(const MMatrix& mat)
{
	initialize();
	set_size(mat.rows(), mat.cols());
	copy_values(mat);
}

MMatrix::~MMatrix()
{
	clean_up();
}

void MMatrix::initialize()
{
	_data = 0;
	_rows = 0;
	_cols = 0;
}

void MMatrix::clean_up()
{
	if(_data != 0)
		delete[] _data;
}

void MMatrix::set_size(int rows, int cols)
{
	if(rows <= 0 || cols <= 0)
		DISPLAY_ERROR_AND_EXIT(INVALID_MATRIX_SIZE(rows,cols));

	clean_up();
	_data = new double[rows*cols];
	_rows = rows;
	_cols = cols;
}

void MMatrix::copy_values(const MMatrix& rvalue)
{
	if(_rows != rvalue.rows() || _cols != rvalue.cols())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH(_rows, rvalue.rows(), _cols, rvalue.cols()));

	MMATRIX_MAP_IJ(*this, rvalue(i,j));
}

int MMatrix::rows() const
{
	return _rows;
}

int MMatrix::cols() const
{
	return _cols;
}

MMatrix MMatrix::row(int r) const
{
	MMatrix row(1, _cols);
	MMATRIX_MAP_IJ(row, operator()(r, j));

	return row;
}

MMatrix MMatrix::col(int c) const
{
	MMatrix col(_rows, 1);
	MMATRIX_MAP_IJ(col, operator()(i, c));

	return col;
}

MMatrix& MMatrix::operator=(const MMatrix& rvalue)
{
	if(_rows != rvalue.rows() || _cols != rvalue.cols())
		set_size(rvalue.rows(), rvalue.cols());

	copy_values(rvalue);

	return *this;
}

double& MMatrix::operator()(int n)
{
	if(NOT_IN_RANGE(0, n, _rows * _cols))
		DISPLAY_ERROR_AND_EXIT(OUT_OF_BOUNDS_LINEAR(n));

	return _data[n];
}

double MMatrix::operator()(int n) const
{
	if(NOT_IN_RANGE(0, n, _rows * _cols))
		DISPLAY_ERROR_AND_EXIT(OUT_OF_BOUNDS_LINEAR(n));

	return _data[n];
}

double& MMatrix::operator()(int i, int j)
{
	if(NOT_IN_RANGE(0,i,_rows) || NOT_IN_RANGE(0,j,_cols))
		DISPLAY_ERROR_AND_EXIT(OUT_OF_BOUNDS(i,j));

	return _data[i*_cols+j];
}

double MMatrix::operator()(int i, int j) const
{
	if(NOT_IN_RANGE(0,i,_rows) || NOT_IN_RANGE(0,j,_cols))
		DISPLAY_ERROR_AND_EXIT(OUT_OF_BOUNDS(i,j));
	
	return _data[i*_cols+j];
}

MMatrix MMatrix::operator-(const MMatrix& rhs) const
{
	if(_rows != rhs.rows() || _cols != rhs.cols())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH(_rows, rhs.rows(), _cols, rhs.cols()));

	MMatrix res(_rows, _cols);
	MMATRIX_MAP_IJ(res, operator()(i,j) - rhs(i,j));

	return res;
}

MMatrix& MMatrix::operator/=(double rhs)
{
	if( abs(rhs) < DBL_TOLERANCE_2_ZERO )
		DISPLAY_ERROR_AND_EXIT(NEAR_ZERO_DIVISION(rhs));

	MMATRIX_MAP_IJ(*this, operator()(i,j) / rhs);

	return *this;
}

MMatrix MMatrix::operator*(const MMatrix& rhs) const
{
	if(_cols != rhs.rows())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH(_rows, rhs.rows(), _cols, rhs.cols()));

	MMatrix res(_rows, rhs.cols());

	MMATRIX_MAP_IJ(res, dot_row_col(*this, i, rhs, j));

	return res;
}

MMatrix& MMatrix::operator*=(const MMatrix& rhs)
{
	if(_cols != rhs.rows() || rhs.rows() != rhs.cols())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH_MULT_INPLACE(_rows, rhs.rows(), _cols, rhs.cols()));

	MMatrix aux_row;
	MMATRIX_WALK_IJ(*this,{
		if(j == 0) aux_row = row(i);
		operator()(i,j) = dot_row_col(aux_row, 0, rhs, j);
	});

	return *this;
}

MMatrix MMatrix::t() const
{
	MMatrix res(_cols, _rows);
	MMATRIX_MAP_IJ(res, operator()(j,i));

	return res;
}

MMatrix& MMatrix::t_in_place()
{
	if(_rows != _cols)
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH_TRANSP_INPLACE(_rows,_cols));

	MMATRIX_WALK_IJ(*this,{
		if(i <= j) break;
		swap(operator()(i,j), operator()(j,i));
	});

	return *this;
}

MMatrix& MMatrix::make_identity_matrix(MMatrix& mat, int size)
{
	if(mat.rows() != size || mat.cols() != size)
		mat.set_size(size, size);

	MMATRIX_MAP_IJ(mat, (i == j)?(1):(0));

	return mat;
}

ostream& operator<<(ostream &os, const MMatrix &mat)
{
	os << "[";
	for (int i = 0; i < mat.rows(); ++i)
	{
		for (int j = 0; j < mat.cols(); ++j)
		{
			if( 0 < i && j == 0 )
				os << '\t';
			os << setw(20) << mat(i,j);
			if(j + 1 < mat.cols())
				os << " ";
		}
		if(i + 1 < mat.rows())
			os << endl;
	}
	os << "]";

	return os;
}
