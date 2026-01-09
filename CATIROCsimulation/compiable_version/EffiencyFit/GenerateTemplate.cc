#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void GenerateTemplate()
{
    const double DelayTime =  37;

    TFile *DACuSpectrumFile = TFile::Open("TriggerSpectrum.root");
    TH1 *DACuSpectrum = nullptr;
    DACuSpectrum = (TH1*)DACuSpectrumFile->Get("TriggerSpectrum");
    const int nq = 100;
    double trigger[nq];
    double quantiles[nq];
    for (int i = 0; i < nq; i++)
    {
        quantiles[i] = (double)(i+1)/nq;
    }
    DACuSpectrum->GetQuantiles(nq,trigger,quantiles);
    DACuSpectrumFile->Close();
    

    TFile *SignalFile = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/PMTsignal/Signals.root");

    TH1 *ChargeSpectrum[nq];

    for (int i = 0; i < nq; i++)
    {
        ChargeSpectrum[i] = new TH1F(TString::Format("%2.0f%% Spectrum",100*quantiles[i]),TString::Format("%3.2f%% Spectrum",100*quantiles[i]),1000,0,500);
    }
    

    for (int i = 0; i < 200000; i++)
    {
        TH1 *tmpSignal = nullptr;
        TH1 *tmpFSH = nullptr;
        TH1 *tmpSSH = nullptr;

        tmpSignal = (TH1*)SignalFile->Get(TString::Format("%1d/signal %1d",i/1000,i));
        tmpSignal->SetBins(2000,0,2000);

        shaper(tmpSignal,tmpFSH,tmpSSH);

        for (int j = 0; j < nq; j++)
        {
            int Bin = 0;
            double value = 0;
            double charge = 0;
            for (int t = 0; t < 2000; t++)
            {
                Bin = t+1;
                value = tmpFSH->GetBinContent(Bin);
                if ( value+991 <= trigger[j] )
                {
                    charge = 66.9 + tmpSSH->GetBinContent(Bin+DelayTime);
                    ChargeSpectrum[j]->Fill(charge);
                    break;
                }  
            }
        }
        delete tmpSignal;
        delete tmpFSH;
        delete tmpSSH;

        if (i%2000 == 0)
        {
            cout<<i/2000<<endl;
        }
        
    }

    

    TFile *output = TFile::Open("Template.root","recreate");

    for (int i = 0; i < nq; i++)
    {
        output->cd();
        ChargeSpectrum[i]->Write();
    }
    
    output->Close();
    SignalFile->Close();
    
}