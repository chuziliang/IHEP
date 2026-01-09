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

Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
   double xx =x[0];
   double miu = 500 - sqrt(500*500 - 1000*xx);
   Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
   return f;
}

int GetCorrespondingSPMT(int (&CorrespondingSPMT)[17612][6])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map.txt");
    string line;
    int currentLine = 0;
    int SPMT_ID[6];

    while (getline(file, line)) 
    { 
        stringstream ss(line);
        int LPMTID;
        ss >> LPMTID >> SPMT_ID[0] >> SPMT_ID[1] >> SPMT_ID[2] >> SPMT_ID[3] >> SPMT_ID[4] >> SPMT_ID[5];
        for (int i = 0; i < 6; i++)
        {
            CorrespondingSPMT[LPMTID][i] = SPMT_ID[i];
        }
    }

    file.close();
    return 0;
}

double GetSPMTQE(double (&SPMTQE)[25600])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    for (int id = 0; id < 25600; id ++)
    {
        PMTData->GetEntry(id);
        SPMTQE[id] = Effiency;
    }
    
    // if (PMTID == SPMTID)
    // {
    //     inputfile->Close();
    //     return 100*Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong SPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetLPMTQE(double (&LPMTQE)[17612])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    for (int id = 0; id < 17612; id ++)
    {
        PMTData->GetEntry(id);
        LPMTQE[id] = Effiency/100.0;
    }
    
    // if (PMTID == LPMTID)
    // {
    //     inputfile->Close();
    //     return Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong LPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetSPMTDCR(double (&SPMTDCR)[25600])
{
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
    TTree *PMTData = (TTree*)DarkRate_file->Get("data");
    double darkRate;
    PMTData->SetBranchAddress("darkRate",&darkRate);
    for (int i = 0; i < 25600; i++)
    {
        PMTData->GetEntry(i);
        SPMTDCR[i] = darkRate;
    }
    DarkRate_file->Close();
    return 0;
}

