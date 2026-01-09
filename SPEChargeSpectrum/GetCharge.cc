Double_t specFit(Double_t* x, Double_t* par) {
	double f1, f2, f3, f4, f5, f6;
	double f = 0;
	for (int n = 0; n <= 3; n ++) {
		f1 = (1 - par[5]) * exp(-0.5 * (x[0] - par[0] - n * par[2]) * (x[0] - par[0] - n * par[2]) / (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * TMath::Pi() * (par[1] * par[1] + n * par[3] * par[3])));
		f2 = 0.5 * par[4] * exp(-1 * par[4] * (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f3 = TMath::Erf(TMath::Abs(n * par[2] + par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f4 = TMath::Erf(TMath::Abs(x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f5 = TMath::Sign(1.0, (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f6 = TMath::Poisson(n, par[6]);
		f += f6 * (f1 + par[5] * (f2 * (f3 + f5 * f4)));
	}
	return f * par[7];
}

void GetCharge()
{
    TH1F* spec;
    double scale = 1.0;
    TCanvas* c1 = new TCanvas("c1","c1",1);
    //c1->Divide(4,4); 
    
   
	//for (int j=0; j < 16; j++)
    {
        TString name;
        TString histname;
        name = "./HV1024_2223.txt";
        histname = "HV1024_2223 Charge Spectrum";
        ifstream in(name);
    	spec = new TH1F(histname, histname, 100, -1, 3);
        //int j = 0;
	    while (in.good()) {
	    	double charge = 0, baseline = 0, RMS = 0;
	    	double ori, nan;
	    	TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
            TH1F* t = new TH1F("t","",1000,-2,2);
	    	for (int i = 0; i < 1008; i ++) {
	    		in >> ori >> nan >> nan >> nan;
	    		tmp->SetBinContent(i + 1, ori/10.0);
	    	}
            int peakPos = tmp->GetMinimumBin();
            if ((peakPos < 90) || (peakPos > 850))
            {
                delete t;
	    	    delete tmp;
                continue;
            }
	    	for (int i = (peakPos - 80); i < (peakPos - 50); i ++) {
	    		baseline += tmp->GetBinContent(i);
	    	}
	    	baseline /= 30;
	    	for (int i = 1; i <= 1008; i ++) {
	    		tmp->SetBinContent(i, baseline - tmp->GetBinContent(i));
	    		if (i >= (peakPos - 30) && i < (peakPos + 70))
                {
                    charge += tmp->GetBinContent(i) * scale / 50;
                }
                if (i <= (peakPos - 50) || i >(peakPos + 150))
                {
                    t->Fill(tmp->GetBinContent(i));
                }
	    	}
            RMS = t->GetRMS();
            if (RMS < 0.1)
            {
                spec ->Fill(charge);
                //cout<<charge<<endl;
            }
            delete t;
	    	delete tmp;
            //j++;
	    }
        Double_t Q0 = 0;
	    TF1* func = new TF1("SpecModel", specFit,-0.20, 2.5, 8);
	    func->SetParameters(Q0, 0.03, 0.8, 0.5, 0.034, -20, 0.5, 7);
	    func->SetParNames("Q_{0}", "#sigma_{0}", "Q_{1}", "#sigma_{1}", "#alpha", "w", "u", "A");
        double para[8];
        spec->Fit(func, "R");
        func->GetParameters(para); 
        double gain = para[2] / 0.16;
        // c1->cd(j+1);
        // spec->SetLineColor(j);
        spec->Draw();
        func->Draw("same");
        cout << "gain: " << gain << endl;
	    cout <<"gain error: " << func->GetParError(2) / 0.16 << endl;
        delete func;
	    in.close();
    }
}
