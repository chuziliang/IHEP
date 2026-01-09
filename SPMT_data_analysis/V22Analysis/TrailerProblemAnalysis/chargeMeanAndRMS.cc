void chargeMeanAndRMS()
{
    TFile *inputfile1 = TFile::Open("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/TrailerProblemAnalysis/test.root","read");
    TTree *EventData1 = (TTree*)inputfile1->Get("EventData");

    TFile *inputfile2 = TFile::Open("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/TrailerProblemAnalysis/test2.root","read");
    TTree *EventData2 = (TTree*)inputfile2->Get("EventData");

    TH1 *HGPing1[128];
    TH1 *HGPong1[128];
    for (int i = 0; i < 128; i++)
    {
        TString name_Histo_Charge_ping_HG1 = "Histo_Charge_HG_1_" + TString(Form("%i",i)) + "_ping";
		TString title_Histo_Charge_ping_HG1 = "HG Charges in channel 1 " + TString(Form("%i",i)) + " (ping)";
        TString name_Histo_Charge_pong_HG1 = "Histo_Charge_HG_1_" + TString(Form("%i",i)) + "_pong";
		TString title_Histo_Charge_pong_HG1 = "HG Charges in channel 1 " + TString(Form("%i",i)) + " (pong)";
        HGPing1[i] = new TH1F(name_Histo_Charge_ping_HG1,title_Histo_Charge_ping_HG1,1024,0,1024);
        HGPong1[i] = new TH1F(name_Histo_Charge_pong_HG1,title_Histo_Charge_pong_HG1,1024,0,1024);

    }

    TH1 *HGPing2[128];
    TH1 *HGPong2[128];
    for (int i = 0; i < 128; i++)
    {
        TString name_Histo_Charge_ping_HG2 = "Histo_Charge_HG_2_" + TString(Form("%i",i)) + "_ping";
		TString title_Histo_Charge_ping_HG2 = "HG Charges in channel 2 " + TString(Form("%i",i)) + " (ping)";
        TString name_Histo_Charge_pong_HG2 = "Histo_Charge_HG_2_" + TString(Form("%i",i)) + "_pong";
		TString title_Histo_Charge_pong_HG2 = "HG Charges in channel 2 " + TString(Form("%i",i)) + " (pong)";
        HGPing2[i] = new TH1F(name_Histo_Charge_ping_HG2,title_Histo_Charge_ping_HG2,1024,0,1024);
        HGPong2[i] = new TH1F(name_Histo_Charge_pong_HG2,title_Histo_Charge_pong_HG2,1024,0,1024);

    }
    

    UInt_t chargeValue1;
    UInt_t chargeValue2;
    UInt_t blockID1;
    UInt_t blockID2;
    UInt_t blockChannelNumber1;
    UInt_t blockChannelNumber2;
    UInt_t EventCounter1;
    UInt_t EventCounter2;
    UInt_t ABCChannelNumber1;
    UInt_t ABCChannelNumber2;



    EventData1->SetBranchAddress("ChargeValue",&chargeValue1);
    EventData2->SetBranchAddress("ChargeValue",&chargeValue2);

    EventData1->SetBranchAddress("BlockId",&blockID1);
    EventData2->SetBranchAddress("BlockId",&blockID2);

    EventData1->SetBranchAddress("ChannelNumber",&blockChannelNumber1);
    EventData2->SetBranchAddress("ChannelNumber",&blockChannelNumber2);

    EventData1->SetBranchAddress("EventCounter",&EventCounter1);
    EventData2->SetBranchAddress("EventCounter",&EventCounter2);

    for(int i = 0; i < EventData1->GetEntries(); i++)
    {
        EventData1->GetEntry(i);
        if (EventCounter1%2 == 0)
        {
            HGPing1[blockID1*8 + blockChannelNumber1]->Fill(chargeValue1);
        }
        else
        {
            HGPong1[blockID1*8 + blockChannelNumber1]->Fill(chargeValue1);
        }
        
        
    }

    for(int i = 0; i < EventData2->GetEntries(); i++)
    {
        EventData2->GetEntry(i);
        if (EventCounter2%2 == 0)
        {
            HGPing2[blockID2*8 + blockChannelNumber2]->Fill(chargeValue2);
        }
        else
        {
            HGPong2[blockID2*8 + blockChannelNumber2]->Fill(chargeValue2);
        }
    }

    TH1 *MeanPing1 = new TH1F("Mean Ping total","Mean Ping",128,0,128);
    TH1 *MeanPong1 = new TH1F("Mean Pong total","Mean Pong",128,0,128);
    TH1 *MeanPing2 = new TH1F("Mean Ping Problematic blocks","Mean Ping Problematic blocks",128,0,128);
    TH1 *MeanPong2 = new TH1F("Mean Pong Problematic blocks","Mean Pong Problematic blocks",128,0,128);

    MeanPing1->GetYaxis()->SetRangeUser(50,120);
    MeanPong1->GetYaxis()->SetRangeUser(50,120);
    MeanPing1->GetXaxis()->SetTitle("ABC channel");
    MeanPong1->GetXaxis()->SetTitle("ABC channel");
    MeanPing1->GetYaxis()->SetTitle("ADCu");
    MeanPong1->GetYaxis()->SetTitle("ADCu");

    TH1 *RMSPing1 = new TH1F("RMS Ping total","RMS Ping",128,0,128);
    TH1 *RMSPong1 = new TH1F("RMS Pong total","RMS Pong",128,0,128);
    TH1 *RMSPing2 = new TH1F("RMS Ping Problematic blocks","RMS Ping Problematic blocks",128,0,128);
    TH1 *RMSPong2 = new TH1F("RMS Pong Problematic blocks","RMS Pong Problematic blocks",128,0,128);

    RMSPing1->GetYaxis()->SetRangeUser(1,6);
    RMSPong1->GetYaxis()->SetRangeUser(1,6);
    RMSPing1->GetXaxis()->SetTitle("ABC channel");
    RMSPong1->GetXaxis()->SetTitle("ABC channel");


    for (int i = 0; i < 128; i++)
    {
        MeanPing1->SetBinContent(i+1,HGPing1[i]->GetMean());
        MeanPong1->SetBinContent(i+1,HGPong1[i]->GetMean());
        MeanPing2->SetBinContent(i+1,HGPing2[i]->GetMean());
        MeanPong2->SetBinContent(i+1,HGPong2[i]->GetMean());

        RMSPing1->SetBinContent(i+1,HGPing1[i]->GetRMS());
        RMSPong1->SetBinContent(i+1,HGPong1[i]->GetRMS());
        RMSPing2->SetBinContent(i+1,HGPing2[i]->GetRMS());
        RMSPong2->SetBinContent(i+1,HGPong2[i]->GetRMS());
    }
    

    TCanvas *cMeanPing = new TCanvas("cMeanPing","cMeanPing",1);
    MeanPing1->SetLineColor(1);
    MeanPing2->SetLineColor(2);
    MeanPing1->Draw();

    cMeanPing->Update();
    TPaveStats* stats1MeanPing = (TPaveStats*)cMeanPing->GetPrimitive("stats");
	double y1MeanPing = stats1MeanPing->GetY1NDC();
	double y2MeanPing = stats1MeanPing->GetY2NDC();
	stats1MeanPing->SetName("Total MeanPing");
	stats1MeanPing->SetY1NDC(y1MeanPing);    //set new y start position
	stats1MeanPing->SetY2NDC(y2MeanPing);    //set new y end position
    stats1MeanPing->SetX1NDC(0.67);
    stats1MeanPing->SetX2NDC(0.87);
	cMeanPing->Update();

    MeanPing2->Draw("sames HIST");
    cMeanPing->Update();
    TPaveStats* stats2MeanPing = (TPaveStats*)cMeanPing->GetPrimitive("stats");
	double newy1MeanPing = 2 * y1MeanPing - y2MeanPing;   // new (lower) y start position of stats box
	double newy2MeanPing = y1MeanPing;            // new (upper) y start position of stats box
	stats2MeanPing->SetName("Problematic blocks MeanPing");
	stats2MeanPing->SetY1NDC(newy1MeanPing);    //set new y start position
	stats2MeanPing->SetY2NDC(newy2MeanPing);    //set new y end position
    stats2MeanPing->SetX1NDC(0.67);
    stats2MeanPing->SetX2NDC(0.87);
    stats2MeanPing->SetLineColor(kRed);
    stats2MeanPing->SetTextColor(kRed);
	cMeanPing->Update();

    TCanvas *cMeanPong = new TCanvas("cMeanPong","cMeanPong",1);
    MeanPong1->SetLineColor(1);
    MeanPong2->SetLineColor(2);
    MeanPong1->Draw();

    cMeanPong->Update();
    TPaveStats* stats1MeanPong = (TPaveStats*)cMeanPong->GetPrimitive("stats");
	double y1MeanPong = stats1MeanPong->GetY1NDC();
	double y2MeanPong = stats1MeanPong->GetY2NDC();
	stats1MeanPong->SetName("Total MeanPong");
	stats1MeanPong->SetY1NDC(y1MeanPong);    //set new y start position
	stats1MeanPong->SetY2NDC(y2MeanPong);    //set new y end position
    stats1MeanPong->SetX1NDC(0.67);
    stats1MeanPong->SetX2NDC(0.87);
	cMeanPong->Update();

    MeanPong2->Draw("sames HIST");
    cMeanPong->Update();
    TPaveStats* stats2MeanPong = (TPaveStats*)cMeanPong->GetPrimitive("stats");
	double newy1MeanPong = 2 * y1MeanPong - y2MeanPong;   // new (lower) y start position of stats box
	double newy2MeanPong = y1MeanPong;            // new (upper) y start position of stats box
	stats2MeanPong->SetName("Problematic blocks MeanPong");
	stats2MeanPong->SetY1NDC(newy1MeanPong);    //set new y start position
	stats2MeanPong->SetY2NDC(newy2MeanPong);    //set new y end position
    stats2MeanPong->SetX1NDC(0.67);
    stats2MeanPong->SetX2NDC(0.87);
    stats2MeanPong->SetLineColor(kRed);
    stats2MeanPong->SetTextColor(kRed);
	cMeanPong->Update();


    TCanvas *cRMSPing = new TCanvas("cRMSPing","cRMSPing",1);
    RMSPing1->SetLineColor(1);
    RMSPing2->SetLineColor(2);
    RMSPing1->Draw();

    cRMSPing->Update();
    TPaveStats* stats1RMSPing = (TPaveStats*)cRMSPing->GetPrimitive("stats");
	double y1RMSPing = stats1RMSPing->GetY1NDC();
	double y2RMSPing = stats1RMSPing->GetY2NDC();
	stats1RMSPing->SetName("Total RMSPing");
	stats1RMSPing->SetY1NDC(y1RMSPing);    //set new y start position
	stats1RMSPing->SetY2NDC(y2RMSPing);    //set new y end position
    stats1RMSPing->SetX1NDC(0.67);
    stats1RMSPing->SetX2NDC(0.87);
	cRMSPing->Update();

    RMSPing2->Draw("sames HIST");
    cRMSPing->Update();
    TPaveStats* stats2RMSPing = (TPaveStats*)cRMSPing->GetPrimitive("stats");
	double newy1RMSPing = 2 * y1RMSPing - y2RMSPing;   // new (lower) y start position of stats box
	double newy2RMSPing = y1RMSPing;            // new (upper) y start position of stats box
	stats2RMSPing->SetName("Problematic blocks RMSPing");
	stats2RMSPing->SetY1NDC(newy1RMSPing);    //set new y start position
	stats2RMSPing->SetY2NDC(newy2RMSPing);    //set new y end position
    stats2RMSPing->SetX1NDC(0.67);
    stats2RMSPing->SetX2NDC(0.87);
    stats2RMSPing->SetLineColor(kRed);
    stats2RMSPing->SetTextColor(kRed);
	cRMSPing->Update();

    TCanvas *cRMSPong = new TCanvas("cRMSPong","cRMSPong",1);
    RMSPong1->SetLineColor(1);
    RMSPong2->SetLineColor(2);
    RMSPong1->Draw();

    cRMSPong->Update();
    TPaveStats* stats1RMSPong = (TPaveStats*)cRMSPong->GetPrimitive("stats");
	double y1RMSPong = stats1RMSPong->GetY1NDC();
	double y2RMSPong = stats1RMSPong->GetY2NDC();
	stats1RMSPong->SetName("Total RMSPong");
	stats1RMSPong->SetY1NDC(y1RMSPong);    //set new y start position
	stats1RMSPong->SetY2NDC(y2RMSPong);    //set new y end position
    stats1RMSPong->SetX1NDC(0.67);
    stats1RMSPong->SetX2NDC(0.87);
	cRMSPong->Update();

    RMSPong2->Draw("sames HIST");
    cRMSPong->Update();
    TPaveStats* stats2RMSPong = (TPaveStats*)cRMSPong->GetPrimitive("stats");
	double newy1RMSPong = 2 * y1RMSPong - y2RMSPong;   // new (lower) y start position of stats box
	double newy2RMSPong = y1RMSPong;            // new (upper) y start position of stats box
	stats2RMSPong->SetName("Problematic blocks RMSPong");
	stats2RMSPong->SetY1NDC(newy1RMSPong);    //set new y start position
	stats2RMSPong->SetY2NDC(newy2RMSPong);    //set new y end position
    stats2RMSPong->SetX1NDC(0.67);
    stats2RMSPong->SetX2NDC(0.87);
    stats2RMSPong->SetLineColor(kRed);
    stats2RMSPong->SetTextColor(kRed);
	cRMSPong->Update();
    RMSPong2->Draw("sames HIST");
}