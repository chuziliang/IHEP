void re_detsim()
{
	TFile* f = new TFile("sample_detsim_user.root");
	TTree* t1 = (TTree*)f->Get("evt");
	vector<int>* pmtID=NULL;
	vector<int>* nPE=NULL;
	vector<int>* PMTID_byPMT=NULL;
	int nPMTs;
	int totalPE;
	t1->SetBranchAddress("pmtID",&pmtID);
	t1->SetBranchAddress("nPE",&nPE);
	t1->SetBranchAddress("nPMTs",&nPMTs);
	t1->SetBranchAddress("totalPE",&totalPE);
	t1->SetBranchAddress("PMTID_byPMT",&PMTID_byPMT);
	Int_t nentries = (Int_t)t1->GetEntries();
	cout<<"+++++++++++++++  Entries = "<<nentries<<"    +++++++++++++++++++++"<<endl;
	TH1F *pe = new TH1F("pe","pe",50,0,70);
	TH1F *npmt = new TH1F("npmt","npmt",50,0,70);
	for (Int_t i=0; i < nentries ; i++) {
		t1->GetEntry(i);
		Int_t nPE_SPMT=0;
		Int_t nSPMT=0;
		for (Int_t j=0; j<totalPE;j++){
			if ( pmtID->at(j) > 200000)
			{
				nPE_SPMT++;
			}
		}
		for (Int_t j=0; j<nPMTs;j++){
			if ( PMTID_byPMT->at(j) > 200000)
			{
				nSPMT++;
			}
		}
		pe->Fill(nPE_SPMT);
		npmt->Fill(nSPMT);
		pe->SetLineColor(kRed);
		pe->Draw();
		npmt->Draw("same");
	}
	if (gROOT->IsBatch()) return;
}
