const double QuantilesR[24] = {6000, 7000, 8000, 9000, 11830, 14778.3, 16948.3, 18591.7, 20054, 21497.5, 22676, 23890, 24942, 25821.7, 26645, 27396.7, 28168.3, 28955, 29728, 30610, 31603.3, 32616.7, 33966.7, 39990};

const double QuantilesE[10] = {3.4449470,4.7075471,5.6179705,6.3958938,7.123,7.8501062,8.6280295,9.5384529,10.801053,100};

const double Quantiles[20] = {1./(19434.*19434.), 2./(19434.*19434.),3./(19434.*19434.),4./(19434.*19434.),5./(19434.*19434.),6./(19434.*19434.),7./(19434.*19434.),8./(19434.*19434.),9./(19434.*19434.),10./(19434.*19434.),
                              20./(19434.*19434.),30./(19434.*19434.),40./(19434.*19434.),50./(19434.*19434.),60./(19434.*19434.),70./(19434.*19434.),80./(19434.*19434.),90./(19434.*19434.),100./(19434.*19434.),200./(19434.*19434.)};

struct DataWithError
{
    double Data;
    double Error;
};

struct Vector3D
{
    double x,y,z;
};

double DotProduct(Vector3D A, Vector3D B)//求两个向量的点乘
{
    double result;
    result = A.x*B.x + A.y*B.y + A.z*B.z;
    return result;
}

Vector3D CrossProduct(Vector3D A, Vector3D B)
{
    Vector3D result;
    result.x = A.y*B.z - A.z*B.y;
    result.y = A.z*B.x - A.x*B.z;
    result.z = A.x*B.y - A.y*B.x;
    return result;
}

double Norm(Vector3D A)//求向量的模
{
    double result;
    result = sqrt(pow(A.x, 2) + pow(A.y, 2) + pow(A.z, 2));
    return result;
}

Vector3D VectorMinus(Vector3D A,Vector3D B)//A向量减B向量
{
    Vector3D result;
    result.x = A.x - B.x;
    result.y = A.y - B.y;
    result.z = A.z - B.z;
    return result;
}

Vector3D VectorAdd(Vector3D A,Vector3D B)//A向量加B向量
{
    Vector3D result;
    result.x = A.x + B.x;
    result.y = A.y + B.y;
    result.z = A.z + B.z;
    return result;
}

Vector3D Multiply(double A,Vector3D B)//A乘B向量
{
    Vector3D result;
    result.x = A*B.x;
    result.y = A*B.y;
    result.z = A*B.z;
    return result;
}

double GetLPMTPosition(Vector3D (&LPMTPosition)[17612])
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;

    while (getline(file, line)) 
    {
        stringstream ss(line);
        Vector3D PMT;
        double nun;
        int currentPMT;
        ss >> currentPMT >> PMT.x >> PMT.y >> PMT.z >> nun >> nun;
        LPMTPosition[currentPMT] = PMT;
    }

    file.close();
    return 0;
}

double GetTheta(Vector3D PMT,Vector3D edep)
{
    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);
    Vector3D FZ;
    FZ.x = -PMT.x;
    FZ.y = -PMT.y;
    FZ.z = -PMT.z;
    double z = DotProduct(FZ,LPMT_edep)/Norm(FZ);
    double r = Norm(LPMT_edep);
    double angle = acos(z/r) * 180 / M_PI;
    return angle;
}

double GetR(Vector3D PMT,Vector3D edep)
{
    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);
    double r = Norm(LPMT_edep);
    return r;
}

double GetPhi(Vector3D PMT, Vector3D edep, Vector3D ImagePoint)
{
    Vector3D FX;//随动坐标系的x
    Vector3D FY;//随动坐标系的y
    Vector3D fx = VectorMinus(ImagePoint,PMT);
    double a = -DotProduct(fx,PMT)/DotProduct(PMT,PMT);
    FX = VectorAdd(fx,Multiply(a,PMT));
    FY = CrossProduct(FX,PMT);

    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);

    double x = DotProduct(LPMT_edep,FX)/Norm(FX);
    double y = DotProduct(LPMT_edep,FY)/Norm(FY);
    double angle = atan(y/x) * 180 / M_PI;
    if (y>0 && angle<0)
    {
        angle += 180;
    }
    else if (y<0 && angle>0)
    {
        angle += 180;
    }
    else if (y<0 && angle<0)
    {
        angle += 360;
    }
    
    return angle;
}

