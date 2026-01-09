struct DataWithError {
    double Data;
    double Error;
};

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

double GetSPMTQE(double (&SPMTQE)[25600]) {
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

double GetLPMTQE(double (&LPMTQE)[17612]) {
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

double GetSPMTDCR(double (&SPMTDCR)[25600]) {
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
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
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/PMTID_map.txt");
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

void GetTrueRatio() {
    // 初始化要用到的PMT参数
    double LPMTQE[17612];
    double SPMTQE[25600];
    double SPMTDCR[25600];
    double DtoBar[17612];
    int CorrespondingSPMT[17612][6];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    GetSPMTDCR(SPMTDCR);
    GetDtoBar(DtoBar);

    double CenterCalibFactor;
    double CenterCalibFactorError;

    TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/TrueRatio.root","recreate");
    TTree *TrueRatio = new TTree("TrueRatio","TrueRatio");
    TrueRatio->Branch("CenterCalibFactor",&CenterCalibFactor,"CenterCalibFactor/D");
    TrueRatio->Branch("CenterCalibFactorError",&CenterCalibFactorError,"CenterCalibFactorError/D");

    // 得到每一个LPMT在没有遮光效应、探测器中心事例时的刻度系数
    TStatistic *LPMTMeanQE_Sta[17612];
    TStatistic *SPMTMeanQE_Sta[25600];

    for (int ID = 0; ID < 17612; ID++) {
        LPMTMeanQE_Sta[ID] = new TStatistic(TString::Format("LPMT Mean_QE Chn%1d",ID));
    }
    
    for (int ID = 0; ID < 25600; ID++) {
        SPMTMeanQE_Sta[ID] = new TStatistic(TString::Format("SPMT Mean_QE Chn%1d",ID));
    }

    TFile *inputfile_NoBar = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ChargeSpectrum_DisableSticks.root","read");

    double LPMTCharge[17612]; 
    double SPMTCharge[25600]; 
    TTree *charge = (TTree*)inputfile_NoBar->Get("charge");
    charge->SetBranchAddress("LPMTCharge",LPMTCharge);
    charge->SetBranchAddress("SPMTCharge",SPMTCharge);

    for (int entry = 0; entry < charge->GetEntries(); entry++) {
        charge->GetEntry(entry);
        for (int LPMTID = 0; LPMTID < 17612; LPMTID++) {
            LPMTMeanQE_Sta[LPMTID]->Fill(LPMTCharge[LPMTID]/LPMTQE[LPMTID]);
        }
        for (int SPMTID = 0; SPMTID < 25600; SPMTID++) {
            SPMTMeanQE_Sta[SPMTID]->Fill(SPMTCharge[SPMTID]/SPMTQE[SPMTID]);
        }
    }
    inputfile_NoBar->Close();

    for (int LPMTID = 0; LPMTID < 17612; LPMTID++) {
        vector<double> ToGet_AverageMuSPMT_QE;
        vector<double> ToGet_AverageMuSPMT_QEError;
        for (int CorrespondingID = 0; CorrespondingID < 6; CorrespondingID++) {
            if (CorrespondingSPMT[LPMTID][CorrespondingID] >= 300000) {
                ToGet_AverageMuSPMT_QE.push_back(SPMTMeanQE_Sta[CorrespondingSPMT[LPMTID][CorrespondingID]-300000]->GetMean());
                ToGet_AverageMuSPMT_QEError.push_back(SPMTMeanQE_Sta[CorrespondingSPMT[LPMTID][CorrespondingID]-300000]->GetMeanErr());
            }
        }

        if (ToGet_AverageMuSPMT_QE.size()>0) {
            DataWithError AverageMuSPMT_QE = CombineResult(ToGet_AverageMuSPMT_QE,ToGet_AverageMuSPMT_QEError);
            double LPMTMuQE = LPMTMeanQE_Sta[LPMTID]->GetMean();
            double LPMTMuQEError = LPMTMeanQE_Sta[LPMTID]->GetMeanErr();
            CenterCalibFactor = LPMTMuQE/AverageMuSPMT_QE.Data;
            CenterCalibFactorError = CenterCalibFactor*sqrt((LPMTMuQEError*LPMTMuQEError)/(LPMTMuQE*LPMTMuQE)+(AverageMuSPMT_QE.Error*AverageMuSPMT_QE.Error)/(AverageMuSPMT_QE.Data*AverageMuSPMT_QE.Data));
        }
        else {
            CenterCalibFactor = -1;
            CenterCalibFactorError = -1;
        }
        outputfile->cd();
        TrueRatio->Fill();

        
        ToGet_AverageMuSPMT_QE.clear();
        ToGet_AverageMuSPMT_QEError.clear();
    }

    TrueRatio->Write();
    outputfile->Close();
    
}