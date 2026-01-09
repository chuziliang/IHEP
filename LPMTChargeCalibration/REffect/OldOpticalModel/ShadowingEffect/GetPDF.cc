const int NR = 30;
const int NTheta = 6;
const int ND = 25;
const int NPhi = 36;

const double MaxD = 5;

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

    TFile *inputfile_NoBar = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/OldOpticalModel/ChargeSpectrum_DetCenter/ChargeSpectrum.root","read");
    vector<double> *LPMTCharge;
    vector<double> *SPMTCharge;
    TTree *charge = (TTree*)inputfile_NoBar->Get("charge");
    charge->SetBranchAddress("TrueLPMTCharge",&LPMTCharge);
    charge->SetBranchAddress("TrueSPMTCharge",&SPMTCharge);

    for (int entry = 0; entry < charge->GetEntries(); entry++) {
        charge->GetEntry(entry);
        for (int LPMTID = 0; LPMTID < 17612; LPMTID++) {
            LPMTMeanQE_Sta->Fill((*LPMTCharge)[LPMTID]/LPMTQE[LPMTID]);
            for (int ID = 0; ID < 6; ID++)
            {
                if (CorrespondingSPMT[LPMTID][ID] >= 300000)
                {
                    SPMTMeanQE_Sta->Fill((*SPMTCharge)[CorrespondingSPMT[LPMTID][ID] - 300000]/SPMTQE[CorrespondingSPMT[LPMTID][ID] - 300000]);
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

void GetPDF()
{
    DataWithError TrueFactor;
    TrueFactor = GetTrueFactor();

    TCanvas *CalibFactor = new TCanvas("CalibFactor","CalibFactor",800,1200);
    CalibFactor->Divide(2,3);
    CalibFactor->Print("CalibFactor.pdf[");

    // TCanvas *Ratio = new TCanvas("Ratio","Ratio",800,1200);
    // Ratio->Divide(2,3);
    // Ratio->Print("Ratio.pdf[");

    TCanvas *TrueOrFalse = new TCanvas("TrueOrFalse","TrueOrFalse",800,1200);
    TrueOrFalse->Divide(2,3);
    TrueOrFalse->Print("TrueOrFalse_1Error.pdf[");

    TCanvas *PairNum = new TCanvas("PairNum","PairNum",800,1200);
    PairNum->Divide(2,3);
    PairNum->Print("PairNum.pdf[");

    double CalibFactorContour[20];
    CalibFactorContour[0] = 0;
    for (int id = 1; id < 20; id++)
    {
        CalibFactorContour[id] = 34 + id;
    }

    

    for (int RID = 0; RID < NR; RID++)
    {
        TFile *inputfile[NTheta];
        for (int ThetaID = 0; ThetaID < NTheta; ThetaID++)
        {
            TString filename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/OldOpticalModel/ShadowingEffect/R%1d_Theta%1d.root",RID,ThetaID);
            inputfile[ThetaID] = TFile::Open(filename,"read");
            TH2* CalibFactorHist = (TH2*)inputfile[ThetaID]->Get("CalibFactor");
            CalibFactorHist->SetName(TString::Format("R%1d_Theta%1d",RID,ThetaID));
            CalibFactorHist->SetContour(20,CalibFactorContour);
            CalibFactorHist->SetMinimum(34); // 设置Z轴的下限
            CalibFactorHist->SetMaximum(54); // 设置Z轴的上限
            TPaletteAxis *palette = (TPaletteAxis*)CalibFactorHist->GetListOfFunctions()->FindObject("palette");
            CalibFactor->cd(ThetaID+1);
            gStyle->SetOptStat(kFALSE);
            CalibFactorHist->Draw("COL Z CJUST");
            CalibFactor->Update();

            TH2* LPMTEventNum = (TH2*)inputfile[ThetaID]->Get("LPMTEventNum");
            LPMTEventNum->SetName(TString::Format("LPMTEventNum_R%1d_Theta%1d",RID,ThetaID));
            // LPMTEventNum->SetContour(20,CalibFactorContour);
            // LPMTEventNum->SetMinimum(40); // 设置Z轴的下限
            // LPMTEventNum->SetMaximum(60); // 设置Z轴的上限
            TPaletteAxis *palette2 = (TPaletteAxis*)LPMTEventNum->GetListOfFunctions()->FindObject("palette");
            PairNum->cd(ThetaID+1);
            gStyle->SetOptStat(kFALSE);
            LPMTEventNum->Draw("COL Z CJUST");
            PairNum->Update();

            TH2 *TrueOrFalseHist = new TH2F(TString::Format("TrueOrFalse_R%1d_Theta%1d",RID,ThetaID),CalibFactorHist->GetTitle(),ND, 0.0, MaxD, NPhi, 0.0, 360.0);
            for (int DID = 0; DID < ND; DID++)
            {
                for (int PhiID = 0; PhiID < NPhi; PhiID++)
                {
                    if (CalibFactorHist->GetBinContent(DID+1,PhiID+1) > 0)
                    {
                        double diff = abs(CalibFactorHist->GetBinContent(DID+1,PhiID+1) - TrueFactor.Data);
                        double Error = CalibFactorHist->GetBinError(DID+1,PhiID+1) + TrueFactor.Error;
                        if (Error > diff)
                        {
                            TrueOrFalseHist->SetBinContent(DID+1,PhiID+1,1);
                        }
                        else
                        {
                            TrueOrFalseHist->SetBinContent(DID+1,PhiID+1,2);
                        }
                    }
                }
            }

            TrueOrFalse->cd(ThetaID+1);
            TrueOrFalseHist->Draw("COL");
            TrueOrFalse->Update();

        }
        CalibFactor->Print("CalibFactor.pdf");
        PairNum->Print("PairNum.pdf");
        TrueOrFalse->Print("TrueOrFalse_1Error.pdf");
        for (int ThetaID = 0; ThetaID < NTheta; ThetaID++)
        {
            inputfile[ThetaID]->Close();
        }
    }
    CalibFactor->Print("CalibFactor.pdf]");
    // Ratio->Print("Ratio.pdf]");
    PairNum->Print("PairNum.pdf]");
    TrueOrFalse->Print("TrueOrFalse_1Error.pdf]");
}