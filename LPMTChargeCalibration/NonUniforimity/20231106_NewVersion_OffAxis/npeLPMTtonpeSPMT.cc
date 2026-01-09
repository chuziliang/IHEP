//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 

const int LPMTID = 8951;
const int SPMTID[6] = {303780,304088,304089,304230,304231,324324};

std::string readSpecificLine(int lineNumber) 
{
    std::ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231106_NewVersion_OffAxis/folder_path.txt");
    std::string line;
    int currentLine = 0;

    while (std::getline(file, line)) 
    {
        if (currentLine == lineNumber) 
        {
            return line;
        }
        currentLine++;
    }
    return ""; // 如果未找到指定行，则返回空字符串
}

double GetSPMTQE(int SPMTID)
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    PMTData->GetEntry(SPMTID-300000);
    if (PMTID == SPMTID)
    {
        return 100*Effiency;
    }
    else
    {
        cout<<"Wrong SPMTID!!!"<<endl;
        return 0;
    }
}

double GetLPMTQE(int LPMTID)
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    PMTData->GetEntry((Long64_t)LPMTID);
    if (PMTID == LPMTID)
    {
        return Effiency;
    }
    else
    {
        cout<<"Wrong LPMTID!!!"<<endl;
        return 0;
    }
}


void npeLPMTtonpeSPMT()
{
    double LPMTEffiency = GetLPMTQE(LPMTID);
    double SPMTEffiency[6];
    for (int id = 0; id < 6; id++)
    {
        SPMTEffiency[id] = GetSPMTQE(SPMTID[id]);
    }
    vector<double> TotFactor;
    vector<double> TotFactorError;
    vector<double> TotTheta;
    vector<double> TotThetaError;
    vector<double> TotPhi;
    vector<double> TotPhiError;
    
    for (int num = 0; num < 227; num++)
    {
        string inputname;
        string subname = readSpecificLine(num);
        inputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231106_NewVersion_OffAxis/CombineChargeSpectrum/" + subname + ".root";
        TFile *inputfile = TFile::Open((TString)inputname,"read");
        TTree *charge = (TTree*)inputfile->Get("Charge");
        double LPMTnpe; 
        double SPMTnpe[6];
        double Theta;
        double Phi;
        charge->SetBranchAddress("LPMTCharge",&LPMTnpe);
        charge->SetBranchAddress("SPMTCharge",SPMTnpe);
        charge->SetBranchAddress("Theta",&Theta);
        charge->SetBranchAddress("Phi",&Phi);

        TH1 *LPMTChargeSpectrum;
        string LPMTHistName = "LPMT charge spectrum" + subname;
        LPMTChargeSpectrum = new TH1F((TString)LPMTHistName,(TString)LPMTHistName,1000,0,1000);
        TH1 *SPMTChargeSpectrum[6];
        for (int id = 0; id < 6; id++)
        {
            string SPMTHistName = "LPMT charge spectrum" + subname + to_string(id);
            SPMTChargeSpectrum[id] = new TH1F((TString)SPMTHistName,(TString)SPMTHistName,300,0,300);
        }
        TH1 *ThetaSpectrum;
        string ThetaHistName = "Theta spectrum" + subname;
        ThetaSpectrum = new TH1F((TString)ThetaHistName,(TString)ThetaHistName,1800,0,180);
        TH1 *PhiSpectrum;
        string PhiHistName = "Phi spectrum" + subname;
        PhiSpectrum = new TH1F((TString)PhiHistName,(TString)PhiHistName,1800,-90,90);


        for (int i = 0; i < charge->GetEntries(); i++)
        {
            charge->GetEntry(i);
            LPMTChargeSpectrum->Fill(LPMTnpe);
            for (int id = 0; id < 6; id++)
            {
                SPMTChargeSpectrum[id]->Fill(SPMTnpe[id]);
            }
            ThetaSpectrum->Fill(Theta);
            PhiSpectrum->Fill(Phi);
        }
        double LPMTMean = LPMTChargeSpectrum->GetMean();
        double LPMTError = LPMTChargeSpectrum->GetMeanError();
        double SPMTMean[6];
        double SPMTMeanError[6];
        for (int id = 0; id < 6; id++)
        {
            SPMTMean[id] = SPMTChargeSpectrum[id]->GetMean();
            SPMTMeanError[id] = SPMTChargeSpectrum[id]->GetMeanError();
        }
        double ThetaMean = ThetaSpectrum->GetMean();
        double ThetaMeanError = ThetaSpectrum->GetMeanError();
        double PhiMean = PhiSpectrum->GetMean();
        double PhiMeanError = PhiSpectrum->GetMeanError();
        
        double Factor;
        double FactorError;
        for (int id = 0; id < 6; id++)
        {
            double ThisFactor = (LPMTMean/LPMTEffiency)/(SPMTMean[id]/SPMTEffiency[id]);
            Factor += ThisFactor;
            FactorError += ThisFactor*ThisFactor *( (LPMTError/LPMTMean)*(LPMTError/LPMTMean)+(SPMTMeanError[id]/SPMTMean[id])*(SPMTMeanError[id]/SPMTMean[id]) );
        }
        Factor /= 6;
        FactorError = sqrt(FactorError);
        FactorError /= 6;
        inputfile->Close();

        TotFactor.push_back(Factor);
        TotFactorError.push_back(FactorError);
        TotTheta.push_back(ThetaMean);
        TotThetaError.push_back(ThetaMeanError);
        TotPhi.push_back(PhiMean);
        TotPhiError.push_back(PhiMeanError);

        cout<<num<<" Finished"<<endl;
    }

    TGraphErrors *Factor_positions = new TGraphErrors(TotFactor.size(),
                                                            &TotTheta[0],&TotFactor[0],
                                                            &TotThetaError[0],&TotFactorError[0]);

    TCanvas *c2 = new TCanvas("c2","c2",1200,900);
    Factor_positions->SetFillStyle(3002);
    Factor_positions->SetMarkerSize(0.8);
    Factor_positions->SetMarkerStyle(20);
    Factor_positions->SetTitle("");
    Factor_positions->SetFillStyle(3002);
    Factor_positions->SetMarkerColor(4);
    Factor_positions->SetLineColor(4);
    Factor_positions->SetMarkerSize(1.0);
    Factor_positions->SetMarkerStyle(20);
    Factor_positions->SetLineWidth(2);
    Factor_positions->GetXaxis()->CenterTitle();
    Factor_positions->GetYaxis()->CenterTitle();
    Factor_positions->GetXaxis()->SetTitleFont(22);
    Factor_positions->GetYaxis()->SetTitleFont(22);
    Factor_positions->GetXaxis()->SetTitleSize(0.06);
    Factor_positions->GetYaxis()->SetTitleSize(0.06);
    Factor_positions->GetXaxis()->SetLabelSize(0.06);
    Factor_positions->GetYaxis()->SetLabelSize(0.06);
    Factor_positions->GetXaxis()->SetTitleOffset(0.83);
    Factor_positions->GetYaxis()->SetTitleOffset(1.30);

   
    Factor_positions->SetTitle("");
    Factor_positions->GetXaxis()->SetTitle("Angle of incidence [deg]");
    Factor_positions->GetYaxis()->SetTitle("#mu_{LPMT}/#mu_{SPMT}");

    // Factor_positions->Draw("a");
    Factor_positions->Draw("a P");

    // 在x=48处添加一条直线
    double x1 = 48;
    double y1 = 37;
    double x2 = 48;
    double y2 = 129;
    TLine *line = new TLine(x1, y1, x2, y2);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(3);
    line->Draw();

    // 在x=52处添加一条直线
    double x3 = 52;
    double y3 = 37;
    double x4 = 52;
    double y4 = 129;
    TLine *line2 = new TLine(x3, y3, x4, y4);
    line2->SetLineColor(kGreen);
    line2->SetLineStyle(9);
    line2->SetLineWidth(3);
    line2->Draw();

    TLegend *leg = new TLegend(0.5, 0.7, 0.8, 0.8);
    leg->AddEntry(line, "SPMT coverd by LPMT", "L");
    leg->AddEntry(line2, "SPMT and LPMT coverd by supporting bar", "L");
    leg->Draw();
    leg->SetBorderSize(0);
    leg->SetFillStyle(0); 
    leg->SetTextFont(22);
}