/************************************************************
用ADOL-C求解最优潮流.
*************************************************************/
#include <iostream>
#include <ipsotextreader.h>
#include <ymatrix.h>
using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    DMY::iPsoTextReader reader;
    reader.Read("E:/OPF/ieee4.dat");
    DMY::YMatrix yMatrix(reader);
    std::cout<<yMatrix.GetYMatrix()<<std::endl;
    return 0;
}
