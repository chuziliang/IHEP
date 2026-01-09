void unnamed()
{
//=========Macro generated from canvas: NonlinearityLPMTtoMu_P0_Canvas/NonlinearityLPMTtoMu_P0_Canvas
//=========  (Mon Apr 29 10:42:04 2024) by ROOT version 6.24/08
   TCanvas *NonlinearityLPMTtoMu_P0_Canvas = new TCanvas("NonlinearityLPMTtoMu_P0_Canvas", "NonlinearityLPMTtoMu_P0_Canvas",18,41,700,500);
   gStyle->SetOptFit(1);
   NonlinearityLPMTtoMu_P0_Canvas->Range(-1.773997,-0.05025305,0.9309399,0.02527815);
   NonlinearityLPMTtoMu_P0_Canvas->SetFillColor(0);
   NonlinearityLPMTtoMu_P0_Canvas->SetBorderMode(0);
   NonlinearityLPMTtoMu_P0_Canvas->SetBorderSize(2);
   NonlinearityLPMTtoMu_P0_Canvas->SetLogx();
   NonlinearityLPMTtoMu_P0_Canvas->SetGridy();
   NonlinearityLPMTtoMu_P0_Canvas->SetLeftMargin(0.1504298);
   NonlinearityLPMTtoMu_P0_Canvas->SetRightMargin(0.0487106);
   NonlinearityLPMTtoMu_P0_Canvas->SetTopMargin(0.02941176);
   NonlinearityLPMTtoMu_P0_Canvas->SetBottomMargin(0.1701681);
   NonlinearityLPMTtoMu_P0_Canvas->SetFrameBorderMode(0);
   NonlinearityLPMTtoMu_P0_Canvas->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1001[15] = {
   0.04776014,
   0.1164183,
   0.2251247,
   0.3360688,
   0.4473912,
   0.5596343,
   0.6680118,
   0.7827151,
   0.8930113,
   1.00374,
   1.482743,
   2.644328,
   3.741904,
   4.854376,
   5.710975};
   Double_t Graph0_fy1001[15] = {
   0.008554743,
   0.01488674,
   -0.004986669,
   -0.00613117,
   0.006433469,
   -0.01904074,
   0.0005691091,
   -0.003553213,
   -0.01174145,
   -0.008111619,
   -0.01467383,
   -0.007255249,
   -0.001469797,
   0.001720786,
   -0.01994695};
   Double_t Graph0_fex1001[15] = {
   4.423504e-05,
   8.438657e-05,
   0.0002295274,
   0.0004270223,
   0.0006789003,
   0.0009561933,
   0.001282953,
   0.001633143,
   0.002021332,
   0.002444531,
   0.001484326,
   0.003367806,
   0.005482757,
   0.008075163,
   0.01852945};
   Double_t Graph0_fey1001[15] = {
   0.003821319,
   0.003131842,
   0.004103345,
   0.005027742,
   0.006054783,
   0.006541583,
   0.007550285,
   0.00814793,
   0.00871683,
   0.009393827,
   0.003898933,
   0.004944437,
   0.005636403,
   0.006434803,
   0.01241504};
   TGraphErrors *gre = new TGraphErrors(15,Graph0_fx1001,Graph0_fy1001,Graph0_fex1001,Graph0_fey1001);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillStyle(3002);
   gre->SetLineColor(4);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_Graph01001 = new TH1F("Graph_Graph01001","",100,0.04294432,6.297683);
   Graph_Graph01001->SetMinimum(-0.03740005);
   Graph_Graph01001->SetMaximum(0.02305664);
   Graph_Graph01001->SetDirectory(0);
   Graph_Graph01001->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph01001->SetLineColor(ci);
   Graph_Graph01001->GetXaxis()->SetTitle("#mu_{LPMT}(p.e.)");
   Graph_Graph01001->GetXaxis()->CenterTitle(true);
   Graph_Graph01001->GetXaxis()->SetLabelFont(42);
   Graph_Graph01001->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph01001->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph01001->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph01001->GetXaxis()->SetTitleFont(22);
   Graph_Graph01001->GetYaxis()->SetTitle("Nonlinearity");
   Graph_Graph01001->GetYaxis()->CenterTitle(true);
   Graph_Graph01001->GetYaxis()->SetLabelFont(42);
   Graph_Graph01001->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph01001->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph01001->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph01001->GetYaxis()->SetTitleFont(22);
   Graph_Graph01001->GetZaxis()->SetLabelFont(42);
   Graph_Graph01001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph01001);
   
   gre->Draw("a p");
   NonlinearityLPMTtoMu_P0_Canvas->Modified();
   NonlinearityLPMTtoMu_P0_Canvas->cd();
   NonlinearityLPMTtoMu_P0_Canvas->SetSelected(NonlinearityLPMTtoMu_P0_Canvas);
}
