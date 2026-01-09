void GenerateSignal()
{
    int triggerDACu;
	triggerDACu = 650;
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
    int TimeWindow = 1000;
    Double_t MeanCharge[10];
    Double_t SigNum[10]={1,2,3,4,5,6,7,8,9,10};
    Double_t StdDev[10];
    Double_t Charge;
    Double_t t1,t2,t3;
    Double_t p1,p2,p3;
    t1=4.93,t2=20.6,t3=190;
    p1=0.799,p2=0.171,p3=0.03;
    Double_t t;
    TFile* f = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/PMTsignal/joint.root");
    TH2 *joint = nullptr; 
    f->GetObject("joint",joint);
    TF1* func = new TF1("SpecModel", "landau",0 , TimeWindow);
    TF1* TimePdf = new TF1("TimePdf","[3]*exp(-x/[0])+[4]*exp(-x/[1])+[5]*exp(-x/[2])", 0, TimeWindow);
    TimePdf ->SetParameters(t1,t2,t3,p1/t1,p2/t2,p3/t3);
    // int Maximum = h->GetMaximum();
    // TRandom3 r;
    // double x = 17.0*r.Rndm()+3.0;
    // double y = 5.0*r.Rndm()+2.0;
    // double z = Maximum*r.Rndm();
    // if (z<h->GetBinContent());
    Double_t x;
    Double_t y;
    for (Int_t PEnum = 1; PEnum <= 10; PEnum++)
    {
        TH1* ToCharge = new TH1F("ToCharge","ToCharge",1000,0,1000);
        for (int j = 0; j < 100 ; j++)
        {
            TH1* tmp = new TH1F("signal","signal",1008,0,8+TimeWindow);
            for ( Int_t i = 0; i<PEnum; i++)
            {
                t=TimePdf->GetRandom();
                joint->GetRandom2(x,y);
                func->SetParameters(x,t+100,y);
                func->SetNpx(TimeWindow);
                tmp->Add(func);
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
                    Charge = (hbSSH->GetBinContent(i+27))*0.3854+78.59;
                    break;
                }
            }
            delete hbFSH;
            delete hbSSH;
            delete fft_backFSH;
            delete fft_backSSH;
            delete tmp;
            ToCharge->Fill(Charge);
        }
        MeanCharge[PEnum-1] =ToCharge->GetMean();
        StdDev[PEnum-1]=ToCharge->GetStdDev();
        delete ToCharge;
    }
    TGraph* Charge2SigNum = new TGraphErrors(10,SigNum,MeanCharge,nullptr,StdDev);
    Charge2SigNum -> Draw();
}