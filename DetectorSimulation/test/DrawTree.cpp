using namespace std;
void DrawTree(int ievt=0){

  vector<float>* nodex=NULL; 
  vector<float>* nodey=NULL; 
  vector<float>* nodez=NULL; 
  vector<float>* nodeE=NULL;
  vector<float>* nodeType=NULL; 
  //TH2D *h2D_nodes = new TH2D("h2D_nodes","", 40,-200,200,30,1800,2100);
 
  TFile *yfile = new TFile("recdir/Rec_5GeV.root","read");
  TTree *ytree = (TTree*)yfile->Get("dataColIter0");
  //int Nentries = ytree->GetEntries();   //Yang. There are too many entries. Set it as 10
  int Nentries = 5;
  TH3D* truthHit[Nentries];
  TH2D *h2D_nodes[Nentries];
  TCanvas *canvas[Nentries];
  for(int i=0; i<Nentries; i++){
    TString s_it; s_it.Form("%d", i);
    canvas[i] = new TCanvas("c_"+s_it, "c_"+s_it, 1200,600);
    canvas[i]->Divide(2,1);
    h2D_nodes[i] = new TH2D("h2D_nodes"+s_it,"", 40,-200,200,30,1800,2100);
    truthHit[i]= new TH3D("truthHit"+s_it, "", 30, 1800, 2100., 50, -250, 250, 50, -250, 250);
  }

  ytree->SetBranchAddress("clus_x", &nodex);
  ytree->SetBranchAddress("clus_y", &nodey);
  ytree->SetBranchAddress("clus_z", &nodez);
  ytree->SetBranchAddress("clus_E", &nodeE);
  //ytree->SetBranchAddress("node0Type", &nodeType);

  cout<<"num of entries: "<<Nentries<<endl;
  for(int ii=0; ii<Nentries; ii++){
    ytree->GetEntry(ii);
    cout<<"Node size: "<<nodex->size()<<endl;
    for(int i=0; i<nodex->size(); i++){ 
      h2D_nodes[ii]->Fill( nodey->at(i), nodex->at(i), ii+1);
      int ibinx = (int)(nodex->at(i)-1800)/10 + 1;
      int ibiny = (int)(nodey->at(i)+250)/10 + 1;
      int ibinz = (int)(nodez->at(i)+250)/10 + 1;
      printf("  (%.2f, %.2f, %.2f) \n", nodex->at(i), nodey->at(i), nodez->at(i));
      truthHit[ii]->SetBinContent(ibinx, ibiny, ibinz, 1);
    }
    h2D_nodes[ii]->GetZaxis()->SetRangeUser(0, Nentries+1);
    int Nbinsx = h2D_nodes[ii]->GetNbinsX(); 
    int Nbinsy = h2D_nodes[ii]->GetNbinsY();
    for(int i=0; i<Nbinsx; i++)
    for(int j=0; j<Nbinsy; j++){ 
      if(h2D_nodes[ii]->GetBinContent(i+1, j+1)!=0 ) h2D_nodes[ii]->SetBinContent(i+1, j+1, ii+1);
    }
  }

  for(int i=0; i<Nentries; i++){
    canvas[i]->cd(1);
    h2D_nodes[i]->GetXaxis()->SetTitle("y/mm");
    h2D_nodes[i]->GetYaxis()->SetTitle("x/mm");
    h2D_nodes[i]->Draw("same colz");
    
    truthHit[i]->GetXaxis()->SetTitle("x/mm");
    truthHit[i]->GetYaxis()->SetTitle("y/mm");
    truthHit[i]->GetZaxis()->SetTitle("z/mm");
    truthHit[i]->SetFillColor(i+1);
    canvas[i]->cd(2);
    truthHit[i]->Draw("BOX1 same");
    canvas[i]->Draw();
  }

}
