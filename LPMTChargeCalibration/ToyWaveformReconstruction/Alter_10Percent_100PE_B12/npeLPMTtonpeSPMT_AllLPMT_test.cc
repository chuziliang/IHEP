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
    for (int i = 1; i < 20; i++)//均值为“xx”时，测到i PE且过阈的概率。认为“xx”几乎不会超过5PE
    {
        P0 += ROOT::Math::poisson_pdf(i,xx)*ROOT::Math::normal_cdf(par[0],par[1]*sqrt((double)i),(double)i);
    }
    return -TMath::Log(P0);
}

DataWithError GetTrueCharge(DataWithError RoughCharge, DataWithError DarkRate)//RoughCharge为修正前的电荷，输出修正后的电荷
{
    DataWithError TrueCharge;
    // if (DarkRate.Data > RoughCharge.Data)
    // {
    //     TrueCharge.Data = -1;
    //     TrueCharge.Error = -1;
    // }
    double R_Spe = 33.2/100.0;  //J22.1.0-rc4，SPMT电子学模拟中使用了固定分辨率
    double Threshold = 1.0/3.0; //J22.1.0-rc4，SPMT电子学模拟中使用了固定阈值
    auto f = new TF1("function",Myfunction,0,5,3);
    f->SetParameters(Threshold,R_Spe,DarkRate.Data);
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
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map_J23.1.0-rc2.txt");
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
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc8.dc1/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
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
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc8.dc1/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
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
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc8.dc1/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
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

double GetDistanceToBar(double (&DistanceToBar)[17612])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/PMTID_map_J23_1_0_rc2.txt");
    string line;
    while (getline(file, line)) 
    { 
        stringstream ss(line);
        int LPMTID;
        double Distance;
        ss >> LPMTID >> Distance;
        DistanceToBar[LPMTID] = Distance;
    }

    file.close();
    return 0;
}

