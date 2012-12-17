#include "pfjacobian.h"


namespace DMY
{

PFJacobian::PFJacobian(const AbstractReader &reader,const YMatrix &yMatrix):
    mYMatrix(yMatrix),
    mReader(reader)
{
    //ctor
}


void PFJacobian::Make(const std::vector<double> &VoltAngle,const std::vector<double> &Volt)
{


    int totalNum=this->mReader.GetTotalNodeNum();
    double *x=new double[totalNum*2];
    //double ** jacoMat=new double *[totalNum*2];
    double *y=new double[totalNum*2];
    for(int i=0; i<totalNum; i++)
    {
        x[i]=VoltAngle.at(i);
        x[i+totalNum]=Volt.at(i);
    }

//    for(int i=0; i<totalNum*2; i++)
//    {
//
//        jacoMat[i]=new double[totalNum*2];
//    }

    this->MakeTrace(x);
    this->Unbalance(x);
    int option[4];
    option[0]=0;
    option[1]=0;
    option[2]=0;
    option[3]=1;
    //jacobian(10,totalNum*2,totalNum*2,x,jacoMat);
    int nnz=0;
    unsigned int *rind=NULL;
    unsigned int *cind=NULL;
    double *values=NULL;
    sparse_jac(10,totalNum*2,totalNum*2,0,x,&nnz,&rind,&cind,&values,option);
    boost::shared_array<unsigned int> PtrRind(new unsigned int[nnz]);
    boost::shared_array<unsigned int> PtrCind(new unsigned int[nnz]);
    boost::shared_array<double> PtrValue(new double[nnz]);
    for(int i=0; i<nnz; ++i)
    {
        PtrCind[i]=cind[i];
        PtrRind[i]=rind[i];
        PtrValue[i]=values[i];
    }
    free(rind);
    free(cind);
    free(values);
//    this->Modify(jacoMat);
    this->Modify(nnz,PtrRind.get(),PtrCind.get(),PtrValue.get());

    //用Eigen存储一下，然后等下变成稀疏的形式
    std::list<Eigen::Triplet<double> > jacoTripletList;
    for(int i=0; i<nnz; ++i)
    {

        jacoTripletList.push_back(
            Eigen::Triplet<double>(
                PtrRind[i],
                PtrCind[i],
                PtrValue[i]
            )

        );

    }

    Eigen::SparseMatrix<double> spJacoMat(totalNum*2,totalNum*2);
    spJacoMat.setFromTriplets(jacoTripletList.begin(),jacoTripletList.end());
    spJacoMat.makeCompressed();

//    std::cout<<"spJacoMat.makeCompressed()"<<std::endl;
//    std::cout<<spJacoMat<<std::endl;
    sparseMatSruct sparseMatT;
    sparseMatT.Ai.reset(new int[spJacoMat.nonZeros()]);
    sparseMatT.Ap.reset(new int[spJacoMat.outerSize()+1]);
    sparseMatT.Ax.reset(new double[spJacoMat.nonZeros()]);
    for(int i=0; i<spJacoMat.nonZeros(); ++i) //注意！！ 其实传过去的是sparseMat的转置。
    {
        sparseMatT.Ai[i]=spJacoMat.innerIndexPtr()[i];
        sparseMatT.Ax[i]=spJacoMat.valuePtr()[i];
    }
    for(int i=0; i<spJacoMat.outerSize()+1; ++i) //注意！！ 其实传过去的是sparseMat的转置。
    {
        sparseMatT.Ap[i]=spJacoMat.outerIndexPtr()[i];
    }
    sparseMatT.dim=totalNum*2;
    this->mSparseMatSruct=sparseMatT;
    delete[] x;
    delete[] y;
//    for(int i=0; i<totalNum; i++)
//    {
////        for(int j=0; j<totalNum; j++)
////        {
////            std::cout<<jacoMat[i+totalNum][j+totalNum]<<"\t";
////        }
////        std::cout<<std::endl;
//        delete[] jacoMat[i];
//    }
//    delete[] jacoMat;



}

void PFJacobian::MakeTrace(double *x)
{

//无功矩阵
    //PL+PD-PG=0
    int totalNum=this->mReader.GetTotalNodeNum();
    Eigen::SparseMatrix<adouble> spVolt(totalNum,1);
    Eigen::SparseMatrix<adouble> spCosAngleIJ(totalNum,totalNum);
    Eigen::SparseMatrix<adouble> spSinAngleIJ(totalNum,totalNum);
    Eigen::SparseMatrix<adouble> spY(totalNum,totalNum);
    std::list<Eigen::Triplet<adouble> > voltList;//幅值
    std::list<Eigen::Triplet<adouble> > voltAngleList;//角度
    boost::shared_array<adouble> xVolt(new adouble [totalNum]);
    boost::shared_array<adouble> xVoltAngle(new adouble [totalNum]);
    boost::shared_array<double> dummy(new double [totalNum]);
    trace_on(10);


    for(int i=0; i<totalNum; i++)
    {

        xVoltAngle[i]<<=x[i];
    }

    for(int i=0; i<totalNum; i++)
    {
        xVolt[i]<<=x[i+totalNum];
    }
    for(int i=0; i<totalNum; i++)
    {
        voltList.push_back(
            Eigen::Triplet<adouble>(i,
                                    0,
                                    xVolt[i])
        );    //置电压
    }
    //得到电压
    spVolt.setFromTriplets(voltList.begin(),voltList.end());
    //得到AngleIJ
    boost::shared_ptr<Eigen::SparseMatrix<double> > YAngle=this->mYMatrix.GetYAngleMatrix();
    boost::shared_ptr<Eigen::SparseMatrix<double> > Y=this->mYMatrix.GetYMatrix();
    std::list<Eigen::Triplet<adouble> > TripCosAngleIJ;

    for(int i=0; i<YAngle->outerSize(); i++)
    {
        for(Eigen::SparseMatrix<double>::InnerIterator it(*YAngle,i); it; ++it)
        {
            TripCosAngleIJ.push_back(
                Eigen::Triplet<adouble>(
                    it.row(),
                    it.col(),
                    cos(xVoltAngle[it.row()]-xVoltAngle[it.col()]-it.value())
                )
            );


        }
    }
    spCosAngleIJ.setFromTriplets(TripCosAngleIJ.begin(),TripCosAngleIJ.end());
    std::list<Eigen::Triplet<adouble> > TripY;
    for(int i=0; i<Y->outerSize(); i++)
    {
        for(Eigen::SparseMatrix<double>::InnerIterator it(*Y,i); it; ++it)
        {
            TripY.push_back(
                Eigen::Triplet<adouble>(
                    it.row(),
                    it.col(),
                    it.value()
                )
            );
        }
    }
    spY.setFromTriplets(TripY.begin(),TripY.end());
    Eigen::SparseMatrix<adouble> YxCos(totalNum,totalNum);
    YxCos=spY.cwiseProduct(spCosAngleIJ);
    Eigen::SparseMatrix<adouble> P(totalNum,1);
    P=spVolt.cwiseProduct(YxCos*spVolt);
    adouble *_P=new adouble[totalNum];
    for(int i=0; i<P.outerSize(); i++)
    {
        for(Eigen::SparseMatrix<adouble>::InnerIterator it(P,i); it; ++it)
        {
            _P[it.row()]=it.value();
        }
    }
    for(int i=0; i<totalNum; i++)
    {
        _P[i]>>=dummy[i];
    }
    //开始形成Q方程
    std::list<Eigen::Triplet<adouble> > TripSinAngleIJ;

    for(int i=0; i<YAngle->outerSize(); i++)
    {
        for(Eigen::SparseMatrix<double>::InnerIterator it(*YAngle,i); it; ++it)
        {
            TripSinAngleIJ.push_back(
                Eigen::Triplet<adouble>(
                    it.row(),
                    it.col(),
                    sin(xVoltAngle[it.row()]-xVoltAngle[it.col()]-it.value())
                )
            );


        }
    }
    spSinAngleIJ.setFromTriplets(TripSinAngleIJ.begin(),TripSinAngleIJ.end());
    Eigen::SparseMatrix<adouble> YxSin(totalNum,totalNum);
    YxSin=spY.cwiseProduct(spSinAngleIJ);
    Eigen::SparseMatrix<adouble> Q(totalNum,1);
    Q=spVolt.cwiseProduct(YxSin*spVolt);
    adouble *_Q=new adouble[totalNum];
    for(int i=0; i<Q.outerSize(); i++)
    {
        for(Eigen::SparseMatrix<adouble>::InnerIterator it(Q,i); it; ++it)
        {
            _Q[it.row()]=it.value();
        }
    }
    for(int i=0; i<totalNum; i++)
    {
        _Q[i]>>=dummy[i];
    }

    trace_off();
    delete[] _P;
    delete[] _Q;
}


void PFJacobian::Fun(double *x,double *y)
{
    int totalNode=this->mReader.GetTotalNodeNum();
    function(10,totalNode*2,totalNode*2,x,y);
}

void PFJacobian::Modify(double **jacoMat)//为了PV和平衡节点进行修改。
{
    int totalNum=this->mReader.GetTotalNodeNum();
    //平衡节点
    boost::shared_ptr<std::list<swingNodeStruct> > swingNode=this->mReader.GetSwingNodeData();
    for(std::list<swingNodeStruct>::iterator ite=swingNode->begin();
            ite!=swingNode->end();
            ite++
       )
    {
        swingNodeStruct value;
        value=*ite;
        for(int j=0; j<totalNum*2; j++)
        {
            jacoMat[value.i][j]=0;
            jacoMat[j][value.i]=0;
        }
        jacoMat[value.i][value.i]=1;
    }
    //PV节点
    boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRepowLimit=this->mReader.GetGenReactivepowerLimitData();
    for(std::list<genReactivepowerLimitStruct>::iterator ite=genRepowLimit->begin();
            ite!=genRepowLimit->end();
            ite++
       )
    {
        genReactivepowerLimitStruct value;
        value=*ite;
        for(int j=0; j<totalNum*2; j++)
        {
            jacoMat[value.i+totalNum][j]=0;
            jacoMat[j][value.i+totalNum]=0;
        }
        jacoMat[value.i+totalNum][value.i+totalNum]=1;
    }

}

void PFJacobian::Modify(int nnz,unsigned int *rind,unsigned int *cind,double *values)
{
    int totalNum=this->mReader.GetTotalNodeNum();
    //平衡节点
    boost::unordered_map<int, bool> swingNodeList;
    boost::shared_ptr<std::list<swingNodeStruct> > swingNode=this->mReader.GetSwingNodeData();
    for(std::list<swingNodeStruct>::iterator ite=swingNode->begin();
            ite!=swingNode->end();
            ite++
       )
    {
        swingNodeStruct value=*ite;
        swingNodeList.insert(boost::unordered_map<int, bool> :: value_type(value.i, true));
    }
    //PV节点
    boost::unordered_map<int,bool> PVNodeList;
    boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRepowLimit=this->mReader.GetGenReactivepowerLimitData();
    for(std::list<genReactivepowerLimitStruct>::iterator ite=genRepowLimit->begin();
            ite!=genRepowLimit->end();
            ite++
       )
    {
        genReactivepowerLimitStruct value;
        value=*ite;
        PVNodeList.insert(boost::unordered_map<int, bool> :: value_type(value.i, true) );
    }
    for(int k=0; k<nnz; ++k)
    {
        int i=rind[k];
        int j=cind[k];
        //平衡节点
        if(swingNodeList.count(i)!=0 || swingNodeList.count(j)!=0)
        {
            values[k]=0;
        }
        if(swingNodeList.count(i)!=0 && i==j)
        {
            values[k]=1;
        }
        //PV节点
        if(PVNodeList.count(i-totalNum)!=0 || PVNodeList.count(j-totalNum)!=0 )
        {
            values[k]=0;
        }

        if(PVNodeList.count(i-totalNum)!=0 && i==j)
        {
            values[k]=1;
        }

    }
}

void PFJacobian::Unbalance(double *x)
{
    int totalNode=this->mReader.GetTotalNodeNum();
    double *y=new double[totalNode*2];
    function(10,totalNode*2,totalNode*2,x,y);
    this->mUnbanlance.reset(new double[totalNode*2]);
    for(int i=0; i<totalNode*2; i++)
    {
        this->mUnbanlance[i]=y[i];
    }

    boost::shared_ptr<std::list<nodePowerStruct> > nodePow=this->mReader.GetNodePowerData();
    for(std::list<nodePowerStruct>::iterator ite=nodePow->begin();
            ite!=nodePow->end();
            ++ite
       )
    {
        nodePowerStruct value;
        value=*ite;
        this->mUnbanlance[value.i]=value.PG-value.PD-this->mUnbanlance[value.i];
        this->mUnbanlance[value.i+totalNode]=value.QG-value.QD-this->mUnbanlance[value.i+totalNode];
    }
    delete[] y;
}

boost::shared_array<double> PFJacobian::GetUnbalance() const
{
    return this->mUnbanlance;
}

sparseMatSruct PFJacobian::GetJacoBian() const
{
    return this->mSparseMatSruct;
}

}
