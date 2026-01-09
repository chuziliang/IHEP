void GetCharge()
{
    double scale = 1.0;
    
    TH1F* amplitude = new TH1F("amplitude","amplitude",100,0,25);
    TH1F* PeakPosition = new TH1F("PeakPosition","PeakPosition",100,90,850);
    TH1F* sigma = new TH1F("sigma","sigma",100,0,10);
    TH1F* Chi2 = new TH1F("Chi2","Chisquare",100,1,10);
    TH2* joint = new TH2D("joint","joint",100,10,100,100,0,10);

    TString name;
    // TString histname;
    // TString num = to_string(j);
    name = "./ch1.txt";
    // histname = "num" + num  + " signal";
    ifstream in(name);
    int j = 0;
    int numi = 0;
	while (in.good()) {
        j++;
        cout<<j<<endl;
        TH1F* tmp = new TH1F("tmp"," ", 1008, 0, 1008);
        TH1F* t = new TH1F("t","",1000,-2,2);
        double charge = 0, baseline = 0, RMS = 0;
        double ori, nan;
        // TH1F* t = new TH1F("t","",1000,-2,2);
        for (int i = 0; i < 1008; i ++) {
            in >> ori >> nan >> nan >> nan;
            tmp->SetBinContent(i + 1, ori/10.0);
        }
        int peakPos = tmp->GetMinimumBin();
         if ((peakPos < 100) || (peakPos > 800))
        {
            delete t;
            delete tmp;
            continue;
        }
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
            if (i <= (peakPos - 50) || i >(peakPos + 150))
            {
                t->Fill(tmp->GetBinContent(i));
            }
        }
        RMS = t->GetRMS();
        if (RMS > 0.1)
        {
            delete t;
            delete tmp;
            continue;
        }        
        TF1* func = new TF1("SpecModel", "landau",peakPos-20 , peakPos+60);
        Double_t para[3];
        tmp ->Fit(func, "RM");
        if ((func->GetChisquare())<5)
        {
            func ->GetParameters(para); 
            amplitude->Fill(para[0]);
            PeakPosition->Fill(para[1]);
            sigma->Fill(para[2]);
            joint->Fill(para[0]*para[2],para[2]);
            Chi2->Fill(func->GetChisquare());
        }
        delete t;
        delete tmp;
        delete func;// tmp ->Draw();
        // func->Draw("same");
        // RMS = t->GetRMS();  
        // delete t;
        //j++;	
	}
    TCanvas* c1 = new TCanvas("c1","c1",1);
    c1->Divide(2,2);
    c1->cd(1);
    amplitude->Draw();
    c1->cd(2);
    PeakPosition->Draw();
    c1->cd(3);
    sigma->Draw();
    c1->cd(4);
    joint->Draw("COL");
    joint->GetXaxis()->SetTitle("constant*sigma");
    joint->GetYaxis()->SetTitle("sigma");
    TFile f("joint2.root","new");
    joint->Write();
    TCanvas* c2 = new TCanvas("c2","c2",1);
    Chi2->Draw();
    //c1->cd();
	in.close();
}

