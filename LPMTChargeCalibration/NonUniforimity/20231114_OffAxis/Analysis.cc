//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。
#include <iostream>
#include <vector>
#include "TString.h"
#include <cmath>

#include "Event/SimEvt.h"

using namespace std;
using namespace JM;

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

double GetTheta(const int LPMT_ID,Vector3D edep)
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;
    int currentLine = 0;

    while (getline(file, line)) {
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            Vector3D PMT;
            double nun;
            double currentPMT;
            ss >> currentPMT >> PMT.x >> PMT.y >> PMT.z >> nun >> nun;
            if (currentPMT != LPMT_ID)
            {
                cout<<"Error!!!"<<endl;
            }
            
            Vector3D LPMT_edep;//PMT指向能量沉积点的向量
            LPMT_edep = VectorMinus(edep,PMT);
            Vector3D FZ;
            FZ.x = -PMT.x;
            FZ.y = -PMT.y;
            FZ.z = -PMT.z;
            double z = DotProduct(FZ,LPMT_edep)/Norm(FZ);
            double r = Norm(LPMT_edep);
            double angle = acos(z/r) * 180 / M_PI;
            file.close();
            return angle;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

double GetPhi(const int LPMT_ID, Vector3D edep)
{
    ifstream file("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Detector/Geometry/PMTPos_CD_LPMT.csv");
    string line;
    int currentLine = 0;

    while (getline(file, line)) {
        if (currentLine == LPMT_ID) {
            file.close();
            stringstream ss(line);
            Vector3D PMT;
            double nun;
            ss >> nun >> PMT.x >> PMT.y >> PMT.z >> nun >> nun;
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
            file.close();
            return angle;
        }
        currentLine++;
    }

    file.close();
    return 0;
}

int Analysis(const int foldernum, const int filenum, const int LPMT_ID)
{

    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231114_OffAxis/folder_path.txt");
    string line;
    int currentLine = 0;

    while (getline(file, line)) 
    {
        
        if (currentLine == foldernum) 
        {
            break;
        }
        currentLine++;
    }

    int SPMT_ID[6];
    if (GetCorrespondingSPMT(SPMT_ID,LPMT_ID) != LPMT_ID)
    {
        cerr<<"ERROR: Wrong PMT ID!"<<endl;
        return 0;
    }

    string outputname;
    outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231114_OffAxis/ChargeSpectrum/" + line + "/ChargeSpectrum_"+to_string(filenum)+".root";
    TFile *outputfile = new TFile((TString)outputname,"recreate");
    outputfile->cd();
    TTree *OutputTree = new TTree("Charge","LPMT and SPMT charge");
    double OutputLPMT; 
    double OutputSPMT[6];
    double OutputTheta;
    double OutputPhi;
    double edep_x, edep_y, edep_z;
    OutputTree->Branch("SPMTCharge",OutputSPMT,"SPMTCharge[6]/D");
    OutputTree->Branch("LPMTCharge",&OutputLPMT,"LPMTCharge/D");
    OutputTree->Branch("Theta",&OutputTheta,"Theta/D");
    OutputTree->Branch("Phi",&OutputPhi,"Phi/D");
    OutputTree->Branch("edep_x",&edep_x,"edep_x/D");
    OutputTree->Branch("edep_y",&edep_y,"edep_y/D");
    OutputTree->Branch("edep_z",&edep_z,"edep_z/D");

    TFile *inputfile = NULL;
    string filename = "/junofs/users/junospmt/GXU/Data/Masstest/Input_file/Other_Data/map/out/" + line + "/detsim_GRDM_Ge68_"+to_string(filenum)+".root";
    inputfile = TFile::Open((TString)filename);
    if (!inputfile)
    {
        cerr<<"ERROR: root file not exist!"<<endl;
        cerr<<filename<<endl;
        return 0;
    }
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
        OutputLPMT = LPMTPE;
        for (int id = 0; id < 6; id++)
        {
            OutputSPMT[id] = SPMTPE[id];
        }
        OutputTheta = GetTheta(LPMT_ID,edep);
        if (OutputTheta == 0)
        {
            cout<<edep.x<<" "<<edep.y<<" "<<edep.z<<endl;
        }
        
        OutputPhi = GetPhi(LPMT_ID,edep);

        edep_x = edep.x;
        edep_y = edep.y;
        edep_z = edep.z;
        outputfile->cd();
        OutputTree->Fill();
        inputfile->cd();
    }

    cout<<line + "/detsim_GRDM_Ge68_"+to_string(filenum)+".root"<<"\nFinished"<<endl;
    inputfile->Close();

    outputfile->cd();
    OutputTree->Write();
    outputfile->Close();
    return 1;
}