int npeLPMTtonpeSPMT_AllLPMT(int InputID)
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    double SPMTDCR[25600];
    int CorrespondingSPMT[17612][6];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    GetSPMTDCR(SPMTDCR);

    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    TFile *outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/npeLPMTtonpeSPMT_AllLPMT_Root/npeLPMTtonpeSPMT_%1d.root",InputID),"RECREATE");
    TTree *Data = new TTree("Data","Data");
    
    vector<double> AverageMuSPMT;
    vector<double> MuLPMT;
    vector<double> CalibMuLPMT;
    vector<double> DetTotalPE;
    vector<double> Nonlinearity;
    vector<double> TrueMuLPMT;

    vector<double> NonlinearityError;
    vector<double> AverageMuSPMTError;
    vector<double> MuLPMTError;
    vector<double> CalibMuLPMTError;
    vector<double> DetTotalPEError;
    vector<double> TrueMuLPMTError;
    int LPMTID;
    Data->Branch("AverageMuSPMT",&AverageMuSPMT);
    Data->Branch("MuLPMT",&MuLPMT);
    Data->Branch("CalibMuLPMT",&CalibMuLPMT);
    Data->Branch("TotalPE",&DetTotalPE);
    Data->Branch("Nonlinearity",&Nonlinearity);
    Data->Branch("TrueMuLPMT",&TrueMuLPMT);
    Data->Branch("NonlinearityError",&NonlinearityError);
    Data->Branch("AverageMuSPMTError",&AverageMuSPMTError);
    Data->Branch("MuLPMTError",&MuLPMTError);
    Data->Branch("CalibMuLPMTError",&CalibMuLPMTError);
    Data->Branch("TotalPEError",&DetTotalPEError);
    Data->Branch("TrueMuLPMTError",&TrueMuLPMTError);
    Data->Branch("LPMTID",&LPMTID,"LPMTID/I");

    // 对于每一个LPMT，得到一个刻度系数
    // for (LPMTID = 0; LPMTID<2; LPMTID++)
    LPMTID = InputID;
    {
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

        TStatistic *TotalPEHist[21]; //
        for (int i = 0; i < 21; i++) //
        {
            TString histname = TString::Format("%1d MeV LPMT TotalPE hist",energy[i]);
            TotalPEHist[i] = new TStatistic(histname); 
        }

        for (int i = 0; i < 21; i++)
        {
            TFile *inputfile_SPMT_elec = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut_WithTotalPE_AllSPMT/%1dMevCharge.root",energy[i]),"read");
            TFile *inputfile_LPMT_Toy  = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/ChargeSpectrumAllLPMT/%1dMevCharge.root",energy[i]),"read");
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

                vector<double>* LPMTPE;
                vector<double>* SPMTPE;
                vector<double>* TruePE; //
                double TotalPE = 0;
                LPMT_charge->SetBranchAddress("LPMTCharge",&LPMTPE);
                SPMT_charge->SetBranchAddress("SPMTCharge",&SPMTPE);
                LPMT_charge->SetBranchAddress("TrueChargeLPMT",&TruePE); //
                LPMT_charge->SetBranchAddress("TotalPELPMT",&TotalPE);
                for (int k = 0; k < LPMT_charge->GetEntries(); k++)
                {
                    LPMT_charge->GetEntry(k);
                    SPMT_charge->GetEntry(k);
                    LPMTHist[i]->Fill((*LPMTPE)[LPMTID]);
                    TrueHist[i]->Fill((*TruePE)[LPMTID]); //
                    TotalPEHist[i]->Fill(TotalPE);
                    for (int id = 0; id < 6; id++)
                    {
                        if (CorrespondingSPMT[LPMTID][id] >= 300000)
                        {
                            SPMTHist[id][i]->Fill((*SPMTPE)[CorrespondingSPMT[LPMTID][id]-300000]);
                        }
                    }
                }
                delete LPMT_charge;
                delete SPMT_charge;
            }
            inputfile_SPMT_elec->Close();
            inputfile_LPMT_Toy->Close();
            cout<<i<<" Finished."<<endl;
        }
        
        double LPMTMeanCharge[21];
        double ErrorLPMTMeanCharge[21];
        double TrueLPMTMeanCharge[21]; //
        double ErrorTrueLPMTMeanCharge[21]; //
        double SPMTMeanCharge[6][21];
        double ErrorSPMTMeanCharge[6][21];
        double SPMTMeanCharge_tot[21];
        double ErrorSPMTMeanCharge_tot[21];
        double LPMTTotalPE[21];
        double LPMTTotalPEError[21];
        
        int n = 0;
        for (int i = 0; i < 21; i++)
        { 
            double entries = LPMTHist[i]->GetEntries();
            bool P0LargerThan0 = false;
            bool SelectHist = true;
            for (int id = 0; id < 6; id++)
            {
                if (CorrespondingSPMT[LPMTID][id] >= 300000)
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
            }
            
            if (entries > 1 && SelectHist)
            {
                LPMTMeanCharge[n] = LPMTHist[i]->GetMean();
                ErrorLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*LPMTHist[i]->GetMeanError();
                TrueLPMTMeanCharge[n] = TrueHist[i]->GetMean(); //
                ErrorTrueLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*TrueHist[i]->GetMeanError(); //
                LPMTTotalPE[n] = TotalPEHist[i]->GetMean(); 
                LPMTTotalPEError[n] = TotalPEHist[i]->GetMeanErr(); 
                
                vector<double> ToCombineSPMTMeanCharge;
                vector<double> ToErrorCombineSPMTMeanCharge;
                for (int id = 0; id < 6; id++)
                {
                    if (CorrespondingSPMT[LPMTID][id] >= 300000)
                    {
                        DataWithError RC;
                        DataWithError TC;
                        DataWithError DarkRate;
                        DarkRate.Data = SPMTDCR[CorrespondingSPMT[LPMTID][id]-300000];
                        DarkRate.Error = 0;
                        RC = GetP0Mu(SPMTHist[id][i]->GetEntries(),SPMTHist[id][i]->GetBinContent(1));
                        TC = GetTrueCharge(RC,DarkRate);

                        SPMTMeanCharge[id][n] = TC.Data;
                        ErrorSPMTMeanCharge[id][n] = TC.Error;
                        ToCombineSPMTMeanCharge.push_back(TC.Data);
                        ToErrorCombineSPMTMeanCharge.push_back(TC.Error);
                    }
                }
                DataWithError CombineCharge = CombineResult(ToCombineSPMTMeanCharge,ToErrorCombineSPMTMeanCharge);
                SPMTMeanCharge_tot[n] = CombineCharge.Data;
                ErrorSPMTMeanCharge_tot[n] = CombineCharge.Error;
                AverageMuSPMT.push_back(CombineCharge.Data);
                AverageMuSPMTError.push_back(CombineCharge.Error);
                MuLPMT.push_back(LPMTMeanCharge[n]);
                MuLPMTError.push_back(ErrorLPMTMeanCharge[n]);
                DetTotalPE.push_back(LPMTTotalPE[n]);
                DetTotalPEError.push_back(LPMTTotalPEError[n]);
                TrueMuLPMT.push_back(TrueLPMTMeanCharge[n]);
                TrueMuLPMTError.push_back(ErrorTrueLPMTMeanCharge[n]);

                ToCombineSPMTMeanCharge.clear();
                ToErrorCombineSPMTMeanCharge.clear();
                n++;
            }

        }

        if (CorrespondingSPMT[InputID][0] < 300000)
        {
            for (int i = 0; i < n; i++)
            {
                Nonlinearity.push_back(-1);
                NonlinearityError.push_back(-1);
                CalibMuLPMT.push_back(LPMTMeanCharge[i]);
                CalibMuLPMTError.push_back(ErrorLPMTMeanCharge[i]);
            }
            Data->Fill();
            outputfile->cd();
            Data->Write();
            return 0;
        }

        TF1 *f1 = new TF1("f1","[0]*x",0,10);
        f1->SetParNames("k");

        TF1 *TrueLPMT_SPMT[6];
        for (int i = 0; i < 6; i++)
        {
            TrueLPMT_SPMT[i] = new TF1(TString::Format("TrueLPMT_SPMT%1d ",i),"[0]*x-[0]*[0]*x*x*0.1/100.0",0,10);
            TrueLPMT_SPMT[i]->SetParNames("k");
            TrueLPMT_SPMT[i]->SetLineColor(4);
        }
        // TF1 *TrueLPMT_SPMT_tot;
        // TrueLPMT_SPMT_tot = new TF1("TrueLPMT_SPMT_tot","[0]*x-[0]*[0]*x*x*0.1/100.0",0,10);
        // TrueLPMT_SPMT_tot->SetParNames("k");
        // TrueLPMT_SPMT_tot->SetLineColor(4);

        //找到探测器模拟级别下，大小PMT的关系曲线
        // TFile *DetSimResultFile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/npeLPMTtonpeSPMT.root","read");
        // TGraphErrors *DetSimnpeLPMTtonpeSPMT[6];
        // for (int id = 0; id < 6; id++)
        // {
        //     TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        //     DetSimnpeLPMTtonpeSPMT[id] = (TGraphErrors*)DetSimResultFile->Get(GraphName);
        //     DetSimnpeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        //     DetSimnpeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        //     f1->SetLineColor(3);
        //     DetSimnpeLPMTtonpeSPMT[id]->Fit(f1);
        //     DetSimnpeLPMTtonpeSPMT[id]->SetLineColor(3);
        //     TrueLPMT_SPMT[id]->SetParameter(0,f1->GetParameter(0));
        //     gStyle->SetOptFit(1111);
        //     gStyle->SetStatY(0.5);
        // }

        // double SPMTMeanChargeDetSim_tot[21];
        // double ErrorSPMTMeanChargeDetSim_tot[21];
        // double LPMTMeanChargeDetSim_tot[21];
        // double ErrorLPMTMeanChargeDetSim_tot[21];
        // for (int i = 0; i < 21; i++)
        // {
        //     vector<double> ToCombineSPMTMeanCharge;
        //     vector<double> ToErrorCombineSPMTMeanCharge;
        //     for (int id = 0; id < 6; id++)
        //     {
        //         ToCombineSPMTMeanCharge.push_back(DetSimnpeLPMTtonpeSPMT[id]->GetPointX(i));
        //         ToErrorCombineSPMTMeanCharge.push_back(DetSimnpeLPMTtonpeSPMT[id]->GetErrorX(i));
        //     }
        //     DataWithError CombineCharge = CombineResult(ToCombineSPMTMeanCharge,ToErrorCombineSPMTMeanCharge);
        //     SPMTMeanChargeDetSim_tot[i] = CombineCharge.Data;
        //     ErrorSPMTMeanChargeDetSim_tot[i] = CombineCharge.Error;
        //     ToCombineSPMTMeanCharge.clear();
        //     ToErrorCombineSPMTMeanCharge.clear();

        //     LPMTMeanChargeDetSim_tot[i] = DetSimnpeLPMTtonpeSPMT[0]->GetPointY(i);
        //     ErrorLPMTMeanChargeDetSim_tot[i] = DetSimnpeLPMTtonpeSPMT[0]->GetErrorY(i);
        // }
        // TGraphErrors *DetSimnpeLPMTtonpeSPMT_tot = new TGraphErrors(21,SPMTMeanChargeDetSim_tot,LPMTMeanChargeDetSim_tot,ErrorSPMTMeanChargeDetSim_tot,ErrorLPMTMeanChargeDetSim_tot);
        // DetSimnpeLPMTtonpeSPMT_tot->SetLineColor(3);
        // DetSimnpeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("nPE SPMT");
        // DetSimnpeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("nPE LPMT");
        // f1->SetLineColor(3);
        // DetSimnpeLPMTtonpeSPMT_tot->Fit(f1,"N");
        // TrueLPMT_SPMT_tot->SetParameter(0,f1->GetParameter(0));

        // TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
        // legendb->SetTextFont(22);
        // // legendb->SetTextSize(0.04);
        // // legendb->SetBorderSize(0); // 设置边框大小为0，即去掉边框
        // // legendb->SetFillStyle(0); // 设置填充样式为0，即使图例透明

        // TCanvas *Canvas_tot = new TCanvas("ct","ct",1);

        TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,0.02);
        Below1PE->SetParNames("k");
        Below1PE->SetLineColor(kRed);
        
        TGraphErrors *npeLPMTtonpeSPMT_tot;
        TString GraphName = "LPMT npe to all SPMT npe";
        npeLPMTtonpeSPMT_tot = new TGraphErrors(n,SPMTMeanCharge_tot,LPMTMeanCharge,ErrorSPMTMeanCharge_tot,ErrorLPMTMeanCharge);
        npeLPMTtonpeSPMT_tot->SetTitle(GraphName);
        npeLPMTtonpeSPMT_tot->SetName(GraphName);
        npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu SPMT");
        npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu LPMT");
        // f1->SetLineColor(2);
        npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R");
        Below1PE->SetRange(0,10);
        // gStyle->SetOptFit(1111);
        // gStyle->SetStatY(0.5);
        // gPad->SetLogx();
        // gPad->SetLogy();
        // npeLPMTtonpeSPMT_tot->Draw("AP");
        // DetSimnpeLPMTtonpeSPMT_tot->Draw("P");
        // TrueLPMT_SPMT_tot->Draw("same");
        // Below1PE->Draw("same");

        // legendb->AddEntry(npeLPMTtonpeSPMT_tot,"With nonlinearity","lpe");
        // legendb->AddEntry(DetSimnpeLPMTtonpeSPMT_tot,"Without nonlinearity","lpe"); 
        // legendb->AddEntry(TrueLPMT_SPMT_tot,"Preset value ","l"); 
        // legendb->Draw("same");

        // outputfile->cd();
        // Canvas_tot->Write();

        // TGraphErrors *npeLPMTtonpeSPMT[6];
        // TCanvas *Canvas[6];
        // for (int id = 0; id < 6; id++)
        // {
        //     TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        //     npeLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],LPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorLPMTMeanCharge);
        //     npeLPMTtonpeSPMT[id]->SetTitle(GraphName);
        //     npeLPMTtonpeSPMT[id]->SetName(GraphName);
        //     npeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        //     npeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        //     // f1->SetLineColor(2);
        //     // npeLPMTtonpeSPMT[id]->Fit(f1);
        //     gStyle->SetOptFit(1111);
        //     gStyle->SetStatY(0.5);


        //     TString CanvasName = TString::Format("C%1d",id);
        //     Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        //     Canvas[id]->cd();
        //     gPad->SetLogx();
        //     gPad->SetLogy();
        //     npeLPMTtonpeSPMT[id]->Draw("AP");
        //     DetSimnpeLPMTtonpeSPMT[id]->Draw("p");
        //     TrueLPMT_SPMT[id]->Draw("same");
        //     outputfile->cd();
        //     npeLPMTtonpeSPMT[id]->Write();
        // }

        // TF1 *SetNonlinearity = new TF1("SetNonlinearity","(1-0.1*x/100.0)/(1-0.1*[0]/100.0)-1",0,120);
        // TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,0,120,1);

        double ArrayCalibNonlinearity[21];//刻度的非线性
        double ErrorCalibNonlinearity[21];
        // TGraphErrors *CalibNonlinearity;
        // DataWithError ChooseSPMTPoint;
        // DataWithError ChooseLPMTPoint;
        // ChooseLPMTPoint.Data = LPMTMeanCharge[8];
        // ChooseLPMTPoint.Error = ErrorLPMTMeanCharge[8];
        // ChooseSPMTPoint.Data = SPMTMeanCharge_tot[8];
        // ChooseSPMTPoint.Error = ErrorSPMTMeanCharge_tot[8];

        // double TrueLPMTPoint = 500-sqrt(500*500-1000*ChooseLPMTPoint.Data);
        // 采用归一化到一个点的方法计算非线性
        // SetNonlinearity->SetParameter(0,TrueLPMTPoint);
        // 采用拟合LPMT小于1PE的点的方式计算非线性
        // SetNonlinearity->SetParameter(0,0);
        for (int i = 0; i < n; i++)
        {
            // 采用归一化到一个点的方法计算非线性
            // ArrayCalibNonlinearity[i] = (npeLPMTtonpeSPMT_tot->GetPointY(i)/ChooseLPMTPoint.Data)/(npeLPMTtonpeSPMT_tot->GetPointX(i)/ChooseSPMTPoint.Data)-1;
            // double GetError = 0;
            // double a = npeLPMTtonpeSPMT_tot->GetPointY(i);
            // double ea = npeLPMTtonpeSPMT_tot->GetErrorY(i);
            // double b = ChooseLPMTPoint.Data;
            // double eb = ChooseLPMTPoint.Error;
            // double c = npeLPMTtonpeSPMT_tot->GetPointX(i);
            // double ec = npeLPMTtonpeSPMT_tot->GetErrorX(i);
            // double d = ChooseSPMTPoint.Data;
            // double ed = ChooseSPMTPoint.Error;
            // GetError = (ea*ea)/(a*a) + (eb*eb)/(b*b) + (ec*ec)/(c*c) + (ed*ed)/(d*d);
            // ErrorCalibNonlinearity[i] = (ArrayCalibNonlinearity[i]+1)*sqrt(GetError);

            // 采用拟合LPMT小于1PE的点的方式计算非线性
            ArrayCalibNonlinearity[i] = npeLPMTtonpeSPMT_tot->GetPointY(i)/(npeLPMTtonpeSPMT_tot->GetPointX(i)*Below1PE->GetParameter(0))-1;
            double GetError = 0;
            double a = npeLPMTtonpeSPMT_tot->GetPointY(i);
            double ea = npeLPMTtonpeSPMT_tot->GetErrorY(i);
            double b = npeLPMTtonpeSPMT_tot->GetPointX(i);
            double eb = npeLPMTtonpeSPMT_tot->GetErrorX(i);
            double c = Below1PE->GetParameter(0);
            double ec = Below1PE->GetParError(0);
            GetError = (ea*ea)/(a*a) + (eb*eb)/(b*b) + (ec*ec)/(c*c);
            ErrorCalibNonlinearity[i] = (ArrayCalibNonlinearity[i]+1)*sqrt(GetError);
            Nonlinearity.push_back(ArrayCalibNonlinearity[i]);
            NonlinearityError.push_back(ErrorCalibNonlinearity[i]);

        }
        // CalibNonlinearity = new TGraphErrors(n,LPMTMeanCharge,ArrayCalibNonlinearity,ErrorLPMTMeanCharge,ErrorCalibNonlinearity);
        // TCanvas *CanvasNonlinearity = new TCanvas("CanvasNonlinearity","CanvasNonlinearity",1);
        // CalibNonlinearity->GetXaxis()->SetTitle("#mu_{LPMT}");
        // CalibNonlinearity->GetYaxis()->SetTitle("Nonlinearity");
        // CalibNonlinearity->SetTitle("");
        // gPad->SetLogx();
        // CalibNonlinearity->Draw("AP");
        // SetNonlinearity->Draw("same");
        // TLegend *legenda =new TLegend(0.6,0.65,0.88,0.85);
        // legenda->SetTextFont(22);
        // legenda->AddEntry(CalibNonlinearity,"Calibrated nonliearity","lpe");
        // legenda->AddEntry(SetNonlinearity,"Preset value","l"); 
        // legenda->Draw("same");
        // CanvasNonlinearity->cd();

        // 尝试修正单通道的非线性，得到残余非线性
        double LPMTMeanChargeAfterCalib[21];
        double LPMTMeanChargeAfterCalibError[21];
        for (int i = 0; i < n; i++)
        {
            LPMTMeanChargeAfterCalib[i] = LPMTMeanCharge[i]/(ArrayCalibNonlinearity[i]+1);
            LPMTMeanChargeAfterCalibError[i] = LPMTMeanChargeAfterCalib[i]*
                                            sqrt((ErrorLPMTMeanCharge[i]*ErrorLPMTMeanCharge[i])/(LPMTMeanCharge[i]*LPMTMeanCharge[i])
                                            + (ErrorCalibNonlinearity[i]*ErrorCalibNonlinearity[i])/((ArrayCalibNonlinearity[i]+1)*(ArrayCalibNonlinearity[i]+1)));
            CalibMuLPMT.push_back(LPMTMeanChargeAfterCalib[i]);
            CalibMuLPMTError.push_back(LPMTMeanChargeAfterCalibError[i]);
        }
        Data->Fill();

        // TGraphErrors *CalibLPMTtoTotalPE = new TGraphErrors(n,LPMTTotalPE,LPMTMeanChargeAfterCalib,LPMTTotalPEError,LPMTMeanChargeAfterCalibError);
        // TGraphErrors *LPMTtoTotalPE = new TGraphErrors(n,LPMTTotalPE,LPMTMeanCharge,LPMTTotalPEError,ErrorLPMTMeanCharge);
        // TGraphErrors *TrueLPMTPEToTotalPE = new TGraphErrors(n,LPMTTotalPE,TrueLPMTMeanCharge,LPMTTotalPEError,ErrorTrueLPMTMeanCharge);
        // // TGraphErrors *CalibLPMTtoTotalPE = new TGraphErrors(n,LPMTMeanChargeAfterCalib,LPMTTotalPE,LPMTMeanChargeAfterCalibError,LPMTTotalPEError);
        // // TGraphErrors *LPMTtoTotalPE = new TGraphErrors(n,LPMTMeanCharge,LPMTTotalPE,ErrorLPMTMeanCharge,LPMTTotalPEError);
        // CalibLPMTtoTotalPE->GetXaxis()->SetTitle("Total PE");
        // CalibLPMTtoTotalPE->GetYaxis()->SetTitle("#mu_{LPMT}");
        // CalibLPMTtoTotalPE->SetTitle("");
        // CalibLPMTtoTotalPE->SetFillStyle(3002);
        // CalibLPMTtoTotalPE->SetMarkerColor(4);
        // CalibLPMTtoTotalPE->SetLineColor(4);
        // CalibLPMTtoTotalPE->SetMarkerSize(1.0);
        // CalibLPMTtoTotalPE->SetMarkerStyle(20);
        // CalibLPMTtoTotalPE->SetLineWidth(3);
        // CalibLPMTtoTotalPE->GetXaxis()->CenterTitle();
        // CalibLPMTtoTotalPE->GetYaxis()->CenterTitle();
        // CalibLPMTtoTotalPE->GetXaxis()->SetTitleFont(22);
        // CalibLPMTtoTotalPE->GetYaxis()->SetTitleFont(22);
        // CalibLPMTtoTotalPE->GetXaxis()->SetTitleSize(0.06);
        // CalibLPMTtoTotalPE->GetYaxis()->SetTitleSize(0.06);
        // CalibLPMTtoTotalPE->GetXaxis()->SetLabelSize(0.06);
        // CalibLPMTtoTotalPE->GetYaxis()->SetLabelSize(0.06);
        // CalibLPMTtoTotalPE->GetXaxis()->SetTitleOffset(0.83);
        // CalibLPMTtoTotalPE->GetYaxis()->SetTitleOffset(0.83);


        // LPMTtoTotalPE->SetLineColor(2);
        // LPMTtoTotalPE->SetFillStyle(3002);
        // LPMTtoTotalPE->SetMarkerColor(2);
        // LPMTtoTotalPE->SetMarkerSize(1.0);
        // LPMTtoTotalPE->SetMarkerStyle(20);
        // LPMTtoTotalPE->SetLineWidth(3);
        // LPMTtoTotalPE->GetXaxis()->CenterTitle();
        // LPMTtoTotalPE->GetYaxis()->CenterTitle();
        // LPMTtoTotalPE->GetXaxis()->SetTitleFont(22);
        // LPMTtoTotalPE->GetYaxis()->SetTitleFont(22);

        // TF1 *Below1PETotalPE = new TF1("Below1PE","[0]*x",0,20000000);
        // Below1PETotalPE->SetParNames("k");
        // // Below1PETotalPE->SetLineColor(kRed);
        // Below1PETotalPE->SetLineColor(1);
        // Below1PETotalPE->SetLineStyle(2);

        // TrueLPMTPEToTotalPE->Fit(Below1PETotalPE);
        // double TrueLPMTPEToTotalPE_k = Below1PETotalPE->GetParameter(0);
        // double TrueLPMTPEToTotalPE_kError = Below1PETotalPE->GetParError(0);
        // TCanvas *TotalPECanvas = new TCanvas("TotalPECanvas","TotalPECanvas",1400,1000);    
        // TLegend *legendTotalPE =new TLegend(0.6,0.65,0.88,0.85);
        // legendTotalPE->SetTextFont(22);
        // legendTotalPE->AddEntry(CalibLPMTtoTotalPE,"After Calibration","lpe");
        // legendTotalPE->AddEntry(LPMTtoTotalPE,"Before Calibration","lpe"); 
        // legendTotalPE->SetBorderSize(0); // 设置边框大小为0，即去掉边框
        // legendTotalPE->SetFillStyle(0); // 设置填充样式为0，即使图例透明
        // legendTotalPE->Draw("same");
        // TotalPECanvas->cd();
        // CalibLPMTtoTotalPE->Draw("AP");
        // LPMTtoTotalPE->Draw("P");
        // Below1PETotalPE->Draw("same");
        // legendTotalPE->Draw("same");

        // // 残余非线性
        // double ReminNonlinearity[21];
        // double ReminNonlinearityError[21];
        // for (int i = 0; i < n; i++)
        // {
        //     ReminNonlinearity[i] = LPMTMeanChargeAfterCalib[i]/(TrueLPMTPEToTotalPE_k*LPMTTotalPE[i]) -1;
        //     ReminNonlinearityError[i] = (ReminNonlinearity[i] + 1)*sqrt((LPMTMeanChargeAfterCalibError[i]*LPMTMeanChargeAfterCalibError[i])/(LPMTMeanChargeAfterCalib[i]*LPMTMeanChargeAfterCalib[i])
        //                                                                 +(TrueLPMTPEToTotalPE_kError*TrueLPMTPEToTotalPE_kError)/(TrueLPMTPEToTotalPE_k*TrueLPMTPEToTotalPE_k)
        //                                                                 +(LPMTTotalPEError[i]*LPMTTotalPEError[i])/(LPMTTotalPE[i]*LPMTTotalPE[i]) );
        // }
        // TGraphErrors *ReminNonlinearitytoTotalPE = new TGraphErrors(n,LPMTTotalPE,ReminNonlinearity,LPMTTotalPEError,ReminNonlinearityError);
        // ReminNonlinearitytoTotalPE->GetXaxis()->SetTitle("Total PE");
        // ReminNonlinearitytoTotalPE->GetYaxis()->SetTitle("ReminNonlinearity");
        // ReminNonlinearitytoTotalPE->SetTitle("");
        // ReminNonlinearitytoTotalPE->SetFillStyle(3002);
        // ReminNonlinearitytoTotalPE->SetMarkerColor(4);
        // ReminNonlinearitytoTotalPE->SetLineColor(4);
        // ReminNonlinearitytoTotalPE->SetMarkerSize(1.0);
        // ReminNonlinearitytoTotalPE->SetMarkerStyle(20);
        // ReminNonlinearitytoTotalPE->SetLineWidth(3);
        // ReminNonlinearitytoTotalPE->GetXaxis()->CenterTitle();
        // ReminNonlinearitytoTotalPE->GetYaxis()->CenterTitle();
        // ReminNonlinearitytoTotalPE->GetXaxis()->SetTitleFont(22);
        // ReminNonlinearitytoTotalPE->GetYaxis()->SetTitleFont(22);
        // ReminNonlinearitytoTotalPE->GetXaxis()->SetTitleSize(0.06);
        // ReminNonlinearitytoTotalPE->GetYaxis()->SetTitleSize(0.06);
        // ReminNonlinearitytoTotalPE->GetXaxis()->SetLabelSize(0.06);
        // ReminNonlinearitytoTotalPE->GetYaxis()->SetLabelSize(0.06);
        // ReminNonlinearitytoTotalPE->GetXaxis()->SetTitleOffset(0.83);
        // ReminNonlinearitytoTotalPE->GetYaxis()->SetTitleOffset(0.83);

        // TCanvas *ReminNonlinearityCanvas = new TCanvas("ReminNonlinearityCanvas","ReminNonlinearityCanvas",1);
        // ReminNonlinearitytoTotalPE->Draw("AP");
        AverageMuSPMT.clear();
        MuLPMT.clear();
        CalibMuLPMT.clear();
        DetTotalPE.clear();
        Nonlinearity.clear();
        TrueMuLPMT.clear();
        NonlinearityError.clear();
        AverageMuSPMTError.clear();
        MuLPMTError.clear();
        CalibMuLPMTError.clear();
        DetTotalPEError.clear();
        TrueMuLPMTError.clear();
    }
    outputfile->cd();
    Data->Write();
    return 0;
    
}