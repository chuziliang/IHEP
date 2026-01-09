//用于确认作业结束后，输出的root文件是否完整
void vertify()
{
    int energy[] = {1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
    int numbers[] = {100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};
    ofstream output("errorfile.txt");
    for (int i = 0; i < 21; i++)
    {
        bool FirstHIST = true;
        int Entries = 0;
        TFile *rootfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/%1dMeVChargeSpectrum.root",energy[i]));
        for (int filenum = 0; filenum < numbers[i]; filenum++)
        {
            bool FirstError = true;
            TH1 *LPMT = NULL;
            TString LPMTHistName;
            LPMTHistName = TString::Format("LPMT charge spectrun %1dMeV %1dfile",energy[i],filenum);
            LPMT = (TH1*)rootfile->Get(LPMTHistName);
            if (LPMT)
            {
                if (FirstHIST)
                {
                    Entries = LPMT->GetEntries();
                    FirstHIST = false;
                }
                if (!FirstHIST)
                {
                    if (Entries != LPMT->GetEntries())
                    {
                        cout<<"Entries not equal!!!!!"<<endl;
                        
                    }   
                }

                cout<<"LPMT entries "<<LPMT->GetEntries()<<endl;
                LPMT->Delete();
            }
            else
            {
                if (FirstError)
                {
                    output<<energy[i]<<"  "<<filenum<<endl;
                    FirstError = false;
                }
                cout<<LPMTHistName<<" not exist!!!!!"<<endl;
            }
            for (int id = 0; id < 6; id++)
            {
                TH1 *SPMT = NULL;
                TString SPMTHistName;
                SPMTHistName = TString::Format("SPMT %1d charge spectrun %1dMeV %1dfile",id,energy[i],filenum);
                SPMT = (TH1*)rootfile->Get(SPMTHistName);
                if (SPMT)
                {
                    if (FirstHIST)
                    {
                        Entries = SPMT->GetEntries();
                        FirstHIST = false;
                    }
                    if (!FirstHIST)
                    {
                        if (Entries != SPMT->GetEntries())
                        {
                            cout<<"Entries not equal!!!!!"<<endl;
                        }   
                    }
                    cout<<"SPMT "<<id<<" entries "<<SPMT->GetEntries()<<endl;
                    SPMT->Delete();
                }
                else
                {
                    cout<<SPMTHistName<<" not exist!!!!!"<<endl;
                    if (FirstError)
                    {
                        output<<energy[i]<<"  "<<filenum<<endl;
                        FirstError = false;
                    }
                }
                
            }
            cout<<"Energy "<<energy[i]<<"    number "<<filenum<<endl;
            cout<<endl;
        }
        rootfile->Close();
        cout<<"----------------------------------------------------------------------------"<<endl;
        
    }
    
}