#ifndef PFJACOBIAN_H
#define PFJACOBIAN_H
/*******************************************************************************
获得潮流方程的雅克比矩阵
*******************************************************************************/


#include <Eigen/Sparse>
#include <boost/shared_ptr.hpp>
#include <YMatrix.h>
#include <AbstractReader.h>
#include <commonstructure.h>
#include <list>
#include <vector>
#include <adolc/adolc.h>
#include <math.h>
#include <iostream>
#include <adolc_addtional_support.h>
#include <adolc/adolc_sparse.h>
//#include <boost/functional/hash.hpp>
#include <map>
namespace DMY{
class PFJacobian
{
    public:
        /** Default constructor */
    explicit    PFJacobian(const AbstractReader &reader,const YMatrix &yMatrix);
    void Make(const std::vector<double> &VoltAngle,const std::vector<double> &Volt);
    void Fun(double *x,double *y);
    void Unbalance(double *x);
    boost::shared_array<double> GetUnbalance() const;
    sparseMatSruct GetJacoBian() const;
    private:
        void MakeTrace(double *x);
        void Modify(double **jacoMat);//为了PV和平衡节点进行修改。
        void Modify(int nnz,unsigned int *rind,unsigned int *cind,double *values);//为了PV和平衡节点进行修改。
    protected:
    private:
        const YMatrix &mYMatrix;
        const AbstractReader &mReader;
        boost::shared_array<double> mUnbanlance;
        sparseMatSruct mSparseMatSruct;
};

}
#endif // PFJACOBIAN_H
