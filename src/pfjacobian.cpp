#include "pfjacobian.h"


namespace DMY
{


void MakeV2(double *x)
{
    int totalNum=4;

    boost::shared_array<adouble> xVolt(new adouble [totalNum]);
    //boost::shared_array<adouble> xVoltAngle(new adouble [totalNum]);
    //boost::shared_array<adouble> y(new adouble [totalNum]);
    boost::shared_array<double> dummy(new double [totalNum]);
    trace_on(10);
    for(int i=0; i<totalNum; i++)
    {
        xVolt[i]<<=pow(x[i],3);
        //xVoltAngle[i]<<=x[i+totalNum];
    }

    /********************************************/

    adouble *_P=new adouble[totalNum];
    for(int i=0; i<totalNum; i++)
    {
        _P[i]=xVolt[i];
        _P[i]>>=dummy[i];
        std::cout<<dummy[i]<<std::endl;
    }

    trace_off();

//double *x=new double[totalNum*2];
    double ** jacoMat=new double *[totalNum];
    double *y=new double[totalNum];
    for(int i=0; i<totalNum; i++)
    {

        jacoMat[i]=new double[totalNum];
    }

    jacobian(10,totalNum,totalNum,x,jacoMat);

    std::cout<<"fun"<<std::endl;
    function(10,4,4,x,y);
    for(int i=0; i<totalNum; i++)
    {
        std::cout<<y[i]<<std::endl;
    }


    delete[] x;
    for(int i=0; i<totalNum; i++)
    {
        for(int j=0; j<totalNum; j++)
        {
            std::cout<<jacoMat[i][j]<<"\t";
        }
        std::cout<<std::endl;
        delete[] jacoMat[i];
    }
    delete[] jacoMat;
    delete[] y;


    return;
}

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
    double ** jacoMat=new double *[totalNum*2];
    double *y=new double[totalNum*2];
    for(int i=0; i<totalNum; i++)
    {
        x[i]=VoltAngle.at(i);
        x[i+totalNum]=Volt.at(i);
    }

    for(int i=0; i<totalNum*2; i++)
    {

        jacoMat[i]=new double[totalNum*2];
    }
    //

    this->MakeTrace(x);
    //MakeV2(x);

    //return;



    jacobian(10,totalNum*2,totalNum*2,x,jacoMat);
    std::cout<<"fun"<<std::endl;
    this->Fun(x,y);
    for(int i=0; i<totalNum*2; i++)
    {
        std::cout<<y[i]<<std::endl;
    }


    delete[] x;
    for(int i=0; i<totalNum; i++)
    {
        for(int j=0; j<totalNum; j++)
        {
            std::cout<<jacoMat[i+totalNum][j+totalNum]<<"\t";
        }
        std::cout<<std::endl;
        delete[] jacoMat[i];
    }
    delete[] jacoMat;
    delete[] y;


}

void PFJacobian::MakeTrace(double *x)
{

//无功矩阵
    //PL+PD-PG=0
    int totalNum=this->mReader.GetTotalNodeNum();
    Eigen::SparseMatrix<adouble> spVolt(totalNum,1);
//    Eigen::SparseMatrix<adouble> spVoltAngle(totalNum,1);
    Eigen::SparseMatrix<adouble> spCosAngleIJ(totalNum,totalNum);
    Eigen::SparseMatrix<adouble> spSinAngleIJ(totalNum,totalNum);
    Eigen::SparseMatrix<adouble> spY(totalNum,totalNum);
    std::list<Eigen::Triplet<adouble> > voltList;//幅值
    std::list<Eigen::Triplet<adouble> > voltAngleList;//角度
    boost::shared_array<adouble> xVolt(new adouble [totalNum]);
    boost::shared_array<adouble> xVoltAngle(new adouble [totalNum]);
    //boost::shared_array<adouble> y(new adouble [totalNum]);
    boost::shared_array<double> dummy(new double [totalNum]);
    trace_on(10);


    for(int i=0; i<totalNum; i++)
    {
        //xVolt[i]<<=x[i];
        xVoltAngle[i]<<=x[i];
    }

    for(int i=0; i<totalNum; i++)
    {
        xVolt[i]<<=x[i+totalNum];
        //xVoltAngle[i]<<=x[i+totalNum];
    }
//    /********************************************/
//
//    adouble *_P=new adouble[totalNum];
//    for(int i=0; i<totalNum; i++)
//    {
//        _P[i]=xVolt[i];
//        _P[i]>>=dummy[i];
//        std::cout<<dummy[i]<<std::endl;
//    }
//
//    trace_off();
//    return;
//
//    /**********************************************/


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

//    std::cout<<"spAngleIJ"<<std::endl;
//    std::cout<<spAngleIJ<<std::endl;

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
        //std::cout<<dummy[i]<<std::endl;
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
        //std::cout<<dummy[i]<<std::endl;
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
            jacoMat[value.i][j]=0;
            jacoMat[j][value.i]=0;
        }
        jacoMat[value.i][value.i]=1;
    }

}


}
