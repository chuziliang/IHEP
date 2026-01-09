void DrawPlots()
{
    TFile *ifile = new TFile ("./result/20220223_before_transport_HVOFF.root","read");
    TH2 *ohistPing = new TH2F("charges HG ping","charges HG ping",128,0,127,1024,0,1024);
    TH2 *ohistPong = new TH2F("charges HG pong","charges HG pong",128,0,127,1024,0,1024);
    TH2 *ohistPingLG = new TH2F("charges LG ping","charges LG ping",128,0,127,1024,0,1024);
    TH2 *ohistPongLG = new TH2F("charges LG pong","charges LG pong",128,0,127,1024,0,1024);

    int TotalEntries = 0;
    int AbnormalEvents = 0;
    for(int chn = 0; chn<128; chn++)
    {
        TH1 *tmp1 = nullptr;
        TH1 *tmp2 = nullptr;
        TH1 *tmp3 = nullptr;
        TH1 *tmp4 = nullptr;
        tmp1 = (TH1*)ifile->Get(TString::Format("HighGain/Ping_Chn%1d_HG",chn));
        tmp2 = (TH1*)ifile->Get(TString::Format("HighGain/Pong_Chn%1d_HG",chn));
        tmp3 = (TH1*)ifile->Get(TString::Format("LowGain/Ping_Chn%1d_LG",chn));
        tmp4 = (TH1*)ifile->Get(TString::Format("LowGain/Pong_Chn%1d_LG",chn));
        for (int i = 0; i < 1024; i++)
        {
            ohistPing->SetBinContent(chn+1,i+1,tmp1->GetBinContent(i+1));
            ohistPong->SetBinContent(chn+1,i+1,tmp2->GetBinContent(i+1));
            ohistPingLG->SetBinContent(chn+1,i+1,tmp3->GetBinContent(i+1));
            ohistPongLG->SetBinContent(chn+1,i+1,tmp4->GetBinContent(i+1));

            if (i >= 200)
            {
                AbnormalEvents += tmp1->GetBinContent(i+1);
                AbnormalEvents += tmp2->GetBinContent(i+1);
            }
            AbnormalEvents += tmp3->GetBinContent(i+1);
            AbnormalEvents += tmp4->GetBinContent(i+1);
        }

        TotalEntries += (tmp1->GetEntries() + tmp2->GetEntries() + tmp3->GetEntries() + tmp4->GetEntries());

    }

	double AbnormalRate;
    AbnormalRate = (double) AbnormalEvents/TotalEntries;

    cout<<"Abnormal date rate = "<<AbnormalRate<<endl;

    //ohistPing->GetYaxis()->SetRangeUser(0,500);
    //ohistPong->GetYaxis()->SetRangeUser(0,500);
    
    TProfile *hprof = ohistPing->TH2::ProfileX();
    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->Divide(2,1);
    c1->cd(1);
    gPad->SetLogz();
    ohistPing->GetXaxis()->SetTitle("Channel ID");
    gStyle->SetOptStat(kFALSE);
    ohistPing->Draw("COLZ");
    c1->Update();
    TPaletteAxis *palette1 = (TPaletteAxis*)ohistPing->GetListOfFunctions()->FindObject("palette");
    if (palette1 != nullptr)
    {
        palette1->SetX1NDC(0.902);
        palette1->SetX2NDC(0.932);
        c1->Modified();
        c1->Update();
    }
    c1->cd(2);
    gPad->SetLogz();
    ohistPong->GetXaxis()->SetTitle("Channel ID");
    gStyle->SetOptStat(kFALSE);
    ohistPong->Draw("COLZ");
    c1->Update();
    TPaletteAxis *palette2 = (TPaletteAxis*)ohistPong->GetListOfFunctions()->FindObject("palette");
    if (palette2 != nullptr)
    {
        palette2->SetX1NDC(0.902);
        palette2->SetX2NDC(0.932);
        c1->Modified();
        c1->Update();
    }

    TCanvas *c2 = new TCanvas("c2","c2",1);
    c2->Divide(2,1);
    c2->cd(1);
    gPad->SetLogz();
    ohistPingLG->GetXaxis()->SetTitle("Channel ID");
    gStyle->SetOptStat(kFALSE);
    ohistPingLG->Draw("COLZ");
    c2->Update();
    TPaletteAxis *palette3 = (TPaletteAxis*)ohistPingLG->GetListOfFunctions()->FindObject("palette");
    if (palette3 != nullptr)
    {
        palette3->SetX1NDC(0.902);
        palette3->SetX2NDC(0.932);
        c2->Modified();
        c2->Update();
    }
    
    

    c2->cd(2);
    gPad->SetLogz();
    ohistPongLG->GetXaxis()->SetTitle("Channel ID");
    gStyle->SetOptStat(kFALSE);
    ohistPongLG->Draw("COLZ");
    c2->Update();
    TPaletteAxis *palette4 = (TPaletteAxis*)ohistPongLG->GetListOfFunctions()->FindObject("palette");
    if (palette4 != nullptr)
    {
        palette4->SetX1NDC(0.902);
        palette4->SetX2NDC(0.932);
        c2->Modified();
        c2->Update();
    }

    TCanvas *c3 = new TCanvas("c3","c3",1);
    c3->cd();
    hprof->Draw();
}
