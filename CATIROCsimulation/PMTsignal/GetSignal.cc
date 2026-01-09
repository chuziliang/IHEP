void GetSignal()
{
    double scale = 1.0;
    TCanvas* c1 = new TCanvas("c1","c1",1);
    // c1->Divide(2,2);
    // TH1F* amplitude = new TH1F("amplitude","amplitude",100,0,5);
    // TH1F* PeakPosition = new TH1F("PeakPosition","PeakPosition",100,90,850);
    // TH1F* sigma = new TH1F("sigma","sigma",100,0,10);

    TString name;
    TString histname;
    int j = 14;
    TString num = to_string(j);
    name = "./ch1.txt";
    histname = "num" + num  + " signal";
    ifstream in(name);
    int loop = 0;
	while (in.good()) {
       
        TH1F* tmp = new TH1F(histname,histname, 1008, 0, 1008);
        double charge = 0, baseline = 0, RMS = 0;
        double ori, nan;
        // TH1F* t = new TH1F("t","",1000,-2,2);
        for (int i = 0; i < 1008; i ++) {
            in >> ori >> nan >> nan >> nan;
            tmp->SetBinContent(i + 1, ori/10.0);
        }
        loop++;
        if (loop != j) delete tmp;
        if (loop == j)
        {
        int peakPos = tmp->GetMinimumBin();
        //  if ((peakPos < 90) || (peakPos > 850))
        // {
        //     // delete t;
        //     delete tmp;
        //     continue;
        // }
        for (int i = (peakPos - 80); i < (peakPos - 50); i ++) {
            baseline += tmp->GetBinContent(i);
        }
        baseline /= 30;
        for (int i = 1; i <= 1008; i ++) {
            tmp->SetBinContent(i, baseline - tmp->GetBinContent(i));
            // if (i >= (peakPos - 30) && i < (peakPos + 70))
            // {
            //     charge += tmp->GetBinContent(i) * scale / 50;
            // }
            // if (i <= (peakPos - 50) || i >(peakPos + 150))
            // {
            //     t->Fill(tmp->GetBinContent(i));
            // }
        }
        TF1* func = new TF1("SpecModel", "landau",peakPos-20 , peakPos+50);
        double para[3];
        tmp ->Fit(func, "RM");
        gStyle->SetOptFit(1111);
        func ->GetParameters(para); 
        cout<<"Chisquare "<< func->GetChisquare()<<endl;
        // amplitude->Fill(para[0]);
        // PeakPosition->Fill(para[1]);
        // sigma->Fill(para[2]);
        // delete tmp;
        // delete func;
        tmp ->Draw();
        func->Draw("same");
        break;
        // RMS = t->GetRMS();  
        // delete t;
        //j++;
        }	
	}
    // c1->cd(1);
    // amplitude->Draw();
    // c1->cd(2);
    // PeakPosition->Draw();
    // c1->cd(3);
    // sigma->Draw();
	in.close();
}

