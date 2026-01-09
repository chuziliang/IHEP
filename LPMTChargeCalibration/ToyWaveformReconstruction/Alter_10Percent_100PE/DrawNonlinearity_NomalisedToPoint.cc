struct DataWithError
{
    double Data;
    double Error;
};

Double_t SetNonlinearityFunction(Double_t *x, Double_t *par)
{
    double xx =x[0]; // 有非线性的电荷值
    double miu = 500 - sqrt(500*500 - 1000*xx); // 真实的电荷值
    Double_t f = (1-0.1*miu/100.0)/(1-0.1*par[0]/100.0)-1;
    // Double_t f = xx/miu - 1;
    return f;
}

DataWithError MeanResult(const vector<double>& x, const vector<double>& e)
{
    double sum_x = 0.0;
    double sum_w = 0.0;
    double sum_e = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1;
        // if (x[i] > 0)
        {
            sum_x += w * x[i];
            sum_w += w;
            sum_e += e[i]*e[i];
        }
    }
    cout<<"Chosen LPMTs = "<<x.size()<<endl;
    sum_e = sqrt(sum_e)/sum_w;
    DataWithError combined_result;
    combined_result.Data = sum_x / sum_w;
    combined_result.Error = sum_e;

    return combined_result;
}

DataWithError CombineResult(const vector<double>& x, const vector<double>& e)
{
    double sum_x = 0.0;
    double sum_w = 0.0;
    double sum_e = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1/(e[i]*e[i]);
        // if (x[i] > 0)
        {
            sum_x += w * x[i];
            sum_w += w;
            // sum_e += e[i]*e[i];
        }
    }
    cout<<"Chosen LPMTs = "<<x.size()<<endl;
    sum_e = sqrt(1/sum_w);
    DataWithError combined_result;
    combined_result.Data = sum_x / sum_w;
    combined_result.Error = sum_e;

    return combined_result;
}

DataWithError TotalResult(const vector<double>& x, const vector<double>& e)
{
    double sum_x = 0.0;
    double sum_w = 0.0;
    double sum_e = 0.0;

    for (int i = 0; i < x.size(); i++) {
        double w = 1;
        // if (x[i] > 0)
        {
            sum_x += w * x[i];
            sum_w += w;
            sum_e += e[i]*e[i];
        }
    }
    sum_e = sqrt(sum_e);
    DataWithError combined_result;
    combined_result.Data = sum_x;
    combined_result.Error = sum_e;

    return combined_result;
}

