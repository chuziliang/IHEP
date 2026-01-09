//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 

const int Points = 21;
const double Degree[6]={4.9,9.9,19.7,29.6,39.5,44.4};

void npeLPMTtonpeSPMT(int DegreeNum)
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/CombineChargeSpectrum/ChargeSpectrum%.1fDEG.root",Degree[DegreeNum]),"read");
    double LPMTMean[Points];
    double SPMTMean[6][Points];
    double LPMTError[Points];
    double SPMTError[6][Points];

    TH1 *LPMTnpeSpectrum;
    TH1 *SPMTnpeSpectrum[6];
    for (int i = 0; i < Points; i++)
    {
        TString LPMTnpeSpectrumName = TString::Format("LPMT nPE spectrun %1dMeV %.1fDEG",energy[i],Degree[DegreeNum]);
        LPMTnpeSpectrum = (TH1*)inputfile->Get(LPMTnpeSpectrumName);
        LPMTMean[i] = LPMTnpeSpectrum->GetMean();
        LPMTError[i] = LPMTnpeSpectrum->GetMeanError();

        TString SPMTnpeSpectrumName[6];
        for (int id = 0; id < 6; id++)
        {
            SPMTnpeSpectrumName[id] = TString::Format("SPMT_%1d nPE spectrun %1dMeV %.1fDEG",id,energy[i],Degree[DegreeNum]);
            SPMTnpeSpectrum[id] = (TH1*)inputfile->Get(SPMTnpeSpectrumName[id]);
            SPMTMean[id][i] = SPMTnpeSpectrum[id]->GetMean();
            SPMTError[id][i] = SPMTnpeSpectrum[id]->GetMeanError();
        }
    }
    inputfile->Close();

    TFile *output = new TFile(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/npeLPMTtonpeSPMTroot/npeLPMTtonpeSPMT%.1fDEG.root",Degree[DegreeNum]),"RECREATE");
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