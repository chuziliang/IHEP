const int NR = 30;
const int NTheta = 6;
const int ND = 25;
const int NPhi = 36;

const double MaxD = 5;

const double RMin = pow(3861,3);
const double RStep = (pow(33273,3) - pow(3861,3))/NR;

struct DataWithError {
    double Data;
    double Error;
};

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

DataWithError GetTrueFactor()
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    int CorrespondingSPMT[17612][6];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);

    TStatistic *LPMTMeanQE_Sta = new TStatistic("LPMT Mean_QE");
    TStatistic *SPMTMeanQE_Sta = new TStatistic("SPMT Mean_QE");

    TFile *inputfile_NoBar = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ChargeSpectrum_DisableSticks.root","read");
    double LPMTCharge[17612]; 
    double SPMTCharge[25600]; 
    TTree *charge = (TTree*)inputfile_NoBar->Get("charge");
    charge->SetBranchAddress("LPMTCharge",LPMTCharge);
    charge->SetBranchAddress("SPMTCharge",SPMTCharge);

    for (int entry = 0; entry < charge->GetEntries(); entry++) {
        charge->GetEntry(entry);
        for (int LPMTID = 0; LPMTID < 17612; LPMTID++) {
            LPMTMeanQE_Sta->Fill(LPMTCharge[LPMTID]/LPMTQE[LPMTID]);
            for (int ID = 0; ID < 6; ID++)
            {
                if (CorrespondingSPMT[LPMTID][ID] >= 300000)
                {
                    SPMTMeanQE_Sta->Fill(SPMTCharge[CorrespondingSPMT[LPMTID][ID] - 300000]/SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000]);
                }
            }
        }
    }
    inputfile_NoBar->Close();

    DataWithError result;
    result.Data = LPMTMeanQE_Sta->GetMean()/SPMTMeanQE_Sta->GetMean();
    result.Error = result.Data*sqrt(pow(LPMTMeanQE_Sta->GetMeanErr()/LPMTMeanQE_Sta->GetMean(),2)+pow(SPMTMeanQE_Sta->GetMeanErr()/SPMTMeanQE_Sta->GetMean(),2));

    cout<<"True Factor = "<<result.Data<<"+-"<<result.Error<<endl;
    return result;
}

