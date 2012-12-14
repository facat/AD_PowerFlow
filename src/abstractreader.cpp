/********************************************************
这是一个基类，用于指定读入数据的接口。
*********************************************************/

#include "abstractreader.h"

namespace DMY
{

AbstractReader::AbstractReader():
    mSwingNodeData(new std::list<swingNodeStruct>),
    mLineData(new std::list<lineStruct>),
    mTransData(new std::list<transStruct>  ),
    mCapacitorData(new std::list<capacitorStruct> ),
    mgroundBranchData(new std::list<groundBranchStruct> ),
    mNodePowerData(new std::list<nodePowerStruct>),
    mGenPowerLimitData(new std::list<genPowerLimitStruct>),
    mGenReactivepowerLimitData(new std::list<genReactivepowerLimitStruct>)
{
    //ctor
}

AbstractReader::~AbstractReader()
{
    //dtor
}

boost::shared_ptr<std::list<swingNodeStruct> > AbstractReader::GetSwingNodeData() const
{
    return this->mSwingNodeData;
}
boost::shared_ptr<std::list<lineStruct> > AbstractReader::GetLineData() const
{
    return this->mLineData;
}
boost::shared_ptr<std::list<transStruct> > AbstractReader::GetTransData() const
{
    return this->mTransData;
}
boost::shared_ptr<std::list<capacitorStruct> > AbstractReader::GetCapacitorData() const
{
    return this->mCapacitorData;
}
boost::shared_ptr<std::list<groundBranchStruct> > AbstractReader::GetgroundBranchData() const
{
    return this->mgroundBranchData;
}
boost::shared_ptr<std::list<nodePowerStruct> > AbstractReader::GetNodePowerData() const
{
    return this->mNodePowerData;
}
boost::shared_ptr<std::list<genPowerLimitStruct> > AbstractReader::GetGenPowerLimitData() const
{
    return this->mGenPowerLimitData;
}
boost::shared_ptr<std::list<genReactivepowerLimitStruct> > AbstractReader::GetGenReactivepowerLimitData() const
{
    return this->mGenReactivepowerLimitData;
}

int AbstractReader::GetTotalNodeNum() const
{
    return this->mBasicInfo.totalNode;
}
double AbstractReader::GetEPS() const
{
    return this->mBasicInfo.EPS;
}
int AbstractReader::GetMaxIteration() const
{
    return this->mBasicInfo.maxIte;
}
double AbstractReader::GetCenterParameter() const
{
    return this->mBasicInfo.centerParameter;
}
double AbstractReader::GetBaseMVA() const
{
    return this->mBasicInfo.baseMVA;
}
int AbstractReader::GetModelType() const
{
    return this->mBasicInfo.modelType;
}

}
