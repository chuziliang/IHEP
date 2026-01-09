double MeanRatio = 1;


void DrawNonlinearity()
{
    // TH2 *output = new TH2F("Calib charge to true charge","Calib charge to true charge",200,0,200,2000,0,200);
    TH1 *CalibHist[20];
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/SPMTElectronicsNonlinearity/Nonlinerity.root","read");

    for (int nPE = 0; nPE < 20; nPE++)
    {
        TString histname = TString::Format("%1d PE calib charge",nPE);
        CalibHist[nPE] = (TH1*)inputfile->Get(histname); 
    }
    
    // inputfile->Close();

    double MeanNonlinearity[20];
    double ErrorNonlinearity[20];
    double trueCharge[20];
    double ErrortrueCharge[20];

    double RMS[20];
    double ErrorRMS[20];
    int n = 0;
    for (int nPE = 1; nPE < 13; nPE++)
    {
        double entries = CalibHist[nPE]->GetEntries();
        if (entries > 1)
        {
            double Mean = (CalibHist[nPE]->GetMean())/MeanRatio;
            double MeanError = (CalibHist[nPE]->GetMeanError())/MeanRatio;
            MeanNonlinearity[n] = ( Mean - (double)nPE)/(double)nPE;
            trueCharge[n] = nPE;
            ErrorNonlinearity[n] = (sqrt(entries)/sqrt(entries - 1))*MeanError;
            ErrorNonlinearity[n] /= (double)nPE;
            ErrortrueCharge[n] = 0;

            RMS[n] = CalibHist[nPE]->GetRMS()/CalibHist[nPE]->GetMean();
            ErrorRMS[n] = RMS[n]*sqrt((CalibHist[nPE]->GetRMSError()*CalibHist[nPE]->GetRMSError())/(CalibHist[nPE]->GetRMS()*CalibHist[nPE]->GetRMS()) + (CalibHist[nPE]->GetMeanError()*CalibHist[nPE]->GetMeanError())/(CalibHist[nPE]->GetMean()*CalibHist[nPE]->GetMean()));
            n++;
        }   
    }
    TH1* GetX = new TH1F("x","x",20,0,20);
    TH1* GetY = new TH1F("y","y",20000,0,200);
    for (int nPE = 13; nPE < 20; nPE++)
    {
        double entries = CalibHist[nPE]->GetEntries();
        for (int i = 0; i < entries; i++)
        {
            GetX->Fill(nPE);
        }
        
        // GetX->SetBinContent(nPE+1,entries);
        GetY->Add(CalibHist[nPE]);
    }
    double Mean = (GetY->GetMean())/MeanRatio;
    double MeanError = (GetY->GetMeanError())/MeanRatio;
    MeanNonlinearity[n] = ( Mean - GetX->GetMean())/GetX->GetMean();
    trueCharge[n] = GetX->GetMean();
    ErrorNonlinearity[n] = sqrt((GetX->GetMeanError()*GetX->GetMeanError())/(GetX->GetMean()*GetX->GetMean()) + (GetY->GetMeanError()*GetY->GetMeanError())/(GetY->GetMean()*GetY->GetMean()));
    ErrorNonlinearity[n] *= GetX->GetMean()/GetY->GetMean();
    ErrortrueCharge[n] = GetX->GetMeanError();

    RMS[n] = GetY->GetRMS()/GetY->GetMean();
    ErrorRMS[n] = RMS[n]*sqrt((GetY->GetRMSError()*GetY->GetRMSError())/(GetY->GetRMS()*GetY->GetRMS()) + (GetY->GetMeanError()*GetY->GetMeanError())/(GetY->GetMean()*GetY->GetMean()));
    n++;

//    TCanvas *c1 = new TCanvas("c1","c1",1);
//
//    TPad *pad1 = new TPad("pad1", "pad1", 0.00, 0.00, 0.50, 1.00);
//    TPad *pad2 = new TPad("pad1", "pad1", 0.50, 0, 1.00, 1.00);
//    // pad1->SetMargin(0.12, 0.03, 0., 0.0);
//    // pad2->SetMargin(0.12, 0.03, 0.3, 0.0);
//    pad1->Draw();
//    pad2->Draw();
//    pad1->cd();

    TCanvas *c4 = new TCanvas("c4","",600,600);
    Float_t small = 1.e-5;
    c4->Divide(1,2,small,small);
    c4->cd(1);
    c4->cd(1)->SetTopMargin(0.05);
    c4->cd(1)->SetBottomMargin(small);
    gPad->SetPad(0.0,0.55,1.0,1.0);
    TGraphErrors *NonlinearityGraph = new TGraphErrors(n-1,trueCharge,MeanNonlinearity,ErrortrueCharge,ErrorNonlinearity);
    NonlinearityGraph->SetTitle("Nonlinearity");
    NonlinearityGraph->SetName("Nonlinearity");
    NonlinearityGraph->GetXaxis()->SetTitle("True charge(PE)");
    NonlinearityGraph->GetYaxis()->SetTitle("Nonlinearity");
    NonlinearityGraph->GetXaxis()->CenterTitle();
    NonlinearityGraph->GetYaxis()->CenterTitle();
    NonlinearityGraph->GetYaxis()->SetTitleSize(0.1);
    NonlinearityGraph->GetYaxis()->SetTitleOffset(0.67);
    NonlinearityGraph->GetYaxis()->SetLabelSize(0.1);
    NonlinearityGraph->SetFillColor(4);
    NonlinearityGraph->SetFillStyle(3002);
    NonlinearityGraph->SetMarkerColor(4);
    NonlinearityGraph->SetMarkerSize(.15);
    NonlinearityGraph->SetMarkerStyle(20);
    NonlinearityGraph->SetLineColor(4);
    NonlinearityGraph->SetLineWidth(3);
    NonlinearityGraph->SetLineStyle(2);
    NonlinearityGraph->Draw("a L P");
    gStyle->SetOptTitle(0); 

    // TCanvas *c2 = new TCanvas("c2","c2",1);
    c4->cd(2);
    c4->cd(2)->SetTopMargin(small);
    c4->cd(2)->SetBottomMargin(0.18);
    gPad->SetPad(0.0,0.,1.0,0.55);

    TGraphErrors *StdDEV = new TGraphErrors(n-1,trueCharge,RMS,ErrortrueCharge,ErrorRMS);
    StdDEV->SetTitle("Resolution");
    StdDEV->SetName("Resolution");
    StdDEV->GetXaxis()->SetTitle("True charge(PE)");
    StdDEV->GetYaxis()->SetTitle("Resolution");
    StdDEV->GetXaxis()->CenterTitle();
    StdDEV->GetYaxis()->CenterTitle();
    StdDEV->GetYaxis()->SetTitleSize(0.08);
    StdDEV->GetYaxis()->SetTitleOffset(0.83);
    StdDEV->GetYaxis()->SetLabelSize(0.08);
    StdDEV->GetXaxis()->SetTitleSize(0.08);
    StdDEV->GetXaxis()->SetLabelSize(0.08);
    StdDEV->SetFillColor(4);
    StdDEV->SetFillStyle(3002);
    StdDEV->SetMarkerColor(4);
    StdDEV->SetMarkerSize(0.8);
    StdDEV->SetMarkerStyle(20);
    StdDEV->SetLineColor(4);
    StdDEV->SetLineWidth(3);
    StdDEV->SetLineStyle(2);
    StdDEV->Draw("a L P");
    gStyle->SetOptTitle(0); 

    c4->cd();

    TCanvas *c3 = new TCanvas("c3","c3",1);
    GetX->Draw();
}