double GetDtoImage(double &MinDistance, double &Phi, Vector3D PMT, Vector3D (&ImagePoint)[590], Vector3D Edep_Position) {
    double Distance;
    MinDistance = Norm(VectorMinus(PMT,ImagePoint[0]));
    Phi = GetPhi(PMT,Edep_Position,ImagePoint[0]);
    for (int ImageID = 1; ImageID < 590; ImageID++)
    {
        Distance = Norm(VectorMinus(PMT,ImagePoint[ImageID]));
        if (Distance < MinDistance)
        {
            MinDistance = Distance;
            Phi = GetPhi(PMT,Edep_Position,ImagePoint[ImageID]);
        }
    }
    return 0;
}

double GetNodeImage(Vector3D (&ImagePoint)[590], Vector3D Edep_Position, Vector3D (&NodePosition)[590]) {
    double RLS = 17700.;
    double RPMT = 19434.;
    double RfrIndxLS = 1.54;
    double RfrIndxWR = 1.355;

    for (int NodeID = 0; NodeID < 590; NodeID++) {
        Vector3D Input = VectorMinus(NodePosition[NodeID],Edep_Position);
        Vector3D NormalLine;
        NormalLine.x = -NodePosition[NodeID].x;
        NormalLine.y = -NodePosition[NodeID].y;
        NormalLine.z = -NodePosition[NodeID].z;

        Vector3D VA = VectorAdd(Multiply(-DotProduct(Input,NormalLine)/(Norm(NormalLine)*Norm(NormalLine)), NormalLine),Input);
        // cout<<"VA x="<<VA.x<<"; y="<<VA.y<<"; z="<<VA.z<<endl;
        double SinOut = RfrIndxLS*Norm(VA)/(RfrIndxWR*Norm(Input));
        // double CosIn = DotProduct(Input,NormalLine)/(Norm(Input)*Norm(NormalLine));
        // cout<<"SinIn = "<<sqrt(1-CosIn*CosIn)<<endl;
        // cout<<"SinOut ="<<SinOut<<endl;
        // cout<<"SinIn/SinOut = "<<sqrt(1-CosIn*CosIn)/SinOut<<" Ref="<<RfrIndxWR/RfrIndxLS<<endl;
        Vector3D Output = VectorMinus(Multiply(SinOut/Norm(VA),VA),Multiply(sqrt(1-SinOut*SinOut)/Norm(NormalLine),NormalLine));
        if (Norm(VA) == 0)
        {
            Output = VectorMinus(VA,Multiply(1./Norm(NormalLine),NormalLine));
        }
        
        // cout<<"Output x="<<Output.x<<"; y="<<Output.y<<"; z="<<Output.z<<endl;

        double LenthOfOutput = -RLS*sqrt(1-SinOut*SinOut) + sqrt(RLS*sqrt(1-SinOut*SinOut)*RLS*sqrt(1-SinOut*SinOut) - (RLS*RLS-RPMT*RPMT));

        ImagePoint[NodeID] = VectorAdd(NodePosition[NodeID],Multiply(LenthOfOutput,Output));
    }

    return 0;
}

double GetNodePosition(Vector3D (&NodePosition)[590]) {
    ifstream inputfile_anchor;
    TString filename_anchor="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/junosw/Simulation/DetSimV2/DetSimOptions/data/Strut_Anchor_Acrylic.csv";
    inputfile_anchor.open(filename_anchor,ios::in);
    double R = 17700.;
    double ID_Anchor[590];
    double Theta_Anchor[590];
    double Phi_Anchor[590];
    for(int i=0;i<590;i++) {
        inputfile_anchor>>ID_Anchor[i]>>Theta_Anchor[i]>>Phi_Anchor[i];
        NodePosition[i].z = R*cos(Theta_Anchor[i]*M_PI/180.0);
        NodePosition[i].x = R*sin(Theta_Anchor[i]*M_PI/180.0)*cos(Phi_Anchor[i]*M_PI/180.0);
        NodePosition[i].y = R*sin(Theta_Anchor[i]*M_PI/180.0)*sin(Phi_Anchor[i]*M_PI/180.0);
    }
    return 0;
}

