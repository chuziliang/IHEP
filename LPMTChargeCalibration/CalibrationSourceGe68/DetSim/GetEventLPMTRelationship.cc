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

int GetLPMTType(int (&LPMTType)[17612])
{
    ifstream inputfile_LPMT;
    TString filename_LPMT="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTType_CD_LPMT.csv";
    inputfile_LPMT.open(filename_LPMT,ios::in);
    if(!inputfile_LPMT.good()) {cout<<"error"<<endl;return 0;}
    int PMTID;
    string PMTType;
    for (int i = 0; i < 17612; i++)
    {
        inputfile_LPMT>>PMTID>>PMTType;

        if (PMTType == "Hamamatsu")
        {
            LPMTType[PMTID] = 1;
        }
        else if (PMTType == "NNVT")
        {
            LPMTType[PMTID] = 2;
        }
        else if (PMTType == "HighQENNVT")
        {
            LPMTType[PMTID] = 3;
        }
        else
        {
            cout<<"Error Wrong Type 1!!"<<endl;
        }
        
    }

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

int GetEventLPMTRelationship(const int foldernum)
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/CalibrationSourceGe68/DetSim/folder_path.txt");
    string line;
    int currentLine = 0;

    while (getline(file, line)) 
    {
        if (currentLine == foldernum) 
        {
            break;
        }
        currentLine++;
    }

    std::stringstream ss(line);
    string position;
    Vector3D GeneratePosition;
    getline(ss, position, '_');
    GeneratePosition.x = stod(position);
    getline(ss, position, '_');
    GeneratePosition.y = stod(position);
    getline(ss, position, '_');
    GeneratePosition.z = stod(position);

    if (Norm(GeneratePosition) > 15573)
    {
        // cout<<"Edep_position"<<endl;
        return 1;
    }

    TString ChosenTypeString;
    
    // 初始化要用到的PMT参数
    Vector3D LPMTPosition[17612];
    Vector3D NodePosition[590];

    GetLPMTPosition(LPMTPosition);
    GetNodePosition(NodePosition);

    cout<<"Start reading"<<endl;

    string InputFileName = "/junofs/users/chuziliang125/LPMTChargeCalibration/CalibrationSourceGe68/DetSim/CombineChargeSpectrum/" + line +".root";
    // TString InputFileName = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Spallation_neutron/DetSim/ChargeSpectrum/ChargeSpectrum%1d.root",FileId);
    TFile *inputfile = TFile::Open((TString)InputFileName,"read");

    if (!inputfile) return 1;
   
    TTree *inputcharge = (TTree*)inputfile->Get("charge");
    // vector<double> *CalibChargeLPMT;
    vector<double> *TrueChargeLPMT = nullptr;
    vector<double> *ChargeSPMT = nullptr;
    double edep_x, edep_y, edep_z;
    double edep;

    // charge->SetBranchAddress("LPMTCharge",&CalibChargeLPMT);
    inputcharge->SetBranchAddress("TrueLPMTCharge",&TrueChargeLPMT);
    inputcharge->SetBranchAddress("TrueSPMTCharge",&ChargeSPMT);
    inputcharge->SetBranchAddress("edep_x",&edep_x);
    inputcharge->SetBranchAddress("edep_y",&edep_y);
    inputcharge->SetBranchAddress("edep_z",&edep_z);
    inputcharge->SetBranchAddress("edep",&edep);
    
    string OutputFileName = "/junofs/users/chuziliang125/LPMTChargeCalibration/CalibrationSourceGe68/DetSim/Event_LPMTRelationship/" + line +".root";
    TFile *outputfile = TFile::Open((TString)OutputFileName,"recreate");
    outputfile->cd();
    TTree *outputcharge = inputcharge->CloneTree();
    
    // TTree* outputcharge = new TTree("charge","DetSim charge and calib charge");
    vector<double> OutputR(17612,0);
    vector<double> OutputD(17612,0);
    vector<double> OutputTheta(17612,0);
    vector<double> OutputPhi(17612,0);

    // TBranch* branch1 = inputTree->Branch("newBranch1", &newBranch1, "newBranch1/D");
    // TBranch* branch2 = inputTree->Branch("newBranch2", &newBranch2, "newBranch2/D");
    // outputcharge->Branch("TrueLPMTCharge",TrueChargeLPMT);
    // outputcharge->Branch("TrueSPMTCharge",ChargeSPMT);
    TBranch* branch_R = outputcharge->Branch("R",&OutputR);
    TBranch* branch_D = outputcharge->Branch("D",&OutputD);
    TBranch* branch_Theta = outputcharge->Branch("Theta",&OutputTheta);
    TBranch* branch_Phi = outputcharge->Branch("Phi",&OutputPhi);
    // outputcharge->Branch("edep_x",&edep_x);
    // outputcharge->Branch("edep_y",&edep_y);
    // outputcharge->Branch("edep_z",&edep_z);
    // outputcharge->Branch("edep",&edep);

    for (int entry = 0; entry < inputcharge->GetEntries(); entry++)
    {
        inputcharge->GetEntry(entry);
        Vector3D Edep_position;
        Edep_position.x = edep_x;
        Edep_position.y = edep_y;
        Edep_position.z = edep_z;

        if (Norm(Edep_position) > 15573 || edep == 0)
        {
            OutputR = vector<double>(17612,0);
            OutputD = vector<double>(17612,0);
            OutputTheta = vector<double>(17612,0);
            OutputPhi = vector<double>(17612,0);
            branch_R->Fill(); 
            branch_D->Fill(); 
            branch_Theta->Fill(); 
            branch_Phi->Fill(); 
            // outputcharge->Fill();
            continue;
        }

        Vector3D ImagePoint[590];
        GetNodeImage(ImagePoint,Edep_position,NodePosition);
        
        for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
        {   
            double Theta = 0;
            Theta = GetTheta(LPMTPosition[LPMTID],Edep_position);

            double R;
            R = GetR(LPMTPosition[LPMTID],Edep_position);
            double DistanceToImage;
            double Phi;
            GetDtoImage(DistanceToImage,Phi,LPMTPosition[LPMTID],ImagePoint,Edep_position);
            
            OutputR[LPMTID] = R;
            OutputD[LPMTID] = DistanceToImage;
            OutputTheta[LPMTID] = Theta;
            OutputPhi[LPMTID] = Phi;
        }
        branch_R->Fill(); 
        branch_D->Fill(); 
        branch_Theta->Fill(); 
        branch_Phi->Fill(); 
        // outputcharge->Fill();
        cout<<entry<<endl;
    }

    outputcharge->Write();

    inputfile->Close();
    // outputcharge->Write();
    outputfile->Close();


    return 0;
}
