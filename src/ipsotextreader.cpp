#include "ipsotextreader.h"
#include <cstring>
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

	this->mSwingNodeData->clear();
	this->mLineData->clear();
	this->mTransData->clear();
	this->mCapacitorData->clear();
	this->mgroundBranchData->clear();
	this->mNodePowerData->clear();
	this->mGenPowerLimitData->clear();
	this->mGenReactivepowerLimitData->clear();
	std::string line;
	std::ifstream file(filePath);
	std::vector<std::string> sep;
	char buff[1024];
	int currentBlock=0;
	if(!file.is_open())
	{
		std::cout<<"�����ļ��򿪴���!"<<std::endl;
		return false;
	}
	if(!file.eof())
	{
		file.getline(buff,sizeof(buff));
		line.assign(buff);
		line=this->Simplified(line);
		boost::algorithm::split(sep,line,boost::is_any_of(" "));//��,���зָ�
		if(sep.size()<5)
		{
			std::cout<<"������Ϣ���ݸ�ʽ����!"<<std::endl;
			std::cout<<buff<<std::endl;
			return false;
		}
		mBasicInfo.totalNode=boost::lexical_cast<int>(sep.at(0));
		mBasicInfo.totalBranch=boost::lexical_cast<int>(sep.at(1));
		mBasicInfo.baseMVA=boost::lexical_cast<double>(sep.at(2));
		mBasicInfo.maxIte=boost::lexical_cast<double>(sep.at(3));
		mBasicInfo.centerParameter=boost::lexical_cast<double>(sep.at(4));
		//mBasicInfo.centerParameter=boost::lexical_cast<double>(".1");
	}
	if(!file.eof())
	{
		file.getline(buff,sizeof(buff));
		line.assign(buff);
		line=this->Simplified(line);
		boost::algorithm::split(sep,line,boost::is_any_of(" "));//�� ���зָ�
		if(sep.size()<2)
		{
			std::cout<<"������ģ�͸�ʽ����!"<<std::endl;
			std::cout<<buff<<std::endl;
			return false;
		}
		mBasicInfo.EPS=boost::lexical_cast<double>(sep.at(0));
		mBasicInfo.modelType=boost::lexical_cast<int>(sep.at(1));
	}
	while(!file.eof())
	{
		file.getline(buff,sizeof(buff));
		line.assign(buff);
		line=this->Simplified(line);
		if(boost::algorithm::starts_with(line,"0"))
		{
			currentBlock++;
			continue;
		}
		boost::algorithm::split(sep,line,boost::is_any_of(" "));//�� ���зָ�
		switch(currentBlock)
		{
		case 0:
		{
			if(sep.size()<2)
			{
				std::cout<<"ƽ��ڵ����ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			swingNodeStruct swingNode;
			swingNode.volt=1;//��ʱ��1����������
			swingNode.i=boost::lexical_cast<int>(sep.at(1))-1;
			this->mSwingNodeData->push_back(swingNode);
		}
		break;
		case 1:
		{
			if(sep.size()<6)
			{
				std::cout<<"��·���ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			lineStruct lineData;
			lineData.i=boost::lexical_cast<int>(sep.at(1))-1;
			lineData.j=boost::lexical_cast<int>(sep.at(2))-1;
			lineData.r=boost::lexical_cast<double>(sep.at(3));
			lineData.x=boost::lexical_cast<double>(sep.at(4));
			lineData.bi_2=boost::lexical_cast<double>(sep.at(5));
			lineData.bj_2=boost::lexical_cast<double>(sep.at(5));
			this->mLineData->push_back(lineData);
		}
		break;
		case 2:
		{
			if(sep.size()<2)
			{
				std::cout<<"�ӵ�֧·���ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			groundBranchStruct groundBranch;
			groundBranch.i=boost::lexical_cast<int>(sep.at(0))-1;
			groundBranch.b=boost::lexical_cast<int>(sep.at(1));
			groundBranch.g=0;
			this->mgroundBranchData->push_back(groundBranch);
		}
		break;
		case 3:
		{
			if(sep.size()<8)
			{
				std::cout<<"��ѹ�����ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			transStruct trans;
			trans.i=boost::lexical_cast<int>(sep.at(1))-1;
			trans.j=boost::lexical_cast<int>(sep.at(2))-1;
			trans.r=boost::lexical_cast<double>(sep.at(3));
			trans.x=boost::lexical_cast<double>(sep.at(4));
			trans.k0=boost::lexical_cast<double>(sep.at(5));
			trans.kl=boost::lexical_cast<double>(sep.at(6));
			trans.ku=boost::lexical_cast<double>(sep.at(7));
			trans.adjustable=-1;
			trans.tap=-1;
			this->mTransData->push_back(trans);
		}
		break;
		case 4:
		{
			if(sep.size()<5)
			{
				std::cout<<"�ڵ㹦�����ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			nodePowerStruct nodePow;
			double baseMVA=this->mBasicInfo.baseMVA;
			nodePow.i=boost::lexical_cast<int>(sep.at(0))-1;
			nodePow.PG=boost::lexical_cast<double>(sep.at(1))/baseMVA;//ȫ��������ֵ
			nodePow.QG=boost::lexical_cast<double>(sep.at(2))/baseMVA;
			nodePow.PD=boost::lexical_cast<double>(sep.at(3))/baseMVA;
			nodePow.QD=boost::lexical_cast<double>(sep.at(4))/baseMVA;
			nodePow.voltLevel=-1;//-1��ʾû��
			nodePow.zone=-1;//-1��ʾû��
			this->mNodePowerData->push_back(nodePow);
		}
		break;
		case 5:
		{
			if(sep.size()<4)
			{
				std::cout<<"������޹��������ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			genReactivepowerLimitStruct genRepow;
			genRepow.i=boost::lexical_cast<int>(sep.at(0))-1;
			genRepow.fixVolt=boost::lexical_cast<double>(sep.at(1));
			genRepow.ql=boost::lexical_cast<double>(sep.at(2));
			genRepow.qu=boost::lexical_cast<double>(sep.at(3));
			genRepow.adjustable=-1;
			genRepow.isSwing=-1;
			this->mGenReactivepowerLimitData->push_back(genRepow);
		}
		break;
		case 6:
		{
			if(sep.size()<6)
			{
				std::cout<<"������й����������ݸ�ʽ����!"<<std::endl;
				std::cout<<buff<<std::endl;
				return false;
			}
			genPowerLimitStruct genPow;
			double baseMVA=this->mBasicInfo.baseMVA;
			genPow.i=boost::lexical_cast<int>(sep.at(0))-1;
			genPow.pl=boost::lexical_cast<double>(sep.at(1))/baseMVA;
			genPow.pu=boost::lexical_cast<double>(sep.at(2))/baseMVA;
			genPow.cost0=boost::lexical_cast<double>(sep.at(3));
			genPow.cost1=boost::lexical_cast<double>(sep.at(4));
			genPow.cost2=boost::lexical_cast<double>(sep.at(5));
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

std::string iPsoTextReader::Simplified(std::string &str)//ɾ������Ŀո�ֻ��һ���ո���Ϊ�����
{
	std::string newStr;
	std::string _str(boost::algorithm::trim_copy(str));
	newStr.reserve(_str.length()+1);//��һ��\0
	bool spaceAhead=false;//ǰ��һ���Ƿ��ǿո�
	for(size_t i=0; i<_str.length(); i++)
	{
		if( (_str.at(i)==' ' || _str.at(i)=='\t') && spaceAhead)
		{
			continue;
		}


		if(_str.at(i)==' ' || _str.at(i)=='\t')
		{
			spaceAhead=true;
			newStr.push_back(' ');
			//newStr+=" ";
		}
		else
		{
			spaceAhead=false;
			newStr.push_back(_str.at(i));
			//newStr+=_str.at(i);
		}
	}


	return newStr;


}

/*std::string iPsoTextReader::Simplified(std::string &str)//ɾ������Ŀո�ֻ��һ���ո���Ϊ�����
{
	return std::string();
	std::string newStr;
	std::string _str;
	//std::string _str(boost::algorithm::trim_copy(str));
	_str="2 1 4 .12 .5 0.0192";
	return _str;

}*/


}