//修正前后电荷的对应关系函数，这个函数的自变量是真实电荷，因变量是包含阈值和暗噪声效应的电荷
double Myfunction(double* x, double* par) //par[0] = Threshold, par[1] = Sigma_SPE, par[2] = DarkRate
{
    double P0 = 0;
    double DarkRateCharge = par[2]*1e-6;//1000ns内暗噪声贡献的电荷的均值
    double xx = x[0]+DarkRateCharge;//暗噪声和物理事例总的电荷均值
    P0 += ROOT::Math::poisson_pdf(0,xx);//均值为“xx”时，测到0 PE的概率
    for (int i = 1; i < 20; i++)//均值为“xx”时，测到i PE且过阈的概率。认为“xx”几乎不会超过5PE
    {
        P0 += ROOT::Math::poisson_pdf(i,xx)*ROOT::Math::normal_cdf(par[0],par[1]*sqrt((double)i),(double)i);
    }
    return -TMath::Log(P0);
}

DataWithError GetTrueCharge(DataWithError RoughCharge, DataWithError DarkRate)//RoughCharge为修正前的电荷，输出修正后的电荷
{
    DataWithError TrueCharge;
    // if (DarkRate.Data > RoughCharge.Data)
    // {
    //     TrueCharge.Data = -1;
    //     TrueCharge.Error = -1;
    // }
    double R_Spe = 33.2/100.0;  //J22.1.0-rc4，SPMT电子学模拟中使用了固定分辨率
    double Threshold = 1.0/3.0; //J22.1.0-rc4，SPMT电子学模拟中使用了固定阈值
    auto f = new TF1("function",Myfunction,0,5,3);
    f->SetParameters(Threshold,R_Spe,DarkRate.Data);
    TrueCharge.Data = f->GetX(RoughCharge.Data,0,0,1.E-12,1000,false);//反解出真实电荷

    //传递误差
    double df;
    df = f->Derivative(TrueCharge.Data,nullptr,1.e-5);
    TrueCharge.Error = sqrt( RoughCharge.Error*RoughCharge.Error/(df*df) + DarkRate.Error*DarkRate.Error );
    return TrueCharge;
}

DataWithError GetP0Mu(int TotalEvents, int ZeroEvents)
{
    double P0 = (double)(ZeroEvents)/(double)(TotalEvents);
    double mean = -TMath::Log(P0);
    double error = mean/TotalEvents;
    error = sqrt(error);
    DataWithError P0Mu;
    P0Mu.Data = mean;
    P0Mu.Error = error;
    return P0Mu;
}

DataWithError CombineResult(const vector<double>& x, const vector<double>& e)
{
    double sum_x = 0.0;
    double sum_w = 0.0;
    double sum_e = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1;
        sum_x += w * x[i];
        sum_w += w;
        sum_e += e[i]*e[i];
    }
    sum_e = sqrt(sum_e)/sum_w;
    DataWithError combined_result;
    combined_result.Data = sum_x / sum_w;
    combined_result.Error = sum_e;

    return combined_result;
}

double combined_measurement(const vector<double>& x, const vector<double>& e) 
{
    double sum_x = 0.0;
    double sum_w = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1 / (e[i] * e[i]);
        sum_x += w * x[i];
        sum_w += w;
    }

    double combined_x = sum_x / sum_w;
    return combined_x;
}

double combined_error(const vector<double>& e) 
{
    double sum_w = 0.0;

    for (int i = 0; i < e.size(); i++) {
        double w = 1 / (e[i] * e[i]);
        sum_w += w;
    }

    double combined_e = 1 / sqrt(sum_w);
    return combined_e;
}

Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
   double xx =x[0];
   double miu = 500 - sqrt(500*500 - 1000*xx);
   Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
   return f;
}

int GetCorrespondingSPMT(int (&CorrespondingSPMT)[17612][6])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map.txt");
    string line;
    int currentLine = 0;
    int SPMT_ID[6];

    while (getline(file, line)) 
    { 
        stringstream ss(line);
        int LPMTID;
        ss >> LPMTID >> SPMT_ID[0] >> SPMT_ID[1] >> SPMT_ID[2] >> SPMT_ID[3] >> SPMT_ID[4] >> SPMT_ID[5];
        for (int i = 0; i < 6; i++)
        {
            CorrespondingSPMT[LPMTID][i] = SPMT_ID[i];
        }
    }

    file.close();
    return 0;
}

