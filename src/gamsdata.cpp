#include "gamsdata.h"
#include <iomanip>
#include <iosfwd>


GAMSData::GAMSData(const char * fileName):
    nodePD(NULL),nodeQD(NULL),nodeQG(NULL),nodePG(NULL),p_Y_Matrix(NULL),mSparseYMatrix(NULL)
{
	this->Update(fileName);
}

GAMSData::~GAMSData()
{
	if(this->p_Y_Matrix!=NULL)
	{
		delete this->p_Y_Matrix;
	}
    if(this->nodePG!=NULL)
    {
        delete[] this->nodePG;
    }
    if(this->nodeQG!=NULL)
    {
        delete[] this->nodeQG;
    }
    if(this->nodePD!=NULL)
    {
        delete[] this->nodePD;
    }
    if(this->nodeQD!=NULL)
    {
        delete[] this->nodeQD;
    }
}

bool GAMSData::Update(const char * fileName)
{
	char c_r;//读入无用的数据
	UINT U_r;//读入无用的数据
	UINT NodNum;
	UINT BalNum;
	double BaseNodVolt;
	double EPS;
	std::stringstream ss_dataline;//stringstream 的一行
	std::ifstream file(fileName);//
	if(file.is_open()==false)
	{
		return false;
	}
	std::string dataline;//读入的一行数据

	getline(file,dataline);
	ss_dataline<<dataline;
	ss_dataline>>NodNum>>c_r>>BalNum>>c_r>>BaseNodVolt>>c_r>>EPS;
	this->mNodeNum=NodNum;
	getline(file,dataline);//跳过一行
	UINT YDim=NodNum;//Y矩阵维数
	p_Y_Matrix=new SparseMat<D_Complex<double> >(YDim);
	getline(file,dataline);
	ss_dataline.str("");
	ss_dataline.clear();
	ss_dataline<<dataline;
	ss_dataline>>U_r;
	while(0!=U_r)
	{
		UINT NodI;//节点I
		UINT NodJ;//几点J
		UINT t_Nod;//交换NodI和NodJ用
		double R;//电阻
		double X;//电抗
		double b2;//电纳
		ss_dataline>>c_r>>NodI>>c_r>>NodJ>>c_r>>R>>c_r>>X>>c_r>>b2;
		D_Complex<double> t(R,X);
		D_Complex<double> admit(0,0);
		admit=D_Complex<double>(1,0)/t;
		if(NodI>NodJ)
		{
			t_Nod=NodI;
			NodI=NodJ;
			NodJ=t_Nod;
		}
		t=D_Complex<double>(0,0)-admit;
		p_Y_Matrix->Add((NodI-1),NodJ-1,t);//Y也是极坐标形式
		p_Y_Matrix->Add((NodJ-1),NodI-1,t);//Y也是极坐标形式
		p_Y_Matrix->Add((NodI-1),NodI-1,admit);
		p_Y_Matrix->Add((NodJ-1),NodJ-1,admit);
		p_Y_Matrix->Add((NodI-1),(NodI-1),D_Complex<double>(0,b2));
		p_Y_Matrix->Add((NodJ-1),(NodJ-1),D_Complex<double>(0,b2));
		getline(file,dataline);
		ss_dataline.str("");
		ss_dataline.clear();
		ss_dataline<<dataline;
		ss_dataline>>U_r;
	}
	getline(file,dataline);
	ss_dataline.str("");
	ss_dataline.clear();
	ss_dataline<<dataline;
	ss_dataline>>U_r;
	while(0!=U_r)   //读取变压器支路
	{
		UINT NodI;//节点I
		UINT NodJ;//几点J
		UINT t_Nod;//交换NodI和NodJ用
		double R;//电阻
		double X;//电抗
		double K;//变压器变比
		ss_dataline>>c_r>>NodI>>c_r>>NodJ>>c_r>>R>>c_r>>X>>c_r>>K;

		D_Complex<double> t(0,0);
		D_Complex<double> admit(R,X);
		admit=D_Complex<double>(1,0)/admit;//得到Y导纳
		t=admit/K/K;
		p_Y_Matrix->Add((NodI-1),NodI-1,t);
		p_Y_Matrix->Add((NodJ-1),NodJ-1,admit);
		t=D_Complex<double>(0,0)-(admit/K);
		if(NodI>NodJ)
		{
			t_Nod=NodI;
			NodI=NodJ;
			NodJ=t_Nod;
		}
		p_Y_Matrix->Add((NodI-1),NodJ-1,t);
		p_Y_Matrix->Add((NodJ-1),NodI-1,t);

		getline(file,dataline);
		ss_dataline.str("");
		ss_dataline.clear();
		ss_dataline<<dataline;
		ss_dataline>>U_r;
	}
	getline(file,dataline);
	ss_dataline.str("");
	ss_dataline.clear();
	ss_dataline<<dataline;
	ss_dataline>>U_r;
	while(U_r!=0)   //接地导纳
	{
		UINT Nod;
		double B,G;
		ss_dataline>>c_r>>Nod>>c_r>>G>>c_r>>B;
		D_Complex<double> admit(G,B);
		p_Y_Matrix->Add((Nod-1),Nod-1,admit);

		getline(file,dataline);
		ss_dataline.str("");
		ss_dataline.clear();
		ss_dataline<<dataline;
		ss_dataline>>U_r;
	}
	getline(file,dataline);
	ss_dataline.str("");
	ss_dataline.clear();
	ss_dataline<<dataline;
	ss_dataline>>U_r;
    this->nodePG=new double[this->mNodeNum];
    for(int i=0;i<this->mNodeNum;i++)
    {
        this->nodePG[i]=0;
    }
    this->nodeQG=new double[this->mNodeNum];
    for(int i=0;i<this->mNodeNum;i++)
    {
        this->nodeQG[i]=0;
    }
    this->nodePD=new double[this->mNodeNum];
    for(int i=0;i<this->mNodeNum;i++)
    {
        this->nodePD[i]=0;
    }
    this->nodeQD=new double[this->mNodeNum];
    for(int i=0;i<this->mNodeNum;i++)
    {
        this->nodeQD[i]=0;
    }
	while(U_r!=0)   //功率
	{
		double PD,PG,QD,QG;
		UINT Nod;//节点号
		ss_dataline>>c_r>>Nod>>c_r>>PG>>c_r>>QG>>c_r>>PD>>c_r>>QD;
        this->nodePG[Nod-1]=PG;//注入有功功率
        if(fabs(this->nodePG[Nod-1])<1e-5)
        {
            this->nodePG[Nod-1]=0;
        }
        this->nodeQG[Nod-1]=QG;//注入无功功率
        if(fabs(this->nodeQG[Nod-1])<1e-5)
        {
            this->nodeQG[Nod-1]=0;
        }
        this->nodePD[Nod-1]=PD;//注入有功功率
        if(fabs(this->nodePD[Nod-1])<1e-5)
        {
            this->nodePD[Nod-1]=0;
        }
        this->nodeQD[Nod-1]=QD;//注入无功功率
        if(fabs(this->nodeQD[Nod-1])<1e-5)
        {
            this->nodeQD[Nod-1]=0;
        }
		getline(file,dataline);
		ss_dataline.str("");
		ss_dataline.clear();
		ss_dataline<<dataline;
		ss_dataline>>U_r;
	}
	getline(file,dataline);
	ss_dataline.str("");
	ss_dataline.clear();
	ss_dataline<<dataline;
	ss_dataline>>U_r;
	while(U_r!=0)
	{
		UINT Nod;//节点电压
		double Volt=0;
		ss_dataline>>c_r>>Nod>>c_r>>Volt;
		ss_dataline<<dataline;
		ss_dataline>>U_r;
		getline(file,dataline);
		ss_dataline.str("");
		ss_dataline.clear();
		ss_dataline<<dataline;
		ss_dataline>>U_r;
	}

	file.close();
	return true;

}

void GAMSData::FormSparseYMatrix()
{
	if(NULL==this->p_Y_Matrix)
	{
		std::cout<<"必须先运行Updata形成Y_Matrix"<<std::endl;
		return;
	}
	//先数一遍有多少个元素
	int count=0;
	std::list< DSparseMat::Ele >* allHeadEle(this->p_Y_Matrix->GetHeadEle());//
	
	UINT dim= this->p_Y_Matrix->GetDim();
	for(UINT i=0;i<dim;i++)
	{
		count+=allHeadEle[i].size();
	}

	this->mNodeNum=new double[count];
	//开始从YMatrix把数据导入mSparseMatrix中
	int acc=0;
	for(UINT i=0;i<dim;i++)
	{
		for(std::list< DSparseMat::Ele >:iterator ite=allHeadEle[i].begin();
			ite!=allHeadEle[i].end();
			ite++
			)
		{
			count+=
		}
	}

}

