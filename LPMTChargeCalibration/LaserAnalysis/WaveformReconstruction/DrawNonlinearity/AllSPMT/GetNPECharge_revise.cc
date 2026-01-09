//由于电子学非线性，电荷谱不再符合泊松卷积高斯，很难拟合

Double_t specFit(Double_t* x, Double_t* par) {
	double f1, f2;
	double f = 0;
	for (int n = 1; n <= 10; n ++) {
		f1 = TMath::Gaus(x[0],par[0],sqrt(n)*par[1],kTRUE);
		f2 = TMath::PoissonI(n, par[2]);
		f += f2 * f1;
	}
	return f * par[3];
}//定义拟合函数，用来拟合电荷谱

int GetNPECharge_revise()
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    double MeanDarkRate;
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
    TTree *PMTData = (TTree*)DarkRate_file->Get("data");
    double darkRate;
    PMTData->SetBranchAddress("darkRate",&darkRate);
    for (int ID = 0; ID < 25600; ID++)
    {
        PMTData->GetEntry(ID);
        MeanDarkRate += darkRate;
    }
    MeanDarkRate /= 25600;
    DarkRate_file->Close();

    TH1 *SPMTHist[21];
    for (int i = 0; i < 21; i++)
    {
        TString histname = TString::Format("%1d MeV SPMT hist",energy[i]);
        SPMTHist[i] = new TH1F(histname,histname,300,0,30); 
    }

    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/ChargeSpectrum/%1dMevCharge.root",energy[i]));
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
                    SPMTHist[i]->Fill(SPMTPE[id]);
                }
            }
            delete charge;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("TruenPECharge.root","RECREATE");
    outputfile->mkdir("Spectrum");

    double SPMTMeanCharge;
    double ErrorSPMTMeanCharge;
    int Energy;

    TTree* charge = new TTree("charge","npe charge");
    charge->Branch("SPMTMeanCharge",&SPMTMeanCharge,"SPMTMeanCharge/D");
    charge->Branch("ErrorSPMTMeanCharge",&ErrorSPMTMeanCharge,"ErrorSPMTMeanCharge/D");
    charge->Branch("energy",&Energy,"energy/I");

    for (int i = 0; i < 21; i++)
    {
        outputfile->cd("Spectrum");
        double entries = SPMTHist[i]->GetEntries();
        if (entries > 2)
        {
            TF1* func = new TF1("SpecModel", specFit,1.0/3.0, 30, 4);
            func->SetParameters(1, 33.2/100.0, SPMTHist[i]->GetMean(), entries - SPMTHist[i]->GetBinContent(1));
            func->FixParameter(0,1);
            func->FixParameter(1,33.2/100.0);
            func->SetParNames("Q_{0}", "#sigma_{0}", "u", "A");
            double para[4];

            SPMTHist[i]->Fit(func,"R B");
            func->GetParameters(para);
            SPMTMeanCharge = para[2];
            ErrorSPMTMeanCharge = func->GetParError(2);
            SPMTMeanCharge -= MeanDarkRate*1e-6;
            double TempError = sqrt(ErrorSPMTMeanCharge*ErrorSPMTMeanCharge + MeanDarkRate*1e-6*MeanDarkRate*1e-6/entries/entries);
            ErrorSPMTMeanCharge = TempError;
            outputfile->cd("Spectrum");
            SPMTHist[i]->Write();
            outputfile->cd();
            Energy = energy[i];
            charge->Fill();
        }
    }
    charge->Write();
    outputfile->Close();
    return 0;

}