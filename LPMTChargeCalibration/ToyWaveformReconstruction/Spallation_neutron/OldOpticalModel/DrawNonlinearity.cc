Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
    double xx =x[0]; // 有非线性的电荷值
    double miu = 500 - sqrt(500*500 - 1000*xx); // 真实的电荷值
    // Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
    Double_t f = xx/miu - 1;

    // Double_t f = -(0.1/100.)*xx;
    return f;
}

void DrawNonlinearity()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Spallation_neutron/OldOpticalModel/result/npeLPMTtonpeSPMT_B12_DetSim_AllPMT_HighQENNVT_Cut4.root","read");
    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TString GraphName = "P0 LPMT #mu to SPMT #mu_QE_PDE";
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

    TGraphErrors *TruenpeLPMTtonpeSPMT_tot;
    TString GraphName_True = "P0 True LPMT #mu to SPMT #mu_QE_PDE";
    TruenpeLPMTtonpeSPMT_tot = (TGraphErrors*)inputfile->Get(GraphName_True);
    TruenpeLPMTtonpeSPMT_tot->SetMarkerColor(6);
    TruenpeLPMTtonpeSPMT_tot->SetLineColor(6);

    for (int QNum = npeLPMTtonpeSPMT_tot->GetN(); QNum>0; QNum--)
    {
        if (TMath::IsNaN(npeLPMTtonpeSPMT_tot->GetPointY(QNum)) || TMath::IsNaN(npeLPMTtonpeSPMT_tot->GetPointX(QNum)))
        {
            npeLPMTtonpeSPMT_tot->RemovePoint(QNum);
        } 
    }

    for (int QNum = TruenpeLPMTtonpeSPMT_tot->GetN(); QNum>0; QNum--)
    {
        if (TMath::IsNaN(TruenpeLPMTtonpeSPMT_tot->GetPointY(QNum)) || TMath::IsNaN(TruenpeLPMTtonpeSPMT_tot->GetPointX(QNum)))
        {
            TruenpeLPMTtonpeSPMT_tot->RemovePoint(QNum);
        } 
    }

    TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,0.02);
    Below1PE->SetParNames("k");
    Below1PE->SetLineColor(kRed);
    Below1PE->SetParameter(0,50);

    npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R F");
    Below1PE->SetRange(0,0.55);
    TLegend *legendb =new TLegend(0.6031519,0.4327731,0.9942693,0.6365546);
    legendb->SetTextFont(22);
    legendb->AddEntry(npeLPMTtonpeSPMT_tot,"ElecSim LPMT #mu","lpe");
    legendb->AddEntry(TruenpeLPMTtonpeSPMT_tot,"True LPMT #mu","lpe"); 
    legendb->SetLineColor(0);


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
    TruenpeLPMTtonpeSPMT_tot->Draw("P");
    legendb->Draw("same");
    c1->Update();
    TPaveStats *s = (TPaveStats*)c1->GetPrimitive("stats");
    s->SetX1NDC(0.5974212);
    s->SetX2NDC(0.9584527);
    s->SetY1NDC(0.2605042);
    s->SetY2NDC(0.4222689);

    vector<double> Nonlinearity;
    vector<double> NonlinearityError;
    vector<double> TrueNonlinearity;
    vector<double> TrueNonlinearityError;
    vector<double> LPMTMu;
    vector<double> LPMTMuError;

    for (int i = 0; i < npeLPMTtonpeSPMT_tot->GetN(); i++)
    {
        if (npeLPMTtonpeSPMT_tot->GetPointY(i) > 0)
        {
            double LPMTMui = npeLPMTtonpeSPMT_tot->GetPointY(i);
            double LPMTMuErrori = npeLPMTtonpeSPMT_tot->GetErrorY(i);
            double CalibLPMTMui = npeLPMTtonpeSPMT_tot->GetPointX(i)*Below1PE->GetParameter(0);
            double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_tot->GetErrorX(i)*npeLPMTtonpeSPMT_tot->GetErrorX(i))/(npeLPMTtonpeSPMT_tot->GetPointX(i)*npeLPMTtonpeSPMT_tot->GetPointX(i)) + (Below1PE->GetParError(0)*Below1PE->GetParError(0))/(Below1PE->GetParameter(0)*Below1PE->GetParameter(0)));
            double TruePMTMui = TruenpeLPMTtonpeSPMT_tot->GetPointY(i);
            double TruePMTMuErrori = TruenpeLPMTtonpeSPMT_tot->GetErrorY(i);
            double Nonlinearityi = (LPMTMui-CalibLPMTMui)/(CalibLPMTMui);
            double NonlinearityErrori = (LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));
            double TrueNonlinearityi = (LPMTMui-TruePMTMui)/(TruePMTMui);
            double TrueNonlinearityErrori = (LPMTMui/TruePMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (TruePMTMuErrori*TruePMTMuErrori)/(TruePMTMui*TruePMTMui));

            LPMTMu.push_back(LPMTMui);
            LPMTMuError.push_back(LPMTMuErrori);
            Nonlinearity.push_back(Nonlinearityi);
            NonlinearityError.push_back(NonlinearityErrori);
            TrueNonlinearity.push_back(TrueNonlinearityi);
            TrueNonlinearityError.push_back(TrueNonlinearityErrori);
        }
    }

    TGraphErrors *NonlinearityLPMTtoMu = new TGraphErrors(LPMTMu.size(),&LPMTMu[0],&Nonlinearity[0],&LPMTMuError[0],&NonlinearityError[0]);
    NonlinearityLPMTtoMu->SetTitle("");
    NonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT}(p.e.)");
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

    TGraphErrors *TrueNonlinearityLPMTtoMu = new TGraphErrors(LPMTMu.size(),&LPMTMu[0],&TrueNonlinearity[0],&LPMTMuError[0],&TrueNonlinearityError[0]);
    TrueNonlinearityLPMTtoMu->SetTitle("");
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT}(p.e.)");
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitle("Nonlinearity");
    TrueNonlinearityLPMTtoMu->SetFillStyle(3002);
    TrueNonlinearityLPMTtoMu->SetMarkerColor(3);
    TrueNonlinearityLPMTtoMu->SetLineColor(3);
    TrueNonlinearityLPMTtoMu->SetMarkerSize(1.0);
    TrueNonlinearityLPMTtoMu->SetMarkerStyle(20);
    TrueNonlinearityLPMTtoMu->SetLineWidth(3);
    TrueNonlinearityLPMTtoMu->GetXaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu->GetYaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitleOffset(1.2);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitleOffset(1.30);

    TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,0,130,1);
    SetNonlinearity->SetParameter(0,0);

    TLegend *legenda =new TLegend(0.5,0.4327731,0.9942693,0.6365546);
    legenda->SetTextFont(22);
    legenda->AddEntry(NonlinearityLPMTtoMu,"Calibrated nonlinearity","lpe");
    legenda->AddEntry(TrueNonlinearityLPMTtoMu,"True nonlinearity","lpe");
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
    NonlinearityLPMTtoMu_Canvas->cd();

    NonlinearityLPMTtoMu->Draw("A P");
    TrueNonlinearityLPMTtoMu->Draw("P");
    SetNonlinearity->Draw("same");
    legenda->Draw("same");
}
