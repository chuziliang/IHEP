Double_t specFit(Double_t* x, Double_t* par) {
	double f1, f2, f3, f4, f5, f6;
	double f = 0;
	for (int n = 0; n <= 3; n ++) {
		f1 = (1 - par[5]) * exp(-0.5 * (x[0] - par[0] - n * par[2]) * (x[0] - par[0] - n * par[2]) / (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * TMath::Pi() * (par[1] * par[1] + n * par[3] * par[3])));
		f2 = 0.5 * par[4] * exp(-1 * par[4] * (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f3 = TMath::Erf(TMath::Abs(n * par[2] + par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f4 = TMath::Erf(TMath::Abs(x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f5 = TMath::Sign(1.0, (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f6 = TMath::Poisson(n, par[6]);
		f += f6 * (f1 + par[5] * (f2 * (f3 + f5 * f4)));
	}
	return f * par[7];
}//定义拟合函数，用来拟合电荷谱

void GetChargeTestForLi()
{
    double scale = 1.0;

    //Hist图和Canva
    TH1F* spec;
    TCanvas* c1 = new TCanvas("c1","c1",1);
    //c1->Divide(4,4); 
    

    
	//for (int j=0; j < 1; j++)//这里是因为我有16个txt文件
    {
        TString name;
        TString histname;
        //TString num = to_string(j);
        name = "sPMT_calib_2_1.85V_1300V_8ns_1kHz.txt";
        histname = "sPMT_calib_2_1.85V_1300V_8ns_1kHz Charge Spectrum";
        ifstream in(name);
    	spec = new TH1F(histname, histname, 100, -1, 3);//每个Hist图需要有一个独特的name
        //int j = 0;
        int cyc = 0;
	    while (in.good()) {
            cyc++;
            if (cyc>100)
            {
                break;
            }
            
	    	double charge = 0, baseline = 0, RMS = 0;
	    	double ori, nan;
	    	TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
            TH1F* t = new TH1F("t","",1000,-2,2);
	    	for (int i = 0; i < 1008; i ++) {
	    		in >> ori >> nan >> nan >> nan;//txt文件里面有4列，只有第一列是我需要的数据
	    		tmp->SetBinContent(i + 1, ori);//把取到波形填到这个hist图里面。这里除以10是因为取数时用了10倍的放大器，没有放大器不需要除以10
	    	}
            int peakPos = tmp->GetMinimumBin();
            if ((peakPos < 90) || (peakPos > 850))//有时候因为有噪声，导致最小值在数据的两侧，不要这样的数据
            {
                delete t;
	    	    delete tmp;
                continue;
            }
	    	for (int i = (peakPos - 80); i < (peakPos - 50); i ++) {//计算基线，选择峰位前80-50ns的数据
	    		baseline += tmp->GetBinContent(i);
	    	}
	    	baseline /= 30;
	    	for (int i = 1; i <= 1008; i ++) {
	    		tmp->SetBinContent(i, baseline - tmp->GetBinContent(i));//剪掉基线，得到真实的电压值。
	    		if (i >= (peakPos - 30) && i < (peakPos + 70))
                {
                    charge += tmp->GetBinContent(i) * scale / 50;//对电流积分，得到电荷值。除以50是因为电阻为50Ω，由电压得到电流。
                }
                if (i <= (peakPos - 50) || i >(peakPos + 150))//将积分范围外的数据Fill到一个Hist里，计算这个hist的RMS值。如果RMS值较大，说明
                {
                    t->Fill(tmp->GetBinContent(i));
                }
	    	}
            RMS = t->GetRMS();
            //if (RMS < 0.1)
            {
                spec->Fill(charge);//Fill数据到hist图里，得到电荷谱
                cout<<charge<<endl;
            }
            delete t;
	    	delete tmp;
            //j++;
	    }
        Double_t Q0 = 0;

        //拟合函数和拟合参数
	    TF1* func = new TF1("SpecModel", specFit,0.5, 3, 8);
	    func->SetParameters(Q0, 0.03, 0.8, 0.3, 0.034, -20, 0.5, 7);
	    func->SetParNames("Q_{0}", "#sigma_{0}", "Q_{1}", "#sigma_{1}", "#alpha", "w", "u", "A");
        double para[8];


        spec->Fit(func, "R");//拟合
        func->GetParameters(para); 
        double gain = para[2] / 0.16;
        //spec->SetLineColor(j);
        spec->Draw();
        func->Draw("same");
        cout << "gain: " << gain << endl;
	    cout <<"gain error: " << func->GetParError(2) / 0.16 << endl;
        delete func;
	    in.close();
    }
}
