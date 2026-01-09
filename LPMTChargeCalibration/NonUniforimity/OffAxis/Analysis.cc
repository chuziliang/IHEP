//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"

using namespace std;
using namespace JM;

// const int LPMT_ID = 8947;
// const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324335};
const double R = 19.434; // m
const double position[11] = {0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9};
const double energy[5]={0.5,1.16,2,5,10};
const string sposition[10] = {"0","0.1","0.2","0.3","0.4","0.5","0.6","0.7","0.8","0.9"};
const string senergy[5]={"0.5","1.16","2","5","10"};

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

int Analysis(int filenum, int energynum, const int LPMT_ID)
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
    LPMTHistName = TString::Format("LPMT charge spectrun %.1fMeV %.1fm %.1fDEG",energy[energynum],position[filenum]*R,degree);
    LPMTChargeSpectrum = new TH1F(LPMTHistName,LPMTHistName,1000,0,1000);
    TH1 *SPMTChargeSpectrum[6];
    for (int id = 0; id < 6; id++)
    {
        TString SPMTHistName;
        SPMTHistName = TString::Format("SPMT %1d charge spectrun %.1fMeV %.1fm",id,energy[energynum],position[filenum]*R);
        SPMTChargeSpectrum[id] = new TH1F(SPMTHistName,SPMTHistName,300,0,300);
    }

    outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/ChargeSpectrum/ChargeSpectrum%.1fMeV%.1fDEG.root",energy[energynum],degree);

    TString filename;
    int num = 1;
    if (filenum == 5 & energynum == 4)
    {
        num = 10;
    }

    for (int subnum = 0; subnum < num; subnum++)
    {   
        filename = "/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/root_file/detsim_"+senergy[energynum]+"MeV"+sposition[filenum]+".root";
        if (filenum == 5 & energynum == 4)
        {
            filename = "/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/root_file/detsim_"+senergy[energynum]+"MeV"+sposition[filenum]+"_"+to_string(subnum)+".root";
        }
        
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
    outputfile->Delete(TString::Format("LPMT charge spectrun %.1fMeV %.1fm %.1fDEG",energy[energynum],position[filenum]*R,degree));
    LPMTChargeSpectrum->Write();
    for (int id = 0; id < 6; id++)
    {
        outputfile->Delete(TString::Format("SPMT %1d charge spectrun %.1fMeV %.1fm",id,energy[energynum],position[filenum]*R));
        SPMTChargeSpectrum[id]->Write();
    }

    outputfile->Close();
    return 1;
}