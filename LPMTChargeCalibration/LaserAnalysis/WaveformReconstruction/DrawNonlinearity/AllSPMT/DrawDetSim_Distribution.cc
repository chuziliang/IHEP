int DrawDetSim_Distribution()
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    TH1 *SPMTHist[25600];
    for (int id = 0; id < 25600; id++)
    {
        TString histname = TString::Format("SPMT_%1d hist",id);
        SPMTHist[id] = new TH1F(histname,histname,30,0,30); 
    }

    TH1 *SPMTHistTotal;
    SPMTHistTotal = new TH1F("Total","Total",30,0,30);

    TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/ChargeSpectrum_DetSim/%1dMevCharge.root",1500));
    // if (!inputfile)
    // {
    //     continue;
    // }

    for (int j = 0; j < numbers[20]; j++)
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
                SPMTHist[id]->Fill(SPMTPE[id]);
                SPMTHistTotal->Fill(SPMTPE[id]);
            }
        }
        delete charge;
    }
    inputfile->Close();

    TFile *outputfile = TFile::Open("DetSim_Distribution.root","RECREATE");
    for (int id = 0; id < 256; id++)
    {
        outputfile->mkdir(TString::Format("%1d",id));
    }
    

    TF1 *Poisson = new TF1("Possion","[0]*TMath::PoissonI(x,[1])",0,30);

    for (int id = 0; id < 25600; id++)
    {
        Poisson->SetParameter(0,SPMTHist[id]->GetEntries());
        Poisson->SetParameter(1,SPMTHist[id]->GetMean());
        SPMTHist[id]->Fit(Poisson);
        outputfile->cd(TString::Format("%1d",id/100));
        SPMTHist[id]->Write();
    }

    Poisson->SetParameter(0,SPMTHistTotal->GetEntries());
    Poisson->SetParameter(1,SPMTHistTotal->GetMean());
    SPMTHistTotal->Fit(Poisson);
    outputfile->cd();
    SPMTHistTotal->Write();

    outputfile->Close();
    return 0;

}