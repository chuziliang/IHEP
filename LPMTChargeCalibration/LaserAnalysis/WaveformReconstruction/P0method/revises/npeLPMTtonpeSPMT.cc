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
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut/%1dMevCharge.root",energy[i]));
        if (!inputfile)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double LPMTPE = 0;
            double SPMTPE[6] = {0,0,0,0,0,0};
            double TruePE = 0; //
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("LPMTCharge",&LPMTPE);
            charge->SetBranchAddress("SPMTCharge",SPMTPE);
            charge->SetBranchAddress("TrueCharge",&TruePE); //
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                LPMTHist[i]->Fill(LPMTPE);
                TrueHist[i]->Fill(TruePE); //
                for (int id = 0; id < 6; id++)
                {
                    SPMTHist[id][i]->Fill(SPMTPE[id]);
                }
            }
            delete charge;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("npeLPMTtonpeSPMT.root","RECREATE");
    outputfile->mkdir("Spectrum");
    
    double LPMTMeanCharge[21];
    double ErrorLPMTMeanCharge[21];
    double TrueLPMTMeanCharge[21]; //
    double ErrorTrueLPMTMeanCharge[21]; //
    double SPMTMeanCharge[6][21];
    double ErrorSPMTMeanCharge[6][21];
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
            for (int id = 0; id < 6; id++)
            {
                DataWithError RC;
                DataWithError TC;
                DataWithError DarkRate;
                DarkRate.Data = SPMTDarkRate[id];
                DarkRate.Error = 0;

                double P0 = (double)SPMTHist[id][i]->GetBinContent(1)/SPMTHist[id][i]->GetEntries();
                double miuP0 = P0*(1 - P0)/SPMTHist[id][i]->GetEntries();
                miuP0 = sqrt(miuP0);

                RC.Data = -TMath::Log(P0);
                RC.Error = miuP0/P0;
                TC = GetTrueCharge(RC,DarkRate);
                // SPMTMeanCharge[id][n] = -TMath::Log(P0);
                // ErrorSPMTMeanCharge[id][n] = miuP0/P0;

                SPMTMeanCharge[id][n] = TC.Data;
                ErrorSPMTMeanCharge[id][n] = TC.Error;

            }
            n++;
        }

    }

    
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
        npeLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);


        TString CanvasName = TString::Format("C%1d",id);
        Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        Canvas[id]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT[id]->Draw("AP");
        outputfile->cd();
        npeLPMTtonpeSPMT[id]->Write();
    }

    TGraphErrors *npeTrueLPMTtonpeSPMT[6]; //
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("True LPMT npe to SPMT_%1d npe",id);
        npeTrueLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],TrueLPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorTrueLPMTMeanCharge);
        npeTrueLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeTrueLPMTtonpeSPMT[id]->SetName(GraphName);
        npeTrueLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeTrueLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        npeTrueLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);

        outputfile->cd();
        npeTrueLPMTtonpeSPMT[id]->Write();
    }
  
}