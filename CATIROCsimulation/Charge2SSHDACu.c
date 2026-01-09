void Charge2SSHDACu()
{
	int triggerDACu;
	triggerDACu = 650;
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
	double ChargeSSH = 0.0;
	TString name;
	name = "./ch1.txt";
	TH1F *result = new TH1F("Charge","Charge",100,0.0,2.0);
	TH1F *result2 = new TH1F("ChargeSSH","ChargeSSH",1000,0.0,300);
	TH1F *result3 = new TH1F("Charge trigger","Charge trigger",100,0.0,2.0);
	TH2 *Charge2DACu = new TH2D("Charge-SSH","Charge-SSH",1000,0,2.0,1000,0.0,800); 
	ifstream in(name);
	while (in.good()){
		double ori, nan;
		double baseline = 0, charge = 0;
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
		Int_t n =1008;
		complex <double> backFSH = 0;
		complex <double> backSSH = 0;
		complex <double> ResponseSlow = 0;
		complex <double> ResponseFast = 0;
		for(Int_t i=0;i<n;i++)
		{
			re_full[i] = (hr->GetBinContent(i+1))/(sqrt(n)*50);
			im_full[i] = (hi->GetBinContent(i+1))/(sqrt(n)*50);
			ResponseSlow = complex<double>(0.0,TMath::Pi()*i*RCslow/1008.0)*RCslow/(CfHG*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/1008.0));
			ResponseFast = RCfast/(CfHG*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/1008.0));
			backFSH = ResponseFast*complex<double>(re_full[i],im_full[i])*1000.0;
			backSSH = ResponseSlow*complex<double>(re_full[i],im_full[i])*1000.0;
			re_backFSH[i] = backFSH.real();
			im_backFSH[i] = backFSH.imag();
			re_backSSH[i] = backSSH.real();
			im_backSSH[i] = backSSH.imag();
		}
		delete hr;
		delete hi;
		TVirtualFFT *fft_backFSH = TVirtualFFT::FFT(1,&n,"C2R M K");
		fft_backFSH->SetPointsComplex(re_backFSH,im_backFSH);
		fft_backFSH->Transform();
		TVirtualFFT *fft_backSSH = TVirtualFFT::FFT(1,&n,"C2R M K");
		fft_backSSH->SetPointsComplex(re_backSSH,im_backSSH);
		fft_backSSH->Transform();
		TH1 *hbFSH = 0;
		hbFSH = TH1::TransformHisto(fft_backFSH,hbFSH,"Re");
		hbFSH->Scale(1.0/sqrt(1008.0));
		hbFSH->SetName("hbFSH");
		TH1 *hbSSH = 0;
		hbSSH = TH1::TransformHisto(fft_backSSH,hbSSH,"Re");
		hbSSH->Scale(1.0/sqrt(1008.0));
		for (int i = 0; i<n; i++)
		{
			if (hbFSH->GetBinContent(i+1) >= 0.9*(950-triggerDACu))
			{
				ChargeSSH = (hbSSH->GetBinContent(i+27))*0.3854+78.59;
				result2->Fill(ChargeSSH);
				result3->Fill(charge);
				Charge2DACu->Fill(charge,ChargeSSH);
				break;
			}
		}
		result->Fill(charge);
		delete hbFSH;
		delete hbSSH;
		delete fft_backFSH;
		delete fft_backSSH;
		delete tmp;
		delete t;
	}
	TCanvas *c1 = new TCanvas("c1","c1",800,400);
	c1->Divide(2,2);
	c1->cd(1);
	Charge2DACu->Draw("COL");
	TF1* func = new TF1("f1","[0]*x+[1]",0,800);
	func->SetParNames("k","b");
	Charge2DACu->Fit("f1","R");
	func->Draw("SAME");
	Charge2DACu->GetYaxis()->SetTitle("SSH output/ADCu");
	Charge2DACu->GetXaxis()->SetTitle("charge/pC");
	gStyle->SetOptFit(1011);
	c1->cd(2);
	result2->Draw();
	c1->cd(3);
	result3->Draw();
	c1->cd(4);
	result->Draw();
	TFile *ofile = new TFile("effiency_fit_data.root","recreate");
	result2->Write("effiency_fit_data");
	in.close();
}
 
