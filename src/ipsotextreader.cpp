#include "ipsotextreader.h"
namespace DMY
{
iPsoTextReader::iPsoTextReader()
{
    //ctor
}

iPsoTextReader::~iPsoTextReader()
{
    //dtor
}

bool iPsoTextReader::Read(const char *filePath)
{
    std::string line;
    std::ifstream file(filePath);
    std::vector<std::string> sep;
    //typedef boost::numeric::converter<int,char *> Str2Int ;
    //blockType currentBlock;
    int currentBlock=0;
    if(!file.is_open())
    {
        std::cout<<"数据文件打开错误!"<<std::endl;
        return false;
    }
    if(!file.eof())
    {
        file>>line;
        boost::algorithm::trim(line);
        boost::algorithm::split(sep,line,boost::is_any_of(","));//用,进行分割
        mBasicInfo.totalNode=boost::lexical_cast<int>(sep.at(0));
        mBasicInfo.totalBranch=boost::lexical_cast<int>(sep.at(1));
        mBasicInfo.baseMVA=boost::lexical_cast<double>(sep.at(2));
        mBasicInfo.maxIte=boost::lexical_cast<double>(sep.at(3));
        mBasicInfo.centerParameter=boost::lexical_cast<double>(sep.at(4));
    }
    if(!file.eof())
    {
        file>>line;
        boost::algorithm::trim(line);
        boost::algorithm::split(sep,line,boost::is_any_of(","));//用,进行分割
        mBasicInfo.EPS=boost::lexical_cast<double>(sep.at(0));
        mBasicInfo.modelType=boost::lexical_cast<int>(sep.at(1));
    }
    while(!file.eof())
    {
        file>>line;
        boost::algorithm::trim(line);
        if(boost::algorithm::starts_with(line,"0"))
        {
            currentBlock++;
            continue;
        }
        boost::algorithm::split(sep,line,boost::is_any_of(","));//用,进行分割
        switch(currentBlock)
        {
        case 0:
        {
            swingNodeStruct swingNode;
            swingNode.volt=1;//暂时给1，后面会更新
            swingNode.i=boost::lexical_cast<int>(sep.at(1));
            this->mSwingNodeData->push_back(swingNode);
        }
        break;
        case 1:
        {
            lineStruct lineData;
            lineData.i=boost::lexical_cast<int>(sep.at(1));
            lineData.j=boost::lexical_cast<int>(sep.at(2));
            lineData.r=boost::lexical_cast<int>(sep.at(3));
            lineData.x=boost::lexical_cast<int>(sep.at(4));
            lineData.bi_2=boost::lexical_cast<int>(sep.at(5));
            lineData.bj_2=boost::lexical_cast<int>(sep.at(5));
            this->mLineData->push_back(lineData);
        }
        break;
        case 2:
        {
            groundBranchStruct groundBranch;
            groundBranch.i=boost::lexical_cast<int>(sep.at(0));
            groundBranch.b=boost::lexical_cast<int>(sep.at(1));
            groundBranch.g=0;
            this->mgroundBranchData->push_back(groundBranch);
        }
        break;
        case 3:
        {
            transStruct trans;
            trans.i=boost::lexical_cast<int>(sep.at(1));
            trans.j=boost::lexical_cast<int>(sep.at(2));
            trans.r=boost::lexical_cast<int>(sep.at(3));
            trans.x=boost::lexical_cast<int>(sep.at(4));
            trans.k0=boost::lexical_cast<int>(sep.at(5));
            trans.kl=boost::lexical_cast<int>(sep.at(6));
            trans.ku=boost::lexical_cast<int>(sep.at(7));
            trans.adjustable=-1;
            trans.tap=-1;
            this->mTransData->push_back(trans);
        }
        break;
        case 4:
        {
            nodePowerStruct nodePow;
            double baseMVA=this->mBasicInfo.baseMVA;
            nodePow.i=boost::lexical_cast<int>(sep.at(0));
            nodePow.PG=boost::lexical_cast<int>(sep.at(1))/baseMVA;//全部给标幺值
            nodePow.QG=boost::lexical_cast<int>(sep.at(2))/baseMVA;
            nodePow.PD=boost::lexical_cast<int>(sep.at(3))/baseMVA;
            nodePow.QD=boost::lexical_cast<int>(sep.at(4))/baseMVA;
            nodePow.voltLevel=-1;//-1表示没有
            nodePow.zone=-1;//-1表示没有
            this->mNodePowerData->push_back(nodePow);
        }
        break;
        case 5:
        {
            genReactivepowerLimitStruct genRepow;
            genRepow.i=boost::lexical_cast<int>(sep.at(0));
            genRepow.fixVolt=boost::lexical_cast<int>(sep.at(1));
            genRepow.ql=boost::lexical_cast<int>(sep.at(2));
            genRepow.qu=boost::lexical_cast<int>(sep.at(3));
            genRepow.adjustable=-1;
            genRepow.isSwing=-1;
            this->mGenReactivepowerLimitData->push_back(genRepow);
        }
        break;
        case 6:
        {
            genPowerLimitStruct genPow;
            double baseMVA=this->mBasicInfo.baseMVA;
            genPow.i=boost::lexical_cast<int>(sep.at(0));
            genPow.pl=boost::lexical_cast<int>(sep.at(1))/baseMVA;
            genPow.pu=boost::lexical_cast<int>(sep.at(2))/baseMVA;
            genPow.cost0=boost::lexical_cast<int>(sep.at(3));
            genPow.cost1=boost::lexical_cast<int>(sep.at(4));
            genPow.cost2=boost::lexical_cast<int>(sep.at(5));
            this->mGenPowerLimitData->push_back(genPow);
        }
        break;
        default:
            break;
        }
        //boost::algorithm::split(sep,line,',');
    }
    file.close();
    return true;
}




}
