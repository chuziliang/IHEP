// 这个脚本用于把真实的非线性曲线、P（0）和nPE方法得到的非线性曲线画在一起对比
// 需要先运行以下程序：
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/GetTruePE.cc”
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/P0method/npeLPMTtonpeSPMT.cc”
// “/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/nPEmethod/npeLPMTtonpeSPMT.cc”
// 这里P0和nPE的曲线是这两种方法自身的非线性曲线
using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

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

void DrawNonlinearityCompare_cut_SPMT()
{
    TFile *inputfile_true = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/Nonlinearity/Nonlinerity.root","read");
    TFile *inputfile_P0 = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/P0method/Cut/npeLPMTtonpeSPMT.root","read");
    TFile *inputfile_nPE = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/nPEmethod/Cut/npeLPMTtonpeSPMT.root","read");
    TFile *inputfilr_DetSim = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/npeLPMTtonpeSPMT.root");

    TGraphErrors *TrueNonlinearity = (TGraphErrors*)inputfile_true->Get("Nonlinearity");

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

    // TGraphErrors *P0Nonlinearity;
    vector<double> VectorP0CalibPE_total;
    vector<double> VectorP0CalibPEerror_total;
    vector<double> VectorP0Nonlinearity_total;
    vector<double> VectorP0Nonlinearityerror_total;

    vector<double> VectorP0CalibPE[6];
    vector<double> VectorP0CalibPEerror[6];
    vector<double> VectorP0ReconstructPE[6];
    vector<double> VectorP0ReconstructPEerror[6];


    vector<double> VectorNPECalibPE_total;
    vector<double> VectorNPECalibPEerror_total;
    vector<double> VectorNPENonlinearity_total;
    vector<double> VectorNPENonlinearityerror_total;

    vector<double> VectorNPECalibPE[6];
    vector<double> VectorNPECalibPEerror[6];
    vector<double> VectorNPEReconstructPE[6];
    vector<double> VectorNPEReconstructPEerror[6];


    vector<double> VectorDetSimPE_total;
    vector<double> VectorDetSimPEerror_total;
    vector<double> VectorDetSimNonlinearity_total;
    vector<double> VectorDetSimNonlinearityerror_total;

    vector<double> VectorDetSimPE[6];
    vector<double> VectorDetSimPEerror[6];
    vector<double> VectorDetSimReconstructPE[6];
    vector<double> VectorDetSimReconstructPEerror[6];


    TGraphErrors *P0_SPMT[6];
    TGraphErrors *NPE_SPMT[6];
    for (int i = 0; i < 6; i++)
    {
        TString GraphName = TString::Format("True LPMT npe to SPMT_%1d npe",i);
        P0_SPMT[i] = (TGraphErrors*)inputfile_P0->Get(GraphName);
        NPE_SPMT[i] = (TGraphErrors*)inputfile_nPE->Get(GraphName);
        for (int j = 0; j < P0_SPMT[i]->GetN(); j++)
        {
            double x;
            double y;
            double ex;
            double ey;
            double etot;
            P0_SPMT[i]->GetPoint(j,x,y);
            ex = P0_SPMT[i]->GetErrorX(j);
            ey = P0_SPMT[i]->GetErrorY(j);
            etot = sqrt( DetSim_p1[i]*DetSim_p1[i]*ex*ex + x*x*DetSim_p1error[i]*DetSim_p1error[i] );
            VectorP0CalibPE[i].push_back(x*DetSim_p1[i]);
            VectorP0ReconstructPE[i].push_back(y);
            VectorP0CalibPEerror[i].push_back(etot);
            VectorP0ReconstructPEerror[i].push_back(ey);
        }

        for (int j = 0; j < NPE_SPMT[i]->GetN(); j++)
        {
            double x;
            double y;
            double ex;
            double ey;
            double etot;
            NPE_SPMT[i]->GetPoint(j,x,y);
            ex = NPE_SPMT[i]->GetErrorX(j);
            ey = NPE_SPMT[i]->GetErrorY(j);
            etot = sqrt( DetSim_p1[i]*DetSim_p1[i]*ex*ex + x*x*DetSim_p1error[i]*DetSim_p1error[i] );
            VectorNPECalibPE[i].push_back(x*DetSim_p1[i]);
            VectorNPEReconstructPE[i].push_back(y);
            VectorNPECalibPEerror[i].push_back(etot);
            VectorNPEReconstructPEerror[i].push_back(ey);
        }

        for (int j = 0; j < DetSim_SPMT[i]->GetN(); j++)
        {
            double x;
            double y;
            double ex;
            double ey;
            double etot;
            DetSim_SPMT[i]->GetPoint(j,x,y);
            ex = DetSim_SPMT[i]->GetErrorX(j);
            ey = DetSim_SPMT[i]->GetErrorY(j);
            etot = sqrt( DetSim_p1[i]*DetSim_p1[i]*ex*ex + x*x*DetSim_p1error[i]*DetSim_p1error[i] );
            VectorDetSimPE[i].push_back(x*DetSim_p1[i]);
            VectorDetSimReconstructPE[i].push_back(y);
            VectorDetSimPEerror[i].push_back(etot);
            VectorDetSimReconstructPEerror[i].push_back(ey);
        }
        
    }
    for (int i = 0; i < P0_SPMT[0]->GetN(); i++)
    {
        double totx;
        double toty;
        double totex;
        double totey;
        double NonlinearityError;
        vector<double> CombineP0CalibPE(6,0.0);
        vector<double> CombineP0CalibPEerror(6,0.0);
        vector<double> CombineP0ReconstructPE(6,0.0);
        vector<double> CombineP0ReconstructPEerror(6,0.0);
        for (int j = 0; j < 6; j++)
        {
            CombineP0CalibPE[j] = VectorP0CalibPE[j][i];
            CombineP0CalibPEerror[j] = VectorP0CalibPEerror[j][i];
            CombineP0ReconstructPE[j] = VectorP0ReconstructPE[j][i];
            CombineP0ReconstructPEerror[j] = VectorP0ReconstructPEerror[j][i];
        }
        
        totx = combined_measurement(CombineP0CalibPE,CombineP0CalibPEerror);
        totex = combined_error(CombineP0CalibPEerror);
        toty = combined_measurement(CombineP0ReconstructPE,CombineP0ReconstructPEerror);
        totey = combined_error(CombineP0ReconstructPEerror);
        NonlinearityError = sqrt(totey*totey/(totx*totx) + (toty*toty*totex*totex)/(totx*totx*totx*totx));

        VectorP0CalibPE_total.push_back(toty);
        VectorP0CalibPEerror_total.push_back(totey);
        VectorP0Nonlinearity_total.push_back((totx - toty)/toty);
        VectorP0Nonlinearityerror_total.push_back(NonlinearityError);
    }
    
    for (int i = 0; i < NPE_SPMT[0]->GetN(); i++)
    {
        double totx;
        double toty;
        double totex;
        double totey;
        double NonlinearityError;
        vector<double> CombineNPECalibPE(6,0.0);
        vector<double> CombineNPECalibPEerror(6,0.0);
        vector<double> CombineNPEReconstructPE(6,0.0);
        vector<double> CombineNPEReconstructPEerror(6,0.0);
        for (int j = 0; j < 6; j++)
        {
            CombineNPECalibPE[j] = VectorNPECalibPE[j][i];
            CombineNPECalibPEerror[j] = VectorNPECalibPEerror[j][i];
            CombineNPEReconstructPE[j] = VectorNPEReconstructPE[j][i];
            CombineNPEReconstructPEerror[j] = VectorNPEReconstructPEerror[j][i];
        }
        
        totx = combined_measurement(CombineNPECalibPE,CombineNPECalibPEerror);
        totex = combined_error(CombineNPECalibPEerror);
        toty = combined_measurement(CombineNPEReconstructPE,CombineNPEReconstructPEerror);
        totey = combined_error(CombineNPEReconstructPEerror);
        NonlinearityError = sqrt(totey*totey/(totx*totx) + (toty*toty*totex*totex)/(totx*totx*totx*totx));

        VectorNPECalibPE_total.push_back(toty);
        VectorNPECalibPEerror_total.push_back(totey);
        VectorNPENonlinearity_total.push_back((totx - toty)/toty);
        VectorNPENonlinearityerror_total.push_back(NonlinearityError);
    }

    for (int i = 0; i < DetSim_SPMT[0]->GetN(); i++)
    {
        double totx;
        double toty;
        double totex;
        double totey;
        double NonlinearityError;
        vector<double> CombineDetSimPE(6,0.0);
        vector<double> CombineDetSimPEerror(6,0.0);
        vector<double> CombineDetSimReconstructPE(6,0.0);
        vector<double> CombineDetSimReconstructPEerror(6,0.0);
        for (int j = 0; j < 6; j++)
        {
            CombineDetSimPE[j] = VectorDetSimPE[j][i];
            CombineDetSimPEerror[j] = VectorDetSimPEerror[j][i];
            CombineDetSimReconstructPE[j] = VectorDetSimReconstructPE[j][i];
            CombineDetSimReconstructPEerror[j] = VectorDetSimReconstructPEerror[j][i];
        }
        
        totx = combined_measurement(CombineDetSimPE,CombineDetSimPEerror);
        totex = combined_error(CombineDetSimPEerror);
        toty = combined_measurement(CombineDetSimReconstructPE,CombineDetSimReconstructPEerror);
        totey = combined_error(CombineDetSimReconstructPEerror);
        NonlinearityError = sqrt(totey*totey/(totx*totx) + (toty*toty*totex*totex)/(totx*totx*totx*totx));

        VectorDetSimPE_total.push_back(toty);
        VectorDetSimPEerror_total.push_back(totey);
        VectorDetSimNonlinearity_total.push_back((totx - toty)/toty);
        VectorDetSimNonlinearityerror_total.push_back(NonlinearityError);
    }

    TGraphErrors *P0Nonlinearity = new TGraphErrors(VectorP0CalibPE_total.size(),&VectorP0CalibPE_total[0],
                                                    &VectorP0Nonlinearity_total[0],&VectorP0CalibPEerror_total[0],&VectorP0Nonlinearityerror_total[0]);

    TGraphErrors *NPENonlinearity = new TGraphErrors(VectorNPECalibPE_total.size(),&VectorNPECalibPE_total[0],
                                                    &VectorNPENonlinearity_total[0],&VectorNPECalibPEerror_total[0],&VectorNPENonlinearityerror_total[0]);
    TGraphErrors *DetSimNonlinearity = new TGraphErrors(VectorDetSimPE_total.size(),&VectorDetSimPE_total[0],
                                                    &VectorDetSimNonlinearity_total[0],&VectorDetSimPEerror_total[0],&VectorDetSimNonlinearityerror_total[0]);
    TCanvas *c1 = new TCanvas("c1","c1",1);
    
    P0Nonlinearity->SetFillColor(5);
    P0Nonlinearity->SetFillStyle(3002);
    P0Nonlinearity->SetMarkerColor(5);
    P0Nonlinearity->SetMarkerSize(0.8);
    P0Nonlinearity->SetMarkerStyle(20);
    P0Nonlinearity->SetTitle("Nonlinearity of P0 and nPE method");
    P0Nonlinearity->GetXaxis()->SetTitle("True LPMT nPE form ElecSim(DetSim)");
    P0Nonlinearity->GetYaxis()->SetTitle("(CalibNPE_bySPMT - TrueNPE)/TrueNPE");

    P0Nonlinearity->Draw("a3");
    P0Nonlinearity->Draw("same L X P");

    NPENonlinearity->SetFillColor(6);
    NPENonlinearity->SetFillStyle(3002);
    NPENonlinearity->SetMarkerColor(6);
    NPENonlinearity->SetMarkerSize(0.8);
    NPENonlinearity->SetMarkerStyle(20);

    NPENonlinearity->Draw("same 3");
    NPENonlinearity->Draw("same L X P");

    DetSimNonlinearity->SetFillColor(7);
    DetSimNonlinearity->SetFillStyle(3002);
    DetSimNonlinearity->SetMarkerColor(7);
    DetSimNonlinearity->SetMarkerSize(0.8);
    DetSimNonlinearity->SetMarkerStyle(20);

    DetSimNonlinearity->Draw("same 3");
    DetSimNonlinearity->Draw("same L X P");

    TLegend *legenda =new TLegend(0.6,0.65,0.88,0.85);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.04);
    legenda->AddEntry(P0Nonlinearity,"P0 method","lpe");
    legenda->AddEntry(NPENonlinearity,"NPE method","lpe");
    legenda->AddEntry(DetSimNonlinearity,"DetSim NPE method","lpe");
    legenda->Draw();
    
}