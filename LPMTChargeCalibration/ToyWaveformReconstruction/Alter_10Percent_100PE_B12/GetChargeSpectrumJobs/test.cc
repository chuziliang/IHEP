using namespace JM;

int test(const int FileID)
{ 
    // TString outputfilename = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE_B12/ChargeSpectrum/ChargeSpectrum%1d.root",FileID);
    // TFile* outputfile = TFile::Open(outputfilename,"recreate");
    // TTree* charge = new TTree("charge","DetSim charge and calib charge");
    // vector<double> CalibChargeLPMT;
    // vector<double> TrueChargeLPMT;
    // vector<double> ChargeSPMT;
    // double edep_x, edep_y, edep_z;

    // charge->Branch("LPMTCharge",&CalibChargeLPMT);
    // charge->Branch("TrueLPMTCharge",&TrueChargeLPMT);
    // charge->Branch("SPMTCharge",&ChargeSPMT);
    // charge->Branch("edep_x",&edep_x);
    // charge->Branch("edep_y",&edep_y);
    // charge->Branch("edep_z",&edep_z);

    TString CalibFileName = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/B12/calib_ToyWaveRec/root/cailb-%1d.root",FileID);
    TString ElecFileName  = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/B12/elec/user/elec-%1d.root",FileID);
    TFile *inputfile_calib = TFile::Open(CalibFileName,"read");
    TFile *inputfile_elec  = TFile::Open(ElecFileName,"read");

    //定义CalibFile下的CalibEvevt，及CalibEvent下的数据，得到CalibChargeLPMT
    TTree *CalibEventTree = (TTree*)inputfile_calib->Get("Event/CdLpmtCalib/CdLpmtCalibEvt");
    CdLpmtCalibEvt* CdLpmtCalibEvt_a = new CdLpmtCalibEvt();
    CalibEventTree->SetBranchAddress("CdLpmtCalibEvt",&CdLpmtCalibEvt_a);

    //定义CalibFile下的SimEvt，及SimEvt下的数据，得到TrueChargeLPMT，和edep position
    TTree *SimEvtTree = (TTree*)inputfile_calib->Get("Event/Sim/SimEvt");
    SimEvt* SimEvt_a = new SimEvt();
    SimEvtTree->SetBranchAddress("SimEvt",&SimEvt_a);

    // 定义ElecFile下的spmt，得到SPMT电子学后的charge
    TTree *SPMTTree = (TTree*)inputfile_elec->Get("SPMT");
    int SPMTTreeEvtID;
    int SPMTTreeSPMTID;
    double SPMTTreeCharge;
    SPMTTree->SetBranchAddress("evtID",&SPMTTreeEvtID);
    SPMTTree->SetBranchAddress("SPMT_Id",&SPMTTreeSPMTID);
    SPMTTree->SetBranchAddress("Charge",&SPMTTreeCharge);

    // 定义ElecFile下的eventindex，用于确认EventID
    TTree *eventindex = (TTree*)inputfile_elec->Get("eventindex");
    vector<int>* entries = nullptr;
    eventindex->SetBranchAddress("entries",&entries);

    int CalibID = 0;
    int ElecID = 0;
    int SPMTentry = 0;

    int testnum = 0;

    float FirstFirstHitTime = 0;
    for (int i =0; i<CalibEventTree->GetEntries(); i++)
    {
        CalibEventTree->GetEntry(CalibID);
        SimEvtTree->GetEntry(CalibID);
        CalibID++;
        auto CalibPMTCol = CdLpmtCalibEvt_a->calibPMTCol();
        
        for (auto it = CalibPMTCol.begin(); it != CalibPMTCol.end(); ++it) 
        {
            if (it == CalibPMTCol.begin())
            {
                FirstFirstHitTime = (*it)->firstHitTime();
            }
            if ((*it)->firstHitTime() < FirstFirstHitTime)
            {
                FirstFirstHitTime = (*it)->firstHitTime();
            }
        }
        if (FirstFirstHitTime < 1000)
        {
            testnum++;
            cout<<testnum<<endl;
        }
    }


    // for (int DetSimID = 0; DetSimID < 10000; DetSimID++)
    // {
    //     TrueChargeLPMT = vector<double>(17612,0);
    //     CalibChargeLPMT = vector<double>(17612,0);
    //     ChargeSPMT = vector<double>(25600,0);

    //     // CalibFile下的Tree拿到DitSimID的事例
    //     float FirstFirstHitTime = 0;
    //     do 
    //     {
    //         cout<<"Calib cycle"<<endl;
    //         CalibEventTree->GetEntry(CalibID);
    //         SimEvtTree->GetEntry(CalibID);
    //         CalibID++;
    //         auto CalibPMTCol = CdLpmtCalibEvt_a->calibPMTCol();
            
    //         for (auto it = CalibPMTCol.begin(); it != CalibPMTCol.end(); ++it) 
    //         {
    //             if (it == CalibPMTCol.begin())
    //             {
    //                 FirstFirstHitTime = (*it)->firstHitTime();
    //             }
    //             if ((*it)->firstHitTime() < FirstFirstHitTime)
    //             {
    //                 FirstFirstHitTime = (*it)->firstHitTime();
    //             }
    //         }
    //     } while (FirstFirstHitTime > 1000);

    //     // ElecFile下的Tree拿到DitSimID的事例
    //     do
    //     {
    //         cout<<"Elec cycle"<<endl;
    //         eventindex->GetEntry(ElecID);
    //         ElecID++;
    //     } while ((*entries)[0] < DetSimID);

    //     if ((*entries)[0] > DetSimID) 
    //     {
    //         ElecID--;
    //         continue;
    //     }
    //     // 拿到这个事例的Edep position
    //     auto m_tracks  = SimEvt_a->getTracksVec();
    //     if (m_tracks.size() > 1)
    //     {
    //         cout<<CalibID-1<<" More than 1 tracks!!"<<endl;
    //     }
    //     JM::SimTrack* track_info = m_tracks[0];
    //     edep_x = track_info->getEdepX();
    //     edep_y = track_info->getEdepY();
    //     edep_z = track_info->getEdepZ();

    //     // 拿到这个事例的TrueChargeLPMT
    //     auto CDHits = SimEvt_a->getCDHitsVec();
    //     for (int i = 0; i < CDHits.size(); i++)
    //     {
    //         int PMTID = CDHits[i]->getPMTID();
            
    //         if (PMTID < 17612)
    //         {
    //             TrueChargeLPMT[PMTID] += CDHits[i]->getNPE();
    //         }
            
    //     }

    //     // 拿到这个事例的CalibChargeLPMT
    //     auto CalibPMTCol = CdLpmtCalibEvt_a->calibPMTCol();
    //     for (auto it = CalibPMTCol.begin(); it != CalibPMTCol.end(); ++it) 
    //     {
    //         int PMTID = (*it)->pmtId();
    //         if (PMTID < 17612)
    //         {
    //             CalibChargeLPMT[PMTID] += (*it)->nPE();
    //         }
            
    //     }

    //     // 拿到这个事例的SPMTCharge
    //     cout<<"skip SPMTentry"<<endl;
    //     do
    //     {
    //         SPMTTree->GetEntry(SPMTentry);
    //         SPMTentry++;
    //     } while (SPMTTreeEvtID != (ElecID-1));
    //     SPMTentry--; 
    //     // 上面的循环去掉了SPMTTreeEvtID不是(ElecID-1)的entry
    //     cout<<"read SPMTentry"<<endl;
    //     while (SPMTTreeEvtID == (ElecID-1) )
    //     {
    //         if (SPMTentry >= SPMTTree->GetEntries())
    //         {
    //             break;
    //         }
            
    //         SPMTTree->GetEntry(SPMTentry);
    //         ChargeSPMT[SPMTTreeSPMTID - 300000] += SPMTTreeCharge;
    //         SPMTentry++;
    //     }

    //     charge->Fill();

    //     TrueChargeLPMT.clear();
    //     CalibChargeLPMT.clear();
    //     ChargeSPMT.clear();
    //     cout<<"CalibID = "<<CalibID-1<<"  ElecID = "<<ElecID-1<<"  SPMTentry = "<<SPMTentry-1<<endl;
    //     cout<<DetSimID<<" Finished"<<endl;
    //     cout<<"---------------------------------------"<<endl;
    // }
    // outputfile->cd();
    // charge->Write();
    return 1;
}