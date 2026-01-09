//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"

using namespace std;
using namespace JM;

const int ChoseLPMT_ID = 6828;

struct Vector3D
{
    double x,y,z;
};

double DotProduct(Vector3D A, Vector3D B)//求两个向量的点乘
{
    double result;
    result = A.x*B.x + A.y*B.y + A.z*B.z;
    return result;
}

double Norm(Vector3D A)//求向量的模
{
    double result;
    result = sqrt(pow(A.x, 2) + pow(A.y, 2) + pow(A.z, 2));
    return result;
}

Vector3D VectorMinus(Vector3D A,Vector3D B)//A向量减B向量
{
    Vector3D result;
    result.x = A.x - B.x;
    result.y = A.y - B.y;
    result.z = A.z - B.z;
    return result;
}

int GetCorrespondingSPMT(double (&CorrespondingSPMT)[17612][6])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map.txt");
    string line;
    int currentLine = 0;
    int SPMT_ID[6];

    while (getline(file, line)) 
    { 
        stringstream ss(line);
        int LPMTID;
        ss >> LPMTID >> SPMT_ID[0] >> SPMT_ID[1] >> SPMT_ID[2] >> SPMT_ID[3] >> SPMT_ID[4] >> SPMT_ID[5];
        for (int i = 0; i < 6; i++)
        {
            CorrespondingSPMT[LPMTID][i] = SPMT_ID[i];
        }
    }

    file.close();
    return 0;
}

double GetLPMTPosition(Vector3D (&LPMTPosition)[17612])
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;

    while (getline(file, line)) 
    {
        stringstream ss(line);
        Vector3D PMT;
        double nun;
        int currentPMT;
        ss >> currentPMT >> PMT.x >> PMT.y >> PMT.z >> nun >> nun;
        LPMTPosition[currentPMT] = PMT;
    }

    file.close();
    return 0;
}

double GetTheta(Vector3D PMT,Vector3D edep)
{
    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);
    Vector3D FZ;
    FZ.x = -PMT.x;
    FZ.y = -PMT.y;
    FZ.z = -PMT.z;
    double z = DotProduct(FZ,LPMT_edep)/Norm(FZ);
    double r = Norm(LPMT_edep);
    double angle = acos(z/r) * 180 / M_PI;
    return angle;
}

double GetPhi(Vector3D PMT, Vector3D edep)
{
    Vector3D FX;//随动坐标系的x
    Vector3D FY;//随动坐标系的y
    FX.x = -PMT.x;
    FX.y = -PMT.y;
    FX.z = (pow(PMT.x, 2)+pow(PMT.y, 2))/PMT.z;
    FY.x = -PMT.y;
    FY.y = PMT.x;
    FY.z = 0;

    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);

    double x = DotProduct(LPMT_edep,FX)/Norm(FX);
    double y = DotProduct(LPMT_edep,FY)/Norm(FY);
    double angle = atan(y/x) * 180 / M_PI;
    if (y>0 && angle<0)
    {
        angle += 180;
    }
    else if (y<0 && angle>0)
    {
        angle += 180;
    }
    else if (y<0 && angle<0)
    {
        angle += 360;
    }
    
    return angle;
}

double GetR(Vector3D PMT,Vector3D edep)
{
    Vector3D LPMT_edep;//PMT指向能量沉积点的向量
    LPMT_edep = VectorMinus(edep,PMT);
    double r = Norm(LPMT_edep);
    return r;
}

double GetSPMTQE(double (&SPMTQE)[25600])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    for (int id = 0; id < 25600; id ++)
    {
        PMTData->GetEntry(id);
        SPMTQE[id] = Effiency;
    }
    
    // if (PMTID == SPMTID)
    // {
    //     inputfile->Close();
    //     return 100*Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong SPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetLPMTQE(double (&LPMTQE)[17612])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    for (int id = 0; id < 17612; id ++)
    {
        PMTData->GetEntry(id);
        LPMTQE[id] = Effiency/100.0;
    }
    
    // if (PMTID == LPMTID)
    // {
    //     inputfile->Close();
    //     return Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong LPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