double GetSPMTQE(double (&SPMTQE)[25600])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    for (int id = 0; id < 25600; id ++)
    {
        PMTData->GetEntry(id);
        SPMTQE[id] = Effiency;
    }
    
    // if (PMTID == SPMTID)
    // {
    //     inputfile->Close();
    //     return 100*Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong SPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetLPMTQE(double (&LPMTQE)[17612])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    for (int id = 0; id < 17612; id ++)
    {
        PMTData->GetEntry(id);
        LPMTQE[id] = Effiency/100.0;
    }
    
    // if (PMTID == LPMTID)
    // {
    //     inputfile->Close();
    //     return Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong LPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetSPMTDCR(double (&SPMTDCR)[25600])
{
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc8.dc1/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
    TTree *PMTData = (TTree*)DarkRate_file->Get("data");
    double darkRate;
    PMTData->SetBranchAddress("darkRate",&darkRate);
    for (int i = 0; i < 25600; i++)
    {
        PMTData->GetEntry(i);
        SPMTDCR[i] = darkRate;
    }
    DarkRate_file->Close();
    return 0;
}

double GetDistanceToBar(double (&DistanceToBar)[17612])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/PMTID_map_J23_1_0_rc2.txt");
    string line;
    while (getline(file, line)) 
    { 
        stringstream ss(line);
        int LPMTID;
        double Distance;
        double BarID;
        ss >> LPMTID >> BarID >> Distance;
        DistanceToBar[LPMTID] = Distance;
    }

    file.close();
    return 0;
}

