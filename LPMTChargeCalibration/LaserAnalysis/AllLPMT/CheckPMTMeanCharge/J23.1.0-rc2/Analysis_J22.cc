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
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
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

void Analysis_J22()
{
    double LPMTQE[17612];
    double SPMTQE[25600];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);

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

    for (int FileNum = 0; FileNum < 50; FileNum++)
    {
        TFile *inputfile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/dirac/juno/production/calibration/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/Laser/MultiElectron/5MeV/detsim/root/detsim-%1d.root",FileNum));
        if (!inputfile)
        {
            cerr<<"ERROR: root file not exist!"<<endl;
            return 0;
        }
        inputfile->cd();

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
    }
    
    

    TFile *outputfile = TFile::Open("result_J22.root","recreate");
    outputfile->cd();
    TH1 *LPMTResult = new TH1F("LPMT Charge_QE","LPMT Charge_QE",1000,0,10);
    TH1 *SPMTResult = new TH1F("SPMT Charge_QE","SPMT Charge_QE",1000,0,1);
    for (int ID = 0; ID < 17612; ID++)
    {
        LPMTResult->Fill(LPMTMeanQE[ID]->GetMean()/LPMTQE[ID]);
    }
    for (int ID = 0; ID < 25600; ID++)
    {
        SPMTResult->Fill(SPMTMeanQE[ID]->GetMean()/SPMTQE[ID]);
    }
    TCanvas *c1 = new TCanvas("c1","c1",1);
    LPMTResult->Draw();
    c1->Write();
    TCanvas *c2 = new TCanvas("c2","c2",1);
    SPMTResult->Draw();
    c2->Write();
    

}