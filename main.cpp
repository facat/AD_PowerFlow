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
	DMY::iPsoTextReader reader;
	if(reader.Read("E:/OPF/ieee4.dat"))
	{
		DMY::YMatrix yMatrix(reader);
		if(yMatrix.MakeParameter())
		{
//		    std::cout<<"Y"<<std::endl;
//			std::cout<<*(yMatrix.GetYMatrix())<<std::endl;
//			std::cout<<"YAngle"<<std::endl;
//			std::cout<<*(yMatrix.GetYAngleMatrix())<<std::endl;
			DMY::PFJacobian pfJacobian(reader,yMatrix);
			std::vector<double> volt;
			volt.resize(reader.GetTotalNodeNum());
			std::vector<double> voltAngle;
			voltAngle.resize(reader.GetTotalNodeNum());
			for(int i=0;i<reader.GetTotalNodeNum();++i)
            {
                volt[i]=1;
                voltAngle[i]=0;
            }
            //置平衡节点电压
            boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRepowLimit(reader.GetGenReactivepowerLimitData());
            for(std::list<genReactivepowerLimitStruct>::iterator ite=genRepowLimit->begin();
            ite!=genRepowLimit->end();
            ite++)
            {
                genReactivepowerLimitStruct value;
                value=*ite;
                volt[value.i]=value.fixVolt;
            }
			pfJacobian.Make(voltAngle,volt);

		}
		else
        {
            std::cout<<"线路参数错误!"<<std::endl;
        }

	}


	return 0;
}
