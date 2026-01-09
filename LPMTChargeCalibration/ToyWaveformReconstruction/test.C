#ifdef __CLING__
#pragma cling optimize(0)
#endif
void test()
{
//=========Macro generated from canvas: NonlinearityLPMTtoMu_Canvas/NonlinearityLPMTtoMu_Canvas
//=========  (Mon Jun 23 23:52:45 2025) by ROOT version 6.32.02
   TCanvas *NonlinearityLPMTtoMu_Canvas = new TCanvas("NonlinearityLPMTtoMu_Canvas", "NonlinearityLPMTtoMu_Canvas",60,83,800,600);
   gStyle->SetOptFit(1);
   NonlinearityLPMTtoMu_Canvas->Range(0,0,1,1);
   NonlinearityLPMTtoMu_Canvas->SetFillColor(0);
   NonlinearityLPMTtoMu_Canvas->SetBorderMode(0);
   NonlinearityLPMTtoMu_Canvas->SetBorderSize(2);
   NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
  
// ------------>Primitives in pad: pad1
   TPad *pad1__0 = new TPad("pad1", "pad1",0,0.4,1,0.98);
   pad1__0->Draw();
   pad1__0->cd();
   pad1__0->Range(-2.490888,-3.5,1.599843,1.5);
   pad1__0->SetFillColor(0);
   pad1__0->SetBorderMode(0);
   pad1__0->SetBorderSize(2);
   pad1__0->SetLogx();
   pad1__0->SetLeftMargin(0.12);
   pad1__0->SetRightMargin(0.03);
   pad1__0->SetTopMargin(0);
   pad1__0->SetBottomMargin(0);
   pad1__0->SetFrameBorderMode(0);
   pad1__0->SetFrameBorderMode(0);
   
   TMultiGraph *multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("");
   
   Double_t Graph_fx1001[20] = { 0.07088048, 0.1348355, 0.1958755, 0.2699821, 0.3521812, 0.4373954, 0.5242529, 0.6129014, 0.7005019, 1.072158, 2.042972, 2.991336, 3.931701, 4.8585, 5.797622, 6.721088, 7.632185,
   8.514893, 12.04559, 20.71303 };
   Double_t Graph_fy1001[20] = { -1.023431, -0.1743162, -0.5631124, -0.5298687, -0.7439471, -0.6473128, -0.3250661, -1.641475, 0.7745201, -0.8602367, -0.8076355, -0.9035305, -0.2984079, -0.01272878, -1.558249, -1.703109, -1.213485,
   -1.936234, -1.454611, -2.697722 };
   Double_t Graph_fex1001[20] = { 0.0001215959, 0.0001553578, 0.0002039987, 0.0003001669, 0.0004389562, 0.0006081787, 0.0007845623, 0.0009865142, 0.001200482, 0.0007787596, 0.002063056, 0.003747125, 0.005760972, 0.008049813, 0.01052353, 0.01329151, 0.01633286,
   0.01899218, 0.01392933, 0.05499344 };
   Double_t Graph_fey1001[20] = { 0.7146858, 0.4594135, 0.4030969, 0.4241648, 0.4683752, 0.5197788, 0.5596602, 0.5880204, 0.6471497, 0.2653689, 0.368179, 0.455743, 0.538568, 0.6111587, 0.6561362, 0.7079057, 0.7696615,
   0.7997243, 0.3560165, 0.830977 };
   TGraphErrors *gre = new TGraphErrors(20,Graph_fx1001,Graph_fy1001,Graph_fex1001,Graph_fey1001);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillStyle(3002);
   gre->SetLineColor(4);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_Graph1001 = new TH1F("Graph_Graph1001","",100,0.063683,22.83774);
   Graph_Graph1001->SetMinimum(-4.023736);
   Graph_Graph1001->SetMaximum(1.916707);
   Graph_Graph1001->SetDirectory(nullptr);
   Graph_Graph1001->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1001->SetLineColor(ci);
   Graph_Graph1001->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   Graph_Graph1001->GetXaxis()->CenterTitle(true);
   Graph_Graph1001->GetXaxis()->SetLabelFont(42);
   Graph_Graph1001->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph1001->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1001->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph1001->GetXaxis()->SetTitleFont(22);
   Graph_Graph1001->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
   Graph_Graph1001->GetYaxis()->CenterTitle(true);
   Graph_Graph1001->GetYaxis()->SetLabelFont(42);
   Graph_Graph1001->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph1001->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1001->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph1001->GetYaxis()->SetTitleFont(22);
   Graph_Graph1001->GetZaxis()->SetLabelFont(42);
   Graph_Graph1001->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1001);
   
   multigraph->Add(gre,"P");
   
   Double_t Graph_fx1002[15] = { 0.03975341, 0.08140691, 0.1679644, 0.255377, 0.3429123, 0.4307091, 0.5197644, 0.6082419, 0.6970558, 0.7870613, 1.162305, 2.102184, 2.978726, 3.836917, 4.539167 };
   Double_t Graph_fy1002[15] = { -0.1488095, -0.2806611, -0.04511747, 0.09022387, 0.1092004, -0.1885706, 0.1485731, 0.1037587, -0.4849706, 0.2534457, -0.4048871, -0.9029992, -0.4632347, -0.7519558, -0.9144714 };
   Double_t Graph_fex1002[15] = { 1.524814e-05, 2.31173e-05, 6.53661e-05, 0.0001225934, 0.0001926619, 0.0002767631, 0.0003711991, 0.0004775735, 0.0005933013, 0.0007205816, 0.0004488116, 0.001076178, 0.001683046, 0.002504983, 0.005571253 };
   Double_t Graph_fey1002[15] = { 0.1871274, 0.1312368, 0.1630044, 0.1933178, 0.2214885, 0.2487121, 0.2755541, 0.3008649, 0.3219864, 0.348836, 0.1515778, 0.1933524, 0.2116151, 0.2413298, 0.4570522 };
   gre = new TGraphErrors(15,Graph_fx1002,Graph_fy1002,Graph_fex1002,Graph_fey1002);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   ci = TColor::GetColor("#ff6400");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#ff6400");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_Graph1002 = new TH1F("Graph_Graph1002","",100,0.03576434,4.995238);
   Graph_Graph1002->SetMinimum(-1.568904);
   Graph_Graph1002->SetMaximum(0.7996622);
   Graph_Graph1002->SetDirectory(nullptr);
   Graph_Graph1002->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1002->SetLineColor(ci);
   Graph_Graph1002->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   Graph_Graph1002->GetXaxis()->CenterTitle(true);
   Graph_Graph1002->GetXaxis()->SetLabelFont(42);
   Graph_Graph1002->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph1002->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1002->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph1002->GetXaxis()->SetTitleFont(22);
   Graph_Graph1002->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
   Graph_Graph1002->GetYaxis()->CenterTitle(true);
   Graph_Graph1002->GetYaxis()->SetLabelFont(42);
   Graph_Graph1002->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph1002->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1002->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph1002->GetYaxis()->SetTitleFont(22);
   Graph_Graph1002->GetZaxis()->SetLabelFont(42);
   Graph_Graph1002->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1002);
   
   multigraph->Add(gre,"P");
   
   Double_t Graph_fx1003[21] = { 0.03971208, 0.08143538, 0.1638447, 0.2424291, 0.3250218, 0.4113833, 0.500047, 0.5878772, 0.6767072, 0.7659826, 1.141049, 2.089772, 2.980865, 3.852311, 4.649046, 5.797622, 6.721088,
   7.632185, 8.514893, 12.04559, 20.71303 };
   Double_t Graph_fy1003[21] = { -0.1756669, -0.287316, -0.04415081, 0.01049818, 0.02183424, -0.2678012, 0.01743912, 0.04763613, -0.7081485, 0.4022504, -0.4829782, -0.8819107, -0.5351899, -0.6699717, -0.6130813, -1.558249, -1.703109,
   -1.213485, -1.936234, -1.454611, -2.697722 };
   Double_t Graph_fex1003[21] = { 1.518915e-05, 2.284778e-05, 6.043204e-05, 0.0001058218, 0.0001632284, 0.0002352842, 0.0003180585, 0.000409193, 0.000509662, 0.0006191065, 0.0003892837, 0.0009543416, 0.001535312, 0.002297671, 0.004596684, 0.01052353, 0.01329151,
   0.01633286, 0.01899218, 0.01392933, 0.05499344 };
   Double_t Graph_fey1003[21] = { 0.186599, 0.1299564, 0.1557353, 0.1772571, 0.1992881, 0.2223631, 0.2459248, 0.2673261, 0.2846185, 0.3091263, 0.1361871, 0.1744038, 0.1944229, 0.2223378, 0.3681193, 0.6561362, 0.7079057,
   0.7696615, 0.7997243, 0.3560165, 0.830977 };
   gre = new TGraphErrors(21,Graph_fx1003,Graph_fy1003,Graph_fex1003,Graph_fey1003);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   ci = TColor::GetColor("#9400d3");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#9400d3");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(22);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_Graph1003 = new TH1F("Graph_Graph1003","",100,0.0357272,22.84085);
   Graph_Graph1003->SetMinimum(-3.952707);
   Graph_Graph1003->SetMaximum(1.135384);
   Graph_Graph1003->SetDirectory(nullptr);
   Graph_Graph1003->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1003->SetLineColor(ci);
   Graph_Graph1003->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   Graph_Graph1003->GetXaxis()->CenterTitle(true);
   Graph_Graph1003->GetXaxis()->SetLabelFont(42);
   Graph_Graph1003->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph1003->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1003->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph1003->GetXaxis()->SetTitleFont(22);
   Graph_Graph1003->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
   Graph_Graph1003->GetYaxis()->CenterTitle(true);
   Graph_Graph1003->GetYaxis()->SetLabelFont(42);
   Graph_Graph1003->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph1003->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1003->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph1003->GetYaxis()->SetTitleFont(22);
   Graph_Graph1003->GetZaxis()->SetLabelFont(42);
   Graph_Graph1003->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1003);
   
   multigraph->Add(gre,"P");
   multigraph->Draw("A P");
   multigraph->GetXaxis()->SetLimits(0.01, 30);
   multigraph->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetTitleOffset(1);
   multigraph->GetXaxis()->SetTitleFont(42);
   multigraph->GetYaxis()->SetTitle("1 - #alpha^{CH}_{L}(#mu_{L}) [%]");
   multigraph->GetYaxis()->CenterTitle(true);
   multigraph->GetYaxis()->SetNdivisions(505);
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelSize(0.08);
   multigraph->GetYaxis()->SetTitleSize(0.1);
   multigraph->GetYaxis()->SetTitleOffset(0.37);
   multigraph->GetYaxis()->SetTitleFont(22);
   
   TF1 *SetNonlinearity1 = new TF1("*SetNonlinearity",0.01,130,1);
    //The original function : SetNonlinearity had originally been created by:
    //TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearity,0.01,130,1, 1, TF1::EAddToList::kDefault);
   SetNonlinearity1->SetRange(0.01,130);
   SetNonlinearity1->SetName("SetNonlinearity");
   SetNonlinearity1->SetTitle("SetNonlinearity");
   SetNonlinearity1->SetSavedPoint(0,-0.00100001);
   SetNonlinearity1->SetSavedPoint(1,-0.131162);
   SetNonlinearity1->SetSavedPoint(2,-0.2616647);
   SetNonlinearity1->SetSavedPoint(3,-0.3925106);
   SetNonlinearity1->SetSavedPoint(4,-0.5237026);
   SetNonlinearity1->SetSavedPoint(5,-0.6552434);
   SetNonlinearity1->SetSavedPoint(6,-0.7871358);
   SetNonlinearity1->SetSavedPoint(7,-0.9193826);
   SetNonlinearity1->SetSavedPoint(8,-1.051987);
   SetNonlinearity1->SetSavedPoint(9,-1.184951);
   SetNonlinearity1->SetSavedPoint(10,-1.318279);
   SetNonlinearity1->SetSavedPoint(11,-1.451972);
   SetNonlinearity1->SetSavedPoint(12,-1.586035);
   SetNonlinearity1->SetSavedPoint(13,-1.72047);
   SetNonlinearity1->SetSavedPoint(14,-1.855281);
   SetNonlinearity1->SetSavedPoint(15,-1.99047);
   SetNonlinearity1->SetSavedPoint(16,-2.12604);
   SetNonlinearity1->SetSavedPoint(17,-2.261996);
   SetNonlinearity1->SetSavedPoint(18,-2.39834);
   SetNonlinearity1->SetSavedPoint(19,-2.535076);
   SetNonlinearity1->SetSavedPoint(20,-2.672207);
   SetNonlinearity1->SetSavedPoint(21,-2.809736);
   SetNonlinearity1->SetSavedPoint(22,-2.947667);
   SetNonlinearity1->SetSavedPoint(23,-3.086004);
   SetNonlinearity1->SetSavedPoint(24,-3.22475);
   SetNonlinearity1->SetSavedPoint(25,-3.363909);
   SetNonlinearity1->SetSavedPoint(26,-3.503484);
   SetNonlinearity1->SetSavedPoint(27,-3.643479);
   SetNonlinearity1->SetSavedPoint(28,-3.783899);
   SetNonlinearity1->SetSavedPoint(29,-3.924746);
   SetNonlinearity1->SetSavedPoint(30,-4.066026);
   SetNonlinearity1->SetSavedPoint(31,-4.207741);
   SetNonlinearity1->SetSavedPoint(32,-4.349896);
   SetNonlinearity1->SetSavedPoint(33,-4.492495);
   SetNonlinearity1->SetSavedPoint(34,-4.635543);
   SetNonlinearity1->SetSavedPoint(35,-4.779042);
   SetNonlinearity1->SetSavedPoint(36,-4.922999);
   SetNonlinearity1->SetSavedPoint(37,-5.067417);
   SetNonlinearity1->SetSavedPoint(38,-5.212301);
   SetNonlinearity1->SetSavedPoint(39,-5.357655);
   SetNonlinearity1->SetSavedPoint(40,-5.503483);
   SetNonlinearity1->SetSavedPoint(41,-5.649791);
   SetNonlinearity1->SetSavedPoint(42,-5.796584);
   SetNonlinearity1->SetSavedPoint(43,-5.943865);
   SetNonlinearity1->SetSavedPoint(44,-6.091641);
   SetNonlinearity1->SetSavedPoint(45,-6.239915);
   SetNonlinearity1->SetSavedPoint(46,-6.388694);
   SetNonlinearity1->SetSavedPoint(47,-6.537982);
   SetNonlinearity1->SetSavedPoint(48,-6.687785);
   SetNonlinearity1->SetSavedPoint(49,-6.838107);
   SetNonlinearity1->SetSavedPoint(50,-6.988955);
   SetNonlinearity1->SetSavedPoint(51,-7.140334);
   SetNonlinearity1->SetSavedPoint(52,-7.292249);
   SetNonlinearity1->SetSavedPoint(53,-7.444707);
   SetNonlinearity1->SetSavedPoint(54,-7.597712);
   SetNonlinearity1->SetSavedPoint(55,-7.751272);
   SetNonlinearity1->SetSavedPoint(56,-7.905392);
   SetNonlinearity1->SetSavedPoint(57,-8.060079);
   SetNonlinearity1->SetSavedPoint(58,-8.215338);
   SetNonlinearity1->SetSavedPoint(59,-8.371176);
   SetNonlinearity1->SetSavedPoint(60,-8.5276);
   SetNonlinearity1->SetSavedPoint(61,-8.684615);
   SetNonlinearity1->SetSavedPoint(62,-8.84223);
   SetNonlinearity1->SetSavedPoint(63,-9.000451);
   SetNonlinearity1->SetSavedPoint(64,-9.159285);
   SetNonlinearity1->SetSavedPoint(65,-9.318739);
   SetNonlinearity1->SetSavedPoint(66,-9.47882);
   SetNonlinearity1->SetSavedPoint(67,-9.639537);
   SetNonlinearity1->SetSavedPoint(68,-9.800896);
   SetNonlinearity1->SetSavedPoint(69,-9.962905);
   SetNonlinearity1->SetSavedPoint(70,-10.12557);
   SetNonlinearity1->SetSavedPoint(71,-10.28891);
   SetNonlinearity1->SetSavedPoint(72,-10.45291);
   SetNonlinearity1->SetSavedPoint(73,-10.61761);
   SetNonlinearity1->SetSavedPoint(74,-10.78299);
   SetNonlinearity1->SetSavedPoint(75,-10.94907);
   SetNonlinearity1->SetSavedPoint(76,-11.11586);
   SetNonlinearity1->SetSavedPoint(77,-11.28338);
   SetNonlinearity1->SetSavedPoint(78,-11.45161);
   SetNonlinearity1->SetSavedPoint(79,-11.62059);
   SetNonlinearity1->SetSavedPoint(80,-11.79032);
   SetNonlinearity1->SetSavedPoint(81,-11.9608);
   SetNonlinearity1->SetSavedPoint(82,-12.13205);
   SetNonlinearity1->SetSavedPoint(83,-12.30407);
   SetNonlinearity1->SetSavedPoint(84,-12.47689);
   SetNonlinearity1->SetSavedPoint(85,-12.6505);
   SetNonlinearity1->SetSavedPoint(86,-12.82493);
   SetNonlinearity1->SetSavedPoint(87,-13.00018);
   SetNonlinearity1->SetSavedPoint(88,-13.17626);
   SetNonlinearity1->SetSavedPoint(89,-13.35319);
   SetNonlinearity1->SetSavedPoint(90,-13.53097);
   SetNonlinearity1->SetSavedPoint(91,-13.70963);
   SetNonlinearity1->SetSavedPoint(92,-13.88917);
   SetNonlinearity1->SetSavedPoint(93,-14.06961);
   SetNonlinearity1->SetSavedPoint(94,-14.25096);
   SetNonlinearity1->SetSavedPoint(95,-14.43323);
   SetNonlinearity1->SetSavedPoint(96,-14.61644);
   SetNonlinearity1->SetSavedPoint(97,-14.80061);
   SetNonlinearity1->SetSavedPoint(98,-14.98575);
   SetNonlinearity1->SetSavedPoint(99,-15.17186);
   SetNonlinearity1->SetSavedPoint(100,-15.35898);
   SetNonlinearity1->SetSavedPoint(101,0.01);
   SetNonlinearity1->SetSavedPoint(102,130);
   SetNonlinearity1->SetFillColor(19);
   SetNonlinearity1->SetFillStyle(0);
   SetNonlinearity1->SetLineColor(2);
   SetNonlinearity1->SetLineWidth(2);
   SetNonlinearity1->GetXaxis()->SetLabelFont(42);
   SetNonlinearity1->GetXaxis()->SetTitleOffset(1);
   SetNonlinearity1->GetXaxis()->SetTitleFont(42);
   SetNonlinearity1->GetYaxis()->SetLabelFont(42);
   SetNonlinearity1->GetYaxis()->SetTitleFont(42);
   SetNonlinearity1->SetParameter(0,0);
   SetNonlinearity1->SetParError(0,0);
   SetNonlinearity1->SetParLimits(0,0,0);
   SetNonlinearity1->Draw("same");
   
   TLegend *leg = new TLegend(0.1428571,0.004789272,0.585213,0.519636,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextFont(22);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("Graph","^{12}B","pe");
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);
   entry->SetMarkerColor(4);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.3);
   entry->SetTextFont(22);
   entry=leg->AddEntry("Graph","Neutron","pe");

   ci = TColor::GetColor("#ff6400");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);

   ci = TColor::GetColor("#ff6400");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1.3);
   entry->SetTextFont(22);
   entry=leg->AddEntry("Graph","^{12}B + Neutron","pe");

   ci = TColor::GetColor("#9400d3");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(3);

   ci = TColor::GetColor("#9400d3");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(22);
   entry->SetMarkerSize(1.3);
   entry->SetTextFont(22);
   entry=leg->AddEntry("SetNonlinearity","Set value","l");
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(22);
   leg->Draw();
   pad1__0->Modified();
   NonlinearityLPMTtoMu_Canvas->cd();
  
