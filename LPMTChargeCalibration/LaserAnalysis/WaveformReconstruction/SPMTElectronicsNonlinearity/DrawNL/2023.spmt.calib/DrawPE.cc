double MeanRatio = 1;


void DrawPE()
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
            MeanNonlinearity[n] = 100.*( Mean - (double)nPE)/(double)nPE;
            trueCharge[n] = nPE;
            ErrorNonlinearity[n] = 100.*(sqrt(entries)/sqrt(entries - 1))*MeanError;
            ErrorNonlinearity[n] /= (double)nPE;
            ErrortrueCharge[n] = 0;

            RMS[n] = 100.*CalibHist[nPE]->GetRMS()/CalibHist[nPE]->GetMean();
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
    MeanNonlinearity[n] = 100.*( Mean - GetX->GetMean())/GetX->GetMean();
    trueCharge[n] = GetX->GetMean();
    ErrorNonlinearity[n] = sqrt((GetX->GetMeanError()*GetX->GetMeanError())/(GetX->GetMean()*GetX->GetMean()) + (GetY->GetMeanError()*GetY->GetMeanError())/(GetY->GetMean()*GetY->GetMean()));
    ErrorNonlinearity[n] *= 100.*GetX->GetMean()/GetY->GetMean();
    ErrortrueCharge[n] = GetX->GetMeanError();

    RMS[n] = 100.*GetY->GetRMS()/GetY->GetMean();
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

    TCanvas *c4 = new TCanvas("c4","",700,600);
    Float_t small = 1.e-5;
    c4->Divide(1,2,small,small);
    c4->cd(1);
    c4->cd(1)->SetTopMargin(0.01);
    c4->cd(1)->SetBottomMargin(small);
    c4->cd(1)->SetLeftMargin(0.10);
    c4->cd(1)->SetRightMargin(0.03);
    gPad->SetPad(0.0,0.55,1.0,1.0);
    TGraphErrors *NonlinearityGraph = new TGraphErrors(n-1,trueCharge,MeanNonlinearity,ErrortrueCharge,ErrorNonlinearity);
    NonlinearityGraph->SetTitle("Nonlinearity");
    NonlinearityGraph->SetName("Nonlinearity");
    NonlinearityGraph->GetXaxis()->SetTitle("True charge [p.e.]");
    NonlinearityGraph->GetYaxis()->SetTitle("Nonlinearity [%]");
    NonlinearityGraph->GetXaxis()->CenterTitle();
    NonlinearityGraph->GetYaxis()->CenterTitle();
    NonlinearityGraph->GetYaxis()->SetTitleSize(0.1);
    NonlinearityGraph->GetYaxis()->SetTitleOffset(0.51);
    NonlinearityGraph->GetYaxis()->SetLabelSize(0.1);
    NonlinearityGraph->GetYaxis()->SetNdivisions(505);
    NonlinearityGraph->GetXaxis()->SetTitleFont(22);
    NonlinearityGraph->GetYaxis()->SetTitleFont(22);
    NonlinearityGraph->SetFillColor(4);
    NonlinearityGraph->SetFillStyle(3002);
    NonlinearityGraph->SetMarkerColor(4);
    NonlinearityGraph->SetMarkerSize(1.5);
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
    c4->cd(2)->SetLeftMargin(0.10);
    c4->cd(2)->SetRightMargin(0.03);
    gPad->SetPad(0.0,0.,1.0,0.55);

    TGraphErrors *StdDEV = new TGraphErrors(n-1,trueCharge,RMS,ErrortrueCharge,ErrorRMS);
    StdDEV->SetTitle("Resolution");
    StdDEV->SetName("Resolution");
    StdDEV->GetXaxis()->SetTitle("True charge [p.e.]");
    StdDEV->GetYaxis()->SetTitle("Resolution [%]");
    StdDEV->GetXaxis()->SetTitleFont(22);
    StdDEV->GetYaxis()->SetTitleFont(22);
    StdDEV->GetXaxis()->CenterTitle();
    StdDEV->GetYaxis()->CenterTitle();
    StdDEV->GetYaxis()->SetTitleSize(0.08);
    StdDEV->GetYaxis()->SetTitleOffset(0.63);
    StdDEV->GetYaxis()->SetLabelSize(0.08);
    StdDEV->GetXaxis()->SetTitleSize(0.08);
    StdDEV->GetXaxis()->SetLabelSize(0.08);
    StdDEV->GetYaxis()->SetNdivisions(505);
    StdDEV->SetFillColor(4);
    StdDEV->SetFillStyle(3002);
    StdDEV->SetMarkerColor(4);
    StdDEV->SetMarkerSize(1.5);
    StdDEV->SetMarkerStyle(20);
    StdDEV->SetLineColor(4);
    StdDEV->SetLineWidth(3);
    StdDEV->SetLineStyle(2);
    StdDEV->Draw("a L P");
    gStyle->SetOptTitle(0); 

    c4->cd();

    TCanvas *c3 = new TCanvas("c3","c3",700,600);
    c3->Divide(1,4,small,small);
    c3->cd(1);
    c3->cd(1)->SetTopMargin(small);
    c3->cd(1)->SetBottomMargin(small);
    //gPad->SetPad(0.0,0.7,1.0,1.0);
    gPad->SetPad(0.0,0.78,1.0,1.0);
    gStyle->SetOptStat(0);
    CalibHist[1]->Draw();
    CalibHist[1]->GetXaxis()->SetRangeUser(0,6);
    CalibHist[1]->GetYaxis()->SetLabelSize(0.);
    CalibHist[1]->GetYaxis()->SetNdivisions(505);
    CalibHist[1]->GetXaxis()->SetTitleFont(22);
    CalibHist[1]->GetYaxis()->SetTitleFont(22);
    TText *text1 = new TText(4.5, 57, "1 PE");
    text1->SetTextSize(0.39);
    text1->SetTextColor(kBlack);
    text1->Draw();
    c3->Update();
    c3->cd(2);
    c3->cd(2)->SetTopMargin(small);
    c3->cd(2)->SetBottomMargin(small);
    //gPad->SetPad(0.0,0.4,1.0,0.7);
    gPad->SetPad(0.0,0.56,1.0,0.78);
    CalibHist[2]->Draw();
    CalibHist[2]->GetXaxis()->SetRangeUser(0,6);
    CalibHist[2]->GetYaxis()->SetTitle("Unit");
    CalibHist[2]->GetYaxis()->CenterTitle();
    CalibHist[2]->GetYaxis()->SetTitleSize(0.2);
    CalibHist[2]->GetYaxis()->SetTitleOffset(0.2);
    CalibHist[2]->GetYaxis()->SetLabelSize(0.);
    CalibHist[2]->GetYaxis()->SetNdivisions(505);
    CalibHist[2]->GetXaxis()->SetTitleFont(22);
    CalibHist[2]->GetYaxis()->SetTitleFont(22);
    TText *text2 = new TText(4.5, 93, "2 PE");
    text2->SetTextSize(0.39);
    text2->SetTextColor(kBlack);
    text2->Draw();
    c3->Update();
    c3->cd(3);
    c3->cd(3)->SetTopMargin(small);
    c3->cd(3)->SetBottomMargin(small);
    //gPad->SetPad(0.0,0.,1.0,0.4);
    gPad->SetPad(0.0,0.34,1.0,0.56);
    CalibHist[3]->Draw();
    CalibHist[3]->GetXaxis()->SetRangeUser(0,6);
    CalibHist[3]->GetYaxis()->SetTitle("Arbitrary");
    CalibHist[3]->GetYaxis()->SetTitleSize(0.2);
    CalibHist[3]->GetYaxis()->SetTitleOffset(0.2);
    CalibHist[3]->GetYaxis()->SetLabelSize(0.);
    CalibHist[3]->GetYaxis()->SetNdivisions(505);
    CalibHist[3]->GetXaxis()->SetTitleFont(22);
    CalibHist[3]->GetYaxis()->SetTitleFont(22);
    TText *text3 = new TText(4.5, 93, "3 PE");
    text3->SetTextSize(0.39);
    text3->SetTextColor(kBlack);
    text3->Draw();
    c3->Update();
    c3->cd(4);
    c3->cd(4)->SetTopMargin(small);
    c3->cd(4)->SetBottomMargin(0.35);
    gPad->SetPad(0.0,0.,1.0,0.34);
    CalibHist[4]->Draw();
    CalibHist[4]->GetXaxis()->SetRangeUser(0,6);
    CalibHist[4]->GetXaxis()->SetTitle("Calibrated charge [p.e.]");
    CalibHist[4]->GetXaxis()->CenterTitle();
    CalibHist[4]->GetXaxis()->SetTitleSize(0.14);
    CalibHist[4]->GetXaxis()->SetLabelSize(0.14);
    CalibHist[4]->GetYaxis()->SetLabelSize(0.);
    CalibHist[4]->GetYaxis()->SetNdivisions(505);
    CalibHist[4]->GetXaxis()->SetTitleFont(22);
    CalibHist[4]->GetYaxis()->SetTitleFont(22);
    TText *text4 = new TText(4.5, 93, "4 PE");
    text4->SetTextSize(0.26);
    text4->SetTextColor(kBlack);
    text4->Draw();
    c3->Update();
    c3->cd();
}
