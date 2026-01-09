//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。 

const int Points = 21;
const double Degree[6]={4.9,9.9,19.7,29.6,39.5,44.4};
const int LPMT_ID = 6828;

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

double combined_measurement(const vector<double>& x, const vector<double>& e) {
    double sum_x = 0.0;
    double sum_w = 0.0;

    for (int i = 0; i < x.size(); i++) {
        // double w = 1 / (e[i] * e[i]);
        double w = 1;
        sum_x += w * x[i];
        sum_w += w;
    }

    double combined_x = sum_x / sum_w;
    return combined_x;
}

double combined_error(const vector<double>& e) {
    double sum_w = 0.0;

    for (int i = 0; i < e.size(); i++) {
        double w = (e[i] * e[i]);
        sum_w += w;
    }

    double combined_e = sqrt(sum_w);
    return combined_e;
}

void npeLPMTtonpeSPMT()
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    int CorrespondingSPMT[17612][6];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231114_OffAxis/TrueNonlinearity/CombineSpectrum/ChargeSpectrum.root","read");
    double LPMTMean[Points];
    double SPMTMean[6][Points];
    double LPMTError[Points];
    double SPMTError[6][Points];

    double SPMTMean_tot[Points];
    double SPMTError_tot[Points];

    TH1 *LPMTnpeSpectrum;
    TH1 *SPMTnpeSpectrum[6];
    vector<double> SPMT_value = vector<double>(6, 0);
    vector<double> SPMT_Error = vector<double>(6, 0);
    for (int i = 0; i < Points; i++)
    {
        TString LPMTnpeSpectrumName = TString::Format("LPMT nPE spectrun %1dMeV %.1fDEG",energy[i],0.0);
        LPMTnpeSpectrum = (TH1*)inputfile->Get(LPMTnpeSpectrumName);
        LPMTMean[i] = LPMTnpeSpectrum->GetMean()/LPMTQE[LPMT_ID];
        LPMTError[i] = LPMTnpeSpectrum->GetMeanError()/LPMTQE[LPMT_ID];
        TString SPMTnpeSpectrumName[6];
        // SPMT_value = vector<double>(6, 0);
        // SPMT_Error = vector<double>(6, 0);
        for (int id = 0; id < 6; id++)
        {
            SPMTnpeSpectrumName[id] = TString::Format("SPMT_%1d nPE spectrun %1dMeV %.1fDEG",id,energy[i],0.0);
            SPMTnpeSpectrum[id] = (TH1*)inputfile->Get(SPMTnpeSpectrumName[id]);
            
            SPMTMean[id][i] = SPMTnpeSpectrum[id]->GetMean()/SPMTQE[CorrespondingSPMT[LPMT_ID][id] - 300000];
            SPMTError[id][i] = SPMTnpeSpectrum[id]->GetMeanError()/SPMTQE[CorrespondingSPMT[LPMT_ID][id] - 300000];
            SPMT_value[id] = SPMTMean[id][i];
            SPMT_Error[id] = SPMTError[id][i]; 
        }
        SPMTMean_tot[i] = combined_measurement(SPMT_value,SPMT_Error);
        SPMTError_tot[i] = combined_error(SPMT_Error);
        
    }
    inputfile->Close();

    TFile *output = new TFile("/junofs/users/chuziliang125/LPMTChargeCalibration/Spallation_neutron/DetetorCenterResult/npeLPMTtonpeSPMT.root","RECREATE");
    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(Points,SPMTMean[id],LPMTMean,SPMTError[id],LPMTError);
        npeLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeLPMTtonpeSPMT[id]->SetName(GraphName);
        npeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        TF1 *f1 = new TF1("f1","[0]*x",0,10);
        f1->SetParNames("k");
        npeLPMTtonpeSPMT[id]->Fit(f1);
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);


        TString CanvasName = TString::Format("C%1d",id);
        Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        Canvas[id]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT[id]->Draw("AP");
        output->cd();
        npeLPMTtonpeSPMT[id]->Write();
    }

    TGraphErrors *npeLPMTtonpeSPMT_tot;
    TCanvas *C7 = new TCanvas("tot","tot",1);
    TString GraphName_tot = "LPMT npe to SPMT npe tot";
    npeLPMTtonpeSPMT_tot = new TGraphErrors(Points,SPMTMean_tot,LPMTMean,SPMTError_tot,LPMTError);
    npeLPMTtonpeSPMT_tot->SetTitle(GraphName_tot);
    npeLPMTtonpeSPMT_tot->SetName(GraphName_tot);
    npeLPMTtonpeSPMT_tot->GetXaxis()->SetTitle("nPE SPMT");
    npeLPMTtonpeSPMT_tot->GetYaxis()->SetTitle("nPE LPMT");
    TF1 *f1 = new TF1("f1","[0]*x",0,10);
    f1->SetParNames("k");
    npeLPMTtonpeSPMT_tot->Fit(f1);
    gStyle->SetOptFit(1111);
    gStyle->SetStatY(0.5);

    C7->cd();
    gPad->SetLogx();
    gPad->SetLogy();
    npeLPMTtonpeSPMT_tot->Draw("AP");
    output->cd();
    npeLPMTtonpeSPMT_tot->Write();
}