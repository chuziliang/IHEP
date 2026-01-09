void TriggerEff()
{
	int triggerDACu;
	triggerDACu = 650;
	double RC = 5.0;
	double Cf = 0.25;
	TString name;
	name = "./ch1.txt";
	ifstream in(name);
	double eventnum = 0;
	double sum = 0;
	TH1F *h1 = new TH1F("Charge-DACu","Charge-DACu",800,0,800); 
	TH1F *result = new TH1F("S-curve","S-curve",800,0,800);
	while (in.good()){
		double ori, nan;
		Double_t baseline = 0, charge = 0, RMS = 0;
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
               charge += tmp->GetBinContent(i) * 1.0 / 50.0;
        	}
			if (i <= (peakPos - 50) || i >(peakPos + 150))
                {
                    t->Fill(tmp->GetBinContent(i));
                }
	    	}
		RMS = t->GetRMS();
		if ( RMS < 0.1 )
		{
			Double_t signalmax = 0;
			signalmax = tmp->GetMaximum();
			eventnum++;
			TH1 *hr = 0;
			TH1 *hi = 0;
			hr = tmp->FFT(hr,"RE");
			hi = tmp->FFT(hi,"IM");
			Double_t *re_full = new Double_t[1008];
			Double_t *im_full = new Double_t[1008];
			Double_t *re_back = new Double_t[1008];
			Double_t *im_back = new Double_t[1008];
			Int_t n =1008;
			complex <double> full = 0;
			complex <double> response = 0;
			for(Int_t i=0;i<n;i++)
			{
				re_full[i] = (hr->GetBinContent(i+1))/(sqrt(n)*50);
				im_full[i] = (hi->GetBinContent(i+1))/(sqrt(n)*50);
				response = RC/(Cf*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0));
				full = response*complex<double>(re_full[i],im_full[i])*1000.0;
				re_back[i] = full.real();
				im_back[i] = full.imag();
			}
			delete hr;
			delete hi;
			TVirtualFFT *fft_back = TVirtualFFT::FFT(1,&n,"C2R M K");
			fft_back->SetPointsComplex(re_back,im_back);
			fft_back->Transform();
			TH1 *hb = 0;
			hb = TH1::TransformHisto(fft_back,hb,"Re");
			hb->Scale(1.0/sqrt(1008.0));
			Double_t maximum = 0;
			maximum = hb->GetMaximum();
			h1->Fill(950-maximum/0.9);
			delete hb;
			delete fft_back;
		}
		delete tmp;
		delete t;
	}
	for (int i = 800; i > 0; i--)
	{
		sum += h1->GetBinContent(i);
		result->SetBinContent(i,(eventnum-sum)/eventnum);
	}
	result->Draw();
	result->GetXaxis()->SetTitle("DACu");
	result->GetYaxis()->SetTitle("effiency");
	result->SetStats(kFALSE);
	in.close();
}
 
