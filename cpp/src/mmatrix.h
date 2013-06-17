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
	int size() const;

	MMatrix row(int r) const;
	void copy_row(int r, MMatrix& row) const;
	MMatrix col(int c) const;
	void copy_col(int c, MMatrix& col) const;

	MMatrix& operator=(const MMatrix& rvalue);

	double& operator()(int n);
	double operator()(int n) const;
	double& operator()(int i, int j);
	double operator()(int i, int j) const;
	double& e(int i, int j);
	double e(int i, int j) const;

	MMatrix operator-(const MMatrix& rhs) const;
	MMatrix& operator*=(double rhs);
	MMatrix& operator/=(double rhs);
	MMatrix operator*(const MMatrix& rhs) const;
	MMatrix& operator*=(const MMatrix& rhs);

	MMatrix t() const;					// transposition
	MMatrix& t_in_place();				// (only squared matrices)

	static MMatrix identity_matrix(int size);
	MMatrix& make_identity_matrix(int size);

	static double dot_row_col(const MMatrix &lhs, int i, const MMatrix &rhs, int j);
	static double dot_col_col(const MMatrix &lhs, int j1, const MMatrix &rhs, int j2);
	static double dot(const MMatrix &lhs, const MMatrix &rhs);

private:
	void initialize();
	void clean_up();

	int _rows;
	int _cols;
	double* _data;
};

ostream& operator<<(ostream &os, const MMatrix &mat);

/* No dimension check */
inline double& MMatrix::e(int i, int j)
{
	return _data[i*_cols+j];
}

/* No dimension check */
inline double MMatrix::e(int i, int j) const
{
	return _data[i*_cols+j];
}

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

/* No dimension check, only vectors */
inline double MMatrix::dot(const MMatrix &lhs, const MMatrix &rhs)
{
	double res = 0;
	for (int k = 0; k < lhs.size(); ++k)
		res += lhs(k) * rhs(k);

	return res;
}

#define		foreach_a_ij(mat, code)									\
{																	\
	int _foreach_a_ij_rs = (mat).rows();							\
	int _foreach_a_ij_cs = (mat).cols();							\
	for (int i = 0; i < _foreach_a_ij_rs; ++i)						\
		for (int j = 0; j < _foreach_a_ij_cs; ++j)					\
			{														\
				double& a_ij = (mat).operator()(i,j);				\
				{code;}												\
			}														\
}

#define		foreach_a_ij_lower_triangular(mat, code)				\
{																	\
	int _foreach_a_ij_rs = (mat).rows();							\
	int _foreach_a_ij_cs = (mat).cols();							\
	for (int i = 0; i < _foreach_a_ij_rs; ++i)						\
		for (int j = 0; j <= i; ++j)								\
			{														\
				double& a_ij = (mat).operator()(i,j);				\
				{code;}												\
			}														\
}

#define		foreach_v_i(vec, code)									\
{																	\
	int _foreach_v_i_size = (vec).size();							\
	for (int i = 0; i < _foreach_v_i_size; ++i)						\
		{															\
			double& v_i = (vec).operator()(i);						\
			{code;}													\
		}															\
}

#endif	// __MMATRIX_H__