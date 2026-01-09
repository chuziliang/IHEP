struct DataWithError
{
    double Data;
    double Error;
};

Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
    double xx =x[0]; // 有非线性的电荷值
    double miu = 500 - sqrt(500*500 - 1000*xx); // 真实的电荷值
    // Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
    Double_t f = xx/miu - 1;

    // Double_t f = -(0.1/100.)*xx;
    return f;
}

DataWithError GetP0Mu(unsigned long long TotalEvents, unsigned long long ZeroEvents)
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

void DrawNonlinearity_ForMergedResult(int ChosenType)
{
    TString ChosenTypeString;
    if (ChosenType==0) ChosenTypeString = "All";
    else if (ChosenType==1) ChosenTypeString = "Hamamatsu";
    else if (ChosenType==2) ChosenTypeString = "NNVT";
    else if (ChosenType==3) ChosenTypeString = "HighQENNVT";

    vector<double> ToGraph_TotalLPMTMu;
    vector<double> ToGraph_TotalLPMTMuErr;

    vector<double> ToGraph_TotalLPMTMu_QE;
    vector<double> ToGraph_TotalLPMTMu_QEErr;

    vector<double> ToGraph_TotalTrueLPMTMu;
    vector<double> ToGraph_TotalTrueLPMTMuErr;

    vector<double> ToGraph_TotalSPMTMu_QE;
    vector<double> ToGraph_TotalSPMTMu_QEErr;

    vector<double> ToGraph_TotalSPMTMuDetSim_P0;
    vector<double> ToGraph_TotalSPMTMuDetSim_P0Err;

    vector<double> ToGraph_TotalSPMTMuCalib_P0;
    vector<double> ToGraph_TotalSPMTMuCalib_P0Err;

    TStatistic *Sta_LPMTMu[21];
    TStatistic *Sta_TrueLPMTMu[21];
    TStatistic *Sta_TrueLPMTMu_QE[21];
    TStatistic *Sta_SPMTMu[21];
    TStatistic *Sta_SPMTDCR[21];
    TStatistic *Sta_SPMTQE[21];
    TStatistic *Sta_LPMTQE[21];

    vector<unsigned long long> SPMT0_Calib(21,0);
    vector<unsigned long long> SPMT0_DetSim(21,0);

    TString InputName = "/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12_OldOpticalModel/result/npeLPMTtonpeSPMT_B12_DetSim_AllPMT_"+ChosenTypeString+"_Cut"+TString::Format("%1d",4)+".root";
    TFile *inputfile = TFile::Open(InputName,"read");
    
    TTree *SPMT0_Tree = (TTree*)inputfile->Get("SPMT0");
    int SPMT0_Calib_i;
    int SPMT0_DetSim_i;
    SPMT0_Tree->SetBranchAddress("Calib",&SPMT0_Calib_i);
    SPMT0_Tree->SetBranchAddress("DetSim",&SPMT0_DetSim_i);
    for (int QNum = 0; QNum < 21; QNum++)
    {
        Sta_LPMTMu[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/LPMTMu Q%1d",QNum,QNum));
        Sta_TrueLPMTMu[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/TrueLPMTMu Q%1d",QNum,QNum));
        Sta_TrueLPMTMu_QE[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/TrueLPMTMu_QE Q%1d",QNum,QNum));
        Sta_SPMTMu[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/SPMTMu Q%1d",QNum,QNum));
        Sta_SPMTDCR[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/SPMTDCR Q%1d",QNum,QNum));
        Sta_SPMTQE[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/SPMTQE Q%1d",QNum,QNum));
        Sta_LPMTQE[QNum] = (TStatistic*)inputfile->Get(TString::Format("%1d/LPMTQE Q%1d",QNum,QNum));
        SPMT0_Tree->GetEntry(QNum);
        SPMT0_Calib[QNum]=SPMT0_Calib_i;
        SPMT0_DetSim[QNum]=SPMT0_DetSim_i;
    }

    SPMT0_Calib[1]  += SPMT0_Calib[0];
    SPMT0_DetSim[1] += SPMT0_DetSim[0];

    TList *List_LPMTMu = new TList();
    TList *List_TrueLPMTMu = new TList();
    TList *List_TrueLPMTMu_QE = new TList();
    TList *List_SPMTMu = new TList();
    TList *List_SPMTDCR = new TList();
    TList *List_SPMTQE = new TList();
    TList *List_LPMTQE = new TList();

    List_LPMTMu->Add(Sta_LPMTMu[0]);
    List_TrueLPMTMu->Add(Sta_TrueLPMTMu[0]);
    List_TrueLPMTMu_QE->Add(Sta_TrueLPMTMu_QE[0]);
    List_SPMTMu->Add(Sta_SPMTMu[0]);
    List_SPMTDCR->Add(Sta_SPMTDCR[0]);
    List_SPMTQE->Add(Sta_SPMTQE[0]);
    List_LPMTQE->Add(Sta_LPMTQE[0]);

    Sta_LPMTMu[1]->Merge(List_LPMTMu);
    Sta_TrueLPMTMu_QE[1]->Merge(List_TrueLPMTMu);
    Sta_TrueLPMTMu[1]->Merge(List_TrueLPMTMu_QE);
    Sta_SPMTMu[1]->Merge(List_SPMTMu);
    Sta_SPMTDCR[1]->Merge(List_SPMTDCR);
    Sta_SPMTQE[1]->Merge(List_SPMTQE);
    Sta_LPMTQE[1]->Merge(List_LPMTQE);

    List_LPMTMu->Clear();
    List_TrueLPMTMu->Clear();
    List_TrueLPMTMu_QE->Clear();
    List_SPMTMu->Clear();
    List_SPMTDCR->Clear();
    List_SPMTQE->Clear();
    List_LPMTQE->Clear();

    
    for (int QNum = 1; QNum < 21; QNum++)
    {
        // for (int EnergyNum = 0; EnergyNum < 10; EnergyNum++)
        // {
        double LPMTMu = Sta_LPMTMu[QNum]->GetMean();
        double LPMTMu_Error = Sta_LPMTMu[QNum]->GetMeanErr();
        double TrueLPMTMu = Sta_TrueLPMTMu[QNum]->GetMean();
        double TrueLPMTMu_Error = Sta_TrueLPMTMu[QNum]->GetMeanErr();

        double TrueLPMTMu_QE = Sta_TrueLPMTMu_QE[QNum]->GetMean();
        double TrueLPMTMu_QE_Error = Sta_TrueLPMTMu_QE[QNum]->GetMeanErr();

        double AverageSPMTQE = Sta_SPMTQE[QNum]->GetMean();
        double AverageLPMTQE = Sta_LPMTQE[QNum]->GetMean();
        DataWithError AverageSPMTDCR;
        AverageSPMTDCR.Data = Sta_SPMTDCR[QNum]->GetMean();
        AverageSPMTDCR.Error = 0;
        DataWithError P0Mu_DetSim = GetP0Mu(Sta_SPMTQE[QNum]->GetN(),SPMT0_DetSim[QNum]);
        DataWithError P0Mu_Calib = GetP0Mu(Sta_SPMTQE[QNum]->GetN(),SPMT0_Calib[QNum]);
        DataWithError TrueMu_calib = GetTrueCharge(P0Mu_Calib,AverageSPMTDCR);
        cout<<"N="<<Sta_SPMTQE[QNum]->GetN()<<"; N0_DetSim="<<SPMT0_DetSim[QNum]<<"; N0_Calib="<<SPMT0_Calib[QNum]<<endl;

        DataWithError TrueMu_DetSim;
        TrueMu_DetSim.Data = Sta_SPMTMu[QNum]->GetMean();
        TrueMu_DetSim.Error = Sta_SPMTMu[QNum]->GetMeanErr();

        ToGraph_TotalLPMTMu.push_back(LPMTMu);
        ToGraph_TotalLPMTMuErr.push_back(LPMTMu_Error);
        ToGraph_TotalTrueLPMTMu.push_back(TrueLPMTMu);
        ToGraph_TotalTrueLPMTMuErr.push_back(TrueLPMTMu_Error);
        ToGraph_TotalLPMTMu_QE.push_back(TrueLPMTMu_QE);
        ToGraph_TotalLPMTMu_QEErr.push_back(TrueLPMTMu_QE_Error);
        ToGraph_TotalSPMTMu_QE.push_back(TrueMu_DetSim.Data);
        ToGraph_TotalSPMTMu_QEErr.push_back(TrueMu_DetSim.Error);
        ToGraph_TotalSPMTMuDetSim_P0.push_back(P0Mu_DetSim.Data/(AverageSPMTQE/AverageLPMTQE));
        ToGraph_TotalSPMTMuDetSim_P0Err.push_back(P0Mu_DetSim.Error/(AverageSPMTQE/AverageLPMTQE));
        ToGraph_TotalSPMTMuCalib_P0.push_back(TrueMu_calib.Data/(AverageSPMTQE/AverageLPMTQE));
        ToGraph_TotalSPMTMuCalib_P0Err.push_back(TrueMu_calib.Error/(AverageSPMTQE/AverageLPMTQE));

        cout<<"Q"<<QNum<<" finished"<<endl;
        // }
    }
    
    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TString GraphName = "P0 LPMT #mu to SPMT #mu_QE_PDE";
    npeLPMTtonpeSPMT_tot = new TGraphErrors(ToGraph_TotalLPMTMu.size(),&ToGraph_TotalSPMTMuCalib_P0[0],&ToGraph_TotalLPMTMu[0],&ToGraph_TotalSPMTMuCalib_P0Err[0],&ToGraph_TotalLPMTMuErr[0]);
    npeLPMTtonpeSPMT_tot->SetName(GraphName);
    npeLPMTtonpeSPMT_tot->SetTitle("");
    npeLPMTtonpeSPMT_tot->SetMarkerColor(4);
    npeLPMTtonpeSPMT_tot->SetLineColor(4);
    npeLPMTtonpeSPMT_tot->SetMarkerSize(1.0);
    npeLPMTtonpeSPMT_tot->SetMarkerStyle(20);
    npeLPMTtonpeSPMT_tot->SetLineWidth(3);
    npeLPMTtonpeSPMT_tot->GetXaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetYaxis()->CenterTitle();
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleFont(22);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetLabelSize(0.06);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitleOffset(1.30);
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitleOffset(0.86);

    TGraphErrors *TruenpeLPMTtonpeSPMT_tot;
    TString GraphName_True = "P0 True LPMT #mu to SPMT #mu_QE_PDE";
    TruenpeLPMTtonpeSPMT_tot = new TGraphErrors(ToGraph_TotalTrueLPMTMu.size(),&ToGraph_TotalSPMTMuCalib_P0[0],&ToGraph_TotalTrueLPMTMu[0],&ToGraph_TotalSPMTMuCalib_P0Err[0],&ToGraph_TotalTrueLPMTMuErr[0]);
    TruenpeLPMTtonpeSPMT_tot->SetMarkerColor(6);
    TruenpeLPMTtonpeSPMT_tot->SetName(GraphName_True);
    TruenpeLPMTtonpeSPMT_tot->SetLineColor(6);

    for (int QNum = npeLPMTtonpeSPMT_tot->GetN(); QNum>0; QNum--)
    {
        if (TMath::IsNaN(npeLPMTtonpeSPMT_tot->GetPointY(QNum)) || TMath::IsNaN(npeLPMTtonpeSPMT_tot->GetPointX(QNum)))
        {
            npeLPMTtonpeSPMT_tot->RemovePoint(QNum);
        } 
    }

    for (int QNum = TruenpeLPMTtonpeSPMT_tot->GetN(); QNum>0; QNum--)
    {
        if (TMath::IsNaN(TruenpeLPMTtonpeSPMT_tot->GetPointY(QNum)) || TMath::IsNaN(TruenpeLPMTtonpeSPMT_tot->GetPointX(QNum)))
        {
            TruenpeLPMTtonpeSPMT_tot->RemovePoint(QNum);
        } 
    }

    TF1 *Below1PE = new TF1("Below1PE","[0]*x",0,0.02);
    Below1PE->SetParNames("k");
    Below1PE->SetLineColor(kRed);
    Below1PE->SetParameter(0,50);

    npeLPMTtonpeSPMT_tot->Fit(Below1PE,"R F");
    Below1PE->SetRange(0,0.55);
    TLegend *legendb =new TLegend(0.6031519,0.4327731,0.9942693,0.6365546);
    legendb->SetTextFont(22);
    legendb->AddEntry(npeLPMTtonpeSPMT_tot,"ElecSim LPMT #mu","lpe");
    legendb->AddEntry(TruenpeLPMTtonpeSPMT_tot,"True LPMT #mu","lpe"); 
    legendb->SetLineColor(0);


    TCanvas *c1 = new TCanvas("c1","c1",1);
    c1->SetBorderMode(0);
    c1->SetBorderSize(2);
    c1->SetLeftMargin(0.1117479);
    c1->SetRightMargin(0.08739255);
    c1->SetTopMargin(0.02941176);
    c1->SetBottomMargin(0.1701681);
    c1->SetFrameBorderMode(0);
    c1->SetFrameBorderMode(0);
    c1->cd();
    gStyle->SetOptFit(1111);
    npeLPMTtonpeSPMT_tot->Draw("A P");
    Below1PE->Draw("same");
    TruenpeLPMTtonpeSPMT_tot->Draw("P");
    legendb->Draw("same");
    c1->Update();
    TPaveStats *s = (TPaveStats*)c1->GetPrimitive("stats");
    s->SetX1NDC(0.5974212);
    s->SetX2NDC(0.9584527);
    s->SetY1NDC(0.2605042);
    s->SetY2NDC(0.4222689);

    vector<double> Nonlinearity;
    vector<double> NonlinearityError;
    vector<double> TrueNonlinearity;
    vector<double> TrueNonlinearityError;
    vector<double> Vector_LPMTMu;
    vector<double> LPMTMuError;

    for (int i = 0; i < npeLPMTtonpeSPMT_tot->GetN(); i++)
    {
        if (npeLPMTtonpeSPMT_tot->GetPointY(i) > 0)
        {
            double LPMTMui = npeLPMTtonpeSPMT_tot->GetPointY(i);
            double LPMTMuErrori = npeLPMTtonpeSPMT_tot->GetErrorY(i);
            double CalibLPMTMui = npeLPMTtonpeSPMT_tot->GetPointX(i)*Below1PE->GetParameter(0);
            double CalibLPMTMuErrori = CalibLPMTMui*sqrt((npeLPMTtonpeSPMT_tot->GetErrorX(i)*npeLPMTtonpeSPMT_tot->GetErrorX(i))/(npeLPMTtonpeSPMT_tot->GetPointX(i)*npeLPMTtonpeSPMT_tot->GetPointX(i)) + (Below1PE->GetParError(0)*Below1PE->GetParError(0))/(Below1PE->GetParameter(0)*Below1PE->GetParameter(0)));
            double TruePMTMui = TruenpeLPMTtonpeSPMT_tot->GetPointY(i);
            double TruePMTMuErrori = TruenpeLPMTtonpeSPMT_tot->GetErrorY(i);
            double Nonlinearityi = (LPMTMui-CalibLPMTMui)/(CalibLPMTMui);
            double NonlinearityErrori = (LPMTMui/CalibLPMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (CalibLPMTMuErrori*CalibLPMTMuErrori)/(CalibLPMTMui*CalibLPMTMui));
            double TrueNonlinearityi = (LPMTMui-TruePMTMui)/(TruePMTMui);
            double TrueNonlinearityErrori = (LPMTMui/TruePMTMui)*sqrt((LPMTMuErrori*LPMTMuErrori)/(LPMTMui*LPMTMui) + (TruePMTMuErrori*TruePMTMuErrori)/(TruePMTMui*TruePMTMui));

            Vector_LPMTMu.push_back(LPMTMui);
            LPMTMuError.push_back(LPMTMuErrori);
            Nonlinearity.push_back(Nonlinearityi);
            NonlinearityError.push_back(NonlinearityErrori);
            TrueNonlinearity.push_back(TrueNonlinearityi);
            TrueNonlinearityError.push_back(TrueNonlinearityErrori);
        }
    }

    TGraphErrors *NonlinearityLPMTtoMu = new TGraphErrors(Vector_LPMTMu.size(),&Vector_LPMTMu[0],&Nonlinearity[0],&LPMTMuError[0],&NonlinearityError[0]);
    NonlinearityLPMTtoMu->SetTitle("");
    NonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT}(p.e.)");
    NonlinearityLPMTtoMu->GetYaxis()->SetTitle("Nonlinearity");
    NonlinearityLPMTtoMu->SetFillStyle(3002);
    NonlinearityLPMTtoMu->SetMarkerColor(4);
    NonlinearityLPMTtoMu->SetLineColor(4);
    NonlinearityLPMTtoMu->SetMarkerSize(1.0);
    NonlinearityLPMTtoMu->SetMarkerStyle(20);
    NonlinearityLPMTtoMu->SetLineWidth(3);
    NonlinearityLPMTtoMu->GetXaxis()->CenterTitle();
    NonlinearityLPMTtoMu->GetYaxis()->CenterTitle();
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleFont(22);
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleSize(0.06);
    NonlinearityLPMTtoMu->GetXaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu->GetYaxis()->SetLabelSize(0.06);
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleOffset(1.2);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleOffset(1.30);

    TGraphErrors *TrueNonlinearityLPMTtoMu = new TGraphErrors(Vector_LPMTMu.size(),&Vector_LPMTMu[0],&TrueNonlinearity[0],&LPMTMuError[0],&TrueNonlinearityError[0]);
    TrueNonlinearityLPMTtoMu->SetTitle("");
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT}(p.e.)");
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitle("Nonlinearity");
    TrueNonlinearityLPMTtoMu->SetFillStyle(3002);
    TrueNonlinearityLPMTtoMu->SetMarkerColor(3);
    TrueNonlinearityLPMTtoMu->SetLineColor(3);
    TrueNonlinearityLPMTtoMu->SetMarkerSize(1.0);
    TrueNonlinearityLPMTtoMu->SetMarkerStyle(20);
    TrueNonlinearityLPMTtoMu->SetLineWidth(3);
    TrueNonlinearityLPMTtoMu->GetXaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu->GetYaxis()->CenterTitle();
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitleFont(22);
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitleSize(0.06);
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetLabelSize(0.06);
    TrueNonlinearityLPMTtoMu->GetXaxis()->SetTitleOffset(1.2);
    TrueNonlinearityLPMTtoMu->GetYaxis()->SetTitleOffset(1.30);

    TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,0,130,1);
    SetNonlinearity->SetParameter(0,0);

    TLegend *legenda =new TLegend(0.5,0.4327731,0.9942693,0.6365546);
    legenda->SetTextFont(22);
    legenda->AddEntry(NonlinearityLPMTtoMu,"Calibrated nonlinearity","lpe");
    legenda->AddEntry(TrueNonlinearityLPMTtoMu,"True nonlinearity","lpe");
    legenda->AddEntry(SetNonlinearity,"Set value","l"); 
    legenda->SetLineColor(0);

    TCanvas *NonlinearityLPMTtoMu_Canvas = new TCanvas("NonlinearityLPMTtoMu_Canvas","NonlinearityLPMTtoMu_Canvas",1);
    NonlinearityLPMTtoMu_Canvas->SetBorderMode(0);
    NonlinearityLPMTtoMu_Canvas->SetBorderSize(2);
    NonlinearityLPMTtoMu_Canvas->SetLeftMargin(0.1504298);
    NonlinearityLPMTtoMu_Canvas->SetRightMargin(0.0487106);
    NonlinearityLPMTtoMu_Canvas->SetTopMargin(0.02941176);
    NonlinearityLPMTtoMu_Canvas->SetBottomMargin(0.1701681);
    NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
    NonlinearityLPMTtoMu_Canvas->SetFrameBorderMode(0);
    NonlinearityLPMTtoMu_Canvas->cd();

    NonlinearityLPMTtoMu->Draw("A P");
    TrueNonlinearityLPMTtoMu->Draw("P");
    SetNonlinearity->Draw("same");
    legenda->Draw("same");
    TString OutputName = "/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12_OldOpticalModel/result/Graph_"+ChosenTypeString+"_Cut"+TString::Format("%1d",4)+".root";
    TFile *outputfile = TFile::Open(OutputName,"recreate");
    npeLPMTtonpeSPMT_tot->Write();
    TruenpeLPMTtonpeSPMT_tot->Write();
    outputfile->Close();
}
