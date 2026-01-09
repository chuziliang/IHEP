void GetCharge()
{
    TH1F* spec[5];
    TF1* func[5];
    double scale = 1.0;
    TCanvas* c1 = new TCanvas("c1","c1",1);
   // c1->Divide(3,2); 
    
   
	for (int j=0; j < 1; j++)
    {
        TString name;
        TString histname;
        TString num = to_string(50*j+1051);
        name = "./testfile_Z6_pmt1_HV" + num + "V_gain10.txt";
        histname = "HV" + num + " Charge Spectrum";
        ifstream in(name);
    	spec[j] = new TH1F(histname, histname, 1000, -0.5, 3);
        //int j = 0;
	    while (in.good()) {
	    	double charge = 0, baseline = 0, RMS = 0;
	    	double ori;
	    	TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
            TH1F* t = new TH1F("t","",1000,-2,2);
	    	for (int i = 0; i < 1008; i ++) {
	    		in >> ori ;
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
         //    if (RMS < 0.1)
           // {
            //    spec[j]->Fill(charge);
            //}
			if (charge > 0.7)
			{
				spec[j] = (TH1F*)tmp->Clone();
				delete t;
				delete tmp;
				break;
			}
            delete t;
	    	delete tmp;
        //    j++;
	    }
		c1->cd(j+1);
        Double_t Q0 = 0;
	    func[j] = new TF1("SpecModel","gaus",0.5,3);
	 //   func[j]->SetParameters(Q0, 0.03, 0.8, 0.3, 0.034, -20, 0.5, 7);
	 //   func[j]->SetParNames("Q", "sigma");
        double para[3];
        spec[j]->Fit(func[j], "R");
        func[j]->GetParameters(para);
		gStyle->SetOptFit(1011);
        double gain = para[1] / 0.16;
	//	if (j<9)
	//	{
    //    spec[j]->SetLineColor(j+1);
	//	}
	//	if (j>=9)
	//	{
	//		spec[j]->SetLineColor((j-7)*10);
	//	}
        spec[j]->Draw();
        func[j]->Draw("same");
        cout << "gain: " << gain << endl;
	    cout <<"gain error: " << func[j]->GetParError(2) / 0.16 << endl;
	    in.close();
    }
}
