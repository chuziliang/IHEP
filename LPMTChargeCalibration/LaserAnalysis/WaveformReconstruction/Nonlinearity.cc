//这个脚本用于找到offline中，大小PMT的非线性程度
//分析calib后的root文件
//这是rc0的版本
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

int Nonlinearity(int energy, int filenum)
{
    TString outputname;

    TH1 *LPMTChargeSpectrumSim;
    TString LPMTHistName;
    TString SPMTHistName;
    LPMTHistName = TString::Format("Sim LPMT charge spectrun %1dMeV %1dfile",energy,filenum);
    LPMTChargeSpectrumSim = new TH1F(LPMTHistName,LPMTHistName,300,0,300);
    TH1 *SPMTChargeSpectrumSim[6];
    for (int id = 0; id < 6; id++)
    {
        SPMTHistName = TString::Format("Sim SPMT %1d charge spectrun %1dMeV %1dfile",id,energy,filenum);
        SPMTChargeSpectrumSim[id] = new TH1F(SPMTHistName,SPMTHistName,20,0,20);
    }

    TH1 *LPMTChargeSpectrumCalib;
    LPMTHistName = TString::Format("Calib LPMT charge spectrun %1dMeV %1dfile",energy,filenum);
    LPMTChargeSpectrumCalib = new TH1F(LPMTHistName,LPMTHistName,3000,0,300);
    TH1 *SPMTChargeSpectrumCalib[6];
    for (int id = 0; id < 6; id++)
    {
        SPMTHistName = TString::Format("Calib SPMT %1d charge spectrun %1dMeV %1dfile",id,energy,filenum);
        SPMTChargeSpectrumCalib[id] = new TH1F(SPMTHistName,SPMTHistName,200,0,20);
    }

    TH2 *LPMTCalib2Sim;
    LPMTHistName = TString::Format("Calib to Sim LPMT %1dMeV %1dfile",energy,filenum);
    LPMTCalib2Sim = new TH2F(LPMTHistName,LPMTHistName,3000,0,300,300,0,300);
    TH2 *SPMTCalib2Sim[6];
    for (int id = 0; id < 6; id++)
    {
        SPMTHistName = TString::Format("Calib to Sim SPMT_%1d %1dMeV %1dfile",id,energy,filenum);
        SPMTCalib2Sim[id] = new TH2F(SPMTHistName,SPMTHistName,200,0,20,20,0,20);
    }

    outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy);

    TString filename;
    //for (int filenum = 0; filenum < 50; filenum++)
    {   
        //打开文件
        filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/root/%1dMeV/calib-%1d.root",energy,filenum);
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
        TTree *SimEvt = (TTree*)inputfile->Get("Event/Sim/SimEvt");

        JM::SimEvt *sim_evt = NULL;
        vector<SimPMTHit*> m_cd_hits;

        int m_nhits = 0;
        int pmtid = 0;
        int npe = 0;

        SimEvt->SetBranchAddress("SimEvt",&sim_evt);
        SimEvt->SetBranchAddress("m_nhits",&m_nhits);

        //定义CalibEvevt，及CalibEvent下的数据
        TTree *CalibEventTree = (TTree*)inputfile->Get("Event/Calib/CalibEvent");
        JM::CalibEvent* CalibEvent = NULL;
        CalibEventTree->SetBranchAddress("CalibEvent",&CalibEvent);
        //list<JM::CalibPMTChannel*>* m_CalibPMTCol = NULL;
        //JM::CalibPMTChannel* m_CalibPMTCol = NULL;
        double m_npe;
        
        Int_t nentries = 0;
        if ( (Int_t)SimEvt->GetEntries() == (Int_t)CalibEventTree->GetEntries())
        {
            nentries = (Int_t)SimEvt->GetEntries();
        }
        else
        {
            cerr<<"Events in SimEvt and CalibEvent not equal, check!!!"<<endl;
            return 0;
        }
        
        for (int i = 0; i < nentries; i++)
        {
            int LPMTPE = 0;
            int SPMTPE[6] = {0,0,0,0,0,0};

            int CalibLPMTPE = 0;
            int CalibSPMTPE[6] = {0,0,0,0,0,0};

            SimEvt->GetEntry(i);
            CalibEventTree->GetEntry(i);
            m_cd_hits = sim_evt->getCDHitsVec();

            const std::list<JM::CalibPMTChannel*> recinfo = CalibEvent->calibPMTCol();
            for(auto it : recinfo)
            {
                int pmtid = CdID2pmtid(it->pmtId());
                if(i==0)
                {
                    cout<<pmtid<<endl;
                    //cout<<it->nPE()<<endl;
                }
                if(pmtid == LPMT_ID)
                {
                    CalibLPMTPE = it->nPE();
                }
                for (int id = 0; id < 6; id++)
                {
                    if (pmtid == SPMT_ID[id])
                    {
                        CalibSPMTPE[id] = it->nPE();
                        //cout<<"test here SPMT"<<endl;
                    }
                }
            }

            //if (CalibEvent->getCalibPmtChannel(LPMT_ID))
            // {
            //     CalibLPMTPE = CalibEvent->getCalibPmtChannel(LPMT_ID)->nPE();
            //     //cout<<"test here"<<endl;
            // }
            

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
            LPMTChargeSpectrumSim->Fill(LPMTPE);
            LPMTChargeSpectrumCalib->Fill(CalibLPMTPE);
            LPMTCalib2Sim->Fill(CalibLPMTPE,LPMTPE);
            for (int id = 0; id < 6; id++)
            {
                SPMTChargeSpectrumSim[id]->Fill(SPMTPE[id]);
                SPMTChargeSpectrumCalib[id]->Fill(CalibSPMTPE[id]);
                SPMTCalib2Sim[id]->Fill(CalibSPMTPE[id],SPMTPE[id]);
            }
        }
        
        cout<<"Number "<<filenum<<" finished"<<endl;
        inputfile->Close();
    }


    TFile *outputfile = new TFile(outputname,"UPDATE");
    // outputfile->Delete(TString::Format("LPMT charge spectrun %1dMeV %1dfile;*",energy,filenum));
    LPMTChargeSpectrumSim->Write();
    LPMTChargeSpectrumCalib->Write();
    for (int id = 0; id < 6; id++)
    {
        // outputfile->Delete(TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile;*",id,energy,filenum));
        SPMTChargeSpectrumSim[id]->Write();
        SPMTChargeSpectrumCalib[id]->Write();
    }

    outputfile->Close();
    return 1;
}