//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。

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

void npeLPMTtonpeSPMT_BinnedByDistance()
{
    double DistanceToBar[17612];
    GetDistanceToBar(DistanceToBar);

    double OutputLPMT; 
    double OutputSPMT[6];
    double OutputTheta;
    double OutputPhi;
    double OutputR;
    double edep_x, edep_y, edep_z;
    double edepEnergy;
    int SPMT_ID[6];
    int LPMT_ID;
    TTree *OutputTree;
    // TH1 *LPMTChargeHist;
    // TH1 *SPMTChargeHist;

    vector<double> LPMTMean[50];
    vector<double> SPMTMean[50];
    vector<double> LPMTError[50];
    vector<double> SPMTError[50];
    TStatistic *EdepR[50];
    for (int RNum = 0; RNum < 50; RNum++)
    {
        EdepR[RNum] = new TStatistic(TString::Format("EdepR%1d",RNum));
    }
    
    for (int EnergyNum = 0; EnergyNum < 10; EnergyNum++)
    {
        for(int RNum = 0; RNum < 50; RNum++)
        {
            TStatistic *LPMTChargeHist = new TStatistic(TString::Format("LPMT Charge Hist_E%1d_R%1d",EnergyNum,RNum));
            TStatistic *SPMTChargeHist = new TStatistic(TString::Format("SPMT Charge Hist_E%1d_R%1d",EnergyNum,RNum));
            for (int filenum = 0; filenum < 10; filenum++)
            {
                TString filename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/B12/Analysis/ChargeSpectrum/Charge_%1d.root",filenum);
                TFile *inputfile = TFile::Open(filename,"read");
                OutputTree = (TTree*)inputfile->Get(TString::Format("Charge_E%1d_R%1d",EnergyNum,RNum));
                OutputTree->SetBranchAddress("LPMTCharge_QE",&OutputLPMT);            
                OutputTree->SetBranchAddress("SPMTCharge_QE",OutputSPMT);
                OutputTree->SetBranchAddress("LPMT_ID",&LPMT_ID);
                OutputTree->SetBranchAddress("SPMT_ID",SPMT_ID);
                OutputTree->SetBranchAddress("Theta",&OutputTheta);
                OutputTree->SetBranchAddress("Phi",&OutputPhi);
                OutputTree->SetBranchAddress("Distance between LPMT and event",&OutputR);
                OutputTree->SetBranchAddress("edep_x",&edep_x);
                OutputTree->SetBranchAddress("edep_y",&edep_y);
                OutputTree->SetBranchAddress("edep_z",&edep_z);
                OutputTree->SetBranchAddress("edep",&edepEnergy);
                if (OutputTree->GetEntries() == 0)
                {
                    continue;
                }
                for (int i = 0; i < OutputTree->GetEntries(); i++)
                {
                    OutputTree->GetEntry(i);
                    // LPMTChargeHist->Fill(OutputLPMT);
                    EdepR[RNum]->Fill(OutputR);

                    if ( OutputTheta > 20 || DistanceToBar[LPMT_ID] < 1600)
                    {
                        continue;
                    }
                    for (int SPMTID = 0; SPMTID < 6; SPMTID++)
                    {
                        if (OutputSPMT[SPMTID] >= 0)
                        {
                            LPMTChargeHist->Fill(OutputLPMT);
                            SPMTChargeHist->Fill(OutputSPMT[SPMTID]);
                        }
                    }
                }
                inputfile->Close();
            }
            
            if (LPMTChargeHist->GetN()>2)
            {
                LPMTMean[RNum].push_back(LPMTChargeHist->GetMean());
                SPMTMean[RNum].push_back(SPMTChargeHist->GetMean());
                LPMTError[RNum].push_back(LPMTChargeHist->GetMeanErr());
                SPMTError[RNum].push_back(SPMTChargeHist->GetMeanErr());
            }
            // LPMTChargeHist->SetDirectory(nullptr);
            // SPMTChargeHist->SetDirectory(nullptr);
            LPMTChargeHist->Delete();
            SPMTChargeHist->Delete();
            // OutputTree->Delete();
            cout<<EnergyNum<<" "<<RNum<<endl;
        }
    }

    TFile *output = new TFile("/junofs/users/chuziliang125/LPMTChargeCalibration/B12/Analysis/npeLPMTtonpeSPMT_BinnedByDistance.root","RECREATE");

    TGraphErrors *CalibrationFactorToR_tot;
    vector<double> Distance_tot;
    vector<double> CalibrationFactor_tot;
    vector<double> Distance_error_tot;
    vector<double> CalibrationFactor_error_tot;

    TGraphErrors *npeLPMTtonpeSPMT_R[50];
    for (int RNum = 0; RNum < 50; RNum++)
    {
        npeLPMTtonpeSPMT_R[RNum] = new TGraphErrors(LPMTMean[RNum].size(),&SPMTMean[RNum][0],&LPMTMean[RNum][0],&SPMTError[RNum][0],&LPMTError[RNum][0]);
        if (npeLPMTtonpeSPMT_R[RNum]->GetN() < 3)
        {
            continue;
        }
        TF1 *f1 = new TF1("f1","[0]*x",0,10);
        f1->SetParNames("k");
        npeLPMTtonpeSPMT_R[RNum]->Fit(f1);
        CalibrationFactor_tot.push_back(f1->GetParameter(0));
        CalibrationFactor_error_tot.push_back(f1->GetParError(0));
        Distance_tot.push_back(EdepR[RNum]->GetMean());
        Distance_error_tot.push_back(EdepR[RNum]->GetRMS());
    }
    CalibrationFactorToR_tot = new TGraphErrors(Distance_tot.size(),&Distance_tot[0],&CalibrationFactor_tot[0],&Distance_error_tot[0],&CalibrationFactor_error_tot[0]);
    TString GraphName = TString("Calibration factor in different positions");
    CalibrationFactorToR_tot->SetTitle(GraphName);
    CalibrationFactorToR_tot->SetName(GraphName);
    CalibrationFactorToR_tot->GetXaxis()->SetTitle("R/m");
    CalibrationFactorToR_tot->GetYaxis()->SetTitle("Calibration factor");
    // TF1 *f1 = new TF1("f1","[0]*x",0,10);
    // f1->SetParNames("k");
    // npeLPMTtonpeSPMT->Fit(f1);
    // gStyle->SetOptFit(1111);
    // gStyle->SetStatY(0.5);
    // gPad->SetLogx();
    // gPad->SetLogy();
    // npeLPMTtonpeSPMT->Draw("AP");
    output->cd();
    CalibrationFactorToR_tot->Write();
}