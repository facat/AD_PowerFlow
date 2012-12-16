#include "powerflow.h"

#include <vector>
#include <pfjacobian.h>
#include <ipsotextreader.h>
#include <boost/shared_ptr.hpp>
#include <solve.h>
namespace DMY
{

PowerFlow::PowerFlow()
{
    //ctor
}

bool PowerFlow::Run(const char *filePath)
{
    iPsoTextReader reader;
    int K;
    if(reader.Read(filePath))
    {
        YMatrix yMatrix(reader);
        if(yMatrix.MakeParameter())
        {
            int totalNum=reader.GetTotalNodeNum();
            PFJacobian pfJacobian(reader,yMatrix);
            std::vector<double> voltAngle;
            std::vector<double> volt;
            boost::shared_array<double> unbalance;
            double *b=NULL;
            boost::shared_ptr<std::list<swingNodeStruct> > swingNode=reader.GetSwingNodeData();
            boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRepowLimit(reader.GetGenReactivepowerLimitData());
            this->Initialization(totalNum,voltAngle,volt,reader.GetGenReactivepowerLimitData());
            //开始进入循环
            for(K=0; K<reader.GetMaxIteration(); ++K)
            {
                pfJacobian.Make(voltAngle,volt);
                std::cout<<"得到不平衡量"<<std::endl;
                //得到不平衡量
                unbalance=pfJacobian.GetUnbalance();
                b=unbalance.get();

                //准备解出修正量
                std::cout<<"准备解出修正量"<<std::endl;
                for(int i=0; i<reader.GetTotalNodeNum()*2; ++i)
                {
                    b[i]=-b[i];
                }
                //对平衡节点进行处理
                std::cout<<"对平衡节点进行处理"<<std::endl;
                for(std::list<swingNodeStruct>::iterator ite=swingNode->begin();
                        ite!=swingNode->end();
                        ++ite)
                {
                    swingNodeStruct value=*ite;
                    b[value.i]=0;
                }
                //对pv节点进行处理
                std::cout<<"对pv节点进行处理"<<std::endl;
                for(std::list<genReactivepowerLimitStruct>::iterator ite=genRepowLimit->begin();
                        ite!=genRepowLimit->end();
                        ++ite)
                {
                    genReactivepowerLimitStruct value=*ite;
                    b[value.i+totalNum]=0;
                }
                //检查终止条件
                std::cout<<"检查终止条件"<<std::endl;
                if(this->AbsMax(b,totalNum*2)<reader.GetEPS())
                {
                    break;
                }
                sparseMatSruct spMat;
                spMat=pfJacobian.GetJacoBian();
                Solve::solveT(spMat.dim,spMat.Ap.get(),spMat.Ai.get(),spMat.Ax.get(),b);
                std::cout<<"开始计算迭代数"<<std::endl;
                std::cout<<"正在进行第"<<K<<"次迭代"<<std::endl;
                this->Update(voltAngle,volt,b);

            }

        }
        else
        {
            std::cout<<"线路参数错误!"<<std::endl;
        }
        std::cout<<"迭代"<<K<<"次"<<std::endl;
        return true;

    }
    else
    {
        return false;
    }
}

void PowerFlow::Initialization(int totalNum,std::vector<double> &VoltAagle,std::vector<double>& Volt,boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRePowLimit)//初始化电压和相角;
{
    VoltAagle.resize(totalNum);
    Volt.resize(totalNum);
    for(int i=0; i<totalNum; ++i)
    {
        Volt[i]=1;
        VoltAagle[i]=0;
    }
    //置平衡节点电压
    //boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRepowLimit(reader.GetGenReactivepowerLimitData());
    for(std::list<genReactivepowerLimitStruct>::iterator ite=genRePowLimit->begin();
            ite!=genRePowLimit->end();
            ite++)
    {
        genReactivepowerLimitStruct value;
        value=*ite;
        Volt[value.i]=value.fixVolt;
    }
}

void PowerFlow::Update(std::vector<double> &VoltAngle,std::vector<double>& Volt,double *b)//更新变量
{
    int totalNum=(int)Volt.size();
    for(int i=0; i<totalNum; ++i)
    {
        VoltAngle[i]+=b[i];
        Volt[i]+=b[i+totalNum];
    }
}

double PowerFlow::AbsMax(double *b,int dim)//找出最大不平衡量
{
    double maxValue=0;
    double fabsB;
    for(int i=0; i<dim; ++i)
    {
        fabsB=std::fabs(b[i]);
        if(fabsB>maxValue)
        {
            maxValue=fabsB;
        }
    }
    return maxValue;
}

}
