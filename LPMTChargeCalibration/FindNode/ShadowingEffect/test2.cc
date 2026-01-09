void test2()
{
   TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/FindNode/ShadowingEffect/ChargeSpectrum.root","read");
    TTree *charge = (TTree*)inputfile->Get("charge");
    vector<double> *LPMTCharge; 
    vector<double> *SPMTCharge;
    double edep_x, edep_y, edep_z;
    double edepEnergy;
    charge->SetBranchAddress("TrueLPMTCharge",&LPMTCharge);
    charge->SetBranchAddress("TrueSPMTCharge",&SPMTCharge);
    charge->SetBranchAddress("edep_x",&edep_x);
    charge->SetBranchAddress("edep_y",&edep_y);
    charge->SetBranchAddress("edep_z",&edep_z);
    charge->SetBranchAddress("edep",&edepEnergy);

    TH1 *LPMTHist = new TH1F("LPMT","LPMT",3000,0,3000);
    TH1 *SPMTHist = new TH1F("SPMT","SPMT",3000,0,3000);

    for (int entry = 4; entry<5; entry++)
    {
        charge->GetEntry(entry);
        int TotalPELPMT = 0;
        int TotalPESPMT = 0;
        for (int LPMTID = 0; LPMTID<(*LPMTCharge).size(); LPMTID++)
        {
            TotalPELPMT += (*LPMTCharge)[LPMTID];
            LPMTHist->Fill((*LPMTCharge)[LPMTID]);
        }

        for (int SPMTID = 0; SPMTID<(*SPMTCharge).size(); SPMTID++)
        {
            TotalPESPMT += (*SPMTCharge)[SPMTID];
            SPMTHist->Fill((*SPMTCharge)[SPMTID]);
        }

        cout<<"TotalLPMT = "<<TotalPELPMT<<endl;
        cout<<"TotalSPMT = "<<TotalPESPMT<<endl;
    }

    TCanvas *c1 = new TCanvas("c1","c1",1);
    LPMTHist->Draw();
    TCanvas *c2 = new TCanvas("c2","c2",1);
    SPMTHist->Draw();
}