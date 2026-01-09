// usage: root -l re_detsim.cc
void re_detsim()
{
	TFile* f = new TFile("sample_detsim_user.root"); // 读取user文件，这里可以换成你自己的文件
	TTree* t1 = (TTree*)f->Get("evt"); // 读取user文件下“evt”这个tree

	// 定义一些要分析的变量
	vector<int>* pmtID=NULL; 
	vector<int>* nPE=NULL;
	vector<int>* PMTID_byPMT=NULL;
	int nPMTs;
	int totalPE;

	// 把上面定义的变量和t1这个tree里的数据联系起来
	t1->SetBranchAddress("pmtID",&pmtID);
	t1->SetBranchAddress("nPE",&nPE);
	t1->SetBranchAddress("nPMTs",&nPMTs);
	t1->SetBranchAddress("totalPE",&totalPE);
	t1->SetBranchAddress("PMTID_byPMT",&PMTID_byPMT);
	Int_t nentries = (Int_t)t1->GetEntries();
	cout<<"+++++++++++++++  Entries = "<<nentries<<"    +++++++++++++++++++++"<<endl;
	
	// 准备画的图
	TH1F *pe = new TH1F("pe","pe",50,0,70);
	TH1F *npmt = new TH1F("npmt","npmt",50,0,70);
	for (Int_t i=0; i < nentries ; i++) {
		t1->GetEntry(i); // 这里使得上面定义的变量的值变成第i个entry的值
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
		pe->Fill(nPE_SPMT); // 向hist图里填数据
		npmt->Fill(nSPMT);
		pe->SetLineColor(kRed); // 改变图的颜色
		pe->Draw(); // 画图
		npmt->Draw("same"); // “same”使得两张图画在一张面板上
	}
	if (gROOT->IsBatch()) return;
}