void DrawNonlinearity_NomalisedToPoint()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/npeLPMTtonpeSPMT_NormalisedToPoint.root","read");
    TTree *Data = (TTree*)inputfile->Get("Data");//定义输出的tree及输出的内容

    vector<double>* AverageMuSPMT_QE = new vector<double>();
    vector<double>* MuLPMT = new vector<double>();
    vector<double>* CalibMuLPMT = new vector<double>();
    vector<double>* DetTotalPE = new vector<double>();
    vector<double>* Nonlinearity = new vector<double>();
    vector<double>* NonlinearityError = new vector<double>();
    vector<double>* AverageMuSPMT_QEError = new vector<double>();
    vector<double>* MuLPMTError = new vector<double>();
    vector<double>* CalibMuLPMTError = new vector<double>();
    vector<double>* DetTotalPEError = new vector<double>();
    vector<double>* TrueMuLPMT = new vector<double>();
    vector<double>* TrueMuLPMTError = new vector<double>();
    int LPMTID;
    Data->SetBranchAddress("AverageMuSPMT_QE",&AverageMuSPMT_QE);
    Data->SetBranchAddress("MuLPMT",&MuLPMT);
    Data->SetBranchAddress("CalibMuLPMT",&CalibMuLPMT);
    Data->SetBranchAddress("TotalPE",&DetTotalPE);
    Data->SetBranchAddress("Nonlinearity",&Nonlinearity);
    Data->SetBranchAddress("NonlinearityError",&NonlinearityError);
    Data->SetBranchAddress("AverageMuSPMT_QEError",&AverageMuSPMT_QEError);
    Data->SetBranchAddress("MuLPMTError",&MuLPMTError);
    Data->SetBranchAddress("CalibMuLPMTError",&CalibMuLPMTError);
    Data->SetBranchAddress("TotalPEError",&DetTotalPEError);
    Data->SetBranchAddress("TrueMuLPMT",&TrueMuLPMT);
    Data->SetBranchAddress("TrueMuLPMTError",&TrueMuLPMTError);
    Data->SetBranchAddress("LPMTID",&LPMTID);
   
    double TotalNonlinearity[21];
    double TotalNonlinearityError[21];
    double TotalPE[21];
    double TotalPEError[21];
    double TotalPECalib[21];
    double TotalPECalibError[21];
    double TotalPERec[21];
    double TotalPERecError[21];
    double RatioCalib[21];
    double RatioCalibError[21];
    double RatioRec[21];
    double RatioRecError[21]; 
    double AverageMu[21];
    double AverageMuError[21];

	// TFile *TotalPEfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/zhangxin/TotalPE.root");
	// TTree *TotalPETree = (TTree*)TotalPEfile->Get("TotalPETree");
	// double TotalPEData;
	// double TotalPEDataError;
	// TotalPETree->SetBranchAddress("TotalPEData",&TotalPEData);
	// TotalPETree->SetBranchAddress("TotalPEError",&TotalPEDataError);
	// for (int i = 0; i<21; i++)
	// {
	// 	TotalPETree->GetEntry(i);
	// 	TotalPE[i] = TotalPEData;
	// 	TotalPEError[i] = TotalPEDataError;
	// }
    for (int FileNum = 0; FileNum<21; FileNum++)
    {
        vector<double> ToGet_TotalNonlinearity;
        vector<double> ToGet_TotalNonlinearityError;
        vector<double> ToGet_TotalPE;
        vector<double> ToGet_TotalPEError;
        vector<double> ToGet_TotalPECalib;
        vector<double> ToGet_TotalPECalibError;
        vector<double> ToGet_TotalPERec;
        vector<double> ToGet_TotalPERecError;

        for (int PMTID = 0; PMTID<17612; PMTID++)
        {
            Data->GetEntry(PMTID);
            if ((*NonlinearityError)[FileNum] > 0 )
            {
                ToGet_TotalNonlinearity.push_back((*Nonlinearity)[FileNum]);
                ToGet_TotalNonlinearityError.push_back((*NonlinearityError)[FileNum]);
            }
            if ((!std::isnan((double)(*CalibMuLPMT)[FileNum])) && (!std::isnan((double)(*CalibMuLPMTError)[FileNum])))
            {
                ToGet_TotalPECalib.push_back((*CalibMuLPMT)[FileNum]);
                ToGet_TotalPECalibError.push_back((*CalibMuLPMTError)[FileNum]);
            }
            else
            {
                cout<<PMTID<<endl;
            }
            
            
            ToGet_TotalPE.push_back((*TrueMuLPMT)[FileNum]);
            ToGet_TotalPEError.push_back((*TrueMuLPMTError)[FileNum]);
            ToGet_TotalPERec.push_back((*MuLPMT)[FileNum]);
            ToGet_TotalPERecError.push_back((*MuLPMTError)[FileNum]);
        }
        DataWithError result;
        result = MeanResult(ToGet_TotalNonlinearity,ToGet_TotalNonlinearityError);
        TotalNonlinearity[FileNum]=result.Data;
        TotalNonlinearityError[FileNum]=result.Error;
        result = TotalResult(ToGet_TotalPE,ToGet_TotalPEError);
        TotalPE[FileNum]=result.Data;
        TotalPEError[FileNum]=result.Error;
        result = TotalResult(ToGet_TotalPECalib,ToGet_TotalPECalibError);
        TotalPECalib[FileNum]=result.Data;
        TotalPECalibError[FileNum]=result.Error;
        result = TotalResult(ToGet_TotalPERec,ToGet_TotalPERecError);
        TotalPERec[FileNum]=result.Data;
        TotalPERecError[FileNum]=result.Error;
        result = MeanResult(ToGet_TotalPERec,ToGet_TotalPERecError);
        AverageMu[FileNum]=result.Data;
        AverageMuError[FileNum]=result.Error;

        RatioCalib[FileNum]=TotalPECalib[FileNum]/TotalPE[FileNum];
        RatioCalibError[FileNum]=RatioCalib[FileNum]*sqrt((TotalPECalibError[FileNum]*TotalPECalibError[FileNum])/(TotalPECalib[FileNum]*TotalPECalib[FileNum])+(TotalPEError[FileNum]*TotalPEError[FileNum])/(TotalPE[FileNum]*TotalPE[FileNum]));
        RatioRec[FileNum]=TotalPERec[FileNum]/TotalPE[FileNum];
        RatioRecError[FileNum]=RatioRec[FileNum]*sqrt((TotalPERecError[FileNum]*TotalPERecError[FileNum])/(TotalPERec[FileNum]*TotalPERec[FileNum])+(TotalPEError[FileNum]*TotalPEError[FileNum])/(TotalPE[FileNum]*TotalPE[FileNum]));

        ToGet_TotalNonlinearity.clear();
        ToGet_TotalNonlinearityError.clear();
        ToGet_TotalPE.clear();
        ToGet_TotalPEError.clear();
        ToGet_TotalPECalib.clear();
        ToGet_TotalPECalibError.clear();
        ToGet_TotalPERec.clear();
        ToGet_TotalPERecError.clear();


        // cout<<"TotalNonlinearity = "<<TotalNonlinearity[FileNum]<<endl;
        // cout<<"TotalPECalib = "<<TotalPECalib[FileNum]<<endl;
        // cout<<"TotalNonlinearityError = "<<TotalNonlinearityError[FileNum]<<endl;
        // cout<<"TotalPECalibError = "<<TotalPECalibError[FileNum]<<endl;

        cout<<"FileNum "<<FileNum<<" finished."<<endl;
    }

    TCanvas *NonlinearityLPMTtoMu_Canvas = new TCanvas("NonlinearityLPMTtoMu_Canvas","NonlinearityLPMTtoMu_Canvas",1);
    TF1 *SetNonlinearity = new TF1("SetNonlinearity",SetNonlinearityFunction,0,130,1);
    
    NonlinearityLPMTtoMu_Canvas->cd();
    TGraphErrors *NonlinearityLPMTtoMu = new TGraphErrors(21,AverageMu,TotalNonlinearity,AverageMuError,TotalNonlinearityError);

    SetNonlinearity->SetParameter(0,AverageMu[9]);
    // NonlinearityLPMTtoMu->GetXaxis()->SetRangeUser(5,130);

    NonlinearityLPMTtoMu->GetXaxis()->SetTitle("#mu_{LPMT}");
    NonlinearityLPMTtoMu->GetYaxis()->SetTitle("Nonlinearity");
    NonlinearityLPMTtoMu->SetTitle("");
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
    NonlinearityLPMTtoMu->GetXaxis()->SetTitleOffset(0.83);
    NonlinearityLPMTtoMu->GetYaxis()->SetTitleOffset(0.83);

    TLegend *legendb =new TLegend(0.6,0.65,0.88,0.85);
    legendb->SetTextFont(22);
    legendb->AddEntry(NonlinearityLPMTtoMu,"Calibrated nonliearity","lpe");
    legendb->AddEntry(SetNonlinearity,"Preset value","l"); 
    
    NonlinearityLPMTtoMu->Draw("A P");
    SetNonlinearity->Draw("same");
    legendb->Draw("same");


    TCanvas *CalibToTrue_Canvas = new TCanvas("CalibToTrue_Canvas","CalibToTrue_Canvas",1);
    TPad* pad1 = new TPad("pad1", "pad1", 0.00, 0.32, 1.00, 0.95);
    TPad* pad2 = new TPad("pad1", "pad1", 0.00, 0, 1.00, 0.32);
    pad1->SetMargin(0.12, 0.1, 0., 0.0);
    pad2->SetMargin(0.12, 0.1, 0.3, 0.0);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();
    // CalibToTrue_Canvas->cd();
    TGraphErrors *CalibToTrue = new TGraphErrors(21,TotalPE,TotalPECalib,TotalPEError,TotalPECalibError);

    TF1 *Below1PE = new TF1("Below1PE","x",0,3000000);
    Below1PE->SetLineColor(kRed);

    // CalibToTrue->GetXaxis()->SetRangeUser(70000,3000000);

    CalibToTrue->GetXaxis()->SetTitle("TotalPE");
    CalibToTrue->GetYaxis()->SetTitle("Reconstructed TotalPE");
    CalibToTrue->SetTitle("");
    CalibToTrue->SetFillStyle(3002);
    CalibToTrue->SetMarkerColor(4);
    CalibToTrue->SetLineColor(4);
    CalibToTrue->SetMarkerSize(1.0);
    CalibToTrue->SetMarkerStyle(20);
    CalibToTrue->SetLineWidth(3);
    CalibToTrue->GetXaxis()->CenterTitle();
    CalibToTrue->GetYaxis()->CenterTitle();
    CalibToTrue->GetXaxis()->SetTitleFont(22);
    CalibToTrue->GetYaxis()->SetTitleFont(22);
    CalibToTrue->GetXaxis()->SetTitleSize(0.06);
    CalibToTrue->GetYaxis()->SetTitleSize(0.06);
    CalibToTrue->GetXaxis()->SetLabelSize(0.06);
    CalibToTrue->GetYaxis()->SetLabelSize(0.06);
    CalibToTrue->GetXaxis()->SetTitleOffset(0.83);
    CalibToTrue->GetYaxis()->SetTitleOffset(0.83);

    TGraphErrors *RecToTrue = new TGraphErrors(21,TotalPE,TotalPERec,TotalPEError,TotalPERecError);

    // RecToTrue->GetXaxis()->SetRangeUser(70000,3000000);

    RecToTrue->SetFillStyle(3002);
    RecToTrue->SetMarkerColor(6);
    RecToTrue->SetLineColor(6);
    RecToTrue->SetMarkerSize(1.0);
    RecToTrue->SetMarkerStyle(20);
    RecToTrue->SetLineWidth(3);

    TLegend *legenda =new TLegend(0.6,0.65,0.50,0.40);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.1);
    legenda->AddEntry(CalibToTrue,"After caliberation","lpe");
    legenda->AddEntry(RecToTrue,"Before caliberation","lpe");

    CalibToTrue->Draw("A P");
    RecToTrue->Draw("P");
    Below1PE->Draw("same");
    legenda->Draw();

    pad2->cd();
    TGraphErrors *CalibRatioToTrue = new TGraphErrors(21,TotalPE,RatioCalib,TotalPEError,RatioCalibError);
    CalibRatioToTrue->GetXaxis()->SetTitle("TotalPE");
    CalibRatioToTrue->GetYaxis()->SetTitle("Ratio");
    CalibRatioToTrue->SetTitle("");
    CalibRatioToTrue->SetFillStyle(3002);
    CalibRatioToTrue->SetMarkerColor(4);
    CalibRatioToTrue->SetLineColor(4);
    CalibRatioToTrue->SetMarkerSize(1.0);
    CalibRatioToTrue->SetMarkerStyle(20);
    CalibRatioToTrue->SetLineWidth(3);
    CalibRatioToTrue->GetXaxis()->CenterTitle();
    CalibRatioToTrue->GetYaxis()->CenterTitle();
    CalibRatioToTrue->GetXaxis()->SetTitleFont(22);
    CalibRatioToTrue->GetYaxis()->SetTitleFont(22);
    CalibRatioToTrue->GetXaxis()->SetTitleSize(0.17);
    CalibRatioToTrue->GetYaxis()->SetTitleSize(0.15);
    CalibRatioToTrue->GetXaxis()->SetLabelSize(0.12);
    CalibRatioToTrue->GetYaxis()->SetLabelSize(0.12);
    CalibRatioToTrue->GetXaxis()->SetTitleOffset(0.83);
    CalibRatioToTrue->GetYaxis()->SetTitleOffset(0.35);
    
    CalibRatioToTrue->Draw("A P");
}
