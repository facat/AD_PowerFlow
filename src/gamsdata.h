#ifndef GAMSData_H
#define GAMSData_H
#include <sstream>
#include <fstream>
//#include "SparseMat.h"
//#include "D_Complex.h"
#include <iostream>
#include <list>
#include <boost/algorithm/string.hpp>
typedef unsigned int UINT ;

class GAMSData
{
public:
    GAMSData(const char * fileName);
    bool Update(const char * fileName);
    ~GAMSData();
private:
	void FormSparseYMatrix();//得到一个稀疏的Y矩阵，也几时节点导纳矩阵。
private:
    double *nodeQG;
    double *nodePG;
    double *nodePD;
    double *nodeQD;
    SparseMat<D_Complex<double> > *p_Y_Matrix;
    int mNodeNum;
	double *mSparseYMatrix;
};

#endif // GAMSData_H
