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
	foreach_a_ij(*this, a_ij = value);
}

MMatrix::MMatrix(const MMatrix& mat)
{
	initialize();
	set_size(mat.rows(), mat.cols());
	foreach_a_ij(*this, a_ij = mat(i,j));
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

int MMatrix::rows() const
{
	return _rows;
}

int MMatrix::cols() const
{
	return _cols;
}

int MMatrix::size() const
{
	return MAX(_rows,_cols);
}

MMatrix MMatrix::row(int r) const
{
	MMatrix row(1, _cols);
	foreach_v_i(row, v_i = operator()(r, i));

	return row;
}

void MMatrix::copy_row(int r, MMatrix& row) const
{
	if(row.rows() != 1 || row.cols() != _cols)
		row.set_size(1, _cols);

	foreach_v_i(row, v_i = operator()(r, i));
}

MMatrix MMatrix::col(int c) const
{
	MMatrix col(_rows, 1);
	foreach_v_i(col, v_i = operator()(i, c));

	return col;
}

void MMatrix::copy_col(int c, MMatrix& col) const
{
	if(col.rows() != _rows || col.cols() != 1)
		col.set_size(_rows, 1);

	foreach_v_i(col, v_i = operator()(i, c));
}

MMatrix& MMatrix::operator=(const MMatrix& rvalue)
{
	if(_rows != rvalue.rows() || _cols != rvalue.cols())
		set_size(rvalue.rows(), rvalue.cols());

	foreach_a_ij(*this, a_ij = rvalue(i,j));

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
	foreach_a_ij(res, a_ij = operator()(i,j) - rhs(i,j));

	return res;
}

MMatrix& MMatrix::operator*=(double rhs)
{
	foreach_a_ij(*this, a_ij = operator()(i,j) * rhs);

	return *this;
}

MMatrix& MMatrix::operator/=(double rhs)
{
	if( abs(rhs) < DBL_TOLERANCE_2_ZERO )
		DISPLAY_ERROR_AND_EXIT(NEAR_ZERO_DIVISION(rhs));

	foreach_a_ij(*this, a_ij = operator()(i,j) / rhs);

	return *this;
}

MMatrix MMatrix::operator*(const MMatrix& rhs) const
{
	if(_cols != rhs.rows())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH(_rows, rhs.rows(), _cols, rhs.cols()));

	MMatrix res(_rows, rhs.cols());
	foreach_a_ij(res, a_ij = dot_row_col(*this, i, rhs, j));

	return res;
}

MMatrix& MMatrix::operator*=(const MMatrix& rhs)
{
	if(_cols != rhs.rows() || rhs.rows() != rhs.cols())
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH_MULT_INPLACE(_rows, rhs.rows(), _cols, rhs.cols()));

	MMatrix aux_row;
	foreach_a_ij(*this,{
		if(j == 0) copy_row(i, aux_row);
		a_ij = dot_row_col(aux_row, 0, rhs, j);
	});

	return *this;
}

MMatrix MMatrix::t() const
{
	MMatrix res(_cols, _rows);
	foreach_a_ij(res, a_ij = operator()(j,i));

	return res;
}

MMatrix& MMatrix::t_in_place()
{
	if(_rows != _cols)
		DISPLAY_ERROR_AND_EXIT(DIMENSIONS_MISMATCH_TRANSP_INPLACE(_rows,_cols));

	foreach_a_ij_lower_triangular(*this,{
		swap(a_ij, operator()(j,i));
	});

	return *this;
}

MMatrix MMatrix::identity_matrix(int size)
{
	MMatrix res(size, size);
	foreach_a_ij(res, a_ij = ((i == j)?(1):(0)));

	return res;
}

MMatrix& MMatrix::make_identity_matrix(int size)
{
	if(_rows != size || _cols != size)
		set_size(size, size);

	foreach_a_ij(*this, a_ij = ((i == j)?(1):(0)));

	return *this;
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
