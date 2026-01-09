void EffiencyFitEff()
{
    TFile *HistFile = new TFile("hist_in_different_trigger_Eff.root","read");
    TFile *DataFile = new TFile("hist_in_different_trigger_sim_300_eff.root","read");
    Int_t triggerDACu[20] = {768,665,637,612,590,571,553,536,519,503,486,469,453,435,414,392,369,339,300,241};
    TH1* Hist[20];
    TH1* Data[20]; 
    Double_t scale;
    Double_t ChiSquare = 0;
    Double_t ChiSquareTemp = 0;
    Double_t posibility = 0;
    Double_t TrueEff[20];
    Double_t ConstructedEff[20];
    Int_t EventNum[20];
    Double_t MostFitEff[20];
    for (int k = 0 ; k<20; k++)
    {
        Data[k]  =(TH1*) DataFile->Get(TString::Format("Simulation charge spectrum of %1d DACu",triggerDACu[k]));
        ConstructedEff[k] = Data[k]->Integral()/1000000.0;
        Data[k]->Rebin(10);
        scale = 1.0/Data[k]->Integral();
        Data[k]->Scale(scale);
        // cout<<"-----------------------------------------------------------"<<endl;
        // cout<<TString::Format("charge spectrum of %1d DACu",200+50*k)<<endl;
        Hist[k]=(TH1*) HistFile->Get(TString::Format("charge spectrum of %1d DACu",triggerDACu[k]));
        TrueEff[k] = Hist[k]->Integral()/19951.0;
        Hist[k]->Rebin(10);
        EventNum[k] = Hist[k]->Integral();
        // scale = 1.0/Hist[k]->Integral();
        // Hist[k]->Scale(scale);
    }
    for (int k = 0 ; k<20; k++)
    {
        cout<<"-----------------------------------------------------------"<<endl;
        cout<<TString::Format("charge spectrum of %1d DACu",triggerDACu[k])<<endl;
        for (int i = 0 ; i < 20; i++)
        {
            ChiSquare = -EventNum[k];
            Int_t BinNum = 0;
            for (Int_t j = 0; j < 30; j++)
            {
                if (Data[i]->GetBinContent(j+1) != 0) 
                {
                    ChiSquare += pow(Hist[k]->GetBinContent(j+1),2)/(EventNum[k]*Data[i]->GetBinContent(j+1));
                    BinNum++;
                }
            }
            if ( i == 0 )
            {
                ChiSquareTemp = ChiSquare;
                MostFitEff[k] = ConstructedEff[i];
            }
            if (ChiSquare < ChiSquareTemp )
            {
                ChiSquareTemp = ChiSquare;
                MostFitEff[k] = ConstructedEff[i];
            }
            posibility = ROOT::Math::chisquared_cdf_c(ChiSquare,BinNum-1);
            //ChiSquare = ChiSquare/( BinNum - 1);
            cout<<"Trigger "<<triggerDACu[i]<<"\t"<<"ChiSquare "<<ChiSquare<<endl;
        }
    }
    TCanvas *c2 = new TCanvas("c2","c2",1);
    TGraph* ResultData = new TGraph(20,TrueEff,MostFitEff);
    ResultData->Draw();
    TCanvas *c1 = new TCanvas("c1","c1",1);
	// c1->Divide(4,5);
	// for (int i = 0; i<20; i++)
	// {
	// 	c1->cd(i+1);
	// 	Data[i]->Draw();
	// }
    // TCanvas *c3 = new TCanvas("c3","c3",1);
	// c3->Divide(4,5);
	// for (int i = 0; i<20; i++)
	// {
	// 	c3->cd(i+1);
	// 	Hist[i]->Draw();
	// }
}