void AnalysisRhist(const int filenum)
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    double CorrespondingSPMT[17612][6];
    Vector3D LPMTPosition[17612];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);
    GetLPMTPosition(LPMTPosition);

    // 打开文件
    TFile *inputfile = NULL;
    string filename = "root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/Spallation_neutron/root/detsim_"+to_string(filenum)+".root";
    inputfile = TFile::Open((TString)filename);
    if (!inputfile)
    {
        cerr<<"ERROR: root file not exist!"<<endl;
        cerr<<filename<<endl;
        return 0;
    }

    // 输出文件
    // string outputname;
    // outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/Spallation_neutron/Analysis/ChargeSpectrum/Rhist_"+to_string(filenum)+".root";
    // TFile *outputfile = new TFile((TString)outputname,"recreate");
    // outputfile->cd();

    TH1 *Rhist = new TH1F("R distribution","R distribution",4000,0,40000);

    // 需要输出的tree
    double OutputLPMT; 
    double OutputSPMT[6];
    double OutputTheta;
    double OutputPhi;
    double OutputR;
    double edep_x, edep_y, edep_z;
    double edepEnergy;
    int SPMT_ID[6];
    int LPMT_ID;
    // TTree *OutputTree[10][50];
    // for (int EnergyNum = 0; EnergyNum < 10; EnergyNum++)
    // {
    //     for(int RNum = 0; RNum < 50; RNum++)
    //     {
    //         OutputTree[EnergyNum][RNum] = new TTree(TString::Format("Charge_E%1d_R%1d",EnergyNum,RNum),"LPMT and SPMT charge");
    //         OutputTree[EnergyNum][RNum]->Branch("LPMTCharge_QE",&OutputLPMT,"LPMTCharge_QE/D");            
    //         OutputTree[EnergyNum][RNum]->Branch("SPMTCharge_QE",OutputSPMT,"SPMTCharge_QE[6]/D");
    //         OutputTree[EnergyNum][RNum]->Branch("LPMT_ID",&LPMT_ID,"LPMT_ID/I");
    //         OutputTree[EnergyNum][RNum]->Branch("SPMT_ID",SPMT_ID,"SPMT_ID[6]/I");
    //         OutputTree[EnergyNum][RNum]->Branch("Theta",&OutputTheta,"Theta/D");
    //         OutputTree[EnergyNum][RNum]->Branch("Phi",&OutputPhi,"Phi/D");
    //         OutputTree[EnergyNum][RNum]->Branch("Distance between LPMT and event",&OutputR,"R/D");
    //         OutputTree[EnergyNum][RNum]->Branch("edep_x",&edep_x,"edep_x/D");
    //         OutputTree[EnergyNum][RNum]->Branch("edep_y",&edep_y,"edep_y/D");
    //         OutputTree[EnergyNum][RNum]->Branch("edep_z",&edep_z,"edep_z/D");
    //         OutputTree[EnergyNum][RNum]->Branch("edep",&edepEnergy,"edep/D");
    //     }
    // }

    inputfile->cd();
    TTree *Evt = (TTree*)inputfile->Get("Event/Sim/SimEvt");

    JM::SimEvt *sim_evt = NULL;
    vector<SimPMTHit*> m_cd_hits;
    vector<SimTrack*>  m_tracks;

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
        // for (int ID = 0; ID < 17612; ID++)
        // {
        //     LPMTPE[ID] = 0;
        //     SPMTPE[ID] = 0;
        // }
        // for (int ID = 17612; ID < 25600; ID++)
        // {
        //     SPMTPE[ID] = 0;
        // }

        Evt->GetEntry(i);
        m_cd_hits = sim_evt->getCDHitsVec();
        m_tracks  = sim_evt->getTracksVec();

        if (m_nhits != m_cd_hits.size())
        {
            cerr<<"ERROR m_nhits != m_cd_hits.size()!"<<endl;
        }

        JM::SimTrack* track_info = m_tracks[0];
        if (track_info->getEdep() == 0)
        {
            continue;
        }
        
        Vector3D edep;
        edep.x = track_info->getEdepX();
        edep.y = track_info->getEdepY();
        edep.z = track_info->getEdepZ();
        edepEnergy = track_info->getEdep();

        for (int HitNum = 0; HitNum<m_nhits; HitNum++)
        {
            JM::SimPMTHit* hit_info = m_cd_hits[HitNum];
            
            pmtid = hit_info->getPMTID();
            npe = hit_info->getNPE();
            if (pmtid == ChoseLPMT_ID)
            {
                LPMTPE += npe;
            }
            for (int id = 0; id < 6; id++)
            {
                if (pmtid == CorrespondingSPMT[ChoseLPMT_ID][id])
                {
                    SPMTPE[id] += npe;
                }
            }
        }


        OutputLPMT = LPMTPE/LPMTQE[ChoseLPMT_ID];
        LPMT_ID = ChoseLPMT_ID;
        
        for (int id = 0; id < 6; id++)
        {
            SPMT_ID[id] = CorrespondingSPMT[ChoseLPMT_ID][id];
            if (SPMT_ID[id] >= 300000)
            {
                OutputSPMT[id] = SPMTPE[id]/SPMTQE[SPMT_ID[id]-300000];
            }
            else
            {
                OutputSPMT[id] = -1;
            }
        }
        OutputTheta = GetTheta(LPMTPosition[LPMT_ID],edep);
        if (OutputTheta > 40)
        {
            continue;
        }
        OutputPhi = GetPhi(LPMTPosition[LPMT_ID],edep);
        OutputR = GetR(LPMTPosition[LPMT_ID],edep);

        edep_x = edep.x;
        edep_y = edep.y;
        edep_z = edep.z;
        // outputfile->cd();
        Rhist->Fill(OutputR);
        int En, Rn;
        En = static_cast<int>(edepEnergy/1.4);
        Rn = static_cast<int>((OutputR-1934)*50.0/35000.0);
        if (En > 9) En = 9;
        if (Rn > 49) Rn = 49;
        if (Rn < 0) Rn = 0;
    
        // OutputTree[En][Rn]->Fill();
    
        inputfile->cd();
        cout<<i<<" Finished"<<endl;
    }
    // inputfile->Close();

    Rhist->Draw();
    Double_t xq[20];  // position where to compute the quantiles in [0,1]
    Double_t yq[20];  // array to contain the quantiles
    for (Int_t i=0;i<20;i++) xq[i] = Float_t(i+1)/20;
    Rhist->GetQuantiles(20,yq,xq);
    for (Int_t i=0;i<20;i++)
    {
        cout<<"Quantiles = "<<xq[i]<<"   R number = "<<yq[i]<<endl;
    }
    // outputfile->cd();
    // for (int EnergyNum = 0; EnergyNum < 10; EnergyNum++)
    // {
    //     for(int RNum = 0; RNum < 50; RNum++)
    //     {
    //         OutputTree[EnergyNum][RNum]->Write();
    //     }
    // }
    // outputfile->Close();
    // return 1;
}