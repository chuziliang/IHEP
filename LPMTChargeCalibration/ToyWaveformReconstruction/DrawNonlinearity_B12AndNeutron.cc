Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
    double xx =x[0]; // 有非线性的电荷值
    double miu = 500 - sqrt(500*500 - 1000*xx); // 真实的电荷值
    // Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
    Double_t f = xx/miu - 1;

    // Double_t f = -(0.1/100.)*xx;
    return 100.*f;
}

void DrawNonlinearity_B12AndNeutron(int ChosenType)
{
    TString ChosenTypeString;
    if (ChosenType==0) ChosenTypeString = "All";
    else if (ChosenType==1) ChosenTypeString = "Hamamatsu";
    else if (ChosenType==2) ChosenTypeString = "NNVT";
    else if (ChosenType==3) ChosenTypeString = "HighQENNVT";

    Int_t color_B12 = TColor::GetColor(0, 0, 255);
    Int_t color_Neutron = TColor::GetColor(255,100,0);
    Int_t color_Both = TColor::GetColor(148, 0, 211);

    Int_t PoinrStyle_B12 = 20;
    Int_t PoinrStyle_Neutron = 21;
    Int_t PoinrStyle_Both = 22;

    TFile *inputfile_B12 = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12_OldOpticalModel/result/Graph_"+ChosenTypeString+"_Cut4.root","read");
    TFile *inputfile_Neutron = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Spallation_neutron/OldOpticalModel/MeragedResult/Graph_"+ChosenTypeString+"_Cut4.root","read");
    TFile *inputfile_Both = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/CombineB12AndNeutron/result/Graph_"+ChosenTypeString+"_Cut4.root","read");
    
    TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,1e-2,130,1);
    SetNonlinearity->SetParameter(0,0);

    // 得到Neutron的Graph
    TGraphErrors *npeLPMTtonpeSPMT_Neutron;
    TString GraphName = "P0 LPMT #mu to SPMT #mu_QE_PDE";
    npeLPMTtonpeSPMT_Neutron = (TGraphErrors*)inputfile_Neutron->Get(GraphName);
    npeLPMTtonpeSPMT_Neutron->SetTitle("");
    npeLPMTtonpeSPMT_Neutron->SetMarkerColorAlpha(color_Neutron,1.0);
    npeLPMTtonpeSPMT_Neutron->SetLineColorAlpha(color_Neutron,1.0);
    npeLPMTtonpeSPMT_Neutron->SetMarkerSize(1.3);
    npeLPMTtonpeSPMT_Neutron->SetMarkerStyle(PoinrStyle_Neutron);
    npeLPMTtonpeSPMT_Neutron->SetLineWidth(3);
    npeLPMTtonpeSPMT_Neutron->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_Neutron->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_Neutron->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_Neutron->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_Neutron->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_Neutron->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_Neutron->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_Neutron->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_Neutron->GetXaxis()->SetTitleOffset(1.30);
    npeLPMTtonpeSPMT_Neutron->GetYaxis()->SetTitleOffset(0.86);

    TGraphErrors *TruenpeLPMTtonpeSPMT_Neutron;
    TString GraphName_True = "P0 True LPMT #mu to SPMT #mu_QE_PDE";
    TruenpeLPMTtonpeSPMT_Neutron = (TGraphErrors*)inputfile_Neutron->Get(GraphName_True);
    TruenpeLPMTtonpeSPMT_Neutron->SetMarkerColorAlpha(color_Neutron,1.0);
    TruenpeLPMTtonpeSPMT_Neutron->SetLineColorAlpha(color_Neutron,1.0);

    for (int QNum = npeLPMTtonpeSPMT_Neutron->GetN(); QNum>0; QNum--)
    {
        if (TMath::IsNaN(npeLPMTtonpeSPMT_Neutron->GetPointY(QNum)) || TMath::IsNaN(npeLPMTtonpeSPMT_Neutron->GetPointX(QNum)))
        {
            npeLPMTtonpeSPMT_Neutron->RemovePoint(QNum);
        } 
    }

    for (int QNum = TruenpeLPMTtonpeSPMT_Neutron->GetN(); QNum>0; QNum--)
    {
        if (TMath::IsNaN(TruenpeLPMTtonpeSPMT_Neutron->GetPointY(QNum)) || TMath::IsNaN(TruenpeLPMTtonpeSPMT_Neutron->GetPointX(QNum)))
        {
            TruenpeLPMTtonpeSPMT_Neutron->RemovePoint(QNum);
        } 
    }

    // 得到B12的Graph
    TGraphErrors *npeLPMTtonpeSPMT_B12;
    // TString GraphName = "P0 LPMT #mu to SPMT #mu_QE_PDE";
    npeLPMTtonpeSPMT_B12 = (TGraphErrors*)inputfile_B12->Get(GraphName);
    npeLPMTtonpeSPMT_B12->SetTitle("");
    npeLPMTtonpeSPMT_B12->SetMarkerColorAlpha(color_B12,1.0);
    npeLPMTtonpeSPMT_B12->SetLineColorAlpha(color_B12,1.0);
    npeLPMTtonpeSPMT_B12->SetMarkerSize(1.3);
    npeLPMTtonpeSPMT_B12->SetMarkerStyle(PoinrStyle_B12);
    npeLPMTtonpeSPMT_B12->SetLineWidth(3);
    npeLPMTtonpeSPMT_B12->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_B12->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_B12->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_B12->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_B12->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_B12->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_B12->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_B12->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_B12->GetXaxis()->SetTitleOffset(1.30);
    npeLPMTtonpeSPMT_B12->GetYaxis()->SetTitleOffset(0.86);

    TGraphErrors *TruenpeLPMTtonpeSPMT_B12;
    // TString GraphName_True = "P0 True LPMT #mu to SPMT #mu_QE_PDE";
    TruenpeLPMTtonpeSPMT_B12 = (TGraphErrors*)inputfile_B12->Get(GraphName_True);
    TruenpeLPMTtonpeSPMT_B12->SetMarkerColorAlpha(color_B12,1.0);
    TruenpeLPMTtonpeSPMT_B12->SetLineColorAlpha(color_B12,1.0);

    // 得到Both的Graph
    TGraphErrors *npeLPMTtonpeSPMT_Both;
    // TString GraphName = "P0 LPMT #mu to SPMT #mu_QE_PDE";
    npeLPMTtonpeSPMT_Both = (TGraphErrors*)inputfile_Both->Get(GraphName);
    npeLPMTtonpeSPMT_Both->SetTitle("");
    npeLPMTtonpeSPMT_Both->SetMarkerColorAlpha(color_Both,1.0);
    npeLPMTtonpeSPMT_Both->SetLineColorAlpha(color_Both,1.0);
    npeLPMTtonpeSPMT_Both->SetMarkerSize(1.3);
    npeLPMTtonpeSPMT_Both->SetMarkerStyle(PoinrStyle_Both);
    npeLPMTtonpeSPMT_Both->SetLineWidth(3);
    npeLPMTtonpeSPMT_Both->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_Both->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_Both->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_Both->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_Both->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_Both->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_Both->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_Both->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_Both->GetXaxis()->SetTitleOffset(1.30);
    npeLPMTtonpeSPMT_Both->GetYaxis()->SetTitleOffset(0.86);

    TGraphErrors *TruenpeLPMTtonpeSPMT_Both;
    // TString GraphName_True = "P0 True LPMT #mu to SPMT #mu_QE_PDE";
    TruenpeLPMTtonpeSPMT_Both = (TGraphErrors*)inputfile_Both->Get(GraphName_True);
    TruenpeLPMTtonpeSPMT_Both->SetMarkerColorAlpha(color_Both,1.0);
    TruenpeLPMTtonpeSPMT_Both->SetLineColorAlpha(color_Both,1.0);


    TMultiGraph *npeLPMTtonpeSPMT_tot = new TMultiGraph();
    npeLPMTtonpeSPMT_tot->Add(npeLPMTtonpeSPMT_Neutron,"P");
    npeLPMTtonpeSPMT_tot->Add(npeLPMTtonpeSPMT_B12,"P");
    npeLPMTtonpeSPMT_tot->Add(npeLPMTtonpeSPMT_Both,"P");

    npeLPMTtonpeSPMT_tot->SetTitle("");
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu_{SPMT}/(QE_{SPMT}/PDE_{LPMT}) [p.e.]");
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    npeLPMTtonpeSPMT_tot->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleOffset(1.2);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleOffset(0.8);


    TMultiGraph *TruenpeLPMTtonpeSPMT_tot = new TMultiGraph();
    TruenpeLPMTtonpeSPMT_tot->Add(TruenpeLPMTtonpeSPMT_Neutron,"P");
    TruenpeLPMTtonpeSPMT_tot->Add(TruenpeLPMTtonpeSPMT_B12,"P");
    TruenpeLPMTtonpeSPMT_tot->Add(TruenpeLPMTtonpeSPMT_Both,"P");

    TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,0.02);
    Below1PE->SetParNames("k");
    Below1PE->SetLineColor(kRed);
    Below1PE->SetParameter(0,50);

    TruenpeLPMTtonpeSPMT_Both->Fit(Below1PE,"R F");
    Below1PE->SetRange(0,0.55);
    TLegend *legendb =new TLegend(0.6031519,0.4327731,0.9942693,0.6365546);
    legendb->SetTextFont(22);
    legendb->AddEntry(npeLPMTtonpeSPMT_B12,"^{12}B","pe");
    legendb->AddEntry(npeLPMTtonpeSPMT_Neutron,"Neutron","pe"); 
    legendb->AddEntry(npeLPMTtonpeSPMT_Both,"^{12}B + Neutron","pe"); 
    legendb->SetLineColor(0);
    legendb->SetLineColor(0);
    legendb->SetFillStyle(0);
    legendb->SetLineWidth(0);
    legendb->SetLineStyle(0);


    TCanvas *c1 = new TCanvas("c1","c1",800,600);
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
    legendb->Draw("same");
    c1->Update();
    TPaveStats *s = (TPaveStats*)npeLPMTtonpeSPMT_B12->GetListOfFunctions()->FindObject("stats");
    s->SetX1NDC(0.5974212);
    s->SetX2NDC(0.9584527);
    s->SetY1NDC(0.2605042);
    s->SetY2NDC(0.4222689);

    TPaveStats *s_N = (TPaveStats*)npeLPMTtonpeSPMT_Neutron->GetListOfFunctions()->FindObject("stats");
    s_N->SetX1NDC(0);
    s_N->SetX2NDC(0);
    s_N->SetY1NDC(0);
    s_N->SetY2NDC(0);

    vector<double> Nonlinearity_Neutron;
    vector<double> NonlinearityError_Neutron;
    vector<double> TrueNonlinearity_Neutron;
    vector<double> TrueNonlinearityError_Neutron;
    vector<double> LPMTMu_Neutron;
    vector<double> LPMTMuError_Neutron;
    vector<double> ResidualNonlinearity_Neutron;
    vector<double> ResidualNonlinearityError_Neutron;

    for (int i = 0; i < npeLPMTtonpeSPMT_Neutron->GetN(); i++)
    {
        if (npeLPMTtonpeSPMT_Neutron->GetPointY(i) > 0)
        {
            double LPMTMui = npeLPMTtonpeSPMT_Neutron->GetPointY(i);
            double LPMTMuErrori = npeLPMTtonpeSPMT_Neutron->GetErrorY(i);
            double CalibLPMTMui = npeLPMTtonpeSPMT_Neutron->GetPointX(i)*Below1PE->GetParameter(0);
            double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_Neutron->GetErrorX(i)*npeLPMTtonpeSPMT_Neutron->GetErrorX(i))/(npeLPMTtonpeSPMT_Neutron->GetPointX(i)*npeLPMTtonpeSPMT_Neutron->GetPointX(i)) + (Below1PE->GetParError(0)*Below1PE->GetParError(0))/(Below1PE->GetParameter(0)*Below1PE->GetParameter(0)));
            double TruePMTMui = TruenpeLPMTtonpeSPMT_Neutron->GetPointY(i);
            double TruePMTMuErrori = TruenpeLPMTtonpeSPMT_Neutron->GetErrorY(i);
            double Nonlinearityi = 100*(LPMTMui-CalibLPMTMui)/(CalibLPMTMui);
            double NonlinearityErrori = 100*(LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));
            double TrueNonlinearityi = 100*(LPMTMui-TruePMTMui)/(TruePMTMui);
            double TrueNonlinearityErrori = 100*(LPMTMui/TruePMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (TruePMTMuErrori*TruePMTMuErrori)/(TruePMTMui*TruePMTMui));

            LPMTMu_Neutron.push_back(LPMTMui);
            LPMTMuError_Neutron.push_back(LPMTMuErrori);
            Nonlinearity_Neutron.push_back(Nonlinearityi);
            NonlinearityError_Neutron.push_back(NonlinearityErrori);
            TrueNonlinearity_Neutron.push_back(TrueNonlinearityi);
            TrueNonlinearityError_Neutron.push_back(TrueNonlinearityErrori);
            ResidualNonlinearity_Neutron.push_back(Nonlinearityi-SetNonlinearity->Eval(LPMTMui));
            ResidualNonlinearityError_Neutron.push_back(NonlinearityErrori+LPMTMuErrori*SetNonlinearity->Derivative(LPMTMui));
        }
    }

    vector<double> Nonlinearity_B12;
    vector<double> NonlinearityError_B12;
    vector<double> TrueNonlinearity_B12;
    vector<double> TrueNonlinearityError_B12;
    vector<double> LPMTMu_B12;
    vector<double> LPMTMuError_B12;
    vector<double> ResidualNonlinearity_B12;
    vector<double> ResidualNonlinearityError_B12;

    for (int i = 0; i < npeLPMTtonpeSPMT_B12->GetN(); i++)
    {
        if (npeLPMTtonpeSPMT_B12->GetPointY(i) > 0)
        {
            double LPMTMui = npeLPMTtonpeSPMT_B12->GetPointY(i);
            double LPMTMuErrori = npeLPMTtonpeSPMT_B12->GetErrorY(i);
            double CalibLPMTMui = npeLPMTtonpeSPMT_B12->GetPointX(i)*Below1PE->GetParameter(0);
            double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_B12->GetErrorX(i)*npeLPMTtonpeSPMT_B12->GetErrorX(i))/(npeLPMTtonpeSPMT_B12->GetPointX(i)*npeLPMTtonpeSPMT_B12->GetPointX(i)) + (Below1PE->GetParError(0)*Below1PE->GetParError(0))/(Below1PE->GetParameter(0)*Below1PE->GetParameter(0)));
            double TruePMTMui = TruenpeLPMTtonpeSPMT_B12->GetPointY(i);
            double TruePMTMuErrori = TruenpeLPMTtonpeSPMT_B12->GetErrorY(i);
            double Nonlinearityi = 100.*(LPMTMui-CalibLPMTMui)/(CalibLPMTMui);
            double NonlinearityErrori = 100.*(LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));
            double TrueNonlinearityi = 100.*(LPMTMui-TruePMTMui)/(TruePMTMui);
            double TrueNonlinearityErrori = 100.*(LPMTMui/TruePMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (TruePMTMuErrori*TruePMTMuErrori)/(TruePMTMui*TruePMTMui));

            LPMTMu_B12.push_back(LPMTMui);
            LPMTMuError_B12.push_back(LPMTMuErrori);
            Nonlinearity_B12.push_back(Nonlinearityi);
            NonlinearityError_B12.push_back(NonlinearityErrori);
            TrueNonlinearity_B12.push_back(TrueNonlinearityi);
            TrueNonlinearityError_B12.push_back(TrueNonlinearityErrori);
            ResidualNonlinearity_B12.push_back(Nonlinearityi-SetNonlinearity->Eval(LPMTMui));
            ResidualNonlinearityError_B12.push_back(NonlinearityErrori+LPMTMuErrori*SetNonlinearity->Derivative(LPMTMui));
        }
    }

    vector<double> Nonlinearity_Both;
    vector<double> NonlinearityError_Both;
    vector<double> TrueNonlinearity_Both;
    vector<double> TrueNonlinearityError_Both;
    vector<double> LPMTMu_Both;
    vector<double> LPMTMuError_Both;
    vector<double> ResidualNonlinearity_Both;
    vector<double> ResidualNonlinearityError_Both;

    for (int i = 0; i < npeLPMTtonpeSPMT_Both->GetN(); i++)
    {
        if (npeLPMTtonpeSPMT_Both->GetPointY(i) > 0)
        {
            double LPMTMui = npeLPMTtonpeSPMT_Both->GetPointY(i);
            double LPMTMuErrori = npeLPMTtonpeSPMT_Both->GetErrorY(i);
            double CalibLPMTMui = npeLPMTtonpeSPMT_Both->GetPointX(i)*Below1PE->GetParameter(0);
            double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_Both->GetErrorX(i)*npeLPMTtonpeSPMT_Both->GetErrorX(i))/(npeLPMTtonpeSPMT_Both->GetPointX(i)*npeLPMTtonpeSPMT_Both->GetPointX(i)) + (Below1PE->GetParError(0)*Below1PE->GetParError(0))/(Below1PE->GetParameter(0)*Below1PE->GetParameter(0)));
            double TruePMTMui = TruenpeLPMTtonpeSPMT_Both->GetPointY(i);
            double TruePMTMuErrori = TruenpeLPMTtonpeSPMT_Both->GetErrorY(i);
            double Nonlinearityi = 100.*(LPMTMui-CalibLPMTMui)/(CalibLPMTMui);
            double NonlinearityErrori = 100.*(LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));
            double TrueNonlinearityi = 100.*(LPMTMui-TruePMTMui)/(TruePMTMui);
            double TrueNonlinearityErrori = 100.*(LPMTMui/TruePMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (TruePMTMuErrori*TruePMTMuErrori)/(TruePMTMui*TruePMTMui));

            LPMTMu_Both.push_back(LPMTMui);
            LPMTMuError_Both.push_back(LPMTMuErrori);
            Nonlinearity_Both.push_back(Nonlinearityi);
            NonlinearityError_Both.push_back(NonlinearityErrori);
            TrueNonlinearity_Both.push_back(TrueNonlinearityi);
            TrueNonlinearityError_Both.push_back(TrueNonlinearityErrori);
            ResidualNonlinearity_Both.push_back(Nonlinearityi-SetNonlinearity->Eval(LPMTMui));
            ResidualNonlinearityError_Both.push_back(NonlinearityErrori+LPMTMuErrori*SetNonlinearity->Derivative(LPMTMui));
        }
    }

    TGraphErrors *NonlinearityLPMTtoMu_B12 = new TGraphErrors(LPMTMu_B12.size(),&LPMTMu_B12[0],&Nonlinearity_B12[0],&LPMTMuError_B12[0],&NonlinearityError_B12[0]);
    NonlinearityLPMTtoMu_B12->SetTitle("");
    NonlinearityLPMTtoMu_B12->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    NonlinearityLPMTtoMu_B12->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    NonlinearityLPMTtoMu_B12->SetFillStyle(3002);
    NonlinearityLPMTtoMu_B12->SetMarkerColorAlpha(color_B12,1.0);
    NonlinearityLPMTtoMu_B12->SetLineColorAlpha(color_B12,1.0);
    NonlinearityLPMTtoMu_B12->SetMarkerSize(1.3);
    NonlinearityLPMTtoMu_B12->SetMarkerStyle(PoinrStyle_B12);
    NonlinearityLPMTtoMu_B12->SetLineWidth(3);
    NonlinearityLPMTtoMu_B12->GetXaxis()->CenterTitle();
    NonlinearityLPMTtoMu_B12->GetYaxis()->CenterTitle();
    NonlinearityLPMTtoMu_B12->GetXaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu_B12->GetYaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu_B12->GetXaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu_B12->GetYaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu_B12->GetXaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu_B12->GetYaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu_B12->GetXaxis()->SetTitleOffset(1.2);
    NonlinearityLPMTtoMu_B12->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *NonlinearityLPMTtoMu_Neutron = new TGraphErrors(LPMTMu_Neutron.size(),&LPMTMu_Neutron[0],&Nonlinearity_Neutron[0],&LPMTMuError_Neutron[0],&NonlinearityError_Neutron[0]);
    NonlinearityLPMTtoMu_Neutron->SetTitle("");
    NonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    NonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    NonlinearityLPMTtoMu_Neutron->SetFillStyle(3002);
    NonlinearityLPMTtoMu_Neutron->SetMarkerColorAlpha(color_Neutron,1.0);
    NonlinearityLPMTtoMu_Neutron->SetLineColorAlpha(color_Neutron,1.0);
    NonlinearityLPMTtoMu_Neutron->SetMarkerSize(1.3);
    NonlinearityLPMTtoMu_Neutron->SetMarkerStyle(PoinrStyle_Neutron);
    NonlinearityLPMTtoMu_Neutron->SetLineWidth(3);
    NonlinearityLPMTtoMu_Neutron->GetXaxis()->CenterTitle();
    NonlinearityLPMTtoMu_Neutron->GetYaxis()->CenterTitle();
    NonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu_Neutron->GetXaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu_Neutron->GetYaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitleOffset(1.2);
    NonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *NonlinearityLPMTtoMu_Both = new TGraphErrors(LPMTMu_Both.size(),&LPMTMu_Both[0],&Nonlinearity_Both[0],&LPMTMuError_Both[0],&NonlinearityError_Both[0]);
    NonlinearityLPMTtoMu_Both->SetTitle("");
    NonlinearityLPMTtoMu_Both->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    NonlinearityLPMTtoMu_Both->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    NonlinearityLPMTtoMu_Both->SetFillStyle(3002);
    NonlinearityLPMTtoMu_Both->SetMarkerColorAlpha(color_Both,1.0);
    NonlinearityLPMTtoMu_Both->SetLineColorAlpha(color_Both,1.0);
    NonlinearityLPMTtoMu_Both->SetMarkerSize(1.3);
    NonlinearityLPMTtoMu_Both->SetMarkerStyle(PoinrStyle_Both);
    NonlinearityLPMTtoMu_Both->SetLineWidth(3);
    NonlinearityLPMTtoMu_Both->GetXaxis()->CenterTitle();
    NonlinearityLPMTtoMu_Both->GetYaxis()->CenterTitle();
    NonlinearityLPMTtoMu_Both->GetXaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu_Both->GetYaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu_Both->GetXaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu_Both->GetYaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu_Both->GetXaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu_Both->GetYaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu_Both->GetXaxis()->SetTitleOffset(1.2);
    NonlinearityLPMTtoMu_Both->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *TrueNonlinearityLPMTtoMu_B12 = new TGraphErrors(LPMTMu_B12.size(),&LPMTMu_B12[0],&TrueNonlinearity_B12[0],&LPMTMuError_B12[0],&TrueNonlinearityError_B12[0]);
    TrueNonlinearityLPMTtoMu_B12->SetTitle("");
    TrueNonlinearityLPMTtoMu_B12->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    TrueNonlinearityLPMTtoMu_B12->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    TrueNonlinearityLPMTtoMu_B12->SetFillStyle(3002);
    TrueNonlinearityLPMTtoMu_B12->SetMarkerColorAlpha(color_B12,1.0);
    TrueNonlinearityLPMTtoMu_B12->SetLineColorAlpha(color_B12,1.0);
    TrueNonlinearityLPMTtoMu_B12->SetMarkerSize(1.3);
    TrueNonlinearityLPMTtoMu_B12->SetMarkerStyle(PoinrStyle_B12);
    TrueNonlinearityLPMTtoMu_B12->SetLineWidth(3);
    TrueNonlinearityLPMTtoMu_B12->GetXaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu_B12->GetYaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu_B12->GetXaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu_B12->GetYaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu_B12->GetXaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu_B12->GetYaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu_B12->GetXaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu_B12->GetYaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu_B12->GetXaxis()->SetTitleOffset(1.2);
    TrueNonlinearityLPMTtoMu_B12->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *TrueNonlinearityLPMTtoMu_Neutron = new TGraphErrors(LPMTMu_Neutron.size(),&LPMTMu_Neutron[0],&TrueNonlinearity_Neutron[0],&LPMTMuError_Neutron[0],&TrueNonlinearityError_Neutron[0]);
    TrueNonlinearityLPMTtoMu_Neutron->SetTitle("");
    TrueNonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    TrueNonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    TrueNonlinearityLPMTtoMu_Neutron->SetFillStyle(3002);
    TrueNonlinearityLPMTtoMu_Neutron->SetMarkerColorAlpha(color_Neutron,1.0);
    TrueNonlinearityLPMTtoMu_Neutron->SetLineColorAlpha(color_Neutron,1.0);
    TrueNonlinearityLPMTtoMu_Neutron->SetMarkerSize(1.3);
    TrueNonlinearityLPMTtoMu_Neutron->SetMarkerStyle(PoinrStyle_Neutron);
    TrueNonlinearityLPMTtoMu_Neutron->SetLineWidth(3);
    TrueNonlinearityLPMTtoMu_Neutron->GetXaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu_Neutron->GetYaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu_Neutron->GetXaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu_Neutron->GetYaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu_Neutron->GetXaxis()->SetTitleOffset(1.2);
    TrueNonlinearityLPMTtoMu_Neutron->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *TrueNonlinearityLPMTtoMu_Both = new TGraphErrors(LPMTMu_Both.size(),&LPMTMu_Both[0],&TrueNonlinearity_Both[0],&LPMTMuError_Both[0],&TrueNonlinearityError_Both[0]);
    TrueNonlinearityLPMTtoMu_Both->SetTitle("");
    TrueNonlinearityLPMTtoMu_Both->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    TrueNonlinearityLPMTtoMu_Both->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    TrueNonlinearityLPMTtoMu_Both->SetFillStyle(3002);
    TrueNonlinearityLPMTtoMu_Both->SetMarkerColorAlpha(color_Both,1.0);
    TrueNonlinearityLPMTtoMu_Both->SetLineColorAlpha(color_Both,1.0);
    TrueNonlinearityLPMTtoMu_Both->SetMarkerSize(1.3);
    TrueNonlinearityLPMTtoMu_Both->SetMarkerStyle(PoinrStyle_Both);
    TrueNonlinearityLPMTtoMu_Both->SetLineWidth(3);
    TrueNonlinearityLPMTtoMu_Both->GetXaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu_Both->GetYaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu_Both->GetXaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu_Both->GetYaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu_Both->GetXaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu_Both->GetYaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu_Both->GetXaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu_Both->GetYaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu_Both->GetXaxis()->SetTitleOffset(1.2);
    TrueNonlinearityLPMTtoMu_Both->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *GraphResidualNonlinearity_B12 = new TGraphErrors(LPMTMu_B12.size(),&LPMTMu_B12[0],&ResidualNonlinearity_B12[0],&LPMTMuError_B12[0],&ResidualNonlinearityError_B12[0]);
    GraphResidualNonlinearity_B12->SetTitle("");
    GraphResidualNonlinearity_B12->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    GraphResidualNonlinearity_B12->GetYaxis()->SetTitle("Residual nonlinearity [%]");
    GraphResidualNonlinearity_B12->SetFillStyle(3002);
    GraphResidualNonlinearity_B12->SetMarkerColorAlpha(color_B12,1.0);
    GraphResidualNonlinearity_B12->SetLineColorAlpha(color_B12,1.0);
    GraphResidualNonlinearity_B12->SetMarkerSize(1.3);
    GraphResidualNonlinearity_B12->SetMarkerStyle(PoinrStyle_B12);
    GraphResidualNonlinearity_B12->SetLineWidth(3);
    GraphResidualNonlinearity_B12->GetXaxis()->CenterTitle();
    GraphResidualNonlinearity_B12->GetYaxis()->CenterTitle();
    GraphResidualNonlinearity_B12->GetXaxis()->SetTitleFont(22);
    GraphResidualNonlinearity_B12->GetYaxis()->SetTitleFont(22);
    GraphResidualNonlinearity_B12->GetXaxis()->SetTitleSize(0.06);
    GraphResidualNonlinearity_B12->GetYaxis()->SetTitleSize(0.06);
    GraphResidualNonlinearity_B12->GetXaxis()->SetLabelSize(0.06);
    GraphResidualNonlinearity_B12->GetYaxis()->SetLabelSize(0.06);
    GraphResidualNonlinearity_B12->GetXaxis()->SetTitleOffset(1.2);
    GraphResidualNonlinearity_B12->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *GraphResidualNonlinearity_Neutron = new TGraphErrors(LPMTMu_Neutron.size(),&LPMTMu_Neutron[0],&ResidualNonlinearity_Neutron[0],&LPMTMuError_Neutron[0],&ResidualNonlinearityError_Neutron[0]);
    GraphResidualNonlinearity_Neutron->SetTitle("");
    GraphResidualNonlinearity_Neutron->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    GraphResidualNonlinearity_Neutron->GetYaxis()->SetTitle("Residual nonlinearity [%]");
    GraphResidualNonlinearity_Neutron->SetFillStyle(3002);
    GraphResidualNonlinearity_Neutron->SetMarkerColorAlpha(color_Neutron,1.0);
    GraphResidualNonlinearity_Neutron->SetLineColorAlpha(color_Neutron,1.0);
    GraphResidualNonlinearity_Neutron->SetMarkerSize(1.3);
    GraphResidualNonlinearity_Neutron->SetMarkerStyle(PoinrStyle_Neutron);
    GraphResidualNonlinearity_Neutron->SetLineWidth(3);
    GraphResidualNonlinearity_Neutron->GetXaxis()->CenterTitle();
    GraphResidualNonlinearity_Neutron->GetYaxis()->CenterTitle();
    GraphResidualNonlinearity_Neutron->GetXaxis()->SetTitleFont(22);
    GraphResidualNonlinearity_Neutron->GetYaxis()->SetTitleFont(22);
    GraphResidualNonlinearity_Neutron->GetXaxis()->SetTitleSize(0.06);
    GraphResidualNonlinearity_Neutron->GetYaxis()->SetTitleSize(0.06);
    GraphResidualNonlinearity_Neutron->GetXaxis()->SetLabelSize(0.06);
    GraphResidualNonlinearity_Neutron->GetYaxis()->SetLabelSize(0.06);
    GraphResidualNonlinearity_Neutron->GetXaxis()->SetTitleOffset(1.2);
    GraphResidualNonlinearity_Neutron->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *GraphResidualNonlinearity_Both = new TGraphErrors(LPMTMu_Both.size(),&LPMTMu_Both[0],&ResidualNonlinearity_Both[0],&LPMTMuError_Both[0],&ResidualNonlinearityError_Both[0]);
    GraphResidualNonlinearity_Both->SetTitle("");
    GraphResidualNonlinearity_Both->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    GraphResidualNonlinearity_Both->GetYaxis()->SetTitle("Residual nonlinearity [%]");
    GraphResidualNonlinearity_Both->SetFillStyle(3002);
    GraphResidualNonlinearity_Both->SetMarkerColorAlpha(color_Both,1.0);
    GraphResidualNonlinearity_Both->SetLineColorAlpha(color_Both,1.0);
    GraphResidualNonlinearity_Both->SetMarkerSize(1.3);
    GraphResidualNonlinearity_Both->SetMarkerStyle(PoinrStyle_Both);
    GraphResidualNonlinearity_Both->SetLineWidth(3);
    GraphResidualNonlinearity_Both->GetXaxis()->CenterTitle();
    GraphResidualNonlinearity_Both->GetYaxis()->CenterTitle();
    GraphResidualNonlinearity_Both->GetXaxis()->SetTitleFont(22);
    GraphResidualNonlinearity_Both->GetYaxis()->SetTitleFont(22);
    GraphResidualNonlinearity_Both->GetXaxis()->SetTitleSize(0.06);
    GraphResidualNonlinearity_Both->GetYaxis()->SetTitleSize(0.06);
    GraphResidualNonlinearity_Both->GetXaxis()->SetLabelSize(0.06);
    GraphResidualNonlinearity_Both->GetYaxis()->SetLabelSize(0.06);
    GraphResidualNonlinearity_Both->GetXaxis()->SetTitleOffset(1.2);
    GraphResidualNonlinearity_Both->GetYaxis()->SetTitleOffset(1.30);

    TMultiGraph *NonlinearityLPMTtoMu = new TMultiGraph();
    NonlinearityLPMTtoMu->SetTitle("");
    
    TMultiGraph *TrueNonlinearityLPMTtoMu = new TMultiGraph();

    TMultiGraph *GraphResidualNonlinearity = new TMultiGraph();

    GraphResidualNonlinearity->Add(GraphResidualNonlinearity_B12,"P");
    GraphResidualNonlinearity->Add(GraphResidualNonlinearity_Neutron,"P");
    GraphResidualNonlinearity->Add(GraphResidualNonlinearity_Both,"P");

    NonlinearityLPMTtoMu->Add(NonlinearityLPMTtoMu_B12,"P");
    NonlinearityLPMTtoMu->Add(NonlinearityLPMTtoMu_Neutron,"P");
    NonlinearityLPMTtoMu->Add(NonlinearityLPMTtoMu_Both,"P");

    TrueNonlinearityLPMTtoMu->Add(TrueNonlinearityLPMTtoMu_B12,"P");
    TrueNonlinearityLPMTtoMu->Add(TrueNonlinearityLPMTtoMu_Neutron,"P");
    TrueNonlinearityLPMTtoMu->Add(TrueNonlinearityLPMTtoMu_Both,"P");

    TLegend *legenda =new TLegend(0.1428571,0.004789272,0.585213,0.519636,NULL,"brNDC");
    legenda->SetTextFont(22);
    legenda->AddEntry(NonlinearityLPMTtoMu_B12,"^{12}B","pe");
    legenda->AddEntry(NonlinearityLPMTtoMu_Neutron,"Neutron","pe");
    legenda->AddEntry(NonlinearityLPMTtoMu_Both,"^{12}B + Neutron","pe");
    legenda->AddEntry(SetNonlinearity,"Set value","l"); 
    legenda->SetLineColor(0);
    legenda->SetFillStyle(0);
    legenda->SetLineWidth(0);
    legenda->SetLineStyle(0);

    TCanvas *NonlinearityLPMTtoMu_Canvas = new TCanvas("NonlinearityLPMTtoMu_Canvas","NonlinearityLPMTtoMu_Canvas",800,600);
    TPad *pad1 = new TPad("pad1", "pad1", 0.00, 0.4, 1.00, 0.98);
    TPad *pad2 = new TPad("pad2", "pad2", 0.00, 0.00, 1.00, 0.4);
    pad1->SetMargin(0.12, 0.03, 0., 0.0);
    pad2->SetMargin(0.12, 0.03, 0.38, 0.0);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();
    // NonlinearityLPMTtoMu_Canvas->SetBorderMode(0);
    // NonlinearityLPMTtoMu_Canvas->SetBorderSize(2);
    // NonlinearityLPMTtoMu_Canvas->SetLeftMargin(0.1504298);
    // NonlinearityLPMTtoMu_Canvas->SetRightMargin(0.0487106);
    // NonlinearityLPMTtoMu_Canvas->SetTopMargin(0.02941176);
    // NonlinearityLPMTtoMu_Canvas->SetBottomMargin(0.1701681);
    // NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
    // NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
    // NonlinearityLPMTtoMu_Canvas->cd();

    NonlinearityLPMTtoMu->Draw("A P");
    // TrueNonlinearityLPMTtoMu->Draw("P");
    SetNonlinearity->Draw("same");
    legenda->Draw("same");

    NonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    NonlinearityLPMTtoMu->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
    NonlinearityLPMTtoMu->GetYaxis()->CenterTitle(true);
    NonlinearityLPMTtoMu->GetYaxis()->SetLabelFont(42);
    NonlinearityLPMTtoMu->GetYaxis()->SetLabelSize(0.08);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleSize(0.1);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleOffset(0.37);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu->GetXaxis()->SetLimits(0.01,30);
    NonlinearityLPMTtoMu->GetYaxis()->SetRangeUser(-3.5,1.5);
    NonlinearityLPMTtoMu->GetYaxis()->SetNdivisions(505);

    GraphResidualNonlinearity->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
    GraphResidualNonlinearity->GetXaxis()->CenterTitle(true);
    GraphResidualNonlinearity->GetXaxis()->SetLabelFont(42);
    GraphResidualNonlinearity->GetXaxis()->SetLabelSize(0.12);
    GraphResidualNonlinearity->GetXaxis()->SetTitleSize(0.16);
    GraphResidualNonlinearity->GetXaxis()->SetTitleOffset(1);
    GraphResidualNonlinearity->GetXaxis()->SetTitleFont(22);
    GraphResidualNonlinearity->GetYaxis()->SetTitle("Residual [%]");
    GraphResidualNonlinearity->GetYaxis()->CenterTitle(true);
    GraphResidualNonlinearity->GetYaxis()->SetLabelFont(42);
    GraphResidualNonlinearity->GetYaxis()->SetLabelSize(0.12);
    GraphResidualNonlinearity->GetYaxis()->SetTitleSize(0.12);
    GraphResidualNonlinearity->GetYaxis()->SetTitleOffset(0.30);
    GraphResidualNonlinearity->GetYaxis()->SetTitleFont(22);
    GraphResidualNonlinearity->GetYaxis()->SetNdivisions(205);
    GraphResidualNonlinearity->GetXaxis()->SetLimits(0.01,30);
    GraphResidualNonlinearity->GetYaxis()->SetRangeUser(-2.5,2.5);
    pad2->cd();
    GraphResidualNonlinearity->Draw("A P Z");
    TLine *l = new TLine(0,0,LPMTMu_B12.back(),0);
    l->SetLineStyle(2);
    l->Draw("same");

    pad1->SetLogx();
    pad2->SetLogx();
}