int npeLPMTtonpeSPMT_AllLPMT_test()
{
    // 初始化要用到的PMT参数
    double LPMTQE[17612];
    double SPMTQE[25600];
    double SPMTDCR[25600];
    int CorrespondingSPMT[17612][6];
    double DistanceToBar[25600];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    GetSPMTDCR(SPMTDCR);

    // 定义输出文件及输出格式
    TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/npeLPMTtonpeSPMT_B12.root","RECREATE");
    TTree *Data = new TTree("Data","Data");
    for (int dir = 0; dir < 176; dir++)
    {
        outputfile->mkdir(TString::Format("%1d/%1d",dir/10,dir%10));
        outputfile->cd();
    }

    vector<vector<double>> MuSPMT;
    vector<vector<double>> MuSPMTError;
    
    vector<double> AverageMuSPMT_QE;
    vector<double> MuLPMT;
    vector<double> CalibMuLPMT;
    vector<double> DetTotalPE;
    vector<double> Nonlinearity;
    vector<double> TrueMuLPMT;

    vector<double> NonlinearityError;
    vector<double> AverageMuSPMT_QEError;
    vector<double> MuLPMTError;
    vector<double> CalibMuLPMTError;
    vector<double> DetTotalPEError;
    vector<double> TrueMuLPMTError;
    int LPMTID;

    // 增加刻度系数、所有SPMT的Mu值及误差
    Data->Branch("AverageMuSPMT_QE",&AverageMuSPMT_QE);
    Data->Branch("MuLPMT",&MuLPMT);
    Data->Branch("CalibMuLPMT",&CalibMuLPMT);
    Data->Branch("TotalPE",&DetTotalPE);
    Data->Branch("Nonlinearity",&Nonlinearity);
    Data->Branch("TrueMuLPMT",&TrueMuLPMT);
    Data->Branch("NonlinearityError",&NonlinearityError);
    Data->Branch("AverageMuSPMT_QEError",&AverageMuSPMT_QEError);
    Data->Branch("MuLPMTError",&MuLPMTError);
    Data->Branch("CalibMuLPMTError",&CalibMuLPMTError);
    Data->Branch("TotalPEError",&DetTotalPEError);
    Data->Branch("TrueMuLPMTError",&TrueMuLPMTError);
    Data->Branch("MuSPMT",&MuSPMT);
    Data->Branch("MuSPMTError",&MuSPMTError);
    Data->Branch("LPMTID",&LPMTID,"LPMTID/I");


    // 定义需要计算的数据，完成初始化
    vector<vector<double>> LPMTMu(17612,vector<double>(21,0));
    vector<vector<double>> LPMTMuErr(17612,vector<double>(21,0));

    vector<vector<int>> SPMT0(25600,vector<int>(21,0));
    vector<vector<int>> SPMTTotal(25600,vector<int>(21,0));

    vector<vector<double>> SPMTMu(25600,vector<double>(21,0));
    vector<vector<double>> SPMTMuErr(25600,vector<double>(21,0));

    vector<vector<double>> LPMTTrueMu(17612,vector<double>(21,0));
    vector<vector<double>> LPMTTrueMuErr(17612,vector<double>(21,0));

    vector<double> TotalPEMean(21,0);
    vector<double> TotalPEMeanErr(21,0);

    // 读取文件，计算数据
    for (int i = 0; i < 21; i++) // 总共21个能量
    {
        TFile *inputfile_SPMT_elec = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut_WithTotalPE_AllSPMT_AllLPMT/%1dMevCharge.root",energy[i]),"read");
        TFile *inputfile_LPMT_Toy  = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/ChargeSpectrumAllLPMT/%1dMevCharge.root",energy[i]),"read");
        if (!inputfile_SPMT_elec || !inputfile_LPMT_Toy)
        {
            continue;
        }

        double TotalEntries = 0;
        for (int j = 0; j < numbers[i]; j++) // 每个能量下的文件
        {
            TTree *SPMT_charge = (TTree*)inputfile_SPMT_elec->Get(TString::Format("Charge%1d",j));
            TTree *LPMT_charge = (TTree*)inputfile_LPMT_Toy->Get(TString::Format("Charge%1d",j));
            if (!SPMT_charge || !LPMT_charge)
            {
                continue;
            }

            vector<double>* LPMTPE = nullptr;
            vector<double>* SPMTPE = nullptr;
            vector<double>* TruePE = nullptr;
            double TotalPE = 0;
            LPMT_charge->SetBranchAddress("LPMTCharge",&LPMTPE);
            SPMT_charge->SetBranchAddress("SPMTCharge",&SPMTPE);
            LPMT_charge->SetBranchAddress("TrueChargeLPMT",&TruePE); //
            LPMT_charge->SetBranchAddress("TotalPELPMT",&TotalPE);
            TotalEntries += (double)LPMT_charge->GetEntries();

            for (int k = 0; (k < LPMT_charge->GetEntries()) && (k < SPMT_charge->GetEntries()); k++) // 每个文件下的事例
            {
                LPMT_charge->GetEntry(k);
                SPMT_charge->GetEntry(k);
                
                TotalPEMean[i] += TotalPE;
                TotalPEMeanErr[i] += TotalPE*TotalPE;

                for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
                {
                    LPMTMu[LPMTID][i] += (*LPMTPE)[LPMTID];
                    LPMTMuErr[LPMTID][i] += (*LPMTPE)[LPMTID]*(*LPMTPE)[LPMTID];
                    LPMTTrueMu[LPMTID][i] += (*TruePE)[LPMTID];
                    LPMTTrueMuErr[LPMTID][i] += (*TruePE)[LPMTID]*(*TruePE)[LPMTID];
                }

                for (int SPMTID = 0; SPMTID < 25600; SPMTID++)
                {
                    if ((*SPMTPE)[SPMTID] == 0)
                    {
                        SPMT0[SPMTID][i]++;
                    }
                    SPMTTotal[SPMTID][i]++;
                }
            }

            delete LPMT_charge;
            delete SPMT_charge;
        }

        TotalPEMean[i] /= TotalEntries;
        TotalPEMeanErr[i] /= TotalEntries;
        TotalPEMeanErr[i] -= TotalPEMean[i]*TotalPEMean[i];
        TotalPEMeanErr[i] = sqrt(TotalPEMeanErr[i]/TotalEntries);

        for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
        {
            LPMTMu[LPMTID][i] /= TotalEntries;
            LPMTMuErr[LPMTID][i] /= TotalEntries;
            LPMTMuErr[LPMTID][i] -= LPMTMu[LPMTID][i]*LPMTMu[LPMTID][i];
            LPMTMuErr[LPMTID][i] = sqrt(LPMTMuErr[LPMTID][i]/TotalEntries);

            LPMTTrueMu[LPMTID][i] /= TotalEntries;
            LPMTTrueMuErr[LPMTID][i] /= TotalEntries;
            LPMTTrueMuErr[LPMTID][i] -= LPMTTrueMu[LPMTID][i]*LPMTTrueMu[LPMTID][i];
            LPMTTrueMuErr[LPMTID][i] = sqrt(LPMTTrueMuErr[LPMTID][i]/TotalEntries);
        }

        for (int SPMTID = 0; SPMTID < 25600; SPMTID++)
        {
            DataWithError RC;
            DataWithError TC;
            DataWithError DarkRate;
            DarkRate.Data = SPMTDCR[SPMTID];
            DarkRate.Error = 0;
            if (SPMT0[SPMTID][i]>0)
            {
                RC = GetP0Mu(SPMTTotal[SPMTID][i],SPMT0[SPMTID][i]);
                TC = GetTrueCharge(RC,DarkRate);
                if (TMath::IsNaN(TC.Data) )
                {
                    cout<<"Can't find a root!!!"<<endl;
                    cout<<"PMTID="<<SPMTID<<";  Energy="<<energy[i]<<";  RC="<<RC.Data<<";  RC.Error="<<RC.Error<<";  DCR="<<DarkRate.Data<<endl;
                    SPMTMu[SPMTID][i] = -1;
                    SPMTMuErr[SPMTID][i] = -1;
                }
                else
                {
                    SPMTMu[SPMTID][i] = TC.Data;
                    SPMTMuErr[SPMTID][i] = TC.Error;
                }
            }
            else
            {
                SPMTMu[SPMTID][i] = -1;
                SPMTMuErr[SPMTID][i] = -1;
            }
        }

        inputfile_SPMT_elec->Close();
        inputfile_LPMT_Toy->Close();
        cout<<i<<" Finished."<<endl;
    }

    // 拟合及画图
    for (LPMTID = 0; LPMTID < 17612; LPMTID++)
    {
        // 已经得到的数值
        MuLPMT = LPMTMu[LPMTID]; //LPMT μ值
        MuLPMTError = LPMTMuErr[LPMTID]; //LPMT μ值的误差
        TrueMuLPMT = LPMTTrueMu[LPMTID]; //真实的LPMT μ值
        TrueMuLPMTError = LPMTTrueMuErr[LPMTID]; //真实的LPMT μ值的误差
        DetTotalPE = TotalPEMean; //LPMT TotalPE
        DetTotalPEError = TotalPEMeanErr; //LPMT TotalPE的误差

        // 需要拟合或者计算得到的数值
        AverageMuSPMT_QE = vector<double>(21,-1); //6个corrresponding SPMT μ值的均值
        AverageMuSPMT_QEError = vector<double>(21,-1); //6个corrresponding SPMT μ值均值的误差
        CalibMuLPMT = vector<double>(21,-1); //刻度后LPMT的Mu值
        CalibMuLPMTError = vector<double>(21,-1); //刻度后LPMT Mu值的误差
        Nonlinearity = vector<double>(21,-1); // 刻度得到的非线性
        NonlinearityError = vector<double>(21,-1); // 刻度的非线性的误差
        MuSPMT = vector<vector<double>>(6,vector<double>(21,-1)); //corresponding SPMT μ值
        MuSPMTError = vector<vector<double>>(6,vector<double>(21,-1)); //corresponding SPMT μ值的误差

        // 中间变量
        vector<double> MuLPMT_PDE = vector<double>(21,-1);
        vector<double> MuLPMT_PDEError = vector<double>(21,-1);

        // 如果没有corresponding SPMT, 不做刻度
        if (CorrespondingSPMT[LPMTID][0] < 300000)
        {
            for (int i = 0; i < 21; i++)
            {
                CalibMuLPMT[i] = MuLPMT[i];
                CalibMuLPMTError[i] = MuLPMTError[i];
            }
            Data->Fill();
            outputfile->cd();
            // Data->Write();

            AverageMuSPMT_QE.clear();
            MuLPMT.clear();
            CalibMuLPMT.clear();
            DetTotalPE.clear();
            Nonlinearity.clear();
            TrueMuLPMT.clear();
            NonlinearityError.clear();
            AverageMuSPMT_QEError.clear();
            MuLPMTError.clear();
            CalibMuLPMTError.clear();
            DetTotalPEError.clear();
            TrueMuLPMTError.clear();
            continue;
        }

        // 计算corresponding SPMT μ的均值和误差，计算MuLPMT_PDE的均值和误差
        bool ChosePoint[21];
        int FitPoints = 0;
        for (int i = 0; i < 21; i++) // 得到MuSPMT、AverageMuSPMT_QE、MuLPMT_PDE
        {
            MuLPMT_PDE[i] = MuLPMT[i]/LPMTQE[LPMTID];
            MuLPMT_PDEError[i] = MuLPMTError[i]/LPMTQE[LPMTID];
            if (MuLPMT[i] < 1) FitPoints = i;

            vector<double> ToCombineSPMTMeanCharge;
            vector<double> ToErrorCombineSPMTMeanCharge;
            for (int id = 0; id < 6; id++)
            {
                if ((CorrespondingSPMT[LPMTID][id] >= 300000))
                {
                    MuSPMT[id][i] = SPMTMu[CorrespondingSPMT[LPMTID][id] - 300000][i];
                    MuSPMTError[id][i] = SPMTMuErr[CorrespondingSPMT[LPMTID][id] - 300000][i];
                    if (MuSPMT[id][i] > 0)
                    {
                        ToCombineSPMTMeanCharge.push_back(MuSPMT[id][i]/SPMTQE[CorrespondingSPMT[LPMTID][id] - 300000]);
                        ToErrorCombineSPMTMeanCharge.push_back(MuSPMTError[id][i]/SPMTQE[CorrespondingSPMT[LPMTID][id] - 300000]);
                    }
                }
            }

            if (ToCombineSPMTMeanCharge.size()>0)
            {
                DataWithError CombineCharge = CombineResult(ToCombineSPMTMeanCharge,ToErrorCombineSPMTMeanCharge);
                AverageMuSPMT_QE[i] = CombineCharge.Data;
                AverageMuSPMT_QEError[i] = CombineCharge.Error;
                ChosePoint[i] = true;
            }
            else
            {
                cout<<"LPMT_"<<LPMTID<<" Point"<<i<<" not correct"<<endl;
                AverageMuSPMT_QE[i] = -1;
                AverageMuSPMT_QEError[i] = -1;
                ChosePoint[i] = false;
            }
            

            ToCombineSPMTMeanCharge.clear();
            ToErrorCombineSPMTMeanCharge.clear();
        }

        TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,AverageMuSPMT_QE[FitPoints]);
        Below1PE->SetParNames("k");
        Below1PE->SetLineColor(kRed);
        Below1PE->SetParameter(0,50);
        
        TGraphErrors *npeLPMTtonpeSPMT_tot;
        TString GraphName = TString::Format("LPMT_%1d npe to all SPMT npe",LPMTID);
        npeLPMTtonpeSPMT_tot = new TGraphErrors(21,&AverageMuSPMT_QE[0],&MuLPMT_PDE[0],&AverageMuSPMT_QEError[0],&MuLPMT_PDEError[0]);
        npeLPMTtonpeSPMT_tot->SetTitle(GraphName);
        npeLPMTtonpeSPMT_tot->SetName(GraphName);
        npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("Average #mu_{SPMT}/QE");
        npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("#mu_{LPMT}/PDE");
        for (int i = 0; i < 21; i++) // 去掉不正常的点
        {
            if ( !ChosePoint[i] )
            {
                npeLPMTtonpeSPMT_tot->RemovePoint(i);
            }
        }
        // f1->SetLineColor(2);
        npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R F");
        Below1PE->SetRange(0,10);

        // double TrueLPMTPoint = 500-sqrt(500*500-1000*ChooseLPMTPoint.Data);
        // 采用归一化到一个点的方法计算非线性
        // SetNonlinearity->SetParameter(0,TrueLPMTPoint);
        // 采用拟合LPMT小于1PE的点的方式计算非线性
        // SetNonlinearity->SetParameter(0,0);
        for (int i = 0; i < 21; i++) // 计算Nonlinearity和Error
        {
            if (!ChosePoint[i])
            {
                continue;
            }

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
            Nonlinearity[i] = npeLPMTtonpeSPMT_tot->GetPointY(i)/(npeLPMTtonpeSPMT_tot->GetPointX(i)*Below1PE->GetParameter(0))-1;
            double GetError = 0;
            double a = npeLPMTtonpeSPMT_tot->GetPointY(i);
            double ea = npeLPMTtonpeSPMT_tot->GetErrorY(i);
            double b = npeLPMTtonpeSPMT_tot->GetPointX(i);
            double eb = npeLPMTtonpeSPMT_tot->GetErrorX(i);
            double c = Below1PE->GetParameter(0);
            double ec = Below1PE->GetParError(0);
            GetError = (ea*ea)/(a*a) + (eb*eb)/(b*b) + (ec*ec)/(c*c);
            NonlinearityError[i] = (Nonlinearity[i]+1)*sqrt(GetError);

        }

        // 尝试修正单通道的非线性，得到残余非线性
        for (int i = 0; i < 21; i++) // 计算CalibMuLPMT和error
        {
            if (!ChosePoint[i])
            {
                continue;
            }

            CalibMuLPMT[i] = MuLPMT[i]/(Nonlinearity[i]+1);
            CalibMuLPMTError[i] = CalibMuLPMT[i]*
                                            sqrt((MuLPMTError[i]*MuLPMTError[i])/(MuLPMT[i]*MuLPMT[i])
                                            + (NonlinearityError[i]*NonlinearityError[i])/((Nonlinearity[i]+1)*(Nonlinearity[i]+1)));
        }
        Data->Fill();

        outputfile->cd(TString::Format("%1d/%1d",LPMTID/1000,(LPMTID%1000)/100));
        npeLPMTtonpeSPMT_tot->Write();
        outputfile->cd();

        AverageMuSPMT_QE.clear();
        MuLPMT.clear();
        CalibMuLPMT.clear();
        DetTotalPE.clear();
        Nonlinearity.clear();
        TrueMuLPMT.clear();
        NonlinearityError.clear();
        AverageMuSPMT_QEError.clear();
        MuLPMTError.clear();
        CalibMuLPMTError.clear();
        DetTotalPEError.clear();
        TrueMuLPMTError.clear();

        cout<<"PMT_"<<LPMTID<<" Finished"<<endl;
    }
    outputfile->cd();
    Data->Write();
    return 0;
}