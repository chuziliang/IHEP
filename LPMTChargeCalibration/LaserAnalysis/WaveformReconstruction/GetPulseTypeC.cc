//用于比较Calib之后的电荷数与DetSim的PE数
//用于调试
#include "Context/TimeStamp.h"

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int GetPulseTypeC(int energy, int filenum)
{

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/PulseType/%1dMev_test.root",energy),"UPDATE");

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
    



    TTree* event = new TTree(TString::Format("Event%1d",filenum),"elecsim event");
    double IBDPercent;
    double DarkPulsePercent;
    double AfterPulsePercent;
    double TotalIBDPercent;
    double TotalDarkPulsePercent;
    double TotalAfterPulsePercent;
    double Charge;
    int StartSecond;
    int StartNanosecond;
    double TotalCharge;
    
    event->Branch("EventNumber",&EventNumber,"EventNumber/I");
    event->Branch("IBDPercent",&IBDPercent,"IBDPercent/D");
    event->Branch("DarkPulsePercent",&DarkPulsePercent,"DarkPulsePercent/D");
    event->Branch("AfterPulsePercent",&AfterPulsePercent,"AfterPulsePercent/D");
    event->Branch("TotalIBDPercent",&TotalIBDPercent,"TotalIBDPercent/D");
    event->Branch("TotalDarkPulsePercent",&TotalDarkPulsePercent,"TotalDarkPulsePercent/D");
    event->Branch("TotalAfterPulsePercent",&TotalAfterPulsePercent,"TotalAfterPulsePercent/D");
    event->Branch("Charge",&Charge,"Charge/D");
    event->Branch("StartSecond",&StartSecond,"StartSecond/I");
    event->Branch("StartNanosecond",&StartNanosecond,"StartNanosecond/I");
    event->Branch("TotalCharge",&TotalCharge,"TotalCharge/D");

    // for (int filenum = 0; filenum < 1; filenum++)
    {
        TFile* ElecFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/elec/root/%1dMeV/elec-%1d.root",energy,filenum));
        if (!ElecFile) return 1;

        TTree* ElecTree = (TTree*)ElecFile->Get("Event/CdLpmtTruth/CdLpmtElecTruthEvt");

        JM::CdLpmtElecTruthEvt* aEvent = NULL;
        ElecTree->SetBranchAddress("CdLpmtElecTruthEvt",&aEvent);

        int nEvent = ElecTree->GetEntries();
        
        for(int i = 0; i < nEvent; i++)
        {
            EventNumber = i;
            TrueChargeLPMT = 0;
            CalibChargeLPMT = 0;
            Amplitude = 0;
            TotalCharge = 0;

            ElecTree->GetEntry(i);
            const std::vector<JM::LpmtElecTruth>& truth = aEvent->truths();

            int nPulse = truth.size();
            
            nPulseElec = 0;
            int nIBDPulse = 0;
            int nDarkPulse = 0;
            int nAfterPulse = 0;
            int TotalnIBDPulse = 0;
            int TotalnDarkPulse = 0;
            int TotalnAfterPulse = 0;
            Charge = 0;
            for(int j = 0; j < nPulse; j++)
            {
                TotalCharge += truth[j].npe();
                if (PulseType == "IBD")
                {
                    TotalnIBDPulse++;
                }
                else if (PulseType == "DarkPulse")
                {
                    TotalnDarkPulse++;
                }
                else if (PulseType == "AfterPulse")
                {
                    TotalnAfterPulse++;
                }
                
                if (truth[j].pmtId() == LPMT_ID)
                {
                    TrueChargeLPMT = truth[j].npe();
                    Amplitude = truth[j].amplitude();
                    PulseType = truth[j].pulsetype();
                    HitTime = truth[i].pulseHitTime();
                    Seconds = HitTime.GetSec();
                    Nanosceonds = HitTime.GetNanoSec();

                    Charge += TrueChargeLPMT;
                    if (PulseType == "IBD")
                    {
                        nIBDPulse++;
                    }
                    else if (PulseType == "DarkPulse")
                    {
                        nDarkPulse++;
                    }
                    else if (PulseType == "AfterPulse")
                    {
                        nAfterPulse++;
                    }
                    nPulseElec++;
                    charge->Fill();
                }
            }
            IBDPercent = static_cast<double>(nIBDPulse)/static_cast<double>(nPulseElec);
            DarkPulsePercent = static_cast<double>(nDarkPulse)/static_cast<double>(nPulseElec);
            AfterPulsePercent = static_cast<double>(nAfterPulse)/static_cast<double>(nPulseElec);
            TotalIBDPercent = static_cast<double>(TotalnIBDPulse)/static_cast<double>(nPulse);
            TotalDarkPulsePercent = static_cast<double>(TotalnDarkPulse)/static_cast<double>(nPulse);
            TotalAfterPulsePercent = static_cast<double>(TotalnAfterPulse)/static_cast<double>(nPulse);
            StartSecond = truth[0].pulseHitTime().GetSec();
            StartNanosecond = truth[0].pulseHitTime().GetNanoSec();

            event->Fill();

        }

    }
    outputfile->cd();

    charge->Write();
    event->Write();

    return 0;

}