void P1toQE()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/npeLPMTtonpeSPMT.root");
    const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};
    TGraphErrors *npeLPMTtonpeSPMT[6];
    double QE[6];
    double qe;
    int spmtid;
    double p1[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT[id] = (TGraphErrors*) inputfile->Get(GraphName);
        TString FunctionName = TString::Format("Function_%1d",id);
        TF1 *f = new TF1(FunctionName,"pol1");
        //TF1 *f = npeLPMTtonpeSPMT[id]->GetFunction(FunctionName);
        npeLPMTtonpeSPMT[id]->Fit(FunctionName);
        p1[id] = f->GetParameter(1);
    }

    TFile *PMTParam_CD_SPMT = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root");
    TTree *t1 =(TTree*) PMTParam_CD_SPMT->Get("data");
    t1->SetBranchAddress("QE",&qe);
    t1->SetBranchAddress("pmtID",&spmtid);
    for (int id = 0; id < 6; id++)
    {
        t1->GetEntry(SPMT_ID[id]-300000);
        if (spmtid == SPMT_ID[id])
        {
            QE[id] = 1.0/qe;
        }
        else
        {
            cout<<"PMT id isn't correct"<<id<<endl;
        }
    }

    TGraph *result = new TGraph(6,QE,p1);
    result->SetMarkerColor(4);
    result->SetMarkerStyle(20);
    result->Fit("pol1");
    gStyle->SetOptFit(1111);
    result->Draw("AP");
    
    
}