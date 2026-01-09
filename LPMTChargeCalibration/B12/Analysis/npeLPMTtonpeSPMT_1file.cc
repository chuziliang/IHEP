//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 
void npeLPMTtonpeSPMT_1file()
{
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
    TStatistic *LPMTChargeHist;
    TStatistic *SPMTChargeHist;

    vector<double> LPMTMean;
    vector<double> SPMTMean;
    vector<double> LPMTError;
    vector<double> SPMTError;
    for (int EnergyNum = 0; EnergyNum < 1; EnergyNum++)
    {
        for(int RNum = 0; RNum < 50; RNum++)
        {
            LPMTChargeHist = new TStatistic(TString::Format("LPMT Charge Hist_E%1d_R%1d",EnergyNum,RNum));
            SPMTChargeHist = new TStatistic(TString::Format("SPMT Charge Hist_E%1d_R%1d",EnergyNum,RNum));

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

                for (int i = 0; i < OutputTree->GetEntries(); i++)
                {
                    OutputTree->GetEntry(i);
                    // LPMTChargeHist->Fill(OutputLPMT);
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
            cout<<"test"<<endl;
            
            
            if (LPMTChargeHist->GetN()>2)
            {
                LPMTMean.push_back(LPMTChargeHist->GetMean());
                SPMTMean.push_back(SPMTChargeHist->GetMean());
                LPMTError.push_back(LPMTChargeHist->GetMeanErr());
                SPMTError.push_back(SPMTChargeHist->GetMeanErr());
            }
            LPMTChargeHist->Delete();
            SPMTChargeHist->Delete();
            // OutputTree->Delete();
            cout<<EnergyNum<<" "<<RNum<<endl;
        }
    }

    TFile *output = new TFile("/junofs/users/chuziliang125/LPMTChargeCalibration/B12/Analysis/npeLPMTtonpeSPMT_1file.root","RECREATE");
    TGraphErrors *npeLPMTtonpeSPMT;
    TCanvas *Canvas = new TCanvas("c1","c1",1);
    TString GraphName = TString("LPMT npe to SPMT npe");
    npeLPMTtonpeSPMT = new TGraphErrors(LPMTMean.size(),&SPMTMean[0],&LPMTMean[0],&SPMTError[0],&LPMTError[0]);
    npeLPMTtonpeSPMT->SetTitle(GraphName);
    npeLPMTtonpeSPMT->SetName(GraphName);
    npeLPMTtonpeSPMT->GetXaxis()->SetTitle("nPE_SPMT/QE");
    npeLPMTtonpeSPMT->GetYaxis()->SetTitle("nPE_LPMT/QE");
    TF1 *f1 = new TF1("f1","[0]*x",0,10);
    f1->SetParNames("k");
    npeLPMTtonpeSPMT->Fit(f1);
    gStyle->SetOptFit(1111);
    gStyle->SetStatY(0.5);
    gPad->SetLogx();
    gPad->SetLogy();
    npeLPMTtonpeSPMT->Draw("AP");
    output->cd();
    npeLPMTtonpeSPMT->Write();
}