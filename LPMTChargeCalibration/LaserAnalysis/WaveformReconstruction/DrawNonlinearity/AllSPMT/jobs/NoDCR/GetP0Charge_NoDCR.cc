int GetP0Charge_NoDCR()
{
    int energy[]={1,1500};
    int numbers[]={50,200};

    TH1 *SPMTHist[25600][2];
    for (int id = 0; id < 25600; id++)
    {
        for (int i = 0; i < 2; i++)
        {
            TString histname = TString::Format("%1d MeV SPMT_%1d hist",energy[i],id);
            SPMTHist[id][i] = new TH1F(histname,histname,300,0,30); 
        }
    }

    TH1 *TrueSPMTHist[25600][2];
    for (int id = 0; id < 25600; id++)
    {
        for (int i = 0; i < 2; i++)
        {
            TString histname = TString::Format("%1d MeV SPMT_%1d True hist",energy[i],id);
            TrueSPMTHist[id][i] = new TH1F(histname,histname,300,0,30); 
        }
    }

    for (int i = 0; i < 2; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/ChargeSpectrum/%1dMevCharge_NoDCR.root",energy[i]),"read");
        if (!inputfile)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double SPMTPE[25600];
            double TrueSPMTPE[25600];
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("SPMTCharge",SPMTPE);
            charge->SetBranchAddress("TrueSPMTCharge",TrueSPMTPE);
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                for (int id = 0; id < 25600; id++)
                {
                    SPMTHist[id][i]->Fill(SPMTPE[id]);
                    TrueSPMTHist[id][i]->Fill(TrueSPMTPE[id]);
                }
            }
            delete charge;
            cout<<j<<endl;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("P0Charge_NoDCR.root","RECREATE");
    outputfile->mkdir("Spectrum");

    double SPMTMeanCharge[25600];
    double ErrorSPMTMeanCharge[25600];
    double TrueSPMTMeanCharge[25600];
    double ErrorTrueSPMTMeanCharge[25600];
    int Energy;

    TTree* charge = new TTree("charge","npe charge");
    charge->Branch("SPMTMeanCharge",SPMTMeanCharge,"SPMTMeanCharge[25600]/D");
    charge->Branch("ErrorSPMTMeanCharge",ErrorSPMTMeanCharge,"ErrorSPMTMeanCharge[25600]/D");
    charge->Branch("TrueSPMTMeanCharge",TrueSPMTMeanCharge,"TrueSPMTMeanCharge[25600]/D");
    charge->Branch("ErrorTrueSPMTMeanCharge",ErrorTrueSPMTMeanCharge,"ErrorTrueSPMTMeanCharge[25600]/D");
    charge->Branch("energy",&Energy,"energy/I");

    for (int i = 0; i < 2; i++)
    {
        double entries = SPMTHist[0][i]->GetEntries();
        if (entries > 1)
        {
            for (int id = 0; id < 25600; id++)
            {
                double P0 = (double)SPMTHist[id][i]->GetBinContent(1)/SPMTHist[id][i]->GetEntries();
                if (P0 != 0)
                {
                    SPMTMeanCharge[id] = -TMath::Log(P0);
                    double miuP0 = P0*(1 - P0)/SPMTHist[id][i]->GetEntries();
                    miuP0 = sqrt(miuP0);
                    ErrorSPMTMeanCharge[id] = miuP0/P0;
                }
                else
                {
                    SPMTMeanCharge[id] = -1;
                    ErrorSPMTMeanCharge[id] = -1;
                }
                TrueSPMTMeanCharge[id] = TrueSPMTHist[id][i]->GetMean();
                ErrorTrueSPMTMeanCharge[id] = TrueSPMTHist[id][i]->GetMeanError();
            }
            Energy = energy[i];
            charge->Fill();
        }
    }
    charge->Write();
    outputfile->Close();
    return 0;

}