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
            //��ʼ����ѭ��
            for(K=0; K<reader.GetMaxIteration(); ++K)
            {
                pfJacobian.Make(voltAngle,volt);
                std::cout<<"�õ���ƽ����"<<std::endl;
                //�õ���ƽ����
                unbalance=pfJacobian.GetUnbalance();
                b=unbalance.get();

                //׼�����������
                std::cout<<"׼�����������"<<std::endl;
                for(int i=0; i<reader.GetTotalNodeNum()*2; ++i)
                {
                    b[i]=-b[i];
                }
                //��ƽ��ڵ���д���
                std::cout<<"��ƽ��ڵ���д���"<<std::endl;
                for(std::list<swingNodeStruct>::iterator ite=swingNode->begin();
                        ite!=swingNode->end();
                        ++ite)
                {
                    swingNodeStruct value=*ite;
                    b[value.i]=0;
                }
                //��pv�ڵ���д���
                std::cout<<"��pv�ڵ���д���"<<std::endl;
                for(std::list<genReactivepowerLimitStruct>::iterator ite=genRepowLimit->begin();
                        ite!=genRepowLimit->end();
                        ++ite)
                {
                    genReactivepowerLimitStruct value=*ite;
                    b[value.i+totalNum]=0;
                }
                //�����ֹ����
                std::cout<<"�����ֹ����"<<std::endl;
                if(this->AbsMax(b,totalNum*2)<reader.GetEPS())
                {
                    break;
                }
                sparseMatSruct spMat;
                spMat=pfJacobian.GetJacoBian();
                Solve::solveT(spMat.dim,spMat.Ap.get(),spMat.Ai.get(),spMat.Ax.get(),b);
                std::cout<<"��ʼ���������"<<std::endl;
                std::cout<<"���ڽ��е�"<<K<<"�ε���"<<std::endl;
                this->Update(voltAngle,volt,b);

            }

        }
        else
        {
            std::cout<<"��·��������!"<<std::endl;
        }
        std::cout<<"����"<<K<<"��"<<std::endl;
        return true;

    }
    else
    {
        return false;
    }
}

void PowerFlow::Initialization(int totalNum,std::vector<double> &VoltAagle,std::vector<double>& Volt,boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRePowLimit)//��ʼ����ѹ�����;
{
    VoltAagle.resize(totalNum);
    Volt.resize(totalNum);
    for(int i=0; i<totalNum; ++i)
    {
        Volt[i]=1;
        VoltAagle[i]=0;
    }
    //��ƽ��ڵ��ѹ
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

void PowerFlow::Update(std::vector<double> &VoltAngle,std::vector<double>& Volt,double *b)//���±���
{
    int totalNum=(int)Volt.size();
    for(int i=0; i<totalNum; ++i)
    {
        VoltAngle[i]+=b[i];
        Volt[i]+=b[i+totalNum];
    }
}

double PowerFlow::AbsMax(double *b,int dim)//�ҳ����ƽ����
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
