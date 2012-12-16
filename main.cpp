/************************************************************
用ADOL-C求解最优潮流.
*************************************************************/
#include <iostream>
#include <ipsotextreader.h>
#include <ymatrix.h>
#include <string>
#include <PFjacobian.h>
#include <vector>
#include <commonstructure.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <powerflow.h>
using namespace std;

//int main()
//{
//    int totalNum=4;
//    double *x=new double[4];
//    for(int i=0;i<totalNum;i++)
//    {
//        x[i]=1.5;
//    }
//    boost::shared_array<adouble> xVolt(new adouble [totalNum]);
//    boost::shared_array<double> dummy(new double [totalNum]);
//    trace_on(10);
//    for(int i=0; i<totalNum; i++)
//    {
//        xVolt[i]<<=x[i];
//        //xVoltAngle[i]<<=x[i+totalNum];
//    }
//
//    /********************************************/
//
//    adouble *_P=new adouble[totalNum];
//    for(int i=0; i<totalNum; i++)
//    {
//        _P[i]=pow(xVolt[i],2);
//        _P[i]>>=dummy[i];
//        std::cout<<dummy[i]<<std::endl;
//    }
//
//    trace_off(1);
//
////double *x=new double[totalNum*2];
//    double ** jacoMat=new double *[totalNum];
//    double *y=new double[totalNum];
//    for(int i=0; i<totalNum; i++)
//    {
//
//        jacoMat[i]=new double[totalNum];
//    }
//
//    jacobian(10,totalNum,totalNum,x,jacoMat);
//
//    std::cout<<"fun"<<std::endl;
//    function(10,4,4,x,y);
//    for(int i=0; i<totalNum; i++)
//    {
//        std::cout<<y[i]<<std::endl;
//    }
//
//
//    delete[] x;
//    for(int i=0; i<totalNum; i++)
//    {
//        for(int j=0; j<totalNum; j++)
//        {
//            std::cout<<jacoMat[i][j]<<"\t";
//        }
//        std::cout<<std::endl;
//        delete[] jacoMat[i];
//    }
//    delete[] jacoMat;
//    delete[] y;
//    delete[] x;
//
//    return 0;
//}

int main()
{
	cout << "Hello world!" << endl;
    DMY::PowerFlow pf;
    pf.Run("e:/opf/ieee14.dat");


	return 0;
}
