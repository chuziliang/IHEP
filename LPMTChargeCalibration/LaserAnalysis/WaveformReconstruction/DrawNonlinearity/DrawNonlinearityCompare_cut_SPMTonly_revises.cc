// 这个脚本用于把真实的非线性曲线、P（0）和nPE方法得到的非线性曲线画在一起对比
// 需要先运行以下程序：
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/GetTruePE.cc”
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/P0method/npeLPMTtonpeSPMT.cc”
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/nPEmethod/npeLPMTtonpeSPMT.cc”
// 这里P0和nPE的曲线是这两种方法自身的非线性曲线（SPMT自身电荷）
// 经过暗噪声和阈值效应的修正
using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

vector<double> Total_Mean(const vector<double> x[6]) {

    vector<double> result;
    
    for (int j = 0; j < x[0].size(); j++)
    {
        double sum_x = 0.0;
        for (int i = 0; i < 6; i++) 
        {
            sum_x +=  x[i][j];
        }
        double combined_x = sum_x /6;
        result.push_back(combined_x);
    }
    return result;
}

vector<double> Total_Error(const vector<double> e[6]) {

    vector<double> result;

    for (int j = 0; j < e[0].size(); j++)
    {
        double sum_w = 0.0;

        for (int i = 0; i < 6; i++)
        {
            double w = e[i][j] * e[i][j];
            sum_w += w;
        }
        double combined_e = sqrt(sum_w)/6;
        result.push_back(combined_e);
    }
    return result;
}

double GetNonlinearityError(double x, double y, double ex, double ey)
{
    double NonlinearityError = sqrt(ey*ey/(x*x) + (y*y*ex*ex)/(x*x*x*x));
    return NonlinearityError;
}

