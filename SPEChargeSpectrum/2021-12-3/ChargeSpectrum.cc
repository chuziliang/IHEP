void ChargeSpectrum()
{
    TFile *ifile = TFile::Open("./Signals.root");
    TH1 *ChargeSpectrum = new TH1F("Charge spectrum","Charge Spectrum",100,0,1);
    TH1 *Amplitude = new TH1F("Amplitude","Amplitude",100,1,5);
    for(int i = 0;i<100000;i++)
    {
        TH1 *tmp = nullptr; 
        //ifile->cd("0");
        double charge = 0;
        tmp = (TH1*)ifile->Get(TString::Format("%1d/signal %1d",i/1000,i));
        double SignalMaxBin =tmp->GetMaximumBin();
        for (int i = SignalMaxBin - 30; i < SignalMaxBin + 70; i++)
        {
            charge += tmp->GetBinContent(i) / 50;
        }
        ChargeSpectrum->Fill(charge);
        if (charge > 0.46 && charge < 0.50)
        {
            Amplitude->Fill(tmp->GetBinContent(tmp->GetMaximumBin()));
        }
        delete tmp;
    }
    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->Divide(2,1);
    c1->cd(1);
    ChargeSpectrum->Draw();
    c1->cd(2);
    Amplitude->Draw();
}