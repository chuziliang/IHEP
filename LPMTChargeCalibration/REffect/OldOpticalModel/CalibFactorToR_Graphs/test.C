#ifdef __CLING__
#pragma cling optimize(0)
#endif
void test()
{
//=========Macro generated from canvas: c1/c1
//=========  (Sun Jun 22 23:20:11 2025) by ROOT version 6.30/08
   TCanvas *c1 = new TCanvas("c1", "c1",599,158,800,600);
   c1->Range(-2.992152,43.27945,37.40594,45.34498);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLeftMargin(0.1332378);
   c1->SetRightMargin(0.06590258);
   c1->SetTopMargin(0.06932773);
   c1->SetBottomMargin(0.1302521);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TMultiGraph *multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("");
   
   Double_t CalibFactorToR_fx1001[30] = { 7.3643, 12.2284, 14.55261, 16.28582, 17.70701, 18.92851, 20.00887, 20.98312, 21.8741, 22.69765, 23.46527, 24.18561, 24.86535, 25.50977, 26.12314, 26.70896, 27.27012,
   27.80907, 28.32789, 28.82836, 29.31202, 29.78021, 30.23412, 30.67479, 31.10315, 31.52001, 31.92613, 32.32216, 32.70872, 33.08635 };
   Double_t CalibFactorToR_fy1001[30] = { 43.78514, 43.80907, 43.77199, 43.7093, 43.70025, 43.72462, 43.71987, 43.72651, 43.74182, 43.72116, 43.74766, 43.73511, 43.68194, 43.67943, 43.66626, 43.67703, 43.72746,
   43.74679, 43.66174, 43.6722, 43.65303, 43.68638, 43.75102, 43.77711, 43.77747, 43.74919, 43.71403, 43.795, 43.85302, 43.82009 };
   Double_t CalibFactorToR_fex1001[30] = { 3.5033, 1.360804, 0.9634038, 0.7698064, 0.651385, 0.5701118, 0.5102521, 0.463995, 0.4269812, 0.3965686, 0.3710538, 0.3492851, 0.330453, 0.3139708, 0.2994018, 0.2864136, 0.2747486,
   0.2642033, 0.254615, 0.2458521, 0.2378063, 0.2303881, 0.2235227, 0.217147, 0.2112073, 0.2056579, 0.2004592, 0.1955771, 0.1909819, 0.1866474 };
   Double_t CalibFactorToR_fey1001[30] = { 0.03087729, 0.01252411, 0.01319911, 0.01447768, 0.01672408, 0.01841215, 0.01954114, 0.02030669, 0.02070452, 0.02108453, 0.02210311, 0.02318176, 0.02393608, 0.02510292, 0.02568399, 0.02653803, 0.02720588,
   0.02746359, 0.02830942, 0.02885768, 0.02939103, 0.0302726, 0.0309693, 0.0315744, 0.0322511, 0.03334861, 0.03447815, 0.03491183, 0.03506004, 0.03526791 };
   TGraphErrors *gre = new TGraphErrors(30,CalibFactorToR_fx1001,CalibFactorToR_fy1001,CalibFactorToR_fex1001,CalibFactorToR_fey1001);
   gre->SetName("CalibFactorToR");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#686789");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#686789");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_CalibFactorToR1001 = new TH1F("Graph_CalibFactorToR1001","",100,0.9198,36.2142);
   Graph_CalibFactorToR1001->SetMinimum(43.5972);
   Graph_CalibFactorToR1001->SetMaximum(43.91453);
   Graph_CalibFactorToR1001->SetDirectory(nullptr);
   Graph_CalibFactorToR1001->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_CalibFactorToR1001->SetLineColor(ci);
   Graph_CalibFactorToR1001->GetXaxis()->SetTitle("R");
   Graph_CalibFactorToR1001->GetXaxis()->CenterTitle(true);
   Graph_CalibFactorToR1001->GetXaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1001->GetXaxis()->SetLabelSize(0.06);
   Graph_CalibFactorToR1001->GetXaxis()->SetTitleSize(0.06);
   Graph_CalibFactorToR1001->GetXaxis()->SetTitleOffset(1.2);
   Graph_CalibFactorToR1001->GetXaxis()->SetTitleFont(22);
   Graph_CalibFactorToR1001->GetYaxis()->SetTitle("CalibFactor");
   Graph_CalibFactorToR1001->GetYaxis()->CenterTitle(true);
   Graph_CalibFactorToR1001->GetYaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1001->GetYaxis()->SetLabelSize(0.06);
   Graph_CalibFactorToR1001->GetYaxis()->SetTitleSize(0.06);
   Graph_CalibFactorToR1001->GetYaxis()->SetTitleOffset(1.3);
   Graph_CalibFactorToR1001->GetYaxis()->SetTitleFont(22);
   Graph_CalibFactorToR1001->GetZaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1001->GetZaxis()->SetTitleOffset(1);
   Graph_CalibFactorToR1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_CalibFactorToR1001);
   
   multigraph->Add(gre,"P");
   
   Double_t CalibFactorToR_fx1002[30] = { 7.3643, 12.2284, 14.55261, 16.28582, 17.70701, 18.92851, 20.00887, 20.98312, 21.8741, 22.69765, 23.46527, 24.18561, 24.86535, 25.50977, 26.12314, 26.70896, 27.27012,
   27.80907, 28.32789, 28.82836, 29.31202, 29.78021, 30.23412, 30.67479, 31.10315, 31.52001, 31.92613, 32.32216, 32.70872, 33.08635 };
   Double_t CalibFactorToR_fy1002[30] = { 44.42622, 45.12353, 44.98066, 44.6837, 44.41615, 44.19595, 44.05585, 43.9703, 43.91257, 43.87937, 43.8784, 43.88289, 43.87688, 43.89956, 43.95608, 43.99088, 43.98696,
   43.96894, 43.98649, 43.96798, 43.94715, 43.97076, 43.98239, 43.94497, 43.9852, 43.9772, 43.97003, 44.00907, 44.01169, 43.9326 };
   Double_t CalibFactorToR_fex1002[30] = { 3.5033, 1.360804, 0.9634038, 0.7698064, 0.651385, 0.5701118, 0.5102521, 0.463995, 0.4269812, 0.3965686, 0.3710538, 0.3492851, 0.330453, 0.3139708, 0.2994018, 0.2864136, 0.2747486,
   0.2642033, 0.254615, 0.2458521, 0.2378063, 0.2303881, 0.2235227, 0.217147, 0.2112073, 0.2056579, 0.2004592, 0.1955771, 0.1909819, 0.1866474 };
   Double_t CalibFactorToR_fey1002[30] = { 0.006248766, 0.003104972, 0.003265113, 0.003544545, 0.003858578, 0.004183605, 0.00451649, 0.004858806, 0.005204405, 0.005557248, 0.005932303, 0.006324629, 0.006718486, 0.007127322, 0.007553357, 0.008000876, 0.008472571,
   0.008959611, 0.009509377, 0.01008545, 0.01068882, 0.01141254, 0.01218863, 0.01301979, 0.01401104, 0.01508186, 0.01633958, 0.0177652, 0.01947479, 0.02155115 };
   gre = new TGraphErrors(30,CalibFactorToR_fx1002,CalibFactorToR_fy1002,CalibFactorToR_fex1002,CalibFactorToR_fey1002);
   gre->SetName("CalibFactorToR");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   ci = TColor::GetColor("#8a95a9");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#8a95a9");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_CalibFactorToR1002 = new TH1F("Graph_CalibFactorToR1002","",100,0.9198,36.2142);
   Graph_CalibFactorToR1002->SetMinimum(43.74451);
   Graph_CalibFactorToR1002->SetMaximum(45.25228);
   Graph_CalibFactorToR1002->SetDirectory(nullptr);
   Graph_CalibFactorToR1002->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_CalibFactorToR1002->SetLineColor(ci);
   Graph_CalibFactorToR1002->GetXaxis()->SetTitle("R");
   Graph_CalibFactorToR1002->GetXaxis()->CenterTitle(true);
   Graph_CalibFactorToR1002->GetXaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1002->GetXaxis()->SetLabelSize(0.06);
   Graph_CalibFactorToR1002->GetXaxis()->SetTitleSize(0.06);
   Graph_CalibFactorToR1002->GetXaxis()->SetTitleOffset(1.2);
   Graph_CalibFactorToR1002->GetXaxis()->SetTitleFont(22);
   Graph_CalibFactorToR1002->GetYaxis()->SetTitle("CalibFactor");
   Graph_CalibFactorToR1002->GetYaxis()->CenterTitle(true);
   Graph_CalibFactorToR1002->GetYaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1002->GetYaxis()->SetLabelSize(0.06);
   Graph_CalibFactorToR1002->GetYaxis()->SetTitleSize(0.06);
   Graph_CalibFactorToR1002->GetYaxis()->SetTitleOffset(1.3);
   Graph_CalibFactorToR1002->GetYaxis()->SetTitleFont(22);
   Graph_CalibFactorToR1002->GetZaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1002->GetZaxis()->SetTitleOffset(1);
   Graph_CalibFactorToR1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_CalibFactorToR1002);
   
   multigraph->Add(gre,"P");
   
   Double_t CalibFactorToR_fx1003[30] = { 7.3643, 12.2284, 14.55261, 16.28582, 17.70701, 18.92851, 20.00887, 20.98312, 21.8741, 22.69765, 23.46527, 24.18561, 24.86535, 25.50977, 26.12314, 26.70896, 27.27012,
   27.80907, 28.32789, 28.82836, 29.31202, 29.78021, 30.23412, 30.67479, 31.10315, 31.52001, 31.92613, 32.32216, 32.70872, 33.08635 };
   Double_t CalibFactorToR_fy1003[30] = { 43.97301, 43.98836, 43.93198, 43.87179, 43.85587, 43.85525, 43.86927, 43.86656, 43.89323, 43.85606, 43.90367, 43.87547, 43.79558, 43.80779, 43.75719, 43.82327, 43.88161,
   43.85603, 43.73855, 43.76559, 43.71148, 43.79438, 43.88874, 43.94925, 43.89302, 43.81091, 43.8422, 43.88962, 43.94079, 43.89735 };
   Double_t CalibFactorToR_fex1003[30] = { 3.5033, 1.360804, 0.9634038, 0.7698064, 0.651385, 0.5701118, 0.5102521, 0.463995, 0.4269812, 0.3965686, 0.3710538, 0.3492851, 0.330453, 0.3139708, 0.2994018, 0.2864136, 0.2747486,
   0.2642033, 0.254615, 0.2458521, 0.2378063, 0.2303881, 0.2235227, 0.217147, 0.2112073, 0.2056579, 0.2004592, 0.1955771, 0.1909819, 0.1866474 };
   Double_t CalibFactorToR_fey1003[30] = { 0.05399722, 0.01451139, 0.01540759, 0.0170201, 0.01982512, 0.02187463, 0.02333765, 0.02434475, 0.02492702, 0.02535035, 0.02662803, 0.02802244, 0.02894264, 0.03039358, 0.03112838, 0.03227411, 0.03311402,
   0.0333591, 0.03443568, 0.03529947, 0.03585156, 0.03696599, 0.03793026, 0.03873876, 0.03952367, 0.04076172, 0.04234469, 0.04283085, 0.04304607, 0.04326698 };
   gre = new TGraphErrors(30,CalibFactorToR_fx1003,CalibFactorToR_fy1003,CalibFactorToR_fex1003,CalibFactorToR_fey1003);
   gre->SetName("CalibFactorToR");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   ci = TColor::GetColor("#9900cc");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#9900cc");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(20);
   
   TH1F *Graph_CalibFactorToR1003 = new TH1F("Graph_CalibFactorToR1003","",100,0.9198,36.2142);
   Graph_CalibFactorToR1003->SetMinimum(43.64049);
   Graph_CalibFactorToR1003->SetMaximum(44.06215);
   Graph_CalibFactorToR1003->SetDirectory(nullptr);
   Graph_CalibFactorToR1003->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_CalibFactorToR1003->SetLineColor(ci);
   Graph_CalibFactorToR1003->GetXaxis()->SetTitle("R");
   Graph_CalibFactorToR1003->GetXaxis()->CenterTitle(true);
   Graph_CalibFactorToR1003->GetXaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1003->GetXaxis()->SetLabelSize(0.06);
   Graph_CalibFactorToR1003->GetXaxis()->SetTitleSize(0.06);
   Graph_CalibFactorToR1003->GetXaxis()->SetTitleOffset(1.2);
   Graph_CalibFactorToR1003->GetXaxis()->SetTitleFont(22);
   Graph_CalibFactorToR1003->GetYaxis()->SetTitle("CalibFactor");
   Graph_CalibFactorToR1003->GetYaxis()->CenterTitle(true);
   Graph_CalibFactorToR1003->GetYaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1003->GetYaxis()->SetLabelSize(0.06);
   Graph_CalibFactorToR1003->GetYaxis()->SetTitleSize(0.06);
   Graph_CalibFactorToR1003->GetYaxis()->SetTitleOffset(1.3);
   Graph_CalibFactorToR1003->GetYaxis()->SetTitleFont(22);
   Graph_CalibFactorToR1003->GetZaxis()->SetLabelFont(42);
   Graph_CalibFactorToR1003->GetZaxis()->SetTitleOffset(1);
   Graph_CalibFactorToR1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_CalibFactorToR1003);
   
   multigraph->Add(gre,"P");
   multigraph->Draw("A P");
   multigraph->GetXaxis()->SetLimits(2.3904, 34.7436);
   multigraph->GetXaxis()->SetTitle("R [m]");
   multigraph->GetXaxis()->CenterTitle(true);
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetLabelSize(0.06);
   multigraph->GetXaxis()->SetTitleSize(0.06);
   multigraph->GetXaxis()->SetTitleOffset(1);
   multigraph->GetXaxis()->SetTitleFont(22);
   multigraph->GetYaxis()->SetTitle("Calibration factor");
   multigraph->GetYaxis()->CenterTitle(true);
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelSize(0.06);
   multigraph->GetYaxis()->SetTitleSize(0.06);
   multigraph->GetYaxis()->SetTitleOffset(1.1);
   multigraph->GetYaxis()->SetTitleFont(22);
   
   TLegend *leg = new TLegend(0.5429799,0.7016807,0.9613181,0.9054622,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextFont(22);
   leg->SetTextSize(0.05252101);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("CalibFactorToR","#theta < 10 deg","pe");

   ci = TColor::GetColor("#686789");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);

   ci = TColor::GetColor("#686789");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(22);
   entry=leg->AddEntry("CalibFactorToR","No selection","pe");

   ci = TColor::GetColor("#8a95a9");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);

   ci = TColor::GetColor("#8a95a9");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(22);
   entry=leg->AddEntry("CalibFactorToR","This selection","pe");

   ci = TColor::GetColor("#9900cc");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);

   ci = TColor::GetColor("#9900cc");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry->SetTextFont(22);
   leg->Draw();
   
   leg = new TLegend(0.5238095,0.6111111,0.9423559,0.6614583,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextFont(22);
   leg->SetTextSize(0.05252101);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   entry=leg->AddEntry("TLine","#splitline{Detector center}{w/o supporting bars}","l");
   entry->SetLineColor(1);
   entry->SetLineStyle(2);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(22);
   leg->Draw();
   TLine *line = new TLine(4,43.918,34,43.918);
   line->SetLineStyle(2);
   line->SetLineWidth(3);
   line->Draw();
   c1->Modified();
   c1->SetSelected(c1);
}
