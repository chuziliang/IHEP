void EffiencyFit()
{
    TFile *HistFile = new TFile("hist_in_different_trigger.root","read");
    TFile *DataFile = new TFile("hist_in_different_trigger_sim_300.root","read");
    TH1* Hist[16];
    TH1* Data[16]; 
    Double_t scale;
    Double_t ChiSquare = 0;
    for (int k = 0 ; k<16; k++)
    {
        Data[k]  =(TH1*) DataFile->Get(TString::Format("Simulation charge spectrum of %1d DACu",200+50*k));
        scale = 1.0/Data[k]->Integral();
        Data[k]->Scale(scale);
        // cout<<"-----------------------------------------------------------"<<endl;
        // cout<<TString::Format("charge spectrum of %1d DACu",200+50*k)<<endl;
        Hist[k]=(TH1*) HistFile->Get(TString::Format("charge spectrum of %1d DACu",200+50*k));
        scale = 1.0/Hist[k]->Integral();
        Hist[k]->Scale(scale);
    }
    for (int k = 0 ; k<16; k++)
    {
        cout<<"-----------------------------------------------------------"<<endl;
        cout<<TString::Format("charge spectrum of %1d DACu",200+50*k)<<endl;
        for (int i = 0 ; i < 16; i++)
        {
            ChiSquare = 0;
            Int_t BinNum = 0;
            for (Int_t j = 0; j < 300; j++)
            {
                if (Data[i]->GetBinContent(j+1) != 0) 
                {
                    BinNum++;
                    ChiSquare += pow((Data[i]->GetBinContent(j+1)-Hist[k]->GetBinContent(j+1)),2)/Data[i]->GetBinContent(j+1);
                }
            }
            //ChiSquare = ChiSquare/( BinNum - 1);
            cout<<"Trigger "<<200+50*i<<"\t"<<"ChiSquare "<<ChiSquare<<endl;
        }
    }
}