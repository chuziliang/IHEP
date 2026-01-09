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

void SignalFit()
{
    TFile *ifile = TFile::Open("./Signals.root");

    TH1 *RaiseTime = new TH1F("Raise Time","Raise Time",100,0,10);
    TH1 *PeakPos = new TH1F("Peak Position","Peak Position",100,350,450);
    TH1 *Amplitude = new TH1F("Amplitude","Amplitude",200,0,20);
    TH1 *ShapeFactor = new TH1F("Shape Factor","Shape Factor",100,0,1);
    TH1 *DeltaTime = new TH1F("PeakPosition - StartPoint","PeakPosition - StartPoint",100,0,10);

    TH2 *JointRD = new TH2F("Joint of Raise and Delta time","Joint of Raise and Delta time",100,0,10,100,0,20);
    TH2 *JointAD = new TH2F("Joint of Amplitude and Delta time","Joint of Amplitude and Delta time",200,0,6,100,0,20);
    TH2 *JointSD = new TH2F("Joint of Shape Factor and Delta time","Joint of Shape Factor and Delta time",100,0,1,100,0,20);
    TH2 *JointAS = new TH2F("Joint of Amplitude and Shape Factor","Joint of Amplitude and Shape Factor",200,0,6,100,0,1);

    for(int i = 0;i<10000;i++)
    {
        TH1 *tmp = nullptr; 
        //ifile->cd("0");
        int SignalStartPoint = 0;
        int SignalEndPoint = 0;
        tmp = (TH1*)ifile->Get(TString::Format("%1d/signal %1d",i/1000,i));
        double RT,PP,AM,SF,DT,SP;
        RT=0,PP=0,AM=0,SF=0,DT=0,SP=0;
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
        
        TF1 *func = new TF1("fit",FitFunc,SignalStartPoint,SignalEndPoint+80,4);
        func->SetParameters(SignalMax,tmp->GetMaximumBin(),0.3,SignalStartPoint-3);
        func->SetParNames ("amplitude","peak_position","shape_factor","start_point");
        func->SetParLimits(0,SignalMax-0.2,SignalMax+0.2);
        func->SetParLimits(1,tmp->GetMaximumBin()-3,tmp->GetMaximumBin()+3);
        // func->SetParLimits(2,0,1);
        func->SetParLimits(3,SignalStartPoint-20,SignalStartPoint-1);
        tmp->Fit("fit","RM WW Q");

        TF1 *landuFunc = new TF1("Landau function","landau",SignalStartPoint - 20, SiganalEndPoint+80);

        AM = func->GetParameter(0);
        PP = func->GetParameter(1);
        SF = func->GetParameter(2);
        SP = func->GetParameter(3);
        RT = func->GetX(0.9*AM,SP+1,PP)-func->GetX(0.1*AM,SP+0.01,PP);
        DT = PP-SP;
        
        // if (RT > 7)
        // {
        //     TCanvas *c3 = new TCanvas("c3","c3",1);
        //     tmp->Draw();
        //     func->Draw("same");
        //     break;
        // }
        

        RaiseTime->Fill(RT);
        PeakPos->Fill(PP);
        Amplitude->Fill(AM);
        ShapeFactor->Fill(SF);
        DeltaTime->Fill(DT);

        JointRD->Fill(RT,DT);
        JointAD->Fill(AM,DT);
        JointSD->Fill(SF,DT);
        JointAS->Fill(AM,SF);
        
        delete tmp;
        delete func;
    }

    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->Divide(3,2);
    c1->cd(1);
    RaiseTime->Draw();
    c1->cd(2);
    PeakPos->Draw();
    c1->cd(3);
    Amplitude->Draw();
    c1->cd(4);
    ShapeFactor->Draw();
    c1->cd(5);
    DeltaTime->Draw();

    TCanvas *c2 = new TCanvas("c2","c2",1);
    c2->Divide(2,2);
    c2->cd(1);
    JointRD->Draw("COL");
    c2->cd(2);
    JointAD->Draw("COL");
    c2->cd(3);
    JointSD->Draw("COL");
    c2->cd(4);
    JointAS->Draw("COL");

}