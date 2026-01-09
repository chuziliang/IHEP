struct DataWithError
{
    double Data;
    double Error;
};

DataWithError CombineResult(const vector<double>& x, const vector<double>& e)
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

void DrawNonlinearity()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/npeLPMTtonpeSPMT_AllLPMT.root","read");
    TTree *Data = (TTree*)inputfile->Get("Data");//定义输出的tree及输出的内容

    vector<double>* AverageMuSPMT = new vector<double>();
    vector<double>* MuLPMT = new vector<double>();
    vector<double>* CalibMuLPMT = new vector<double>();
    vector<double>* DetTotalPE = new vector<double>();
    vector<double>* Nonlinearity = new vector<double>();
    vector<double>* NonlinearityError = new vector<double>();
    vector<double>* AverageMuSPMTError = new vector<double>();
    vector<double>* MuLPMTError = new vector<double>();
    vector<double>* CalibMuLPMTError = new vector<double>();
    vector<double>* DetTotalPEError = new vector<double>();
    int LPMTID;
    Data->SetBranchAddress("AverageMuSPMT",&AverageMuSPMT);
    Data->SetBranchAddress("MuLPMT",&MuLPMT);
    Data->SetBranchAddress("CalibMuLPMT",&CalibMuLPMT);
    Data->SetBranchAddress("TotalPE",&DetTotalPE);
    Data->SetBranchAddress("Nonlinearity",&Nonlinearity);
    Data->SetBranchAddress("NonlinearityError",&NonlinearityError);
    Data->SetBranchAddress("AverageMuSPMTError",&AverageMuSPMTError);
    Data->SetBranchAddress("MuLPMTError",&MuLPMTError);
    Data->SetBranchAddress("CalibMuLPMTError",&CalibMuLPMTError);
    Data->SetBranchAddress("TotalPEError",&DetTotalPEError);
    Data->SetBranchAddress("LPMTID",&LPMTID);
    
    double TotalNonlinearity[21];
    double TotalNonlinearityError[21];
    double TotalPE[21];
    double TotalPEError[21];
    double TotalPECalib[21];
    double TotalPECalibError[21];
    for (int FileNum = 0; FileNum<21; FileNum++)
    {
        vector<double> ToGet_TotalNonlinearity;
        vector<double> ToGet_TotalNonlinearityError;
        vector<double> ToGet_TotalPE;
        vector<double> ToGet_TotalPEError;
        vector<double> ToGet_TotalPECalib;
        vector<double> ToGet_TotalPECalibError;
        vector<double> ToGet_TotalPE_ByMu;
        vector<double> ToGet_TotalPE_ByMuError;

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
            
            
            ToGet_TotalPE.push_back((*DetTotalPE)[FileNum]);
            ToGet_TotalPEError.push_back((*DetTotalPEError)[FileNum]);
        }
        DataWithError result;
        result = CombineResult(ToGet_TotalNonlinearity,ToGet_TotalNonlinearityError);
        TotalNonlinearity[FileNum]=result.Data;
        TotalNonlinearityError[FileNum]=result.Error;
        result = CombineResult(ToGet_TotalPE,ToGet_TotalPEError);
        TotalPE[FileNum]=result.Data;
        TotalPEError[FileNum]=result.Error;
        result = TotalResult(ToGet_TotalPECalib,ToGet_TotalPECalibError);
        TotalPECalib[FileNum]=result.Data;
        TotalPECalibError[FileNum]=result.Error;

        ToGet_TotalNonlinearity.clear();
        ToGet_TotalNonlinearityError.clear();
        ToGet_TotalPE.clear();
        ToGet_TotalPEError.clear();
        ToGet_TotalPECalib.clear();
        ToGet_TotalPECalibError.clear();


        // cout<<"TotalNonlinearity = "<<TotalNonlinearity[FileNum]<<endl;
        // cout<<"TotalPECalib = "<<TotalPECalib[FileNum]<<endl;
        // cout<<"TotalNonlinearityError = "<<TotalNonlinearityError[FileNum]<<endl;
        // cout<<"TotalPECalibError = "<<TotalPECalibError[FileNum]<<endl;

        cout<<"FileNum "<<FileNum<<" finished."<<endl;
    }

    TCanvas *NonlinearityLPMTtoMu_Canvas = new TCanvas("NonlinearityLPMTtoMu_Canvas","NonlinearityLPMTtoMu_Canvas",1);
    NonlinearityLPMTtoMu_Canvas->cd();
    TGraphErrors *NonlinearityLPMTtoMu = new TGraphErrors(21,TotalPE,TotalNonlinearity,TotalPEError,TotalNonlinearityError);

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

    NonlinearityLPMTtoMu->Draw("A P");

    TCanvas *CalibToTrue_Canvas = new TCanvas("CalibToTrue_Canvas","CalibToTrue_Canvas",1);
    CalibToTrue_Canvas->cd();
    TGraphErrors *CalibToTrue = new TGraphErrors(21,TotalPE,TotalPECalib,TotalPEError,TotalPECalibError);

    TF1 *Below1PE = new TF1("Below1PE","x",0,3000000);
    Below1PE->SetLineColor(kRed);

    CalibToTrue->GetXaxis()->SetTitle("TotalPE");
    CalibToTrue->GetYaxis()->SetTitle("Calib TotalPE");
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

    CalibToTrue->Draw("A P");
    Below1PE->Draw("same");
}