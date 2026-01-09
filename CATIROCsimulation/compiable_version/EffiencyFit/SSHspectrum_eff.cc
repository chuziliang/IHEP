void SSHspectrum_eff()
{
    TFile *SSHspectrumFile = TFile::Open("SSHspectrum.root");

    ofstream eff("SSHspectrum_eff.txt",ios::trunc);

    TH1 *SSHspectrum[20];

    const int ns = 20;
    double trigger[ns];
    for (int i = 0; i < ns; i++)
    {
        trigger[i] = 500 + (990.0-500.0)*(double)(i+1)/ns;
    }

    for (int i = 0; i < 20; i++)
    {
        double effiency;
        SSHspectrum[i] = (TH1*)SSHspectrumFile->Get(TString::Format("Trigger = %2.0fDACu",trigger[i]));
        effiency = (SSHspectrum[i]->GetEntries())/200000.0;
        eff<<effiency<<endl;

    }
    
}