//用于比较Calib之后的电荷数与DetSim的PE数
// 2023年9月12日，用所有LPMT的电荷总数做Cut，去掉过小的。

using namespace std;
using namespace JM;

int GetTruePECut_AllSPMT(int energy, int filenum)
{
    double Threshold = 170.0*energy;

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut_WithTotalPE_AllSPMT_AllLPMT/%1dMevCharge.root",energy),"UPDATE");

    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    vector<double> TrueChargeLPMT;
    vector<double> CalibChargeLPMT;
    vector<double> SPMTPE;
    double TotalChargeLPMT = 0;

    charge->Branch("SPMTCharge",&SPMTPE);
    charge->Branch("TrueChargeLPMT",&TrueChargeLPMT);
    charge->Branch("CalibChargeLPMT",&CalibChargeLPMT);
    charge->Branch("TotalChargeLPMT",&TotalChargeLPMT,"TotalChargeLPMT/D");

    // for (int filenum = 0; filenum < 1; filenum++)
    {
        TFile* ElecFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/elec/root/%1dMeV/elec-%1d.root",energy,filenum));
        TFile* CalibFile = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/user/%1dMeV/calib-%1d.root",energy,filenum));
        if (!ElecFile)
        {
            cout<<energy<<"Mev/elec-"<<filenum<<" not exist!"<<endl;
        }
        if (!CalibFile)
        {
            cout<<energy<<"Mev/Calib-"<<filenum<<" not exist!"<<endl;
        }
        if (!ElecFile || !CalibFile) return 1;
        
        TTree* ElecTree = (TTree*)ElecFile->Get("Event/CdLpmtTruth/CdLpmtElecTruthEvt");
        TTree* CalibTree = (TTree*)CalibFile->Get("calibevt");
        TTree* SPMTTree = (TTree*)CalibFile->Get("spmtcalibevt");

        JM::CdLpmtElecTruthEvt* aEvent = NULL;
        ElecTree->SetBranchAddress("CdLpmtElecTruthEvt",&aEvent);

        int EvtID;
        int SPMTID;
        float TotalPE;
        SPMTTree->SetBranchAddress("EvtId",&EvtID);
        SPMTTree->SetBranchAddress("PMTID",&SPMTID);
        SPMTTree->SetBranchAddress("TotalPE",&TotalPE);

        vector<float>* Charge = NULL;
        vector<int>* PMTID = NULL;
        vector<float>* Time = NULL;
        float DetTotalPE;
        CalibTree->SetBranchAddress("Charge",&Charge);
        CalibTree->SetBranchAddress("SimplePMTID",&PMTID);
        CalibTree->SetBranchAddress("TotalPE",&DetTotalPE);
        CalibTree->SetBranchAddress("Time",&Time);

        int nEvent = CalibTree->GetEntries();
        if (nEvent != ElecTree->GetEntries())
        {
            cerr<<"Entries not equal between Elec and Calib!!!"<<endl;
            return 1;
        }

        int SPMTentry = 0;
        for(int i = 0; i < nEvent; i++)
        {
            ElecTree->GetEntry(i);
            CalibTree->GetEntry(i);

            const std::vector<JM::LpmtElecTruth>& truth = aEvent->truths();
            TrueChargeLPMT = vector<double>(17612,0);
            CalibChargeLPMT = vector<double>(17612,0);
            SPMTPE = vector<double>(25600,0);
            TotalChargeLPMT = 0;

            
            //cout << "EventID: " << i << endl;
            

            int nPulse = truth.size();
            
            for(int j = 0; j < nPulse; j++)
            {
                if (truth[j].pmtId() < 17612)
                {
                    TrueChargeLPMT[truth[j].pmtId()] += truth[j].npe();
                    TotalChargeLPMT += truth[j].npe();
                }
            }

            for (int j = 0; j < Charge->size(); j++)
            {
                if (PMTID->at(j) < 17612)
                {
                    CalibChargeLPMT[PMTID->at(j)] += Charge->at(j);
                }
            }

            SPMTTree->GetEntry(SPMTentry);
            while (EvtID == i)
            {
                SPMTTree->GetEntry(SPMTentry);
                SPMTPE[SPMTID - 300000] += TotalPE;
                SPMTentry++;
            }

            if (TotalChargeLPMT > Threshold)
            {
                charge->Fill();
            }


            // charge->Fill();
            SPMTPE.clear();
            TrueChargeLPMT.clear();
            CalibChargeLPMT.clear();
        }
    }
    outputfile->cd();

    charge->Write();

    return 0;

}