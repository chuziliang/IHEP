// 用于比较SPMT Calib之后的电荷数与DetSim的PE数

using namespace std;
using namespace JM;


int GetTruePE(int energy, int filenum)
{

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/SPMTElectronicsNonlinearity/%1dMevNonlinearity.root",energy),"UPDATE");

    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    double TrueChargeSPMT;
    double CalibChargeSPMT;

    charge->Branch("TrueCharge",&TrueChargeSPMT,"TrueCharge/D");
    charge->Branch("CalibCharge",&CalibChargeSPMT,"CalibCharge/D");

    TH2 *output = new TH2F(TString::Format("Calib charge to true charge %1d",filenum),"Calib charge to true charge",1000,0,100,1000,0,100);

    // for (int filenum = 0; filenum < 1; filenum++)
    {
        TFile* ElecFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/elec/root/%1dMeV/elec-%1d.root",energy,filenum));
        TFile* CalibFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum));
        if (!ElecFile || !CalibFile) return 1;
        
        TTree* ElecTree = (TTree*)ElecFile->Get("Event/CdSpmtTruth/CdSpmtElecTruthEvt");
        TTree* CalibTree = (TTree*)CalibFile->Get("spmtcalibevt");
        
        JM::CdSpmtElecTruthEvt* aEvent = NULL;
        ElecTree->SetBranchAddress("CdSpmtElecTruthEvt",&aEvent);

        vector<float>* Charge = NULL;
        int PMTID;
        int EvtId;
        CalibTree->SetBranchAddress("Charge",&Charge);
        CalibTree->SetBranchAddress("PMTID",&PMTID);
        CalibTree->SetBranchAddress("EvtId",&EvtId);

        int nEvent = ElecTree->GetEntries();
        // if (nEvent != CalibTree->GetEntries())
        // {
        //     cerr<<"Entries not equal between Elec and Calib!!!"<<endl;
        //     return 1;
        // }

        int CalibEntries = 0;
        CalibTree->GetEntry(CalibEntries);
        for(int i = 0; i < nEvent; i++)
        {
            cout<<i<<endl;
            TrueChargeSPMT = 0;
            CalibChargeSPMT = 0;

            double TrueChargeSPMT_onePMT[25600];
            double CalibChargeSPMT_onePMT[25600];
            for (int ID = 0; ID < 25600; ID++)
            {
                TrueChargeSPMT_onePMT[ID] = 0;
                CalibChargeSPMT_onePMT[ID] = 0;
            }
            

            ElecTree->GetEntry(i);
            
            //cout << "EventID: " << i << endl;
            const std::vector<JM::SpmtElecTruth>& truth = aEvent->truths();

            int nPulse = truth.size();
            
            for(int j = 0; j < nPulse; j++)
            {
                TrueChargeSPMT_onePMT[truth[j].pmtId() - 300000] += truth[j].npe();
            }

            
            while (EvtId == i )
            {
                for (int j = 0; j < Charge->size(); j++)
                {
                    CalibChargeSPMT_onePMT[PMTID - 300000] += Charge->at(j);
                }
                CalibEntries++;
                CalibTree->GetEntry(CalibEntries);
            }

            for (int ID = 0; ID < 25600; ID++)
            {
                TrueChargeSPMT = TrueChargeSPMT_onePMT[ID];
                CalibChargeSPMT = CalibChargeSPMT_onePMT[ID];

                charge->Fill();
                output->Fill(TrueChargeSPMT,CalibChargeSPMT);
            }
        }

    }
    outputfile->cd();

    output->Write();
    charge->Write();

    return 0;

}