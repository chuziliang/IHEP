// 这个脚本用于把真实的非线性曲线、P（0）和nPE方法得到的非线性曲线画在一起对比
// 需要先运行以下程序：
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/GetTruePE.cc”
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/P0method/npeLPMTtonpeSPMT.cc”
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/nPEmethod/npeLPMTtonpeSPMT.cc”
// 这里P0和nPE的曲线是这两种方法自身的非线性曲线（SPMT自身电荷）
// 经过暗噪声和阈值效应的修正
// 考虑所有的SPMT
using namespace std;
// using namespace JM;

double iPEP0[19];

struct DataWithError
{
    double Data;
    double Error;
};

int GetP0()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/SPMTElectronicsNonlinearity/Nonlinerity.root","read");
    for (int i = 1; i < 20; i++)
    {
        TString histname = TString::Format("%1d PE calib charge",i);
        TH1 *CalibHist = (TH1F*)inputfile->Get(histname);
        iPEP0[i-1] = (double)CalibHist->GetBinContent(1)/CalibHist->GetEntries();
        delete CalibHist;
    }
    inputfile->Close();
    return 0;
}

double Myfunction(double* x, double* par) //par[0] = DarkRate, par[1~19] = iPEP0
{
    double P0 = 0;
    double DarkRateCharge = par[2]*1e-6;//1000ns内暗噪声贡献的电荷的均值
    double xx = x[0]+DarkRateCharge;//暗噪声和物理事例总的电荷均值
    P0 += ROOT::Math::poisson_pdf(0,xx);//均值为“xx”时，测到0 PE的概率
    for (int i = 1; i < 10; i++)//均值为“xx”时，测到i PE且过阈的概率。认为“xx”几乎不会超过5PE
    {
        double ThisP0 = iPEP0[i-1]; //从nPE的电荷谱中，获得nPE时未触发的概率
        P0 += ROOT::Math::poisson_pdf(i,xx)*ThisP0;
    }
    return -TMath::Log(P0);
}

//修正前后电荷的对应关系函数，这个函数的自变量是真实电荷，因变量是包含阈值和暗噪声效应的电荷
// double Myfunction(double* x, double* par) //par[0] = Threshold, par[1] = Sigma_SPE, par[2] = DarkRate
// {
//     double P0 = 0;
//     double DarkRateCharge = par[2]*1e-6;//1000ns内暗噪声贡献的电荷的均值
//     double xx = x[0]+DarkRateCharge;//暗噪声和物理事例总的电荷均值
//     P0 += ROOT::Math::poisson_pdf(0,xx);//均值为“xx”时，测到0 PE的概率
//     for (int i = 1; i < 5; i++)//均值为“xx”时，测到i PE且过阈的概率。认为“xx”几乎不会超过5PE
//     {
//         P0 += ROOT::Math::poisson_pdf(i,xx)*ROOT::Math::normal_cdf(par[0],par[1]*sqrt((double)i),(double)i);
//     }
//     return -TMath::Log(P0);
// }

DataWithError GetTrueCharge(DataWithError RoughCharge, DataWithError DarkRate)//RoughCharge为修正前的电荷，输出修正后的电荷
{
    DataWithError TrueCharge;
    if (RoughCharge.Data >= DarkRate.Data*1e-6 )
    {
        double R_Spe = 33.2/100.0;  //J22.1.0-rc4，SPMT电子学模拟中使用了固定分辨率
        double Threshold = 1.0/3.0; //J22.1.0-rc4，SPMT电子学模拟中使用了固定阈值
        auto f = new TF1("function",Myfunction,0,5,3);
        f->SetParameters(Threshold,R_Spe,DarkRate.Data);
        TrueCharge.Data = f->GetX(RoughCharge.Data,0,0,1.E-12,1000,false);//反解出真实电荷
        //传递误差
        double df;
        df = f->Derivative(TrueCharge.Data,nullptr,1.e-5);
        TrueCharge.Error = sqrt( RoughCharge.Error*RoughCharge.Error*(df*df) + DarkRate.Error*DarkRate.Error*1e-6*1e-6 );
    }
    else
    {
        TrueCharge.Data = -1;
        TrueCharge.Error = -1;
    }
    return TrueCharge;
}

