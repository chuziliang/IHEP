// 用于计算小于0.3PE的比例
void GetSPE()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/SPMTElectronicsNonlinearity/Nonlinerity.root","read");
    TH1 *SPEHist = (TH1*)inputfile->Get("1 PE calib charge");
    int eventSmall = 0;
    int TotalEvent = SPEHist->GetEntries();
    int eventSmall_not0 = 0;
    for (int i = 0; SPEHist->GetBinCenter(i) < 0.3; i++)
    {
        eventSmall += SPEHist->GetBinContent(i);
        cout<<i<<endl;
    }
    eventSmall_not0 = eventSmall - SPEHist->GetBinContent(1);
    double rate = (double)eventSmall/TotalEvent;
    double rate_not0 = (double)eventSmall_not0/TotalEvent;
    cout<<rate<<endl;
    cout<<rate_not0<<endl;
    inputfile->Close();
}