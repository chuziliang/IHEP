void mutiHist()
{
	TFile *c1 = new TFile("histos.root");
	TH1F *h1 = (TH1F*)c1->Get("True PE");
	TH1F *h2 = (TH1F*)c1->Get("Number of PMT");
	TH1F *h3 = (TH1F*)c1->Get("Reconstructed total charge");

	h1->SetLineColor(kRed);
	h2->SetLineColor(kBlue);
	h3->SetLineColor(kBlack);
	TCanvas* a = new TCanvas();
//	gStyle->SetOptStat("1111");
	h2->SetTitle("True PE, PMTs and reconstructed charge per event");
	h2->GetXaxis()->SetTitle("number of PE");
	h2->Draw();
	a->Update();
	TPaveStats* stats2 = (TPaveStats*)a->GetPrimitive("stats");
	double y1 = stats2->GetY1NDC();
	double y2 = stats2->GetY2NDC();
	double newy1 = 2 * y1 - y2;   // new (lower) y start position of stats box
	double newy2 = y1;            // new (upper) y start position of stats box
	stats2->SetName("h2");
	stats2->SetY1NDC(newy1);    //set new y start position
	stats2->SetY2NDC(newy2);    //set new y end position
	a->Update();

	h3->Draw("sames");
	a->Update();
	newy1 = 3 * y1 - 2 * y2;   // new (lower) y start position of stats box
	newy2 = 2 * y1 -   y2;            // new (upper) y start position of stats box
	TPaveStats* stats3 = (TPaveStats*) a->GetPrimitive("stats");
	stats3->SetName("h3");
	stats3->SetY1NDC(newy1);
	stats3->SetY2NDC(newy2);
	stats3->SetLineColor(kRed);
	a->Update();

	h1->Draw("sames");
	a->Update();

//	gStyle->SetOptStat("nemr");
//   gPad->BuildLegend(0.7,0.7,0.95,0.95,"");
}
