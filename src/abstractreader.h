/********************************************************
这是一个基类，用于指定读入数据的接口。
*********************************************************/

#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H
#include <list>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include "commonstructure.h"
namespace DMY
{


class AbstractReader
{
public:
    /** Default constructor */
    AbstractReader();
    /** Default destructor */
    virtual ~AbstractReader();
    virtual bool Read(const char *filePath)=0;
    boost::shared_ptr<std::list<swingNodeStruct> > GetSwingNodeData() const;
    boost::shared_ptr<std::list<lineStruct> > GetLineData() const;
    boost::shared_ptr<std::list<transStruct> > GetTransData() const;
    boost::shared_ptr<std::list<capacitorStruct> > GetCapacitorData() const;
    boost::shared_ptr<std::list<groundBranchStruct> > GetgroundBranchData() const;
    boost::shared_ptr<std::list<nodePowerStruct> > GetNodePowerData() const;
    boost::shared_ptr<std::list<genPowerLimitStruct> > GetGenPowerLimitData() const;
    boost::shared_ptr<std::list<genReactivepowerLimitStruct> > GetGenReactivepowerLimitData() const;
    int GetTotalNodeNum() const;
    double GetEPS() const;
    int GetMaxIteration() const;
    double GetCenterParameter() const;
    double GetBaseMVA() const;
    int GetModelType() const;

protected:
    basicInfoStruct mBasicInfo;
    boost::shared_ptr<std::list<swingNodeStruct> >  mSwingNodeData;
    boost::shared_ptr<std::list<lineStruct> > mLineData;
    boost::shared_ptr<std::list<transStruct> > mTransData;
    boost::shared_ptr<std::list<capacitorStruct> > mCapacitorData;
    boost::shared_ptr<std::list<groundBranchStruct> > mgroundBranchData;
    boost::shared_ptr<std::list<nodePowerStruct> > mNodePowerData;
    boost::shared_ptr<std::list<genPowerLimitStruct> > mGenPowerLimitData;
    boost::shared_ptr<std::list<genReactivepowerLimitStruct> > mGenReactivepowerLimitData;
private:



};

}
#endif // ABSTRACTREADER_H
