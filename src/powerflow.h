#ifndef POWERFLOW_H
#define POWERFLOW_H
#include <vector>
#include <boost/shared_ptr.hpp>
#include <list>
#include <commonstructure.h>
namespace DMY{

class PowerFlow
{
    public:
        /** Default constructor */
        PowerFlow();
    bool Run(const char *filePath);
    protected:
    private:
        void Initialization(int totalNum,std::vector<double> &VoltAagle,std::vector<double>& Volt,boost::shared_ptr<std::list<genReactivepowerLimitStruct> > genRePowLimit);//初始化电压和相角;
        void Update(std::vector<double> &VoltAngle,std::vector<double>& Volt,double *b);//更新变量
        double AbsMax(double *b,int dim);
};


}

#endif // POWERFLOW_H
