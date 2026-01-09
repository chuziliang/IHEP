//分析EOS中激光刻度的数据，找出探测器模拟级别下，LPMT与Corresponding SPMT的nPE分布。

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

int GetChargeSpectrum_DisableSticks()
{
    // 打开文件
    TFile *inputfile = NULL;
    TString filename = "root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/test/detsim_0_DisableSticks.root";
    inputfile = TFile::Open(filename,"read");
    if (!inputfile)
    {
        cerr<<"ERROR: root file not exist!"<<endl;
        cerr<<filename<<endl;
        return 0;
    }

    // 输出文件
    string outputname;
    outputname = "/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ChargeSpectrum_DisableSticks.root";
    TFile *outputfile = new TFile((TString)outputname,"recreate");
    outputfile->cd();

    // 需要输出的tree
    double OutputLPMT[17612]; 
    double OutputSPMT[25600];
    TTree *OutputTree;
    OutputTree = new TTree("charge","LPMT and SPMT charge");
    OutputTree->Branch("LPMTCharge",OutputLPMT,"LPMTCharge[17612]/D");
    OutputTree->Branch("SPMTCharge",OutputSPMT,"SPMTCharge[25600]/D");
    
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

        if (m_nhits != m_cd_hits.size())
        {
            cerr<<"ERROR m_nhits != m_cd_hits.size()!"<<endl;
        }
        
        for (int HitNum = 0; HitNum<m_nhits; HitNum++)
        {
            JM::SimPMTHit* hit_info = m_cd_hits[HitNum];
            
            pmtid = hit_info->getPMTID();
            npe = hit_info->getNPE();
            if (pmtid < 30000)
            {
                OutputLPMT[pmtid] += npe;
            }
            else
            {
                OutputSPMT[pmtid - 300000] += npe;
            }
        }

        OutputTree->Fill();

        inputfile->cd();
        // cout<<i<<" Finished"<<endl;
    }
    inputfile->Close();

    outputfile->cd();
    OutputTree->Write();
    outputfile->Close();
    return 1;
}