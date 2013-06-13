#ifndef __MMATRIX_H__
#define __MMATRIX_H__

class MMatrix
{
public:
	MMatrix();

	int rows() const;
	double& operator()(int i, int j);
};

#endif	// __MMATRIX_H__