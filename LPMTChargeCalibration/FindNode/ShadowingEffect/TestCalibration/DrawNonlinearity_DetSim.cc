Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
    double xx =x[0]; // 有非线性的电荷值
    double miu = 500 - sqrt(500*500 - 1000*xx); // 真实的电荷值
    // Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
    Double_t f = xx/miu - 1;
    return f;
}

void DrawNonlinearity_DetSim()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/TestCalibration/results/npeLPMTtonpeSPMT_DetSim_AllPMT_NNVT_Cut2.root","read");
    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TString GraphName = "True LPMT #mu to SPMT #mu";
    npeLPMTtonpeSPMT_tot = (TGraphErrors*)inputfile->Get(GraphName);
    npeLPMTtonpeSPMT_tot->SetTitle("");
    npeLPMTtonpeSPMT_tot->SetMarkerColor(4);
    npeLPMTtonpeSPMT_tot->SetLineColor(4);
    npeLPMTtonpeSPMT_tot->SetMarkerSize(1.0);
    npeLPMTtonpeSPMT_tot->SetMarkerStyle(20);
    npeLPMTtonpeSPMT_tot->SetLineWidth(3);
    npeLPMTtonpeSPMT_tot->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleOffset(1.30);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleOffset(0.86);

    TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,500);
    Below1PE->SetParNames("k");
    Below1PE->SetLineColor(kRed);
    Below1PE->SetParameter(0,50);

    npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R F");
    // Below1PE->SetRange(0,0.55);

    // TGraphErrors *TruenpeLPMTtonpeSPMT_tot;
    // TString GraphName_True = "True LPMT #mu to SPMT #mu";
    // TruenpeLPMTtonpeSPMT_tot = (TGraphErrors*)inputfile->Get(GraphName_True);
    // TruenpeLPMTtonpeSPMT_tot->SetMarkerColor(6);
    // TruenpeLPMTtonpeSPMT_tot->SetLineColor(6);

    // TLegend *legendb =new TLegend(0.6031519,0.4327731,0.9942693,0.6365546);
    // legendb->SetTextFont(22);
    // legendb->AddEntry(npeLPMTtonpeSPMT_tot,"ElecSim LPMT #mu","lpe");
    // legendb->AddEntry(TruenpeLPMTtonpeSPMT_tot,"True LPMT #mu","lpe"); 
    // legendb->SetLineColor(0);


    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->SetBorderMode(0);
    c1->SetBorderSize(2);
    c1->SetLeftMargin(0.1117479);
    c1->SetRightMargin(0.08739255);
    c1->SetTopMargin(0.02941176);
    c1->SetBottomMargin(0.1701681);
    c1->SetFrameBorderMode(0);
    c1->SetFrameBorderMode(0);
    c1->cd();
    gStyle->SetOptFit(1111);
    npeLPMTtonpeSPMT_tot->Draw("A P");
    Below1PE->Draw("same");
    // TruenpeLPMTtonpeSPMT_tot->Draw("P");
    // legendb->Draw("same");
    c1->Update();
    TPaveStats *s = (TPaveStats*)c1->GetPrimitive("stats");
    s->SetX1NDC(0.5974212);
    s->SetX2NDC(0.9584527);
    s->SetY1NDC(0.2605042);
    s->SetY2NDC(0.4222689);

    vector<double> Nonlinearity;
    vector<double> NonlinearityError;
    vector<double> LPMTMu;
    vector<double> LPMTMuError;

    for (int i = 0; i < npeLPMTtonpeSPMT_tot->GetN(); i++)
    {
        if (npeLPMTtonpeSPMT_tot->GetPointY(i) > 0 && npeLPMTtonpeSPMT_tot->GetPointX(i) > 0)
        {
            double LPMTMui = npeLPMTtonpeSPMT_tot->GetPointY(i);
            double LPMTMuErrori = npeLPMTtonpeSPMT_tot->GetErrorY(i);
            double CalibLPMTMui = npeLPMTtonpeSPMT_tot->GetPointX(i)*Below1PE->GetParameter(0);
            double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_tot->GetErrorX(i)*npeLPMTtonpeSPMT_tot->GetErrorX(i))/(npeLPMTtonpeSPMT_tot->GetPointX(i)*npeLPMTtonpeSPMT_tot->GetPointX(i)) + (Below1PE->GetParError(0)*Below1PE->GetParError(0))/(Below1PE->GetParameter(0)*Below1PE->GetParameter(0)));
            double Nonlinearityi = (LPMTMui-CalibLPMTMui)/CalibLPMTMui;
            double NonlinearityErrori = (LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));

            LPMTMu.push_back(LPMTMui);
            LPMTMuError.push_back(LPMTMuErrori);
            Nonlinearity.push_back(Nonlinearityi);
            NonlinearityError.push_back(NonlinearityErrori);
        }
    }

    TGraphErrors *NonlinearityLPMTtoMu = new TGraphErrors(LPMTMu.size(),&LPMTMu[0],&Nonlinearity[0],&LPMTMuError[0],&NonlinearityError[0]);
    NonlinearityLPMTtoMu->SetTitle("");
    NonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT}/PDE_{LPMT} (p.e.)");
    NonlinearityLPMTtoMu->GetYaxis()->SetTitle("Nonlinearity");
    NonlinearityLPMTtoMu->SetFillStyle(3002);
    NonlinearityLPMTtoMu->SetMarkerColor(4);
    NonlinearityLPMTtoMu->SetLineColor(4);
    NonlinearityLPMTtoMu->SetMarkerSize(1.0);
    NonlinearityLPMTtoMu->SetMarkerStyle(20);
    NonlinearityLPMTtoMu->SetLineWidth(3);
    NonlinearityLPMTtoMu->GetXaxis()->CenterTitle();
    NonlinearityLPMTtoMu->GetYaxis()->CenterTitle();
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu->GetXaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu->GetYaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleOffset(1.2);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleOffset(1.30);

    TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,0,130,1);
    SetNonlinearity->SetParameter(0,0);

    TLegend *legenda =new TLegend(0.5,0.4327731,0.9942693,0.6365546);
    legenda->SetTextFont(22);
    legenda->AddEntry(NonlinearityLPMTtoMu,"Calibrated nonlinearity","lpe");
    legenda->AddEntry(SetNonlinearity,"Set value","l"); 
    legenda->SetLineColor(0);

    TCanvas *NonlinearityLPMTtoMu_Canvas = new TCanvas("NonlinearityLPMTtoMu_Canvas","NonlinearityLPMTtoMu_Canvas",1);
    NonlinearityLPMTtoMu_Canvas->SetBorderMode(0);
    NonlinearityLPMTtoMu_Canvas->SetBorderSize(2);
    NonlinearityLPMTtoMu_Canvas->SetLeftMargin(0.1504298);
    NonlinearityLPMTtoMu_Canvas->SetRightMargin(0.0487106);
    NonlinearityLPMTtoMu_Canvas->SetTopMargin(0.02941176);
    NonlinearityLPMTtoMu_Canvas->SetBottomMargin(0.1701681);
    NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
    NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
    NonlinearityLPMTtoMu_Canvas->SetLogx();
    NonlinearityLPMTtoMu_Canvas->SetGridy();
    NonlinearityLPMTtoMu_Canvas->cd();

    NonlinearityLPMTtoMu->Draw("A P");
    // SetNonlinearity->Draw("same");
    // legenda->Draw("same");


    // TGraphErrors *npeLPMTtonpeSPMT_P0;
    // TString GraphName_P0 = "P0 LPMT #mu to SPMT #mu";
    // npeLPMTtonpeSPMT_P0 = (TGraphErrors*)inputfile->Get(GraphName_P0);
    // npeLPMTtonpeSPMT_P0->SetTitle("");
    // npeLPMTtonpeSPMT_P0->SetMarkerColor(4);
    // npeLPMTtonpeSPMT_P0->SetLineColor(4);
    // npeLPMTtonpeSPMT_P0->SetMarkerSize(1.0);
    // npeLPMTtonpeSPMT_P0->SetMarkerStyle(20);
    // npeLPMTtonpeSPMT_P0->SetLineWidth(3);
    // npeLPMTtonpeSPMT_P0->GetXaxis()->CenterTitle();
    // npeLPMTtonpeSPMT_P0->GetYaxis()->CenterTitle();
    // npeLPMTtonpeSPMT_P0->GetXaxis()->SetTitleFont(22);
    // npeLPMTtonpeSPMT_P0->GetYaxis()->SetTitleFont(22);
    // npeLPMTtonpeSPMT_P0->GetXaxis()->SetTitleSize(0.06);
    // npeLPMTtonpeSPMT_P0->GetYaxis()->SetTitleSize(0.06);
    // npeLPMTtonpeSPMT_P0->GetXaxis()->SetLabelSize(0.06);
    // npeLPMTtonpeSPMT_P0->GetYaxis()->SetLabelSize(0.06);
    // npeLPMTtonpeSPMT_P0->GetXaxis()->SetTitleOffset(1.30);
    // npeLPMTtonpeSPMT_P0->GetYaxis()->SetTitleOffset(0.86);

    // TF1 *Below1PE_P0 = new TF1("Below1PE_P0","[0]*x",0,10);
    // Below1PE_P0->SetParNames("k");
    // Below1PE_P0->SetLineColor(kRed);
    // Below1PE_P0->SetParameter(0,50);

    // npeLPMTtonpeSPMT_P0->Fit(Below1PE_P0,"R F");
    // Below1PE_P0->SetRange(0,0.55);

    // // TGraphErrors *TruenpeLPMTtonpeSPMT_tot;
    // // TString GraphName_True = "True LPMT #mu to SPMT #mu";
    // // TruenpeLPMTtonpeSPMT_tot = (TGraphErrors*)inputfile->Get(GraphName_True);
    // // TruenpeLPMTtonpeSPMT_tot->SetMarkerColor(6);
    // // TruenpeLPMTtonpeSPMT_tot->SetLineColor(6);

    // // TLegend *legendb =new TLegend(0.6031519,0.4327731,0.9942693,0.6365546);
    // // legendb->SetTextFont(22);
    // // legendb->AddEntry(npeLPMTtonpeSPMT_tot,"ElecSim LPMT #mu","lpe");
    // // legendb->AddEntry(TruenpeLPMTtonpeSPMT_tot,"True LPMT #mu","lpe"); 
    // // legendb->SetLineColor(0);


    // TCanvas *c2 = new TCanvas("c2","c2",1);
    // c2->SetBorderMode(0);
    // c2->SetBorderSize(2);
    // c2->SetLeftMargin(0.1117479);
    // c2->SetRightMargin(0.08739255);
    // c2->SetTopMargin(0.02941176);
    // c2->SetBottomMargin(0.1701681);
    // c2->SetFrameBorderMode(0);
    // c2->SetFrameBorderMode(0);
    // c2->cd();
    // gStyle->SetOptFit(1111);
    // npeLPMTtonpeSPMT_P0->Draw("A P");
    // Below1PE_P0->Draw("same");
    // // TruenpeLPMTtonpeSPMT_tot->Draw("P");
    // // legendb->Draw("same");
    // c2->Update();
    // TPaveStats *s2 = (TPaveStats*)c2->GetPrimitive("stats");
    // s2->SetX1NDC(0.5974212);
    // s2->SetX2NDC(0.9584527);
    // s2->SetY1NDC(0.2605042);
    // s2->SetY2NDC(0.4222689);

    // vector<double> Nonlinearity_P0;
    // vector<double> NonlinearityError_P0;
    // vector<double> LPMTMu_P0;
    // vector<double> LPMTMuError_P0;

    // for (int i = 0; i < npeLPMTtonpeSPMT_P0->GetN(); i++)
    // {
    //     if (npeLPMTtonpeSPMT_P0->GetPointY(i) > 0 && npeLPMTtonpeSPMT_P0->GetPointX(i) > 0)
    //     {
    //         double LPMTMui = npeLPMTtonpeSPMT_P0->GetPointY(i);
    //         double LPMTMuErrori = npeLPMTtonpeSPMT_P0->GetErrorY(i);
    //         double CalibLPMTMui = npeLPMTtonpeSPMT_P0->GetPointX(i)*Below1PE_P0->GetParameter(0);
    //         double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_P0->GetErrorX(i)*npeLPMTtonpeSPMT_P0->GetErrorX(i))/(npeLPMTtonpeSPMT_P0->GetPointX(i)*npeLPMTtonpeSPMT_P0->GetPointX(i)) + (Below1PE_P0->GetParError(0)*Below1PE_P0->GetParError(0))/(Below1PE_P0->GetParameter(0)*Below1PE_P0->GetParameter(0)));
    //         double Nonlinearityi = (LPMTMui-CalibLPMTMui)/CalibLPMTMui;
    //         double NonlinearityErrori = (LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));

    //         LPMTMu_P0.push_back(LPMTMui);
    //         LPMTMuError_P0.push_back(LPMTMuErrori);
    //         Nonlinearity_P0.push_back(Nonlinearityi);
    //         NonlinearityError_P0.push_back(NonlinearityErrori);
    //     }
    // }

    // TGraphErrors *NonlinearityLPMTtoMu_P0 = new TGraphErrors(LPMTMu_P0.size(),&LPMTMu_P0[0],&Nonlinearity_P0[0],&LPMTMuError_P0[0],&NonlinearityError_P0[0]);
    // NonlinearityLPMTtoMu_P0->SetTitle("");
    // NonlinearityLPMTtoMu_P0->GetXaxis()->SetTitle("#mu_{LPMT}(p.e.)");
    // NonlinearityLPMTtoMu_P0->GetYaxis()->SetTitle("Nonlinearity");
    // NonlinearityLPMTtoMu_P0->SetFillStyle(3002);
    // NonlinearityLPMTtoMu_P0->SetMarkerColor(4);
    // NonlinearityLPMTtoMu_P0->SetLineColor(4);
    // NonlinearityLPMTtoMu_P0->SetMarkerSize(1.0);
    // NonlinearityLPMTtoMu_P0->SetMarkerStyle(20);
    // NonlinearityLPMTtoMu_P0->SetLineWidth(3);
    // NonlinearityLPMTtoMu_P0->GetXaxis()->CenterTitle();
    // NonlinearityLPMTtoMu_P0->GetYaxis()->CenterTitle();
    // NonlinearityLPMTtoMu_P0->GetXaxis()->SetTitleFont(22);
    // NonlinearityLPMTtoMu_P0->GetYaxis()->SetTitleFont(22);
    // NonlinearityLPMTtoMu_P0->GetXaxis()->SetTitleSize(0.06);
    // NonlinearityLPMTtoMu_P0->GetYaxis()->SetTitleSize(0.06);
    // NonlinearityLPMTtoMu_P0->GetXaxis()->SetLabelSize(0.06);
    // NonlinearityLPMTtoMu_P0->GetYaxis()->SetLabelSize(0.06);
    // NonlinearityLPMTtoMu_P0->GetXaxis()->SetTitleOffset(1.2);
    // NonlinearityLPMTtoMu_P0->GetYaxis()->SetTitleOffset(1.30);

    // // TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,0,130,1);
    // // SetNonlinearity->SetParameter(0,0);

    // TLegend *legendb =new TLegend(0.5,0.4327731,0.9942693,0.6365546);
    // legendb->SetTextFont(22);
    // legendb->AddEntry(NonlinearityLPMTtoMu_P0,"Calibrated nonlinearity","lpe");
    // legendb->AddEntry(SetNonlinearity,"Set value","l"); 
    // legendb->SetLineColor(0);

    // TCanvas *NonlinearityLPMTtoMu_P0_Canvas = new TCanvas("NonlinearityLPMTtoMu_P0_Canvas","NonlinearityLPMTtoMu_P0_Canvas",1);
    // NonlinearityLPMTtoMu_P0_Canvas->SetBorderMode(0);
    // NonlinearityLPMTtoMu_P0_Canvas->SetBorderSize(2);
    // NonlinearityLPMTtoMu_P0_Canvas->SetLeftMargin(0.1504298);
    // NonlinearityLPMTtoMu_P0_Canvas->SetRightMargin(0.0487106);
    // NonlinearityLPMTtoMu_P0_Canvas->SetTopMargin(0.02941176);
    // NonlinearityLPMTtoMu_P0_Canvas->SetBottomMargin(0.1701681);
    // NonlinearityLPMTtoMu_P0_Canvas->SetFrameBorderMode(0);
    // NonlinearityLPMTtoMu_P0_Canvas->SetFrameBorderMode(0);
    // NonlinearityLPMTtoMu_P0_Canvas->SetLogx();
    // NonlinearityLPMTtoMu_P0_Canvas->SetGridy();
    // NonlinearityLPMTtoMu_P0_Canvas->cd();

    // NonlinearityLPMTtoMu_P0->Draw("A P");
    // // SetNonlinearity->Draw("same");
    // // legenda->Draw("same");

    // TCanvas *CompareP0AndnPE_canvas = new TCanvas("CompareP0AndnPE_Canvas","CompareP0AndnPE_Canvas",1);

    // CompareP0AndnPE_canvas->SetBorderMode(0);
    // CompareP0AndnPE_canvas->SetBorderSize(2);
    // CompareP0AndnPE_canvas->SetLeftMargin(0.1504298);
    // CompareP0AndnPE_canvas->SetRightMargin(0.0487106);
    // CompareP0AndnPE_canvas->SetTopMargin(0.02941176);
    // CompareP0AndnPE_canvas->SetBottomMargin(0.1701681);
    // CompareP0AndnPE_canvas->SetFrameBorderMode(0);
    // CompareP0AndnPE_canvas->SetFrameBorderMode(0);
    // CompareP0AndnPE_canvas->cd();

    // vector<double> Difference;
    // vector<double> SPMTMu_QE;
    // vector<double> DifferenceError;
    // vector<double> SPMTMu_QEError;
    // for (int i = 0; i < npeLPMTtonpeSPMT_P0->GetN(); i++)
    // {
    //     if (npeLPMTtonpeSPMT_P0->GetPointX(i) > 0)
    //     {
    //         double SPMTMu_QEi = npeLPMTtonpeSPMT_tot->GetPointX(i);
    //         double SPMTMu_QEErrori = npeLPMTtonpeSPMT_tot->GetErrorX(i);
    //         double LPMTPDE;
    //         double LPMTPDEError;
    //         TH1F *Hist_LPMTQE = (TH1F*)inputfile->Get(TString::Format("%1d/Hist LPMTQE Q%1d",i,i));
    //         LPMTPDE = Hist_LPMTQE->GetMean();
    //         LPMTPDEError = Hist_LPMTQE->GetMeanError();
    //         double SPMTMu_QEP0 = npeLPMTtonpeSPMT_P0->GetPointX(i)/LPMTPDE;
    //         double SPMTMu_QEP0Error = npeLPMTtonpeSPMT_P0->GetErrorX(i)/LPMTPDE;
    //         // double CalibLPMTMui = npeLPMTtonpeSPMT_P0->GetPointX(i)*Below1PE_P0->GetParameter(0);
    //         // double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_P0->GetErrorX(i)*npeLPMTtonpeSPMT_P0->GetErrorX(i))/(npeLPMTtonpeSPMT_P0->GetPointX(i)*npeLPMTtonpeSPMT_P0->GetPointX(i)) + (Below1PE_P0->GetParError(0)*Below1PE_P0->GetParError(0))/(Below1PE_P0->GetParameter(0)*Below1PE_P0->GetParameter(0)));
    //         double Nonlinearityi = (SPMTMu_QEP0-SPMTMu_QEi)/SPMTMu_QEi;
    //         double NonlinearityErrori = (SPMTMu_QEP0/SPMTMu_QEi)*sqrt((SPMTMu_QEP0Error*SPMTMu_QEP0Error)/(SPMTMu_QEP0*SPMTMu_QEP0) + (SPMTMu_QEErrori*SPMTMu_QEErrori)/(SPMTMu_QEi*SPMTMu_QEi));

    //         SPMTMu_QE.push_back(SPMTMu_QEi);
    //         SPMTMu_QEError.push_back(SPMTMu_QEErrori);
    //         Difference.push_back(Nonlinearityi);
    //         DifferenceError.push_back(NonlinearityErrori);
    //     }
    // }

    // TGraphErrors *CompareP0AndnPE = new TGraphErrors(SPMTMu_QE.size(),&SPMTMu_QE[0],&Difference[0],&SPMTMu_QEError[0],&DifferenceError[0]);
    // CompareP0AndnPE->SetTitle("");
    // CompareP0AndnPE->GetXaxis()->SetTitle("#mu_{SPMT}/QE(p.e.)");
    // CompareP0AndnPE->GetYaxis()->SetTitle("Difference");
    // CompareP0AndnPE->SetFillStyle(3002);
    // CompareP0AndnPE->SetMarkerColor(4);
    // CompareP0AndnPE->SetLineColor(4);
    // CompareP0AndnPE->SetMarkerSize(1.0);
    // CompareP0AndnPE->SetMarkerStyle(20);
    // CompareP0AndnPE->SetLineWidth(3);
    // CompareP0AndnPE->GetXaxis()->CenterTitle();
    // CompareP0AndnPE->GetYaxis()->CenterTitle();
    // CompareP0AndnPE->GetXaxis()->SetTitleFont(22);
    // CompareP0AndnPE->GetYaxis()->SetTitleFont(22);
    // CompareP0AndnPE->GetXaxis()->SetTitleSize(0.06);
    // CompareP0AndnPE->GetYaxis()->SetTitleSize(0.06);
    // CompareP0AndnPE->GetXaxis()->SetLabelSize(0.06);
    // CompareP0AndnPE->GetYaxis()->SetLabelSize(0.06);
    // CompareP0AndnPE->GetXaxis()->SetTitleOffset(1.2);
    // CompareP0AndnPE->GetYaxis()->SetTitleOffset(1.30);

    // CompareP0AndnPE->Draw("A P");

}
