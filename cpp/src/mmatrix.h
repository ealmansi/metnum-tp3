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
	void multiply_in_place(const MMatrix& rhs);

	MMatrix t() const;		// transposition

private:
	void initialize();
	void clean_up();

	int _rows;
	int _cols;
	double* _data;
};

ostream& operator<<(ostream &os, const MMatrix &mat);

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