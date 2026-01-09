double GetSPMTQE(double (&SPMTQE)[25600])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    for (int id = 0; id < 25600; id ++)
    {
        PMTData->GetEntry(id);
        SPMTQE[id] = Effiency;
        if (PMTID - 300000 != id)
        {
            cout<<"Wrong SPMTID!!!"<<endl;
            inputfile->Close();
            return 0;
        }
        
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
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    for (int id = 0; id < 17612; id ++)
    {
        PMTData->GetEntry(id);
        LPMTQE[id] = Effiency/100.0;
        if (PMTID != id)
        {
            cout<<"Wrong LPMTID!!!"<<endl;
            inputfile->Close();
            return 0;
        }

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

int GetLPMTType(int (&LPMTType)[17612])
{
    ifstream inputfile_LPMT;
    TString filename_LPMT="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTType_CD_LPMT.csv";
    inputfile_LPMT.open(filename_LPMT,ios::in);
    if(!inputfile_LPMT.good()) {cout<<"error"<<endl;return 0;}
    int PMTID;
    string PMTType;
    for (int i = 0; i < 17612; i++)
    {
        inputfile_LPMT>>PMTID>>PMTType;

        if (PMTType == "Hamamatsu")
        {
            LPMTType[PMTID] = 1;
        }
        else if (PMTType == "NNVT")
        {
            LPMTType[PMTID] = 2;
        }
        else if (PMTType == "HighQENNVT")
        {
            LPMTType[PMTID] = 3;
        }
        else
        {
            cout<<"Error Wrong Type 1!!"<<endl;
        }
        
    }

    return 0;
}

void Analysis()
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    int LPMTType[17612];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetLPMTType(LPMTType);

    TFile *inputfile = TFile::Open("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/test/detsim_0_DisableSticks.root");
    if (!inputfile)
    {
        cerr<<"ERROR: root file not exist!"<<endl;
        return 0;
    }
    inputfile->cd();

    TStatistic *LPMTMeanQE[17612];
    TStatistic *SPMTMeanQE[25600];

    for (int ID = 0; ID < 17612; ID++)
    {
        LPMTMeanQE[ID] = new TStatistic(TString::Format("LPMT Mean_QE Chn%1d",ID));
    }
    
    for (int ID = 0; ID < 25600; ID++)
    {
        SPMTMeanQE[ID] = new TStatistic(TString::Format("SPMT Mean_QE Chn%1d",ID));
    }

    TTree *Evt = (TTree*)inputfile->Get("Event/Sim/SimEvt");

    JM::SimEvt *sim_evt = NULL;
    vector<JM::SimPMTHit*> m_cd_hits;
    vector<JM::SimTrack*>  m_tracks;

    int m_nhits = 0;
    int pmtid = 0;
    int npe = 0;

    Evt->SetBranchAddress("SimEvt",&sim_evt);
    Evt->SetBranchAddress("m_nhits",&m_nhits);

    Int_t nentries = (Int_t)Evt->GetEntries();
    double OutputLPMT[17612];
    double OutputSPMT[25600];
    for (int i = 0; i < nentries; i++)
    {
        for (int ID = 0; ID < 17612; ID++)
        {
            OutputLPMT[ID] = 0;
        }
        for (int ID = 0; ID < 25600; ID++)
        {
            OutputSPMT[ID] = 0;
        }

        Evt->GetEntry(i);
        m_cd_hits = sim_evt->getCDHitsVec();

        if (m_nhits != m_cd_hits.size())
        {
            cerr<<"ERROR m_nhits != m_cd_hits.size()!"<<endl;
        }
        
        for (int HitNum = 0; HitNum<m_nhits; HitNum++)
        {
            JM::SimPMTHit* hit_info = m_cd_hits[HitNum];
            
            pmtid = hit_info->getPMTID();
            npe = hit_info->getNPE();
            if (pmtid < 30000)
            {
                OutputLPMT[pmtid] += npe;
            }
            else
            {
                OutputSPMT[pmtid - 300000] += npe;
            }
        }
        
        for (int ID = 0; ID < 17612; ID++)
        {
            LPMTMeanQE[ID]->Fill(OutputLPMT[ID]);
        }
        for (int ID = 0; ID < 25600; ID++)
        {
            SPMTMeanQE[ID]->Fill(OutputSPMT[ID]);
        }

        inputfile->cd();
        // cout<<i<<" Finished"<<endl;
    }
    inputfile->Close();

    TFile *outputfile = TFile::Open("result_DisableSticks.root","recreate");
    outputfile->cd();
    TH1 *LPMTResult = new TH1F("LPMT Charge_QE","LPMT Charge_QE",2000,0,2);

    TH1 *LPMTResult_H = new TH1F("Hamamatsu Charge_QE","Hamamatsu Charge_QE",2000,0,2);
    LPMTResult_H->SetLineColor(kRed);
    TH1 *LPMTResult_N = new TH1F("NNVT Charge_QE","NNVT Charge_QE",2000,0,2);
    LPMTResult_N->SetLineColor(kGreen);
    TH1 *LPMTResult_HQ = new TH1F("HighQENNVT Charge_QE","HighQENNVT Charge_QE",2000,0,2);
    LPMTResult_HQ->SetLineColor(kOrange);
    
    TH1 *SPMTResult = new TH1F("SPMT Charge_QE","SPMT Charge_QE",1000,0,0.1);

    TH1 *LPMTQEHist_H = new TH1F("Hamamatsu QE value","Hamamatsu QE value",1000,0,1);
    LPMTQEHist_H->SetLineColor(kRed);
    TH1 *LPMTQEHist_N = new TH1F("NNVT QE value","NNVT QE value",1000,0,1);
    LPMTQEHist_N->SetLineColor(kGreen);
    TH1 *LPMTQEHist_HQ = new TH1F("HighQENNVT QE value","HighQENNVT QE value",1000,0,1);
    LPMTQEHist_HQ->SetLineColor(kOrange);

    for (int ID = 0; ID < 17612; ID++)
    {
        LPMTResult->Fill(LPMTMeanQE[ID]->GetMean()/LPMTQE[ID]);
        // LPMTResult->Fill(LPMTMeanQE[ID]->GetMean());

        switch (LPMTType[ID])
        {
        case 1:
            LPMTResult_H->Fill(LPMTMeanQE[ID]->GetMean()/LPMTQE[ID]);
            // LPMTResult_H->Fill(LPMTMeanQE[ID]->GetMean());
            LPMTQEHist_H->Fill(LPMTQE[ID]);
            break;

        case 2:
            LPMTResult_N->Fill(LPMTMeanQE[ID]->GetMean()/LPMTQE[ID]);
            // LPMTResult_N->Fill(LPMTMeanQE[ID]->GetMean());
            LPMTQEHist_N->Fill(LPMTQE[ID]);
            break;
        
        case 3:
            LPMTResult_HQ->Fill(LPMTMeanQE[ID]->GetMean()/LPMTQE[ID]);
            // LPMTResult_HQ->Fill(LPMTMeanQE[ID]->GetMean());
            LPMTQEHist_HQ->Fill(LPMTQE[ID]);
            break;

        default:
            cout<<"Error Wrong Type!!"<<endl;
            break;
        }

    }
    for (int ID = 0; ID < 25600; ID++)
    {
        SPMTResult->Fill(SPMTMeanQE[ID]->GetMean()/SPMTQE[ID]);
        // SPMTResult->Fill(SPMTMeanQE[ID]->GetMean());
    }
    TCanvas *c1 = new TCanvas("c1","c1",1);
    TLegend *legenda =new TLegend(0.2,0.55,0.4,0.85);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.06);
    legenda->SetLineColor(0);
    legenda->AddEntry(LPMTResult,"Total");
    LPMTResult->Draw();
    c1->Update();
    legenda->AddEntry(LPMTResult_N,"NNVT");
    LPMTResult_N->Draw("same");
    c1->Update();
    legenda->AddEntry(LPMTResult_H,"Hamamatsu");
    LPMTResult_H->Draw("same");
    c1->Update();
    legenda->AddEntry(LPMTResult_HQ,"HighQENNVT");
    LPMTResult_HQ->Draw("same");
    c1->Update();
    legenda->Draw();
    c1->Write();
    TCanvas *c2 = new TCanvas("c2","c2",1);
    SPMTResult->Draw();
    c2->Write();

    TCanvas *c3 = new TCanvas("c3","c3",1);
    TLegend *legendb =new TLegend(0.2,0.55,0.4,0.85);
    legendb->SetTextFont(72);
    legendb->SetTextSize(0.06);
    legendb->SetLineColor(0);
    legendb->AddEntry(LPMTQEHist_H,"Hamamatsu");
    LPMTQEHist_H ->Draw();
    legendb->AddEntry(LPMTQEHist_N,"NNVT");
    LPMTQEHist_N ->Draw("same");
    legendb->AddEntry(LPMTQEHist_HQ,"HighQENNVT");
    LPMTQEHist_HQ ->Draw("same");
    legendb->Draw();
    c3->Write();

    LPMTResult->Write();
    LPMTResult_N->Write();
    LPMTResult_H->Write();
    LPMTResult_HQ->Write();

    LPMTQEHist_H ->Write();
    LPMTQEHist_N ->Write();
    LPMTQEHist_HQ ->Write();
    
    outputfile->Close();
}