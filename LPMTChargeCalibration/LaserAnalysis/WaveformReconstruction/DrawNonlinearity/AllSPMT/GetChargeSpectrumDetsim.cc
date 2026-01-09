//用于比较Calib之后的电荷数与DetSim的PE数
// 2023年9月12日，用所有LPMT的电荷总数做Cut，去掉过小的。
// 2023年10月9日，考虑所有的SPMT
// 获得DetSim级别的电荷谱

using namespace std;
using namespace JM;

const int LPMT_ID = 8947;
// const int SPMT_ID[6] = {300157,300158,300163,300164,303776,324355};

int GetChargeSpectrumDetsim(int energy, int filenum)
{

    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/DrawNonlinearity/AllSPMT/ChargeSpectrum_DetSim/%1dMevCharge.root",energy),"UPDATE");

    TTree* charge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
    double SPMTPE[25600];

    charge->Branch("SPMTCharge",SPMTPE,"SPMTCharge[25600]/D");

    TString filename;
    // for (int filenum = 0; filenum < 1; filenum++)
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
            for (int num = 0; num < 25600; num++)
            {
                SPMTPE[num] = 0;
            }
            
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

                if (pmtid >= 300000)
                {
                    SPMTPE[pmtid - 300000] += npe;
                }
            }
            charge->Fill();
        }

        cout<<"Number "<<filenum<<" finished"<<endl;
        inputfile->Close();

    }
    outputfile->cd();

    charge->Write();

    return 0;

}