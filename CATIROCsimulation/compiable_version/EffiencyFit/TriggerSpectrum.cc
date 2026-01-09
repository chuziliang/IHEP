#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void TriggerSpectrum()
{
    TFile *SignalFile = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/PMTsignal/Signals.root");

    TH1* TriggerFSH = new TH1F("TriggerSpectrum","TriggerSpectrum",1000,0,1000);

    for (int i = 0; i < 200000; i++)
    {
        TH1 *tmpSignal = nullptr;
        TH1 *tmpFSH = nullptr;
        TH1 *tmpSSH = nullptr;

        tmpSignal = (TH1*)SignalFile->Get(TString::Format("%1d/signal %1d",i/1000,i));
        shaper(tmpSignal,tmpFSH,tmpSSH);

        double MinSSH;
        double DACu;
        MinSSH = tmpFSH->GetBinContent(tmpFSH->GetMinimumBin());
        DACu = 991 + MinSSH;
        TriggerFSH->Fill(DACu);

        delete tmpSignal;
        delete tmpFSH;
        delete tmpSSH;

        if (i%2000 == 0)
        {
            cout<<i/2000<<endl;
        }
        
    }
    TFile *output = TFile::Open("TriggerSpectrum.root","recreate");
    output->cd();
    TriggerFSH->Write();
}