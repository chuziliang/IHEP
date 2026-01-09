void GetTimeHist()
{
    TFile* f = TFile::Open("output3.root");
    TTree *ChannelTree = (TTree*)f->Get("PMTtree");

    TH1* hist[20];

    const int MaxEvent = 2550;
    int t_hit_num;
    int t_eventNum;
    double t_hittime[MaxEvent];

    ChannelTree->SetBranchAddress("eventNum",&t_eventNum);
    ChannelTree->SetBranchAddress("pmt_hit_num",&t_hit_num);
    ChannelTree->SetBranchAddress("pmt_hittime",t_hittime);

    Int_t nentries = ChannelTree->GetEntries();
    TCanvas* c1 = new TCanvas("c1","c1",1);
    c1->Divide(4,5);
    for (int i = 0; i<nentries; i++)
    {
        ChannelTree->GetEntry(i);
        hist[i] = new TH1F(TString::Format("Channel %1d time pdf",t_eventNum),TString::Format("Channel %1d time pdf",t_eventNum),500,20,140);
        for (int j = 0; j<t_hit_num; j++)
        {
            hist[i]->Fill(t_hittime[j]);
        }
        c1->cd(i+1);
        hist[i]->Draw();
    }
}