// ------------>Primitives in pad: pad2
   TPad *pad2__1 = new TPad("pad2", "pad2",0,0,1,0.4);
   pad2__1->Draw();
   pad2__1->cd();
   pad2__1->Range(-2.490888,-5.564516,1.599843,2.5);
   pad2__1->SetFillColor(0);
   pad2__1->SetBorderMode(0);
   pad2__1->SetBorderSize(2);
   pad2__1->SetLogx();
   pad2__1->SetLeftMargin(0.12);
   pad2__1->SetRightMargin(0.03);
   pad2__1->SetTopMargin(0);
   pad2__1->SetBottomMargin(0.38);
   pad2__1->SetFrameBorderMode(0);
   pad2__1->SetFrameBorderMode(0);
   
   multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("");
   
   Double_t Graph_fx1004[20] = { 0.07088048, 0.1348355, 0.1958755, 0.2699821, 0.3521812, 0.4373954, 0.5242529, 0.6129014, 0.7005019, 1.072158, 2.042972, 2.991336, 3.931701, 4.8585, 5.797622, 6.721088, 7.632185,
   8.514893, 12.04559, 20.71303 };
   Double_t Graph_fy1004[20] = { -1.016343, -0.1608309, -0.5435211, -0.5028632, -0.7087166, -0.6035542, -0.2726133, -1.580147, 0.8446194, -0.7529057, -0.6029192, -0.6034967, 0.09632031, 0.475505, -0.9750858, -1.026421, -0.4443504,
   -1.077369, -0.2351817, -0.5816418 };
   Double_t Graph_fex1004[20] = { 0.0001215959, 0.0001553578, 0.0002039987, 0.0003001669, 0.0004389562, 0.0006081787, 0.0007845623, 0.0009865142, 0.001200482, 0.0007787596, 0.002063056, 0.003747125, 0.005760972, 0.008049813, 0.01052353, 0.01329151, 0.01633286,
   0.01899218, 0.01392933, 0.05499344 };
   Double_t Graph_fey1004[20] = { 0.7146737, 0.4593979, 0.4030765, 0.4241347, 0.4683313, 0.5197179, 0.5595817, 0.5879216, 0.6470295, 0.2652909, 0.3679718, 0.455366, 0.5379874, 0.6103457, 0.6550715, 0.7065583, 0.7680027,
   0.7977919, 0.3545888, 0.8252346 };
   gre = new TGraphErrors(20,Graph_fx1004,Graph_fy1004,Graph_fex1004,Graph_fey1004);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillStyle(3002);
   gre->SetLineColor(4);
   gre->SetLineWidth(3);
   gre->SetMarkerColor(4);
   gre->SetMarkerStyle(20);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_Graph1004 = new TH1F("Graph_Graph1004","",100,0.063683,22.83774);
   Graph_Graph1004->SetMinimum(-2.534041);
   Graph_Graph1004->SetMaximum(1.857621);
   Graph_Graph1004->SetDirectory(nullptr);
   Graph_Graph1004->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1004->SetLineColor(ci);
   Graph_Graph1004->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   Graph_Graph1004->GetXaxis()->CenterTitle(true);
   Graph_Graph1004->GetXaxis()->SetLabelFont(42);
   Graph_Graph1004->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph1004->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1004->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph1004->GetXaxis()->SetTitleFont(22);
   Graph_Graph1004->GetYaxis()->SetTitle("Residual nonlinearity [%]");
   Graph_Graph1004->GetYaxis()->CenterTitle(true);
   Graph_Graph1004->GetYaxis()->SetLabelFont(42);
   Graph_Graph1004->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph1004->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1004->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph1004->GetYaxis()->SetTitleFont(22);
   Graph_Graph1004->GetZaxis()->SetLabelFont(42);
   Graph_Graph1004->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1004);
   
   multigraph->Add(gre,"P");
   
   Double_t Graph_fx1005[15] = { 0.03975341, 0.08140691, 0.1679644, 0.255377, 0.3429123, 0.4307091, 0.5197644, 0.6082419, 0.6970558, 0.7870613, 1.162305, 2.102184, 2.978726, 3.836917, 4.539167 };
   Double_t Graph_fy1005[15] = { -0.144834, -0.2725198, -0.0283182, 0.1157681, 0.1435034, -0.1454811, 0.2005766, 0.1646199, -0.4152164, 0.3322139, -0.2885212, -0.692337, -0.1644695, -0.3667805, -0.4584753 };
   Double_t Graph_fex1005[15] = { 1.524814e-05, 2.31173e-05, 6.53661e-05, 0.0001225934, 0.0001926619, 0.0002767631, 0.0003711991, 0.0004775735, 0.0005933013, 0.0007205816, 0.0004488116, 0.001076178, 0.001683046, 0.002504983, 0.005571253 };
   Double_t Graph_fey1005[15] = { 0.1871259, 0.1312345, 0.1629979, 0.1933055, 0.2214692, 0.2486844, 0.275517, 0.300817, 0.3219269, 0.3487638, 0.1515328, 0.1932443, 0.2114458, 0.2410774, 0.45649 };
   gre = new TGraphErrors(15,Graph_fx1005,Graph_fy1005,Graph_fex1005,Graph_fey1005);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   ci = TColor::GetColor("#ff6400");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#ff6400");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(21);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_Graph1005 = new TH1F("Graph_Graph1005","",100,0.03576434,4.995238);
   Graph_Graph1005->SetMinimum(-1.07456);
   Graph_Graph1005->SetMaximum(0.840572);
   Graph_Graph1005->SetDirectory(nullptr);
   Graph_Graph1005->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1005->SetLineColor(ci);
   Graph_Graph1005->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   Graph_Graph1005->GetXaxis()->CenterTitle(true);
   Graph_Graph1005->GetXaxis()->SetLabelFont(42);
   Graph_Graph1005->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph1005->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1005->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph1005->GetXaxis()->SetTitleFont(22);
   Graph_Graph1005->GetYaxis()->SetTitle("Residual nonlinearity [%]");
   Graph_Graph1005->GetYaxis()->CenterTitle(true);
   Graph_Graph1005->GetYaxis()->SetLabelFont(42);
   Graph_Graph1005->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph1005->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1005->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph1005->GetYaxis()->SetTitleFont(22);
   Graph_Graph1005->GetZaxis()->SetLabelFont(42);
   Graph_Graph1005->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1005->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1005);
   
   multigraph->Add(gre,"P");
   
   Double_t Graph_fx1006[21] = { 0.03971208, 0.08143538, 0.1638447, 0.2424291, 0.3250218, 0.4113833, 0.500047, 0.5878772, 0.6767072, 0.7659826, 1.141049, 2.089772, 2.980865, 3.852311, 4.649046, 5.797622, 6.721088,
   7.632185, 8.514893, 12.04559, 20.71303 };
   Double_t Graph_fy1006[21] = { -0.1716955, -0.2791718, -0.02776365, 0.03474698, 0.05434699, -0.226646, 0.06746885, 0.1064585, -0.6404319, 0.4789074, -0.3687428, -0.672495, -0.2362094, -0.283245, -0.145995, -0.9750858, -1.026421,
   -0.4443504, -1.077369, -0.2351817, -0.5816418 };
   Double_t Graph_fex1006[21] = { 1.518915e-05, 2.284778e-05, 6.043204e-05, 0.0001058218, 0.0001632284, 0.0002352842, 0.0003180585, 0.000409193, 0.000509662, 0.0006191065, 0.0003892837, 0.0009543416, 0.001535312, 0.002297671, 0.004596684, 0.01052353, 0.01329151,
   0.01633286, 0.01899218, 0.01392933, 0.05499344 };
   Double_t Graph_fey1006[21] = { 0.1865974, 0.1299541, 0.1557292, 0.1772466, 0.1992717, 0.2223395, 0.2458929, 0.2672851, 0.2845674, 0.3090643, 0.1361481, 0.1743079, 0.1942684, 0.2221063, 0.3676553, 0.6550715, 0.7065583,
   0.7680027, 0.7977919, 0.3545888, 0.8252346 };
   gre = new TGraphErrors(21,Graph_fx1006,Graph_fy1006,Graph_fex1006,Graph_fey1006);
   gre->SetName("Graph");
   gre->SetTitle("");
   gre->SetFillStyle(3002);

   ci = TColor::GetColor("#9400d3");
   gre->SetLineColor(ci);
   gre->SetLineWidth(3);

   ci = TColor::GetColor("#9400d3");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(22);
   gre->SetMarkerSize(1.3);
   
   TH1F *Graph_Graph1006 = new TH1F("Graph_Graph1006","",100,0.0357272,22.84085);
   Graph_Graph1006->SetMinimum(-2.141474);
   Graph_Graph1006->SetMaximum(1.054285);
   Graph_Graph1006->SetDirectory(nullptr);
   Graph_Graph1006->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph1006->SetLineColor(ci);
   Graph_Graph1006->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   Graph_Graph1006->GetXaxis()->CenterTitle(true);
   Graph_Graph1006->GetXaxis()->SetLabelFont(42);
   Graph_Graph1006->GetXaxis()->SetLabelSize(0.06);
   Graph_Graph1006->GetXaxis()->SetTitleSize(0.06);
   Graph_Graph1006->GetXaxis()->SetTitleOffset(1.2);
   Graph_Graph1006->GetXaxis()->SetTitleFont(22);
   Graph_Graph1006->GetYaxis()->SetTitle("Residual nonlinearity [%]");
   Graph_Graph1006->GetYaxis()->CenterTitle(true);
   Graph_Graph1006->GetYaxis()->SetLabelFont(42);
   Graph_Graph1006->GetYaxis()->SetLabelSize(0.06);
   Graph_Graph1006->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1006->GetYaxis()->SetTitleOffset(1.3);
   Graph_Graph1006->GetYaxis()->SetTitleFont(22);
   Graph_Graph1006->GetZaxis()->SetLabelFont(42);
   Graph_Graph1006->GetZaxis()->SetTitleOffset(1);
   Graph_Graph1006->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1006);
   
   multigraph->Add(gre,"P");
   multigraph->Draw("A P Z");
   multigraph->GetXaxis()->SetLimits(0.01, 30);
   multigraph->GetXaxis()->SetTitle("#mu_{LPMT} [p.e.]");
   multigraph->GetXaxis()->CenterTitle(true);
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetLabelSize(0.12);
   multigraph->GetXaxis()->SetTitleSize(0.16);
   multigraph->GetXaxis()->SetTitleOffset(1);
   multigraph->GetXaxis()->SetTitleFont(22);
   multigraph->GetYaxis()->SetTitle("Residual [%]");
   multigraph->GetYaxis()->CenterTitle(true);
   multigraph->GetYaxis()->SetNdivisions(205);
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelSize(0.12);
   multigraph->GetYaxis()->SetTitleSize(0.12);
   multigraph->GetYaxis()->SetTitleOffset(0.3);
   multigraph->GetYaxis()->SetTitleFont(22);
   TLine *line = new TLine(0,0,20.71303,0);
   line->SetLineStyle(2);
   line->Draw();
   pad2__1->Modified();
   NonlinearityLPMTtoMu_Canvas->cd();
   NonlinearityLPMTtoMu_Canvas->Modified();
   NonlinearityLPMTtoMu_Canvas->SetSelected(NonlinearityLPMTtoMu_Canvas);
}
