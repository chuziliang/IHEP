//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 

const int LPMTID = 8951;
// const int SPMTID[6] = {303780,304088,304089,304230,304231,324324};

struct Vector3D
{
    double x,y,z;
};

struct DataWithError
{
    double Data;
    double Error;
};

double DotProduct(Vector3D A, Vector3D B)//求两个向量的点乘
{
    double result;
    result = A.x*B.x + A.y*B.y + A.z*B.z;
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

double GetR(Vector3D PMT,Vector3D edep)
{
    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);
    double r = Norm(LPMT_edep);
    return r;
}

int GetCorrespondingSPMT(int (&SPMT_ID)[6], const int LPMT_ID)
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map.txt");
    string line;
    int currentLine = 0;

    while (getline(file, line)) { 
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            int firstData;
            ss >> firstData >> SPMT_ID[0] >> SPMT_ID[1] >> SPMT_ID[2] >> SPMT_ID[3] >> SPMT_ID[4] >> SPMT_ID[5];
            return firstData;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

double GetTheta(const int LPMT_ID,Vector3D edep)
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;
    int currentLine = 0;

    while (getline(file, line)) {
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            Vector3D PMT;
            double nun;
            double currentPMT;
            ss >> currentPMT >> PMT.x >> PMT.y >> PMT.z >> nun >> nun;
            if (currentPMT != LPMT_ID)
            {
                cout<<"Error!!!"<<endl;
            }
            
            Vector3D LPMT_edep;//PMT指向能量沉积点的向量
            LPMT_edep = VectorMinus(edep,PMT);
            Vector3D FZ;
            FZ.x = -PMT.x;
            FZ.y = -PMT.y;
            FZ.z = -PMT.z;
            double z = DotProduct(FZ,LPMT_edep)/Norm(FZ);
            double r = Norm(LPMT_edep);
            double angle = acos(z/r) * 180 / M_PI;
            file.close();
            return angle;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

double GetPhi(const int LPMT_ID, Vector3D edep)
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;
    int currentLine = 0;

    while (getline(file, line)) {
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            Vector3D PMT;
            double nun;
            ss >> nun >> PMT.x >> PMT.y >> PMT.z >> nun >> nun;
            Vector3D FX;//随动坐标系的x
            Vector3D FY;//随动坐标系的y
            FX.x = -PMT.x;
            FX.y = -PMT.y;
            FX.z = (pow(PMT.x, 2)+pow(PMT.y, 2))/PMT.z;
            FY.x = -PMT.y;
            FY.y = PMT.x;
            FY.z = 0;

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
            file.close();
            return angle;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

std::string readSpecificLine(int lineNumber) 
{
    std::ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231106_NewVersion_OffAxis/folder_path.txt");
    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) 
    {
        if (currentLine == lineNumber) 
        {
            return line;
        }
        currentLine++;
    }
    return ""; // 如果未找到指定行，则返回空字符串
}

double GetSPMTQE(int SPMTID)
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    PMTData->GetEntry(SPMTID-300000);
    if (PMTID == SPMTID)
    {
        return 100*Effiency;
    }
    else
    {
        cout<<"Wrong SPMTID!!!"<<endl;
        return 0;
    }
}

double GetLPMTQE(int LPMTID)
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    PMTData->GetEntry((Long64_t)LPMTID);
    if (PMTID == LPMTID)
    {
        return Effiency;
    }
    else
    {
        cout<<"Wrong LPMTID!!!"<<endl;
        return 0;
    }
}

DataWithError GetTrueFactor(int LPMTID)
{
    DataWithError TrueFactor;
    TrueFactor.Data = 0;
    TrueFactor.Error = 0;

    double CenterCalibFactor;
    double CenterCalibFactorError;
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/TrueRatio.root","read");
    TTree *TrueRatio = (TTree*)inputfile->Get("TrueRatio");
    TrueRatio->SetBranchAddress("CenterCalibFactor",&CenterCalibFactor);
    TrueRatio->SetBranchAddress("CenterCalibFactorError",&CenterCalibFactorError);

    TrueRatio->GetEntry(LPMTID);

    TrueFactor.Data = CenterCalibFactor;
    TrueFactor.Error = CenterCalibFactorError;
    return TrueFactor;
}

