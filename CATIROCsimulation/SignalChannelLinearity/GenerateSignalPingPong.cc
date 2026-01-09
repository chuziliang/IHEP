#define TimeLength 1008
void GenerateSignalPingPong()
{
    int triggerDACu;
	triggerDACu = 650;
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
    int TimeWindow = 1000;
    // TRandom* rand = new TRandom3(4851);
    TRandom* rand = new TRandom3(0);
    Double_t MeanChargePing[10];
    Double_t MeanChargePong[10];
    Double_t SigNum[10]={1,2,3,4,5,6,7,8,9,10};
    Double_t StdDevPing[10];
    Double_t StdDevPong[10];
    Double_t ChargePing = 0;
    Double_t ChargePong = 0;
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
    TH1* SSHresult[10];
    TH1* FSHresult[10];
    TH1* SSHmodel;
    Double_t ModelCharge = 0;
    for (Int_t PEnum = 1; PEnum <= 10; PEnum++)
    {
        TH1* ToChargePing = new TH1F("ToChargePing","ToChargePing",1000,0,1000);
        TH1* ToChargePong = new TH1F("ToChargePong","ToChargePong",1000,0,1000);
        for (int j = 0; j < 1 ; j++)
        {
            TH1* tmp = new TH1F("signal","signal",TimeLength,0,8+TimeWindow);
            for ( Int_t i = 0; i<PEnum; i++)
            {
                t=TimePdf->GetRandom(rand);
                joint->GetRandom2(x,y);
                func->SetParameters(x,t+100,y);
                func->SetNpx(TimeWindow);
                tmp->Add(func);
            }

            TH1 *hr = 0;
            TH1 *hi = 0;
            hr = tmp->FFT(hr,"RE");
            hi = tmp->FFT(hi,"IM");
            Double_t *re_full = new Double_t[TimeLength];
            Double_t *im_full = new Double_t[TimeLength];
            Double_t *re_backFSH = new Double_t[TimeLength];
            Double_t *im_backFSH = new Double_t[TimeLength];
            Double_t *re_backSSH = new Double_t[TimeLength];
            Double_t *im_backSSH = new Double_t[TimeLength];
            Int_t n =TimeLength;
            complex <double> backFSH = 0;
            complex <double> backSSH = 0;
            complex <double> ResponseSlow = 0;
            complex <double> ResponseFast = 0;
            for(Int_t i=0;i<n;i++)
            {
                re_full[i] = (hr->GetBinContent(i+1))/(sqrt(n)*50);
                im_full[i] = (hi->GetBinContent(i+1))/(sqrt(n)*50);
                ResponseSlow = complex<double>(0.0,TMath::Pi()*i*RCslow/(double)TimeLength)*RCslow/(CfHG*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/(double)TimeLength)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/(double)TimeLength)*complex<double>(1.0,2.0*TMath::Pi()*i*RCslow/(double)TimeLength));
                ResponseFast = RCfast/(CfHG*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/(double)TimeLength)*complex<double>(1.0,2.0*TMath::Pi()*i*RCfast/(double)TimeLength));
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
            hbFSH->Scale(1.0/sqrt((double)TimeLength));
            hbFSH->SetName("hbFSH");
            TH1 *hbSSH = 0;
            hbSSH = TH1::TransformHisto(fft_backSSH,hbSSH,"Re");
            hbSSH->Scale(1.0/sqrt((double)TimeLength));
            if (j==0 && PEnum==1)
            {
                SSHmodel = (TH1*) hbSSH->Clone();
                SSHmodel->SetName("SSHmodel");
            }
            SSHresult[PEnum-1] = (TH1*) hbSSH->Clone();
            SSHresult[PEnum-1]->SetName(TString::Format("SSH output of %1d PE",PEnum));
            SSHresult[PEnum-1]->SetTitle(TString::Format("SSH output of %1d PE",PEnum));
            FSHresult[PEnum-1] = (TH1*) hbFSH->Clone();
            FSHresult[PEnum-1]->SetName(TString::Format("FSH output of %1d PE",PEnum));
            FSHresult[PEnum-1]->SetTitle(TString::Format("FSH output of %1d PE",PEnum));
            bool ExchangePingPong = false;
            for (int i = 0; i<n; i++)
            {
                if ((hbFSH->GetBinContent(i+1) >= 0.9*(950-triggerDACu)) && !ExchangePingPong )
                {
                    ChargePing = (hbSSH->GetBinContent(i+27))*0.3854+78.59;
                    if (j==0 && PEnum==1)
                    {
                        ModelCharge = ChargePing;
                    } 
                    i += 90;
                    ExchangePingPong = true;
                }
                if ((hbFSH->GetBinContent(i+1) >= 0.9*(950-triggerDACu)) && ExchangePingPong )
                {
                    ChargePong = ((hbSSH->GetBinContent(i+27))-(ChargePing/ModelCharge)*(SSHmodel->GetBinContent(i+27)))*0.3854+78.59;
                    break;
                }
            }
            delete hbFSH;
            delete hbSSH;
            delete fft_backFSH;
            delete fft_backSSH;
            delete tmp;
            ToChargePing->Fill(ChargePing);
            ToChargePong->Fill(ChargePong);
            ChargePing = 0;
            ChargePong = 0;
        }
        MeanChargePing[PEnum-1] =ToChargePing->GetMean();
        MeanChargePong[PEnum-1] =ToChargePong->GetMean();
        StdDevPing[PEnum-1]=ToChargePing->GetStdDev();
        StdDevPong[PEnum-1]=ToChargePong->GetStdDev();
        delete ToChargePing;
        delete ToChargePong;
    }
    TGraph* ChargePing2SigNum = new TGraphErrors(10,SigNum,MeanChargePing,nullptr,StdDevPing);
    ChargePing2SigNum->SetName("Charge ping to signal number");
    ChargePing2SigNum->SetTitle("Charge ping to signal number");
    TGraph* ChargePong2SigNum = new TGraphErrors(10,SigNum,MeanChargePong,nullptr,StdDevPong);
    ChargePong2SigNum->SetName("Charge pong to signal number");
    ChargePong2SigNum->SetTitle("Charge pong to signal number");
    //Charge2SigNum -> Draw();
    TF1* ExpectLine = new TF1("expect line","[0]*x+[1]",0,10);
    ExpectLine->SetParameters(MeanChargePing[0]-78.59,78.59);
    TCanvas* c1 = new TCanvas("c1","c1",1);
    c1->Divide(2,1);
    c1->cd(1);
    ChargePing2SigNum->Draw();
    ExpectLine->Draw("SAME");
    c1->cd(2);
    ChargePong2SigNum->Draw();
    TCanvas* c2 = new TCanvas("c2","c2",1);
    c2->Divide(4,3);
    for (int i = 0; i < 10; i++)
    {
        c2->cd(i+1);
        FSHresult[i]->Draw();
    }
    TCanvas* c3 = new TCanvas("c3","c3",1);
    c3->Divide(4,3);
    for (int i = 0; i < 10; i++)
    {
        c3->cd(i+1);
        SSHresult[i]->Draw();
    }
}