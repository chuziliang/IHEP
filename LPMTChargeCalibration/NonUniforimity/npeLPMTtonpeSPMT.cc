//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 
const double R = 19.434; // m
const double position[11] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.95};
const int Points = 10;
const double Energy[5]={0.5,1.16,2,5,10};

void npeLPMTtonpeSPMT(int num)
{
    TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/ChargeSpectrum/ChargeSpectrum%.1fMeV.root",Energy[num]),"read");
    double LPMTMean[Points];
    double SPMTMean[6][Points];
    double LPMTError[Points];
    double SPMTError[6][Points];

    TH1 *LPMTnpeSpectrum;
    TH1 *SPMTnpeSpectrum[6];
    for (int i = 0; i < Points; i++)
    {
        TString LPMTnpeSpectrumName = TString::Format("LPMT charge spectrun %.1fMeV %.1fm",Energy[num],position[i]*R);;
        LPMTnpeSpectrum = (TH1*)inputfile->Get(LPMTnpeSpectrumName);
        LPMTMean[i] = LPMTnpeSpectrum->GetMean();
        LPMTError[i] = LPMTnpeSpectrum->GetMeanError();

        TString SPMTnpeSpectrumName[6];
        for (int id = 0; id < 6; id++)
        {
            SPMTnpeSpectrumName[id] = TString::Format("SPMT %1d charge spectrun %.1fMeV %.1fm",id,Energy[num],position[i]*R);
            SPMTnpeSpectrum[id] = (TH1*)inputfile->Get(SPMTnpeSpectrumName[id]);
            SPMTMean[id][i] = SPMTnpeSpectrum[id]->GetMean();
            SPMTError[id][i] = SPMTnpeSpectrum[id]->GetMeanError();
        }
    }

    TFile *output = new TFile(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/npeLPMTtonpeSPMT%.1fMeV.root",Energy[num]),"RECREATE");
    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(Points,SPMTMean[id],LPMTMean,SPMTError[id],LPMTError);
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