int GetCalibFactorToR_NoBar(const int CutLevel)
{
    DataWithError TrueFactor;
    // TrueFactor = GetTrueFactor();

    vector<double> CalibFactor;
    vector<double> R;
    vector<double> CalibFactor_Error;
    vector<double> R_Error;

    for (int RID = 0; RID < NR; RID++)
    {
        // TFile *inputfile ;
        TStatistic *Sta_LPMTMu_QE = new TStatistic("Sta_LPMTMu_QE");
        TStatistic *Sta_SPMTMu_QE = new TStatistic("Sta_SPMTMu_QE");
        TList *List_LPMTMu_QE = new TList();
        TList *List_SPMTMu_QE = new TList();
        for (int ThetaID = 0; ThetaID < NTheta; ThetaID++)
        {
            TString filename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ShadowingEffectHist/NoBar/R%1d_Theta%1d.root",RID,ThetaID);
            TFile *inputfile = TFile::Open(filename,"read");
            TH2* CalibFactorHist = (TH2*)inputfile->Get("CalibFactor");
            TH2* LPMTEventNum = (TH2*)inputfile->Get("LPMTEventNum");
            for (int DID = 0; DID < ND; DID++)
            {
                for (int PhiID = 0; PhiID < NPhi; PhiID++)
                {
                    if (CutLevel == 1)
                    {
                        if ((ThetaID==0 && DID<5) || (ThetaID==1 && DID<13) || ((ThetaID==2 || ThetaID==3) && DID<18) || ThetaID>3)
                        {
                            continue;
                        }
                    }
                    else if (CutLevel == 2)
                    {
                        if ((ThetaID==0 && DID<13) || (ThetaID==1 && DID<13) || ((ThetaID==2 || ThetaID==3) && DID<18) || ThetaID>3)
                        {
                            continue;
                        }
                    }
                    else if (CutLevel == 3)
                    {
                        if ((ThetaID==0 && PhiID<=25 && PhiID>=10 && DID<13) || (ThetaID==0 && (PhiID>25 || PhiID<10) && DID<7) || (ThetaID==1 && DID<13) || ((ThetaID==2 || ThetaID==3) && DID<18) || ThetaID>3)
                        {
                            continue;
                        }
                    }
                    else if (CutLevel == 4)
                    {
                        if ( (ThetaID!=0))
                        {
                            continue;
                        }
                    }

                    if (CalibFactorHist->GetBinContent(DID+1,PhiID+1) > 0)
                    {
                        TStatistic *Sta_Temp_LPMTMu_QE = (TStatistic*)inputfile->Get(TString::Format("LPMTMu_QE D%1d_Phi%1d",DID,PhiID));
                        TStatistic *Sta_Temp_SPMTMu_QE = (TStatistic*)inputfile->Get(TString::Format("AverageSPMTMu_QE D%1d_Phi%1d",DID,PhiID));

                        List_LPMTMu_QE->Add(Sta_Temp_LPMTMu_QE);
                        List_SPMTMu_QE->Add(Sta_Temp_SPMTMu_QE);

                        Sta_LPMTMu_QE->Merge(List_LPMTMu_QE);
                        Sta_SPMTMu_QE->Merge(List_SPMTMu_QE);

                        List_LPMTMu_QE->Clear();
                        List_SPMTMu_QE->Clear();

                        Sta_Temp_LPMTMu_QE->Delete();
                        Sta_Temp_SPMTMu_QE->Delete();
                        cout<<"DID = "<<DID<<"; PhiID = "<<PhiID<<"; CalibFactor = "<<CalibFactorHist->GetBinContent(DID+1,PhiID+1)<<endl;
                    }
                }
            }
            inputfile->Close();
        }
        double Temp_CalibFactor = Sta_LPMTMu_QE->GetMean()/Sta_SPMTMu_QE->GetMean();
        double Temp_CalibFactor_Error = Temp_CalibFactor*sqrt(pow(Sta_LPMTMu_QE->GetMeanErr()/Sta_LPMTMu_QE->GetMean(),2)+pow(Sta_SPMTMu_QE->GetMeanErr()/Sta_SPMTMu_QE->GetMean(),2));

        Sta_LPMTMu_QE->Clear();
        Sta_SPMTMu_QE->Clear();

        CalibFactor.push_back(Temp_CalibFactor);
        CalibFactor_Error.push_back(Temp_CalibFactor_Error);
        R.push_back((pow((RID*RStep + RMin),1.0/3.0)/1000. + pow(((RID+1)*RStep + RMin),1.0/3.0)/1000.)/2.);
        R_Error.push_back((pow(((RID+1)*RStep + RMin),1.0/3.0)/1000. - pow((RID*RStep + RMin),1.0/3.0)/1000.)/2.);
    }

    TGraphErrors *CalibFactorToR = new TGraphErrors(CalibFactor.size(),&R[0],&CalibFactor[0],&R_Error[0],&CalibFactor_Error[0]);

    CalibFactorToR->Draw();
    CalibFactorToR->SetTitle("");
    CalibFactorToR->GetXaxis()->SetTitle("R");
    CalibFactorToR->GetYaxis()->SetTitle("CalibFactor");
    CalibFactorToR->SetFillStyle(3002);
    CalibFactorToR->SetMarkerColor(4);
    CalibFactorToR->SetLineColor(4);
    CalibFactorToR->SetMarkerSize(1.0);
    CalibFactorToR->SetMarkerStyle(20);
    CalibFactorToR->SetLineWidth(3);
    CalibFactorToR->GetXaxis()->CenterTitle();
    CalibFactorToR->GetYaxis()->CenterTitle();
    CalibFactorToR->GetXaxis()->SetTitleFont(22);
    CalibFactorToR->GetYaxis()->SetTitleFont(22);
    CalibFactorToR->GetXaxis()->SetTitleSize(0.06);
    CalibFactorToR->GetYaxis()->SetTitleSize(0.06);
    CalibFactorToR->GetXaxis()->SetLabelSize(0.06);
    CalibFactorToR->GetYaxis()->SetLabelSize(0.06);
    CalibFactorToR->GetXaxis()->SetTitleOffset(1.2);
    CalibFactorToR->GetYaxis()->SetTitleOffset(1.30);


    return 0;
}