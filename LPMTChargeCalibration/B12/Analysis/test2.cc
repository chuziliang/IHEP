void test2()
{
    int num;
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/B12/Analysis/test.root","read");
    TTree *t;
    for (int i = 0; i < 2; i++)
    {
        t = (TTree*)inputfile->Get(TString::Format("t%1d",i));
        t->SetBranchAddress("num/QE",&num);
        for (int i = 0; i < 10; i++)
        {
            t->GetEntry(i);
            cout<<num<<endl;
        }
    }

}