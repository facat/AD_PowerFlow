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
namespace DMY{
class PFJacobian
{
    public:
        /** Default constructor */
    explicit    PFJacobian(const AbstractReader &reader,const YMatrix &yMatrix);
    void Make(const std::vector<double> &Volt,const std::vector<double> &VoltAngle);
    void Fun(double *x,double *y);
    private:
        void MakeTrace(double *x);
    protected:
    private:
        const YMatrix &mYMatrix;
        const AbstractReader &mReader;
};

}
#endif // PFJACOBIAN_H
