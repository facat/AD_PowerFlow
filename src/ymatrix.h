/*******************************************
形成节点导纳矩阵，用稀疏矩阵的形式。
********************************************/

#ifndef YMATRIX_H
#define YMATRIX_H
#include "ipsotextreader.h"
#include <Eigen/Sparse>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <list>
namespace DMY
{

struct Opatan {
EIGEN_EMPTY_STRUCT_CTOR(Opatan)
//typedef complex<Scalar> result_type;
double operator()(const double& a, const double& b) const { return std::atan(a/b); }
};


class YMatrix
{
public:
    /** Default constructor */
    explicit   YMatrix(const AbstractReader &reader);
    /** Default destructor */
    virtual ~YMatrix();
    bool MakeParameter();
    boost::shared_ptr<Eigen::SparseMatrix<double> > GetYMatrix() const;
    boost::shared_ptr<Eigen::SparseMatrix<double> > GetYAngleMatrix() const;
protected:
private:
    void MakeLineParameter(boost::shared_ptr<std::list<lineStruct> > lineData);
    void MakeGroundBranchParameter(boost::shared_ptr<std::list<groundBranchStruct> > groundBranchData);
    void MakeTransParameter(boost::shared_ptr<std::list<transStruct> > transData);
private:
    int mTotalNode;
    Eigen::SparseMatrix<double> mSpYMatrix;
    std::list<Eigen::Triplet<double> > mLineParaB;
    std::list<Eigen::Triplet<double> > mLineParaG;
//    std::list<Eigen::Triplet<double> > mBi_2;
//    std::list<Eigen::Triplet<double> > mBj_2;
    std::list<Eigen::Triplet<double> > mGroundBranchB;
    std::list<Eigen::Triplet<double> > mGroundBranchG;
    std::list<Eigen::Triplet<double> > mTransB;
    std::list<Eigen::Triplet<double> > mTransG;
    const AbstractReader &mReader;
    boost::shared_ptr<Eigen::SparseMatrix<double> > mSpMatY;
    boost::shared_ptr<Eigen::SparseMatrix<double> > mSpMatYAngle;

};

}
#endif // YMATRIX_H
