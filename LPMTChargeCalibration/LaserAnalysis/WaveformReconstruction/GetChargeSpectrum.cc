//分析calib后的user-root文件
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"
#include "Event/CalibEvent.h"

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int CdID2pmtid(unsigned int cdid)
{
    int pmtid = (cdid-(0x10<<24))>>8;
    return pmtid;
}

int GetChargeSpectrum(int energy, int filenum)
{
    TString outputname;
    TString LPMTHistName;
    TString SPMTHistName;

    TH1 *LPMTChargeSpectrumCalib;
    LPMTHistName = TString::Format("Calib LPMT charge spectrun %1dMeV %1dfile",energy,filenum);
    LPMTChargeSpectrumCalib = new TH1F(LPMTHistName,LPMTHistName,3000,0,300);
    TH1 *SPMTChargeSpectrumCalib[6];
    for (int id = 0; id < 6; id++)
    {
        SPMTHistName = TString::Format("Calib SPMT %1d charge spectrun %1dMeV %1dfile",id,energy,filenum);
        SPMTChargeSpectrumCalib[id] = new TH1F(SPMTHistName,SPMTHistName,200,0,20);
    }

    outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy);

    TString filename;
    //for (int filenum = 0; filenum < 50; filenum++)
    {   
        //打开文件
        filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum);
        //filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/dirac/juno/production/calibration/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/Laser/MultiElectron/%1dMeV/detsim/root/detsim-%1d.root",energy,filenum);
        TFile *inputfile = NULL;
        inputfile = TFile::Open(filename);
        if (!inputfile)
        {
            cerr<<"ERROR: root file not exist!"<<endl;
            cerr<<filename<<endl;
            return 0;
        }

        //定义SimEvt，及SimEvt下的数据
        // TTree *SimEvt = (TTree*)inputfile->Get("Event/Sim/SimEvt");

        // JM::SimEvt *sim_evt = NULL;
        // vector<SimPMTHit*> m_cd_hits;

        // int m_nhits = 0;
        // int pmtid = 0;
        // int npe = 0;

        // SimEvt->SetBranchAddress("SimEvt",&sim_evt);
        // SimEvt->SetBranchAddress("m_nhits",&m_nhits);

        //定义CalibEvevt，及CalibEvent下的数据
        TTree *CalibEventTree = (TTree*)inputfile->Get("calibevt");
        vector<float>* Charge = NULL;
        vector<int>* PMTID = NULL;
        CalibEventTree->SetBranchAddress("Charge",&Charge);
        CalibEventTree->SetBranchAddress("SimplePMTID",&PMTID);
        //list<JM::CalibPMTChannel*>* m_CalibPMTCol = NULL;
        //JM::CalibPMTChannel* m_CalibPMTCol = NULL;
        
        Int_t nentries = (Int_t)CalibEventTree->GetEntries();
        // if ( (Int_t)SimEvt->GetEntries() == (Int_t)CalibEventTree->GetEntries())
        // {
        //     nentries = (Int_t)SimEvt->GetEntries();
        // }
        // else
        // {
        //     cerr<<"Events in SimEvt and CalibEvent not equal, check!!!"<<endl;
        //     return 0;
        // }

        
        for (int i = 0; i < nentries; i++)
        {
            float LPMTPE = 0;
            CalibEventTree->GetEntry(i);
            int testnum = 0;
            for (int j = 0; j < PMTID->size(); j++)
            {
                if (PMTID->at(j) == LPMT_ID)
                {
                    LPMTPE = Charge->at(j);
                    LPMTChargeSpectrumCalib->Fill(LPMTPE);
                    testnum++;
                    cout<<testnum<<"    "<<LPMTPE<<endl;
                }
            }
            
        cout<<endl;        
        }
        cout<<nentries<<endl;
        LPMTChargeSpectrumCalib->Draw();
        cout<<"Number "<<filenum<<" finished"<<endl;
        inputfile->Close();
    }


    // TFile *outputfile = new TFile(outputname,"UPDATE");
    // // outputfile->Delete(TString::Format("LPMT charge spectrun %1dMeV %1dfile;*",energy,filenum));
    // LPMTChargeSpectrumSim->Write();
    // LPMTChargeSpectrumCalib->Write();
    // for (int id = 0; id < 6; id++)
    // {
    //     // outputfile->Delete(TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile;*",id,energy,filenum));
    //     SPMTChargeSpectrumSim[id]->Write();
    //     SPMTChargeSpectrumCalib[id]->Write();
    // }

    // outputfile->Close();
    return 1;
}