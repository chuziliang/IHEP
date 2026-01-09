void npeLPMTtonpeSPMT()
{
    int energy[]={1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
    int numbers[]={100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};

    TH1 *LPMTHist[200];
    for (int nPE = 0; nPE < 200; nPE++)
    {
        TString histname = TString::Format("%1d PE LPMT hist",nPE);
        LPMTHist[nPE] = new TH1F(histname,histname,2000,0,200); 
    }

    TH1 *SPMTHist[6][200];
    for (int id = 0; id < 6; id++)
    {
        for (int nPE = 0; nPE < 200; nPE++)
        {
            TString histname = TString::Format("%1d PE SPMT_%1d hist",nPE,id);
            SPMTHist[id][nPE] = new TH1F(histname,histname,300,0,30); 
        }
    }
    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy[i]));
        if (!inputfile)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double LPMTPE = 0;
            double SPMTPE[6] = {0,0,0,0,0,0};
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("LPMTCharge",&LPMTPE);
            charge->SetBranchAddress("SPMTCharge",SPMTPE);
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                int npe = (int)(LPMTPE+0.5);
                LPMTHist[npe]->Fill(LPMTPE);
                for (int id = 0; id < 6; id++)
                {
                    SPMTHist[id][npe]->Fill(SPMTPE[id]);
                }
            }
            delete charge;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("npeLPMTtonpeSPMT.root","RECREATE");
    outputfile->mkdir("Spectrum");
    
    double LPMTMeanCharge[200];
    double ErrorLPMTMeanCharge[200];
    double SPMTMeanCharge[6][200];
    double ErrorSPMTMeanCharge[6][200];
    int n = 0;
    for (int nPE = 1; nPE < 200; nPE++)
    {
        
        outputfile->cd("Spectrum");
        LPMTHist[nPE]->Write();
        for (int id = 0; id < 6; id++)
        {
            SPMTHist[id][nPE]->Write();
        }
        
        double entries = LPMTHist[nPE]->GetEntries();
        if (entries > 2)
        {
            LPMTMeanCharge[n] = LPMTHist[nPE]->GetMean();
            ErrorLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*LPMTHist[nPE]->GetMeanError();
            for (int id = 0; id < 6; id++)
            {
                SPMTMeanCharge[id][n] = SPMTHist[id][nPE]->GetMean();
                ErrorSPMTMeanCharge[id][n] = (sqrt(entries)/sqrt(entries - 1))*SPMTHist[id][nPE]->GetMeanError();
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
  
}