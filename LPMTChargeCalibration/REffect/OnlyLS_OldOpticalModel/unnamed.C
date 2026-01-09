#ifdef __CLING__
#pragma cling optimize(0)
#endif
void unnamed()
{
//=========Macro generated from canvas: c1/c1
//=========  (Wed Jun 18 23:28:57 2025) by ROOT version 6.32.02
   TCanvas *c1 = new TCanvas("c1", "c1",563,170,800,600);
   c1->Range(-5.110121,44.26348,39.03554,46.26696);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLeftMargin(0.1365915);
   c1->SetRightMargin(0.06390978);
   c1->SetTopMargin(0.08333334);
   c1->SetBottomMargin(0.1180556);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1001[30] = { 7.3643, 12.2284, 14.55261, 16.28582, 17.70701, 18.92851, 20.00887, 20.98312, 21.8741, 22.69765, 23.46527, 24.18561, 24.86535, 25.50977, 26.12314, 26.70896, 27.27012,
   27.80907, 28.32789, 28.82836, 29.31202, 29.78021, 30.23412, 30.67479, 31.10315, 31.52001, 31.92613, 32.32216, 32.70872, 33.08635 };
   Double_t Graph0_fy1001[30] = { 45.57664, 45.47212, 45.4276, 45.34288, 45.28726, 45.2695, 45.21373, 45.24086, 45.16691, 45.12049, 45.15176, 45.11728, 45.05668, 45.08806, 45.09256, 45.0612, 45.14717,
   45.13994, 45.11735, 45.01157, 45.16484, 45.19526, 45.19109, 45.28125, 45.20703, 45.26133, 45.39252, 45.27368, 45.34632, 45.41528 };
   Double_t Graph0_fex1001[30] = { 3.5033, 1.360804, 0.9634038, 0.7698064, 0.651385, 0.5701118, 0.5102521, 0.463995, 0.4269812, 0.3965686, 0.3710538, 0.3492851, 0.330453, 0.3139708, 0.2994018, 0.2864136, 0.2747486,
   0.2642033, 0.254615, 0.2458521, 0.2378063, 0.2303881, 0.2235227, 0.217147, 0.2112073, 0.2056579, 0.2004592, 0.1955771, 0.1909819, 0.1866474 };
   Double_t Graph0_fey1001[30] = { 0.03257287, 0.01271174, 0.01360001, 0.014258, 0.01600712, 0.01741355, 0.01845635, 0.01946145, 0.01981623, 0.02110749, 0.02254087, 0.02343641, 0.02389628, 0.02489981, 0.02588352, 0.02684157, 0.02759537,
   0.02789996, 0.02871491, 0.02958278, 0.03058448, 0.03096458, 0.03145051, 0.03244464, 0.03280125, 0.03341214, 0.03470271, 0.03555835, 0.03628714, 0.03662258 };
   TGraphErrors *gre = new TGraphErrors(30,Graph0_fx1001,Graph0_fy1001,Graph0_fex1001,Graph0_fey1001);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillStyle(3002);
   gre->SetLineColor(4);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_Graph01001 = new TH1F("Graph_Graph01001","",100,0.9198,36.2142);
   Graph_Graph01001->SetMinimum(44.5);
   Graph_Graph01001->SetMaximum(46.1);
   Graph_Graph01001->SetDirectory(nullptr);
   Graph_Graph01001->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph01001->SetLineColor(ci);
   Graph_Graph01001->GetXaxis()->SetTitle("R [m]");
   Graph_Graph01001->GetXaxis()->CenterTitle(true);
   Graph_Graph01001->GetXaxis()->SetLabelFont(42);
   Graph_Graph01001->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph01001->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph01001->GetXaxis()->SetTitleOffset(0.92);
   Graph_Graph01001->GetXaxis()->SetTitleFont(22);
   Graph_Graph01001->GetYaxis()->SetTitle("Calibration factor");
   Graph_Graph01001->GetYaxis()->CenterTitle(true);
   Graph_Graph01001->GetYaxis()->SetLabelFont(42);
   Graph_Graph01001->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph01001->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph01001->GetYaxis()->SetTitleOffset(1.08);
   Graph_Graph01001->GetYaxis()->SetTitleFont(22);
   Graph_Graph01001->GetZaxis()->SetLabelFont(42);
   Graph_Graph01001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph01001);
   
   gre->Draw("alp");
   TText *text = new TText(12.762,45.99444,"Fluctuation about 1.3%");
   text->SetTextAlign(22);

   ci = TColor::GetColor("#990000");
   text->SetTextColor(ci);
   text->SetTextFont(43);
   text->SetTextSize(40);
   text->Draw();
   c1->Modified();
   c1->SetSelected(c1);
}
