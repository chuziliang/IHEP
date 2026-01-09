struct DataWithError
{
    double Data;
    double Error;
};

//修正前后电荷的对应关系函数，这个函数的自变量是真实电荷，因变量是包含阈值和暗噪声效应的电荷
double Myfunction(double* x, double* par) //par[0] = Threshold, par[1] = Sigma_SPE, par[2] = DarkRate
{
    double P0 = 0;
    double DarkRateCharge = par[2]*1e-6;//1000ns内暗噪声贡献的电荷的均值
    double xx = x[0]+DarkRateCharge;//暗噪声和物理事例总的电荷均值
    P0 += ROOT::Math::poisson_pdf(0,xx);//均值为“xx”时，测到0 PE的概率
    for (int i = 1; i < 5; i++)//均值为“xx”时，测到i PE且过阈的概率。认为“xx”几乎不会超过5PE
    {
        P0 += ROOT::Math::poisson_pdf(i,xx)*ROOT::Math::normal_cdf(par[0],par[1]*sqrt((double)i),(double)i);
    }
    return -TMath::Log(P0);
}

DataWithError GetTrueCharge(DataWithError RoughCharge, DataWithError DarkRate)//RoughCharge为修正前的电荷，输出修正后的电荷
{
    double R_Spe = 33.2/100.0;  //J22.1.0-rc4，SPMT电子学模拟中使用了固定分辨率
    double Threshold = 1.0/3.0; //J22.1.0-rc4，SPMT电子学模拟中使用了固定阈值
    auto f = new TF1("function",Myfunction,0,5,3);
    f->SetParameters(Threshold,R_Spe,DarkRate.Data);
    DataWithError TrueCharge;
    TrueCharge.Data = f->GetX(RoughCharge.Data,0,0,1.E-12,1000,false);//反解出真实电荷

    //传递误差
    double df;
    df = f->Derivative(TrueCharge.Data,nullptr,1.e-5);
    TrueCharge.Error = sqrt( RoughCharge.Error*RoughCharge.Error/(df*df) + DarkRate.Error*DarkRate.Error );
    return TrueCharge;
}

DataWithError GetP0Mu(int TotalEvents, int ZeroEvents)
{
    double P0 = (double)(ZeroEvents)/(double)(TotalEvents);
    double mean = -TMath::Log(P0);
    double error = mean/TotalEvents;
    error = sqrt(error);
    DataWithError P0Mu;
    P0Mu.Data = mean;
    P0Mu.Error = error;
    return P0Mu;
}

DataWithError CombineResult(const vector<double>& x, const vector<double>& e)
{
    double sum_x = 0.0;
    double sum_w = 0.0;
    double sum_e = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1;
        sum_x += w * x[i];
        sum_w += w;
        sum_e += e[i]*e[i];
    }
    sum_e = sqrt(sum_e)/sum_w;
    DataWithError combined_result;
    combined_result.Data = sum_x / sum_w;
    combined_result.Error = sum_e;

    return combined_result;
}

double combined_measurement(const vector<double>& x, const vector<double>& e) {
    double sum_x = 0.0;
    double sum_w = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1 / (e[i] * e[i]);
        sum_x += w * x[i];
        sum_w += w;
    }

    double combined_x = sum_x / sum_w;
    return combined_x;
}

double combined_error(const vector<double>& e) {
    double sum_w = 0.0;

    for (int i = 0; i < e.size(); i++) {
        double w = 1 / (e[i] * e[i]);
        sum_w += w;
    }

    double combined_e = 1 / sqrt(sum_w);
    return combined_e;
}

