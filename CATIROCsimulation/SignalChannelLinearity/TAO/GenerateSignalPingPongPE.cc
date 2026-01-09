#define TimeLength 1008
#define TotalChannel 20
#define EventNumPerChannel 1000
#define MaxPEnum 100
int GenerateSignalPingPongPE( int PDFchannel)
{
    TFile* TimeFile = TFile::Open("output3.root");
    TFile* outfile = new TFile("PElinearity2.root","UPDATE");
    TTree *ChannelTree = (TTree*)TimeFile->Get("PMTtree");

    TH1* TimePDF[20];

    const int MaxEvent = 2550;
    int t_hit_num;
    int t_eventNum;
    double hit_num[20];
    int eventNum[20];
    double t_hittime[MaxEvent];

    ChannelTree->SetBranchAddress("eventNum",&t_eventNum);
    ChannelTree->SetBranchAddress("pmt_hit_num",&t_hit_num);
    ChannelTree->SetBranchAddress("pmt_hittime",t_hittime);

    Int_t nentries = ChannelTree->GetEntries();
    
    for (int i = 0; i<nentries; i++)
    {
        ChannelTree->GetEntry(i);
        TimePDF[i] = new TH1F(TString::Format("Channel %1d time pdf",t_eventNum),TString::Format("Channel %1d time pdf",t_eventNum),500,20,140);
        for (int j = 0; j<t_hit_num; j++)
        {
            TimePDF[i]->Fill(t_hittime[j]);
        }
        hit_num[i] = t_hit_num;
        eventNum[i] = t_eventNum;
    }


    int triggerDACu;
	triggerDACu = 850;
    int ChargeThreshold = 520;
	double RCslow = 50.0;
	double RCfast = 5.0;
	double CfHG = 0.25;
    int TimeWindow = 1000;
    int DelayTime = 43;
    // TRandom* rand = new TRandom3(4851);
    TRandom* rand = new TRandom3(0);
    Double_t MeanChargePing[MaxPEnum];
    Double_t MeanChargePong[MaxPEnum];
    Double_t MeanChargePingAndPong[MaxPEnum];
    Double_t SigNum[MaxPEnum];
    for (int i = 0; i < MaxPEnum; i++)
    {
        SigNum[i] = i+1;
    }
    
    Double_t StdDevPing[MaxPEnum];
    Double_t StdDevPong[MaxPEnum];
    Double_t StdDevPingAndPong[MaxPEnum];
    Double_t ChargePing = 0;
    Double_t ChargePong = 0;
   
    Double_t t;
    TFile* f = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/PMTsignal/joint.root");
    TH2 *joint = nullptr; 
    f->GetObject("joint",joint);
    TF1* func = new TF1("SpecModel", "landau",0 , TimeWindow);
    // int Maximum = h->GetMaximum();
    // TRandom3 r;
    // double x = 17.0*r.Rndm()+3.0;
    // double y = 5.0*r.Rndm()+2.0;
    // double z = Maximum*r.Rndm();
    // if (z<h->GetBinContent());
    Double_t x;
    Double_t y;
    TH1* ToChargePing;
    TH1* ToChargePong;
    TH1* ToChargePingAndPong;
    // TH1* SSHresult[MaxPEnum];
    // TH1* FSHresult[MaxPEnum];
    //TH1* SSHmodel;
    Double_t ModelCharge = 0;
    //for (int PDFchannel = 0; PDFchannel < 20; PDFchannel++)
    {
        cout<<"Channel = "<<PDFchannel<<endl;
        for (Int_t PEnum = 1; PEnum <= MaxPEnum; PEnum++)
        {
            // ToChargePing[PEnum-1]->SetName(TString::Format("Charge ping of %1d PE",PEnum));
            // ToChargePing[PEnum-1]->SetTitle(TString::Format("Charge ping of %1d PE",PEnum));
            // ToChargePong[PEnum-1]->SetName(TString::Format("Charge pong of %1d PE",PEnum));
            // ToChargePong[PEnum-1]->SetTitle(TString::Format("Charge pong of %1d PE",PEnum));
            ToChargePing = new TH1F(TString::Format("Charge ping of %1d PE",PEnum),TString::Format("Charge ping of %1d PE",PEnum),10240,0,10240);
            ToChargePong = new TH1F(TString::Format("Charge pong of %1d PE",PEnum),TString::Format("Charge pong of %1d PE",PEnum),10240,0,10240);
            ToChargePingAndPong = new TH1F(TString::Format("Charge ping and pong of %1d PE",PEnum),TString::Format("Charge ping and pong of %1d PE",PEnum),10240,0,10240);
            bool flag = false;
            
            for (int j = 0; j < 1000 ; j++)
            {
                TH1* tmp = new TH1F("signal","signal",TimeLength,0,8+TimeWindow);
                for ( Int_t i = 0; i<PEnum; i++)
                {
                    t=TimePDF[PDFchannel]->GetRandom(rand);
                    joint->GetRandom2(x,y);
                    func->SetParameters(x,t,y);
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
                // if (j==0)
                // {
                //     SSHmodel = (TH1*) hbSSH->Clone();
                //     SSHmodel->SetName("SSHmodel");
                // }


                // SSHresult[PEnum-1] = (TH1*) hbSSH->Clone();
                // SSHresult[PEnum-1]->SetName(TString::Format("SSH output of %1d PE",PEnum));
                // SSHresult[PEnum-1]->SetTitle(TString::Format("SSH output of %1d PE",PEnum));
                // FSHresult[PEnum-1] = (TH1*) hbFSH->Clone();
                // FSHresult[PEnum-1]->SetName(TString::Format("FSH output of %1d PE",PEnum));
                // FSHresult[PEnum-1]->SetTitle(TString::Format("FSH output of %1d PE",PEnum));
                bool ExchangePingPong = false;
                for (int i = 0; i<n; i++)
                {
                    if ((hbFSH->GetBinContent(i+1) >= 0.9*(950-triggerDACu)) && !ExchangePingPong )
                    {
                        ChargePing = (hbSSH->GetBinContent(i+DelayTime))*0.3051+75.8;
                        if (j==0)
                        {
                            ModelCharge = ChargePing;
                        } 
                        i += 90;
                        ExchangePingPong = true;
                    }
                    if ((hbFSH->GetBinContent(i+1) >= 0.9*(950-triggerDACu)) && ExchangePingPong )
                    {
                        ChargePong = (hbSSH->GetBinContent(i+DelayTime))*0.3051+75.8;
                        //ChargePong = ((hbSSH->GetBinContent(i+DelayTime))-(ChargePing/ModelCharge)*(SSHmodel->GetBinContent(i+DelayTime)))*0.3051+75.8;
                        // if (ChargePong<0) 
                        // {
                        // flag = true;
                        // cout<<"charge pong = "<<ChargePong<<"PEnum = "<<PEnum<<endl;
                        // }
                        break;
                    }
                }
                delete hbFSH;
                delete hbSSH;
                delete fft_backFSH;
                delete fft_backSSH;
                delete tmp;
                if (j != 0)
                {
                    ToChargePing->Fill(ChargePing);
                    if (ChargePong > 0) ToChargePong->Fill(ChargePong);
                    if (ChargePong < 0) ChargePong = 0;
                    if (ChargePong > 10240) ChargePong = 10240;
                    if (ChargePing > 10240) ChargePing = 10240;
                    ToChargePingAndPong->Fill( ChargePing + ChargePong);
                }            
                ChargePing = 0;
                ChargePong = 0;
                //if(flag) break;
            }
            MeanChargePing[PEnum-1] =ToChargePing->GetMean();
            MeanChargePong[PEnum-1] =ToChargePong->GetMean();
            MeanChargePingAndPong[PEnum-1] =ToChargePingAndPong->GetMean();
            StdDevPing[PEnum-1]=ToChargePing->GetStdDev();
            StdDevPong[PEnum-1]=ToChargePong->GetStdDev();
            StdDevPingAndPong[PEnum-1]=ToChargePingAndPong->GetStdDev();

            delete ToChargePing;
            delete ToChargePong;
            delete ToChargePingAndPong;
        }
    
        TGraph* ChargePing2SigNum = new TGraphErrors(MaxPEnum,SigNum,MeanChargePing,nullptr,StdDevPing);
        ChargePing2SigNum->SetName("Charge ping to signal number");
        ChargePing2SigNum->SetTitle("Charge ping to signal number");
        ChargePing2SigNum->GetXaxis()->SetTitle("PE number");
        ChargePing2SigNum->GetYaxis()->SetTitle("ADCu HG+10*LG");
        TGraph* ChargePong2SigNum = new TGraphErrors(MaxPEnum,SigNum,MeanChargePong,nullptr,StdDevPong);
        ChargePong2SigNum->SetName("Charge pong to signal number");
        ChargePong2SigNum->SetTitle("Charge pong to signal number");
        ChargePong2SigNum->GetXaxis()->SetTitle("PE number");
        ChargePong2SigNum->GetYaxis()->SetTitle("ADCu HG+10*LG");
        //Charge2SigNum -> Draw();
        TF1* ExpectLine = new TF1("expect line","[0]*x+[1]",0,MaxPEnum);
        ExpectLine->SetParameters(MeanChargePing[0]-75.8,75.8);
        TCanvas* c1 = new TCanvas("c1","c1",1);
        c1->Divide(2,1);
        c1->cd(1);
        ChargePing2SigNum->SetFillColor(4);
        ChargePing2SigNum->SetFillStyle(3002);
        ChargePing2SigNum->SetMarkerColor(4);
        ChargePing2SigNum->SetMarkerSize(0.8);
        ChargePing2SigNum->SetMarkerStyle(20);
        ChargePing2SigNum->Draw("a3");
        ChargePing2SigNum->Draw("same L X P");
        ExpectLine->SetLineStyle(2);
        ExpectLine->Draw("SAME");
        c1->cd(2);
        ChargePong2SigNum->SetFillColor(4);
        ChargePong2SigNum->SetFillStyle(3002);
        ChargePong2SigNum->SetMarkerColor(4);
        ChargePong2SigNum->SetMarkerSize(0.8);
        ChargePong2SigNum->SetMarkerStyle(20);
        ChargePong2SigNum->Draw("a3");
        ChargePong2SigNum->Draw("same L X P");
        // TCanvas* c2 = new TCanvas("c2","c2",1);
        // c2->Divide(4,3);
        // for (int i = 0; i < 10; i++)
        // {
        //     c2->cd(i+1);
        //     FSHresult[i]->Draw();
        // }
        // TCanvas* c3 = new TCanvas("c3","c3",1);
        // c3->Divide(4,3);
        // for (int i = 0; i < 10; i++)
        // {
        //     c3->cd(i+1);
        //     SSHresult[i]->Draw();
        // }
        // TCanvas* c4 = new TCanvas("c4","c4",1);
        // c4->Divide(4,3);
        // for (int i = 0; i < 10; i++)
        // {
        //     c4->cd(i+1);
        //     ToChargePing[i]->Draw();
        // }
        // TCanvas* c5 = new TCanvas("c5","c5",1);
        // c5->Divide(4,3);
        // for (int i = 0; i < 10; i++)
        // {
        //     c5->cd(i+1);
        //     ToChargePong[i]->Draw();
        //     ToChargePong[i]->GetXaxis()->SetTitle("ADCu HG+10*LG");
        // }
        TGraph* ChargePingAndPong2SigNum = new TGraphErrors(MaxPEnum,SigNum,MeanChargePingAndPong,nullptr,StdDevPingAndPong);
        ChargePingAndPong2SigNum->SetName("Charge ping and pong to signal number");
        ChargePingAndPong2SigNum->SetTitle("Charge ping and pong to signal number");
        ChargePingAndPong2SigNum->GetXaxis()->SetTitle("PE number");
        ChargePingAndPong2SigNum->GetYaxis()->SetTitle("ADCu HG+10*LG");
        //Charge2SigNum -> Draw();
        TF1* ExpectLine2 = new TF1("expect line 2","[0]*x+[1]",0,MaxPEnum);
        ExpectLine2->SetParameters(MeanChargePingAndPong[0]-75.8,75.8);
        TCanvas* c6 = new TCanvas("c6","c6",1);
        ChargePingAndPong2SigNum->SetFillColor(4);
        ChargePingAndPong2SigNum->SetFillStyle(3002);
        ChargePingAndPong2SigNum->SetMarkerColor(4);
        ChargePingAndPong2SigNum->SetMarkerSize(0.8);
        ChargePingAndPong2SigNum->SetMarkerStyle(20);
        ChargePingAndPong2SigNum->Draw("a3");
        ChargePingAndPong2SigNum->Draw("same L X P");
        ExpectLine2->SetLineStyle(2);
        ExpectLine2->Draw("SAME");
        Double_t biasPing[MaxPEnum];
        Double_t biasPingAndPong[MaxPEnum];
        Double_t SigmaBiasPing[MaxPEnum];
        Double_t SigmaBiasPingAndPong[MaxPEnum];
        for (int i = 0; i < MaxPEnum; i++)
        {
            biasPing[i]=MeanChargePing[i]/((i+1)*MeanChargePing[0]-i*75.8)-1;
            biasPingAndPong[i]=MeanChargePingAndPong[i]/((i+1)*MeanChargePingAndPong[0]-i*75.8)-1;
            SigmaBiasPing[i] = fabs(biasPing[i])*sqrt(pow(StdDevPing[i]/MeanChargePing[i],2)+pow(StdDevPing[0]/MeanChargePing[0],2));
            SigmaBiasPingAndPong[i] = fabs(biasPingAndPong[i])*sqrt(pow(StdDevPingAndPong[i]/MeanChargePingAndPong[i],2)+pow(StdDevPingAndPong[0]/MeanChargePingAndPong[0],2));
        }
        TGraph* biasPing2SigNum = new TGraphErrors(MaxPEnum,SigNum,biasPing,nullptr,SigmaBiasPing);
        TGraph* biasPingAndPong2SigNum = new TGraphErrors(MaxPEnum,SigNum,biasPingAndPong,nullptr,SigmaBiasPingAndPong);
        TCanvas* c7 = new TCanvas("c7","c7",1);
        c7->Divide(2,1);
        c7->cd(1);
        biasPing2SigNum->SetTitle("Charge bias ping");
        biasPing2SigNum->GetXaxis()->SetTitle("PE number");
        biasPing2SigNum->SetFillColor(4);
        biasPing2SigNum->SetFillStyle(3002);
        biasPing2SigNum->SetMarkerColor(4);
        biasPing2SigNum->SetMarkerSize(0.8);
        biasPing2SigNum->SetMarkerStyle(20);
        biasPing2SigNum->Draw("a3");
        biasPing2SigNum->Draw("same L X P");
        c7->cd(2);
        biasPingAndPong2SigNum->SetTitle("Charge bias ping and pong");
        biasPingAndPong2SigNum->GetXaxis()->SetTitle("PE number");
        biasPingAndPong2SigNum->SetFillColor(4);
        biasPingAndPong2SigNum->SetFillStyle(3002);
        biasPingAndPong2SigNum->SetMarkerColor(4);
        biasPingAndPong2SigNum->SetMarkerSize(0.8);
        biasPingAndPong2SigNum->SetMarkerStyle(20);
        biasPingAndPong2SigNum->Draw("a3");
        biasPingAndPong2SigNum->Draw("same L X P");



        outfile->cd();
        outfile->mkdir(TString::Format("Channel %1d ping %1d PE",eventNum[PDFchannel],(int)hit_num[PDFchannel]));
        outfile->cd(TString::Format("Channel %1d ping %1d PE",eventNum[PDFchannel],(int)hit_num[PDFchannel]));
        c1->Write();
        c6->Write();
        c7->Write();
        TimePDF[PDFchannel]->Write();

        c1->Close();
        c6->Close();
        c7->Close();

        delete ChargePing2SigNum;
        delete ChargePong2SigNum;
        delete ChargePingAndPong2SigNum;
        delete biasPing2SigNum;
        delete biasPingAndPong2SigNum;
        delete ExpectLine;
        delete ExpectLine2;
        return 0;
    }
}