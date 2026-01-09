//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。

using namespace std;
using namespace JM;

const int ChoseLPMT_ID = 6828;

const double QuantilesR[20] = {11830,14778.3,16948.3,18591.7,20054,21497.5,22676,23890,24942,25821.7,26645,27396.7,28168.3,28955,29728,30610,31603.3,32616.7,33966.7,39990};

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

int GetChargeSpectrum_Neutron(const int filenum)
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
    string outputname;
    outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/NeutronChargeSpectrum/Charge_"+to_string(filenum)+".root";
    TFile *outputfile = new TFile((TString)outputname,"recreate");
    outputfile->cd();

    // 需要输出的tree
    double OutputLPMT[17612]; 
    double OutputSPMT[25600];
    double OutputTheta[17612];
    double OutputPhi[17612];
    double OutputR[17612];
    double edep_x, edep_y, edep_z;
    double edepEnergy;
    TTree *OutputTree;
    OutputTree = new TTree("charge","LPMT and SPMT charge");
    OutputTree->Branch("LPMTCharge_QE",OutputLPMT,"LPMTCharge_QE[17612]/D");
    OutputTree->Branch("SPMTCharge_QE",OutputSPMT,"SPMTCharge_QE[25600]/D");
    // OutputTree->Branch("Theta",OutputTheta,"Theta[17612]/D");
    // OutputTree->Branch("Phi",OutputPhi,"Phi[17612]/D");
    // OutputTree->Branch("Distance between LPMT and event",OutputR,"R[17612]/D");
    OutputTree->Branch("edep_x",&edep_x,"edep_x/D");
    OutputTree->Branch("edep_y",&edep_y,"edep_y/D");
    OutputTree->Branch("edep_z",&edep_z,"edep_z/D");
    OutputTree->Branch("edep",&edepEnergy,"edep/D");

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
        for (int ID = 0; ID < 17612; ID++)
        {
            OutputLPMT[ID] = 0;
            OutputSPMT[ID] = 0;
        }
        for (int ID = 17612; ID < 25600; ID++)
        {
            OutputSPMT[ID] = 0;
        }

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
            if (pmtid < 17612)
            {
                OutputLPMT[pmtid] += npe;
            }
            else if (pmtid >= 300000 && pmtid <= 325600)
            {
                OutputSPMT[pmtid - 300000] += npe;
            }
            
        }

        for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
        {
            OutputLPMT[LPMTID] /= LPMTQE[LPMTID];
            OutputTheta[LPMTID] = GetTheta(LPMTPosition[LPMTID],edep);
            OutputPhi[LPMTID] = GetPhi(LPMTPosition[LPMTID],edep);
            OutputR[LPMTID] = GetR(LPMTPosition[LPMTID],edep);
        }

        for (int SPMTID = 0; SPMTID < 25600; SPMTID++)
        {
            OutputSPMT[SPMTID] /= SPMTQE[SPMTID];
        }

        edep_x = edep.x;
        edep_y = edep.y;
        edep_z = edep.z;
        outputfile->cd();
        OutputTree->Fill();
    
        inputfile->cd();
        cout<<i<<" Finished"<<endl;
    }
    inputfile->Close();

    outputfile->cd();
    OutputTree->Write();
    outputfile->Close();
    return 1;
}