/************************************************************
用ADOL-C求解最优潮流.
*************************************************************/
#include <iostream>
#include <ipsotextreader.h>
#include <ymatrix.h>
#include <string>
#include <PFjacobian.h>
#include <vector>
#include <commonstructure.h>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <powerflow.h>
#include <fstream>
using namespace std;


int main()
{
	cout << "Hello world!" << endl;
    DMY::PowerFlow pf;
    pf.Run("e:/opf/ieee1047.dat");
    std::ofstream file("c:/pfresult.csv");
    pf.Output(file);
    file.close();
	return 0;
}
