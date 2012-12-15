#include "ymatrix.h"

namespace DMY
{

YMatrix::YMatrix(const AbstractReader &reader):
    mReader(reader),
    mSpMatY(new Eigen::SparseMatrix<double> (reader.GetTotalNodeNum(),reader.GetTotalNodeNum()) ),
    mSpMatYAngle(new Eigen::SparseMatrix<double> (reader.GetTotalNodeNum(),reader.GetTotalNodeNum()) )
{
    //ctor

}

YMatrix::~YMatrix()
{
    //dtor
}
bool YMatrix::MakeParameter()
{
    this->mTotalNode=this->mReader.GetTotalNodeNum();
    this->mSpYMatrix.resize(this->mTotalNode,this->mTotalNode);
    this->MakeLineParameter(this->mReader.GetLineData());
    this->MakeTransParameter(this->mReader.GetTransData());
    this->MakeGroundBranchParameter(this->mReader.GetgroundBranchData());
    std::list<Eigen::Triplet<double> > allTripletB;
    allTripletB.insert(allTripletB.end(),this->mLineParaB.begin(),this->mLineParaB.end());
    allTripletB.insert(allTripletB.end(),this->mTransB.begin(),this->mTransB.end());
    allTripletB.insert(allTripletB.end(),this->mGroundBranchB.begin(),this->mGroundBranchB.end());
    //电纳
    std::list<Eigen::Triplet<double> > allTripletG;
    allTripletG.insert(allTripletG.end(),this->mLineParaG.begin(),this->mLineParaG.end());
    allTripletG.insert(allTripletG.end(),this->mTransG.begin(),this->mTransG.end());
    allTripletG.insert(allTripletG.end(),this->mGroundBranchG.begin(),this->mGroundBranchG.end());
    Eigen::SparseMatrix<double> spMatB(this->mTotalNode,this->mTotalNode);
    spMatB.setFromTriplets(allTripletB.begin(),allTripletB.end());
    Eigen::SparseMatrix<double> spMatG(this->mTotalNode,this->mTotalNode);
    spMatG.setFromTriplets(allTripletG.begin(),allTripletG.end());
    *this->mSpMatY=(spMatB.cwiseAbs2()+spMatG.cwiseAbs2()).cwiseSqrt();
    *this->mSpMatYAngle=spMatB.binaryExpr(spMatG,Opatan());
    return true;
}
void YMatrix::MakeLineParameter(boost::shared_ptr<std::list<lineStruct> > lineData)
{
//    t1=-lineR./(lineR.^2+lineX.^2);%电导
//t2=lineX./(lineR.^2+lineX.^2);%电纳
//G = +sparse(lineI,lineJ,t1,Busnum,Busnum) + sparse(lineJ,lineI,t1,Busnum,Busnum);
//B = sparse(lineI,lineJ,t2,Busnum,Busnum)+sparse(lineJ,lineI,t2,Busnum,Busnum);

    std::vector<double> addB;//累积电纳用
    addB.resize(this->mTotalNode,0);
    std::vector<double> addG;//累积电导
    addG.resize(this->mTotalNode,0);
    for(std::list<lineStruct>::iterator ite=lineData->begin();
            ite!=lineData->end();
            ite++)
    {
        lineStruct line=*ite;
        //充电电容
        this->mLineParaB.push_back(
            Eigen::Triplet<double>(line.i,
                                   line.i,
                                   line.bi_2
                                  )
        );
        this->mLineParaB.push_back(
            Eigen::Triplet<double>(line.j,
                                   line.j,
                                   line.bj_2
                                  )
        );
        double r2=std::pow(line.r,2);
        double x2=std::pow(line.x,2);
        //电阻
        this->mLineParaG.push_back(Eigen::Triplet<double>(line.i,
                                   line.j,
                                   -line.r/(r2+x2)
                                                         )
                                  );
        addG[line.j]+=-line.r/(r2+x2);//按列累加
        this->mLineParaG.push_back(Eigen::Triplet<double>(line.j,
                                   line.i,
                                   -line.r/(r2+x2)
                                                         )
                                  );
        addG[line.i]+=-line.r/(r2+x2);//按列累加
        //电抗
        this->mLineParaB.push_back(Eigen::Triplet<double>(line.i,
                                   line.j,
                                   line.x/(r2+x2)
                                                         )
                                  );
        addB[line.j]+=line.x/(r2+x2);//按列累加
        this->mLineParaB.push_back(Eigen::Triplet<double>(line.j,
                                   line.i,
                                   line.x/(r2+x2)
                                                         )
                                  );
        addB[line.i]+=line.x/(r2+x2);//按列累加
        //电纳
    }

//G = G - sparse(1:Busnum,1:Busnum,sum(G,2)');
//B = B - sparse(1:Busnum,1:Busnum,sum(B,2)');
    for(int i=0; i<this->mTotalNode; i++)
    {
        this->mLineParaB.push_back(
            Eigen::Triplet<double>(i,
                                   i,
                                   -addB.at(i)
                                  )
        );
//        std::cout<<-addB.at(i)<<std::endl;
        this->mLineParaG.push_back(
            Eigen::Triplet<double>(i,
                                   i,
                                   -addG.at(i)
                                  )
        );
    }

}

void YMatrix::MakeGroundBranchParameter(boost::shared_ptr<std::list<groundBranchStruct> > groundBranchData)
{
    for(std::list<groundBranchStruct>::iterator ite=groundBranchData->begin();
            ite!=groundBranchData->end();
            ite++)
    {
        groundBranchStruct groundBranch;
        this->mGroundBranchB.push_back(
            Eigen::Triplet<double>(groundBranch.i,
                                   groundBranch.i,
                                   groundBranch.b
                                  )
        );
        this->mGroundBranchG.push_back(
            Eigen::Triplet<double>(groundBranch.i,
                                   groundBranch.i,
                                   groundBranch.g
                                  )
        );
    }
}

void YMatrix::MakeTransParameter(boost::shared_ptr<std::list<transStruct> > transData)
{
//    t1 = -transR./(transR.^2+transX.^2);
//    t2 = transX./(transR.^2+transX.^2);
//    G = G+sparse(transI,transJ,t1./transK,Busnum,Busnum)+sparse(transJ,transI,t1./transK,Busnum,Busnum)-sparse(transI,transI,t1./transK./transK,Busnum,Busnum)-sparse(transJ,transJ,t1,Busnum,Busnum);
//    B = B+sparse(transI,transJ,t2./transK,Busnum,Busnum)+sparse(transJ,transI,t2./transK,Busnum,Busnum)-sparse(transI,transI,t2./transK./transK,Busnum,Busnum)-sparse(transJ,transJ,t2,Busnum,Busnum);
    for(std::list<transStruct>::iterator ite=transData->begin();
            ite!=transData->end();
            ite++)
    {
        transStruct trans=*ite;
        double r2=std::pow(trans.r,2);
        double x2=std::pow(trans.x,2);
        this->mTransG.push_back(
            Eigen::Triplet<double>(trans.i,
                                   trans.j,
                                   -trans.r/(r2+x2)/trans.k0
                                  )
        );
        this->mTransG.push_back(
            Eigen::Triplet<double>(trans.j,
                                   trans.i,
                                   -trans.r/(r2+x2)/trans.k0
                                  )
        );
        //对角
        this->mTransG.push_back(
            Eigen::Triplet<double>(trans.i,
                                   trans.i,
                                   trans.r/(r2+x2)/trans.k0/trans.k0
                                  )
        );
        this->mTransG.push_back(
            Eigen::Triplet<double>(trans.j,
                                   trans.j,
                                   trans.r/(r2+x2)
                                  )
        );
        //B
        this->mTransB.push_back(
            Eigen::Triplet<double>(trans.i,
                                   trans.j,
                                   trans.x/(r2+x2)/trans.k0
                                  )
        );
        this->mTransB.push_back(
            Eigen::Triplet<double>(trans.j,
                                   trans.i,
                                   trans.x/(r2+x2)/trans.k0
                                  )
        );
        //对角
        this->mTransB.push_back(
            Eigen::Triplet<double>(trans.i,
                                   trans.i,
                                   -trans.x/(r2+x2)/trans.k0/trans.k0
                                  )
        );
        this->mTransB.push_back(
            Eigen::Triplet<double>(trans.j,
                                   trans.j,
                                   -trans.x/(r2+x2)
                                  )
        );

    }
}

boost::shared_ptr<Eigen::SparseMatrix<double> > YMatrix::GetYMatrix() const
{
    return this->mSpMatY;
}
boost::shared_ptr<Eigen::SparseMatrix<double> > YMatrix::GetYAngleMatrix() const
{
    return this->mSpMatYAngle;
}


}