int npeLPMTtonpeSPMT()
{
    Vector3D LPMTPosition[17612];
    GetLPMTPosition(LPMTPosition);
    
    int SPMTID[6];
    if (GetCorrespondingSPMT(SPMTID,LPMTID) != LPMTID)
    {
        cerr<<"ERROR: Wrong PMT ID!"<<endl;
        return 0;
    }
    double LPMTEffiency = GetLPMTQE(LPMTID);
    double SPMTEffiency[6];
    for (int id = 0; id < 6; id++)
    {
        SPMTEffiency[id] = GetSPMTQE(SPMTID[id]);
    }
    cout<<"LPMTEffiency = "<<LPMTEffiency<<endl;
    for (int id = 0; id < 6; id++)
    {
        cout<<"SPMT "<<SPMTID[id]<<" effiency = "<<SPMTEffiency[id]<<endl;
    }
    vector<double> TotFactor;
    vector<double> TotFactorError;
    vector<double> TotTheta;
    vector<double> TotThetaError;
    vector<double> TotPhi;
    vector<double> TotPhiError;

    TFile *outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OtherLPMTs/RootFile/%1d.root",LPMTID),"recreate");
    
    for (int num = 0; num < 227; num++)
    {
        string inputname;
        string subname = readSpecificLine(num);
        inputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OtherLPMTs/CombineChargeSpectrum/"+to_string(LPMTID)+"/" + subname + ".root";
        TFile *inputfile = TFile::Open((TString)inputname,"read");
        TTree *charge = (TTree*)inputfile->Get("Charge");
        double LPMTnpe;
        double SPMTnpe[6];
        double Theta;
        double Phi;
        double Edep_x;
        double Edep_y;
        double Edep_z;
        charge->SetBranchAddress("LPMTCharge",&LPMTnpe);
        charge->SetBranchAddress("SPMTCharge",SPMTnpe);
        charge->SetBranchAddress("Theta",&Theta);
        charge->SetBranchAddress("Phi",&Phi);
        charge->SetBranchAddress("edep_x",&Edep_x);
        charge->SetBranchAddress("edep_y",&Edep_y);
        charge->SetBranchAddress("edep_z",&Edep_z);


        TStatistic *LPMTChargeSpectrum;
        string LPMTHistName = "LPMT charge spectrum" + subname;
        LPMTChargeSpectrum = new TStatistic((TString)LPMTHistName);
        TStatistic *SPMTChargeSpectrum[6];
        for (int id = 0; id < 6; id++)
        {
            string SPMTHistName = "SPMT charge spectrum" + subname + to_string(id);
            SPMTChargeSpectrum[id] = new TStatistic((TString)SPMTHistName);
        }
        TStatistic *ThetaSpectrum;
        string ThetaHistName = "Theta spectrum" + subname;
        ThetaSpectrum = new TStatistic((TString)ThetaHistName);
        TStatistic *PhiSpectrum;
        string PhiHistName = "Phi spectrum" + subname;
        PhiSpectrum = new TStatistic((TString)PhiHistName);

        TStatistic *Edep_xSpectrum;
        string Edep_xHistName = "Edep_x spectrum" + subname;
        Edep_xSpectrum = new TStatistic((TString)Edep_xHistName);
        TStatistic *Edep_ySpectrum;
        string Edep_yHistName = "Edep_y spectrum" + subname;
        Edep_ySpectrum = new TStatistic((TString)Edep_yHistName);
        TStatistic *Edep_zSpectrum;
        string Edep_zHistName = "Edep_z spectrum" + subname;
        Edep_zSpectrum = new TStatistic((TString)Edep_zHistName);

        outputfile->mkdir((TString)subname);
        outputfile->cd((TString)subname);
        TH1* LPMTHist = new TH1F((TString)LPMTHistName,(TString)LPMTHistName,1000,0,100);
        TH1* SPMTHist[6];
        for (int id = 0; id < 6; id++)
        {
            string SPMTHistName = "SPMT charge spectrum" + subname + to_string(id);
            SPMTHist[id] = new TH1F((TString)SPMTHistName,(TString)SPMTHistName,100,0,10);
        }

        for (int i = 0; i < charge->GetEntries(); i++)
        {
            charge->GetEntry(i);
            LPMTChargeSpectrum->Fill(LPMTnpe);
            LPMTHist->Fill(LPMTnpe);
            for (int id = 0; id < 6; id++)
            {
                SPMTChargeSpectrum[id]->Fill(SPMTnpe[id]);
                SPMTHist[id]->Fill(SPMTnpe[id]);
            }
            ThetaSpectrum->Fill(Theta);
            PhiSpectrum->Fill(Phi);
            Edep_xSpectrum->Fill(Edep_x);
            Edep_ySpectrum->Fill(Edep_y);
            Edep_zSpectrum->Fill(Edep_z);
        }
        double LPMTMean = LPMTChargeSpectrum->GetMean();
        double LPMTError = LPMTChargeSpectrum->GetMeanErr();
        double SPMTMean[6];
        double SPMTMeanError[6];
        Vector3D Edep_mean;
        Edep_mean.x = Edep_xSpectrum->GetMean();
        Edep_mean.y = Edep_ySpectrum->GetMean();
        Edep_mean.z = Edep_zSpectrum->GetMean();
        double TrueThetaMean = GetTheta(LPMTID,Edep_mean);
        double TruePhiMean = GetPhi(LPMTID,Edep_mean);
        double DistanceToCenter = Norm(Edep_mean);
        for (int id = 0; id < 6; id++)
        {
            SPMTMean[id] = SPMTChargeSpectrum[id]->GetMean();
            SPMTMeanError[id] = SPMTChargeSpectrum[id]->GetMeanErr();
        }
        double ThetaMean = ThetaSpectrum->GetMean();
        double ThetaMeanError = ThetaSpectrum->GetRMS();
        double PhiMean = PhiSpectrum->GetMean();
        double PhiMeanError = PhiSpectrum->GetMeanErr();
        
        double Factor = 0 ;
        double FactorError = 0;
        double NumberOfCorrespondingSPMTs = 0;
        for (int id = 0; id < 6; id++)
        {
            if (SPMTID[id] >= 300000)
            {
                double ThisFactor = (LPMTMean/LPMTEffiency)/(SPMTMean[id]/SPMTEffiency[id]);
                Factor += ThisFactor;
                FactorError += ThisFactor*ThisFactor *( (LPMTError/LPMTMean)*(LPMTError/LPMTMean)+(SPMTMeanError[id]/SPMTMean[id])*(SPMTMeanError[id]/SPMTMean[id]) );
                NumberOfCorrespondingSPMTs += 1.0;
            }
        }
        Factor /= NumberOfCorrespondingSPMTs;
        FactorError = sqrt(FactorError);
        FactorError /= NumberOfCorrespondingSPMTs;
        inputfile->Close();
        TotFactor.push_back(Factor);
        TotFactorError.push_back(FactorError);
        // TotTheta.push_back(ThetaMean);
        // TotThetaError.push_back(ThetaMeanError);
        // TotPhi.push_back(PhiMean);
        // TotPhiError.push_back(PhiMeanError);
        TotTheta.push_back(TrueThetaMean);
        TotThetaError.push_back(0);
        TotPhi.push_back(TruePhiMean);
        TotPhiError.push_back(0);
       
        
        

        cout<<num<<" Finished"<<endl;
        if ((ThetaMean < 35) && (Factor > 60))
        {
            cout<<"Position:"<<subname<<"  ThetaMean:"<<ThetaMean<<"  ThetaMeanError:"<<ThetaMeanError<<"  PhiMean:"<<PhiMean<<"  PhiMeanError"<<PhiMeanError<<endl;
            cout<<"Factor ="<<Factor<<endl;
            cout<<"Edep_x ="<<Edep_mean.x<<"  Edep_y ="<<Edep_mean.y<<"  Edep_z ="<<Edep_mean.z<<"  Distance ="<<DistanceToCenter<<endl;
            // for (int id = 0; id < 6; id++)
            // {
            //     cout<<LPMTMean/SPMTMean[id]<<" ";
            // }
            // cout<<endl;
            for (int id = 0; id < 6; id++)
            {
                if (SPMTID[id] >= 300000) cout<<(LPMTMean/LPMTEffiency)/(SPMTMean[id]/SPMTEffiency[id])<<" ";
            }
            cout<<endl;
        }
        
        delete LPMTChargeSpectrum;
        for (int ID = 0; ID < 6; ID++)
        {
            delete SPMTChargeSpectrum[ID];
        }
        delete ThetaSpectrum;
        delete PhiSpectrum;
        delete Edep_xSpectrum;
        delete Edep_ySpectrum;
        delete Edep_zSpectrum;

        outputfile->cd((TString)subname);
        LPMTHist->Write();
        for (int id = 0; id < 6; id++)
        {
            SPMTHist[id]->Write();
        }

        delete LPMTHist;
        for (int id = 0; id < 6; id++)
        {
            delete SPMTHist[id];
        }
        
    }
    outputfile->Close();

    DataWithError TrueFactor;
    TrueFactor = GetTrueFactor(LPMTID);
    
    TGraphErrors *Factor_positions = new TGraphErrors(TotFactor.size(),
                                                            &TotTheta[0],&TotFactor[0],
                                                            &TotThetaError[0],&TotFactorError[0]);

    TCanvas *c2 = new TCanvas("c2","c2",1);
    Factor_positions->SetFillStyle(3002);
    Factor_positions->SetMarkerSize(1.0);
    Factor_positions->SetMarkerStyle(20);
    Factor_positions->SetTitle(TString::Format("Calibration factor to Theta %1d",LPMTID));
    Factor_positions->GetXaxis()->SetTitle("Theta");
    Factor_positions->GetYaxis()->SetTitle("Calibration factor");

    // Factor_positions->Draw("a");
    Factor_positions->Draw("a P");

    TLine *l = new TLine(0,TrueFactor.Data,60,TrueFactor.Data);
    l->Draw("same");
    TLine *lh = new TLine(0,TrueFactor.Data+TrueFactor.Error,60,TrueFactor.Data+TrueFactor.Error);
    lh->SetLineStyle(2);
    lh->Draw("same");
    TLine *ll = new TLine(0,TrueFactor.Data-TrueFactor.Error,60,TrueFactor.Data-TrueFactor.Error);
    ll->SetLineStyle(2);
    ll->Draw("same");
    cout<<"TrueFactor = "<<TrueFactor.Data<<"TrueFactor error = "<<TrueFactor.Error<<endl;

    TCanvas *c1 = new TCanvas("c1","c1",1);
    
    TGraph2D *dt = new TGraph2D();
    
    for (Int_t N=0; N<TotFactor.size(); N++) {
        dt->SetPoint(N,TotTheta[N],TotPhi[N],TotFactor[N]);
    }
    dt->SetTitle(TString::Format("Calibration factor of different position %1d",LPMTID));
    dt->GetXaxis()->SetTitle("Theta");
    dt->GetYaxis()->SetTitle("Phi");
    dt->GetZaxis()->SetTitle("Calibration factor");
    gStyle->SetPalette(1);
    dt->SetMarkerStyle(20);
    dt->Draw("pcol");        // use "surf1" to generate the left picture


    return 1;
}