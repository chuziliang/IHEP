//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"

using namespace std;
using namespace JM;

const int LPMT_ID = 12618;
const int SPMT_ID[6] = {310668,310836,310838,310965,310968,311082};
// const int SPMTA = 300157;
// const int SPMTB = 300158;
// const int SPMTC = 300163;
// const int SPMTD = 300164;
// const int SPMTE = 303776;
// const int SPMTF = 324355;

int Analysis(int energy, int filenum)
{
    TString outputname;

    TH1 *LPMTChargeSpectrum;
    TString LPMTHistName;
    LPMTHistName = TString::Format("LPMT charge spectrun %1dMeV %1dfile",energy,filenum);
    LPMTChargeSpectrum = new TH1F(LPMTHistName,LPMTHistName,1000,0,1000);
    TH1 *SPMTChargeSpectrum[6];
    for (int id = 0; id < 6; id++)
    {
        TString SPMTHistName;
        SPMTHistName = TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile",id,energy,filenum);
        SPMTChargeSpectrum[id] = new TH1F(SPMTHistName,SPMTHistName,300,0,300);
    }
    
    outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/Check/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy);

    TString filename;
    //for (int filenum = 0; filenum < 50; filenum++)
    {   
        filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/dirac/juno/production/calibration/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc0/Laser/MultiElectron/%1dMeV/detsim/root/detsim-%1d.root",energy,filenum);
        TFile *inputfile = NULL;
        inputfile = TFile::Open(filename);
        if (!inputfile)
        {
            cerr<<"ERROR: root file not exist!"<<endl;
            cerr<<filename<<endl;
            return 0;
        }
        TTree *Evt = (TTree*)inputfile->Get("Event/Sim/SimEvt");

        JM::SimEvt *sim_evt = NULL;
        vector<SimPMTHit*> m_cd_hits;

        int m_nhits = 0;
        int pmtid = 0;
        int npe = 0;

        Evt->SetBranchAddress("SimEvt",&sim_evt);
        Evt->SetBranchAddress("m_nhits",&m_nhits);

        Int_t nentries = (Int_t)Evt->GetEntries();
        for (int i = 0; i < nentries; i++)
        {
            int LPMTPE = 0;
            int SPMTPE[6] = {0,0,0,0,0,0};

            Evt->GetEntry(i);
            m_cd_hits = sim_evt->getCDHitsVec();

            if (m_nhits != m_cd_hits.size())
            {
                cerr<<"ERROR m_nhits != m_cd_hits.size()!"<<endl;
            }

            for (int HitNum = 0; HitNum<m_nhits; HitNum++)
            {
                JM::SimPMTHit* hit_info = m_cd_hits[HitNum];
                pmtid = hit_info->getPMTID();
                npe = hit_info->getNPE();

                if (pmtid == LPMT_ID)
                {   
                    LPMTPE += npe;
                }
                else
                {
                    for (int id = 0; id < 6; id++)
                    {
                        if (pmtid == SPMT_ID[id])
                        {
                            SPMTPE[id] += npe;
                        }
                    }
                }
            }
            LPMTChargeSpectrum->Fill(LPMTPE);
            for (int id = 0; id < 6; id++)
            {
                SPMTChargeSpectrum[id]->Fill(SPMTPE[id]);
            }
        }
        
        cout<<"Number "<<filenum<<" finished"<<endl;
        inputfile->Close();
    }


    TFile *outputfile = new TFile(outputname,"UPDATE");
    outputfile->Delete(TString::Format("LPMT charge spectrun %1dMeV %1dfile;*",energy,filenum));
    LPMTChargeSpectrum->Write();
    for (int id = 0; id < 6; id++)
    {
        outputfile->Delete(TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile;*",id,energy,filenum));
        SPMTChargeSpectrum[id]->Write();
    }

    outputfile->Close();
    return 1;
}