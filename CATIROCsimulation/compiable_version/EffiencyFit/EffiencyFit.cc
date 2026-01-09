void EffiencyFit()
{
    TFile *TemplateFile = TFile::Open("Template.root");
    TFile *SSHspectrumFile = TFile::Open("SSHspectrum.root");

    ofstream Chi("pChi2Martrix.txt");
    ofstream AD("pADMartrix.txt");
    ofstream Kol("pKolMartrix.txt");
    TH1 *Template[100];
    TH1 *SSHspectrum[20];

    const int ns = 20;
    double trigger[ns];
    for (int i = 0; i < ns; i++)
    {
        trigger[i] = 500 + (990.0-500.0)*(double)(i+1)/ns;
    }

    const int nt = 100;
    double quantiles[nt];
    for (int i = 0; i < nt; i++)
    {
        quantiles[i] = (double)(i+1)/nt;
    }

    for (int i = 0; i < 100; i++)
    {
        Template[i] = (TH1*)TemplateFile->Get(TString::Format("%2.0f%% Spectrum",100*quantiles[i]));
    }
    for (int i = 0; i < 20; i++)
    {
        SSHspectrum[i] = (TH1*)SSHspectrumFile->Get(TString::Format("Trigger = %2.0fDACu",trigger[i]));
    }

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            double pChi2;
            double pAD;
            double pKol;

            pChi2 = SSHspectrum[i]->Chi2Test(Template[j]);
            pAD = SSHspectrum[i]->AndersonDarlingTest(Template[j]);
            pKol = SSHspectrum[i]->KolmogorovTest(Template[j]);

            Chi<<pChi2<<" ";
            AD<<pAD<<" ";
            Kol<<pKol<<" ";
        }
        Chi<<endl;
        AD<<endl;
        Kol<<endl;
        
    }
    
}