void DrawNonlinearityCompare_cut_SPMTonly_revises()
{
    TFile *inputfile_P0_fix = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/P0method/revises/npeLPMTtonpeSPMT.root","read");
    TFile *inputfile_P0 = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/P0method/Cut/npeLPMTtonpeSPMT.root","read");
    TFile *inputfile_nPE = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/nPEmethod/Cut/npeLPMTtonpeSPMT.root","read");
    TFile *inputfilr_DetSim = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/npeLPMTtonpeSPMT.root");

    TGraphErrors *DetSim_SPMT[6];
    TF1 *DetSim_fit_function[6];
    double DetSim_p1[6];
    double DetSim_p1error[6];
    for (int i = 0; i < 6; i++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",i);
        DetSim_SPMT[i] = (TGraphErrors*)inputfilr_DetSim->Get(GraphName);
        DetSim_fit_function[i] = DetSim_SPMT[i]->GetFunction("pol1");
        DetSim_p1[i] = DetSim_fit_function[i]->GetParameter(1);
        cout<<DetSim_p1[i]<<endl;
        DetSim_p1error[i] = DetSim_fit_function[i]->GetParError(1);
    }

    vector<double> Vector_P0ElecPE[6];
    vector<double> Vector_P0ElecPE_Error[6];
    vector<double> Vector_NPEElecPE[6];
    vector<double> Vector_NPEElecPE_Error[6];
    vector<double> Vector_DetSimPE[6];
    vector<double> Vector_DetSimPE_Error[6];

    vector<double> Vector_P0ElecPE_Total;
    vector<double> Vector_P0ElecPE_Error_Total;
    vector<double> Vector_NPEElecPE_Total;
    vector<double> Vector_NPEElecPE_Error_Total;
    vector<double> Vector_DetSimPE_Total;
    vector<double> Vector_DetSimPE_Error_Total;

    TGraphErrors *P0_SPMT[6];
    TGraphErrors *NPE_SPMT[6];
    for (int i = 0; i < 6; i++)
    {
        TString GraphName = TString::Format("True LPMT npe to SPMT_%1d npe",i);
        P0_SPMT[i] = (TGraphErrors*)inputfile_P0->Get(GraphName);
        NPE_SPMT[i] = (TGraphErrors*)inputfile_nPE->Get(GraphName);
        for (int j = 0; j < 20; j++)
        {
            double trueCharge;
            double P0Charge;
            double nPECharge;

            double trueChargeError;
            double P0ChargeError;
            double nPEChargeError;
            if (j<6)
            {
                trueCharge = DetSim_SPMT[i]->GetPointX(j);
                trueChargeError = DetSim_SPMT[i]->GetErrorX(j);
            }
            else
            {
                trueCharge = DetSim_SPMT[i]->GetPointX(j+1);
                trueChargeError = DetSim_SPMT[i]->GetErrorX(j+1);
            }
            
            
            P0Charge = P0_SPMT[i]->GetPointX(j);
            nPECharge = NPE_SPMT[i]->GetPointX(j);

            
            P0ChargeError = P0_SPMT[i]->GetErrorX(j);
            nPEChargeError = NPE_SPMT[i]->GetErrorX(j);
            
            Vector_DetSimPE[i].push_back(trueCharge);
            Vector_DetSimPE_Error[i].push_back(trueChargeError);
            // if (j == 6)
            // {
            //     Vector_DetSimPE[i].pop_back();
            //     Vector_DetSimPE_Error[i].pop_back();
            // }

            GetNonlinearityError(trueCharge,P0Charge,trueChargeError,P0ChargeError);
            
            Vector_P0ElecPE[i].push_back((P0Charge-trueCharge)/trueCharge);
            Vector_P0ElecPE_Error[i].push_back(GetNonlinearityError(trueCharge,P0Charge,trueChargeError,P0ChargeError));
            Vector_NPEElecPE[i].push_back((nPECharge-trueCharge)/trueCharge);
            Vector_NPEElecPE_Error[i].push_back(GetNonlinearityError(trueCharge,nPECharge,trueChargeError,nPEChargeError));

        }
    }

    Vector_DetSimPE_Total = Total_Mean(Vector_DetSimPE);
    Vector_DetSimPE_Error_Total = Total_Error(Vector_DetSimPE_Error);
    Vector_P0ElecPE_Total = Total_Mean(Vector_P0ElecPE);
    Vector_P0ElecPE_Error_Total = Total_Error(Vector_P0ElecPE_Error);
    Vector_NPEElecPE_Total = Total_Mean(Vector_NPEElecPE);
    Vector_NPEElecPE_Error_Total = Total_Error(Vector_NPEElecPE_Error);
    

    // TGraphErrors *P0Nonlinearity = new TGraphErrors(VectorP0CalibPE_total.size(),&VectorP0CalibPE_total[0],
    //                                                 &VectorP0Nonlinearity_total[0],&VectorP0CalibPEerror_total[0],&VectorP0Nonlinearityerror_total[0]);

    // TGraphErrors *NPENonlinearity = new TGraphErrors(VectorNPECalibPE_total.size(),&VectorNPECalibPE_total[0],
    //                                                 &VectorNPENonlinearity_total[0],&VectorNPECalibPEerror_total[0],&VectorNPENonlinearityerror_total[0]);
    // TGraphErrors *DetSimNonlinearity = new TGraphErrors(VectorDetSimPE_total.size(),&VectorDetSimPE_total[0],
    //                                                 &VectorDetSimNonlinearity_total[0],&VectorDetSimPEerror_total[0],&VectorDetSimNonlinearityerror_total[0]);

    TGraphErrors *NPENonlinearity_Total = new TGraphErrors(Vector_NPEElecPE_Total.size(),
                                                            &Vector_DetSimPE_Total[0],&Vector_NPEElecPE_Total[0],
                                                            &Vector_DetSimPE_Error_Total[0],&Vector_NPEElecPE_Error_Total[0]);
    TGraphErrors *P0Nonlinearity_Total = new TGraphErrors(Vector_P0ElecPE_Total.size(),
                                                            &Vector_DetSimPE_Total[0],&Vector_P0ElecPE_Total[0],
                                                            &Vector_DetSimPE_Error_Total[0],&Vector_P0ElecPE_Error_Total[0]);
    // for (int i = 0; i < Vector_DetSimPE_Total.size(); i++)
    // {
    //     cout<<Vector_DetSimPE_Total[i]<<endl;
    // }
    
    TGraphErrors *NPENonlinearity[6];
    TGraphErrors *P0Nonlinearity[6];
    for (int i = 0; i < 6; i++)
    {
        NPENonlinearity[i] = new TGraphErrors(Vector_NPEElecPE[i].size(),&Vector_DetSimPE[i][0],&Vector_NPEElecPE[i][0],&Vector_DetSimPE_Error[i][0],&Vector_NPEElecPE_Error[i][0]);
        P0Nonlinearity[i] = new TGraphErrors(Vector_P0ElecPE[i].size(),&Vector_DetSimPE[i][0],&Vector_P0ElecPE[i][0],&Vector_DetSimPE_Error[i][0],&Vector_P0ElecPE_Error[i][0]);
    }
    
    TCanvas *c2 = new TCanvas("c2","c2",1);
    P0Nonlinearity_Total->SetLineColor(1);
    P0Nonlinearity_Total->SetFillStyle(3002);
    P0Nonlinearity_Total->SetMarkerColor(1);
    P0Nonlinearity_Total->SetMarkerSize(1.0);
    P0Nonlinearity_Total->SetMarkerStyle(22);
    P0Nonlinearity_Total->SetTitle("Nonlinearity P0 method before fixing the biases");
    P0Nonlinearity_Total->GetXaxis()->SetTitle("True SPMT charge form DetSim(PE)");
    P0Nonlinearity_Total->GetYaxis()->SetTitle("(ElecSimCharge  - TrueCharge)/TrueCharge");

    // NPENonlinearity_Total->Draw("a");
    P0Nonlinearity_Total->Draw("a L P");

    TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    legendb->SetTextFont(72);
    legendb->SetTextSize(0.04);
    legendb->AddEntry(P0Nonlinearity_Total,"Average of 6 SPMTs","lpe");
    for (int i = 0; i < 6; i++)
    {
        P0Nonlinearity[i]->SetLineColor(i+1);
        P0Nonlinearity[i]->SetFillStyle(3002);
        P0Nonlinearity[i]->SetMarkerColor(i+1);
        P0Nonlinearity[i]->SetMarkerSize(0.8);
        P0Nonlinearity[i]->SetMarkerStyle(20);
        // NPENonlinearity[i]->Draw("same");
        if (i==0)
        {
            P0Nonlinearity[i]->SetLineColor(20);
            P0Nonlinearity[i]->SetMarkerColor(20);
        }

        P0Nonlinearity[i]->Draw("same L P");

        legendb->AddEntry(P0Nonlinearity[i],TString::Format("SPMT_%1d",i),"lpe");
    }
    legendb->Draw();


    TCanvas *c1 = new TCanvas("c1","c1",1);
    NPENonlinearity_Total->SetLineColor(1);
    NPENonlinearity_Total->SetFillStyle(3002);
    NPENonlinearity_Total->SetMarkerColor(1);
    NPENonlinearity_Total->SetMarkerSize(1.0);
    NPENonlinearity_Total->SetMarkerStyle(22);
    NPENonlinearity_Total->SetTitle("Nonlinearity nPE method");
    NPENonlinearity_Total->GetXaxis()->SetTitle("True SPMT charge form DetSim(PE)");
    NPENonlinearity_Total->GetYaxis()->SetTitle("(ElecSimCharge  - TrueCharge)/TrueCharge");

    // NPENonlinearity_Total->Draw("a");
    NPENonlinearity_Total->Draw("a L P");

    TLegend *legenda =new TLegend(0.6,0.65,0.88,0.85);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.04);
    legenda->AddEntry(NPENonlinearity_Total,"Average of 6 SPMTs","lpe");
    for (int i = 0; i < 6; i++)
    {
        NPENonlinearity[i]->SetLineColor(i+1);
        NPENonlinearity[i]->SetFillStyle(3002);
        NPENonlinearity[i]->SetMarkerColor(i+1);
        NPENonlinearity[i]->SetMarkerSize(0.8);
        NPENonlinearity[i]->SetMarkerStyle(20);
        // NPENonlinearity[i]->Draw("same");
        if (i==0)
        {
            NPENonlinearity[i]->SetLineColor(20);
            NPENonlinearity[i]->SetMarkerColor(20);
        }
        
        NPENonlinearity[i]->Draw("same L P");

        legenda->AddEntry(NPENonlinearity[i],TString::Format("SPMT_%1d",i),"lpe");
    }
    legenda->Draw();
    

    // TCanvas *c3 = new TCanvas("c3","c3",1);
    // P0Nonlinearity_Total->SetLineColor(3);
    // P0Nonlinearity_Total->SetMarkerColor(3);
    // NPENonlinearity_Total->SetLineColor(4);
    // NPENonlinearity_Total->SetMarkerColor(4);

    // NPENonlinearity_Total->SetTitle("Nonlinearity of P0 method");

    // NPENonlinearity_Total->Draw("a L P");
    // P0Nonlinearity_Total->Draw("same L P");

    // TLegend *legendc =new TLegend(0.6,0.65,0.88,0.85);
    // legendc->SetTextFont(72);
    // legendc->SetTextSize(0.04);
    // legendc->AddEntry(P0Nonlinearity_Total,"Before fixing biases","lpe");
    // legendc->AddEntry(NPENonlinearity_Total,"After fixing biases","lpe");
    // legendc->Draw();


    
}