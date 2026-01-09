int GetDetsimCharge()
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    TH1 *SPMTHist[25600][21];
    for (int id = 0; id < 25600; id++)
    {
        for (int i = 0; i < 21; i++)
        {
            TString histname = TString::Format("%1d MeV SPMT_%1d hist",energy[i],id);
            SPMTHist[id][i] = new TH1F(histname,histname,300,0,30); 
        }
    }

    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/ChargeSpectrum_DetSim/%1dMevCharge.root",energy[i]));
        if (!inputfile)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double SPMTPE[25600];
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("SPMTCharge",SPMTPE);
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                for (int id = 0; id < 25600; id++)
                {
                    SPMTHist[id][i]->Fill(SPMTPE[id]);
                }
            }
            delete charge;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("DetsimCharge.root","RECREATE");
    outputfile->mkdir("Spectrum");

    double SPMTMeanCharge[25600];
    double ErrorSPMTMeanCharge[25600];
    int Energy;

    TTree* charge = new TTree("charge","npe charge");
    charge->Branch("SPMTMeanCharge",SPMTMeanCharge,"SPMTMeanCharge[25600]/D");
    charge->Branch("ErrorSPMTMeanCharge",ErrorSPMTMeanCharge,"ErrorSPMTMeanCharge[25600]/D");
    charge->Branch("energy",&Energy,"energy/I");

    for (int i = 0; i < 21; i++)
    {
        double entries = SPMTHist[0][i]->GetEntries();
        if (entries > 2)
        {
            for (int id = 0; id < 25600; id++)
            {
                SPMTMeanCharge[id] = SPMTHist[id][i]->GetMean();
                ErrorSPMTMeanCharge[id] = (sqrt(entries)/sqrt(entries - 1))*SPMTHist[id][i]->GetMeanError();
            }
            Energy = energy[i];
            charge->Fill();
        }
    }
    charge->Write();
    outputfile->Close();
    return 0;

}