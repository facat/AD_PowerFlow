/********************************************************
����һ���̳��࣬���ڴ�IEEExxx.dat�ļ��������ݡ�
*********************************************************/

#ifndef IPSOTEXTREADER_H
#define IPSOTEXTREADER_H

#include "abstractreader.h"
#include <boost/algorithm/string/split.hpp>
//#include <boost/numeric/conversion/converter.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
namespace DMY
{
class iPsoTextReader:public AbstractReader
{
//    enum blockTypeEnum
//    {
//        basicInfo=0,
//        EPSandSwingNode=1,//EPS ��xxx
//        swing=2,
//        line=3,
//        groundBranch=4,
//        trans=5,
//        nodePower=6,
//        genReativepow=7,
//        gencost=8
//    };//��¼��ǰ���ݿ�
//    typedef enum blockTypeEnum blockType;
    public:
        /** Default constructor */
        iPsoTextReader();
        /** Default destructor */
        virtual ~iPsoTextReader();
        virtual bool Read(const char *filePath);
    protected:
    private:
};

}
#endif // IPSOTEXTREADER_H
