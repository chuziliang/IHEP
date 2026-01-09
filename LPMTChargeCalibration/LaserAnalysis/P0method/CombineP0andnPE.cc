void CombineP0andnPE()
{
    TFile *inputfilenPE = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/npeLPMTtonpeSPMT.root");
    TFile *inputfileP0 = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/P0method/npeLPMTtonpeSPMT.root");

    const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};
    TGraphErrors *npeLPMTtonpeSPMT_nPE[6];
    TGraphErrors *npeLPMTtonpeSPMT_P0[6];
    double QE[6];
    double qe;
    int spmtid;
    double p1_npe[6];
    double p1_P0[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT_nPE[id] = (TGraphErrors*) inputfilenPE->Get(GraphName);
        TString FunctionName = TString::Format("Function_%1d",id);
        TF1 *f = new TF1(FunctionName,"pol1");
        //TF1 *f = npeLPMTtonpeSPMT[id]->GetFunction(FunctionName);
        npeLPMTtonpeSPMT_nPE[id]->Fit(FunctionName,"0");
        p1_npe[id] = f->GetParameter(1);
        f->SetLineColor(8);
    }

    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d p(0)",id);
        npeLPMTtonpeSPMT_P0[id] = (TGraphErrors*) inputfileP0->Get(GraphName);
        TString FunctionName = TString::Format("Function_%1d",id);
        TF1 *f = new TF1(FunctionName,"pol1");
        //TF1 *f = npeLPMTtonpeSPMT[id]->GetFunction(FunctionName);
        npeLPMTtonpeSPMT_P0[id]->Fit(FunctionName,"0");
        p1_P0[id] = f->GetParameter(1);
        f->SetLineColor(7);
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

    TCanvas *c1 = new TCanvas("c1","c1",1);
    TGraph *QE2p1_npe = new TGraph(6,QE,p1_npe);
    QE2p1_npe->SetMarkerColor(1);
    QE2p1_npe->SetMarkerStyle(25);
    QE2p1_npe->Fit("pol1");
    //gStyle->SetOptFit(1111);
    QE2p1_npe->Draw("AP");

    TGraph *QE2p1_P0 = new TGraph(6,QE,p1_P0);
    QE2p1_P0->SetMarkerColor(2);
    QE2p1_P0->SetMarkerStyle(26);
    // QE2p1_P0->Fit("pol1");
    // gStyle->SetOptFit(1111);
    QE2p1_P0->Draw("P");
    TLegend *legend1=new TLegend(0.6,0.65,0.8,0.85);
    legend1->SetTextFont(72);
    legend1->SetTextSize(0.04);
    legend1->AddEntry(QE2p1_npe,"nPE","pe");
    legend1->AddEntry(QE2p1_P0,"P(0)","pe");
    legend1->Draw();

    TCanvas *c[6];
    for (int i = 0; i < 6; i++)
    {
        c[i] = new TCanvas(TString::Format("c_%1d",i),TString::Format("c_%1d",i),1);
        c[i]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT_nPE[i]->SetMarkerColor(1);
        npeLPMTtonpeSPMT_nPE[i]->SetLineColor(1);
        npeLPMTtonpeSPMT_nPE[i]->Draw("AP");
        npeLPMTtonpeSPMT_P0[i]->SetMarkerColor(2);
        npeLPMTtonpeSPMT_P0[i]->SetLineColor(2);
        npeLPMTtonpeSPMT_P0[i]->Draw("P");
        
        TLegend *legend=new TLegend(0.7,0.4,0.87,0.6);
        legend->SetTextFont(72);
        legend->SetTextSize(0.04);
        legend->AddEntry(npeLPMTtonpeSPMT_nPE[i],"nPE","lpe");
        legend->AddEntry(npeLPMTtonpeSPMT_P0[i],"P(0)","lpe");
        legend->Draw();

    }
    

    
}