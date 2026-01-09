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

const int LPMT_ID = 8947;
// const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int CdID2pmtid(unsigned int cdid)
{
    int pmtid = (cdid-(0x10<<24))>>8;
    return pmtid;
}

int GetChargeSpectrum(int energy, int filenum)
{
    // TString outputname;
    TString LPMTHistName;
    // TString SPMTHistName;

    TH1 *LPMTChargeSpectrumCalib;
    LPMTHistName = TString::Format("Calib LPMT charge spectrun PMTID %1d",LPMT_ID);
    LPMTChargeSpectrumCalib = new TH1F(LPMTHistName,LPMTHistName,3000,0,300);

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/10Percent/DetectorCentor/ChargeSpectrum/%1dMevCharge.root",energy),"UPDATE");
    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    double CalibChargeLPMT;
    charge->Branch("LPMTCharge",&CalibChargeLPMT,"CalibCharge/D");

    TString filename;
    filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/ToyWaveRec/10Percent/root/%1dMeV/calib-%1d.root",energy,filenum);
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
    
    int nentries = CalibEventTree->GetEntries();
    for (int i = 0; i < nentries; i++)
    {
        CalibChargeLPMT = 0;
        CalibEventTree->GetEntry(i);
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
            
            if ((PMTID == LPMT_ID))
            {
                CalibChargeLPMT += (*it)->nPE();
            }
        }
        if (FirstFirstHitTime < 1000)
        {
            charge->Fill();
        }
        cout<<i<<" Finished"<<endl;
    }
    outputfile->cd();
    charge->Write();
    return 1;
}