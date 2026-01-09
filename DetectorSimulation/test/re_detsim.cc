void re_detsim()
{
	TFile* f = new TFile("sample_detsim_user.root");
	TTree* t1 = (TTree*)f->Get("evt");
	vector<int> pmtID;
	t1->SetBranchAddress("pmtID",&pmtID);
	Int_t nentries = (Int_t)t1->GetEntries();
	for (Int_t i=0; i<10; i++) {
		t1->GetEntry(i);
		cout<<"-----------------  Entry  "<<i<<"---------------------"<<endl;
		// const vector<int>::iterator it = pmtID->begin();
		// cout<<(*it)<<end;
		//cout << pmtID << endl;
	}
	if (gROOT->IsBatch()) return;
}
