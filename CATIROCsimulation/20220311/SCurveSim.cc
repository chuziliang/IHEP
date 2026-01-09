//这个脚本用于分析模拟的高斯信号，对比论文中的阈值与电荷的关系（SCurve），确认FSH的模拟是否正确。

#define triggerDACu 650
#define DelayTime 42

void SCurveSim()
{
	
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
	double ChargeSSH = 0.0;

	TFile *ifile = TFile::Open("./TrapezoidalWave.root");
    double PE[10];
    double DACu[10];
    double FSHDACu = 0;
	TH1 *output = 0;

	for(int j = 0;j<10;j++)
	{
		
		TH1* tmp = nullptr;
    	tmp = (TH1*)ifile->Get(TString::Format("Signal %1.1fPE",0.1*(j+1)));

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
			ResponseFast = 20.0*complex<double>(0.0,2.0*TMath::Pi()*i*RCfast/1008.0)/(complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/1008.0)*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/1008.0));
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
		
        
        FSHDACu = 950 - 1.1111*hbFSH->GetBinContent(hbFSH->GetMaximumBin());

        PE[j] = 0.1*(j+1);
        DACu[j] = FSHDACu;
		if (j+1 == 10)
		{
			output = (TH1*)hbFSH->Clone();
		}
		delete hbFSH;
		delete hbSSH;
		delete fft_backFSH;
		delete fft_backSSH;
		delete tmp;
	}
	TCanvas *c1 = new TCanvas("c1","c1",1);
	c1->cd();
    TGraph *PE2DACu = new TGraph(10,DACu,PE);
	PE2DACu->Draw();
	TCanvas *c2 = new TCanvas("c2","c2",1);
	c2->cd();
	output->Draw();
}