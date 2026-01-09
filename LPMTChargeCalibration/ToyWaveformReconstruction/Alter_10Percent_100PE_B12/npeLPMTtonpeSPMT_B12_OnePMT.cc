const double QuantilesR[24] = {6000, 7000, 8000, 9000, 11830, 14778.3, 16948.3, 18591.7, 20054, 21497.5, 22676, 23890, 24942, 25821.7, 26645, 27396.7, 28168.3, 28955, 29728, 30610, 31603.3, 32616.7, 33966.7, 39990};

const double QuantilesE[10] = {3.4449470,4.7075471,5.6179705,6.3958938,7.123,7.8501062,8.6280295,9.5384529,10.801053,100};

const int RNums = 24;
const int ENums = 10;

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
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/PMTID_map.txt");
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

int npeLPMTtonpeSPMT_B12_OnePMT()
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

    // // 定义输出文件及输出格式
    // TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/npeLPMTtonpeSPMT_B12.root","RECREATE");
    // TTree *Data = new TTree("Data","Data");

    // vector<vector<double>> MuSPMT;
    // vector<vector<double>> MuSPMTError;

    // vector<vector<double>> P0SPMT;
    // vector<vector<double>> P0SPMTError;
    
    // vector<double> AverageMuSPMT_QE;
    // vector<double> MuLPMT; //需要存储17612个数据，每个vector长20*10
    // vector<double> TrueMuLPMT;

    // vector<double> AverageMuSPMT_QEError;
    // vector<double> MuLPMTError;
    // vector<double> TrueMuLPMTError;
    // int LPMTID;

    // // 增加刻度系数、所有SPMT的Mu值及误差
    // Data->Branch("AverageMuSPMT_QE",&AverageMuSPMT_QE);
    // Data->Branch("MuLPMT",&MuLPMT);
    // Data->Branch("TrueMuLPMT",&TrueMuLPMT);
    // Data->Branch("AverageMuSPMT_QEError",&AverageMuSPMT_QEError);
    // Data->Branch("MuLPMTError",&MuLPMTError);
    // Data->Branch("TrueMuLPMTError",&TrueMuLPMTError);
    // Data->Branch("MuSPMT",&MuSPMT);
    // Data->Branch("MuSPMTError",&MuSPMTError);
    // Data->Branch("P0SPMT",&P0SPMT);
    // Data->Branch("P0SPMTError",&P0SPMTError);
    // Data->Branch("LPMTID",&LPMTID,"LPMTID/I");


    // 定义需要计算的数据，完成初始化
    vector<vector<vector<double>>> LPMTMu(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));  //LPMTMu[LPMTID][QR][QE]
    vector<vector<vector<double>>> LPMTMuErr(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));
    vector<vector<vector<double>>> Entries(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));

    vector<vector<vector<vector<int>>>> SPMT0(17612,vector<vector<vector<int>>>(RNums,vector<vector<int>>(ENums,vector<int>(6,0)))); //SPMT0[LPMTID][QR][QE][corruspondingID]
    // vector<vector<vector<vector<int>>>> SPMTTotal(17612,vector<vector<vector<int>>>(RNums,vector<vector<int>>(ENums,vector<int>(6,0))));

    vector<vector<vector<vector<double>>>> SPMTMu(17612,vector<vector<vector<double>>>(RNums,vector<vector<double>>(ENums,vector<double>(6,0))));
    vector<vector<vector<vector<double>>>> SPMTMuErr(17612,vector<vector<vector<double>>>(RNums,vector<vector<double>>(ENums,vector<double>(6,0))));

    vector<vector<vector<vector<double>>>> SPMTTrueMu(17612,vector<vector<vector<double>>>(RNums,vector<vector<double>>(ENums,vector<double>(6,0))));
    vector<vector<vector<vector<double>>>> SPMTTrueMuErr(17612,vector<vector<vector<double>>>(RNums,vector<vector<double>>(ENums,vector<double>(6,0))));

    vector<vector<vector<double>>> AverageSPMTMu(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));  //LPMTMu[LPMTID][QR][QE]
    vector<vector<vector<double>>> AverageSPMTMuErr(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));

    vector<vector<vector<double>>> AverageTrueSPMTMu(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));  //LPMTMu[LPMTID][QR][QE]
    vector<vector<vector<double>>> AverageTrueSPMTMuErr(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));

    vector<vector<vector<double>>> LPMTTrueMu(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));  //LPMTTrueMu[LPMTID][QR][QE]
    vector<vector<vector<double>>> LPMTTrueMuErr(17612,vector<vector<double>>(RNums,vector<double>(ENums,0)));

    vector<vector<double>> TotalLPMTMu_QE(RNums,vector<double>(ENums,0));
    vector<vector<double>> TotalLPMTMu_QEErr(RNums,vector<double>(ENums,0));

    vector<vector<double>> TotalTrueLPMTMu_QE(RNums,vector<double>(ENums,0));
    vector<vector<double>> TotalTrueLPMTMu_QEErr(RNums,vector<double>(ENums,0));

    vector<vector<double>> TotalSPMTMu_QE(RNums,vector<double>(ENums,0));
    vector<vector<double>> TotalSPMTMu_QEErr(RNums,vector<double>(ENums,0));

    vector<vector<double>> TotalTrueSPMTMu_QE(RNums,vector<double>(ENums,0));
    vector<vector<double>> TotalTrueSPMTMu_QEErr(RNums,vector<double>(ENums,0));

    vector<double> ToGraph_TotalLPMTMu_QE;
    vector<double> ToGraph_TotalLPMTMu_QEErr;

    vector<double> ToGraph_TotalTrueLPMTMu_QE;
    vector<double> ToGraph_TotalTrueLPMTMu_QEErr;

    vector<double> ToGraph_TotalSPMTMu_QE;
    vector<double> ToGraph_TotalSPMTMu_QEErr;

    vector<double> ToGraph_TotalTrueSPMTMu_QE;
    vector<double> ToGraph_TotalTrueSPMTMu_QEErr;

    cout<<"Start reading"<<endl;
    for (int FileId = 0; FileId < 10; FileId++)
    {
        TString InputFileName = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/ChargeSpectrum/ChargeSpectrum%1d.root",FileId);
        TFile *inputfile = TFile::Open(InputFileName,"read");

        if (!inputfile) continue;

        TTree *charge = (TTree*)inputfile->Get("charge");
        vector<double> *CalibChargeLPMT;
        vector<double> *TrueChargeLPMT;
        vector<double> *ChargeSPMT;
        vector<double> *TrueChargeSPMT;
        double edep_x, edep_y, edep_z;
        double edep;

        charge->SetBranchAddress("LPMTCharge",&CalibChargeLPMT);
        charge->SetBranchAddress("TrueLPMTCharge",&TrueChargeLPMT);
        charge->SetBranchAddress("SPMTCharge",&ChargeSPMT);
        charge->SetBranchAddress("TrueSPMTCharge",&TrueChargeSPMT);
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

            if (Norm(Edep_position) > 15573)
            {
                continue;
            }

            Vector3D ImagePoint[590];
            GetNodeImage(ImagePoint,Edep_position,NodePosition);
            
            int EnergyNum = 0;
            for (int i = 0; i < ENums; i++)
            {
                if (edep < QuantilesE[i])
                {
                    EnergyNum = i;
                    break;
                }
            }
            
            for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
            {
                if ((CorrespondingSPMT[LPMTID][0] < 300000))
                {
                    continue;
                }

                // 计算事例与LPMT间的theta角
                double Theta = 0;
                Theta = GetTheta(LPMTPosition[LPMTID],Edep_position);
                // cout<<"Theta = "<<Theta<<endl;
                if (Theta > 40) continue;

                double R;
                R = GetR(LPMTPosition[LPMTID],Edep_position);
                double DistanceToImage;
                double Phi;
                GetDtoImage(DistanceToImage,Phi,LPMTPosition[LPMTID],ImagePoint,Edep_position);

                if ((Theta<=20 && DistanceToImage<1000) || (Theta<=30 && Theta>20 && DistanceToImage<1500) || (Theta<=40 && Theta>30 && DistanceToImage<2000))
                {
                    continue;
                }

                // 计算事例与LPMT之间的距离
                double distance;
                distance = GetR(LPMTPosition[LPMTID],Edep_position);

                int DistanceNum = 0;
                for (int i = 0; i < RNums; i++)
                {
                    if (distance < QuantilesR[i])
                    {
                        DistanceNum = i;
                        break;
                    }
                }

                // LPMTMu中Fill选中的LPMT_Event对
                LPMTMu[LPMTID][DistanceNum][EnergyNum]  += (*CalibChargeLPMT)[LPMTID];
                LPMTMuErr[LPMTID][DistanceNum][EnergyNum] += (*CalibChargeLPMT)[LPMTID]*(*CalibChargeLPMT)[LPMTID];
                Entries[LPMTID][DistanceNum][EnergyNum]++;
                // cout<<"Select an event_LPMT"<<endl;

                // SPMT0中Fill选中的LPMT_Event对
                for (int ID = 0; ID < 6; ID++)
                {
                    // if ((CorrespondingSPMT[LPMTID][ID] >= 300000) && ((*ChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000]==0) )
                    // {
                    //     SPMT0[LPMTID][DistanceNum][EnergyNum][ID]++;
                    // }

                    if ((CorrespondingSPMT[LPMTID][ID] >= 300000))
                    {
                        if ((*ChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000]==0) SPMT0[LPMTID][DistanceNum][EnergyNum][ID]++;
                        SPMTTrueMu[LPMTID][DistanceNum][EnergyNum][ID] += (*TrueChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000];
                        SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID] += (*TrueChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000]*(*TrueChargeSPMT)[CorrespondingSPMT[LPMTID][ID] - 300000];
                    }
                    
                }
                
                // LPMTTrueMu中Fill选中的LPMT_Event对
                LPMTTrueMu[LPMTID][DistanceNum][EnergyNum]  += (*TrueChargeLPMT)[LPMTID];
                LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum] += (*TrueChargeLPMT)[LPMTID]*(*TrueChargeLPMT)[LPMTID];
            }
        }

        inputfile->Close();
        cout<<"File "<<FileId<<" finished"<<endl;
    }

    cout<<"Start analyzing"<<endl;
    for (int DistanceNum = 0; DistanceNum < RNums; DistanceNum++)
    {
        for (int EnergyNum = 0; EnergyNum < ENums; EnergyNum++)
        {
            double SelectLPMT = 0;
            double SelectEntries = 0;
            for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
            {
                // 得到正确的LPMTMu、SPMTMu、SPMTP0、LPMTTrueMu
                if (Entries[LPMTID][DistanceNum][EnergyNum] > 0)
                {
                    LPMTMu[LPMTID][DistanceNum][EnergyNum] /= Entries[LPMTID][DistanceNum][EnergyNum];
                    LPMTMuErr[LPMTID][DistanceNum][EnergyNum] /= Entries[LPMTID][DistanceNum][EnergyNum];
                    LPMTMuErr[LPMTID][DistanceNum][EnergyNum] -= LPMTMu[LPMTID][DistanceNum][EnergyNum]*LPMTMu[LPMTID][DistanceNum][EnergyNum];
                    LPMTMuErr[LPMTID][DistanceNum][EnergyNum] = sqrt(LPMTMuErr[LPMTID][DistanceNum][EnergyNum]/Entries[LPMTID][DistanceNum][EnergyNum]);

                    LPMTTrueMu[LPMTID][DistanceNum][EnergyNum] /= Entries[LPMTID][DistanceNum][EnergyNum];
                    LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum] /= Entries[LPMTID][DistanceNum][EnergyNum];
                    LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum] -= LPMTTrueMu[LPMTID][DistanceNum][EnergyNum]*LPMTTrueMu[LPMTID][DistanceNum][EnergyNum];
                    LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum] = sqrt(LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum]/Entries[LPMTID][DistanceNum][EnergyNum]);

                    double SelectSPMTNum = 0;
                    for (int ID = 0; ID < 6; ID++)
                    {
                        if (CorrespondingSPMT[LPMTID][ID] >= 300000)
                        {
                            SPMTTrueMu[LPMTID][DistanceNum][EnergyNum][ID] /= Entries[LPMTID][DistanceNum][EnergyNum];
                            SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID] /= Entries[LPMTID][DistanceNum][EnergyNum];
                            SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID] -= SPMTTrueMu[LPMTID][DistanceNum][EnergyNum][ID]*SPMTTrueMu[LPMTID][DistanceNum][EnergyNum][ID];
                            SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID] = sqrt(SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID]/Entries[LPMTID][DistanceNum][EnergyNum]);

                            AverageTrueSPMTMu[LPMTID][DistanceNum][EnergyNum] += SPMTTrueMu[LPMTID][DistanceNum][EnergyNum][ID];
                            AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum] += SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID]*SPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum][ID];
                            // SelectSPMTNum += 1;

                            if (SPMT0[LPMTID][DistanceNum][EnergyNum][ID] > 0 && SPMT0[LPMTID][DistanceNum][EnergyNum][ID] != Entries[LPMTID][DistanceNum][EnergyNum])
                            {
                                DataWithError RC;
                                DataWithError TC;
                                DataWithError DarkRate;
                                DarkRate.Data = SPMTDCR[CorrespondingSPMT[LPMTID][ID] - 300000];
                                DarkRate.Error = 0;
                                RC = GetP0Mu(Entries[LPMTID][DistanceNum][EnergyNum],SPMT0[LPMTID][DistanceNum][EnergyNum][ID]);
                                TC = GetTrueCharge(RC,DarkRate);

                                if (TMath::IsNaN(TC.Data) )
                                {
                                    cout<<"Can't find a root!!!"<<endl;
                                    cout<<"PMTID="<<CorrespondingSPMT[LPMTID][ID]<<"; RC="<<RC.Data<<";  RC.Error="<<RC.Error<<";  DCR="<<DarkRate.Data<<"; n0="<<SPMT0[LPMTID][DistanceNum][EnergyNum][ID]<<"; n="<<Entries[LPMTID][DistanceNum][EnergyNum]<<endl;
                                    SPMTMu[LPMTID][DistanceNum][EnergyNum][ID] = -1;
                                    SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID] = -1;
                                }
                                else
                                {
                                    SPMTMu[LPMTID][DistanceNum][EnergyNum][ID] = TC.Data/SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000];
                                    SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID] = TC.Error/SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000];
                                    SelectSPMTNum += 1;
                                    AverageSPMTMu[LPMTID][DistanceNum][EnergyNum] += SPMTMu[LPMTID][DistanceNum][EnergyNum][ID];
                                    AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum] += SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID]*SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID];
                                }
                            }
                            if (SPMT0[LPMTID][DistanceNum][EnergyNum][ID] == 0)
                            {
                                SPMTMu[LPMTID][DistanceNum][EnergyNum][ID] = 0;
                                SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID] = 0;
                                SelectSPMTNum += 1;
                                AverageSPMTMu[LPMTID][DistanceNum][EnergyNum] += SPMTMu[LPMTID][DistanceNum][EnergyNum][ID];
                                AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum] += SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID]*SPMTMuErr[LPMTID][DistanceNum][EnergyNum][ID];
                            }
                            
                        }
                        
                    }
                    if (SelectSPMTNum > 0)
                    {
                        AverageSPMTMu[LPMTID][DistanceNum][EnergyNum] /= SelectSPMTNum;
                        AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum] = sqrt(AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum])/SelectSPMTNum;
                        AverageTrueSPMTMu[LPMTID][DistanceNum][EnergyNum] /= SelectSPMTNum;
                        AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum] = sqrt(AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum])/SelectSPMTNum;
                    }
                    else
                    {
                        AverageSPMTMu[LPMTID][DistanceNum][EnergyNum] = -1;
                        AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum] = -1;
                        AverageTrueSPMTMu[LPMTID][DistanceNum][EnergyNum] = -1;
                        AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum] = -1;
                    }
                }
                else
                {
                    LPMTMu[LPMTID][DistanceNum][EnergyNum] = -1;
                    LPMTMuErr[LPMTID][DistanceNum][EnergyNum] = -1;
                    LPMTTrueMu[LPMTID][DistanceNum][EnergyNum] = -1;
                    LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum] = -1;
                    AverageSPMTMu[LPMTID][DistanceNum][EnergyNum] = -1;
                    AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum] = -1;
                    AverageTrueSPMTMu[LPMTID][DistanceNum][EnergyNum] = -1;
                    AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum] = -1;
                }

                // 计算平均LPMTMu、SPMTMu、SPMTP0、LPMTTrueMu，考虑QE
                if ( (LPMTMu[LPMTID][DistanceNum][EnergyNum] >= 0) && (AverageTrueSPMTMu[LPMTID][DistanceNum][EnergyNum]>=0))
                {
                    TotalLPMTMu_QE[DistanceNum][EnergyNum] += LPMTMu[LPMTID][DistanceNum][EnergyNum]/LPMTQE[LPMTID];
                    TotalLPMTMu_QEErr[DistanceNum][EnergyNum] += (LPMTMuErr[LPMTID][DistanceNum][EnergyNum]/LPMTQE[LPMTID])*(LPMTMuErr[LPMTID][DistanceNum][EnergyNum]/LPMTQE[LPMTID]);
                    
                    TotalSPMTMu_QE[DistanceNum][EnergyNum] += AverageSPMTMu[LPMTID][DistanceNum][EnergyNum];
                    TotalSPMTMu_QEErr[DistanceNum][EnergyNum] += AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum]*AverageSPMTMuErr[LPMTID][DistanceNum][EnergyNum];

                    TotalTrueSPMTMu_QE[DistanceNum][EnergyNum] += AverageTrueSPMTMu[LPMTID][DistanceNum][EnergyNum];
                    TotalTrueSPMTMu_QEErr[DistanceNum][EnergyNum] += AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum]*AverageTrueSPMTMuErr[LPMTID][DistanceNum][EnergyNum];

                    TotalTrueLPMTMu_QE[DistanceNum][EnergyNum] += LPMTTrueMu[LPMTID][DistanceNum][EnergyNum]/LPMTQE[LPMTID];
                    TotalTrueLPMTMu_QEErr[DistanceNum][EnergyNum] += (LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum]/LPMTQE[LPMTID])*(LPMTTrueMuErr[LPMTID][DistanceNum][EnergyNum]/LPMTQE[LPMTID]);
                    SelectLPMT += 1;
                    SelectEntries += Entries[LPMTID][DistanceNum][EnergyNum];
                }
            }
            
            // 平均LPMTMu、SPMTMu、SPMTP0、LPMTTrueMu，考虑QE
            if (SelectLPMT > 0)
            {
                TotalLPMTMu_QE[DistanceNum][EnergyNum] /= SelectLPMT;
                TotalLPMTMu_QEErr[DistanceNum][EnergyNum] = sqrt(TotalLPMTMu_QEErr[DistanceNum][EnergyNum])/SelectLPMT;

                TotalTrueLPMTMu_QE[DistanceNum][EnergyNum] /= SelectLPMT;
                TotalTrueLPMTMu_QEErr[DistanceNum][EnergyNum] = sqrt(TotalTrueLPMTMu_QEErr[DistanceNum][EnergyNum])/SelectLPMT;

                TotalSPMTMu_QE[DistanceNum][EnergyNum] /= SelectLPMT;
                TotalSPMTMu_QEErr[DistanceNum][EnergyNum] = sqrt(TotalSPMTMu_QEErr[DistanceNum][EnergyNum])/SelectLPMT;

                TotalTrueSPMTMu_QE[DistanceNum][EnergyNum] /= SelectLPMT;
                TotalTrueSPMTMu_QEErr[DistanceNum][EnergyNum] = sqrt(TotalTrueSPMTMu_QEErr[DistanceNum][EnergyNum])/SelectLPMT;

                ToGraph_TotalLPMTMu_QE.push_back(TotalLPMTMu_QE[DistanceNum][EnergyNum]);
                ToGraph_TotalLPMTMu_QEErr.push_back(TotalLPMTMu_QEErr[DistanceNum][EnergyNum]);
                ToGraph_TotalTrueLPMTMu_QE.push_back(TotalTrueLPMTMu_QE[DistanceNum][EnergyNum]);
                ToGraph_TotalTrueLPMTMu_QEErr.push_back(TotalTrueLPMTMu_QEErr[DistanceNum][EnergyNum]);
                ToGraph_TotalSPMTMu_QE.push_back(TotalSPMTMu_QE[DistanceNum][EnergyNum]);
                ToGraph_TotalSPMTMu_QEErr.push_back(TotalSPMTMu_QEErr[DistanceNum][EnergyNum]);
                ToGraph_TotalTrueSPMTMu_QE.push_back(TotalTrueSPMTMu_QE[DistanceNum][EnergyNum]);
                ToGraph_TotalTrueSPMTMu_QEErr.push_back(TotalTrueSPMTMu_QEErr[DistanceNum][EnergyNum]);

                cout<<"TotalLPMTMu_QE = "<<TotalLPMTMu_QE[DistanceNum][EnergyNum]<<"+-"<<TotalLPMTMu_QEErr[DistanceNum][EnergyNum]<<"  TotalTrueSPMTMu_QE = "<<TotalTrueSPMTMu_QE[DistanceNum][EnergyNum]<<"+-"<<TotalTrueSPMTMu_QEErr[DistanceNum][EnergyNum]<<endl;
                cout<<"SelectLPMT = "<<SelectLPMT<<"  SelectEntries = "<<SelectEntries<<endl;
            }

            cout<<"R"<<DistanceNum<<" E"<<EnergyNum<<" finished"<<endl;
        }
    }

    TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/npeLPMTtonpeSPMT_B12_OnePMT.root","RECREATE");

    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TString GraphName = "LPMT #mu to SPMT #mu";
    npeLPMTtonpeSPMT_tot = new TGraphErrors(ToGraph_TotalLPMTMu_QE.size(),&ToGraph_TotalSPMTMu_QE[0],&ToGraph_TotalLPMTMu_QE[0],&ToGraph_TotalSPMTMu_QEErr[0],&ToGraph_TotalLPMTMu_QEErr[0]);
    npeLPMTtonpeSPMT_tot->SetTitle(GraphName);
    npeLPMTtonpeSPMT_tot->SetName(GraphName);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu_{SPMT}/QE");
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu_{LPMT}/PDE");
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

    npeLPMTtonpeSPMT_tot->Write();

    TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,0.02);
    Below1PE->SetParNames("k");
    Below1PE->SetLineColor(kRed);
    Below1PE->SetParameter(0,50);

    npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R F");
    Below1PE->SetRange(0,10);

   
    // npeLPMTtonpeSPMT_tot->Write();

    TGraphErrors *TruenpeLPMTtonpeSPMT_tot;
    TString GraphName_True = "True LPMT #mu to SPMT #mu";
    TruenpeLPMTtonpeSPMT_tot = new TGraphErrors(ToGraph_TotalTrueLPMTMu_QE.size(),&ToGraph_TotalSPMTMu_QE[0],&ToGraph_TotalTrueLPMTMu_QE[0],&ToGraph_TotalSPMTMu_QEErr[0],&ToGraph_TotalTrueLPMTMu_QEErr[0]);
    TruenpeLPMTtonpeSPMT_tot->SetTitle(GraphName_True);
    TruenpeLPMTtonpeSPMT_tot->SetName(GraphName_True);
    TruenpeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu_{SPMT}/QE");
    TruenpeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu_{LPMT}/PDE");
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
    npeLPMTtonpeSPMT_tot->Draw("A L P");
    TruenpeLPMTtonpeSPMT_tot->Draw("P");
    legendb->Draw("same");

    c1->Write();


    return 0;
}
