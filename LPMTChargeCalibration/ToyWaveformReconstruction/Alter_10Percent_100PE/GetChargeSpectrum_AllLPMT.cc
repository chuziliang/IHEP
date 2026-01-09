//分析calib后的root文件
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>
// #include "Identifier/CdID.h"
// #include "Identifier/JunoDetectorID.h"
// #include "Identifier/Identifier.h"

using namespace std;
using namespace JM;

// const int LPMT_ID = 8947;
// const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int CdID2pmtid(unsigned int cdid)
{
    int pmtid = (cdid-(0x10<<24))>>8;
    return pmtid;
}

int GetChargeSpectrum_AllLPMT(int energy, int filenum)
{
    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Alter_10Percent_100PE/ChargeSpectrumAllLPMT/%1dMevCharge.root",energy),"UPDATE");
    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    vector<double> CalibChargeLPMT;
    vector<double> TrueChargeLPMT;
    // vector<double> PMTID;
    double TotalPELPMT;
    charge->Branch("LPMTCharge",&CalibChargeLPMT);
    charge->Branch("TrueChargeLPMT",&TrueChargeLPMT);
    // charge->Branch("PMTID",&PMTID);
    charge->Branch("TotalPELPMT",&TotalPELPMT,"TotalPELPMT/D");


    TString filename;
    filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/ToyWaveRec/Alter_10PercentAt100PE/root/%1dMeV/calib-%1d.root",energy,filenum);
    //打开文件
    TFile *inputfile = NULL;
    inputfile = TFile::Open(filename);
    if (!inputfile)
    {
        cerr<<"ERROR: root file not exist!"<<endl;
        cerr<<filename<<endl;
        return 0;
    }

    //定义CalibEvevt，及CalibEvent下的数据
    TTree *CalibEventTree = (TTree*)inputfile->Get("Event/CdLpmtCalib/CdLpmtCalibEvt");
    CdLpmtCalibEvt* CdLpmtCalibEvt_a = new CdLpmtCalibEvt();
    CalibEventTree->SetBranchAddress("CdLpmtCalibEvt",&CdLpmtCalibEvt_a);

    //定义SimEvt，及SimEvt下的数据
    TTree *SimEvtTree = (TTree*)inputfile->Get("Event/Sim/SimEvt");
    SimEvt* SimEvt_a = new SimEvt();
    SimEvtTree->SetBranchAddress("SimEvt",&SimEvt_a);

    
    int nentries = CalibEventTree->GetEntries();
    for (int i = 0; i < nentries; i++)
    {
        TrueChargeLPMT = vector<double>(17612,0);
        CalibChargeLPMT = vector<double>(17612,0);
        TotalPELPMT = 0;
        
        CalibEventTree->GetEntry(i);
        SimEvtTree->GetEntry(i);
        auto CDHits = SimEvt_a->getCDHitsVec();
        for (int i = 0; i < CDHits.size(); i++)
        {
            int PMTID = CDHits[i]->getPMTID();
            
            if (PMTID < 17612)
            {
                TotalPELPMT += CDHits[i]->getNPE();
                TrueChargeLPMT[PMTID] += CDHits[i]->getNPE();
            }
            
        }
        

        auto CalibPMTCol = CdLpmtCalibEvt_a->calibPMTCol();
        float FirstFirstHitTime;
        for (auto it = CalibPMTCol.begin(); it != CalibPMTCol.end(); ++it) 
        {
            int PMTID = (*it)->pmtId();
            if (it == CalibPMTCol.begin())
            {
                FirstFirstHitTime = (*it)->firstHitTime();
            }
            if ((*it)->firstHitTime() < FirstFirstHitTime)
            {
                FirstFirstHitTime = (*it)->firstHitTime();
            }
            
            if (PMTID < 17612)
            {
                CalibChargeLPMT[PMTID] += (*it)->nPE();
            }
            
        }
        if (FirstFirstHitTime < 1000)
        {
            charge->Fill();
        }
        TrueChargeLPMT.clear();
        CalibChargeLPMT.clear();
        cout<<i<<" Finished"<<endl;
    }
    outputfile->cd();
    charge->Write();
    return 1;
}