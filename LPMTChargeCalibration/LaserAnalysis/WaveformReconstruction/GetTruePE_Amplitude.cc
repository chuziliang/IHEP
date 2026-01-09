//用于比较Calib之后的电荷数与DetSim的PE数
//用于调试

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int GetTruePE_Amplitude(int energy, int filenum)
{

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/Nonlinearity/%1dMevNonlinearity_test.root",energy),"UPDATE");

    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    double TrueChargeLPMT;
    double CalibChargeLPMT;
    double Amplitude;
    // string PulseType;

    charge->Branch("TrueCharge",&TrueChargeLPMT,"TrueCharge/D");
    charge->Branch("CalibCharge",&CalibChargeLPMT,"CalibCharge/D");
    charge->Branch("Amplitude",&Amplitude,"Amplitude/D");
    // charge->Branch("PulseType",&PulseType,"PulseType");

    TH2 *output = new TH2F(TString::Format("Calib charge to true charge %1d",filenum),"Calib charge to true charge",1000,0,100,1000,0,100);

    // for (int filenum = 0; filenum < 1; filenum++)
    {
        TFile* ElecFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/elec/root/%1dMeV/elec-%1d.root",energy,filenum));
        TFile* CalibFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum));
        if (!ElecFile || !CalibFile) return 1;
        
        TTree* ElecTree = (TTree*)ElecFile->Get("Event/Sim/Truth/LpmtElecTruthEvent");
        TTree* CalibTree = (TTree*)CalibFile->Get("calibevt");
        
        JM::LpmtElecTruthEvent* aEvent = NULL;
        ElecTree->SetBranchAddress("LpmtElecTruthEvent",&aEvent);

        vector<float>* Charge = NULL;
        vector<int>* PMTID = NULL;
        CalibTree->SetBranchAddress("Charge",&Charge);
        CalibTree->SetBranchAddress("SimplePMTID",&PMTID);

        int nEvent = ElecTree->GetEntries();
        if (nEvent != CalibTree->GetEntries())
        {
            cerr<<"Entries not equal between Elec and Calib!!!"<<endl;
            return 1;
        }


        for(int i = 0; i < nEvent; i++)
        {
            TrueChargeLPMT = 0;
            CalibChargeLPMT = 0;
            Amplitude = 0;

            ElecTree->GetEntry(i);
            CalibTree->GetEntry(i);
            //cout << "EventID: " << i << endl;
            const std::vector<JM::LpmtElecTruth>& truth = aEvent->truths();

            int nPulse = truth.size();
            // if (nPulse !=  Charge->size())
            // {
            //     cout<<"nPulse not equal between Elec and Calib!!!"<<endl;
            // }

            int nPulseElec = 0;
            int nPulseCaolb = 0;
            for(int j = 0; j < nPulse; j++)
            {
                // if (truth[j].pmtId() != PMTID->at(j))
                // {
                //     cout<<"pmtid not equal between Elec and Calib!!!"<<endl;
                // }
                if (truth[j].pmtId() == LPMT_ID)
                {
                    TrueChargeLPMT += truth[j].npe();
                    Amplitude += truth[j].amplitude();
                    nPulseElec++;
                    // PulseType = truth[j].pulsetype();
                    // CalibChargeLPMT = Charge->at(j);
                    
                }
            }

            for (int j = 0; j < Charge->size(); j++)
            {
                if (PMTID->at(j) == LPMT_ID)
                {
                    CalibChargeLPMT += Charge->at(j);
                    nPulseCaolb++;
                }
            }
            cout<<"Elec - caolb ="<<nPulseElec-nPulseCaolb<<endl;
            charge->Fill();
            output->Fill(TrueChargeLPMT,CalibChargeLPMT);
        }

    }
    outputfile->cd();

    output->Write();
    charge->Write();

    return 0;

}