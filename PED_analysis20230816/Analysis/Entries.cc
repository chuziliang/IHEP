void Entries()
{
    TFile *inputfile[4];
    TString name[4];
    name[0] = "/home/run/SPMT_ziliang/AnalysisABCF2.0/result/root/8164_230616-1957_2049_0.root";
    name[1] = "/home/run/SPMT_ziliang/AnalysisABCF2.0/result/root/8164_230616-2003_2050_0.root";
    name[2] = "/home/run/SPMT_ziliang/AnalysisABCF2.0/result/root/8164_230616-2011_2051_0.root";
    name[3] = "/home/run/SPMT_ziliang/AnalysisABCF2.0/result/root/8164_230616-2017_2052_0.root";
    TH1 *outputhist[4];
    outputhist[0] = new TH1F("600DACu 300s high rate mode","600DACu 300s high rate mode",128,0,128);
    outputhist[0]->SetLineColor(38);
    outputhist[1] = new TH1F("700DACu 300s high rate mode","700DACu 300s high rate mode",128,0,128);
    outputhist[1]->SetLineColor(2);
    outputhist[2] = new TH1F("800DACu 300s high rate mode","800DACu 300s high rate mode",128,0,128);
    outputhist[3] = new TH1F("500DACu 300s high rate mode","500DACu 300s high rate mode",128,0,128);
    outputhist[3]->SetLineColor(3);
    TH1 *HistHGPing= NULL;
    TH1 *HistHGPong= NULL;
    TH1 *HistLGPing= NULL;
    TH1 *HistLGPong= NULL;
    for (int j = 0; j < 4; j++)
    {
        inputfile[j] = TFile::Open(name[j]);
        for (int i = 0; i < 128; i++)
        {
            int AllEntries = 0;
            HistHGPing = (TH1*)inputfile[j]->Get("Histo_Charge_HG_" + TString(Form("%i",i)) + "_ping");
            HistHGPong = (TH1*)inputfile[j]->Get("Histo_Charge_HG_" + TString(Form("%i",i)) + "_pong");
            HistLGPing = (TH1*)inputfile[j]->Get("Histo_Charge_LG_" + TString(Form("%i",i)) + "_ping");
            HistLGPong = (TH1*)inputfile[j]->Get("Histo_Charge_LG_" + TString(Form("%i",i)) + "_pong");
            AllEntries = HistHGPing->GetEntries()+HistHGPong->GetEntries()+HistLGPing->GetEntries()+HistLGPong->GetEntries();
            outputhist[j]->SetBinContent(i+1,AllEntries);
            outputhist[j]->SetStats(0);
        }
        inputfile[j]->Close();
    }
    

    outputhist[2]->Draw();
    outputhist[1]->Draw("same");
    outputhist[0]->Draw("same");
    outputhist[3]->Draw("same");
    
    TLegend *legend2 = new TLegend(.75,.80,.95,.95);
    legend2->AddEntry(outputhist[3],"500DACu 300s high rate mode");
    legend2->AddEntry(outputhist[0],"600DACu 300s high rate mode");
    legend2->AddEntry(outputhist[1],"700DACu 300s high rate mode");
    legend2->AddEntry(outputhist[2],"800DACu 300s high rate mode");
    legend2->Draw();
}
