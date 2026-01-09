void map_PMTID_J23_1_0_rc2()
  {
  const double cut = 400;
  ifstream inputfile_LPMT;
  TString filename_LPMT="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_LPMT.csv";
  inputfile_LPMT.open(filename_LPMT,ios::in);
  if(!inputfile_LPMT.good()) {cout<<"error"<<endl;return 0;}
  ifstream inputfile_SPMT;
  TString filename_SPMT="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/data/Detector/Geometry/PMTPos_CD_SPMT.csv";
  inputfile_SPMT.open(filename_SPMT,ios::in);
  if(!inputfile_SPMT.good()) {cout<<"error"<<endl;return 0;}

  const int N_LPMT = 17612;
  const int N_SPMT = 25600;
  int ID_LPMT[N_LPMT] = {0};
  int ID_SPMT[N_SPMT] = {0};
  //double R = 19000.;
  double Phi_LPMT[N_LPMT]  = {0};
  double Phi_SPMT[N_SPMT] = {0};
  double Theta_LPMT[N_LPMT]  = {0};
  double Theta_SPMT[N_SPMT] = {0};
  double x_LPMT[N_LPMT]  = {0};
  double y_LPMT[N_LPMT]  = {0};
  double z_LPMT[N_LPMT]  = {0};
  double x_SPMT[N_SPMT]  = {0};
  double y_SPMT[N_SPMT]  = {0};
  double z_SPMT[N_SPMT]  = {0};
  for(int i=0;i<N_LPMT;i++)
    {
    inputfile_LPMT>>ID_LPMT[i]>>x_LPMT[i]>>y_LPMT[i]>>z_LPMT[i]>>Phi_LPMT[i]>>Theta_LPMT[i];
    //cout<<temp<<" "<<Phi_LPMT[i]<<" "<<Theta_LPMT[i]<<endl;
    // if(Phi_LPMT[i]==89.083&&Theta_LPMT[i]==90) cout<<"Find LPMTID="<<i<<" "<<Phi_LPMT[i]<<" "<<Theta_LPMT[i]<<endl;
    // Phi_LPMT[i] = Phi_LPMT[i]/180.*TMath::Pi();
    // Theta_LPMT[i] = Theta_LPMT[i]/180.*TMath::Pi();
    // x_LPMT[i] = R*TMath::Sin(Phi_LPMT[i])*TMath::Cos(Theta_LPMT[i]);
    // y_LPMT[i] = R*TMath::Sin(Phi_LPMT[i])*TMath::Sin(Theta_LPMT[i]);
    // z_LPMT[i] = R*TMath::Cos(Phi_LPMT[i]);
    // ID_LPMT[i] = i;
    //cout<<temp<<" "<<Phi_LPMT[i]<<" "<<Theta_LPMT[i]<<endl;
    //cout<<ID_LPMT[i]<<"  "<<x_LPMT[i]<<"  "<<y_LPMT[i]<<"  "<<z_LPMT[i]<<"  "<<Phi_LPMT[i]<<"  "<<Theta_LPMT[i]<<endl;
    }
  
  for(int i=0;i<N_SPMT;i++)
    {
    inputfile_SPMT>>ID_SPMT[i]>>x_SPMT[i]>>y_SPMT[i]>>z_SPMT[i]>>Phi_SPMT[i]>>Theta_SPMT[i];
    // Phi_SPMT[i] = Phi_SPMT[i]/180.*TMath::Pi();
    // Theta_SPMT[i] = Theta_SPMT[i]/180.*TMath::Pi();
    // x_SPMT[i] = R*TMath::Sin(Phi_SPMT[i])*TMath::Cos(Theta_SPMT[i]);
    // y_SPMT[i] = R*TMath::Sin(Phi_SPMT[i])*TMath::Sin(Theta_SPMT[i]);
    // z_SPMT[i] = R*TMath::Cos(Phi_SPMT[i]);
    // ID_SPMT[i] = 300000+i;
    // //cout<<temp<<" "<<Phi_SPMT[i]<<" "<<Theta_SPMT[i]<<endl;
    //cout<<ID_SPMT[i]<<"  "<<x_SPMT[i]<<"  "<<y_SPMT[i]<<"  "<<z_SPMT[i]<<"  "<<Phi_SPMT[i]<<"  "<<Theta_SPMT[i]<<endl;
    }

  double deltaR = 0;
  int tag = 0;
  int mapID[N_LPMT][10];
  for(int i = 0; i < N_LPMT; i++)
    {
    for(int j = 0; j < 10; j++)
      {
      mapID[i][j] = 50000;
      }
    }
  TH1F *hN = new TH1F("hN","hN",20,0,20);
  TH1F *hR = new TH1F("hR","hR",4000,0,40000);
  for(int i = 0; i < N_LPMT; i++)
    {
    if(i%1000==0) cout<<i<<endl;
    tag = 0;
    for(int j = 0; j < N_SPMT; j++)
      {
      deltaR = 0;
      deltaR += (x_LPMT[i]-x_SPMT[j])*(x_LPMT[i]-x_SPMT[j]);
      deltaR += (y_LPMT[i]-y_SPMT[j])*(y_LPMT[i]-y_SPMT[j]);
      deltaR += (z_LPMT[i]-z_SPMT[j])*(z_LPMT[i]-z_SPMT[j]);
      deltaR = sqrt(deltaR);
      hR->Fill(deltaR);
      if(deltaR<=cut) 
        {
        mapID[i][tag] = 300000+j;
        tag++;
        if(tag>9) break;
        }
      }
          //if(tag>0) cout<<i<<" "<<tag<<endl;
          hN->Fill(tag);
          if(tag>9) {cout<<"ID_LPMT:"<<i<<" with corresponding SPMT more than 10, check !!!"<<endl; break;}
          if(tag==0) cout<<"ID_LPMT:"<<i<<" without corresponding SPMT"<<endl;
      }
        
  TFile *f = new TFile("map_result.root","recreate");
  hN->Write();
  hR->Write(); 
  hN->Draw();
  // if(tag>9) break;
 
  std::ofstream outf;
  outf.open("PMTID_map_J23.1.0-rc2.txt", ios::out | ios::trunc);  
  for(int i = 0; i < N_LPMT; i++)
    {
    outf<<i<<" ";
    for(int j = 0; j < 10; j++)
      {
      outf<<mapID[i][j]<<" ";
      }
    outf<<endl;
    }
  
  }
