//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。采用P(0)的方法
void npeLPMTtonpeSPMTP0()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/CombineChargeSpectrum.root");
    int energy[] = {1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
    double LPMTMean[21];
    double SPMTMean[6][21];
    double LPMTError[21];
    double SPMTError[6][21];

    TH1 *LPMTnpeSpectrum;
    TH1 *SPMTnpeSpectrum[6];
    for (int i = 0; i < 21; i++)
    {
        TString LPMTnpeSpectrumName = TString::Format("LPMT nPE spectrun %1dMeV",energy[i]);
        LPMTnpeSpectrum = (TH1*)inputfile->Get(LPMTnpeSpectrumName);
        LPMTMean[i] = LPMTnpeSpectrum->GetMean();
        LPMTError[i] = LPMTnpeSpectrum->GetMeanError();

        TString SPMTnpeSpectrumName[6];
        for (int id = 0; id < 6; id++)
        {
            SPMTnpeSpectrumName[id] = TString::Format("SPMT_%1d nPE spectrun %1dMeV",id,energy[i]);
            SPMTnpeSpectrum[id] = (TH1*)inputfile->Get(SPMTnpeSpectrumName[id]);
            double P0 = (double)SPMTnpeSpectrum[id]->GetBinContent(1)/SPMTnpeSpectrum[id]->GetEntries();
            SPMTMean[id][i] = -TMath::Log(P0);
            double miuP0 = P0*(1 - P0)/SPMTnpeSpectrum[id]->GetEntries();
            miuP0 = sqrt(miuP0);
            SPMTError[id][i] = miuP0/P0;
        }
    }

    TFile *output = new TFile("npeLPMTtonpeSPMT.root","RECREATE");
    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d p(0)",id);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(21,SPMTMean[id],LPMTMean,SPMTError[id],LPMTError);
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
        output->cd();
        npeLPMTtonpeSPMT[id]->Write();
    }
}