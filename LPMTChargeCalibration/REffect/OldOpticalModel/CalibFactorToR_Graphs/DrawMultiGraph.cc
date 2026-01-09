void DrawMultiGraph()
{
    Int_t color_10DEG = TColor::GetColor(104,103,137);
    Int_t color_20DEG = TColor::GetColor(183,127,112);
    Int_t color_30DEG = TColor::GetColor(229,226,185);
    Int_t color_40DEG = TColor::GetColor(190,177,168);
    Int_t color_NoCut = TColor::GetColor(138,149,169);
    Int_t color_TrueCut = TColor::GetColor(148, 0, 211);
    Int_t color_Line = TColor::GetColor(255,0,0);

    TFile *inputfile_10DEG = TFile::Open("Graph_10DEG.root","read");
    TFile *inputfile_20DEG = TFile::Open("Graph_20DEG.root","read");
    TFile *inputfile_30DEG = TFile::Open("Graph_30DEG.root","read");
    TFile *inputfile_40DEG = TFile::Open("Graph_40DEG.root","read");
    TFile *inputfile_NoCut = TFile::Open("Graph_NoCut.root","read");
    TFile *inputfile_TrueCut = TFile::Open("Graph_TrueCut.root","read");

    TGraphErrors *Graph_10DEG = (TGraphErrors*)inputfile_10DEG->Get("CalibFactorToR");
    TGraphErrors *Graph_20DEG = (TGraphErrors*)inputfile_20DEG->Get("CalibFactorToR");
    TGraphErrors *Graph_30DEG = (TGraphErrors*)inputfile_30DEG->Get("CalibFactorToR");
    TGraphErrors *Graph_40DEG = (TGraphErrors*)inputfile_40DEG->Get("CalibFactorToR");
    TGraphErrors *Graph_NoCut = (TGraphErrors*)inputfile_NoCut->Get("CalibFactorToR");
    TGraphErrors *Graph_TrueCut = (TGraphErrors*)inputfile_TrueCut->Get("CalibFactorToR");

    Graph_10DEG->SetMarkerColor(color_10DEG);
    Graph_20DEG->SetMarkerColor(color_20DEG);
    Graph_30DEG->SetMarkerColor(color_30DEG);
    Graph_40DEG->SetMarkerColor(color_40DEG);
    Graph_NoCut->SetMarkerColor(color_NoCut);
    Graph_TrueCut->SetMarkerColor(color_TrueCut);

    Graph_10DEG->SetLineColor(color_10DEG);
    Graph_10DEG->SetMarkerStyle(25);
    Graph_20DEG->SetLineColor(color_20DEG);
    Graph_30DEG->SetLineColor(color_30DEG);
    Graph_40DEG->SetLineColor(color_40DEG);
    Graph_NoCut->SetLineColor(color_NoCut);
    Graph_NoCut->SetMarkerStyle(21);
    Graph_TrueCut->SetLineColor(color_TrueCut);
    Graph_TrueCut->SetMarkerStyle(20);

    TMultiGraph *CalibFactorToR_tot = new TMultiGraph();
    CalibFactorToR_tot->Add(Graph_10DEG,"P");
    // CalibFactorToR_tot->Add(Graph_20DEG,"P");
    // CalibFactorToR_tot->Add(Graph_30DEG,"P");
    // CalibFactorToR_tot->Add(Graph_40DEG,"P");
    CalibFactorToR_tot->Add(Graph_NoCut,"P");
    CalibFactorToR_tot->Add(Graph_TrueCut,"P");

    CalibFactorToR_tot->GetXaxis()->SetTitle("R [m]");
    CalibFactorToR_tot->GetYaxis()->SetTitle("Calibration factor");
    CalibFactorToR_tot->GetXaxis()->CenterTitle();
    CalibFactorToR_tot->GetYaxis()->CenterTitle();
    CalibFactorToR_tot->GetXaxis()->SetTitleFont(22);
    CalibFactorToR_tot->GetYaxis()->SetTitleFont(22);
    CalibFactorToR_tot->GetXaxis()->SetTitleSize(0.06);
    CalibFactorToR_tot->GetYaxis()->SetTitleSize(0.06);
    CalibFactorToR_tot->GetXaxis()->SetLabelSize(0.06);
    CalibFactorToR_tot->GetYaxis()->SetLabelSize(0.06);
    CalibFactorToR_tot->GetXaxis()->SetTitleOffset(1.0);
    CalibFactorToR_tot->GetYaxis()->SetTitleOffset(1.10);

    TLine *l = new TLine(4,43.918,34,43.918);
    l->SetLineStyle(2);
    l->SetLineWidth(3);

    TLegend *legendb =new TLegend(0.5238095,0.7016807,0.9423559,0.9054622,NULL,"brNDC");;
    legendb->SetTextFont(22);
    legendb->AddEntry(Graph_10DEG,"#theta < 10 deg","pe");
    // legendb->AddEntry(Graph_20DEG,"#theta < 20DEG","pe"); 
    // legendb->AddEntry(Graph_30DEG,"#theta < 30DEG","pe"); 
    // legendb->AddEntry(Graph_40DEG,"#theta < 40DEG","pe");
    legendb->AddEntry(Graph_NoCut,"No selection","pe"); 
    legendb->AddEntry(Graph_TrueCut,"This selection","pe");
    // legendb->SetEntrySeparation(0.15);
    TLegend *legendc =new TLegend(0.5238095,0.6113445,0.9423559,0.6617647,NULL,"brNDC");;
    legendc->SetTextFont(22);
    legendc->AddEntry(l,"#splitline{Detector center}{w/o supporting bars}","l"); 
    legendb->SetLineColor(0);
    legendb->SetLineColor(0);
    legendb->SetFillStyle(0);
    legendb->SetLineWidth(0);
    legendb->SetLineStyle(0);
    legendb->SetTextSize(0.05252101);
    legendc->SetLineColor(0);
    legendc->SetLineColor(0);
    legendc->SetFillStyle(0);
    legendc->SetLineWidth(0);
    legendc->SetLineStyle(0);
    legendc->SetTextSize(0.05252101);

    TCanvas *c1 = new TCanvas("c1", "c1",800,600);
    c1->SetLeftMargin(0.1332378);
    c1->SetRightMargin(0.06590258);
    c1->SetTopMargin(0.06932773);
    c1->SetBottomMargin(0.1302521);

    CalibFactorToR_tot->Draw("A P");
    legendb->Draw("same");
    legendc->Draw("same");
    l->Draw("same");
}