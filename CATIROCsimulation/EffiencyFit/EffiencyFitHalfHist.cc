void EffiencyFitHalfHist()
{
    TFile *HistFile = new TFile("hist_in_different_trigger_Eff.root","read");
    TFile *DataFile = new TFile("hist_in_different_trigger_sim_300_eff.root","read");
    Int_t triggerDACu[20] = {768,665,637,612,590,571,553,536,519,503,486,469,453,435,414,392,369,339,300,241};
    TH1* Hist[20];
    TH1* Data[20]; 
    Int_t MaximumBin;
    Double_t TrueEffiencyHist[20];
    Double_t EffiencyHist[20];
    Double_t ErrorsHist[20];
    Double_t TrueEffiencyData[20];
    Double_t EffiencyData[20];
    Double_t ErrorsData[20];
    Double_t beforeMax,afterMax;
    Double_t ErrorsA,ErrorsB;
    for (int k = 0 ; k<20; k++)
    {
        Data[k]  =(TH1*) DataFile->Get(TString::Format("Simulation charge spectrum of %1d DACu",triggerDACu[k]));
        MaximumBin = Data[k]->GetMaximumBin();
        beforeMax =  Data[k]->IntegralAndError(0,MaximumBin,ErrorsA);
        afterMax  =  Data[k]->IntegralAndError(MaximumBin,300,ErrorsB);
        EffiencyData[k] = beforeMax/(2*afterMax)+0.5;
        ErrorsData[k] = fabs(EffiencyData[k])*sqrt(pow(ErrorsA/beforeMax,2)+pow(ErrorsB/afterMax,2));
        TrueEffiencyData[k] = Data[k]->Integral()/1000000.0;
        cout<<"Simulation: ADCu = "<<200+50*k<<"    Ture Effiency = "<<TrueEffiencyData[k]<<"    Constructed Effiency = "<<EffiencyData[k]<<"    Error = "<<ErrorsData[k]<<endl;
        Hist[k]=(TH1*) HistFile->Get(TString::Format("charge spectrum of %1d DACu",triggerDACu[k]));
        MaximumBin = Hist[k]->GetMaximumBin();
        beforeMax =  Hist[k]->IntegralAndError(0,MaximumBin,ErrorsA);
        afterMax  =  Hist[k]->IntegralAndError(MaximumBin,300,ErrorsB);
        EffiencyHist[k] = beforeMax/(2*afterMax)+0.5;
        ErrorsHist[k] = fabs(EffiencyHist[k])*sqrt(pow(ErrorsA/beforeMax,2)+pow(ErrorsB/afterMax,2));
        TrueEffiencyHist[k] = Hist[k]->Integral()/19951.0;
        cout<<"Real Data:  ADCu = "<<200+50*k<<"    Ture Effiency = "<<TrueEffiencyHist[k]<<"    Constructed Effiency = "<<EffiencyHist[k]<<"    Error = "<<ErrorsHist[k]<<endl;
    }
    TGraph* ResultHist = new TGraphErrors(20,TrueEffiencyHist,EffiencyHist,nullptr,ErrorsHist);
    ResultHist->SetTitle("efficiency construction");
    ResultHist->GetXaxis()->SetTitle("Real efficiency");
    ResultHist->GetYaxis()->SetTitle("Constructed efficiency");
    TGraph* ResultData = new TGraphErrors(20,TrueEffiencyData,EffiencyData,nullptr,ErrorsData);
    ResultData->SetTitle("efficiency construction (simulation)");
    ResultData->GetXaxis()->SetTitle("Real efficiency");
    ResultData->GetYaxis()->SetTitle("Constructed efficiency");
    TCanvas* c1 = new TCanvas("c1","c1",800,400);
    c1->Divide(2,1);
    c1->cd(1);
    ResultHist->Draw();
    c1->cd(2);
    ResultData->Draw();
}