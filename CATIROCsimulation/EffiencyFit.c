void EffiencyFit()
{
    TFile *HistFile = new TFile("hist_in_different_trigger.root","read");
    TFile *DataFile = new TFile("effiency_fit_data.root","read");
    TH1* Hist[16];
    TH1* Data =(TH1*) DataFile->Get("effiency_fit_data");
    Double_t scale;
    scale = 1.0/Data->Integral();
    Data->Scale(scale);
    Double_t ChiSquare[16];
    for (int i = 0 ; i < 16; i++)
    {
        ChiSquare[i] = 0;
        Hist[i]=(TH1*) HistFile->Get(TString::Format("charge spectrum of %1d DACu",200+50*i));
        scale = 1.0/Hist[i]->Integral();
        Hist[i]->Scale(scale);
        for (Int_t j = 0; j < 1008; j++)
        {
            if (Hist[i]->GetBinContent(j+1) != 0) ChiSquare[i] += pow((Hist[i]->GetBinContent(j+1)-Data->GetBinContent(j+1)),2)/Hist[i]->GetBinContent(j+1);
        }
        cout<<"Trigger "<<200+50*i<<"\t"<<"ChiSquare "<<ChiSquare[i]<<endl;
    }
    TCanvas* c1 = new TCanvas("c1","c1",1);
    c1->Divide(1,2);
    c1->cd(1);
    Data->Draw();
    c1->cd(2);
    Hist[9]->Draw();
}