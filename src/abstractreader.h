#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
namespace DMY
{

typedef struct
    {
        int totalNode;
        int totalBranch;
        double baseMVA;
        double EPS;
        int modelType;
        double centerParameter;
        int maxIte;//最大迭代次数
    } basicInfoStruct;
    typedef struct
    {
        int i;
        double volt;
    } swingNodeStruct;
    typedef struct//线路结构
    {
        int i;//线路左端编号
        int j;//线路右端编号
        double x;
        double r;
        double bi_2;//左端电容
        double bj_2;//右端电容
    } lineStruct;
    typedef struct//变压器结构
    {
        int i;//
        int j;//
        bool adjustable;//是否可调
        double r;
        double x;
        double kl;//变比下限
        double ku;//变比上限
        double k0;//当前变比
        int tap;//档位数
    } transStruct;
    typedef struct
    {
        int i;
        bool adjustable;//是否可调
        double unit_g;//单位电导
        double unit_b;//单位电纳
        int tap;//组数
        int kl;//调节下限
        int ku;//调节上限
        int k0;//当前档位
    } capacitorStruct; //电容器
    typedef struct
    {
        int i;
        double g;
        double b;
    } grandBranchStruct;
    typedef struct
    {
        int i;
        int zone;//区域号
        int voltLevel;//电压等级
        double PG;
        double QG;
        double PD;
        double QD;
    } nodePowerStruct;
    typedef struct//有功出力限制
    {
        bool adjustable;//是否可调
        int i;
        bool isSwing;//是否缓冲节点
        double pl;//有功功率下限
        double pu;//有功出力上限
        double cost0;//煤耗0次系数
        double cost1;
        double cost2;
        double initP;//有功初值
    } genPowerLimitStruct;
    typedef struct//有功出力限制
    {
        bool adjustable;//是否可调
        int i;
        bool isSwing;//是否缓冲节点
        double ql;//无功功率下限
        double qu;//无功出力上限
        double fixVolt;//电压定值
    } genReactivepowerLimitStruct;

class AbstractReader
{
public:
    /** Default constructor */
    AbstractReader();
    /** Default destructor */
    virtual ~AbstractReader();
    virtual bool Read(const char *filePath)=0;
    boost::shared_ptr<std::list<swingNodeStruct> > GetSwingNodeData();
    boost::shared_ptr<std::list<lineStruct> > GetLineData();
    boost::shared_ptr<std::list<transStruct> > GetTransData();
    boost::shared_ptr<std::list<capacitorStruct> > GetCapacitorData();
    boost::shared_ptr<std::list<grandBranchStruct> > GetgrandBranchData();
    boost::shared_ptr<std::list<nodePowerStruct> > GetNodePowerData();
    boost::shared_ptr<std::list<genPowerLimitStruct> > GetGenPowerLimitData();
    boost::shared_ptr<std::list<genReactivepowerLimitStruct> > GetGenReactivepowerLimitData();

protected:
    basicInfoStruct mBasicInfo;
    boost::shared_ptr<std::list<swingNodeStruct> >  mSwingNodeData;

    boost::shared_ptr<std::list<lineStruct> > mLineData;
    boost::shared_ptr<std::list<transStruct> > mTransData;
    boost::shared_ptr<std::list<capacitorStruct> > mCapacitorData;
    boost::shared_ptr<std::list<grandBranchStruct> > mgrandBranchData;
    boost::shared_ptr<std::list<nodePowerStruct> > mNodePowerData;
    boost::shared_ptr<std::list<genPowerLimitStruct> > mGenPowerLimitData;
    boost::shared_ptr<std::list<genReactivepowerLimitStruct> > mGenReactivepowerLimitData;
private:



};

}
#endif // ABSTRACTREADER_H