int npeLPMTtonpeSPMT_B12()
{
    // 初始化要用到的PMT参数
    double LPMTQE[17612];
    double SPMTQE[25600];
    double SPMTDCR[25600];
    int CorrespondingSPMT[17612][6];
    // double DistanceToBar[17612];
    Vector3D LPMTPosition[17612];
    Vector3D NodePosition[590];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    GetSPMTDCR(SPMTDCR);
    // GetDistanceToBar(DistanceToBar);
    GetLPMTPosition(LPMTPosition);
    GetNodePosition(NodePosition);


    // 定义需要计算的数据，完成初始化
    vector<double> ToGraph_TotalLPMTMu;
    vector<double> ToGraph_TotalLPMTMuErr;

    vector<double> ToGraph_TotalTrueLPMTMu;
    vector<double> ToGraph_TotalTrueLPMTMuErr;

    vector<double> ToGraph_TotalSPMTMu_QE;
    vector<double> ToGraph_TotalSPMTMu_QEErr;

    // TStatistic *Sta_LPMTMu[24][10];
    // TStatistic *Sta_TrueLPMTMu[24][10];
    // TStatistic *Sta_SPMTDCR[24][10];
    // TStatistic *Sta_SPMTQE[24][10];
    // TStatistic *Sta_LPMTQE[24][10];
    // for (int RID = 0; RID < 24; RID++)
    // {
    //     for (int EID = 0; EID < 10; EID++)
    //     {
    //         Sta_LPMTMu[RID][EID] = new TStatistic(TString::Format("LPMTMu R%1d_E%1d",RID,EID));
    //         Sta_TrueLPMTMu[RID][EID] = new TStatistic(TString::Format("TrueLPMTMu R%1d_E%1d",RID,EID));
    //         Sta_SPMTDCR[RID][EID] = new TStatistic(TString::Format("SPMTDCR R%1d_E%1d",RID,EID));
    //         Sta_SPMTQE[RID][EID] = new TStatistic(TString::Format("SPMTQE R%1d_E%1d",RID,EID));
    //         Sta_LPMTQE[RID][EID] = new TStatistic(TString::Format("LPMTQE R%1d_E%1d",RID,EID));
    //     }
    // }
    // vector<vector<int>> SPMT0(24,vector<int>(10,0));

    TStatistic *Sta_LPMTMu[21];
    TStatistic *Sta_TrueLPMTMu[21];
    TStatistic *Sta_SPMTDCR[21];
    TStatistic *Sta_SPMTQE[21];
    TStatistic *Sta_LPMTQE[21];

    for (int QID = 0; QID < 21; QID++)
    {
        Sta_LPMTMu[QID] = new TStatistic(TString::Format("LPMTMu Q%1d",QID));
        Sta_TrueLPMTMu[QID] = new TStatistic(TString::Format("TrueLPMTMu Q%1d",QID));
        Sta_SPMTDCR[QID] = new TStatistic(TString::Format("SPMTDCR Q%1d",QID));
        Sta_SPMTQE[QID] = new TStatistic(TString::Format("SPMTQE Q%1d",QID));
        Sta_LPMTQE[QID] = new TStatistic(TString::Format("LPMTQE Q%1d",QID));
    }
    
    vector<int> SPMT0(21,0);

    TH1F *Hist_LPMTMu[21];
    TH1F *Hist_TrueLPMTMu[21];
    TH1F *Hist_SPMTDCR[21];
    TH1F *Hist_SPMTQE[21];
    TH1F *Hist_LPMTQE[21];
    TH1F *Hist_SPMTMu[21];

    for (int QID = 0; QID < 21; QID++)
    {
        Hist_LPMTMu[QID] = new TH1F(TString::Format("Hist LPMTMu Q%1d",QID),TString::Format("Hist LPMTMu Q%1d",QID),50000,0,500);
        Hist_TrueLPMTMu[QID] = new TH1F(TString::Format("Hist TrueLPMTMu Q%1d",QID),TString::Format("Hist TrueLPMTMu Q%1d",QID),50000,0,500);
        Hist_SPMTDCR[QID] = new TH1F(TString::Format("Hist SPMTDCR Q%1d",QID),TString::Format("Hist SPMTDCR Q%1d",QID),1000,0,1000);
        Hist_SPMTQE[QID] = new TH1F(TString::Format("Hist SPMTQE Q%1d",QID),TString::Format("Hist SPMTQE Q%1d",QID),100,0,1);
        Hist_LPMTQE[QID] = new TH1F(TString::Format("Hist LPMTQE Q%1d",QID),TString::Format("Hist LPMTQE Q%1d",QID),100,0,1);
        Hist_SPMTMu[QID ] = new TH1F(TString::Format("Hist SPMTMu Q%1d",QID),TString::Format("Hist SPMTMu Q%1d",QID),1000,0,10);
    }

    cout<<"Start reading"<<endl;
    for (int FileId = 0; FileId < 10; FileId++)
    {
        TString InputFileName = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/ChargeSpectrum/ChargeSpectrum%1d.root",FileId);
        TFile *inputfile = TFile::Open(InputFileName,"read");

        if (!inputfile) continue;

        TTree *charge = (TTree*)inputfile->Get("charge");
        vector<double> *CalibChargeLPMT = nullptr;
        vector<double> *TrueChargeLPMT = nullptr;
        vector<double> *ChargeSPMT = nullptr;
        double edep_x, edep_y, edep_z;
        double edep;

        charge->SetBranchAddress("LPMTCharge",&CalibChargeLPMT);
        charge->SetBranchAddress("TrueLPMTCharge",&TrueChargeLPMT);
        charge->SetBranchAddress("SPMTCharge",&ChargeSPMT);
        charge->SetBranchAddress("edep_x",&edep_x);
        charge->SetBranchAddress("edep_y",&edep_y);
        charge->SetBranchAddress("edep_z",&edep_z);
        charge->SetBranchAddress("edep",&edep);

        for (int entry = 0; entry < charge->GetEntries(); entry++)
        {
            charge->GetEntry(entry);
            Vector3D Edep_position;
            Edep_position.x = edep_x;
            Edep_position.y = edep_y;
            Edep_position.z = edep_z;
            // int EnergyNum = 0;

            if (Norm(Edep_position) > 15573)
            {
                // cout<<"Edep_position"<<endl;
                continue;
            }

            Vector3D ImagePoint[590];
            GetNodeImage(ImagePoint,Edep_position,NodePosition);
            // for (int i = 0; i < 10; i++)
            // {
            //     if (edep < QuantilesE[i])
            //     {
            //         EnergyNum = i;
            //         break;
            //     }
            // }
            
            for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
            {
                if ((CorrespondingSPMT[LPMTID][0] < 300000))
                {
                    continue;
                }
                
                double Theta = 0;
                Theta = GetTheta(LPMTPosition[LPMTID],Edep_position);
                // cout<<"Theta = "<<Theta<<endl;
                if (Theta > 40) 
                {
                    // cout<<"Theta"<<endl;
                    continue;
                }

                double R;
                R = GetR(LPMTPosition[LPMTID],Edep_position);
                double DistanceToImage;
                double Phi;
                GetDtoImage(DistanceToImage,Phi,LPMTPosition[LPMTID],ImagePoint,Edep_position);

                if ((Theta<=10 && DistanceToImage<1000) || (Theta<=20 && Theta>10 && DistanceToImage<2500) || (Theta<=40 && Theta>20 && DistanceToImage<3500))
                {
                    continue;
                }

                // int RNum = 0;
                int QNum = 0;
                for (int i = 0; i < 20; i++)
                {
                    if (edep/(R*R) < Quantiles[i])
                    {
                        break;
                    }
                    
                    QNum++;
                    // if (R < QuantilesR[i])
                    // {
                    //     DistanceNum = i;
                    //     break;
                    // }
                }

                // LPMTMu中Fill选中的LPMT_Event对
                Sta_LPMTMu[QNum]->Fill((*CalibChargeLPMT)[LPMTID]);
                Sta_TrueLPMTMu[QNum]->Fill((*TrueChargeLPMT)[LPMTID]);

                Hist_LPMTMu[QNum]->Fill((*CalibChargeLPMT)[LPMTID]);
                Hist_TrueLPMTMu[QNum]->Fill((*TrueChargeLPMT)[LPMTID]);
                // SPMT0中Fill选中的LPMT_Event对
                for (int ID = 0; ID < 6; ID++)
                {
                    if ((CorrespondingSPMT[LPMTID][ID] >= 300000))
                    {
                        Sta_SPMTDCR[QNum]->Fill(SPMTDCR[CorrespondingSPMT[LPMTID][ID] - 300000]);
                        Sta_SPMTQE[QNum]->Fill(SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000]);
                        Sta_LPMTQE[QNum]->Fill(LPMTQE[LPMTID]);

                        Hist_SPMTDCR[QNum]->Fill(SPMTDCR[CorrespondingSPMT[LPMTID][ID] - 300000]);
                        Hist_SPMTQE[QNum]->Fill(SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000]);
                        Hist_LPMTQE[QNum]->Fill(LPMTQE[LPMTID]);
                        Hist_SPMTMu[QNum]->Fill((*ChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000]);
                        if ((*ChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000]==0)
                        {
                            SPMT0[QNum]++;
                        }
                    }
                }
            }
        }

        inputfile->Close();
        cout<<"File "<<FileId<<" finished"<<endl;
    }

    cout<<"Start analyzing"<<endl;
    for (int QNum = 0; QNum < 21; QNum++)
    {
        double LPMTMu = Sta_LPMTMu[QNum]->GetMean();
        double LPMTMu_Error = Sta_LPMTMu[QNum]->GetMeanErr();
        double TrueLPMTMu = Sta_TrueLPMTMu[QNum]->GetMean();
        double TrueLPMTMu_Error = Sta_TrueLPMTMu[QNum]->GetMeanErr();

        double AverageSPMTQE = Sta_SPMTQE[QNum]->GetMean();
        double AverageLPMTQE = Sta_LPMTQE[QNum]->GetMean();
        DataWithError AverageSPMTDCR;
        AverageSPMTDCR.Data = Sta_SPMTDCR[QNum]->GetMean();
        AverageSPMTDCR.Error = 0;
        DataWithError RoughMu = GetP0Mu(Sta_SPMTQE[QNum]->GetN(),SPMT0[QNum]);
        DataWithError TrueMu = GetTrueCharge(RoughMu,AverageSPMTDCR);

        ToGraph_TotalLPMTMu.push_back(LPMTMu);
        ToGraph_TotalLPMTMuErr.push_back(LPMTMu_Error);
        ToGraph_TotalTrueLPMTMu.push_back(TrueLPMTMu);
        ToGraph_TotalTrueLPMTMuErr.push_back(TrueLPMTMu_Error);
        ToGraph_TotalSPMTMu_QE.push_back(TrueMu.Data/(AverageSPMTQE/AverageLPMTQE));
        ToGraph_TotalSPMTMu_QEErr.push_back(TrueMu.Error/(AverageSPMTQE/AverageLPMTQE));

        cout<<"Q"<<QNum<<" finished"<<endl;
    }

    TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/npeLPMTtonpeSPMT_B12.root","RECREATE");
    for (int QNum = 0; QNum < 21; QNum++)
    {
        outputfile->mkdir(TString::Format("%1d",QNum));
        outputfile->cd(TString::Format("%1d",QNum));
        Hist_LPMTMu[QNum]->Write();
        Hist_TrueLPMTMu[QNum]->Write();
        Hist_SPMTDCR[QNum]->Write();
        Hist_SPMTQE[QNum]->Write();
        Hist_LPMTQE[QNum]->Write();
        Hist_SPMTMu[QNum]->Write();
    }
    

    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TString GraphName = "LPMT #mu to SPMT #mu";
    npeLPMTtonpeSPMT_tot = new TGraphErrors(ToGraph_TotalLPMTMu.size(),&ToGraph_TotalSPMTMu_QE[0],&ToGraph_TotalLPMTMu[0],&ToGraph_TotalSPMTMu_QEErr[0],&ToGraph_TotalLPMTMuErr[0]);
    npeLPMTtonpeSPMT_tot->SetTitle(GraphName);
    npeLPMTtonpeSPMT_tot->SetName(GraphName);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu_{SPMT}/(QE_{SPMT}/PDE_{LPMT})");
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu_{LPMT}");
    // npeLPMTtonpeSPMT_tot->SetTitle("");
    npeLPMTtonpeSPMT_tot->SetFillStyle(3002);
    npeLPMTtonpeSPMT_tot->SetMarkerColor(4);
    npeLPMTtonpeSPMT_tot->SetLineColor(4);
    npeLPMTtonpeSPMT_tot->SetMarkerSize(1.0);
    npeLPMTtonpeSPMT_tot->SetMarkerStyle(20);
    npeLPMTtonpeSPMT_tot->SetLineWidth(3);
    npeLPMTtonpeSPMT_tot->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleOffset(0.83);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleOffset(1.30);

    TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,0.02);
    Below1PE->SetParNames("k");
    Below1PE->SetLineColor(kRed);
    Below1PE->SetParameter(0,50);

    npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R F");
    Below1PE->SetRange(0,10);

   
    npeLPMTtonpeSPMT_tot->Write();

    TGraphErrors *TruenpeLPMTtonpeSPMT_tot;
    TString GraphName_True = "True LPMT #mu to SPMT #mu";
    TruenpeLPMTtonpeSPMT_tot = new TGraphErrors(ToGraph_TotalTrueLPMTMu.size(),&ToGraph_TotalSPMTMu_QE[0],&ToGraph_TotalTrueLPMTMu[0],&ToGraph_TotalSPMTMu_QEErr[0],&ToGraph_TotalTrueLPMTMuErr[0]);
    TruenpeLPMTtonpeSPMT_tot->SetTitle(GraphName_True);
    TruenpeLPMTtonpeSPMT_tot->SetName(GraphName_True);
    TruenpeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu_{SPMT}/(QE_{SPMT}/PDE_{LPMT})");
    TruenpeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu_{LPMT}");
    TruenpeLPMTtonpeSPMT_tot->SetFillStyle(3002);
    TruenpeLPMTtonpeSPMT_tot->SetMarkerColor(5);
    TruenpeLPMTtonpeSPMT_tot->SetLineColor(5);
    TruenpeLPMTtonpeSPMT_tot->SetMarkerSize(1.0);
    TruenpeLPMTtonpeSPMT_tot->SetMarkerStyle(20);
    TruenpeLPMTtonpeSPMT_tot->SetLineWidth(3);

    TruenpeLPMTtonpeSPMT_tot->Write();

    TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    legendb->SetTextFont(22);
    legendb->AddEntry(npeLPMTtonpeSPMT_tot,"ElecSim LPMT #mu","lpe");
    legendb->AddEntry(TruenpeLPMTtonpeSPMT_tot,"True LPMT #mu","lpe"); 
    
    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->cd();
    npeLPMTtonpeSPMT_tot->Draw("A P");
    TruenpeLPMTtonpeSPMT_tot->Draw("P");
    legendb->Draw("same");

    c1->Write();


    return 0;
}
