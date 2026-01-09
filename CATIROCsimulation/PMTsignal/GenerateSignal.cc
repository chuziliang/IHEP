void GenerateSignal()
{
    TFile* f = TFile::Open("joint.root");
    TH2 *joint = nullptr; 
    f->GetObject("joint",joint);
    TH2* t = new TH2D("t","t",100,10,50,100,10,50);
    TH1F* sigma = new TH1F("sigma","sigma",100,0,10);
    TH1* tmp = new TH1F("tmp"," ", 1008, 0, 1008);
    TF1* func = new TF1("SpecModel", "landau",0 , 1008);
    // int Maximum = h->GetMaximum();
    // TRandom3 r;
    // double x = 17.0*r.Rndm()+3.0;
    // double y = 5.0*r.Rndm()+2.0;
    // double z = Maximum*r.Rndm();
    // if (z<h->GetBinContent());
    Double_t x;
    Double_t y;
    for ( Int_t i = 0; i<10000; i++)
    {
        double charge = 0;
        joint->GetRandom2(x,y);
        //sigma->Fill(y); 
        func->SetParameters(x,463.6,y);
        func->SetNpx(1008);
        tmp = func->CreateHistogram();
        for (int k = tmp->GetMaximumBin()-30 ; k < tmp->GetMaximumBin()+70; k++)
        {
            charge += tmp->GetBinContent(k) * 1.0;
            //cout<<tmp->GetBinContent(k)<<endl;
        }
        //charge = func->Integral(0,1008);
        t->Fill(x*y,charge);
    }
   
    TCanvas *c1 = new TCanvas("c1","c1",800,1000);
    c1->Divide(1,3);
    c1->cd(1);
    joint->Draw();
    c1->cd(2);
    t->Draw("COL");
    c1->cd(3);
    tmp->Draw();
    c1->cd();
}