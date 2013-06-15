#ifndef __MMATRIX_H__
#define __MMATRIX_H__

class MMatrix
{
public:
	MMatrix();
	MMatrix(int rows, int cols);
	MMatrix(int rows, int cols, double value);
	MMatrix(const MMatrix& mat);
	~MMatrix();
	void set_size(int rows, int cols);
	void copy_values(const MMatrix& rvalue);

	int rows() const;
	int cols() const;

	MMatrix row(int r) const;
	MMatrix col(int c) const;

	MMatrix& operator=(const MMatrix& rvalue);

	double& operator()(int n);
	double operator()(int n) const;
	double& operator()(int i, int j);
	double operator()(int i, int j) const;

	MMatrix operator-(const MMatrix& rhs) const;
	MMatrix& operator/=(double rhs);
	MMatrix operator*(const MMatrix& rhs) const;
	MMatrix& operator*=(const MMatrix& rhs);

	MMatrix t() const;					// transposition
	MMatrix& t_in_place();				// (only squared matrices)

	MMatrix& make_identity_matrix(int size);

	static double dot_row_col(const MMatrix &lhs, int i, const MMatrix &rhs, int j);
	static double dot_col_col(const MMatrix &lhs, int j1, const MMatrix &rhs, int j2);

private:
	void initialize();
	void clean_up();

	int _rows;
	int _cols;
	double* _data;
};

ostream& operator<<(ostream &os, const MMatrix &mat);

/* No dimension check */
inline double MMatrix::dot_row_col(const MMatrix &lhs, int i, const MMatrix &rhs, int j)
{
	double res = 0;
	for (int k = 0; k < lhs.cols(); ++k)
		res += lhs(i,k) * rhs(k,j);

	return res;
}

/* No dimension check */
inline double MMatrix::dot_col_col(const MMatrix &lhs, int j1, const MMatrix &rhs, int j2)
{
	double res = 0;
	for (int k = 0; k < lhs.rows(); ++k)
		res += lhs(k,j1) * rhs(k,j2);

	return res;
}

#define		MMATRIX_WALK_IJ(mat, code)						\
{															\
	int _mmwalkij_rs = (mat).rows();						\
	int _mmwalkij_cs = (mat).cols();						\
	for (int i = 0; i < _mmwalkij_rs; ++i)					\
		for (int j = 0; j < _mmwalkij_cs; ++j)				\
			{code;}											\
}

#define		MMATRIX_MAP_IJ(mat, expr)						\
	MMATRIX_WALK_IJ((mat), (mat)(i,j) = (expr);)

#endif	// __MMATRIX_H__