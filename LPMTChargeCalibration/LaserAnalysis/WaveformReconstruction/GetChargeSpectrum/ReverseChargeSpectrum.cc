using namespace std;
using namespace JM;

int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

void ReverseChargeSpectrum(int Eid)
{
    double Threshold = 170.0*energy[Eid];
    TFile* inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut_WithTotalPE_AllSPMT_AllLPMT/%1dMevCharge.root",energy[Eid]),"read");
    TFile* outputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut_WithTotalPE_AllSPMT_AllLPMT_reverse/%1dMevCharge.root",energy[Eid]),"recreate");

    for (int filenum = 0; filenum<numbers[Eid]; filenum++)
    {
        inputfile->cd();
        TTree* charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",filenum));
        vector<double>* TrueChargeLPMT = nullptr;
        vector<double>* CalibChargeLPMT = nullptr;
        vector<double>* SPMTPE = nullptr;
        double TotalChargeLPMT = 0;
        charge->SetBranchAddress("SPMTCharge",&SPMTPE);
        charge->SetBranchAddress("TrueChargeLPMT",&TrueChargeLPMT);
        charge->SetBranchAddress("CalibChargeLPMT",&CalibChargeLPMT);
        charge->SetBranchAddress("TotalChargeLPMT",&TotalChargeLPMT);

        outputfile->cd();
        TTree* outputcharge = new TTree(TString::Format("Charge%1d",filenum),"DetSim charge and calib charge");
        vector<double> outputTrueChargeLPMT;
        vector<double> outputCalibChargeLPMT;
        vector<double> outputSPMTPE;
        double outputTotalChargeLPMT = 0;
        outputcharge->Branch("SPMTCharge",&outputSPMTPE);
        outputcharge->Branch("TrueChargeLPMT",&outputTrueChargeLPMT);
        outputcharge->Branch("CalibChargeLPMT",&outputCalibChargeLPMT);
        outputcharge->Branch("TotalChargeLPMT",&outputTotalChargeLPMT,"TotalChargeLPMT/D");

        inputfile->cd();
        for (int i = 0; i < charge->GetEntries(); i++ )
        {
            charge->GetEntry(i);
            outputTotalChargeLPMT = 0;
            for (int LPMTID  = 0; LPMTID<17612; LPMTID++)
            {
                outputTotalChargeLPMT += (*TrueChargeLPMT)[LPMTID];
            }

            if (outputTotalChargeLPMT > Threshold)
            {
                outputTrueChargeLPMT = *TrueChargeLPMT;
                outputCalibChargeLPMT= *CalibChargeLPMT;
                outputSPMTPE = *SPMTPE;

                outputcharge->Fill();
            }
        }
        outputfile->cd();
        outputcharge->Write();
        delete outputcharge;
        delete charge;
        cout<<"filenum "<<filenum<<" Finished"<<endl;
    }
}