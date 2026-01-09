//这个脚本用于确定模拟的SSH输出值，与电荷间的对应关系。
//[ADCu] = 1/7.9[fC]  + 66.9(+- 0.3)

//2022年3月15日，结果为 [ADCu] = 0.26761*[mV] + 77.3728

constexpr int triggerDACu = 650;
constexpr int DelayTime = 42;
constexpr int length = 1008;//总时长，单位ns
constexpr int nbins = 1008;//总的分bin数
constexpr double step = length/nbins;//每一步的时长

void Charge2SSHDACu()
{
	
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
	double ChargeSSH = 0.0;

	TFile *ifile = TFile::Open("./Signals.root");
	TH1F *result = new TH1F("Charge","Charge",100,0.0,2.0);
	TH1F *result2 = new TH1F("ChargeSSH","ChargeSSH",1000,0.0,500);
	TH1F *result3 = new TH1F("Charge trigger","Charge trigger",100,0.0,2.0);
	TH2 *Charge2DACu = new TH2D("Charge-SSH","Charge-SSH",800,0,800,500,0,2.0); 

	for(int j = 0;j<10000;j++)
	{
		
		if ( j%100 == 0 )
		{
			cout<<j/100<<endl;
		}
		double charge = 0;
		TH1* tmp = nullptr;
    	tmp = (TH1*)ifile->Get(TString::Format("%1d/signal %1d",j/1000,j));

		double SignalMaxBin =tmp->GetMaximumBin();

		for (int i = SignalMaxBin - 30; i < SignalMaxBin + 70; i++)
        {
            charge += tmp->GetBinContent(i) / 50;
        }

		TH1 *hr = 0;
		TH1 *hi = 0;
		hr = tmp->FFT(hr,"RE");
		hi = tmp->FFT(hi,"IM");
		Double_t *re_full = new Double_t[nbins];
		Double_t *im_full = new Double_t[nbins];
		Double_t *re_backFSH = new Double_t[nbins];
		Double_t *im_backFSH = new Double_t[nbins];
		Double_t *re_backSSH = new Double_t[nbins];
		Double_t *im_backSSH = new Double_t[nbins];
		Int_t n =1008;
		complex <double> backFSH = 0;
		complex <double> backSSH = 0;
		complex <double> ResponseSlow = 0;
		complex <double> ResponseFast = 0;
		for(Int_t i=0;i<nbins;i++)
		{
			re_full[i] = (hr->GetBinContent(i+1))/(sqrt(nbins)*50);
			im_full[i] = (hi->GetBinContent(i+1))/(sqrt(nbins)*50);
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
		TVirtualFFT *fft_backFSH = TVirtualFFT::FFT(1,&nbins,"C2R M K");
		fft_backFSH->SetPointsComplex(re_backFSH,im_backFSH);
		fft_backFSH->Transform();
		TVirtualFFT *fft_backSSH = TVirtualFFT::FFT(1,&nbins,"C2R M K");
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
			if ((hbFSH->GetBinContent(i+1) >= 0.9*(950-triggerDACu)) && i < nbins-DelayTime)
			{
				ChargeSSH = (hbSSH->GetBinContent(i+DelayTime))*0.26761+77.3728;//*0.3854+78.59;
				result2->Fill(ChargeSSH);
				result3->Fill(charge);
				Charge2DACu->Fill(ChargeSSH,charge);
				// if (j ==1 )
				// {
				// 	cout<<"Bin num "<<i<<endl;
				// 	cout<<"charge = "<<charge<<endl;
				// }
				break;
			}
		}
		// if (j ==1 )
		// {
		// 	TCanvas *c2 = new TCanvas("c2","c2",800,400);
		// 	c2->Divide(2,3);
		// 	c2->cd(1);
		// 	tmp->Draw();
		// 	c2->cd(2);
		// 	hbSSH->Draw();
		// 	c2->cd(3);
		// 	hbFSH->Draw();
		// 	break;
		// }
		result->Fill(charge);
		delete hbFSH;
		delete hbSSH;
		delete fft_backFSH;
		delete fft_backSSH;
		delete tmp;
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
	//c1->cd(3);
	//result3->Draw();
	c1->cd(3);
	result->Draw();
	cout<<" [ADCu] = "<<1000*func->GetParameter(0)/7.9<<"*[mV] + "<<66.9+1000*func->GetParameter(1)/7.9<<endl;
	//TFile *ofile = new TFile("effiency_fit_data.root","recreate");
	//result2->Write("effiency_fit_data");

}