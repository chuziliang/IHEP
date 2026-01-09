void ScurveSim()
{
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
	double ChargeSSH = 0.0;
	double eventnum = 1000000;
	double sum = 0;
	// TFile *ofile = new TFile("hist_in_different_trigger_sim_300.root","recreate");
	TRandom *rng = new TRandom2();
	rng->SetSeed(3357);
	TFile* f = TFile::Open("joint2.root");
    TH2 *joint = nullptr; 
    f->GetObject("joint",joint);
	TH1* tmp = new TH1F("tmp"," ", 1008, 0, 1008);
	TF1* func = new TF1("SpecModel", "landau",0 , 1008);
	TH1F *h1 = new TH1F("Charge-DACu","Charge-DACu",800,0,800);
	TH1F *result = new TH1F("S-curve","S-curve",800,0,800);
	Double_t x;
    Double_t y;
	TH2* SigJoint = new TH2D("Sigjoint","Sigjoint",1000,10,100,1000,0,10);
	for ( Int_t cyc = 0; cyc<eventnum; cyc++){
		if (cyc%10000 == 0) cout<<cyc/10000<<endl;
		double charge = 0;
		joint->GetRandom2(x,y,rng);
        //sigma->Fill(y); 
		SigJoint->Fill(x,y);
        func->SetParameters(x/y,463.6,y);
        func->SetNpx(1008);
        tmp = func->CreateHistogram();
		
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
		Double_t maximum = 0;
		maximum = hbFSH->GetMaximum();
		h1->Fill(950-maximum/0.9);
		delete hbFSH;
		delete hbSSH;
		delete fft_backFSH;
		delete fft_backSSH;
	}
	double EFF = 1.0;
	for (int i = 800; i > 0; i--)
	{
		sum += h1->GetBinContent(i);
		result->SetBinContent(i,(eventnum-sum)/eventnum);
	}
	TCanvas *c1 = new TCanvas("c1","c1",1);
	result->Draw();
	TCanvas *c2 = new TCanvas("c2","c2",1);
	SigJoint->Draw("COL");
}