void Draw() {
	double x[3] = {1, 5, 10};
	double xer[3] = {0, 0, 0};
	// double y15[3] = {1.402, 1.413, 1.469};
	// double y15er[3] = {0.027, 0.030, 0.031};
	// double y12[3] = {1.691, 1.717, 1.722};
	// double y12er[3] = {0.030, 0.025, 0.021};

	double y15[3] = {1.335, 1.397, 1.469};
	double y15er[3] = {0.023, 0.032, 0.031};
	double y12[3] = {1.638, 1.669, 1.722};
	double y12er[3] = {0.029, 0.0234, 0.021};

	auto TTSvCL15 = new TGraphErrors(3, x, y15, xer, y15er);
	auto TTSvCL12 = new TGraphErrors(3, x, y12, xer, y12er);
	TTSvCL12->SetMarkerStyle(kFullDotLarge);
	TTSvCL12->SetMarkerColor(kBlue);
	TTSvCL12->SetLineColor(kBlue);
	TTSvCL12->SetLineWidth(2);
	TTSvCL12->GetXaxis()->SetRangeUser(-1, 12);
	TTSvCL15->SetMarkerStyle(kFullDotLarge);
	TTSvCL15->SetMarkerColor(kRed);
	TTSvCL15->SetLineColor(kRed);
	TTSvCL15->SetLineWidth(2);
	TTSvCL15->GetXaxis()->SetRangeUser(-1, 12);

	auto mg = new TMultiGraph();
	mg->Add(TTSvCL12);
	mg->Add(TTSvCL15);
	mg->GetXaxis()->SetLabelSize(0.07);
	mg->GetXaxis()->SetTitle("Cable length / m");
	mg->GetXaxis()->SetTitleSize(0.08);
	mg->GetXaxis()->SetRangeUser(0, 11);
	mg->GetYaxis()->SetLabelSize(0.07);
	mg->GetYaxis()->SetTitle("TTS (#sigma) / ns");
	mg->GetYaxis()->SetTitleSize(0.08);
	mg->GetYaxis()->SetTitleOffset(0.8);
	mg->GetYaxis()->SetRangeUser(1.3, 2.0);
	auto c = new TCanvas();
	c->SetBottomMargin(0.2);
	c->SetLeftMargin(0.2);
	c->cd();
	mg->Draw("APL");

	auto leg = new TLegend(0.4, 0.6, 0.89, 0.89);
	leg->AddEntry(TTSvCL12, "TTS @ 1279 V (3E6, 10 m)");
	leg->AddEntry(TTSvCL15, "TTS @ 1529 V (1E7, 10 m)");
	leg->Draw();

}

void Dis(){
	double mean[11] = {277.2, 276.8, 276.8, 277.1, 277.2, 277, 276.9, 277.1, 277, 276.9, 277.4};
	double meanerr[11] = {0.081, 0.084, 0.074, 0.093, 0.079, 0.085, 0.084, 0.074, 0.083, 0.078, 0.043};
	double sig[11] = {1.791, 1.601, 1.688, 1.835, 1.502, 1.745, 1.747, 1.743, 1.661, 1.751, 1.882};
	double sigerr[11] = {0.076, 0.088, 0.067, 0.080, 0.062, 0.066, 0.069, 0.065, 0.078, 0.074, 0.043};

	TH1F* m = new TH1F("mean", "", 20, 276, 278);
	TH1F* s = new TH1F("sigma", "", 100, 1, 2);
	
	double me = 0;
	double se = 0;
	for(int i = 0;i < 11;i++){
		m->Fill(mean[i]);
		s->Fill(sig[i]);
		me += meanerr[i] * meanerr[i];
		se += sigerr[i] * sigerr[i];
	}
	me = sqrt(me) / 11;
	se = sqrt(se) / 11;
	
	double meanrms = m->GetRMS();
	double sigrms = s->GetRMS();
	double sigmean = s->GetMean();
	
	cout << "sigmean: " << sigmean << endl
	     << "se: " << se << endl
	     << "meanrms: " << meanrms << endl;


}

void timePar() {
	ifstream in("./t_related_1_5");
}