void npeLPMTtonpeSPMT()
{
    const int SPMTDarkRate[6] = {611, 78, 313, 94, 1529, 505};
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    TH1 *LPMTHist[21];
    for (int i = 0; i < 21; i++)
    {
        TString histname = TString::Format("%1d MeV LPMT hist",energy[i]);
        LPMTHist[i] = new TH1F(histname,histname,2000,0,200); 
    }

    TH1 *SPMTHist[6][21];
    for (int id = 0; id < 6; id++)
    {
        for (int i = 0; i < 21; i++)
        {
            TString histname = TString::Format("%1d PE SPMT_%1d hist",energy[i],id);
            SPMTHist[id][i] = new TH1F(histname,histname,300,0,30); 
        }
    }

    TH1 *TrueHist[21]; //
    for (int i = 0; i < 21; i++) //
    {
        TString histname = TString::Format("%1d MeV LPMT ElecSim hist",energy[i]);
        TrueHist[i] = new TH1F(histname,histname,2000,0,200); 
    }

    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile_SPMT_elec = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut/%1dMevCharge.root",energy[i]),"read");
        TFile *inputfile_LPMT_Toy  = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/10Percent/DetectorCentor/ChargeSpectrum/%1dMevCharge.root",energy[i]),"read");
        
        if (!inputfile_SPMT_elec || !inputfile_LPMT_Toy)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *SPMT_charge = (TTree*)inputfile_SPMT_elec->Get(TString::Format("Charge%1d",j));
            TTree *LPMT_charge = (TTree*)inputfile_LPMT_Toy->Get(TString::Format("Charge%1d",j));
            if (!SPMT_charge || !LPMT_charge)
            {
                continue;
            }

            double LPMTPE = 0;
            double SPMTPE[6] = {0,0,0,0,0,0};
            double TruePE = 0; //
            LPMT_charge->SetBranchAddress("LPMTCharge",&LPMTPE);
            SPMT_charge->SetBranchAddress("SPMTCharge",SPMTPE);
            SPMT_charge->SetBranchAddress("TrueCharge",&TruePE); //
           
            for (int k = 0; k < LPMT_charge->GetEntries(); k++)
            {
                LPMT_charge->GetEntry(k);
                SPMT_charge->GetEntry(k);
                LPMTHist[i]->Fill(LPMTPE);
                TrueHist[i]->Fill(TruePE); //
                for (int id = 0; id < 6; id++)
                {
                    SPMTHist[id][i]->Fill(SPMTPE[id]);
                }
            }
            delete LPMT_charge;
            delete SPMT_charge;
        }
        inputfile_SPMT_elec->Close();
        inputfile_LPMT_Toy->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/10Percent/DetectorCentor/npeLPMTtonpeSPMT.root","RECREATE");
    outputfile->mkdir("Spectrum");
    
    double LPMTMeanCharge[21];
    double ErrorLPMTMeanCharge[21];
    double TrueLPMTMeanCharge[21]; //
    double ErrorTrueLPMTMeanCharge[21]; //
    double SPMTMeanCharge[6][21];
    double ErrorSPMTMeanCharge[6][21];
    double SPMTMeanCharge_tot[21];
    double ErrorSPMTMeanCharge_tot[21];

    int n = 0;
    for (int i = 0; i < 21; i++)
    {
        
        outputfile->cd("Spectrum");
        LPMTHist[i]->Write();
        TrueHist[i]->Write(); //
        for (int id = 0; id < 6; id++)
        {
            SPMTHist[id][i]->Write();
        }
        
        double entries = LPMTHist[i]->GetEntries();
        bool P0LargerThan0 = false;
        bool SelectHist = true;
        for (int id = 0; id < 6; id++)
        {
            if (SPMTHist[id][i]->GetBinContent(1) > 0 )
            {
                P0LargerThan0 = true;
            }
            else
            {
                P0LargerThan0 = false;
            }
            SelectHist &= P0LargerThan0;
        }
        
        if (entries > 1 && SelectHist)
        {
            LPMTMeanCharge[n] = LPMTHist[i]->GetMean();
            ErrorLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*LPMTHist[i]->GetMeanError();
            TrueLPMTMeanCharge[n] = TrueHist[i]->GetMean(); //
            ErrorTrueLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*TrueHist[i]->GetMeanError(); //
            
            vector<double> ToCombineSPMTMeanCharge;
            vector<double> ToErrorCombineSPMTMeanCharge;
            for (int id = 0; id < 6; id++)
            {
                DataWithError RC;
                DataWithError TC;
                DataWithError DarkRate;
                DarkRate.Data = SPMTDarkRate[id];
                DarkRate.Error = 0;
                RC = GetP0Mu(SPMTHist[id][i]->GetEntries(),SPMTHist[id][i]->GetBinContent(1));
                TC = GetTrueCharge(RC,DarkRate);

                SPMTMeanCharge[id][n] = TC.Data;
                ErrorSPMTMeanCharge[id][n] = TC.Error;
                ToCombineSPMTMeanCharge.push_back(TC.Data);
                ToErrorCombineSPMTMeanCharge.push_back(TC.Error);
            }
            DataWithError CombineCharge = CombineResult(ToCombineSPMTMeanCharge,ToErrorCombineSPMTMeanCharge);
            SPMTMeanCharge_tot[n] = CombineCharge.Data;
            ErrorSPMTMeanCharge_tot[n] = CombineCharge.Error;
            ToCombineSPMTMeanCharge.clear();
            ToErrorCombineSPMTMeanCharge.clear();
            n++;
        }

    }

    TF1 *f1 = new TF1("f1","[0]*x",0,10);
    f1->SetParNames("k");

    TF1 *TrueLPMT_SPMT[6];
    for (int i = 0; i < 6; i++)
    {
        TrueLPMT_SPMT[i] = new TF1(TString::Format("TrueLPMT_SPMT%1d ",i),"[0]*x",0,10);
        TrueLPMT_SPMT[i]->SetParNames("k");
        TrueLPMT_SPMT[i]->SetLineColor(4);
    }
    TF1 *TrueLPMT_SPMT_tot;
    TrueLPMT_SPMT_tot = new TF1("TrueLPMT_SPMT_tot","[0]*x",0,10);
    TrueLPMT_SPMT_tot->SetParNames("k");
    TrueLPMT_SPMT_tot->SetLineColor(4);

    //找到探测器模拟级别下，大小PMT的关系曲线
    TFile *DetSimResultFile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/npeLPMTtonpeSPMT.root","read");
    TGraphErrors *DetSimnpeLPMTtonpeSPMT[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        DetSimnpeLPMTtonpeSPMT[id] = (TGraphErrors*)DetSimResultFile->Get(GraphName);
        DetSimnpeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        DetSimnpeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        f1->SetLineColor(3);
        DetSimnpeLPMTtonpeSPMT[id]->Fit(f1);
        DetSimnpeLPMTtonpeSPMT[id]->SetLineColor(3);
        TrueLPMT_SPMT[id]->SetParameter(0,0.9*f1->GetParameter(0));
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);
    }

    double SPMTMeanChargeDetSim_tot[21];
    double ErrorSPMTMeanChargeDetSim_tot[21];
    double LPMTMeanChargeDetSim_tot[21];
    double ErrorLPMTMeanChargeDetSim_tot[21];
    for (int i = 0; i < 21; i++)
    {
        vector<double> ToCombineSPMTMeanCharge;
        vector<double> ToErrorCombineSPMTMeanCharge;
        for (int id = 0; id < 6; id++)
        {
            ToCombineSPMTMeanCharge.push_back(DetSimnpeLPMTtonpeSPMT[id]->GetPointX(i));
            ToErrorCombineSPMTMeanCharge.push_back(DetSimnpeLPMTtonpeSPMT[id]->GetErrorX(i));
        }
        DataWithError CombineCharge = CombineResult(ToCombineSPMTMeanCharge,ToErrorCombineSPMTMeanCharge);
        SPMTMeanChargeDetSim_tot[i] = CombineCharge.Data;
        ErrorSPMTMeanChargeDetSim_tot[i] = CombineCharge.Error;
        ToCombineSPMTMeanCharge.clear();
        ToErrorCombineSPMTMeanCharge.clear();

        LPMTMeanChargeDetSim_tot[i] = DetSimnpeLPMTtonpeSPMT[0]->GetPointY(i);
        ErrorLPMTMeanChargeDetSim_tot[i] = DetSimnpeLPMTtonpeSPMT[0]->GetErrorY(i);
    }
    TGraphErrors *DetSimnpeLPMTtonpeSPMT_tot = new TGraphErrors(21,SPMTMeanChargeDetSim_tot,LPMTMeanChargeDetSim_tot,ErrorSPMTMeanChargeDetSim_tot,ErrorLPMTMeanChargeDetSim_tot);
    DetSimnpeLPMTtonpeSPMT_tot->SetLineColor(3);
    DetSimnpeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("nPE SPMT");
    DetSimnpeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("nPE LPMT");
    f1->SetLineColor(3);
    DetSimnpeLPMTtonpeSPMT_tot->Fit(f1);
    TrueLPMT_SPMT_tot->SetParameter(0,0.9*f1->GetParameter(0));

    TCanvas *Canvas_tot = new TCanvas("ct","c1",1);
    
    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TString GraphName = "LPMT npe to all SPMT npe";
    npeLPMTtonpeSPMT_tot = new TGraphErrors(n,SPMTMeanCharge_tot,LPMTMeanCharge,ErrorSPMTMeanCharge_tot,ErrorLPMTMeanCharge);
    npeLPMTtonpeSPMT_tot->SetTitle(GraphName);
    npeLPMTtonpeSPMT_tot->SetName(GraphName);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu SPMT");
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu LPMT");
    f1->SetLineColor(2);
    npeLPMTtonpeSPMT_tot->Fit(f1);
    gStyle->SetOptFit(1111);
    gStyle->SetStatY(0.5);
    gPad->SetLogx();
    gPad->SetLogy();
    npeLPMTtonpeSPMT_tot->Draw("AP");
    DetSimnpeLPMTtonpeSPMT_tot->Draw("P");
    TrueLPMT_SPMT_tot->Draw("same");
    outputfile->cd();
    Canvas_tot->Write();

    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],LPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorLPMTMeanCharge);
        npeLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeLPMTtonpeSPMT[id]->SetName(GraphName);
        npeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        f1->SetLineColor(2);
        npeLPMTtonpeSPMT[id]->Fit(f1);
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);


        TString CanvasName = TString::Format("C%1d",id);
        Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        Canvas[id]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT[id]->Draw("AP");
        DetSimnpeLPMTtonpeSPMT[id]->Draw("p");
        TrueLPMT_SPMT[id]->Draw("same");
        outputfile->cd();
        npeLPMTtonpeSPMT[id]->Write();
    }

    TGraphErrors *npeTrueLPMTtonpeSPMT[6]; //
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("True LPMT npe to elec SPMT_%1d npe",id);
        npeTrueLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],TrueLPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorTrueLPMTMeanCharge);
        npeTrueLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeTrueLPMTtonpeSPMT[id]->SetName(GraphName);
        npeTrueLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeTrueLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        npeTrueLPMTtonpeSPMT[id]->Fit(f1);
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);

        outputfile->cd();
        npeTrueLPMTtonpeSPMT[id]->Write();
    }

    
}