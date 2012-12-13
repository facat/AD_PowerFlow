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
        int maxIte;//����������
    } basicInfoStruct;
    typedef struct
    {
        int i;
        double volt;
    } swingNodeStruct;
    typedef struct//��·�ṹ
    {
        int i;//��·��˱��
        int j;//��·�Ҷ˱��
        double x;
        double r;
        double bi_2;//��˵���
        double bj_2;//�Ҷ˵���
    } lineStruct;
    typedef struct//��ѹ���ṹ
    {
        int i;//
        int j;//
        bool adjustable;//�Ƿ�ɵ�
        double r;
        double x;
        double kl;//�������
        double ku;//�������
        double k0;//��ǰ���
        int tap;//��λ��
    } transStruct;
    typedef struct
    {
        int i;
        bool adjustable;//�Ƿ�ɵ�
        double unit_g;//��λ�絼
        double unit_b;//��λ����
        int tap;//����
        int kl;//��������
        int ku;//��������
        int k0;//��ǰ��λ
    } capacitorStruct; //������
    typedef struct
    {
        int i;
        double g;
        double b;
    } grandBranchStruct;
    typedef struct
    {
        int i;
        int zone;//�����
        int voltLevel;//��ѹ�ȼ�
        double PG;
        double QG;
        double PD;
        double QD;
    } nodePowerStruct;
    typedef struct//�й���������
    {
        bool adjustable;//�Ƿ�ɵ�
        int i;
        bool isSwing;//�Ƿ񻺳�ڵ�
        double pl;//�й���������
        double pu;//�й���������
        double cost0;//ú��0��ϵ��
        double cost1;
        double cost2;
        double initP;//�й���ֵ
    } genPowerLimitStruct;
    typedef struct//�й���������
    {
        bool adjustable;//�Ƿ�ɵ�
        int i;
        bool isSwing;//�Ƿ񻺳�ڵ�
        double ql;//�޹���������
        double qu;//�޹���������
        double fixVolt;//��ѹ��ֵ
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
