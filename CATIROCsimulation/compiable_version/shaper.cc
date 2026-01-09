void shaper(TH1* &wave, TH1* &FSH, TH1* &SSH, double FSHscale = -18.919, double SSHscale = 27.6416)
{

    double RCslow = 50.0;
	double RCfast = 5.0;
    int nbins = wave->GetNbinsX();
    double tottime = wave->GetXaxis()->GetXmax();

    TH1 *hr = 0;
    TH1 *hi = 0;
    hr = wave->FFT(hr,"RE");
    hr->SetName("hr");
    hi = wave->FFT(hi,"IM");
    hi->SetName("hi");
    Double_t *re_full = new Double_t[nbins];
    Double_t *im_full = new Double_t[nbins];
    Double_t *re_backFSH = new Double_t[nbins];
    Double_t *im_backFSH = new Double_t[nbins];
    Double_t *re_backSSH = new Double_t[nbins];
    Double_t *im_backSSH = new Double_t[nbins];
    complex <double> backFSH = 0;
    complex <double> backSSH = 0;
    complex <double> ResponseSlow = 0;
    complex <double> ResponseFast = 0;
    for(Int_t i=0;i<nbins;i++)
    {
        re_full[i] = (hr->GetBinContent(i+1))/(sqrt(nbins));
        im_full[i] = (hi->GetBinContent(i+1))/(sqrt(nbins));
        ResponseSlow = 20.0*complex<double>(0.0,2.0*TMath::Pi()*i*RCslow/tottime)/(complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/tottime)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/tottime)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/tottime));
        ResponseFast = 20.0*complex<double>(0.0,2.0*TMath::Pi()*i*RCfast/tottime)/(complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/tottime)*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/tottime));
        backFSH = ResponseFast*complex<double>(re_full[i],im_full[i]);
        backSSH = ResponseSlow*complex<double>(re_full[i],im_full[i]);
        re_backFSH[i] = backFSH.real();
        im_backFSH[i] = backFSH.imag();
        re_backSSH[i] = backSSH.real();
        im_backSSH[i] = backSSH.imag();
    }
    
    TVirtualFFT *fft_backFSH = TVirtualFFT::FFT(1,&nbins,"C2R M K");
    fft_backFSH->SetPointsComplex(re_backFSH,im_backFSH);
    fft_backFSH->Transform();
    TVirtualFFT *fft_backSSH = TVirtualFFT::FFT(1,&nbins,"C2R M K");
    fft_backSSH->SetPointsComplex(re_backSSH,im_backSSH);
    fft_backSSH->Transform();

    FSH = TH1::TransformHisto(fft_backFSH,FSH,"Re");
    FSH->Scale(1.0/sqrt(nbins));
    //FSH->Scale(-18.9193);//20230302，FSHcalib，加基线991得到以DACu为单位的FSH波形。
    FSH->Scale(FSHscale);
    FSH->SetBins(FSH->GetNbinsX(),0,tottime);
    FSH->SetName("FSH");

    SSH = TH1::TransformHisto(fft_backSSH,SSH,"Re");
    SSH->Scale(1.0/sqrt(nbins));
    //SSH->Scale(27.6416);//20230302，SSHcalib，加基线66.9得到以ADCu为单位的SSH波形。
    SSH->Scale(SSHscale);
    SSH->SetBins(SSH->GetNbinsX(),0,tottime);
    SSH->SetName("SSH");


    delete hr;
    delete hi;
    delete [] re_full;
    delete [] im_full;
    delete [] re_backFSH;
    delete [] im_backFSH;
    delete [] re_backSSH;
    delete [] im_backSSH;
    delete fft_backFSH;
    delete fft_backSSH;

}
