using namespace JM;

int GetChargeSpectrumDetSim(const int FileID)
{ 
    TString outputfilename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/DetSimChargeSpectrum/ChargeSpectrum%1d.root",FileID);
    TFile* outputfile = TFile::Open(outputfilename,"recreate");
    TTree* charge = new TTree("charge","DetSim charge and calib charge");
    vector<double> TrueChargeLPMT;
    vector<double> TrueChargeSPMT;
    double edep_x, edep_y, edep_z;
    double edep;

    charge->Branch("TrueLPMTCharge",&TrueChargeLPMT);
    charge->Branch("TrueSPMTCharge",&TrueChargeSPMT);
    charge->Branch("edep_x",&edep_x);
    charge->Branch("edep_y",&edep_y);
    charge->Branch("edep_z",&edep_z);
    charge->Branch("edep",&edep);

    TString FileName = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/B12/root/detsim_%1d.root",FileID);
    TFile *inputfile = TFile::Open(FileName,"read");

    //定义SimEvt，及SimEvt下的数据，得到TrueChargeLPMT，TrueChargeSPMT和edep position
    TTree *SimEvtTree = (TTree*)inputfile->Get("Event/Sim/SimEvt");
    SimEvt* SimEvt_a = new SimEvt();
    SimEvtTree->SetBranchAddress("SimEvt",&SimEvt_a);

    for (int DetSimID = 0; DetSimID < SimEvtTree->GetEntries(); DetSimID++)
    {
        TrueChargeLPMT = vector<double>(17612,0);
        TrueChargeSPMT = vector<double>(25600,0);
        SimEvtTree->GetEntry(DetSimID);
        
        // 拿到这个事例的Edep position
        auto m_tracks  = SimEvt_a->getTracksVec();
        if (m_tracks.size() > 1)
        {
            cout<<DetSimID<<" More than 1 tracks!!"<<endl;
        }
        JM::SimTrack* track_info = m_tracks[0];
        edep_x = track_info->getEdepX();
        edep_y = track_info->getEdepY();
        edep_z = track_info->getEdepZ();
        edep = track_info->getEdep();

        // 拿到这个事例的TrueChargeLPMT
        auto CDHits = SimEvt_a->getCDHitsVec();
        for (int i = 0; i < CDHits.size(); i++)
        {
            int PMTID = CDHits[i]->getPMTID();
            
            if (PMTID < 17612)
            {
                TrueChargeLPMT[PMTID] += CDHits[i]->getNPE();
            }

            if (PMTID >= 300000)
            {
                TrueChargeSPMT[PMTID - 300000] += CDHits[i]->getNPE();
            }
            
        }

        charge->Fill();

        TrueChargeLPMT.clear();
        TrueChargeSPMT.clear();
        cout<<DetSimID<<" Finished"<<endl;
    }
    outputfile->cd();
    charge->Write();
    return 1;
}