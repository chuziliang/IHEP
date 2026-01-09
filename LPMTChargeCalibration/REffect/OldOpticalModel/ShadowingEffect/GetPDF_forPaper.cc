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
    vector<double> *LPMTCharge  = NULL;
    vector<double> *SPMTCharge  = NULL;
    TTree *charge = (TTree*)inputfile_NoBar->Get("charge");
    charge->SetBranchAddress("TrueLPMTCharge",&LPMTCharge);
    charge->SetBranchAddress("TrueSPMTCharge",&SPMTCharge);

    for (int entry = 0; entry < charge->GetEntries(); entry++) {
        charge->GetEntry(entry);
        //这里出现了break
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

void GetPDF_forPaper()
{
    int selectedRID[] = {0,6,29};
    DataWithError TrueFactor;
    TrueFactor = GetTrueFactor();
    TString RID_str[] = {"R=[3.9,10.9) [m]","R=[19.5,20.5) [m]","R=[32.9,33.3) [m]"};
    TString Theta_str[] = {"#theta=[0,10) [deg]","#theta=[10,20) [deg]","#theta=[20,30) [deg]","#theta=[30,40) [deg]"};

    TCanvas *CalibFactor = new TCanvas("CalibFactor","CalibFactor",1600,1200);
    CalibFactor->Divide(4,3,0,0);
    CalibFactor->Print("CalibFactor_forPaper.pdf[");

    // TCanvas *Ratio = new TCanvas("Ratio","Ratio",800,1200);
    // Ratio->Divide(2,3);
    // Ratio->Print("Ratio_forPaper.pdf[");

    TCanvas *TrueOrFalse = new TCanvas("TrueOrFalse","TrueOrFalse",1600,1200);
    TrueOrFalse->Divide(4,3,0,0);
    TrueOrFalse->Print("TrueOrFalse_1Error_forPaper.pdf[");

    TCanvas *PairNum = new TCanvas("PairNum","PairNum",1600,1200);
    PairNum->Divide(4,3);
    PairNum->Print("PairNum_forPaper.pdf[");

    double CalibFactorContour[20];
    CalibFactorContour[0] = 0;
    for (int id = 1; id < 20; id++)
    {
        CalibFactorContour[id] = 34 + id;
    }

    

    for (int RID = 0; RID < 3; RID++)
    {
        TFile *inputfile[4];
        for (int ThetaID = 0; ThetaID < 4; ThetaID++)
        {
            TLine *line;
            if (RID == 0 && ThetaID == 0)
            {
                line = new TLine(1.8,0,1.8,360);
            }
            else if (RID > 0 && ThetaID == 0)
            {
                line = new TLine(1.0,0,1.0,360);
            }
            else if (ThetaID == 1)
            {
                line = new TLine(2.8,0,2.8,360);
            }
            else if (ThetaID == 2)
            {
                line = new TLine(3.2,0,3.2,360);
            }

            line->SetLineColor(kRed);
            line->SetLineWidth(2);

            TString filename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/OldOpticalModel/ShadowingEffect/R%1d_Theta%1d.root",selectedRID[RID],ThetaID);
            inputfile[ThetaID] = TFile::Open(filename,"read");
            TH2* CalibFactorHist = (TH2*)inputfile[ThetaID]->Get("CalibFactor");
            CalibFactorHist->SetName(TString::Format("R%1d_Theta%1d",selectedRID[RID],ThetaID));
            CalibFactorHist->GetXaxis()->SetTitle("D [m]");
            CalibFactorHist->GetYaxis()->SetTitle("#varphi [deg]");
            CalibFactorHist->GetXaxis()->CenterTitle();
            CalibFactorHist->GetYaxis()->CenterTitle();
            CalibFactorHist->GetXaxis()->SetTitleFont(22);
            CalibFactorHist->GetYaxis()->SetTitleFont(22);
            CalibFactorHist->GetXaxis()->SetTitleSize(0.06);
            CalibFactorHist->GetYaxis()->SetTitleSize(0.06);
            CalibFactorHist->GetXaxis()->SetLabelSize(0.05);
            CalibFactorHist->GetYaxis()->SetLabelSize(0);
            CalibFactorHist->GetXaxis()->SetTitleOffset(0.82);
            CalibFactorHist->GetYaxis()->SetTitleOffset(1.04);
            CalibFactorHist->SetContour(20,CalibFactorContour);
            CalibFactorHist->SetMinimum(34); // 设置Z轴的下限
            CalibFactorHist->SetMaximum(54); // 设置Z轴的上限
            CalibFactorHist->SetTitle(RID_str[RID]+", "+Theta_str[ThetaID]);
            TGaxis *axis = new TGaxis(0, 0, 0, 360, 0, 360, 510, "");
            axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
            axis->SetLabelSize(15);
            
            TPaletteAxis *palette = (TPaletteAxis*)CalibFactorHist->GetListOfFunctions()->FindObject("palette");
            CalibFactor->cd(RID*4+ThetaID+1);
            gStyle->SetOptStat(kFALSE);
            gPad->SetLeftMargin(0.12);   // 增加左侧边距
            gPad->SetRightMargin(0.12);  // 调整右侧边距
            gPad->SetBottomMargin(0.14);  // 增加底部边距
            gPad->SetTopMargin(0.1);  // 增加底部边距
            CalibFactorHist->Draw("COL Z CJUST");
            line->Draw();
            axis->Draw();
            CalibFactor->Update();

            TH2* LPMTEventNum = (TH2*)inputfile[ThetaID]->Get("LPMTEventNum");
            LPMTEventNum->SetName(TString::Format("LPMTEventNum_R%1d_Theta%1d",selectedRID[RID],ThetaID));
            LPMTEventNum->GetXaxis()->SetTitle("D [m]");
            LPMTEventNum->GetYaxis()->SetTitle("#varphi [deg]");
            LPMTEventNum->GetXaxis()->CenterTitle();
            LPMTEventNum->GetYaxis()->CenterTitle();
            LPMTEventNum->GetXaxis()->SetTitleFont(22);
            LPMTEventNum->GetYaxis()->SetTitleFont(22);
            LPMTEventNum->GetXaxis()->SetTitleSize(0.06);
            LPMTEventNum->GetYaxis()->SetTitleSize(0.06);
            LPMTEventNum->GetXaxis()->SetLabelSize(0.06);
            LPMTEventNum->GetYaxis()->SetLabelSize(0.06);
            LPMTEventNum->GetXaxis()->SetTitleOffset(0.92);
            LPMTEventNum->GetYaxis()->SetTitleOffset(1.08);
            LPMTEventNum->SetTitle(RID_str[RID]+", "+Theta_str[ThetaID]);
            // LPMTEventNum->SetContour(20,CalibFactorContour);
            // LPMTEventNum->SetMinimum(40); // 设置Z轴的下限
            // LPMTEventNum->SetMaximum(60); // 设置Z轴的上限
            TPaletteAxis *palette2 = (TPaletteAxis*)LPMTEventNum->GetListOfFunctions()->FindObject("palette");
            PairNum->cd(RID*4+ThetaID+1);
            gStyle->SetOptStat(kFALSE);
            gPad->SetLeftMargin(0.12);   // 增加左侧边距
            gPad->SetRightMargin(0.15);  // 调整右侧边距
            gPad->SetBottomMargin(0.15);  // 增加底部边距
            LPMTEventNum->Draw("COL Z CJUST");
            line->Draw();
            PairNum->Update();

            TH2 *TrueOrFalseHist = new TH2F(TString::Format("TrueOrFalse_R%1d_Theta%1d",selectedRID[RID],ThetaID),CalibFactorHist->GetTitle(),ND, 0.0, MaxD, NPhi, 0.0, 360.0);
            TrueOrFalseHist->GetXaxis()->SetTitle("D [m]");
            TrueOrFalseHist->GetYaxis()->SetTitle("#varphi [deg]");
            TrueOrFalseHist->GetXaxis()->CenterTitle();
            TrueOrFalseHist->GetYaxis()->CenterTitle();
            TrueOrFalseHist->GetXaxis()->SetTitleFont(22);
            TrueOrFalseHist->GetYaxis()->SetTitleFont(22);
            TrueOrFalseHist->GetXaxis()->SetTitleSize(0.06);
            TrueOrFalseHist->GetYaxis()->SetTitleSize(0.06);
            TrueOrFalseHist->GetXaxis()->SetLabelSize(0.06);
            TrueOrFalseHist->GetYaxis()->SetLabelSize(0.06);
            TrueOrFalseHist->GetXaxis()->SetTitleOffset(0.92);
            TrueOrFalseHist->GetYaxis()->SetTitleOffset(1.4);
            TrueOrFalseHist->SetTitle(RID_str[RID]+", "+Theta_str[ThetaID]);
            for (int DID = 0; DID < ND; DID++)
            {
                for (int PhiID = 0; PhiID < NPhi; PhiID++)
                {
                    if (CalibFactorHist->GetBinContent(DID+1,PhiID+1) > 0)
                    {
                        double diff = abs(CalibFactorHist->GetBinContent(DID+1,PhiID+1) - TrueFactor.Data);
                        double Error = CalibFactorHist->GetBinError(DID+1,PhiID+1) + TrueFactor.Error;
                        if (2*Error > diff)
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

            TrueOrFalse->cd(RID*4+ThetaID+1);
            gPad->SetLeftMargin(0.16);   // 增加左侧边距
            gPad->SetRightMargin(0.08);  // 调整右侧边距
            gPad->SetBottomMargin(0.14);  // 增加底部边距
            gPad->SetTopMargin(0.1);  // 增加底部边距
            TrueOrFalseHist->Draw("COL");
            line->Draw();
            TrueOrFalse->Update();

        }
        
        // for (int ThetaID = 0; ThetaID < 4; ThetaID++)
        // {
        //     inputfile[ThetaID]->Close();
        // }
    }

    CalibFactor->Print("CalibFactor_forPaper.pdf");
    PairNum->Print("PairNum_forPaper.pdf");
    TrueOrFalse->Print("TrueOrFalse_1Error_forPaper.pdf");
    CalibFactor->Print("CalibFactor_forPaper.pdf]");
    // Ratio->Print("Ratio_forPaper.pdf]");
    PairNum->Print("PairNum_forPaper.pdf]");
    TrueOrFalse->Print("TrueOrFalse_1Error_forPaper.pdf]");
}