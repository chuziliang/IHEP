void map_PMTID()
  {
  ifstream inputfile_LPMT;
  TString filename_LPMT="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_LPMT.csv";
  inputfile_LPMT.open(filename_LPMT,ios::in);
  if(!inputfile_LPMT.good()) {cout<<"error LPMT"<<endl;return 0;}

  ifstream inputfile_anchor;
  TString filename_anchor="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/junosw/Simulation/DetSimV2/DetSimOptions/data/Strut_Anchor_Acrylic.csv";
  inputfile_anchor.open(filename_anchor,ios::in);
  if(!inputfile_anchor.good()) {cout<<"error anchor"<<endl;return 0;}

  const int N_LPMT = 17612;
  int ID_LPMT[N_LPMT] = {0};
  //double R = 19000.;
  double Phi_LPMT[N_LPMT]  = {0};
  double Theta_LPMT[N_LPMT]  = {0};
  double x_LPMT[N_LPMT]  = {0};
  double y_LPMT[N_LPMT]  = {0};
  double z_LPMT[N_LPMT]  = {0};

  const int N_Anchor = 590;
  int ID_Anchor[N_Anchor] = {0};
  double R = 19434.;
  double Phi_Anchor[N_Anchor]  = {0};
  double Theta_Anchor[N_Anchor]  = {0};
  double x_Anchor[N_Anchor]  = {0};
  double y_Anchor[N_Anchor]  = {0};
  double z_Anchor[N_Anchor]  = {0};
  
  for(int i=0;i<N_LPMT;i++)
    {
    inputfile_LPMT>>ID_LPMT[i]>>x_LPMT[i]>>y_LPMT[i]>>z_LPMT[i]>>Theta_LPMT[i]>>Phi_LPMT[i];
    }
  
  for(int i=0;i<N_Anchor;i++)
    {
    inputfile_anchor>>ID_Anchor[i]>>Theta_Anchor[i]>>Phi_Anchor[i];
    z_Anchor[i] = R*cos(Theta_Anchor[i]*M_PI/180.0);
    x_Anchor[i] = R*sin(Theta_Anchor[i]*M_PI/180.0)*cos(Phi_Anchor[i]*M_PI/180.0);
    y_Anchor[i] = R*sin(Theta_Anchor[i]*M_PI/180.0)*sin(Phi_Anchor[i]*M_PI/180.0);
    }

  double deltaR = 0;
  int CloseAnchor[N_LPMT];
  double CloseR[N_LPMT];

  TH1F *hR = new TH1F("hR","hR",4000,0,40000);
  TH1F *hCloseR = new TH1F("hCloseR","hCloseR",3000,0,3000);
  for(int i = 0; i < N_LPMT; i++)
    {
    if(i%1000==0) cout<<i<<endl;
    CloseR[i] = 19434;
    for(int j = 0; j < N_Anchor; j++)
      {
      deltaR = 0;
      deltaR += (x_LPMT[i]-x_Anchor[j])*(x_LPMT[i]-x_Anchor[j]);
      deltaR += (y_LPMT[i]-y_Anchor[j])*(y_LPMT[i]-y_Anchor[j]);
      deltaR += (z_LPMT[i]-z_Anchor[j])*(z_LPMT[i]-z_Anchor[j]);
      deltaR = sqrt(deltaR);
      if (deltaR < CloseR[i])
      {
        CloseR[i] = deltaR;
        CloseAnchor[i] = ID_Anchor[j];
      }

      hR->Fill(deltaR);
      }
    hCloseR->Fill(CloseR[i]);
    }
        
  TFile *f = new TFile("map_result.root","recreate");
  hR->Write(); 
  hCloseR->Write();
  hCloseR->Draw();
  // if(tag>9) break;
 
  std::ofstream outf;
  outf.open("PMTID_map.txt", ios::out | ios::trunc);  
  for(int i = 0; i < N_LPMT; i++)
    {
    outf<<ID_LPMT[i]<<" ";
    outf<<CloseAnchor[i]<<" ";
    outf<<CloseR[i]<<" ";
    outf<<endl;
    }
  
  }
