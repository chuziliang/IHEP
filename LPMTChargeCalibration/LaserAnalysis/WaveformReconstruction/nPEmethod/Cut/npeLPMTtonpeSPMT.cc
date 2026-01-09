void npeLPMTtonpeSPMT()
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    TH1 *LPMTHist[21];
    for (int i = 0; i < 21; i++)
    {
        TString histname = TString::Format("%1d MeV LPMT hist",energy[i]);
        LPMTHist[i] = new TH1F(histname,histname,2000,0,200); 
    }

    TH1 *SPMTHist[6][21];
    for (int id = 0; id < 6; id++)
    {
        for (int i = 0; i < 21; i++)
        {
            TString histname = TString::Format("%1d MeV SPMT_%1d hist",energy[i],id);
            SPMTHist[id][i] = new TH1F(histname,histname,300,0,30); 
        }
    }

    TH1 *TrueHist[21]; //
    for (int i = 0; i < 21; i++) //
    {
        TString histname = TString::Format("%1d MeV LPMT ElecSim hist",energy[i]);
        TrueHist[i] = new TH1F(histname,histname,2000,0,200); 
    }

    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut/%1dMevCharge.root",energy[i]));
        if (!inputfile)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double LPMTPE = 0;
            double SPMTPE[6] = {0,0,0,0,0,0};
            double TruePE = 0; //
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("LPMTCharge",&LPMTPE);
            charge->SetBranchAddress("SPMTCharge",SPMTPE);
            charge->SetBranchAddress("TrueCharge",&TruePE); //
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                LPMTHist[i]->Fill(LPMTPE);
                TrueHist[i]->Fill(TruePE); //
                for (int id = 0; id < 6; id++)
                {
                    SPMTHist[id][i]->Fill(SPMTPE[id]);
                }
            }
            delete charge;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("npeLPMTtonpeSPMT.root","RECREATE");
    outputfile->mkdir("Spectrum");
    
    double LPMTMeanCharge[21];
    double ErrorLPMTMeanCharge[21];
    double TrueLPMTMeanCharge[21]; //
    double ErrorTrueLPMTMeanCharge[21]; //
    double SPMTMeanCharge[6][21];
    double ErrorSPMTMeanCharge[6][21];
    int n = 0;
    for (int i = 0; i < 21; i++)
    {
        
        outputfile->cd("Spectrum");
        LPMTHist[i]->Write();
        TrueHist[i]->Write(); //
        for (int id = 0; id < 6; id++)
        {
            SPMTHist[id][i]->Write();
        }
        
        double entries = LPMTHist[i]->GetEntries();
        if (entries > 2)
        {
            LPMTMeanCharge[n] = LPMTHist[i]->GetMean();
            ErrorLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*LPMTHist[i]->GetMeanError();
            TrueLPMTMeanCharge[n] = TrueHist[i]->GetMean(); //
            ErrorTrueLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*TrueHist[i]->GetMeanError(); //
            for (int id = 0; id < 6; id++)
            {
                SPMTMeanCharge[id][n] = SPMTHist[id][i]->GetMean();
                ErrorSPMTMeanCharge[id][n] = (sqrt(entries)/sqrt(entries - 1))*SPMTHist[id][i]->GetMeanError();
            }
            n++;
        }

    }

    
    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],LPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorLPMTMeanCharge);
        npeLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeLPMTtonpeSPMT[id]->SetName(GraphName);
        npeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        npeLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);


        TString CanvasName = TString::Format("C%1d",id);
        Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        Canvas[id]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT[id]->Draw("AP");
        outputfile->cd();
        npeLPMTtonpeSPMT[id]->Write();
    }

    TGraphErrors *npeTrueLPMTtonpeSPMT[6]; //
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("True LPMT npe to SPMT_%1d npe",id);
        npeTrueLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],TrueLPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorTrueLPMTMeanCharge);
        npeTrueLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeTrueLPMTtonpeSPMT[id]->SetName(GraphName);
        npeTrueLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeTrueLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        npeTrueLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);

        outputfile->cd();
        npeTrueLPMTtonpeSPMT[id]->Write();
    }
  
}