DataWithError GetTrueCharge_ChargeMethod(DataWithError RoughCharge, DataWithError DarkRate)//RoughCharge为修正前的电荷，输出修正后的电荷
{
    DataWithError TrueCharge;
    if (RoughCharge.Data >= DarkRate.Data*1e-6 )
    {
        TrueCharge.Data = RoughCharge.Data - DarkRate.Data*1e-6;
        TrueCharge.Error = RoughCharge.Error;
    }
    else
    {
        TrueCharge.Data = -1;
        TrueCharge.Error = -1;
    }
    return TrueCharge;
}

DataWithError Total(const DataWithError data[25600])
{
    DataWithError result;
    double sum_x = 0.0;
    double sum_w = 0.0;
    int num = 0;
    for (int j = 0; j < 25600; j++)
    {
        if (data[j].Data >= 0)
        {
            sum_x += data[j].Data;
            double w = data[j].Error * data[j].Error;
            sum_w += w;
            num++;
        }
    }
    double combined_x = sum_x /num;
    double combined_e = sqrt(sum_w)/num;
    result.Data = combined_x;
    result.Error = combined_e;
    return result;
}

// vector<double> Total_Mean(const vector<double> x[6]) {
//     vector<double> result;
//     for (int j = 0; j < x[0].size(); j++)
//     {
//         double sum_x = 0.0;
//         for (int i = 0; i < 6; i++) 
//         {
//             sum_x +=  x[i][j];
//         }
//         double combined_x = sum_x /6;
//         result.push_back(combined_x);
//     }
//     return result;
// }
// vector<double> Total_Error(const vector<double> e[6]) {
//     vector<double> result;
//     for (int j = 0; j < e[0].size(); j++)
//     {
//         double sum_w = 0.0;
//         for (int i = 0; i < 6; i++)
//         {
//             double w = e[i][j] * e[i][j];
//             sum_w += w;
//         }
//         double combined_e = sqrt(sum_w)/6;
//         result.push_back(combined_e);
//     }
//     return result;
// }

DataWithError GetNonlinearityError(DataWithError TruePE, DataWithError CalibPE)
{
    DataWithError Nonlinearity;
    Nonlinearity.Data = (CalibPE.Data - TruePE.Data)/TruePE.Data;
    Nonlinearity.Error = sqrt(CalibPE.Error*CalibPE.Error/(TruePE.Data*TruePE.Data) + (CalibPE.Data*CalibPE.Data*TruePE.Error*TruePE.Error)/(TruePE.Data*TruePE.Data*TruePE.Data*TruePE.Data));
    return Nonlinearity;
}

