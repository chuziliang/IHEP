// 用于画出不同位置、不同能量的事例下，LPMT和SPMT的非均匀性对比

const double R = 19.434; // m
const double position[11] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.95};
const int Points = 10;
const double Energy[5]={0.5,1.16,2,5,10};
const int LPMT_ID = 8947;

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

double combined_measurement(const vector<double>& x, const vector<double>& e) {
    double sum_x = 0.0;
    double sum_w = 0.0;

    for (int i = 0; i < x.size(); i++) {
        // double w = 1 / (e[i] * e[i]);
        double w = 1;
        sum_x += w * x[i];
        sum_w += w;
    }

    double combined_x = sum_x / sum_w;
    return combined_x;
}

double combined_error(const vector<double>& e) {
    double sum_w = 0.0;

    for (int i = 0; i < e.size(); i++) {
        double w = (e[i] * e[i]);
        sum_w += w;
    }

    double combined_e = sqrt(sum_w);
    return combined_e;
}

void DrawNonUniforimity_BinedByR()
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    int CorrespondingSPMT[17612][6];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);

    TGraphErrors *npeLPMTtonpeSPMT[6][5]; // PMTID, EnergyNum
    TGraphErrors *ratio[6][5];
    vector<double> MeanRatio[6][5];
    vector<double> ErrorRatio[6][5];
    vector<double> MeanPE[6][5];
    vector<double> ErrorPE[6][5];

    TGraphErrors *npeLPMTtonpeSPMT_BinedByR[6][10]; // PMTID, R
    vector<double> SPMTpe_BinedByR[6][10];
    vector<double> LPMTpe_BinedByR[6][10];
    vector<double> SPMTpe_BinedByR_error[6][10];
    vector<double> LPMTpe_BinedByR_error[6][10];
    for (int num = 0; num < 5; num++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/npeLPMTtonpeSPMT%.1fMeV.root",Energy[num]),"read");
        for (int PMTID = 0; PMTID < 6; PMTID++)
        {
            npeLPMTtonpeSPMT[PMTID][num] = (TGraphErrors*)inputfile->Get(TString::Format("LPMT npe to SPMT_%1d npe",PMTID));
            npeLPMTtonpeSPMT[PMTID][num]->SetName(TString::Format("LPMT npe to SPMT_%1d npe %.1fMeV",PMTID,Energy[num]));
            npeLPMTtonpeSPMT[PMTID][num]->SetMarkerColor(num+1);
            npeLPMTtonpeSPMT[PMTID][num]->SetLineColor(num+1);
            npeLPMTtonpeSPMT[PMTID][num]->GetListOfFunctions()->ls();
            TF1 *fun = npeLPMTtonpeSPMT[PMTID][num]->GetFunction("pol1");
            npeLPMTtonpeSPMT[PMTID][num]->GetListOfFunctions()->Remove(fun);
            for (int i = 0; i < 10; i++)
            {
                double x = npeLPMTtonpeSPMT[PMTID][num]->GetPointX(i);
                double y = npeLPMTtonpeSPMT[PMTID][num]->GetPointY(i);
                double ex = npeLPMTtonpeSPMT[PMTID][num]->GetErrorX(i);
                double ey = npeLPMTtonpeSPMT[PMTID][num]->GetErrorY(i);
                if (x>0 & y>0)
                {
                    SPMTpe_BinedByR[PMTID][i].push_back(x/SPMTQE[CorrespondingSPMT[LPMT_ID][PMTID] - 300000]);
                    LPMTpe_BinedByR[PMTID][i].push_back(y/LPMTQE[LPMT_ID]);
                    SPMTpe_BinedByR_error[PMTID][i].push_back(ex/SPMTQE[CorrespondingSPMT[LPMT_ID][PMTID] - 300000]);
                    LPMTpe_BinedByR_error[PMTID][i].push_back(ey/LPMTQE[LPMT_ID]);

                    MeanRatio[PMTID][num].push_back(y/x);
                    ErrorRatio[PMTID][num].push_back((y/x)*sqrt((ex*ex)/(x*x)+(ey*ey)/(y*y)));
                    MeanPE[PMTID][num].push_back(x);
                    ErrorPE[PMTID][num].push_back(ex);
                }
                else
                {
                    npeLPMTtonpeSPMT[PMTID][num]->RemovePoint(i);
                }
            }
        }
    }

    TGraphErrors *CalibrationFactorToR[6];
    double Distance[6][10];
    double CalibrationFactor[6][10];
    double Distance_error[6][10];
    double CalibrationFactor_error[6][10];

    TGraphErrors *CalibrationFactorToR_tot;
    double Distance_tot[10];
    double CalibrationFactor_tot[10];
    double Distance_error_tot[10];
    double CalibrationFactor_error_tot[10];

    for (int i = 0; i < 10; i++)
    {
        Distance_tot[i] = 0;
        CalibrationFactor_tot[i] = 0;
        Distance_error_tot[i] = 0;
        CalibrationFactor_error_tot[i] = 0;
    }
    

    for (int PMTID = 0; PMTID < 6; PMTID++)
    {
        for (int RNum = 0; RNum < 10; RNum++)
        {
            npeLPMTtonpeSPMT_BinedByR[PMTID][RNum] = new TGraphErrors(LPMTpe_BinedByR[PMTID][RNum].size(), &SPMTpe_BinedByR[PMTID][RNum][0], &LPMTpe_BinedByR[PMTID][RNum][0],&SPMTpe_BinedByR_error[PMTID][RNum][0], &LPMTpe_BinedByR_error[PMTID][RNum][0]);
            npeLPMTtonpeSPMT_BinedByR[PMTID][RNum]->SetName(TString::Format("LPMT npe to SPMT_%1d npe R_%1d",PMTID,RNum));
            npeLPMTtonpeSPMT_BinedByR[PMTID][RNum]->SetTitle(TString::Format("LPMT npe to SPMT_%1d npe R_%1d",PMTID,RNum));
            TF1 *f1 = new TF1("f1","[0]*x",0,10);
            f1->SetParNames("k");
            npeLPMTtonpeSPMT_BinedByR[PMTID][RNum]->Fit(f1);
            Distance[PMTID][RNum] = position[RNum]*R;
            Distance_error[PMTID][RNum] = 0;
            CalibrationFactor[PMTID][RNum] = f1->GetParameter(0);
            CalibrationFactor_error[PMTID][RNum] = f1->GetParError(0);

            Distance_tot[RNum] += Distance[PMTID][RNum];
            CalibrationFactor_tot[RNum] += CalibrationFactor[PMTID][RNum];
            Distance_error_tot[RNum] += Distance_error[PMTID][RNum]*Distance_error[PMTID][RNum];
            CalibrationFactor_error_tot[RNum] += CalibrationFactor_error[PMTID][RNum]*CalibrationFactor_error[PMTID][RNum];
        }
    }
    for (int i = 0; i < 10; i++)
    {
        Distance_tot[i] /= 6.0;
        CalibrationFactor_tot[i] /= 6.0;
        Distance_error_tot[i] = sqrt(Distance_error_tot[i])/6.0;
        CalibrationFactor_error_tot[i] = sqrt(CalibrationFactor_error_tot[i])/6.0;
    }

    CalibrationFactorToR_tot = new TGraphErrors(10,Distance_tot,CalibrationFactor_tot,Distance_error_tot,CalibrationFactor_error_tot);
    CalibrationFactorToR_tot->GetXaxis()->SetTitle("R/m");
    CalibrationFactorToR_tot->GetYaxis()->SetTitle("CalibrationFactor");
    CalibrationFactorToR_tot->SetTitle("Calibration factor in different positions");
    CalibrationFactorToR_tot->GetXaxis()->SetRangeUser(-1,19);
    TCanvas *Canvas_tot = new TCanvas("Canvas_tot","Canvas_tot",1);
    CalibrationFactorToR_tot->Draw("A L P");

    TCanvas *CanvasA[6];
    for (int PMTID = 0; PMTID < 6; PMTID++)
    {
        CalibrationFactorToR[PMTID] = new TGraphErrors(10,Distance[PMTID],CalibrationFactor[PMTID],Distance_error[PMTID],CalibrationFactor_error[PMTID]);
        CalibrationFactorToR[PMTID]->GetXaxis()->SetTitle("R/m");
        CalibrationFactorToR[PMTID]->GetYaxis()->SetTitle("CalibrationFactor");
        CalibrationFactorToR[PMTID]->SetTitle(TString::Format("Calibration factor in different positions SPMT_%1d",PMTID));
        CalibrationFactorToR[PMTID]->GetXaxis()->SetRangeUser(-1,19);
        CanvasA[PMTID] = new TCanvas(TString::Format("A%1d",PMTID));
        CalibrationFactorToR[PMTID]->Draw("A L P");
    }


    // for (int PMTID = 0; PMTID < 6; PMTID++)
    // {
    //     npeLPMTtonpeSPMT[PMTID][4]->SetMarkerColor(6);
    //     npeLPMTtonpeSPMT[PMTID][4]->SetLineColor(6);
    //     ratio[PMTID][4]->SetMarkerColor(6);
    //     ratio[PMTID][4]->SetLineColor(6);

    //     npeLPMTtonpeSPMT[PMTID][1]->SetMarkerColor(7);
    //     npeLPMTtonpeSPMT[PMTID][1]->SetLineColor(7);
    //     ratio[PMTID][1]->SetMarkerColor(7);
    //     ratio[PMTID][1]->SetLineColor(7);
    // }
    
    


    // TCanvas *Canvas[6];
    // TMultiGraph *mg[6];
    // TMultiGraph *Ratiomg[6]; 
    // TPad *pad1[6];
    // TPad *pad2[6];
    // TF1 *f1[6];
    // TF1 *f2[6];
    // for (int PMTID = 0; PMTID < 6; PMTID++)
    // {
    //     TString mgName = TString::Format("LPMT npe to SPMT_%1d npe",PMTID);
    //     mg[PMTID] = new TMultiGraph(mgName,mgName);
    //     Canvas[PMTID] = new TCanvas(TString::Format("C%1d",PMTID));
    //     f1[PMTID] = new TF1(TString::Format("f1%1d",PMTID),"pol1",0,3);
        

    //     pad1[PMTID] = new TPad("pad1", "pad1", 0.00, 0.32, 1.00, 0.95);
    //     pad2[PMTID] = new TPad("pad1", "pad1", 0.00, 0, 1.00, 0.32);
    //     pad1[PMTID]->SetMargin(0.12, 0.03, 0., 0.0);
    //     pad2[PMTID]->SetMargin(0.12, 0.03, 0.3, 0.0);
    //     pad1[PMTID]->Draw();
    //     pad2[PMTID]->Draw();
    //     pad1[PMTID]->cd();

    //     TLegend *legenda =new TLegend(0.2,0.55,0.4,0.85);
    //     legenda->SetTextFont(72);
    //     legenda->SetTextSize(0.06);
    //     legenda->SetLineColor(0);
    //     // npeLPMTtonpeSPMT[PMTID][4]->Draw("A L P");
    //     // legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][4],TString::Format("%.1fMeV",Energy[4]));
    //     for (int num = 0; num < 5; num++)
    //     {
    //         mg[PMTID]->Add(npeLPMTtonpeSPMT[PMTID][num]);
    //         // npeLPMTtonpeSPMT[PMTID][num]->Draw("same L P");
    //         legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][num],TString::Format("%.1fMeV",Energy[num]));
    //     }
    //     mg[PMTID]->SetMinimum(0.001);
    //     mg[PMTID]->GetYaxis()->SetRangeUser(0.01,120);
    //     mg[PMTID]->GetXaxis()->SetLimits(0.0001,3);
    //     mg[PMTID]->GetYaxis()->SetTitle("LPMT charge(p.e.)");
    //     mg[PMTID]->GetYaxis()->SetLabelSize(0.06);
    //     mg[PMTID]->GetYaxis()->SetTitleSize(0.06);
    //     mg[PMTID]->GetYaxis()->SetTitleOffset(0.65);
    //     mg[PMTID]->GetYaxis()->CenterTitle();
    //     gPad->SetLogx();
    //     gPad->SetLogy();
    //     mg[PMTID]->Draw("A L P");
    //     mg[PMTID]->Fit(f1[PMTID]);
    //     gStyle->SetOptFit(1111);
    //     // gStyle->SetStatY(0.5);
    //     // gPad->Update();
    //     // TPaveStats *s1 = (TPaveStats*)Canvas[PMTID]->GetPrimitive("stats");
    //     // s1->SetY1NDC(0.1); // new x start position
    //     // s1->SetY2NDC(0.4); // new x end position
    //     legenda->Draw();

    //     pad2[PMTID]->cd();
    //     TString RatiomgName = TString::Format("LPMT/SPMT_%1d",PMTID);
    //     Ratiomg[PMTID] = new TMultiGraph(RatiomgName,RatiomgName);
    //     f2[PMTID] = new TF1(TString::Format("f2%1d",PMTID),"pol1",0,3);

    //     // RatioCanvas[PMTID] = new TCanvas(TString::Format("RC%1d",PMTID));

    //     // TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    //     // legendb->SetTextFont(72);
    //     // legendb->SetTextSize(0.04);
    //     // npeLPMTtonpeSPMT[PMTID][4]->Draw("A L P");
    //     // legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][4],TString::Format("%.1fMeV",Energy[4]));
    //     for (int num = 0; num < 5; num++)
    //     {
    //         Ratiomg[PMTID]->Add(ratio[PMTID][num]);
    //         // npeLPMTtonpeSPMT[PMTID][num]->Draw("same L P");
    //         // legendb->AddEntry(ratio[PMTID][num],TString::Format("%.1fMeV",Energy[num]));
    //     }
    //     Ratiomg[PMTID]->SetMinimum(0.001);
    //     // Ratiomg[PMTID]->SetTitleSize(20);
    //     // Ratiomg[PMTID]->GetYaxis()->SetRangeUser(0.01,120);
    //     Ratiomg[PMTID]->GetXaxis()->SetLimits(0.0001,3);
    //     Ratiomg[PMTID]->GetXaxis()->SetTitle("SPMT charge(p.e.)");
    //     Ratiomg[PMTID]->GetYaxis()->SetTitle("LPMT/SPMT");
    //     Ratiomg[PMTID]->GetYaxis()->SetTitleSize(0.10);
    //     Ratiomg[PMTID]->GetYaxis()->SetTitleOffset(0.38);
    //     Ratiomg[PMTID]->GetYaxis()->SetLabelSize(0.12);
    //     Ratiomg[PMTID]->GetYaxis()->SetNdivisions(4,5,0);
    //     // Ratiomg[PMTID]->GetYaxis()->SetLabelSize(0.07);
    //     Ratiomg[PMTID]->GetXaxis()->SetTitleSize(0.13);
    //     Ratiomg[PMTID]->GetXaxis()->SetTitleOffset(1.);
    //     Ratiomg[PMTID]->GetXaxis()->SetLabelSize(0.12);
    //     gPad->SetLogx();
    //     // gPad->SetLogy();
    //     Ratiomg[PMTID]->Draw("A L P");
    //     Ratiomg[PMTID]->Fit(f2[PMTID]);
    //     gStyle->SetOptFit(1111);
    //     // gStyle->SetStatY(0.2);
    //     // legendb->Draw();
    // }

    // TCanvas *RatioCanvas[6];
    // TMultiGraph *Ratiomg[6]; 

    // for (int PMTID = 0; PMTID < 6; PMTID++)
    // {
    //     TString RatiomgName = TString::Format("LPMT npe to SPMT_%1d npe",PMTID);
    //     Ratiomg[PMTID] = new TMultiGraph(RatiomgName,RatiomgName);
    //     RatioCanvas[PMTID] = new TCanvas(TString::Format("RC%1d",PMTID));

    //     TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    //     legendb->SetTextFont(72);
    //     legendb->SetTextSize(0.04);
    //     // npeLPMTtonpeSPMT[PMTID][4]->Draw("A L P");
    //     // legenda->AddEntry(npeLPMTtonpeSPMT[PMTID][4],TString::Format("%.1fMeV",Energy[4]));
    //     for (int num = 0; num < 5; num++)
    //     {
    //         Ratiomg[PMTID]->Add(ratio[PMTID][num]);
    //         // npeLPMTtonpeSPMT[PMTID][num]->Draw("same L P");
    //         legendb->AddEntry(ratio[PMTID][num],TString::Format("%.1fMeV",Energy[num]));
    //     }
    //     Ratiomg[PMTID]->SetMinimum(0.001);
    //     // Ratiomg[PMTID]->GetYaxis()->SetRangeUser(0.01,120);
    //     Ratiomg[PMTID]->GetXaxis()->SetLimits(0.0001,3);
    //     gPad->SetLogx();
    //     // gPad->SetLogy();
    //     Ratiomg[PMTID]->Draw("A L P");
    //     Ratiomg[PMTID]->Fit("pol1");
    //     gStyle->SetOptFit(1111);
    //     gStyle->SetStatY(0.5);
    //     legendb->Draw();
    // }

}