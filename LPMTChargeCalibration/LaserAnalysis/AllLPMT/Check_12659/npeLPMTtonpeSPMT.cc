//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 
const int LPMT_ID = 12659;
const int SPMT_ID[6] = {310629,310809,310812,310932,310935,311049};

void npeLPMTtonpeSPMT()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/Check_12659/CombineChargeSpectrum.root");
    int energy[] = {1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
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
            SPMTMean[id][i] = SPMTnpeSpectrum[id]->GetMean();
            SPMTError[id][i] = SPMTnpeSpectrum[id]->GetMeanError();
        }
    }

    TFile *output = new TFile("npeLPMTtonpeSPMT.root","RECREATE");
    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int i = 0; i < 21; i++)
    {
        cout<<LPMTMean[i]<<endl;
    }
    
    
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT_%1d npe to SPMT_%1d npe",LPMT_ID,SPMT_ID[id]);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(21,SPMTMean[id],LPMTMean,SPMTError[id],LPMTError);
        npeLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeLPMTtonpeSPMT[id]->SetName(GraphName);
        npeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        TF1 *f1 = new TF1("f1","[0]*x",0,10);
        f1->SetParNames("k");
        npeLPMTtonpeSPMT[id]->Fit(f1);
        // npeLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);


        TString CanvasName = TString::Format("C%1d",id);
        Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        Canvas[id]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT[id]->Draw("AP");
        output->cd();
        Canvas[id]->Write();
    }
}