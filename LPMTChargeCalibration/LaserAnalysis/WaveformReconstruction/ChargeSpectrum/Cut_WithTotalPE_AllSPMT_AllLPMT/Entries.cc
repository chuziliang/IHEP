int energy[]={1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
int numbers[]={100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};
void Entries()
{
    for (int Energy = 0; Energy < 21; Energy++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut_WithTotalPE_AllSPMT_AllLPMT/%1dMevCharge.root",energy[Energy]));
        cout<<"Energy = "<<energy[Energy]<<endl;
        for (int j = 0; j < numbers[Energy]; j++) // 每个能量下的文件
        {
            TTree *SPMT_charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            cout<<SPMT_charge->GetEntries()<<endl;
            delete SPMT_charge;
        }
        inputfile->Close();
        cout<<"------------------------------------------"<<endl;
    }
}
