using namespace JM;

const int ChosenLPMTID = 8947;

int GetCorrespondingSPMT(int (&CorrespondingSPMT)[17612][6])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map_J23.1.0-rc2.txt");
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

int GetChargeSpectrumDetSim_NoBar(const int FileID, const double Position_ID)
{ 
    int CorrespondingSPMT[17612][6];
    GetCorrespondingSPMT(CorrespondingSPMT);
    TString FileName = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/REffect/NoBar/%.1f/root/detsim_%1d.root",Position_ID,FileID);
    TFile *inputfile = TFile::Open(FileName,"read");
    if (!inputfile) return 0;
    
    TString outputfilename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/REffect/ChargeSpectrum/%.1f/ChargeSpectrum%1d.root",Position_ID,FileID);
    TFile* outputfile = TFile::Open(outputfilename,"recreate");
    TTree* charge = new TTree("charge","DetSim charge and calib charge");
    double TrueChargeLPMT;
    vector<double> TrueChargeSPMT;
    double edep_x, edep_y, edep_z;
    double edep;

    charge->Branch("TrueLPMTCharge",&TrueChargeLPMT);
    charge->Branch("TrueSPMTCharge",&TrueChargeSPMT);
    charge->Branch("edep_x",&edep_x);
    charge->Branch("edep_y",&edep_y);
    charge->Branch("edep_z",&edep_z);
    charge->Branch("edep",&edep);

    inputfile->cd();
    
    //定义SimEvt，及SimEvt下的数据，得到TrueChargeLPMT，TrueChargeSPMT和edep position
    TTree *SimEvtTree = (TTree*)inputfile->Get("Event/Sim/SimEvt");
    SimEvt* SimEvt_a = new SimEvt();
    SimEvtTree->SetBranchAddress("SimEvt",&SimEvt_a);

    for (int DetSimID = 0; DetSimID < SimEvtTree->GetEntries(); DetSimID++)
    {
        TrueChargeLPMT = 0;
        TrueChargeSPMT = vector<double>(6,0);
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
            
            if (PMTID == ChosenLPMTID)
            {
                TrueChargeLPMT += CDHits[i]->getNPE();
            }

            for (int CorrespondingID = 0; CorrespondingID < 6; CorrespondingID++)
            {
                if (PMTID == CorrespondingSPMT[ChosenLPMTID][CorrespondingID])
                {
                    TrueChargeSPMT[CorrespondingID] += CDHits[i]->getNPE();
                }
            }
            
            
        }

        charge->Fill();

        TrueChargeLPMT = 0;
        TrueChargeSPMT.clear();
        cout<<DetSimID<<" Finished"<<endl;
    }
    outputfile->cd();
    charge->Write();
    return 1;
}