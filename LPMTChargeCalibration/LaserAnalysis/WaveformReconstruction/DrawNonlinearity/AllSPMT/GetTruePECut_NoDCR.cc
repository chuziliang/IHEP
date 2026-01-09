//用于比较Calib之后的电荷数与DetSim的PE数
// 2023年9月12日，用所有LPMT的电荷总数做Cut，去掉过小的。
// 2023年10月9日，考虑所有的SPMT
// 获得电子学模拟之后的SPMT电荷谱

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
// const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int GetTruePECut_NoDCR(int energy, int filenum)
{
    double Threshold = 4.0*energy;

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/ChargeSpectrum/%1dMevCharge_NoDCR.root",energy),"UPDATE");

    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    double TrueChargeLPMT;
    double CalibChargeLPMT;
    double SPMTPE[25600];
    double TrueSPMTPE[25600];

    charge->Branch("SPMTCharge",SPMTPE,"SPMTCharge[25600]/D");
    charge->Branch("TrueSPMTCharge",TrueSPMTPE,"TrueSPMTCharge[25600]/D");
    charge->Branch("TrueCharge",&TrueChargeLPMT,"TrueCharge/D");
    charge->Branch("LPMTCharge",&CalibChargeLPMT,"CalibCharge/D");

    TH2 *output = new TH2F(TString::Format("Calib charge to true charge %1d",filenum),"Calib charge to true charge",1000,0,100,1000,0,100);

    // for (int filenum = 0; filenum < 1; filenum++)
    {
        TFile* ElecFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/elec/root/%1dMeV/elec-%1d.root",energy,filenum));
        TFile* CalibFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum));
        if (!ElecFile || !CalibFile) return 1;
        
        TTree* ElecTree = (TTree*)ElecFile->Get("Event/CdLpmtTruth/CdLpmtElecTruthEvt");
        TTree* SPMTElecTree = (TTree*)ElecFile->Get("Event/CdSpmtTruth/CdSpmtElecTruthEvt");
        TTree* CalibTree = (TTree*)CalibFile->Get("calibevt");
        TTree* SPMTTree = (TTree*)CalibFile->Get("spmtcalibevt");

        int EvtID;
        int SPMTID;
        float TotalPE;
        SPMTTree->SetBranchAddress("EvtId",&EvtID);
        SPMTTree->SetBranchAddress("PMTID",&SPMTID);
        SPMTTree->SetBranchAddress("TotalPE",&TotalPE);
        
        JM::CdLpmtElecTruthEvt* aEvent = NULL;
        ElecTree->SetBranchAddress("CdLpmtElecTruthEvt",&aEvent);

        JM::CdSpmtElecTruthEvt* aSPMTEvent = NULL;
        SPMTElecTree->SetBranchAddress("CdSpmtElecTruthEvt",&aSPMTEvent);

        vector<float>* Charge = NULL;
        vector<int>* PMTID = NULL;
        CalibTree->SetBranchAddress("Charge",&Charge);
        CalibTree->SetBranchAddress("SimplePMTID",&PMTID);

        int nEvent = SPMTElecTree->GetEntries();
        // if (nEvent != CalibTree->GetEntries())
        // {
        //     cerr<<"Entries not equal between Elec and Calib!!!"<<endl;
        //     return 1;
        // }

        int SPMTentry = 0;
        for(int i = 0; i < nEvent; i++)
        {
            TrueChargeLPMT = 0;
            CalibChargeLPMT = 0;
            double TotalChargeLPMT = 0;
            double TotalChargeSPMT = 0;
            for (int id = 0; id < 25600; id++)
            {
                SPMTPE[id] = 0;
                TrueSPMTPE[id] = 0;
            }

            ElecTree->GetEntry(i);
            SPMTElecTree->GetEntry(i);
            CalibTree->GetEntry(i);
            //cout << "EventID: " << i << endl;
            const std::vector<JM::LpmtElecTruth>& truth = aEvent->truths();
            const std::vector<JM::SpmtElecTruth>& SPMTtruth = aSPMTEvent->truths();

            int nPulse = truth.size();
            
            for(int j = 0; j < nPulse; j++)
            {
                TotalChargeLPMT += truth[j].npe();
                if (truth[j].pmtId() == LPMT_ID)
                {
                    TrueChargeLPMT += truth[j].npe();
                }
            }

            for (int j = 0; j < SPMTtruth.size(); j++)
            {
                TotalChargeSPMT += SPMTtruth[j].npe();
                TrueSPMTPE[SPMTtruth[j].pmtId() - 300000] += SPMTtruth[j].npe();
            }
            

            for (int j = 0; j < Charge->size(); j++)
            {
                if (PMTID->at(j) == LPMT_ID)
                {
                    CalibChargeLPMT += Charge->at(j);
                }
            }



            SPMTTree->GetEntry(SPMTentry);
            while (EvtID == i)
            {
                SPMTTree->GetEntry(SPMTentry);
                SPMTPE[SPMTID - 300000] += TotalPE;
                SPMTentry++;
            }

            if (TotalChargeSPMT > Threshold)
            {
                charge->Fill();
                output->Fill(TrueChargeLPMT,CalibChargeLPMT);
            }
            
        }

    }
    outputfile->cd();

    output->Write();
    charge->Write();

    return 0;

}