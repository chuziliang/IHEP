//分析EOS中激光刻度的数据，找出波形重建级别下，LPMT与Corresponding SPMT的nPE分布。
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};
// const int SPMTA = 300157;
// const int SPMTB = 300158;
// const int SPMTC = 300163;
// const int SPMTD = 300164;
// const int SPMTE = 303776;
// const int SPMTF = 324355;

int Analysis(int energy, int filenum)
{
    TString outputname;

    // TH1 *LPMTChargeSpectrum;
    // TString LPMTHistName;
    // LPMTHistName = TString::Format("LPMT charge spectrun %1dMeV %1dfile",energy,filenum);
    // LPMTChargeSpectrum = new TH1F(LPMTHistName,LPMTHistName,3000,0,300);
    // TH1 *SPMTChargeSpectrum[6];
    // for (int id = 0; id < 6; id++)
    // {
    //     TString SPMTHistName;
    //     SPMTHistName = TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile",id,energy,filenum);
    //     SPMTChargeSpectrum[id] = new TH1F(SPMTHistName,SPMTHistName,200,0,20);
    // }
    
    outputname = TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy);

    TTree *charge = new TTree(TString::Format("Charge%1d",filenum),"LPMT and SPMT calib charge");
    
    double LPMTPE = 0;
    double SPMTPE[6] = {0,0,0,0,0,0};

    charge->Branch("LPMTCharge",&LPMTPE,"LPMTCharge/D");
    charge->Branch("SPMTCharge",SPMTPE,"SPMTCharge[6]/D");


    TString filename;
    //for (int filenum = 0; filenum < 50; filenum++)
    {   
        filename = TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum);
        TFile *inputfile = NULL;
        inputfile = TFile::Open(filename);
        if (!inputfile)
        {
            cerr<<"ERROR: root file not exist!"<<endl;
            cerr<<filename<<endl;
            return 0;
        }
        TTree* CalibTree = (TTree*)inputfile->Get("calibevt");

        vector<float>* Charge = NULL;
        vector<int>* PMTID = NULL;
        CalibTree->SetBranchAddress("Charge",&Charge);
        CalibTree->SetBranchAddress("SimplePMTID",&PMTID);

        TTree* SPMTTree = (TTree*)inputfile->Get("spmtcalibevt");

        int EvtID;
        int SPMTID;
        float TotalPE;
        SPMTTree->SetBranchAddress("EvtId",&EvtID);
        SPMTTree->SetBranchAddress("PMTID",&SPMTID);
        SPMTTree->SetBranchAddress("TotalPE",&TotalPE);

        int SPMTentry = 0;

        Int_t nEvent = CalibTree->GetEntries();
        for (int i = 0; i < nEvent; i++)
        {
            LPMTPE = 0;
            for (int id = 0; id < 6; id++)
            {
                SPMTPE[id] = 0;
            }

            CalibTree->GetEntry(i);

            for (int j = 0; j < Charge->size(); j++)
            {
                if (PMTID->at(j) == LPMT_ID)
                {
                    LPMTPE += Charge->at(j);
                }
            }

            SPMTTree->GetEntry(SPMTentry);
            while (EvtID == i)
            {
                SPMTTree->GetEntry(SPMTentry);
                for (int id = 0; id < 6; id++)
                {
                    if (SPMTID == SPMT_ID[id])
                    {
                        SPMTPE[id] += TotalPE;
                    }
                }
                SPMTentry++;
            }

            // if (LPMTPE > 0)
            // {
            //     LPMTChargeSpectrum->Fill(LPMTPE);
            //     for (int id = 0; id < 6; id++)
            //     {
            //         SPMTChargeSpectrum[id]->Fill(SPMTPE[id]);
            //     }
            // }
            charge->Fill();
            cout<<"Event "<<i<<endl;
        }
        
        cout<<"Number "<<filenum<<" finished"<<endl;
        inputfile->Close();
    }


    TFile *outputfile = new TFile(outputname,"UPDATE");
    
    // outputfile->Delete(TString::Format("LPMT charge spectrun %1dMeV %1dfile;*",energy,filenum));
    // LPMTChargeSpectrum->Write();
    // for (int id = 0; id < 6; id++)
    // {
    //     outputfile->Delete(TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile;*",id,energy,filenum));
    //     SPMTChargeSpectrum[id]->Write();
    // }

    outputfile->Delete(TString::Format("Charge%1d;*",filenum));
    charge->Write();
    outputfile->Close();
    return 1;
}