
TString path("./result");

void HVfit() {
    Int_t n = 7;

    Double_t HV[n];
    Double_t eh[n];
    Double_t gain[n];
    Double_t eg[n];
    
    double hv, hve, g, gaine;

    ifstream in(path);    
    if (in.good()){
        for (int i = 0; i < n; i ++) {
            in >> hv >> hve >> g >> gaine;
			HV[i]=TMath::Log(hv);
	    	eh[i]=0;
	    	gain[i]=TMath::Log(g*1E6);
	    	eg[i]= gaine / g; 
			cout << hv << "\t" << eh[i] << "\t" << g << "\t" << eg[i] << endl;
        }
        TGraphErrors graph(n, HV, gain, eh, eg);
	graph.SetMarkerStyle(kOpenCircle);
        graph.SetMarkerColor(kBlue);
        graph.SetLineColor(kBlue);
	graph.GetXaxis()->SetTitle("ln(HV)");
	graph.GetXaxis()->SetTitleSize(0.08);
	graph.GetXaxis()->SetLabelSize(0.05);
	graph.GetYaxis()->SetTitle("ln(Gain)");
	graph.GetYaxis()->SetTitleSize(0.08);
	graph.GetYaxis()->SetLabelSize(0.07);
	graph.GetYaxis()->SetTitleOffset(1);
	graph.SetTitle("");
	auto c = new TCanvas();
	c->SetBottomMargin(0.2);
	c->SetLeftMargin(0.2);
	c->cd();
        graph.DrawClone("APE");

        TF1 f("Linear law", "[0]+x*[1]", TMath::Log(920), TMath::Log(1200));
        graph.Fit(&f, "R");
        f.DrawClone("Same");
        
	double k = f.GetParameter(1);
	double b = f.GetParameter(0);

	cout << "Chi2 / ndf: " << f.GetChisquare() << endl;

	cout << "HV@3E6: " << TMath::Exp((TMath::Log(3*1E6)-b)/k) << endl;
	cout << "HV@1E7: " << TMath::Exp((TMath::Log(10*1E6)-b)/k) << endl;    

	auto errHV = new TH1F("ErrorOfHV", "", 1000, 0, 100);
	for (int i = 0; i < n; i ++) {
		double tmpV = (gain[i] - b) / k;
		tmpV = exp(tmpV);
		errHV->Fill(abs(tmpV - exp(HV[i])));
	}
	cout << "HV error: " << errHV->GetRMS() << endl;
	// cout << sqrt(pow(-TMath::Exp((TMath::Log(3*1E6)-b)/k)/k,2)*pow(f.GetParError(0),2)+
	// 	     pow(-((TMath::Log(3*1E6)-b)/k/k)*TMath::Exp((TMath::Log(3*1E6)-b)/k),2)*pow(f.GetParError(1),2)) << endl;

	// double bate = k;
	// double alpha = exp(b);
	// double dbate = pow(3E6 / alpha, 1 / bate) * TMath::Log(3E6 / alpha);
	// double dalpha = pow(3E6 / alpha, 1 / bate) / bate / alpha;
	// double ebate = f.GetParError(1);
	// double ealpha = exp(b) * f.GetParError(0);
	// cout << "bate: " << bate <<endl
	//      << "alpha: " << alpha << endl
	//      << "dbate: " << dbate << endl
	//      << "dalpha: " << dalpha << endl
	//      << "ebate: " << ebate << endl
	//      << "ealpha: " << ealpha << endl;
	// cout << "Gain 3E6 error: " << sqrt(pow(dbate * ebate, 2) + pow(dalpha * ealpha, 2)) << endl;
    }
}
