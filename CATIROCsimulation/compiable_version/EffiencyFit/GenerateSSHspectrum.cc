#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void GenerateSSHspectrum()
{
    const double DelayTime =  37;

    const int nq = 20;
    double trigger[nq];
    for (int i = 0; i < nq; i++)
    {
        trigger[i] = 500 + (990.0-500.0)*(double)(i+1)/nq;
    }
    

    TFile *SignalFile = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/PMTsignal/Signals.root");

    TH1 *ChargeSpectrum[nq];

    for (int i = 0; i < nq; i++)
    {
        ChargeSpectrum[i] = new TH1F(TString::Format("Trigger = %2.0fDACu",trigger[i]),TString::Format("Trigger = %2.0fDACu",trigger[i]),1000,0,500);
    }


    for (int i = 200000; i < 200000+200000; i++)
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
            cout<<(i-200000)/2000<<endl;
        }
        
    }

    TFile *output = TFile::Open("SSHspectrum.root","recreate");

    for (int i = 0; i < nq; i++)
    {
        output->cd();
        ChargeSpectrum[i]->Write();
    }
    
    output->Close();
    SignalFile->Close();
    
}