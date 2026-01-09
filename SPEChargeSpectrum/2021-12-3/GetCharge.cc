void GetCharge()
{
    // TH1F* spec;
    double scale = 1.0;
    int signalNumber = 0;
    TFile *outfile = new TFile("Signals.root","recreate");
	for (int j=0; j < 4; j++)
    {
        TString name;
        TString histname;
        name = TString::Format("./%1d.txt",j);
        histname = "HV1024_2223 Charge Spectrum";
        ifstream in(name);
    	// spec = new TH1F(histname, histname, 100, 0, 1);
        //int j = 0;
	    while (in.good()) {
	    	double baseline = 0, RMS = 0;
	    	double ori, nan;
            double max = 0;
            double min = 1;
	    	TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
            TH1F* t = new TH1F("t","",1000,-2,2);
	    	for (int i = 0; i < 1008; i ++) {
	    		in >> ori >> nan >> nan >> nan;
	    		tmp->SetBinContent(i + 1, ori/10.0);//除以10为放大器倍数。
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
                if (i <= (peakPos - 50) || i >(peakPos + 150))
                {
                    t->Fill(tmp->GetBinContent(i));
                    if (tmp->GetBinContent(i)>max)
                    {
                        max = tmp->GetBinContent(i);
                    }
                    if (tmp->GetBinContent(i)<min)
                    {
                        min = tmp->GetBinContent(i);
                    }
                    
                }
	    	}
            RMS = t->GetRMS();
            // spec ->Fill(max-min);

            if (RMS < 0.2 && (max -min)<0.8)
            {
                if (signalNumber%1000 == 0)
                {   
                    outfile->cd();
                    outfile->mkdir(TString::Format("%1d",signalNumber/1000));
                    cout<<signalNumber/1000<<endl;
                }
                    
                tmp->SetName(TString::Format("signal %1d",signalNumber));
                
                
                //cout<<charge<<endl;

                outfile->cd(TString::Format("%1d",signalNumber/1000));
                tmp->Write();
                signalNumber++;
            }
            max = 0;
            min = 1;
            delete t;
	    	delete tmp;
            //j++;
	    }
        // spec->Draw();
	    in.close();
    }
}
