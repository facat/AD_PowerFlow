#ifndef COMMONSTRUCTURE_H_INCLUDED
#define COMMONSTRUCTURE_H_INCLUDED
#include <boost/shared_array.hpp>
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
} groundBranchStruct;
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

typedef struct//�й���������
{
    int dim;
    boost::shared_array<int > Ap;
    boost::shared_array<int > Ai;
    boost::shared_array<double > Ax;
    //boost::shared_array<double > b;
}sparseMatSruct;

#endif // COMMONSTRUCTURE_H_INCLUDED
