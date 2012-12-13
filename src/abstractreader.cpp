#include "abstractreader.h"

namespace DMY
{

AbstractReader::AbstractReader():
    mSwingNodeData(new std::list<swingNodeStruct>),
    mLineData(new std::list<lineStruct>),
    mTransData(new std::list<transStruct>  ),
    mCapacitorData(new std::list<capacitorStruct> ),
    mgrandBranchData(new std::list<grandBranchStruct> ),
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

boost::shared_ptr<std::list<swingNodeStruct> > AbstractReader::GetSwingNodeData()
{
    return this->mSwingNodeData;
}
boost::shared_ptr<std::list<lineStruct> > AbstractReader::GetLineData()
{
    return this->mLineData;
}
boost::shared_ptr<std::list<transStruct> > AbstractReader::GetTransData()
{
    return this->mTransData;
}
boost::shared_ptr<std::list<capacitorStruct> > AbstractReader::GetCapacitorData()
{
    return this->mCapacitorData;
}
boost::shared_ptr<std::list<grandBranchStruct> > AbstractReader::GetgrandBranchData()
{
    return this->mgrandBranchData;
}
boost::shared_ptr<std::list<nodePowerStruct> > AbstractReader::GetNodePowerData()
{
    return this->mNodePowerData;
}
boost::shared_ptr<std::list<genPowerLimitStruct> > AbstractReader::GetGenPowerLimitData()
{
    return this->mGenPowerLimitData;
}
boost::shared_ptr<std::list<genReactivepowerLimitStruct> > AbstractReader::GetGenReactivepowerLimitData()
{
    return this->mGenReactivepowerLimitData;
}


}
