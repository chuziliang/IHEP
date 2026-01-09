//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 
void npeLPMTtonpeSPMT()
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
    TStatistic *Distance;

    vector<double> LPMTMean;
    vector<double> SPMTMean;
    vector<double> LPMTError;
    vector<double> SPMTError;
    vector<double> DistanceMean;
    vector<double> DistanceError;
    vector<double> Ratio;
    vector<double> RatioError;

    for(int RNum = 0; RNum < 20; RNum++)
    {
        LPMTChargeHist = new TStatistic(TString::Format("LPMT Charge Hist_R%1d",RNum));
        SPMTChargeHist = new TStatistic(TString::Format("SPMT Charge Hist_R%1d",RNum));
        Distance = new TStatistic(TString::Format("R%1d",RNum));

        for (int filenum = 0; filenum < 50; filenum++)
        {
            TString filename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/Spallation_neutron/Analysis/ChargeSpectrum/Charge_%1d.root",filenum);
            TFile *inputfile = TFile::Open(filename,"read");
            OutputTree = (TTree*)inputfile->Get(TString::Format("Charge_R%1d",RNum));
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
                Distance->Fill(OutputR);
            }
            inputfile->Close();
        }
        
        
        if (LPMTChargeHist->GetN()>20)
        {
            LPMTMean.push_back(LPMTChargeHist->GetMean()/0.31908);
            SPMTMean.push_back(SPMTChargeHist->GetMean());
            LPMTError.push_back(LPMTChargeHist->GetMeanErr()/0.31908);
            SPMTError.push_back(SPMTChargeHist->GetMeanErr());
            DistanceMean.push_back(Distance->GetMean());
            DistanceError.push_back(Distance->GetMeanErr());
            Ratio.push_back(LPMTChargeHist->GetMean()/SPMTChargeHist->GetMean());
            RatioError.push_back(
                (LPMTChargeHist->GetMean()/SPMTChargeHist->GetMean())
                *sqrt((LPMTChargeHist->GetMeanErr()*LPMTChargeHist->GetMeanErr())/(LPMTChargeHist->GetMean()*LPMTChargeHist->GetMean()) + 
                (SPMTChargeHist->GetMeanErr()*SPMTChargeHist->GetMeanErr())/(SPMTChargeHist->GetMean()*SPMTChargeHist->GetMean())));
            
        }
        // LPMTChargeHist->SetDirectory(nullptr);
        // SPMTChargeHist->SetDirectory(nullptr);
        LPMTChargeHist->Delete();
        SPMTChargeHist->Delete();
        // OutputTree->Delete();
        cout<<" "<<RNum<<endl;
    }

    TFile *output = new TFile("/junofs/users/chuziliang125/LPMTChargeCalibration/Spallation_neutron/Analysis/npeLPMTtonpeSPMT.root","RECREATE");
    TGraphErrors *npeLPMTtonpeSPMT;
    TCanvas *Canvas = new TCanvas("c1","c1",1);
    TString GraphName = TString("LPMT npe to SPMT npe");
    npeLPMTtonpeSPMT = new TGraphErrors(LPMTMean.size(),&SPMTMean[0],&LPMTMean[0],&SPMTError[0],&LPMTError[0]);
    npeLPMTtonpeSPMT->SetTitle("");
    npeLPMTtonpeSPMT->SetName(GraphName);
    npeLPMTtonpeSPMT->GetXaxis()->SetTitle("#mu_{SPMT}/QE");
    npeLPMTtonpeSPMT->GetYaxis()->SetTitle("#mu_{LPMT}(p.e.)");
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

    TCanvas *Canvas2 = new TCanvas("c2","c2",1);
    TGraphErrors *RatioToR = new TGraphErrors(Ratio.size(),&DistanceMean[0],&Ratio[0],&DistanceError[0],&RatioError[0]);
    RatioToR->SetTitle(GraphName);
    RatioToR->SetName(GraphName);
    RatioToR->GetXaxis()->SetTitle("Ratio");
    RatioToR->GetYaxis()->SetTitle("Distance");
    RatioToR->Draw();

}