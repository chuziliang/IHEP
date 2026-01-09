void readS()
{
    TFile *file = TFile::Open("Smomentum.root");
    TTree *gamtree = (TTree*)file->Get("Gamma");
    TTree *pitree = (TTree*)file->Get("Pion");

    //gamma
    double t_x_zbar, t_x_ybar, t_Sz, t_Sy;
    double second_momentum_z[14]={0};   // second momentum for each verti bar layer
    double second_momentum_y[14]={0};   // second momentum for each horiz bar layer
    double x_zbar[14]={0};      // x position of verti bar in each layer
    double x_ybar[14]={0};
    gamtree->SetBranchAddress("t_x_zbar", &t_x_zbar);
    gamtree->SetBranchAddress("t_x_ybar",  &t_x_ybar);
    gamtree->SetBranchAddress("t_Sz",  &t_Sz);
    gamtree->SetBranchAddress("t_Sy",  &t_Sy);
    for(int i = 0; i < 14; ++i)
    {
        gamtree->GetEntry(i);
        x_zbar[i]=t_x_zbar;
        x_ybar[i]=t_x_ybar;
        second_momentum_z[i]=t_Sz;
        second_momentum_y[i]=t_Sy;
    }

    //pion
    double pt_x_zbar, pt_x_ybar, pt_Sz, pt_Sy;
    double psecond_momentum_z[14]={0};   // second momentum for each verti bar layer
    double psecond_momentum_y[14]={0};   // second momentum for each horiz bar layer
    double px_zbar[14]={0};      // x position of verti bar in each layer
    double px_ybar[14]={0};
    pitree->SetBranchAddress("t_x_zbar", &pt_x_zbar);
    pitree->SetBranchAddress("t_x_ybar",  &pt_x_ybar);
    pitree->SetBranchAddress("t_Sz",  &pt_Sz);
    pitree->SetBranchAddress("t_Sy",  &pt_Sy);
    for(int i = 0; i < 14; ++i)
    {
        pitree->GetEntry(i);
        px_zbar[i]=pt_x_zbar;
        px_ybar[i]=pt_x_ybar;
        psecond_momentum_z[i]=pt_Sz;
        psecond_momentum_y[i]=pt_Sy;
    }

    

    TCanvas* cSnd_Mmt = new TCanvas("cSnd_Mmt","Second Momentum",1000,500);

    TGraph* grSz = new TGraph(14,x_zbar,second_momentum_z);
    TGraph* grSy = new TGraph(14,x_ybar,second_momentum_y);
    TGraph* pgrSz = new TGraph(14,px_zbar,psecond_momentum_z);
    TGraph* pgrSy = new TGraph(14,px_ybar,psecond_momentum_y);

    grSz->GetXaxis()->SetTitle("layer");
    grSy->GetXaxis()->SetTitle("layer");
    pgrSz->GetXaxis()->SetTitle("layer");
    pgrSy->GetXaxis()->SetTitle("layer");

    grSz->GetYaxis()->SetTitle("S");
    grSy->GetYaxis()->SetTitle("S");
    pgrSz->GetYaxis()->SetTitle("S");
    pgrSy->GetYaxis()->SetTitle("S");

    grSz->SetLineColor(1);  // black
    grSz->SetMarkerStyle(20);   // full circle
    grSy->SetLineColor(2);  // red
    grSy->SetMarkerStyle(21);   // full square

    pgrSz->SetLineColor(3);  // grean
    pgrSz->SetMarkerStyle(20);   // circle
    pgrSz->SetLineWidth(2);
    pgrSy->SetLineColor(4);  // blue
    pgrSy->SetMarkerStyle(21);   // square
    pgrSy->SetLineWidth(2);

    grSz->GetHistogram()->SetMaximum(2000.);
    grSz->GetHistogram()->SetMinimum(0.);

    grSz->Draw("ACP");
    grSy->Draw("CP");
    pgrSz->Draw("CP");
    pgrSy->Draw("CP");

    TLegend *legend1 = new TLegend(0.6,0.6,0.9,0.9);
    legend1->AddEntry(grSz,"gamma verticle layer");
    legend1->AddEntry(grSy,"gamma horizontal layer");
    legend1->AddEntry(pgrSz,"pion verticle layer");
    legend1->AddEntry(pgrSy,"pion horizontal layer");
    legend1->SetBorderSize(0);
    legend1->Draw();

}