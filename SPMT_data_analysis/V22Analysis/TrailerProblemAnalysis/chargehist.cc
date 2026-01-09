void chargehist()
{
    TFile *inputfile1 = TFile::Open("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/TrailerProblemAnalysis/test.root","read");
    TTree *EventData1 = (TTree*)inputfile1->Get("EventData");

    TFile *inputfile2 = TFile::Open("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/TrailerProblemAnalysis/test2.root","read");
    TTree *EventData2 = (TTree*)inputfile2->Get("EventData");

    TH1 *Charge1 = new TH1F("Total charge","Total charge",80,40,120);
    TH1 *Charge2 = new TH1F("Charge in problematic blocks","Charge in problematic blocks",80,40,120);

    UInt_t chargeValue1;
    UInt_t chargeValue2;

    EventData1->SetBranchAddress("ChargeValue",&chargeValue1);
    EventData2->SetBranchAddress("ChargeValue",&chargeValue2);

    for(int i = 0; i < EventData1->GetEntries(); i++)
    {
        EventData1->GetEntry(i);
        Charge1->Fill(chargeValue1);
    }

    for(int i = 0; i < EventData2->GetEntries(); i++)
    {
        EventData2->GetEntry(i);
        Charge2->Fill(chargeValue2);
    }

    TCanvas *c1 = new TCanvas("c1","c1",1);
    Charge1->SetLineColor(1);
    Charge2->SetLineColor(2);
    Charge1->Draw();

    c1->Update();
    TPaveStats* stats1 = (TPaveStats*)c1->GetPrimitive("stats");
	double y1 = stats1->GetY1NDC();
	double y2 = stats1->GetY2NDC();
	stats1->SetName("Total charge");
	stats1->SetY1NDC(y1);    //set new y start position
	stats1->SetY2NDC(y2);    //set new y end position
    stats1->SetX1NDC(0.67);
    stats1->SetX2NDC(0.87);
	c1->Update();


    // Float_t rightmax = 1.1*Charge2->GetMaximum();
    // Float_t scale    = gPad->GetUymax()/rightmax;
    Float_t scale = Charge1->GetMaximum()/Charge2->GetMaximum();
    Float_t rightmax = gPad->GetUymax()/scale;
    Charge2->Scale(scale);
    // gStyle->SetOptStat(1);
    Charge2->Draw("sames HIST");
    c1->Update();
    TPaveStats* stats2 = (TPaveStats*)c1->GetPrimitive("stats");
	double newy1 = 2 * y1 - y2;   // new (lower) y start position of stats box
	double newy2 = y1;            // new (upper) y start position of stats box
	stats2->SetName("Charge in problematic blocks");
	stats2->SetY1NDC(newy1);    //set new y start position
	stats2->SetY2NDC(newy2);    //set new y end position
    stats2->SetX1NDC(0.67);
    stats2->SetX2NDC(0.87);
    stats2->SetLineColor(kRed);
    stats2->SetTextColor(kRed);
	c1->Update();


    TGaxis*axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
                            gPad->GetUxmax(),gPad->GetUymax(),
                            0,rightmax,510,"+L");
    axis->SetLineColor(kRed);
    axis->SetLabelColor(kRed);
    axis->Draw();
    c1->Update();
   

}