//用于比较Calib之后的电荷数与DetSim的PE数
//用于调试
#include "Context/TimeStamp.h"

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int GetPulseType(int energy, int filenum)
{

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/PulseType/%1dMev_test.root",energy),"RECREATE");

    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    double TrueChargeLPMT;
    double CalibChargeLPMT;
    double Amplitude;
    string PulseType;
    TimeStamp HitTime;
    int nPulseElec;
    int EventNumber;
    int Seconds;
    int Nanosceonds;

    charge->Branch("TrueCharge",&TrueChargeLPMT,"TrueCharge/D");
    // charge->Branch("CalibCharge",&CalibChargeLPMT,"CalibCharge/D");
    charge->Branch("Amplitude",&Amplitude,"Amplitude/D");
    charge->Branch("PulseType",&PulseType);
    charge->Branch("HitTime",&HitTime);
    charge->Branch("nPulseElec",&nPulseElec,"nPulseElec/I");
    charge->Branch("EventNumber",&EventNumber,"EventNumber/I");
    charge->Branch("Seconds",&Seconds,"Seconds/I");
    charge->Branch("Nanosceonds",&Nanosceonds,"Nanosceonds/I");


    TH1 *total = new TH1F("Total","Total",10000,0,100);

    // TH2 *output = new TH2F(TString::Format("Calib charge to true charge %1d",filenum),"Calib charge to true charge",1000,0,100,1000,0,100);

    // for (int filenum = 0; filenum < 1; filenum++)
    {
        TFile* ElecFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/elec/root/%1dMeV/elec-%1d.root",energy,filenum));
        // TFile* CalibFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum));
        // if (!ElecFile || !CalibFile) return 1;
        if (!ElecFile) return 1;

        TTree* ElecTree = (TTree*)ElecFile->Get("Event/CdLpmtTruth/CdLpmtElecTruthEvt");
        // TTree* CalibTree = (TTree*)CalibFile->Get("calibevt");
        
        JM::CdLpmtElecTruthEvt* aEvent = NULL;
        ElecTree->SetBranchAddress("CdLpmtElecTruthEvt",&aEvent);

        // vector<float>* Charge = NULL;
        // vector<int>* PMTID = NULL;
        // CalibTree->SetBranchAddress("Charge",&Charge);
        // CalibTree->SetBranchAddress("SimplePMTID",&PMTID);

        int nEvent = ElecTree->GetEntries();
        // if (nEvent != CalibTree->GetEntries())
        // {
        //     cerr<<"Entries not equal between Elec and Calib!!!"<<endl;
        //     return 1;
        // }


        for(int i = 0; i < nEvent; i++)
        {
            EventNumber = i;
            TrueChargeLPMT = 0;
            Amplitude = 0;

            ElecTree->GetEntry(i);
            const std::vector<JM::LpmtElecTruth>& truth = aEvent->truths();
            int nPulse = truth.size();
            nPulseElec = 0;
            for(int j = 0; j < nPulse; j++)
            {
                if (truth[j].pmtId() < 17612)
                {
                    TrueChargeLPMT = truth[j].npe();
                    Amplitude = truth[j].amplitude();
                    PulseType = truth[j].pulsetype();
                    HitTime = truth[j].pulseHitTime();
                    Seconds = HitTime.GetSec();
                    Nanosceonds = HitTime.GetNanoSec();
                    
                    nPulseElec++;
                    charge->Fill();
                    if (HitTime.GetSeconds()<100)
                    {
                        total->Fill(HitTime.GetSeconds());
                    }
                    
                }
            }
        }

    }
    outputfile->cd();

    charge->Write();

    total->Draw();
    return 0;

}