//这个脚本用于将不同root文件下的数据合并到一个root文件中。
const double Degree[6]={4.9,9.9,19.7,29.6,39.5,44.4};

void CombineRootFile(int EnergyNum,int DegreeNum)
{
    int energy[] = {1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
    int numbers[] = {100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};
    TFile *outputfile = new TFile(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/CombineChargeSpectrum/ChargeSpectrum%.1fDEG.root",Degree[DegreeNum]),"update");
    //for (int i = 0; i < 21; i++)
    {
        TFile *rootfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/OffAxis/TrueNonlinearity/ChargeSpectrum/%1dMeVChargeSpectrum.root",energy[EnergyNum]),"read");
        if (!rootfile)
        {
            cout<<"Failed to open root file"<<endl;
        }
        
        TH1F *LPMTTotal = NULL;
        TString LPMTTotalHistName;
        LPMTTotalHistName = TString::Format("LPMT nPE spectrun %1dMeV %.1fDEG",energy[EnergyNum],Degree[DegreeNum]);
        LPMTTotal = new TH1F(LPMTTotalHistName,LPMTTotalHistName,1000,0,1000);
        TH1F *SPMTTotal[6];
        for (int id = 0; id < 6; id++)
        {
            TString SPMTTotalHistName;
            SPMTTotalHistName = TString::Format("SPMT_%1d nPE spectrun %1dMeV %.1fDEG",id,energy[EnergyNum],Degree[DegreeNum]);
            SPMTTotal[id] = new TH1F(SPMTTotalHistName,SPMTTotalHistName,300,0,300);
        }
        for (int filenum = 0; filenum < numbers[EnergyNum]; filenum++)
        {
            TH1F *LPMT = NULL;
            TString LPMTHistName;
            LPMTHistName = TString::Format("LPMT charge spectrun %1dMeV %1dfile %.1fDEG",energy[EnergyNum],filenum,Degree[DegreeNum]);
            LPMT = (TH1F*)rootfile->Get(LPMTHistName);
            if (!LPMT)
            {
                cout<<"Failed to load LPMT charge spectrum"<<endl;
				cout<<LPMTHistName<<endl;
            }
            
            LPMTTotal->Add(LPMT);
            LPMT->Delete();
            for (int id = 0; id < 6; id++)
            {
                TH1F *SPMT = NULL;
                TString SPMTHistName;
                SPMTHistName = TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile %.1fDEG",id,energy[EnergyNum],filenum,Degree[DegreeNum]);
                SPMT = (TH1F*)rootfile->Get(SPMTHistName); 
                if (!SPMT)
                {
                    cout<<"Failed to load SPMT charge spectrum"<<endl;
                }
                
                SPMTTotal[id]->Add(SPMT);
                SPMT->Delete();
            }
        }

        outputfile->cd();
        LPMTTotal->Write();
        LPMTTotal->Delete();
        
        for (int id = 0; id < 6; id++)
        {
            outputfile->cd();
            SPMTTotal[id]->Write();
            SPMTTotal[id]->Delete();
        }
        rootfile->Close();
    }
    outputfile->Close();
}
