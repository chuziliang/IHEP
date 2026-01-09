
void txt2root()
{
    double scale = 1.0;
    TCanvas* c1 = new TCanvas("c1","c1",1);
    // c1->Divide(2,2);
    // TH1F* amplitude = new TH1F("amplitude","amplitude",100,0,5);
    // TH1F* PeakPosition = new TH1F("PeakPosition","PeakPosition",100,90,850);
    // TH1F* sigma = new TH1F("sigma","sigma",100,0,10);

    TString name;
    TString histname;
    int j = 0;
    name = "./ch1.txt";
    ifstream in(name);
    TH1* AllRMS = new TH1F("AllRMS","AllRMS",1000,0,1);
    int loop = 0;
	while (in.good())
    {
        TString num = to_string(j);
        histname = "num" + num  + " signal";
        TH1F* tmp = new TH1F(histname,histname, 1008, 0, 1008);
        double charge = 0, baseline = 0, RMS = 0;
        double ori, nan;
        TH1F* t = new TH1F("t","",1000,-2,2);
        for (int i = 0; i < 1008; i ++) 
        {
            in >> ori >> nan >> nan >> nan;
            tmp->SetBinContent(i + 1, ori/10.0);
        }
        loop++;
      
        int peakPos = tmp->GetMinimumBin();
        if ((peakPos < 90) || (peakPos > 850))
        {
            delete t;
            delete tmp;
            continue;
        }

        //cout<<"-------------- test here --------------------"<<endl;
        for (int i = (peakPos - 80); i < (peakPos - 50); i ++) 
        {
            baseline += tmp->GetBinContent(i);
        }
        baseline /= 30;
        for (int i = 1; i <= 1008; i ++) 
        {
            tmp->SetBinContent(i, baseline - tmp->GetBinContent(i));
            // if (i >= (peakPos - 30) && i < (peakPos + 70))
            // {
            //     charge += tmp->GetBinContent(i) * scale / 50;
            // }
            if (i <= (peakPos - 50) || i >(peakPos + 150))
            {
            t->Fill(tmp->GetBinContent(i));
            }
        }
        RMS = t->GetRMS();
        AllRMS->Fill(RMS);
        delete tmp; 
        delete t;
        j++;
	}
	in.close();
    AllRMS->Draw();
}