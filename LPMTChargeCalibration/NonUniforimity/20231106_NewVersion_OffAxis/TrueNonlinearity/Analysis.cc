//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"

using namespace std;
using namespace JM;

// const int LPMT_ID = 8947;
// const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int GetCorrespondingSPMT(int (&SPMT_ID)[6], const int LPMT_ID)
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map.txt");
    string line;
    int currentLine = 0;

    while (getline(file, line)) { 
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            int firstData;
            ss >> firstData >> SPMT_ID[0] >> SPMT_ID[1] >> SPMT_ID[2] >> SPMT_ID[3] >> SPMT_ID[4] >> SPMT_ID[5];
            return firstData;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

double GetDegree(const int LPMT_ID)
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;
    int currentLine = 0;

    while (getline(file, line)) { 
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            double Degree;
            double nun;
            ss >> nun >> nun >> nun >> nun >> nun >> Degree;
            Degree -= 83.6712;
            return Degree;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

int Analysis(int energy, int filenum, const int LPMT_ID)
{
    int SPMT_ID[6];
    if (GetCorrespondingSPMT(SPMT_ID,LPMT_ID) != LPMT_ID)
    {
        cerr<<"ERROR: Wrong PMT ID!"<<endl;
        return 0;
    }
    double degree;
    degree = GetDegree(LPMT_ID);

    TString outputname;

    TH1 *LPMTChargeSpectrum;
    TString LPMTHistName;
    LPMTHistName = TString::Format("LPMT charge spectrun %1dMeV %1dfile %.1fDEG",energy,filenum,degree);
    LPMTChargeSpectrum = new TH1F(LPMTHistName,LPMTHistName,1000,0,1000);
    TH1 *SPMTChargeSpectrum[6];
    for (int id = 0; id < 6; id++)
    {
        TString SPMTHistName;
        SPMTHistName = TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile %.1fDEG",id,energy,filenum,degree);
        SPMTChargeSpectrum[id] = new TH1F(SPMTHistName,SPMTHistName,300,0,300);
    }
    
    outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231106_NewVersion_OffAxis/TrueNonlinearity/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy);

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
    outputfile->Delete(TString::Format("LPMT charge spectrun %1dMeV %1dfile %.1fDEG;*",energy,filenum,degree));
    LPMTChargeSpectrum->Write();
    for (int id = 0; id < 6; id++)
    {
        outputfile->Delete(TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile %.1fDEG;*",id,energy,filenum,degree));
        SPMTChargeSpectrum[id]->Write();
    }

    outputfile->Close();
    return 1;
}