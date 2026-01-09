struct DataWithError {
    double Data;
    double Error;
};

struct Vector3D {
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

DataWithError CombineResult(const vector<double>& x, const vector<double>& e) {
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

double combined_measurement(const vector<double>& x, const vector<double>& e) {
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

double combined_error(const vector<double>& e) {
    double sum_w = 0.0;

    for (int i = 0; i < e.size(); i++) {
        double w = 1 / (e[i] * e[i]);
        sum_w += w;
    }

    double combined_e = 1 / sqrt(sum_w);
    return combined_e;
}

int GetCorrespondingSPMT(int (&CorrespondingSPMT)[17612][6]) {
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map_J23.1.0-rc2.txt");
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

double GetLPMTPosition(Vector3D (&LPMTPosition)[17612])
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
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

double GetSPMTQE(double (&SPMTQE)[25600]) {
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
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

double GetLPMTQE(double (&LPMTQE)[17612]) {
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
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

double GetSPMTDCR(double (&SPMTDCR)[25600]) {
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
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

double GetDtoBar(double (&DtoBar)[17612]) {
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/PMTID_map_J23_1_0_rc2.txt");
    string line;
    int currentLine = 0;
    int BarID;
    double Distance;

    while (getline(file, line)) { 
        stringstream ss(line);
        int LPMTID;
        ss >> LPMTID >> BarID >> Distance;
        DtoBar[LPMTID] = Distance;
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

const int NR = 30;
const int NTheta = 6;
const int ND = 25;
const int NPhi = 36;

const double MaxD = 5;

void GetShadowingEffect_NoBar(const int RID, const int ThetaID) {
    // 初始化要用到的PMT参数
    double LPMTQE[17612];
    double SPMTQE[25600];
    double SPMTDCR[25600];
    double DtoBar[17612];
    int CorrespondingSPMT[17612][6];
    Vector3D NodePosition[590];
    Vector3D LPMTPosition[17612];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    GetNodePosition(NodePosition);
    GetLPMTPosition(LPMTPosition);
    // GetSPMTDCR(SPMTDCR);
    // GetDtoBar(DtoBar);

    // 得到每一个LPMT在有遮光效应、探测器均匀分布时的刻度系数
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ChargeSpectrum_NoBar.root","read");
    TTree *charge = (TTree*)inputfile->Get("charge");
    vector<double> *LPMTCharge; 
    vector<double> *SPMTCharge;
    double edep_x, edep_y, edep_z;
    double edepEnergy;
    charge->SetBranchAddress("TrueLPMTCharge",&LPMTCharge);
    charge->SetBranchAddress("TrueSPMTCharge",&SPMTCharge);
    charge->SetBranchAddress("edep_x",&edep_x);
    charge->SetBranchAddress("edep_y",&edep_y);
    charge->SetBranchAddress("edep_z",&edep_z);
    charge->SetBranchAddress("edep",&edepEnergy);

    TStatistic *LPMTMu_QE_Total[ND][NPhi];
    TStatistic *AverageSPMTMu_QE_Total[ND][NPhi];

    for (int DID = 0; DID < ND; DID++)
    {
        for (int PhiID = 0; PhiID < NPhi; PhiID++)
        {
            LPMTMu_QE_Total[DID][PhiID] = new TStatistic(TString::Format("LPMTMu_QE D%1d_Phi%1d",DID,PhiID));
            AverageSPMTMu_QE_Total[DID][PhiID] = new TStatistic(TString::Format("AverageSPMTMu_QE D%1d_Phi%1d",DID,PhiID));
        }
    }

    double RMin = pow(3861,3);
    double RStep = (pow(33273,3) - pow(3861,3))/NR;
    double ThetaMin = 0;
    double ThetaStep = 60.0/NTheta;

    int SelectedPairs = 0;
    int SelectedPairsA = 0;
    int SelectedPairsB = 0;
    int TotalPairs = 0;
    for (int Entry = 0; Entry<charge->GetEntries(); Entry++)
    {
        charge->GetEntry(Entry);
        Vector3D ImagePoint[590];
        Vector3D Edep_Position;
        Edep_Position.x = edep_x;
        Edep_Position.y = edep_y;
        Edep_Position.z = edep_z;
        GetNodeImage(ImagePoint,Edep_Position,NodePosition);
        for (int LPMTID = 0; LPMTID<17612; LPMTID++)
        {
            double Theta = GetTheta(LPMTPosition[LPMTID],Edep_Position);
            double R = GetR(LPMTPosition[LPMTID],Edep_Position);
            R = pow(R,3);
            double DistanceToImage;
            double Phi;
            if ( Theta>=(ThetaID*ThetaStep + ThetaMin) && Theta<((ThetaID+1)*ThetaStep + ThetaMin) &&  R >=(RID*RStep + RMin) && R <((RID+1)*RStep + RMin) )
            {
                GetDtoImage(DistanceToImage,Phi,LPMTPosition[LPMTID],ImagePoint,Edep_Position);
                int DID = (int)(DistanceToImage/(MaxD*1000./ND));
                int PhiID = (int)(Phi/(360./NPhi));
                if (DID >= ND)
                {
                    cout<<"Distance > MaxD, Distance="<<DistanceToImage<<endl;
                    continue;
                }
                

                LPMTMu_QE_Total[DID][PhiID]->Fill((*LPMTCharge)[LPMTID]/LPMTQE[LPMTID]);
                for (int ID = 0; ID < 6; ID++)
                {
                    if (CorrespondingSPMT[LPMTID][ID] >= 300000)
                    {
                        AverageSPMTMu_QE_Total[DID][PhiID]->Fill((*SPMTCharge)[CorrespondingSPMT[LPMTID][ID] - 300000]/SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000]);
                    }
                }
                SelectedPairs++;
            }
            else
            {
                continue;
            }

            TotalPairs++;
            if ((Theta<=20 && DistanceToImage>1000) || (Theta<=30 && Theta>20 && DistanceToImage>1500) || (Theta<=40 && Theta>30 && DistanceToImage>2000))
            {
                if (CorrespondingSPMT[LPMTID][0] >= 300000)
                {
                    SelectedPairsA++;
                }
                
            }
            if ((Theta<=10 && DistanceToImage>1000) || (Theta<=20 && Theta>10 && DistanceToImage>2500) || (Theta<=40 && Theta>20 && DistanceToImage>3500))
            {
                if (CorrespondingSPMT[LPMTID][0] >= 300000)
                {
                    SelectedPairsB++;
                }
            }
            
            
        }
        if (Entry%1000 == 0)
        {
            cout<<Entry<<" Finished"<<endl;
        }
        
    }
    cout<<"TotalPairs = "<<TotalPairs<<endl;
    // cout<<"SelectedPairs = "<<SelectedPairs<<endl;
    cout<<"SelectedPairsA = "<<SelectedPairsA<<endl;
    cout<<"SelectedPairsB = "<<SelectedPairsB<<endl;

    TFile *outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ShadowingEffectHist/NoBar/R%1d_Theta%1d.root",RID,ThetaID),"recreate");
    TString HistName = TString::Format("R=[%.1f,%.1f) #theta=[%.1f,%.1f)",pow((RID*RStep + RMin),1.0/3.0)/1000.,pow(((RID+1)*RStep + RMin),1.0/3.0)/1000.,(ThetaID*ThetaStep + ThetaMin),((ThetaID+1)*ThetaStep + ThetaMin));
    TH2* CalibFactorHist = new TH2D("CalibFactor",HistName,ND, 0.0, MaxD, NPhi, 0.0, 360.0);
    CalibFactorHist->GetXaxis()->SetTitle("Distance to image/m");
    CalibFactorHist->GetYaxis()->SetTitle("#phi/DEG");
    CalibFactorHist->GetYaxis()->SetLabelFont( 63 );
    CalibFactorHist->GetYaxis()->SetLabelSize( 11 );

    TH2* LPMTEventNum = new TH2D("LPMTEventNum",HistName,ND, 0.0, MaxD, NPhi, 0.0, 360.0);
    LPMTEventNum->GetXaxis()->SetTitle("Distance to image/m");
    LPMTEventNum->GetYaxis()->SetTitle("#phi/DEG");
    LPMTEventNum->GetYaxis()->SetLabelFont( 63 );
    LPMTEventNum->GetYaxis()->SetLabelSize( 11 );
    
    for (int DID = 0; DID < ND; DID++)
    {
        for (int PhiID = 0; PhiID < NPhi; PhiID++)
        {
            double CalibFactor = LPMTMu_QE_Total[DID][PhiID]->GetMean()/AverageSPMTMu_QE_Total[DID][PhiID]->GetMean();
            double CalibFactorError = CalibFactor*sqrt(pow(LPMTMu_QE_Total[DID][PhiID]->GetMeanErr()/LPMTMu_QE_Total[DID][PhiID]->GetMean(),2)+pow(AverageSPMTMu_QE_Total[DID][PhiID]->GetMeanErr()/AverageSPMTMu_QE_Total[DID][PhiID]->GetMean(),2));
            if (AverageSPMTMu_QE_Total[DID][PhiID]->GetMean() > 0)
            {
                CalibFactorHist->SetBinContent(DID+1,PhiID+1,CalibFactor);
                CalibFactorHist->SetBinError(DID+1,PhiID+1,CalibFactorError);
                LPMTEventNum->SetBinContent(DID+1,PhiID+1,LPMTMu_QE_Total[DID][PhiID]->GetN());
                LPMTMu_QE_Total[DID][PhiID]->Write();
                AverageSPMTMu_QE_Total[DID][PhiID]->Write();
            }
        }
    }
    CalibFactorHist->Write();
    LPMTEventNum->Write();

    outputfile->Close();

}