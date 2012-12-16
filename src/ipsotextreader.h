/********************************************************
这是一个继承类，用于从IEEExxx.dat文件读入数据。
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
#include <boost/shared_array.hpp>
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
        std::string Simplified(std::string &str);//删除多余的空格，只用一个空格作为间隔。
};

}
#endif // IPSOTEXTREADER_H
