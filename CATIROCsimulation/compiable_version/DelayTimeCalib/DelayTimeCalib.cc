#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void DelayTimeCalib()
{
    TFile *SignalFile = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/PMTsignal/Signals.root");

    TH1 *DelayTimeHist = new TH1F("Delay Time","Delay Time",200,0,200);
    for (int i = 0; i < 200000; i++)
    {
        double DelayTime = 0;
        TH1 *tmpSignal = nullptr;
        TH1 *tmpFSH = nullptr;
        TH1 *tmpSSH = nullptr;

        tmpSignal = (TH1*)SignalFile->Get(TString::Format("%1d/signal %1d",i/1000,i));
        tmpSignal->SetBins(2000,0,2000);
        shaper(tmpSignal,tmpFSH,tmpSSH);

        DelayTime = tmpSSH->GetMaximumBin() - tmpFSH->GetMinimumBin();
        DelayTimeHist->Fill(DelayTime);

        delete tmpSignal;
        delete tmpFSH;
        delete tmpSSH;
    }

    TFile *output = TFile::Open("DelayTime.root","recreate");
    DelayTimeHist->Write();
    cout<<"Delay time should be  "<<DelayTimeHist->GetMaximumBin()<<"ns"<<endl;
    
}