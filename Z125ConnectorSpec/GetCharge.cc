void GetCharge()
{
    double scale = 1.0;

    //需要画的图和窗口
    TH1F* spec[16];
    TF1* func[16];
    TCanvas* c1 = new TCanvas("c1","c1",1);
    c1->Divide(4,4); 
    
   
	for (int j=0; j < 16; j++)
    {
        TString name;
        TString histname;
        TString num = to_string(j+1);
        name = "./Z125Ch" + num + ".txt";
        histname = "ch" + num + " Charge Spectrum";
        ifstream in(name);
    	spec[j] = new TH1F(histname, histname, 1000, -0.5, 3);//每个Hist图需要有一个独特的name

	    while (in.good()) {
	    	double charge = 0, baseline = 0, RMS = 0;
	    	double ori, nan;
	    	TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
            TH1F* t = new TH1F("t","",1000,-2,2);
	    	for (int i = 0; i < 1008; i ++) {
	    		in >> ori >> nan >> nan >> nan;
	    		tmp->SetBinContent(i + 1, ori/10.0);
	    	}
            int peakPos = tmp->GetMinimumBin();
            if ((peakPos < 90) || (peakPos > 850))
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
	    		if (i >= (peakPos - 30) && i < (peakPos + 70))
                {
                    charge += tmp->GetBinContent(i) * scale / 50;
                }
                if (i <= (peakPos - 50) || i >(peakPos + 150))
                {
                    t->Fill(tmp->GetBinContent(i));
                }
	    	}
            RMS = t->GetRMS();
             if (RMS < 0.1)
            {
                spec[j]->Fill(charge);
            }
            delete t;
	    	delete tmp;
	    }

		c1->cd(j+1);
        Double_t Q0 = 0;
	    func[j] = new TF1("SpecModel","gaus",0.1,1);
	    func[j]->SetParameters(Q0, 0.03, 0.8, 0.3, 0.034, -20, 0.5, 7);
	    func[j]->SetParNames("Q", "sigma");
        double para[3];
        spec[j]->Fit(func[j], "R");
        func[j]->GetParameters(para);
		gStyle->SetOptFit(1011);//设置右上角显示拟合参数
        double gain = para[1] / 0.16;
		if (j<9)
		{
            spec[j]->SetLineColor(j+1);
		}
		if (j>=9)
		{
			spec[j]->SetLineColor((j-7)*10);//设置图像线的颜色。10-20是不同的灰度，所以这里单独设置
		}
        spec[j]->Draw();
        func[j]->Draw("same");//“same”可以把spec[j]和func[j]画在一张图里
        cout << "gain: " << gain << endl;
	    cout <<"gain error: " << func[j]->GetParError(2) / 0.16 << endl;
	    in.close();
    }
}
