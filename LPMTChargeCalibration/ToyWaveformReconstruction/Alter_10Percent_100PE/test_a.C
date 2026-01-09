#ifdef __CLING__
#pragma cling optimize(0)
#endif
void test_a()
{
//=========Macro generated from canvas: CalibToTrue_Canvas/CalibToTrue_Canvas
//=========  (Thu Jun 12 16:52:33 2025) by ROOT version 6.32.02
   TCanvas *CalibToTrue_Canvas = new TCanvas("CalibToTrue_Canvas", "CalibToTrue_Canvas",863,227,700,502);
   CalibToTrue_Canvas->Range(0,0,1,1);
   CalibToTrue_Canvas->SetFillColor(0);
   CalibToTrue_Canvas->SetBorderMode(0);
   CalibToTrue_Canvas->SetBorderSize(2);
   CalibToTrue_Canvas->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: pad1
   TPad *pad1__0 = new TPad("pad1", "pad1",0,0.32,1,1);
   pad1__0->Draw();
   pad1__0->cd();
   pad1__0->Range(-437363.8,1491.93,3207334,3144152);
   pad1__0->SetFillColor(0);
   pad1__0->SetBorderMode(0);
   pad1__0->SetBorderSize(2);
   pad1__0->SetLeftMargin(0.12);
   pad1__0->SetBottomMargin(0);
   pad1__0->SetFrameBorderMode(0);
   pad1__0->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1001[21] = { 1665.62, 3398.81, 5137.27, 6875.939, 8614.392, 10352.31, 12089.79, 13825.59, 15561.97, 17230.34, 34456.26, 51695.18, 86144.41, 172309, 258457, 344602.3, 516901.7,
   861482.2, 1206131, 1722972, 2584527 };
   Double_t Graph0_fy1001[21] = { 1658.119, 3393.548, 5128.51, 6869.172, 8615.293, 10348.14, 12090.01, 13828.55, 15564.68, 17241.73, 34465.23, 51715.27, 86207.33, 172355.4, 258573.5, 344651.6, 516707.7,
   861032.3, 1204317, 1718050, 2572547 };
   Double_t Graph0_fex1001[21] = { 0.1344816, 0.2993075, 0.3833868, 0.4591254, 0.5291433, 0.591348, 0.6479977, 0.7018099, 0.7584837, 0.7415149, 2.371028, 2.88041, 3.752953, 7.578463, 9.134584, 10.46266, 12.80681,
   21.448, 25.885, 38.26152, 46.48611 };
   Double_t Graph0_fey1001[21] = { 0.4194756, 1.238402, 1.50295, 1.736368, 1.94807, 2.143684, 2.330807, 2.506442, 2.676383, 2.834611, 8.446963, 10.50824, 14.00242, 27.94692, 35.52632, 42.49642, 55.43368,
   92.37382, 116.7629, 168.6407, 227.4354 };
   TGraphErrors *gre = new TGraphErrors(21,Graph0_fx1001,Graph0_fy1001,Graph0_fex1001,Graph0_fey1001);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillStyle(3002);
   gre->SetLineColor(4);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_Graph01001 = new TH1F("Graph_Graph01001","",100,0,2842864);
   Graph_Graph01001->SetMinimum(1491.93);
   Graph_Graph01001->SetMaximum(2829886);
   Graph_Graph01001->SetDirectory(nullptr);
   Graph_Graph01001->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph01001->SetLineColor(ci);
   Graph_Graph01001->GetXaxis()->SetTitle("Total PE [p.e.]");
   Graph_Graph01001->GetXaxis()->CenterTitle(true);
   Graph_Graph01001->GetXaxis()->SetLabelFont(42);
   Graph_Graph01001->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph01001->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph01001->GetXaxis()->SetTitleOffset(0.83);
   Graph_Graph01001->GetXaxis()->SetTitleFont(22);
   Graph_Graph01001->GetYaxis()->SetTitle("Reconstructed total PE [p.e.]");
   Graph_Graph01001->GetYaxis()->CenterTitle(true);
   Graph_Graph01001->GetYaxis()->SetLabelFont(42);
   Graph_Graph01001->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph01001->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph01001->GetYaxis()->SetTitleOffset(0.83);
   Graph_Graph01001->GetYaxis()->SetTitleFont(22);
   Graph_Graph01001->GetZaxis()->SetLabelFont(42);
   Graph_Graph01001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph01001);
   
   gre->Draw("a p");
   
   Double_t Graph1_fx1002[21] = { 1665.62, 3398.81, 5137.27, 6875.939, 8614.392, 10352.31, 12089.79, 13825.59, 15561.97, 17230.34, 34456.26, 51695.18, 86144.41, 172309, 258457, 344602.3, 516901.7,
   861482.2, 1202228, 1722972, 2584527 };
   Double_t Graph1_fy1002[21] = { 1663.679, 3394.752, 5130.627, 6866.365, 8601.543, 10335.84, 12069.36, 13800.86, 15532.59, 17196.17, 34354.06, 51491, 85634.83, 170442.6, 254387, 337481.9, 501139.3,
   818274.2, 1124647, 1551862, 2200804 };
   Double_t Graph1_fex1002[21] = { 0.1344816, 0.2993075, 0.3833868, 0.4591254, 0.5291433, 0.591348, 0.6479977, 0.7018099, 0.7584837, 0.7415149, 2.371028, 2.88041, 3.752953, 7.578463, 9.134584, 10.46266, 12.80681,
   21.448, 25.885, 38.26152, 46.48611 };
   Double_t Graph1_fey1002[21] = { 0.08152383, 0.2603628, 0.3200451, 0.3701911, 0.414265, 0.4540514, 0.4905874, 0.5245512, 0.5563916, 0.5853146, 1.84713, 2.258003, 2.903313, 5.749481, 6.969399, 7.966621, 9.556763,
   15.26515, 17.27736, 23.55272, 25.3185 };
   gre = new TGraphErrors(21,Graph1_fx1002,Graph1_fy1002,Graph1_fex1002,Graph1_fey1002);
   gre->SetName("Graph1");
   gre->SetTitle("Graph");
   gre->SetFillStyle(3002);
   gre->SetLineColor(2);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(21);
   
   TH1F *Graph_Graph11002 = new TH1F("Graph_Graph11002","Graph",100,0,2842864);
   Graph_Graph11002->SetMinimum(1497.238);
   Graph_Graph11002->SetMaximum(2420746);
   Graph_Graph11002->SetDirectory(nullptr);
   Graph_Graph11002->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph11002->SetLineColor(ci);
   Graph_Graph11002->GetXaxis()->SetLabelFont(42);
   Graph_Graph11002->GetXaxis()->SetTitleOffset(1);
   Graph_Graph11002->GetXaxis()->SetTitleFont(42);
   Graph_Graph11002->GetYaxis()->SetLabelFont(42);
   Graph_Graph11002->GetYaxis()->SetTitleFont(42);
   Graph_Graph11002->GetZaxis()->SetLabelFont(42);
   Graph_Graph11002->GetZaxis()->SetTitleOffset(1);
   Graph_Graph11002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph11002);
   
   gre->Draw("p");
   
   TF1 *Below1PE1 = new TF1("Below1PE","x",0,3000000, TF1::EAddToList::kDefault);
   Below1PE1->SetFillColor(19);
   Below1PE1->SetFillStyle(0);
   Below1PE1->SetLineStyle(2);
   Below1PE1->SetLineWidth(1);
   Below1PE1->GetXaxis()->SetLabelFont(42);
   Below1PE1->GetXaxis()->SetTitleOffset(1);
   Below1PE1->GetXaxis()->SetTitleFont(42);
   Below1PE1->GetYaxis()->SetLabelFont(42);
   Below1PE1->GetYaxis()->SetTitleFont(42);
   Below1PE1->Draw("same");
   
   TLegend *leg = new TLegend(0.4684814,0.02990608,0.9226361,0.2801532,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(72);
   leg->SetTextSize(0.08341572);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("Graph0","After calibration","lpe");
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(4);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(72);
   entry=leg->AddEntry("Graph1","Before calibration","lpe");
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(2);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(72);
   leg->Draw();
   pad1__0->Modified();
   CalibToTrue_Canvas->cd();
  
// ------------>Primitives in pad: pad1
   TPad *pad1__1 = new TPad("pad1", "pad1",0,0,1,0.32);
   pad1__1->Draw();
   pad1__1->cd();
   pad1__1->Range(-437363.8,0.9805714,3207334,1.012);
   pad1__1->SetFillColor(0);
   pad1__1->SetBorderMode(0);
   pad1__1->SetBorderSize(2);
   pad1__1->SetLeftMargin(0.12);
   pad1__1->SetTopMargin(0);
   pad1__1->SetBottomMargin(0.3);
   pad1__1->SetFrameBorderMode(0);
   pad1__1->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1003[21] = { 1665.62, 3398.81, 5137.27, 6875.939, 8614.392, 10352.31, 12089.79, 13825.59, 15561.97, 17230.34, 34456.26, 51695.18, 86144.41, 172309, 258457, 344602.3, 516901.7,
   861482.2, 1206131, 1722972, 2584527 };
   Double_t Graph0_fy1003[21] = { 0.9954968, 0.9984518, 0.9982946, 0.9990159, 1.000105, 0.9995972, 1.000018, 1.000214, 1.000174, 1.000661, 1.00026, 1.000389, 1.00073, 1.000269, 1.000451, 1.000143, 0.9996247,
   0.9994777, 0.9984955, 0.9971431, 0.9953647 };
   Double_t Graph0_fex1003[21] = { 0.1344816, 0.2993075, 0.3833868, 0.4591254, 0.5291433, 0.591348, 0.6479977, 0.7018099, 0.7584837, 0.7415149, 2.371028, 2.88041, 3.752953, 7.578463, 9.134584, 10.46266, 12.80681,
   21.448, 25.885, 38.26152, 46.48611 };
   Double_t Graph0_fey1003[21] = { 0.0002643586, 0.0003748224, 0.0003018952, 0.0002611902, 0.0002343369, 0.0002148013, 0.0002001035, 0.0001882656, 0.0001787576, 0.0001700557, 0.0002546298, 0.0002107772, 0.0001682912, 0.0001680514, 0.0001419304, 0.0001270038, 0.0001100649,
   0.0001100761, 9.915107e-05, 0.0001003513, 8.980152e-05 };
   gre = new TGraphErrors(21,Graph0_fx1003,Graph0_fy1003,Graph0_fex1003,Graph0_fey1003);
   gre->SetName("Graph0");
   gre->SetTitle("");
   gre->SetFillStyle(3002);
   gre->SetLineColor(4);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_Graph01003 = new TH1F("Graph_Graph01003","",100,0,2842864);
   Graph_Graph01003->SetMinimum(0.99);
   Graph_Graph01003->SetMaximum(1.012);
   Graph_Graph01003->SetDirectory(nullptr);
   Graph_Graph01003->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph01003->SetLineColor(ci);
   Graph_Graph01003->GetXaxis()->SetTitle("Total PE [p.e.]");
   Graph_Graph01003->GetXaxis()->CenterTitle(true);
   Graph_Graph01003->GetXaxis()->SetLabelFont(42);
   Graph_Graph01003->GetXaxis()->SetLabelSize(0.12);
   Graph_Graph01003->GetXaxis()->SetTitleSize(0.17);
   Graph_Graph01003->GetXaxis()->SetTitleOffset(0.77);
   Graph_Graph01003->GetXaxis()->SetTitleFont(22);
   Graph_Graph01003->GetYaxis()->SetTitle("Ratio");
   Graph_Graph01003->GetYaxis()->SetRange(0,1);
   Graph_Graph01003->GetYaxis()->CenterTitle(true);
   Graph_Graph01003->GetYaxis()->SetNdivisions(505);
   Graph_Graph01003->GetYaxis()->SetLabelFont(42);
   Graph_Graph01003->GetYaxis()->SetLabelSize(0.12);
   Graph_Graph01003->GetYaxis()->SetTitleSize(0.15);
   Graph_Graph01003->GetYaxis()->SetTitleOffset(0.35);
   Graph_Graph01003->GetYaxis()->SetTitleFont(22);
   Graph_Graph01003->GetZaxis()->SetLabelFont(42);
   Graph_Graph01003->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph01003);
   
   gre->Draw("a p");
   pad1__1->Modified();
   CalibToTrue_Canvas->cd();
   CalibToTrue_Canvas->Modified();
   CalibToTrue_Canvas->SetSelected(CalibToTrue_Canvas);
}
