#define needchannel 4
void SlowChannel()
{
	int triggerDACu;
	triggerDACu = 650;
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
	TString name;
	int channelnumber = 0;
	name = "./ch1.txt";
	ifstream in(name);
	while (in.good()){
		double ori, nan;
		double baseline = 0, charge = 0;
		TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
		for (int i = 0; i < 1008; i ++) {
    		in >> ori >> nan >> nan >> nan;
    		tmp->SetBinContent(i + 1, ori/10.0);
		}
		channelnumber++;
		if (channelnumber < needchannel)
		{
			delete tmp;
			continue;
		}
		TCanvas *c1 = new TCanvas("c1","c1",1);
		c1->Divide(2,2);
		
       TH1F* t = new TH1F("t","",1000,-2,2);
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
		Double_t *re_backFSH = new Double_t[1008];
		Double_t *im_backFSH = new Double_t[1008];
		Double_t *re_backSSH = new Double_t[1008];
		Double_t *im_backSSH = new Double_t[1008];
		// Double_t *re_test = new Double_t[1008];
		// Double_t *im_test = new Double_t[1008];
		// Double_t *re_testtrans = new Double_t[1008];
		// Double_t *im_testtrans = new Double_t[1008];
		Int_t n =1008;
		complex <double> backFSH;
		complex <double> backSSH;
		complex <double> ResponseSlow;
		complex <double> ResponseFast;
		// complex <double> test;
		// complex <double> testtrans;
		for(Int_t i=0;i<n;i++)
		{
			re_full[i] = (hr->GetBinContent(i+1))/(sqrt(n)*50);
			im_full[i] = (hi->GetBinContent(i+1))/(sqrt(n)*50);
			ResponseSlow = complex<double>(0.0,TMath::Pi()*i*RCslow/1008.0)*RCslow/(CfHG*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/1008.0));
			ResponseFast = RCfast/(CfHG*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/1008.0));
			// test = 2.5*20.0/(complex<double>(1.0,2.0*TMath::Pi()*i*20.0/1008.0)*sqrt(1008.0));
			// testtrans = response*test*1000.0/50.0;
			backFSH = ResponseFast*complex<double>(re_full[i],im_full[i])*1000.0;
			backSSH = ResponseSlow*complex<double>(re_full[i],im_full[i])*1000.0;
			// re_test[i] = test.real();
			// im_test[i] = test.imag();
			// re_testtrans[i] = testtrans.real();
			// im_testtrans[i] = testtrans.imag();
			re_backFSH[i] = backFSH.real();
			im_backFSH[i] = backFSH.imag();
			re_backSSH[i] = backSSH.real();
			im_backSSH[i] = backSSH.imag();
		}
		TVirtualFFT *fft_backFSH = TVirtualFFT::FFT(1,&n,"C2R M K");
		fft_backFSH->SetPointsComplex(re_backFSH,im_backFSH);
		fft_backFSH->Transform();
		TVirtualFFT *fft_backSSH = TVirtualFFT::FFT(1,&n,"C2R M K");
		fft_backSSH->SetPointsComplex(re_backSSH,im_backSSH);
		fft_backSSH->Transform();
		// TVirtualFFT *test_back = TVirtualFFT::FFT(1,&n,"C2R M K");
		// test_back->SetPointsComplex(re_test,im_test);
		// test_back->Transform();
		// TVirtualFFT *testtrans_back = TVirtualFFT::FFT(1,&n,"C2R M K");
		// testtrans_back->SetPointsComplex(re_testtrans,im_testtrans);
		// testtrans_back->Transform();
		// TH1 *tb = 0;
		// tb = TH1::TransformHisto(test_back,tb,"Re");
		// tb->Scale(1.0/sqrt(1008.0));
		TH1 *hbFSH = 0;
		hbFSH = TH1::TransformHisto(fft_backFSH,hbFSH,"Re");
		hbFSH->Scale(1.0/sqrt(1008.0));
		TH1 *hbSSH = 0;
		hbSSH = TH1::TransformHisto(fft_backSSH,hbSSH,"Re");
		hbSSH->Scale(1.0/sqrt(1008.0));
		// TH1 *ttb = 0;
		// ttb = TH1::TransformHisto(testtrans_back,ttb,"Re");
		// ttb->Scale(1.0/sqrt(1008.0));
		c1->cd(1);
		tmp->Draw();
		tmp->SetTitle("real signal");
		tmp->GetXaxis()->SetTitle("time/ns");
		tmp->GetYaxis()->SetTitle("voltage/mV");
		c1->cd(2);
		for (int i = 0; i < 1008; i++)
		{
			if (hbFSH->GetBinContent(i+1) >= 0.9*(950-850))
			{
				cout<<i+1<<endl;
				break;
			}
		}
		
		hbFSH->Draw("hist");
		hbFSH->SetTitle("real signal after FSH");
		hbFSH->GetXaxis()->SetTitle("time/ns");
		hbFSH->GetYaxis()->SetTitle("voltage/mV");
		c1->cd(3);
		hbSSH->Draw("hist");
		hbSSH->SetTitle("real signal after SSH");
		hbSSH->GetXaxis()->SetTitle("time/ns");
		hbSSH->GetYaxis()->SetTitle("voltage/mV");
		// c1->cd(3);
		// tb->Draw("hist");
		// tb->SetTitle("modal signal");
		// tb->GetXaxis()->SetTitle("time/ns");
		// tb->GetYaxis()->SetTitle("voltage/mV");
		// c1->cd(4);
		// ttb->Draw("hist");
		// ttb->SetTitle("modal signal after SSH");
		// ttb->GetXaxis()->SetTitle("time/ns");
		// ttb->GetYaxis()->SetTitle("voltage/mV");
		if (channelnumber == needchannel)
		{
			break;
		}
	}
}
 
