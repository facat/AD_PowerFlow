/************************************************************
用ADOL-C求解最优潮流.
*************************************************************/
#include <iostream>
#include <ipsotextreader.h>
#include <ymatrix.h>
#include <boost/algorithm/string/regex.hpp>
#include <cstring>
using namespace std;

int main()
{
	cout << "Hello world!" << endl;
	DMY::iPsoTextReader reader;
	if(reader.Read("E:/OPF/ieee4.dat"))
	{
		DMY::YMatrix yMatrix(reader);
		if(yMatrix.MakeParameter())
		{
		    std::cout<<"Y"<<std::endl;
			std::cout<<*(yMatrix.GetYMatrix())<<std::endl;
			std::cout<<"YAngle"<<std::endl;
			std::cout<<*(yMatrix.GetYAngleMatrix())<<std::endl;
		}
	}


	return 0;
}
