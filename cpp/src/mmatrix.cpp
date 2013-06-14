#include "../lib/commons.h"

#include "mmatrix.h"

#define 	INVALID_MATRIX_SIZE(rows,cols)			("Tamaño de matriz inválido; filas: " + int2str(rows) + ", columnas: " + int2str(cols))
#define 	OUT_OF_BOUNDS(i,j)						("Índices fuera de rango; i: " + int2str(i) + ", j: " + int2str(j))
#define 	OUT_OF_BOUNDS_LINEAR(n)					("Índice lineal fuera de rango; i: " + int2str(n))
#define		DIMENSIONS_MISMATCH(r1,r2,c1,c2)		("Las dimensiones no concuerdan; el lado izquierdo es de (" + int2str(r1) + ", " + int2str(c1) + "), y el lado derecho de (" + int2str(r2) + ", " + int2str(c2) + ")")

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
	copy_values(*this, mat);
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

void MMatrix::copy_values(MMatrix& lvalue, const MMatrix& rvalue)
{
	if(lvalue.rows() != rvalue.rows() || lvalue.cols() != rvalue.cols())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH(lvalue.rows(), rvalue.rows(), lvalue.cols(), rvalue.cols()));

	MMATRIX_MAP_IJ(lvalue, rvalue(i,j));
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

double& MMatrix::operator()(int n)
{
	if(NOT_IN_RANGE(0, n, _rows * _cols))
		DISPLAY_ERROR_AND_EXIT(OUT_OF_BOUNDS_LINEAR(n));

	return _data[n];
}

double MMatrix::operator()(int n) const
{
	return operator()(n);
}

double& MMatrix::operator()(int i, int j)
{
	if(NOT_IN_RANGE(0,i,_rows) || NOT_IN_RANGE(0,j,_cols))
		DISPLAY_ERROR_AND_EXIT(OUT_OF_BOUNDS(i,j));

	return _data[i*_cols+j];
}

double MMatrix::operator()(int i, int j) const
{
	return operator()(i,j);
}

MMatrix MMatrix::operator-(const MMatrix& rvalue) const
{
	if(_rows != rvalue.rows() || _cols != rvalue.cols())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH(_rows, rvalue.rows(), _cols, rvalue.cols()));

	MMatrix res(_rows, _cols);
	MMATRIX_MAP_IJ(res, operator()(i,j) - rvalue(i,j));

	return res;
}

MMatrix& MMatrix::operator/=(double rhs)
{
	MMATRIX_MAP_IJ(*this, operator()(i,j) / rhs);

	return *this;
}