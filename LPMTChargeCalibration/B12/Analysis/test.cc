void test()
{
    int num;
    TTree *t[2];
    for (int i = 0; i < 2; i++)
    {
        t[i] = new TTree(TString::Format("t%1d",i),TString::Format("t%1d",i));
        t[i]->Branch("num/QE",&num,"num_QE/I");
    }
    TFile *outputfile = new TFile("test.root","recreate");
    for (int i = 0; i < 100; i++)
    {
        num = i;
        if (num>50)
        {
            t[0]->Fill();
        }
        else
        {
            t[1]->Fill();
        }
    }
    for (int i = 0; i < 2; i++)
    {
        t[i]->Write();
    }
    outputfile->Close();
}