Double_t FitFunc(Double_t *x,Double_t *par)
{
    Double_t arg1 = 0;
    Double_t arg2 = 0;
    //if ((par[1]-par[3]) != 0)
    {
        arg1 = std::log(x[0] - par[3]) - std::log(par[1]-par[3]);
    }
    if (par[2] != 0)
    {
        arg2 = arg1/par[2];
    }
    Double_t fitval = par[0]*std::exp(-0.5*arg2*arg2);
    return fitval;
}

void SignalFitOneSignal()
{
    TFile *ifile = TFile::Open("./Signals.root");

    TH1 *tmp = nullptr; 
    //ifile->cd("0");
    int SignalStartPoint = 0;
    int SignalEndPoint = 0;
    tmp = (TH1*)ifile->Get("0/signal 2");
    double SignalMax = tmp->GetBinContent(tmp->GetMaximumBin());
    for (int i = tmp->GetMaximumBin(); i > tmp->GetMaximumBin()-100; i--)
    {
        if (tmp->GetBinContent(i)<0.2*SignalMax)
        {
            SignalStartPoint = i;
            break;
        }
    }
    for (int i = tmp->GetMaximumBin(); i < tmp->GetMaximumBin()+300; i++)
    {
        if (tmp->GetBinContent(i)<0.2*SignalMax)
        {
            SignalEndPoint = i;
            break;
        }
    }
    
    TF1 *func = new TF1("fit",FitFunc,SignalStartPoint+3,SignalEndPoint+80,4);
    func->SetParameters(SignalMax,tmp->GetMaximumBin(),0.3,SignalStartPoint-3);
    func->SetParNames ("amplitude","peak_position","shape_factor","start_point");
    func->SetParLimits(0,SignalMax-0.2,SignalMax+0.2);
    func->SetParLimits(1,tmp->GetMaximumBin()-3,tmp->GetMaximumBin()+3);
    //func->SetParLimits(2,0,0.5);
    func->SetParLimits(3,SignalStartPoint-20,SignalStartPoint-1);
    tmp->Fit("fit","RM WW");
    tmp->Draw();

    double AM = func->GetParameter(0);
    cout<<AM<<endl;
    double PP = func->GetParameter(1);
    cout<<PP<<endl;
    double SP = func->GetParameter(3);
    double raise_time = func->GetX(0.9*AM,PP-10,PP)-func->GetX(0.1*AM,SP+0.01,PP);
    cout<<"raise time = "<<func->GetX(0.9*AM,PP-10,PP)<<" - "<<func->GetX(0.1*AM,SP+0.01,PP)<<" = "<<raise_time<<endl;
    
    func->Draw("same");

}