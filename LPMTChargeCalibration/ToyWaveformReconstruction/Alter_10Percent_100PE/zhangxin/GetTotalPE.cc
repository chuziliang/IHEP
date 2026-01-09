void GetTotalPE()
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};
    TStatistic *TotalPEHist[21];
    for (int i = 0; i < 21; i++) //
    {
        TString histname = TString::Format("%1d MeV LPMT TotalPE hist",energy[i]);
        TotalPEHist[i] = new TStatistic(histname);
    }
    
    double TotalPEData;
    double TotalPEError;
    TFile* outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/zhangxin/TotalPE.root","recreate");
    TTree* TotalPETree = new TTree("TotalPETree","TotalPETree");
    TotalPETree->Branch("TotalPEData",&TotalPEData,"TotalPEData/D");
    TotalPETree->Branch("TotalPEError",&TotalPEError,"TotalPEError/D");

    for(int i = 0; i<21; i++)
    {
        TFile* inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/ChargeSpectrumAllLPMT/%1dMevCharge.root",energy[i]),"read");
        int size = 0;
        for(int j = 0; j<numbers[i]; j++)
        {
            TTree *LPMT_Charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            if (!LPMT_Charge) continue;
            double TotalPE = 0;
            LPMT_Charge->SetBranchAddress("TotalPELPMT",&TotalPE);
            for (int k =0; k<LPMT_Charge->GetEntries(); k++)
            {
                LPMT_Charge->GetEntry(k);
                TotalPEHist[i]->Fill(TotalPE);
                size++;
            }
            delete LPMT_Charge;
            if (size>100000)
            {
                break;
            }
        }
        TotalPEData = TotalPEHist[i]->GetMean();
        TotalPEError = TotalPEHist[i]->GetMeanErr();
        TotalPETree->Fill();
        cout<<energy[i]<<" Finished "<<"TotalPE="<<TotalPEData<<" Error="<<TotalPEError<<endl;
    }
    outputfile->cd();
    TotalPETree->Write();
}
