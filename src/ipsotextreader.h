/********************************************************
����һ���̳��࣬���ڴ�IEEExxx.dat�ļ��������ݡ�
*********************************************************/

#ifndef IPSOTEXTREADER_H
#define IPSOTEXTREADER_H

#include "abstractreader.h"
#include <boost/algorithm/string/split.hpp>
//#include <boost/numeric/conversion/converter.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <string>
#include <vector>
namespace DMY
{
class iPsoTextReader:public AbstractReader
{
    public:
        /** Default constructor */
        iPsoTextReader();
        /** Default destructor */
        virtual ~iPsoTextReader();
        virtual bool Read(const char *filePath);
    protected:
    private:
        std::string Simplified(std::string &str);//ɾ������Ŀո�ֻ��һ���ո���Ϊ�����
};

}
#endif // IPSOTEXTREADER_H