void DrawNonlinearityCompare_cut_SPMTonly_revises()
{
    GetP0();
    for (int i = 0; i < 19; i++)
    {
        cout<<iPEP0[i]<<endl;
    }
    iPEP0[0] = ROOT::Math::normal_cdf(1.0/3.0,33.2/100.0,1.0);
    
    TFile *DarkRate_file = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
    TTree *PMTData = (TTree*)DarkRate_file->Get("data");
    double darkRate;
    PMTData->SetBranchAddress("darkRate",&darkRate);
    DataWithError DarkRateData[25600];
    for (int i = 0; i < 25600; i++)
    {
        PMTData->GetEntry(i);
        DarkRateData[i].Data = darkRate;
        DarkRateData[i].Error = 0;
    }
    

    TFile *inputfile_P0 = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/P0Charge.root","read");
    TFile *inputfile_nPE = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/nPECharge.root","read");
    TFile *inputfile_DetSim = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/DetsimCharge.root","read");

    TTree *P0Charge = (TTree*)inputfile_P0->Get("charge");
    TTree *nPECharge = (TTree*)inputfile_nPE->Get("charge");
    TTree *DetSimCharge = (TTree*)inputfile_DetSim->Get("charge");

    double P0MeanCharge[25600];
    double P0ErrorCharge[25600];
    int    P0Energy;

    double nPEMeanCharge[25600];
    double nPEErrorCharge[25600];
    int    nPEEnergy;

    double DetSimMeanCharge[25600];
    double DetSimErrorCharge[25600];
    int    DetSimEnergy;

    P0Charge->SetBranchAddress("SPMTMeanCharge",P0MeanCharge);
    P0Charge->SetBranchAddress("ErrorSPMTMeanCharge",P0ErrorCharge);
    P0Charge->SetBranchAddress("energy",&P0Energy);

    nPECharge->SetBranchAddress("SPMTMeanCharge",nPEMeanCharge);
    nPECharge->SetBranchAddress("ErrorSPMTMeanCharge",nPEErrorCharge);
    nPECharge->SetBranchAddress("energy",&nPEEnergy);

    DetSimCharge->SetBranchAddress("SPMTMeanCharge",DetSimMeanCharge);
    DetSimCharge->SetBranchAddress("ErrorSPMTMeanCharge",DetSimErrorCharge);
    DetSimCharge->SetBranchAddress("energy",&DetSimEnergy);

    // 存放每个能量下的电荷值
    vector<double> P0_bias_all;
    vector<double> P0_revise_all;
    vector<double> nPE_all;
    vector<double> nPE_revise_all;
    vector<double> DetSim_all;

    vector<double> P0_bias_all_error;
    vector<double> P0_revise_all_error;
    vector<double> nPE_all_error;
    vector<double> nPE_revise_all_error;
    vector<double> DetSim_all_error;

    for (int i = 0; i < 21; i++)
    {
        if (i<6)
        {
            P0Charge->GetEntry(i);
            nPECharge->GetEntry(i);
            DetSimCharge->GetEntry(i);
        }
        else
        {
            P0Charge->GetEntry(i);
            nPECharge->GetEntry(i);
            DetSimCharge->GetEntry(i+1);
        }
        
        
        

        DataWithError RoughP0Charge[25600];
        DataWithError TrueP0Charge[25600];
        DataWithError nPE[25600];
        DataWithError TruenPE[25600];
        DataWithError DetSim[25600];

        for (int PMTID = 0; PMTID < 25600; PMTID++)
        {
            RoughP0Charge[PMTID].Data = P0MeanCharge[PMTID];
            RoughP0Charge[PMTID].Error = P0ErrorCharge[PMTID];
            TrueP0Charge[PMTID] = GetTrueCharge(RoughP0Charge[PMTID],DarkRateData[PMTID]);
            // TrueP0Charge[PMTID] = RoughP0Charge[PMTID];

            nPE[PMTID].Data = nPEMeanCharge[PMTID];
            nPE[PMTID].Error = nPEErrorCharge[PMTID];
            TruenPE[PMTID] = GetTrueCharge_ChargeMethod(nPE[PMTID],DarkRateData[PMTID]);
            DetSim[PMTID].Data = DetSimMeanCharge[PMTID];
            DetSim[PMTID].Error = DetSimErrorCharge[PMTID];
        }
        DataWithError DetSimCharge = Total(DetSim);
        DataWithError RoughP0Charge_Nonlinearity = GetNonlinearityError(DetSimCharge,Total(RoughP0Charge));
        DataWithError TrueP0Charge_Nonlinearity = GetNonlinearityError(DetSimCharge,Total(TrueP0Charge));
        DataWithError nPE_Nonlinearity = GetNonlinearityError(DetSimCharge,Total(nPE));
        DataWithError TruenPE_Nonlinearity = GetNonlinearityError(DetSimCharge,Total(TruenPE));

        P0_bias_all.push_back(RoughP0Charge_Nonlinearity.Data);
        P0_bias_all_error.push_back(RoughP0Charge_Nonlinearity.Error);
        P0_revise_all.push_back(TrueP0Charge_Nonlinearity.Data);
        P0_revise_all_error.push_back(TrueP0Charge_Nonlinearity.Error);
        nPE_all.push_back(nPE_Nonlinearity.Data);
        nPE_all_error.push_back(nPE_Nonlinearity.Error);
        nPE_revise_all.push_back(TruenPE_Nonlinearity.Data);
        nPE_revise_all_error.push_back(TruenPE_Nonlinearity.Error);
        DetSim_all.push_back(DetSimCharge.Data);
        DetSim_all_error.push_back(DetSimCharge.Error);

        cout<<i<<endl;
    }
    
    TGraphErrors *NPENonlinearity_Total = new TGraphErrors(DetSim_all.size(),
                                                            &DetSim_all[0],&nPE_all[0],
                                                            &DetSim_all_error[0],&nPE_all_error[0]);
    TGraphErrors *ReviseNPENonlinearity_Total = new TGraphErrors(DetSim_all.size(),
                                                            &DetSim_all[0],&nPE_revise_all[0],
                                                            &DetSim_all_error[0],&nPE_revise_all_error[0]);
    TGraphErrors *RoughP0Nonlinearity_Total = new TGraphErrors(DetSim_all.size(),
                                                            &DetSim_all[0],&P0_bias_all[0],
                                                            &DetSim_all_error[0],&P0_bias_all_error[0]);
    TGraphErrors *ReviseP0Nonlinearity_Total = new TGraphErrors(DetSim_all.size(),
                                                            &DetSim_all[0],&P0_revise_all[0],
                                                            &DetSim_all_error[0],&P0_revise_all_error[0]);
    
    TCanvas *c2 = new TCanvas("c2","c2",1);
    NPENonlinearity_Total->SetLineColor(1);
    NPENonlinearity_Total->SetFillStyle(3002);
    NPENonlinearity_Total->SetMarkerColor(1);
    NPENonlinearity_Total->SetMarkerSize(1.0);
    NPENonlinearity_Total->SetMarkerStyle(20);
    NPENonlinearity_Total->SetLineWidth(3);
    NPENonlinearity_Total->SetTitle("Nonlinearity of different methods");
    NPENonlinearity_Total->GetXaxis()->SetTitle("True SPMT charge form DetSim(p.e.)");
    NPENonlinearity_Total->GetYaxis()->SetTitle("Nonlinearity");

    // NPENonlinearity_Total->Draw("a");
    NPENonlinearity_Total->Draw("a L P");

    TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    legendb->SetTextFont(72);
    legendb->SetTextSize(0.04);
    legendb->AddEntry(NPENonlinearity_Total,"Charge method","lpe");
    
    ReviseNPENonlinearity_Total->SetLineColor(4);
    ReviseNPENonlinearity_Total->SetFillStyle(3002);
    ReviseNPENonlinearity_Total->SetMarkerColor(4);
    ReviseNPENonlinearity_Total->SetMarkerSize(1.0);
    ReviseNPENonlinearity_Total->SetMarkerStyle(20);
    ReviseNPENonlinearity_Total->SetLineWidth(3);

    // ReviseNPENonlinearity_Total->Draw("same L P");

    // legendb->AddEntry(ReviseNPENonlinearity_Total,"Charge method w/o DCR effect","lpe");

    RoughP0Nonlinearity_Total->SetLineColor(2);
    RoughP0Nonlinearity_Total->SetFillStyle(3002);
    RoughP0Nonlinearity_Total->SetMarkerColor(2);
    RoughP0Nonlinearity_Total->SetMarkerSize(1.0);
    RoughP0Nonlinearity_Total->SetMarkerStyle(20);
    RoughP0Nonlinearity_Total->SetLineWidth(3);

    RoughP0Nonlinearity_Total->Draw("same L P");

    legendb->AddEntry(RoughP0Nonlinearity_Total,"Occupancy method before fixing","lpe");

    ReviseP0Nonlinearity_Total->SetLineColor(3);
    ReviseP0Nonlinearity_Total->SetFillStyle(3002);
    ReviseP0Nonlinearity_Total->SetMarkerColor(3);
    ReviseP0Nonlinearity_Total->SetMarkerSize(1.0);
    ReviseP0Nonlinearity_Total->SetMarkerStyle(20);
    ReviseP0Nonlinearity_Total->SetLineWidth(3);

    ReviseP0Nonlinearity_Total->Draw("same L P");

    legendb->AddEntry(ReviseP0Nonlinearity_Total,"Occupancy method after fixing","lpe");

    legendb->Draw();
}