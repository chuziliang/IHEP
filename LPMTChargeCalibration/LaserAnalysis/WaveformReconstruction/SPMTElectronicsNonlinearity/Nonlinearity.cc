double MeanRatio = 1;


void Nonlinearity()
{
    int energy[]={1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
    int numbers[]={100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};
    TH2 *output = new TH2F("Calib charge to true charge","Calib charge to true charge",200,0,200,2000,0,200);
    TH1 *CalibHist[20];
    for (int nPE = 0; nPE < 20; nPE++)
    {
        TString histname = TString::Format("%1d PE calib charge",nPE);
        CalibHist[nPE] = new TH1F(histname,histname,1000,0,21.93); 
    }
    
    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/SPMTElectronicsNonlinearity/%1dMevNonlinearity.root",energy[i]),"read");
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double TrueChargeSPMT;
            double CalibChargeSPMT; 
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("TrueCharge",&TrueChargeSPMT);
            charge->SetBranchAddress("CalibCharge",&CalibChargeSPMT);
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                output->Fill(TrueChargeSPMT,CalibChargeSPMT);
                CalibHist[(int)(TrueChargeSPMT + 0.5)]->Fill(CalibChargeSPMT);
            }
            delete charge;
        }

        inputfile->Close();

        cout<<i<<" Finished."<<endl;
    }
    double MeanNonlinearity[20];
    double ErrorNonlinearity[20];
    double trueCharge[20];

    double RMS[20];
    double ErrorRMS[20];
    int n = 0;
    for (int nPE = 1; nPE < 20; nPE++)
    {
        double entries = CalibHist[nPE]->GetEntries();
        if (entries > 1)
        {
            double Mean = (CalibHist[nPE]->GetMean())/MeanRatio;
            double MeanError = (CalibHist[nPE]->GetMeanError())/MeanRatio;
            MeanNonlinearity[n] = ( Mean - (double)nPE)/(double)nPE;
            trueCharge[n] = nPE;
            ErrorNonlinearity[n] = (sqrt(entries)/sqrt(entries - 1))*MeanError;
            ErrorNonlinearity[n] /= (double)nPE;

            RMS[n] = CalibHist[nPE]->GetRMS()/CalibHist[nPE]->GetMean();
            ErrorRMS[n] = RMS[n]*sqrt((CalibHist[nPE]->GetRMSError()*CalibHist[nPE]->GetRMSError())/(CalibHist[nPE]->GetRMS()*CalibHist[nPE]->GetRMS()) + (CalibHist[nPE]->GetMeanError()*CalibHist[nPE]->GetMeanError())/(CalibHist[nPE]->GetMean()*CalibHist[nPE]->GetMean()));
            n++;
        }
        
    }
    TCanvas *c1 = new TCanvas("c1","c1",1);
    TGraphErrors *NonlinearityGraph = new TGraphErrors(n,trueCharge,MeanNonlinearity,nullptr,ErrorNonlinearity);
    NonlinearityGraph->SetTitle("Nonlinearity");
    NonlinearityGraph->SetName("Nonlinearity");
    NonlinearityGraph->GetXaxis()->SetTitle("True charge(PE)");
    NonlinearityGraph->GetYaxis()->SetTitle("(WFrec charge - True charge)/True charge");
    NonlinearityGraph->SetFillColor(4);
    NonlinearityGraph->SetFillStyle(3002);
    NonlinearityGraph->SetMarkerColor(4);
    NonlinearityGraph->SetMarkerSize(0.8);
    NonlinearityGraph->SetMarkerStyle(20);
    NonlinearityGraph->Draw("a3");
    NonlinearityGraph->Draw("same L X P");

    TCanvas *c2 = new TCanvas("c2","c2",1);
    TGraphErrors *StdDEV = new TGraphErrors(n,trueCharge,RMS,nullptr,ErrorRMS);
    StdDEV->SetTitle("Resolution");
    StdDEV->SetName("Resolution");
    StdDEV->GetXaxis()->SetTitle("True charge(PE)");
    StdDEV->GetYaxis()->SetTitle("Resolution");
    StdDEV->SetFillColor(4);
    StdDEV->SetFillStyle(3002);
    StdDEV->SetMarkerColor(4);
    StdDEV->SetMarkerSize(0.8);
    StdDEV->SetMarkerStyle(20);
    StdDEV->Draw("a3");
    StdDEV->Draw("same L X P");
    TFile *outputfile = TFile::Open("Nonlinerity.root","RECREATE");
    for (int nPE = 0; nPE < 20; nPE++)
    {
        CalibHist[nPE]->Write();
    }
    
    output->Write();
    NonlinearityGraph->Write();
    StdDEV->Write();
}