void FastChannel()
{
	int triggerDACu;
	triggerDACu = 650;
	double RC = 5.0;
	double Cf = 0.25;
	TString name;
	name = "./ch1.txt";
	ifstream in(name);
//	while (in.good()){
		double ori, nan;
		double baseline = 0, charge = 0;
		TCanvas *c1 = new TCanvas("c1","c1",1);
		c1->Divide(2,2);
		TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
       TH1F* t = new TH1F("t","",1000,-2,2);
		for (int i = 0; i < 1008; i ++) {
    		in >> ori >> nan >> nan >> nan;
    		tmp->SetBinContent(i + 1, ori/10.0);
		}
		int peakPos = tmp->GetMinimumBin();
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
		}
		cout<<"charge is "<<charge<<endl;
		TH1 *hr = 0;
		TH1 *hi = 0;
		hr = tmp->FFT(hr,"RE");
		hi = tmp->FFT(hi,"IM");
		Double_t *re_full = new Double_t[1008];
		Double_t *im_full = new Double_t[1008];
		Double_t *re_back = new Double_t[1008];
		Double_t *im_back = new Double_t[1008];
		Double_t *re_test = new Double_t[1008];
		Double_t *im_test = new Double_t[1008];
		Double_t *re_testtrans = new Double_t[1008];
		Double_t *im_testtrans = new Double_t[1008];
		Int_t n =1008;
		complex <double> full;
		complex <double> response;
		complex <double> test;
		complex <double> testtrans;
		for(Int_t i=0;i<n;i++)
		{
			re_full[i] = (hr->GetBinContent(i+1))/(sqrt(n)*50);
			im_full[i] = (hi->GetBinContent(i+1))/(sqrt(n)*50);
			response = 20.0*complex<double>(0.0,2.0*TMath::Pi()*i*RC/1008.0)/(complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0));
			//RC/(Cf*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0));
			//RC/(Cf*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RC/1008.0));
			test = 2.5*20.0/(complex<double>(1.0,2.0*TMath::Pi()*i*20.0/1008.0)*sqrt(1008.0));
			testtrans = response*test*1000.0/50.0;
			full = response*complex<double>(re_full[i],im_full[i])*1000.0;
			re_test[i] = test.real();
			im_test[i] = test.imag();
			re_testtrans[i] = testtrans.real();
			im_testtrans[i] = testtrans.imag();
			re_back[i] = full.real();
			im_back[i] = full.imag();
		}
		TVirtualFFT *fft_back = TVirtualFFT::FFT(1,&n,"C2R M K");
		fft_back->SetPointsComplex(re_back,im_back);
		fft_back->Transform();
		TVirtualFFT *test_back = TVirtualFFT::FFT(1,&n,"C2R M K");
		test_back->SetPointsComplex(re_test,im_test);
		test_back->Transform();
		TVirtualFFT *testtrans_back = TVirtualFFT::FFT(1,&n,"C2R M K");
		testtrans_back->SetPointsComplex(re_testtrans,im_testtrans);
		testtrans_back->Transform();
		TH1 *tb = 0;
		tb = TH1::TransformHisto(test_back,tb,"Re");
		tb->Scale(1.0/sqrt(1008.0));
		TH1 *hb = 0;
		hb = TH1::TransformHisto(fft_back,hb,"Re");
		hb->Scale(1.0/sqrt(1008.0));
		TH1 *ttb = 0;
		ttb = TH1::TransformHisto(testtrans_back,ttb,"Re");
		ttb->Scale(1.0/sqrt(1008.0));
		c1->cd(1);
		tmp->Draw();
		tmp->SetTitle("real signal");
		tmp->GetXaxis()->SetTitle("time/ns");
		tmp->GetYaxis()->SetTitle("voltage/mV");
		c1->cd(2);
		hb->Draw("hist");
		hb->SetTitle("real signal after FSH");
		hb->GetXaxis()->SetTitle("time/ns");
		hb->GetYaxis()->SetTitle("voltage/mV");
		c1->cd(3);
		tb->Draw("hist");
		tb->SetTitle("modal signal");
		tb->GetXaxis()->SetTitle("time/ns");
		tb->GetYaxis()->SetTitle("voltage/mV");
		c1->cd(4);
		ttb->Draw("hist");
		ttb->SetTitle("modal signal after FSH");
		ttb->GetXaxis()->SetTitle("time/ns");
		ttb->GetYaxis()->SetTitle("voltage/mV");
//	}
}
 
